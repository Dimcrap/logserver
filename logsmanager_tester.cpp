#include <fstream>
#include <iostream>
#include <vector>




int main(){

    std::ifstream source("config.txt");
    if(!source.is_open()){
        std::cerr<<"failed to open config file"<<std::endl;
        return 0;
    }


    std::string word{""};
    std::vector<std::string> contents;

    while(source >> word){
        
        contents.push_back(word);
        
    }

    for(std::string str:contents){
        std::cout<<str<<"|";
    }
    //std::cout<<"content length:"<<contents[1].size()/contents.size();
    std::cout<<"index at 1:"<<contents[1]<<"at 2:"<<contents[2]<<std::endl;

}