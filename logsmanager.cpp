#include "logsmanager.h"




logsmanager::logsmanager():thpool(4){
    for(const auto &[name,file]  :files){
        if(!file.is_open()){
            std::cout<<"unable to open file "<<name<<std::endl;
        }
    }
}

void logsmanager::addlog(logmsg log){
    
    thpool.enqueue([this,log](){
        if(log.priority=="INFO"){
            std::lock_guard<std::mutex> lock(INFO_mutex);
            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;

        }else if(log.priority=="WARN"){
            std::lock_guard<std::mutex> lock(WARN_mutex);
            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;
        }else if(log.priority=="DEBUG"){
            std::lock_guard<std::mutex> lock(DEBUG_mutex);
            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;
        }else{
            std::lock_guard<std::mutex> lock(ERROR_mutex);
            files.at(log.priority)<<"["<<std::string(log.timestamp)<<"] from"<<log.address<<
            ":"<<log.logsport<<"-"<<"priority"<<log.priority<<" message"": "<<log.msgbody;
        }

    });
};


void logsmanager::writeTofile(logmsg log){
    
}