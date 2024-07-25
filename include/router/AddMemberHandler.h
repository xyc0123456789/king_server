#ifndef _ADDMEMBERHANDLER_H_
#define _ADDMEMBERHANDLER_H_


#include "RouteHandler.h"
#include "db/MemberDb.h"
#include "util/HttpTaskUtil.h"
#include "util/HttpReqParameterValidator.h"
#include "util/JsonBuilder.h"
#include "rapidjson/document.h"
#include <cassert>
#include <optional>
#include <string>

class AddMemberHandler : public RouteHandler {
public:
    void Handle(WFHttpTask *task) override {
        
        const char* body = HttpTaskUtil::GetBodyFromTask(task);

        rapidjson::Document root;
        if (root.Parse(body).HasParseError()) {
            return;
        }
        HttpReqParameterValidator validator(params);
        assert(validator.Validate(root));
        const std::string name = root["name"].GetString();

        MemberService& memberService = MemberService::getInstance();
        std::optional<MemberDTO> member = memberService.getMemberByName(name);
        if (member.has_value()) {
            
            return;
        }

        memberService.addMember(name, root["email"].GetString(), root["password"].GetString());
        JSONBuilder jsonBuilder;
        jsonBuilder.AddMember("code", "success");


        task->get_resp()->append_output_body("<html>/////</html>");
    }

    const std::vector<std::pair<std::string, rapidjson::Type>> params = {
        {"name", rapidjson::kStringType},
        {"email", rapidjson::kStringType},
        {"password", rapidjson::kStringType}
    };
};
#endif // _ADDMEMBERHANDLER_H_