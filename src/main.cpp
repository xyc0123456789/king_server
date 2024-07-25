#include "router/AddMemberHandler.h"
#include "router/AboutHandler.h"
#include "router/DefaultHandler.h"
#include "router/HomeHandler.h"
#include "router/RouteHandler.h"
#include "router/Router.h"
#include "workflow/HttpUtil.h"
#include "workflow/WFHttpServer.h"
#include <memory>
#include <cstdio>
#include <string>
#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

#include "util/LoggerUtil.h"
#include "router/LoginHandler.h"

// 示例使用
int main(int argc, char* argv[])
{
    // 初始化日志
    LoggerUtil::Init(argv[0]);

    Router router;
    HomeHandler home_handler;
    AboutHandler about_handler;
    AddMemberHandler addMemberHandler;
    LoginHandler loginHandler;

    std::shared_ptr<RouteHandler> defaultHandler =std::make_shared<DefaultHandler>();

    router.AddRoute("/", "GET", home_handler);
    router.AddRoute("/about", "GET", about_handler);
    router.AddRoute("/add", "GET", addMemberHandler);
    router.AddRoute("/login", "POST", loginHandler);
    LoggerUtil::LOG_INFO("SetDefaultHandler before:"+std::to_string(defaultHandler.use_count()));
    router.SetDefaultHandler(defaultHandler);
    LoggerUtil::LOG_INFO("SetDefaultHandler after:"+std::to_string(defaultHandler.use_count()));
    WFHttpServer server([&router](WFHttpTask* task) {
        std::string path = task->get_req()->get_request_uri();
        const char* method = task->get_req()->get_method();
        protocol::HttpHeaderCursor cursor(task->get_req());
        std::string name, value;
        std::ostringstream headers;

        while (cursor.next(name, value)) {
            std::string modify_value = value;
            if (name == "Cookie") {
                modify_value = "server_get_cookie_len=" + std::to_string(value.size());
            }
            headers << name << ": " << modify_value << "; ";
        }
        RouteHandler* handler = router.GetHandler(path, method, *task);
        LOG_INFO_FORMAT("%s %s ----> %s ----> %s", method, path.c_str(), typeid(*handler).name(), headers.str().c_str());
        handler->Handle(task);
    });

    if (server.start(8888) == 0) {
        getchar();
        server.stop();
    }

    // 关闭日志
    LoggerUtil::Shutdown();
    return 0;
}
