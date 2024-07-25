#include "gtest/gtest.h"
#include "ctime"
#include "util/HttpReqParameterValidator.h"

TEST(HttpReqParameterValidatorTest, Sample1){
    const char* json = R"({"name": "John", "age": 30, "isStudent": false})";
    rapidjson::Document doc;
    doc.Parse(json);

    std::vector<std::pair<std::string, rapidjson::Type>> params = {
        {"name", rapidjson::kStringType},
        {"age", rapidjson::kNumberType},
        {"isStudent", rapidjson::kFalseType}
    };

    HttpReqParameterValidator validator(params);

    GTEST_ASSERT_TRUE(validator.Validate(doc));
}