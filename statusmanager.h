#pragma once 

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include "stats.h"
#include <memory>
#include <optional>
#include <functional>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>



class statusmanager{
    int http_socket;
    std::optional<std::reference_wrapper<stats>> statsobject;
    bool running{false};

    public:
        statusmanager(stats &statsobj):running(true){   
            statsobject=std::ref(statsobj);
        };
        ~statusmanager(){
            running=false;
        }

        int startserver();
        void listenserver();
        void handleHttprequest(int clientsocket);
};