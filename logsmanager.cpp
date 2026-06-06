#include "logsmanager.h"




logsmanager::logsmanager():thpool(4){
    files.try_emplace("INFO",std::ofstream("../logs/info.log",std::ios::app));
    files.try_emplace("ERROR",std::ofstream("../logs/error.log",std::ios::app));
    files.try_emplace("WARNING",std::ofstream("../logs/warning.log",std::ios::app));
    files.try_emplace("DEBUG",std::ofstream("../logs/debug.log",std::ios::app));

    for(const auto &[name,file] :files){
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
    }

};