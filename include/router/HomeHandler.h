#ifndef _HOMEHANDLER_H_
#define _HOMEHANDLER_H_

#include "RouteHandler.h"

class HomeHandler : public RouteHandler {
public:
    void Handle(WFHttpTask *task) override {
        task->get_resp()->append_output_body("<html>Welcome to the homepage!</html>");
    }
};

#endif // _HOMEHANDLER_H_

