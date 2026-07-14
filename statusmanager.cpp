#include "statusmanager.h"
#include <chrono>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <iostream>



int statusmanager::startserver(){
    http_socket=socket(AF_INET, SOCK_STREAM, 0);
    if(http_socket<0){
        std::cerr<<"failed to build socket for server \n";
        return 0;
    }
    int opt=1;
    setsockopt(http_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port =htons(8889);
    addr.sin_addr.s_addr =INADDR_ANY;
    
    if(bind(http_socket,(sockaddr *)&addr,sizeof(addr))<0){
        std::cout<<"could not bind the socket with  socket address \n";
        return -1;
    };

    listen(http_socket, 5);

    return 1;
};



void statusmanager::handleHttprequest(int clientsocket){
    char buffer[512];
    read(clientsocket, buffer, sizeof(buffer)-1);
    std::string request{buffer};

    if(request.find("GET /stats")==0){
        std::string json="{\n"
        "recived messages:"+statsobject.value().get().getValue('r')+
        ",\nwriten messages:"+statsobject.value().get().getValue('w')+
        ",\ndrpped messages:"+statsobject.value().get().getValue('d')+
        ",\nqueue watermark:"+statsobject.value().get().getValue('p')+
        ",\ndrpped messages:"+statsobject.value().get().getValue('d')+
        "}\n";

        std::string response{
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json/\r\n"
        "Content-Length: "+ std::to_string(json.size())+ "\r\n"
        "\r"+json};
        
        write(clientsocket, response.c_str(), response.size());
    }else if(request.find("GET‌ /health")==0){
        std::string response{
            "HTTP/1.1 200 OK\r\n"
            "Content-type: text/plain\r\n"
            "\r\n"
            "Ok\n"};
        write(clientsocket, response.c_str(), response.size());
    }else{
        std::string response{"\n\rHTTP/1.1 404 Not found\r\n\r\n"};
        write(clientsocket, response.c_str(), response.size());
    }
    

};


void statusmanager::listenserver(){

    std::cout<<"\nhttp servser listening on port:8889 . . .\n";
    
    
    while(running){
        int clientsocket=accept(http_socket, nullptr, nullptr);

        if(clientsocket<0 ){
         if(!running)break;
         continue;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        //std::cout<<"from while loop \n";
        handleHttprequest(clientsocket);
        
        close(clientsocket);

    }
    
};