#include "stats.h"




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