#include "stats.h"
#include <string>




void stats::increase_recieved(){
    messagesrecieved+=1;
};


void stats::increase_written(const std::string & priority){
    std::cout<<"increase written called"<<std::endl;
    messagewritten+=1;
    int indx{(priority=="ERROR")?0:(priority=="WARN")?1:priority=="INFO"?2:3};
    priority_count[indx]+=1;
};


void stats::increase_dropped(){
    messagedroppred+=1;
};


void stats::update_queue_size(size_t current){
    queue_high_water_mark=current;
};


std::string stats::getValue(char firstword){

    if(firstword!='r' && firstword!='R' && firstword!='w' && firstword!='W' && firstword!='d' && 
        firstword!='D'&& firstword!='h' && firstword!='H' && firstword != 'p' && firstword != 'P')
        {
        std::cerr<<"undefined value in getValue :"<<firstword<<std::endl;
        return "";
        }

        std::cout<<"contents of priority count:"<<priority_count[0]<<" "<<
        priority_count[1]<<" "<<priority_count[2]<<" "<<priority_count[3]<<std::endl;
        
    return (firstword=='r' || firstword=='R')?std::to_string(messagesrecieved):
    (firstword=='w' || firstword=='W')?std::to_string(messagewritten):
    (firstword=='d' || firstword=='D')?std::to_string(messagedroppred):
    (firstword=='h' || firstword=='H')?std::to_string(queue_high_water_mark):
    "ERRORS: "+std::to_string( priority_count[0])+" WARNING:"+to_string(priority_count[1])+
    " DEBUG: "+std::to_string(priority_count[2])+" INFO:"+std::to_string(priority_count[3]);
};