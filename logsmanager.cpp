#include "logsmanager.h"
#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>




logsmanager::logsmanager():thpool(4),stmanager(statsmanager)
{

    struct stat sb;
    stmanager.startserver();

    if(stat("../logs",&sb)!=0){
        if(mkdir("../logs",0700)!=0){
            std::cerr<<"there where no logs folder == logs folder creation failed\n";
        }
    }
    
    WARN.reset(fopen(definefromconfig("warn_log:").c_str(),"a"),
    [](FILE * F){if(F) fclose(F);});
    ERROR.reset(fopen(definefromconfig("error_log:").c_str(),"a"),
    [](FILE * F){if(F) fclose(F);});//"../logs/error_34-17_00:34:27.log","a"
    DEBUG.reset(fopen(definefromconfig("debug_log:").c_str(),"a"),
    [](FILE * F){if(F) fclose(F);});
    INFO.reset(fopen(definefromconfig("info_log:").c_str(),"a"),
    [](FILE * F){if(F) fclose(F);});
    checkFiles();

    statusserverprocess=std::thread(&statusmanager::listenserver,&stmanager);
    
    thpool.dequeuedaction=[this](){ statsmanager.update_queue_size(
        thpool.getqueuecount());

};
    
}


void logsmanager::addlog(logmsg log){
    statsmanager.increase_recieved();

    thpool.enqueue([this,log](){
        if(log.priority=="INFO"){

            std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            
            std::lock_guard<std::mutex> lock(INFO_mutex);
            
            fputs(fullmsg.c_str(),INFO.get());
            fflush(INFO.get());
        }else if(log.priority=="WARN"){
            std::lock_guard<std::mutex> lock(WARN_mutex);

            std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            fputs(fullmsg.c_str(),WARN.get());
            fflush(WARN.get());
        }else if(log.priority=="DEBUG"){
            std::lock_guard<std::mutex> lock(DEBUG_mutex);

            std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            fputs(fullmsg.c_str(),DEBUG.get());
            fflush(DEBUG.get());
        }else{
            std::cout<<"wrting to error log\n";

            std::lock_guard<std::mutex> lock(ERROR_mutex);

            std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            fputs(fullmsg.c_str(),ERROR.get());
            fflush(ERROR.get());
        }
        
        statsmanager.increase_written(log.priority);
    });
    if(thpool.getqueuecount()>threshold){
        thpool.dropqueuetask();
    }
    statsmanager.update_queue_size(thpool.getqueuecount());
};


logsmanager::~logsmanager(){

    fflush(WARN.get());
    fflush(ERROR.get());
    fflush(DEBUG.get());
    fflush(INFO.get());
    
     if(statusserverprocess.joinable()){
        statusserverprocess.join();
    }
};


std::string  logsmanager::definefromconfig(std::string field){
    std::ifstream source("../config.txt");
    if(!source.is_open()){
        std::cerr<<"failed to open config file"<<std::endl;
        return 0;
    }
    
    std::string word{""};
    std::vector<std::string> contents;

    while( source>>word ){
        contents.push_back(word);
    }
   
    auto comp=[ field](std::string element){
        return field==element;
    };

    auto it=find_if(contents.begin(),contents.end(),comp);
    if(it==contents.end()) std::cerr<<"failed to define from configfile\n";
    
    long int indx{(it-contents.begin())+1};
    //std::cout<<"result for "<<field<<" in definefromconfig:"<<contents[indx]<<std::endl;
    
    
    /*if(field=="rotate_clock:" || field=="rotate_count:"){
        return contents[indx];
    }
    
    const std::string temp{"../"+contents[indx]};
    return temp;*/
    return contents[indx];
};


void logsmanager::editconfig(std::string field ,std::string value){
std::lock_guard lock(cfgfilemutex);
std::ifstream sourcefile("../config.txt");

if(!sourcefile.is_open()){
    std::cerr<<"failed to open config file"<<std::endl;
}else{
    
   std::stringstream buffer{};
   buffer<<sourcefile.rdbuf();
   std::string content{buffer.str()};

   size_t start{content.find(field)+field.length()+1},end;
        if(start !=std::string::npos){
            end=(content.find_first_of("\n\v\t\r\f", start));
            if(end!=std::string::npos){
                //std::cout<<"replacing text:"<<value<<std::endl;
                content.replace(start, end-start, value);
                std::ofstream outfile("../config.txt");
                outfile<<content;
                }else{
                    std::cerr<<"undefined value for field in line\n";
                }
        }else{
            std::cerr<<"undefined field value part \n";
        }
}

};


void logsmanager::rotate_all(){
   
   {
       std::lock_guard lock(WARN_mutex);
       std::string newfilename{"../logs/"+createlogname("warn")};
       std::cout<<"creating new file name:"<<newfilename<<std::endl;
       editconfig("warn_log:",newfilename);
       WARN.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   };
   {
       std::lock_guard lock(DEBUG_mutex);
             std::string newfilename{"../logs/"+createlogname("debug")};
       editconfig("debug_log:",newfilename);
       DEBUG.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   }; 
     {
       std::lock_guard lock(ERROR_mutex);
       std::string newfilename{"../logs/"+createlogname("error")};
       editconfig("error_log:",newfilename);
       ERROR.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   };   
   {
       std::lock_guard lock(INFO_mutex);
       std::string newfilename{"../logs/"+createlogname("info")};
       editconfig("info_log:",newfilename);
       INFO.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   }
}


std::string logsmanager::createlogname(std::string category){
    std::time_t now= std::time(nullptr);
    struct tm *t = localtime(&now);
    char buffer[16];

    strftime(buffer,sizeof(buffer),"%M-%d_%H:%M:%S",t);

    return category+"_"+buffer+".log";

};


void logsmanager::checkFiles(){
    if(!WARN.get()){
        std::cerr<<"there is trouble with oponing warnings file "<<std::endl;
    }else if(!DEBUG.get()){
        std::cerr<<"there is trouble with oponing debugs file "<<std::endl;
    }else if(!INFO.get()){
        std::cerr<<"there is trouble with opening info file "<<std::endl;
    }else if(!ERROR.get()){
        std::cerr<<"there is trouble with opening errors file "<<std::endl;
    }
    
};


std::chrono::seconds logsmanager::getinterval(){
    std::string format{definefromconfig("rotate_clock:")};

if(format=="minute"){
    
    return std::chrono::minutes( std::stoi( definefromconfig("rotate_count:")));
}else if(format=="hour"){
    return std::chrono::hours(std::stoi( definefromconfig("rotate_count:")));
}else{
    std::cout<<"get interval is exectuing inside else condition"<<std::endl;
    return std::chrono::seconds(std::stoi( definefromconfig("rotate_count:")));
}

};   
