#pragma once 

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include "stats.h"
#include <memory>
#include <optional>
#include <functional>


class statusserver{
    int http_socket;
    std::optional<std::reference_wrapper<stats>> statsobject;

    public:
        statusserver(stats &statsobj){   
            statsobject=std::ref(statsobj);
        };
        
        int startserver();
        void listenserver();
        void handleHttprequest(int clientsocket);
        
};