#include "stats.h"
#include <string>




void stats::increase_recieved(){
    messagesrecieved+=1;
};


void stats::increase_written(const std::string & priority){
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


std::string stats::getValue(std::string key){
    return (key=="recievedmsgs")?std::to_string(messagesrecieved):
    ("writtenmsgs")?std::to_string(messagewritten):
    ("droppedmsgs")?std::to_string(messagedroppred):
    ("highwatermarks")?std::to_string(queue_high_water_mark):
    "ERRORS"+std::to_string( priority_count[0])+" WARNING:"+to_string(priority_count[1])+
    " DEBUG"+std::to_string(priority_count[2])+" INFO:"+std::to_string(priority_count[3]);
};