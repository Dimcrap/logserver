#include <fstream>
#include <iostream>





int main(){

    std::ifstream source("config.txt");
    if(!source.is_open()){
        std::cerr<<"failed to open config file"<<std::endl;
        return 0;
    }
}