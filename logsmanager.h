#include <string>
#include <chrono>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include "threadpool.h"




class logsmanager{
    public:
        logsmanager();
        void addlog(logmsg log);
        void writeTofile(logmsg log);

    private:
        threadpool thpool;
        std::mutex WARN_mutex,INFO_mutex,ERROR_mutex,DEBUG_mutex;
        std::unordered_map<std::string,std::ofstream> files{
            {"INFO",std::ofstream("info.log",std::ios::app)},{"ERROR",std::ofstream("error.log",std::ios::app)},
            {"WARNING",std::ofstream("warning.log",std::ios::app)},{"DEBUG",std::ofstream("debug.log",std::ios::app)}};
        std::ofstream &getfile(const std::string &name){
            return files.at(name);
        };

};


struct logmsg{
    std::string priority,msgbody,address;
    int logsport;
    char timestamp[64];
    
    logmsg(std::string field1="",std::string field2="",char ts[64]="",
        std::string addr="0,0,0,0",int port=0):
    priority(field1),msgbody(field2),address(addr),logsport(port){
        strncpy(timestamp,ts,sizeof(timestamp)-1);
        timestamp[sizeof(timestamp)-1]='\0';    
    }
};