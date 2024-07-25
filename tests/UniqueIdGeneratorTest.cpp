#include <cstdint>
#include <thread>
#include <atomic>
#include <cassert>
#include "unordered_set"
#include "gtest/gtest.h"
#include "iostream"
#include "util/UniqueIdGenerator.h"

std::atomic<bool> x, y;
std::atomic<int> z;
 
void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);  // 1
    y.store(true, std::memory_order_relaxed);  // 2 如果这里不是顺序执行，可能导致z==0
}
 
void read_y_then_x() {
    while (!y.load(std::memory_order_relaxed)) { // 3
        /* code */
    }
 
    if (x.load(std::memory_order_relaxed)) { //4
        ++z;
    }
}
 
TEST(UniqueIdGeneratorTest, testAtomic) {
 
    x = false;
    y = false;
    z = 0;
 
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    t1.join();
    t2.join();
    int64_t tmp = z.load();
    std::cout << "Value of z: " << tmp << std::endl; // 打印 z 的值
    GTEST_ASSERT_TRUE(z.load() != 0); // 5
}

TEST(UniqueIdGeneratorTest, generateId) {
    std::unordered_set<int64_t> ids;
    std::vector<std::thread> threads;
    std::mutex ids_mutex;

    auto generateIds = [&ids, &ids_mutex] {
        std::unordered_set<int64_t> local_ids;
        for (int i = 0; i < 500; ++i) {
            int64_t id = UniqueIdGenerator::generateUniqueId();
            local_ids.insert(id);
        }
        std::lock_guard<std::mutex> lock(ids_mutex);
        ids.insert(local_ids.begin(), local_ids.end());
    };

    // 创建两个线程，每个线程生成100个ID
    for (int i=0; i<20; i++) {
        threads.emplace_back(generateIds);
    }

    for (auto& t : threads) {
        t.join();
    }

    // 检查生成的ID数量是否为200
    EXPECT_EQ(ids.size(), 10000);
    // 检查counter的值是否为200
    EXPECT_EQ(UniqueIdGenerator::counter.load(), 10000);
}
