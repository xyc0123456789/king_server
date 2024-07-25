#ifndef _ABOUTHANDLER_H_
#define _ABOUTHANDLER_H_

#include "RouteHandler.h"

class AboutHandler : public RouteHandler {
public:
    void Handle(WFHttpTask *task) override {
        task->get_resp()->append_output_body("<html>About us page</html>");
    }
};



#endif // _ABOUTHANDLER_H_