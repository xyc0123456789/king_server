#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "util/AuthUtil.h"
#include "util/HttpTaskUtil.h"
#include "db/MemberDb.h"
#include "RouteHandler.h"
#include "string"
#include "util/JsonUtil.h"
#include "util/LoggerUtil.h"
#include "workflow/HttpMessage.h"
#include <optional>
#include <string>

class LoginHandler : public RouteHandler {
public:
    void Handle(WFHttpTask *task) override {
        //输入：json 验证用户名密码
        //生成token, 记录时间, 返回链接，设置cookie，cookie+ip三个关联
        //网页跳转至指定链接（通用页面/控制台) 分两部分，一部分页面，一部分具体项目
        //通过认证返回控制台，不通过返回error
        //后续页面在控制台显示，1、每个人根据权限显示的页面项目
        std::string ip = HttpTaskUtil::GetClientIp(task);
        LoggerUtil::LOG_INFO("get_client_ip: "+ip);
        std::string cookie = HttpTaskUtil::GetCookieFromTask(task);
        
        const char* body = HttpTaskUtil::GetBodyFromTask(task);


        rapidjson::Document root;
        if (root.Parse(body).HasParseError()) {

            return;
        }
        if (!root.HasMember("user_name") || !root["user_name"].IsString()){
            return;
        }
        std::string name = root["user_name"].GetString();;

        MemberService &memberService = MemberService::getInstance();
        std::optional<MemberDTO> memberDTO =memberService.getMemberByName(name);// 解析json，查询数据库
        if (!memberDTO.has_value()) {
            return;
        }
        bool ans = AuthUtil::AuthOneMember(memberDTO.value(), root, ip, cookie);


    }
};




#endif // _LOGIN_H_