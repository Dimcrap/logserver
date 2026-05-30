#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string_view>
#include "lorem.h"
#include <chrono>
#include <charconv>
#include <thread>




std::vector<std::string> messagmaker(int msgcount);
int generateseed();

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

    std::vector<std::string>messages {messagmaker(msgcount)};

    for(std::string_view msg :messages){
        std::cout<<msg<<std::endl;
    }


}



int generateseed(){
    auto seq1 =std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    auto since_epoch=seq1.time_since_epoch();

    return std::chrono::duration_cast<std::chrono::seconds>(since_epoch).count();
}


std::vector<std::string> messagmaker(int msgcount){
    
    std::vector<std::string> output;
    char msg[64];
    int seed{0};

    for (int i{0};i<msgcount;i++){
        
        seed=generateseed();
        std::cout<<"generated seed"<<seed<<std::endl;
        LOREM_genBuffer(msg,64,seed);

        output.emplace_back(msg);

    }

    return output;
};