#include <chrono>
#include <functional>
#include <vector>



class timer {
    struct  scheduledTask
    {
        std::chrono::steady_clock::time_point next_excution;
        std::chrono::seconds interval;
        std::function<void()> callback;
        int id;
    };

    std::vector<scheduledTask> tasks;
    int nextId{0};

    public:

    void schedule(std::function <void()> task,std::chrono::seconds interval){
        tasks.push_back({
            std::chrono::steady_clock::now()+ interval,
            interval,
            task,
            nextId++
        });
    };

    void tick(){
        auto now=std::chrono::steady_clock::now();

        for (auto & task: tasks){
            if(now>=task.next_excution){
                task.callback();
                task.next_excution=now+task.interval;
            }
        }
    };
    
    
};