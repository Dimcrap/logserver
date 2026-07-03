#include <atomic>
#include <cstdint>
#include <array>
#include <string>

class stats{
    std::atomic<uint64_t> messagesrecieved{0};
    std::atomic<uint64_t> messagewritten{0};
    std::atomic<uint64_t> messagedropprf{};
    //queue_high_water_mark
    //errors_encountred 
    std::array<std::atomic<uint64_t>,4> piority_count;  
    public:
    void increament_recieved();
    void increament_written(const std::string & priority);
    void increament_dropped();
    void update_queue_size(size_t current);
};