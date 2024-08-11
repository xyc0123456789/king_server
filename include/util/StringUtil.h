#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_


#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

// std::vector<unsigned char> 转换为 std::string
std::string vectorToString(const std::vector<unsigned char>& vec) {
    return std::string(vec.begin(), vec.end());
}

// std::string 转换为 std::vector<unsigned char>
std::vector<unsigned char> stringToVector(const std::string& str) {
    return std::vector<unsigned char>(str.begin(), str.end());
}

// std::vector<unsigned char> 转换为十六进制字符串
std::string vectorToHex(const std::vector<unsigned char>& vec) {
    std::ostringstream oss;
    for (auto ch : vec) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    }
    return oss.str();
}

// 十六进制字符串转换为 std::vector<unsigned char>
std::vector<unsigned char> hexToVector(const std::string& hex) {
    std::vector<unsigned char> vec;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
        vec.push_back(byte);
    }
    return vec;
}


#endif // _STRINGUTIL_H_