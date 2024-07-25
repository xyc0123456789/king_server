#ifndef _DEFAULTHANDLER_H_
#define _DEFAULTHANDLER_H_

#include "RouteHandler.h"

class DefaultHandler : public RouteHandler {
public:
    void Handle(WFHttpTask *task) override {
        task->get_resp()->append_output_body("<html>404 Not Found</html>");
    }
};

#endif // _DEFAULTHANDLER_H_