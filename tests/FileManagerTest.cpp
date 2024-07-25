#include "gtest/gtest.h"
#include "ctime"
#include "util/FileManager.h"

TEST(FileManagerTest, FileWriteAndRead){

    std::string filePath = "example.txt";
    
    // 写入字符串到文件
    std::time_t now = std::time(nullptr);
    std::string timestamp = std::ctime(&now);
    std::string contentToWrite = "Hello, World!" + timestamp;
    FileManager::writeToFile(filePath, contentToWrite);
    // 从文件读取字符串
    std::string contentRead = FileManager::readFromFile(filePath);
    
    ASSERT_EQ(contentToWrite, contentRead);
}