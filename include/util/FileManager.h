#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <iostream>
#include <fstream>
#include <string>

class FileManager {
public:
    // 从文件读取内容并返回字符串
    static std::string readFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filePath << std::endl;
            return "";
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return content;
    }

    // 将字符串写入指定文件
    static void writeToFile(const std::string& filePath, const std::string& content) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filePath << std::endl;
            return;
        }

        file << content;
        file.close();
    }
};

#endif // _FILEMANAGER_H_