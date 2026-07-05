#include "threadpool.h"


threadpool::threadpool(size_t thread_num){

    for(size_t i{0} ; i<thread_num ; i++){
            threads.emplace_back([this,i]{
              
                while(true){
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);

                        my_cv.wait(lock,[this]{
                           
                            bool has_task{ !tasks.empty()};

                            return has_task && !stop.load();
                        });


                        task=std::move(tasks.front());
                        tasks.pop();
                    }

                    if(task){
                       task(); 

                       if(dequeuedaction)dequeuedaction();
                       
                    }

                }
            });
    }
}


threadpool::~threadpool(){
    stop=true;
    my_cv.notify_all();

    for(auto & thread:threads){
        if(thread.joinable()){
            thread.join();
        }
    }
}


void threadpool::enqueue(std::function<void()> task){
    {
        std::lock_guard thelock(queue_mutex);
        tasks.emplace(std::move(task));
    }

    my_cv.notify_one();
}