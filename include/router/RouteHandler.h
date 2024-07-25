#ifndef _ROUTEHANDLER_H_
#define _ROUTEHANDLER_H_

#include "workflow/WFTaskFactory.h"
class RouteHandler {
public:
    virtual void Handle(WFHttpTask *task) = 0;
    virtual ~RouteHandler() = default;
};

#endif // _ROUTEHANDLER_H_
