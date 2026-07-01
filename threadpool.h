#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>



class threadpool{

    private:
        std::vector <std::thread> threads;
        std::queue <std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable my_cv;
        std::atomic<bool> stop{false};


    public:
        threadpool(size_t thread_num =std::thread::hardware_concurrency());
        ~threadpool();
        void enqueue(std::function<void()> task);


};
