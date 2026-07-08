#include <atomic>
#include <cstdint>
#include <array>
#include <string>


class stats{
    std::atomic<uint64_t> messagesrecieved{0};
    std::atomic<uint64_t> messagewritten{0};
    std::atomic<uint64_t> messagedroppred{0};
    std::atomic<uint64_t>queue_high_water_mark{0};
    //errors_encountred 
    std::array<std::atomic<uint64_t>,4> priority_count;  
    public:
    void increase_recieved();
    void increase_written(const std::string & priority);
    void increase_dropped();
    void update_queue_size(size_t current);
    std::string getValue(char firstword);
};