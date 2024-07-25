#ifndef _UNIQUEIDGENERATOR_H_
#define _UNIQUEIDGENERATOR_H_

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <ostream>
#include "bitset"

class UniqueIdGenerator {
public:
    static int64_t generateUniqueId() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        int64_t timePart = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        int64_t counterPart = counter.fetch_add(1, std::memory_order_relaxed);
        int64_t target_id = (timePart << 30) | (counterPart & 0x3FFFFFFF);
        // std::cout<<"uniqueId: "<< std::bitset<64>(timePart) << " : " << std::bitset<64>(counterPart) << " : " << std::bitset<64>(target_id) << std::endl;
        return target_id;
    }


    static std::atomic<int64_t> counter;
};

std::atomic<int64_t> UniqueIdGenerator::counter(0);

#endif // _UNIQUEIDGENERATOR_H_