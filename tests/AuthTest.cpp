#include "auth/UserAuth.h"
#include <gtest/gtest.h>

// 示例测试
TEST(AuthTest, BasicAssertions) {
    UserAuth auth;
    auth.addToWhitelist("/secure/*");
    auth.addToWhitelist("/admin/*");
    auth.addToBlacklist("/banned/*");

    std::string uri = "/secure/home";
    std::string cookie = "user_session=abc123";

    // ASSERT_TRUE(auth.isAuthenticated(uri, cookie));

    // ASSERT_FALSE(auth.isAuthenticated("/banned/5", cookie));
}