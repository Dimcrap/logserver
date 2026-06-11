#include <string>
#include <chrono>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include "threadpool.h"
#include <memory>


struct logmsg{
    std::string priority,msgbody,address;
    int logsport;
    char timestamp[64];
    
    logmsg(std::string field1="",std::string field2="",char ts[64]=nullptr,
        std::string addr="0,0,0,0",int port=0):
    priority(field1),msgbody(field2),address(addr),logsport(port){
        strncpy(timestamp,ts,sizeof(timestamp)-1);
        timestamp[sizeof(timestamp)-1]='\0';    
    }
};


class logsmanager{
    public:
        logsmanager();
        ~logsmanager();
        void addlog(logmsg log);
        void rotate_all();

    private:
        threadpool thpool;
        
        std::mutex WARN_mutex,INFO_mutex,ERROR_mutex,DEBUG_mutex,;
        std::shared_ptr<FILE> WARN;
        std::shared_ptr<FILE> INFO;
        std::shared_ptr<FILE> ERROR;
        std::shared_ptr<FILE> DEBUG;
    
        std::string definefromconfig(std::string field);
        /*std::unordered_map<std::string,std::ofstream> files;
        std::ofstream &getfile(const std::string &name){
            return files.at(name);
        };*/


};

