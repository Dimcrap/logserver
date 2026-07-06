#pragma once 
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>


class statusserver{
    int http_socket;

    public:
        statusserver(){   
        };
        
        int startserver();
        void handleHttprequest();

};