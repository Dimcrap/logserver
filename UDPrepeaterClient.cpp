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
        //std::cout<<"sending message :"<<msg.c_str()<<"\"length of str "<<msg.size()<<std::endl;
        sendto(thesocket,msg.c_str(),msg.size(),0,(struct sockaddr *)&server_addr,
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
    char msgbody[57];
    int seed{0};

    for (int i{0};i<msgcount;i++){
        
        LOREM_genBuffer(msgbody,56,generateseed());
        msgbody[56]='\0';
        fulltext+=priorities[distribution(gen)];
        //fulltext+=std::string(msgbody,56);
        //fulltext+="Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
        //msgbody[0]='\0';
        
        int text_end = 0;
        for(int i = 0; i < 56; i++) {
            if(msgbody[i] == '\0' || msgbody[i] < 32) {  // null or control char
                break;
            }
            if(msgbody[i] >= 32 && msgbody[i] <= 126) {  // printable
                text_end = i + 1;
            }
        }
        std::string lorem_text(msgbody,text_end);


        while(!lorem_text.empty()&& lorem_text.back()==' '){
            lorem_text.pop_back();
        }
        std::cout<<"sending message with body:"<<lorem_text<<" with size: "<<lorem_text.length()<<std::endl;
        fulltext+=lorem_text;
        std::cout<<"fulltext :"<<fulltext<<"with length "<< fulltext.length()<<std::endl;
        output.emplace_back(fulltext);
        fulltext.clear();

    }

    return output;
};