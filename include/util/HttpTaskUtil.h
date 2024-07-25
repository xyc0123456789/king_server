#ifndef _IPUTIL_H_
#define _IPUTIL_H_
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "workflow/HttpUtil.h"
#include "workflow/WFHttpServer.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class HttpTaskUtil {
public:
    static std::string GetClientIp(WFHttpTask* task) {
        std::map<std::string, std::string> headers = GetHeadersMap(task);
        return HttpTaskUtil::GetClientIp(headers, GetIpFromTask(task));
    }
    

    static std::string GetIpFromTask(WFHttpTask *task) {
        struct sockaddr_in sin;
        socklen_t addrlen = sizeof(sin);
        task->get_peer_addr((struct sockaddr *)&sin, &addrlen);

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sin.sin_addr, ip, sizeof(ip));

        return std::string(ip);
    }

    static std::map<std::string, std::string> GetHeadersMap(WFHttpTask* task) {
        std::map<std::string, std::string> headers;
        protocol::HttpHeaderCursor cursor(task->get_req());
        std::string name, value;

        while (cursor.next(name, value)) {
            headers[name] = value;
        }

        return headers;
    }

    static std::string GetCookieFromTask(WFHttpTask* task){
        std::map<std::string, std::string> header = GetHeadersMap(task);
        if (header.find("Cookie")!=header.end()) {
            return header["Cookie"];
        }
        return "";
    }

    static const char* GetBodyFromTask(WFHttpTask* task) {
        const void *body;
        size_t size;
        bool ans = task->get_req()->get_parsed_body(&body, &size);
        if (ans) {
            return static_cast<const char*>(body);
        }
        return nullptr;
    }



    static std::string GetClientIp(const std::map<std::string, std::string>& headers, const std::string& peer_addr) {
        std::string client_ip;

        // 优先级顺序：X-Real-IP -> X-Forwarded-For -> peer_addr
        auto it_real_ip = headers.find("X-Real-IP");
        auto it_forwarded_for = headers.find("X-Forwarded-For");

        if (it_real_ip != headers.end() && !it_real_ip->second.empty()) {
            client_ip = it_real_ip->second;
        } else if (it_forwarded_for != headers.end() && !it_forwarded_for->second.empty()) {
            client_ip = it_forwarded_for->second;
            // X-Forwarded-For 可能包含多个 IP 地址，取第一个
            std::istringstream iss(client_ip);
            std::getline(iss, client_ip, ',');
        } else {
            client_ip = peer_addr;
        }

        // 检查是否为本地 IP
        if (IsLocalIp(client_ip)) {
            if (it_real_ip != headers.end() && !it_real_ip->second.empty()) {
                client_ip = it_real_ip->second;
            } else if (it_forwarded_for != headers.end() && !it_forwarded_for->second.empty()) {
                client_ip = it_forwarded_for->second;
                std::istringstream iss(client_ip);
                std::getline(iss, client_ip, ',');
            }
        }

        return client_ip;
    }

    static bool IsLocalIp(const std::string& ip) {
        static const std::vector<std::string> local_ips = {
            "127.0.0.1", "0.0.0.0", "::1"
        };
        return std::find(local_ips.begin(), local_ips.end(), ip) != local_ips.end();
    }
};


#endif // _IPUTIL_H_