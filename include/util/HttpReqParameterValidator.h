#ifndef _HTTPREQPARAMETERVALIDATOR_H_
#define _HTTPREQPARAMETERVALIDATOR_H_

#include <string>
#include <vector>
#include <iostream>
#include "rapidjson/document.h"


class HttpReqParameterValidator {
public:
    HttpReqParameterValidator(const std::vector<std::pair<std::string, rapidjson::Type>>& params) : params(params) {}

    [[nodiscard]] bool Validate(const rapidjson::Document& doc) const {
        for (const auto& param : params) {
            if (!doc.HasMember(param.first.c_str()) || doc[param.first.c_str()].GetType() != param.second) {
                std::cerr << "Parameter " << param.first << " is missing or has incorrect type." << std::endl;
                return false;
            }
        }
        return true;
    }

private:
    std::vector<std::pair<std::string, rapidjson::Type>> params;
};

#endif // _HTTPREQPARAMETERVALIDATOR_H_