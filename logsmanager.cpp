#include "logsmanager.h"
#include <algorithm>


// debugin path format issues for time rotation schadule

logsmanager::logsmanager():thpool(4){

    std::cout<<"calling reset on pointers"<<std::endl;
    WARN.reset(fopen(definefromconfig("warn_log:"),"a"),[](FILE * F){if(F) fclose(F);});
    ERROR.reset(fopen(definefromconfig("error_log:"),"a"),[](FILE * F){if(F) fclose(F);});
    DEBUG.reset(fopen(definefromconfig("debug_log:"),"a"),[](FILE * F){if(F) fclose(F);});
    INFO.reset(fopen(definefromconfig("info_log:"),"a"),[](FILE * F){if(F) fclose(F);});

    checkFiles();
    
}


void logsmanager::addlog(logmsg log){
    
    thpool.enqueue([this,log](){
        if(log.priority=="INFO"){

            std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            std::lock_guard<std::mutex> lock(INFO_mutex);
            fputs(fullmsg.c_str(),INFO.get());
        
        }else if(log.priority=="WARN"){
            std::lock_guard<std::mutex> lock(WARN_mutex);

            std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            fputs(fullmsg.c_str(),WARN.get());

        }else if(log.priority=="DEBUG"){
            std::lock_guard<std::mutex> lock(DEBUG_mutex);

             std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            fputs(fullmsg.c_str(),DEBUG.get());

        }else{
            std::lock_guard<std::mutex> lock(ERROR_mutex);

             std::string fullmsg{"["+std::string(log.timestamp)+"] from"+log.address+":"+
            std::to_string(log.logsport)+"-"+"priority"+log.priority+" message"": "+
            log.msgbody+"\n"};

            fputs(fullmsg.c_str(),ERROR.get());
        }
    });
};


logsmanager::~logsmanager(){

    fflush(WARN.get());
    fflush(ERROR.get());
    fflush(DEBUG.get());
    fflush(INFO.get());

};


const char * logsmanager::definefromconfig(std::string field){
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
    
    auto comp=[field](std::string element){
        return field==element;
    };

    auto it=find_if(contents.begin(),contents.end(),comp);
    long int indx{(it-contents.begin())+1};
    
    //need not found error
    /*std::cout<<"finding "<<field<<" in items:"<<std::endl;
    for(std::string str:contents){
        std::cout<<str<<"|space|";
    }*/
    //std::cout<<"founded index"<<indx<<std::endl;
    
    if(field=="rotate_clock:"){
        return contents[indx].c_str();
    }
    
    std::string temp{"../"+contents[indx]};
    return temp.c_str();
};


void logsmanager::editconfig(std::string field ,std::string value){
std::fstream sourcefile("config.txt",std::ios::in | std::ios::out);
if(!sourcefile.is_open()){
    std::cerr<<"error"<<std::endl;
}else{
    std::string line{""};
    long pos = sourcefile.tellg();

    while(std::getline(sourcefile,line)){
        if(line.find("field")){
            sourcefile.seekp(pos);
            sourcefile<<field<<": "<<value<<std::endl;
            //std::cout<<"configuration changed successfully"<<std::endl;
            break;
        }
        pos=sourcefile.tellg();
    }

}

};


void logsmanager::rotate_all(){
   
   {
       std::lock_guard lock(WARN_mutex);
       std::string newfilename{createlogname("warn")};
       editconfig("warn_log:",newfilename);
       WARN.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   };
   {
       std::lock_guard lock(DEBUG_mutex);
             std::string newfilename{createlogname("debug")};
       editconfig("debug_log:",newfilename);
       DEBUG.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   }; 
     {
       std::lock_guard lock(ERROR_mutex);
       std::string newfilename{createlogname("error")};
       editconfig("error_log:",newfilename);
       ERROR.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   };   
   {
       std::lock_guard lock(INFO_mutex);
       std::string newfilename{createlogname("info")};
       editconfig("info_log:",newfilename);
       INFO.reset(fopen(newfilename.c_str(),"a"),[](FILE * f){if(f) fclose(f);});
   }
}


std::string logsmanager::createlogname(std::string category){
    std::time_t now=std::time(nullptr);
    struct tm *t =localtime(&now);
    char buffer[16];

    strftime(buffer,sizeof(buffer),"%M-%d_%H:%M:%S",t);

    return category+"_"+buffer+".txt";

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
    std::cout<<"doing the job"<<std::endl;
    return std::chrono::seconds(std::stoi( definefromconfig("rotate_count:")));
}

};   

