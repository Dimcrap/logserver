#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string_view>
#include "lorem.h"
#include <chrono>





std::vector<std::string_view> messagmaker(int msgcount);
int generateseed();

int main(int argc,char * argv[]){

    if (argc!=2){
        std::cout<<"invalid argument count!\n"<<argv[0]<<" \'count of messages\' \n";
    }
    

}



int generateseed(){
    auto seq1 =std::chrono::high_resolution_clock::now();
    auto since_epoch=seq1.time_since_epoch();

    return std::chrono::duration_cast<std::chrono::seconds>(since_epoch).count();
}


std::vector<std::string_view> messagmaker(int msgcount){
    
    std::vector<std::string_view> output;
    for (int i{0};i<msgcount;i++){
        
        LOREM_genBuffer();

    }
};