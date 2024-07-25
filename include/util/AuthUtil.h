#ifndef _AUTHUTIL_H_
#define _AUTHUTIL_H_
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include "db/MemberDb.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"

class AuthUtil {
private:
    static std::map<std::string, MemberDTO> users_;
    static std::map<std::string, std::pair<std::string, std::time_t>> token_map_; // token -> (IP, timestamp)

    static std::string generate_token(int64_t user_id) {
        std::ostringstream oss;
        oss << user_id << "_" << std::time(nullptr);
        return oss.str(); // 简单的 token 生成方式，可以使用更安全的方法
    }

    static bool is_token_valid(const std::string& token, const std::string& ip) {
        auto it = token_map_.find(token);
        if (it != token_map_.end() && it->second.first == ip) {
            // 检查 token 是否过期（假设有效期为 1 小时）
            std::time_t now = std::time(nullptr);
            return (now - it->second.second) < 3600;
        }
        return false;
    }

public:
    static bool AuthOneMember(MemberDTO& memberDto, rapidjson::Document& requestJson, std::string& ip, std::string& cookie) {

        if (!requestJson.HasMember("email") || !requestJson["email"].IsString() ||
            !requestJson.HasMember("password") || !requestJson["password"].IsString()) {
            return false;
        }

        std::string email = requestJson["email"].GetString();
        std::string password = requestJson["password"].GetString();

        // 验证用户名和密码
        auto it = users_.find(email);
        if (it == users_.end() || it->second.password != password || !it->second.enable) {
            return false;
        }

        // 生成 token
        std::string token = generate_token(it->second.id);

        // 设置 cookie
        cookie = "token=" + token + "; Path=/; HttpOnly";

        // 记录时间和 IP
        token_map_[cookie] = std::make_pair(ip, std::time(nullptr));

        // 更新 memberDto
        // memberDto = it->second;

        return true;
    }

    static bool AuthenticateRequest(std::string& cookie, std::string& ip) {
        // 提取 token
        // std::string token;
        // size_t pos = cookie.find("token=");
        // if (pos != std::string::npos) {
        //     token = cookie.substr(pos + 6);
        //     pos = token.find(';');
        //     if (pos != std::string::npos) {
        //         token = token.substr(0, pos);
        //     }
        // }

        // 验证 token 和 IP
        return is_token_valid(cookie, ip);
    }
};

// 初始化静态成员变量
std::map<std::string, MemberDTO> AuthUtil::users_;
std::map<std::string, std::pair<std::string, std::time_t>> AuthUtil::token_map_;

#endif // _AUTHUTIL_H_