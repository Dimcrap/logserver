#include "statusserver.h"
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>



int statusserver::startserver(){
    http_socket=socket(AF_INET, SOCK_STREAM, 0);
    if(http_socket<0){
        std::cerr<<"failed to build socket for server \n";
        return 0;
    }
    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port =htons(8889);
    addr.sin_addr.s_addr =INADDR_ANY;

    if(bind(http_socket,(sockaddr *)&addr,sizeof(addr))<0){
        std::cout<<"could not bind the socket with  socket address \n";
        return 0;
    };
    listen(http_socket, 5);

    return 1;
};



void statusserver::handleHttprequest(int clientsocket){
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
    
    close(clientsocket);

};


void statusserver::listenserver(){

    char buffer[256];
        sockaddr_in client_addr;
        socklen_t client_addrlength=sizeof(client_addr);

    while(true){
        
        int clientsocket=accept(http_socket, nullptr, nullptr);
        int bytes=recvfrom(http_socket, buffer, sizeof(buffer)-1,0,
         (struct sockaddr *) & client_addr,&client_addrlength);
     
        if(bytes>0){
            handleHttprequest(clientsocket);
        }
        close(http_socket);
    }

};