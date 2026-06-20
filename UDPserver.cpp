#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include "logsmanager.h"
#include "timer.h"
#include <sys/time.h>
#include <csignal>



volatile sig_atomic_t need_shutdown = 0;

char * extractPriority(char * bufferObj);

void shutdown(int sig){
    need_shutdown=1;
};


int main(){

    signal(SIGINT,shutdown);

    logsmanager logsMng;
    timer tm;


    //=======================================must call tm.schedule(correctarguments) just here

    int sock{socket(AF_INET,SOCK_DGRAM,0)};
    if(sock<0){
        std::cerr<<"socket creation failed\n";
        return 1;
    }

    struct timeval tv;
    tv.tv_sec=1;
    tv.tv_usec=0;
    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));

   while(need_shutdown==0){

    sockaddr_in server_address;
    server_address.sin_family =AF_INET;
    server_address.sin_port=htons(8888);
    server_address.sin_addr.s_addr=INADDR_ANY;
    
    if(bind(sock,(struct sockaddr*)& server_address,sizeof(server_address))<0){
        std::cerr<<"bind failed"<<std::endl;
        return 0;
    }
   
    std::cout<<"UDP log server is listenig on port : 8888 . . .\n";
   
    char buffer[1024]={0};
    sockaddr_in client_addr;
    socklen_t client_len=sizeof(client_addr);
    
    while(!need_shutdown){
        
        tm.tick();
        int bytes=recvfrom(sock,buffer,sizeof(buffer)-1,0,
        (struct sockaddr *)& client_addr, &client_len);
        
        if(bytes<0){
            if(errno==EAGAIN||errno ==EWOULDBLOCK)
            {
            continue;
            }
            else if(errno== EINTR)
            {
                std::cout<<"Interupted by signal exiting..."<<std::endl;
            break;
            }
            else{
                std::cerr<<"rcvfrom error: "<<errno<<std::endl;
            break;
            }
        }

        if(bytes >0 ){
            
            buffer[bytes] = '\0';
            std::cout<<"full buffer"<<buffer<<"bytes:"<<bytes<<std::endl;
            const char * priority = extractPriority(buffer);
            std::cout<<"buffer after priority extarcting "<< buffer<<std::endl;

            time_t now = time(nullptr);
            char timestamp[64];
            strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H-:%M:%S", localtime(&now));
            
            logsMng.addlog(logmsg(priority,buffer,timestamp,inet_ntoa(client_addr.sin_addr),
                                            ntohs(client_addr.sin_port)));
        
            std::cout<<"["<< timestamp << "] from"
            <<inet_ntoa(client_addr.sin_addr) << ":"
            <<ntohs(client_addr.sin_port)<<" - "
            <<"priority: "<<priority<<"  msg: "
            <<buffer<<std::endl;
            delete[]  priority;
        }
        
    }
    
    close(sock);
    };

    std::cout<<"=====closing server====="<<std::endl;
    
    signal(SIGINT,SIG_DFL);
    raise(SIGINT);
    return 0;

}

//=======problem here - extract priority put whitespace 
char * extractPriority(char * bufferObj){

    int pip{};
    int fieldLength{0};
    while(bufferObj[fieldLength]!='|' && bufferObj[fieldLength] != '\0'){
        fieldLength++;
    }
    
        if(bufferObj[fieldLength] == '\0') {
        // No '|' found
        return nullptr;
    }

    char * priority=new char[fieldLength+1];
    pip=fieldLength;

    for(int i{0} ; i < fieldLength ; i++){
        priority[i]= bufferObj[i];
    }

    priority[pip]='\0';
    
    //std::cout<<"full buffer:"<<bufferObj<<std::endl;
    memmove(bufferObj+0,bufferObj+(fieldLength+1),(strlen(bufferObj)-(fieldLength+1)));

    return priority;
};


