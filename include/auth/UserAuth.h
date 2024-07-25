#ifndef _USERAUTH_H_
#define _USERAUTH_H_

#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include "workflow/WFTaskFactory.h"


class UserAuth
{
public:
    UserAuth() = default;

    void addToWhitelist(const std::string& pattern) {
        whitelist_.emplace_back(std::regex(convertWildcardToRegex(pattern)));
    }

    void addToBlacklist(const std::string& pattern) {
        blacklist_.emplace_back(std::regex(convertWildcardToRegex(pattern)));
    }

    bool isAuthenticated(const std::string& uri, WFHttpTask &task, std::string& redict_uri) const {
        for (const auto& pattern : blacklist_) {
            if (std::regex_match(uri, pattern)) {
                return false; // URI匹配黑名单模式，拒绝访问
            }
        }

        for (const auto& pattern : whitelist_) {
            if (std::regex_match(uri, pattern)) {
                return validateCookie(task, redict_uri); // URI匹配白名单模式，验证Cookie
            }
        }

        return false; // URI不匹配任何白名单模式，拒绝访问
    }

private:
    bool validateCookie(WFHttpTask &task, std::string& redict_uri) const {
        // ip + cookie

        return false;
    }

    std::string convertWildcardToRegex(const std::string& pattern) const {
        std::string regexPattern = "^";
        for (char c : pattern) {
            if (c == '*') {
                regexPattern += ".*";
            } else if (c == '?') {
                regexPattern += ".";
            } else {
                regexPattern += std::regex_replace(std::string(1, c), std::regex(R"([\^$.|()\\[\]{}])"), R"(\$&)");
            }
        }
        regexPattern += "$";
        return regexPattern;
    }

    std::vector<std::regex> whitelist_;
    std::vector<std::regex> blacklist_;
};

#endif // _USERAUTH_H_