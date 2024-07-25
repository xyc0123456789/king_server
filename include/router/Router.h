#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <memory>
#include <unordered_map>
#include <string>
#include "RouteHandler.h"
#include "util/LoggerUtil.h"


class Router {
public:
    void AddRoute(const std::string &path, const std::string &method, RouteHandler &handler) {
        LoggerUtil::LOG_INFO_FORMAT("add_route: %s %s %s", method.c_str(), path.c_str(), typeid(handler).name());
        routes[path][method] = &handler;
    }

    void SetDefaultHandler(std::shared_ptr<RouteHandler> handler) {
        LoggerUtil::LOG_INFO("SetDefaultHandler1:"+std::to_string(handler.use_count()));
        defaultHandler = handler;
        LoggerUtil::LOG_INFO("SetDefaultHandler2:"+std::to_string(handler.use_count()));
    }

    RouteHandler* GetHandler(const std::string &path, const std::string &method, WFHttpTask &task) {
        if (routes.find(path) != routes.end() && routes[path].find(method) != routes[path].end()) {
            return routes[path][method];
        }
        return defaultHandler.get();
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, RouteHandler*>> routes;
    std::shared_ptr<RouteHandler> defaultHandler;
};



#endif // _ROUTER_H_