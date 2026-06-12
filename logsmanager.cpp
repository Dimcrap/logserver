#include "logsmanager.h"




logsmanager::logsmanager():thpool(4){
    WARN.reset(fopen(definefromconfig("warn_log"),"a"),[](FILE * F){if(F) fclose(F);});
    ERROR.reset(fopen(definefromconfig("error_log"),"a"),[](FILE * F){if(F) fclose(F);});
    DEBUG.reset(fopen(definefromconfig("debug_log"),"a"),[](FILE * F){if(F) fclose(F);});
    INFO.reset(fopen(definefromconfig("info_log"),"a"),[](FILE * F){if(F) fclose(F);});

    //check

}


void logsmanager::addlog(logmsg log){
    
    thpool.enqueue([this,log](){
        if(log.priority=="INFO"){
            std::lock_guard<std::mutex> lock(INFO_mutex);
            
            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;

            files.at(log.priority).flush();

        }else if(log.priority=="WARN"){
            std::lock_guard<std::mutex> lock(WARN_mutex);

            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;

            files.at(log.priority).flush();
        }else if(log.priority=="DEBUG"){
            std::lock_guard<std::mutex> lock(DEBUG_mutex);

            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;

            files.at(log.priority).flush();
        }else{
            std::lock_guard<std::mutex> lock(ERROR_mutex);

            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;

            files.at(log.priority).flush();
        }

    });
};


logsmanager::~logsmanager(){

    for(auto i = files.begin();i != files.end(); i++ ){
        i->second.flush();
        i->second.close();
    }

};

char * definefromconfig(std::string field){
    std::ifstream source("config");
    if(!source.is_open()){
        std::cerr<<"failed to open config file"<<std::endl;
        return 0;
    }

    std::string word{""};
    std::vector<std::string> contents;

    while(getline(source,word,' ')){
        contents.push_back(word);
    }
    auto it=find(contents.begin(),contents.end(),field);
    int indx{(it-contents.begin())+1}

    //create a char * of contents[indx]
    
};

void editconfig(std::string field ,std::string value){

};


void logsmanager::rotate_all(){
    /*-apply mutex
    auto deleter = sharedptrobject.getdeleter();
    deleter(sharedptrobject.get())
    sharedptrobject.reset(fopen(newfile),"a")
    */
}