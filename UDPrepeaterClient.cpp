#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string_view>
#include "lorem.h"
#include <chrono>
#include <charconv>
#include <thread>
#include <unistd.h>
#include <random>







std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distribution(0,3);

std::vector<std::string> messagmaker(int msgcount);
int generateseed();
std::vector<std::string> priorities{"ERROR|","INFO|","WARN|","DEBUG|"};


int main(int argc,char * argv[]){

    if (argc!=2){
        std::cout<<"invalid argument count!\n"<<argv[0]<<" \'count of messages\' \n";
    }
    
    int msgcount{1};
    auto casted=std::from_chars(argv[1],argv[1]+std::strlen(argv[1]),msgcount);

    if(casted.ec!=std::errc()){
        std::cerr<<"failed to borrow count(check inputed number count)";
        return 0;
    }

    std::vector<std::string>messages { messagmaker(msgcount) };

    int thesocket {socket(AF_INET,SOCK_DGRAM,0)};
    if(thesocket<0){
        std::cerr<<"could't load the socket "<<std::endl;
        return 0;
    }

    sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8888);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");    

    for(std::string msg:messages){
        sendto(thesocket,&msg,msg.size(),0,(struct sockaddr *)&server_addr,
        sizeof(server_addr));
        };


    close(thesocket);
    return 0;
}



int generateseed(){
    auto seq1=std::chrono::steady_clock::now();
    auto since_epoch=seq1.time_since_epoch();

    return std::chrono::duration_cast<std::chrono::nanoseconds>(since_epoch).count();
}


std::vector<std::string> messagmaker(int msgcount){
    
    std::vector<std::string> output;
    std::string fulltext{""};
    char msgbody[70];
    int seed{0};

    for (int i{0};i<msgcount;i++){
        
        LOREM_genBuffer(msgbody,64,generateseed());
        fulltext+=priorities[distribution(gen)];
        fulltext+=msgbody;

        output.emplace_back(fulltext);

    }

    return output;
};