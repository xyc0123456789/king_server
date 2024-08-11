# 设置测试目标名称
set(TEST_TARGET_NAME king_server_tests)

# 查找所有测试源文件
file(GLOB TEST_SOURCES *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external/rapidjson/include)
include_directories(${PROJECT_SOURCE_DIR}/build/libsodium-install/include)

# 设置libsodium相关变量
set(LIBSODIUM_INSTALL_DIR "${CMAKE_BINARY_DIR}/libsodium-install")
set(LIBSODIUM_INCLUDE_DIR "${LIBSODIUM_INSTALL_DIR}/include")
set(LIBSODIUM_LIBRARY "${LIBSODIUM_INSTALL_DIR}/lib/libsodium.a")
# 包含libsodium
include_directories(${LIBSODIUM_INCLUDE_DIR})
# 打印LIBSODIUM_LIBRARY路径以进行调试
message(STATUS "LIBSODIUM_LIBRARY: ${LIBSODIUM_LIBRARY}")
# 创建导入库目标
add_library(Libsodium STATIC IMPORTED)
set_target_properties(Libsodium PROPERTIES IMPORTED_LOCATION ${LIBSODIUM_LIBRARY})


set(LIBBOTAN_INSTALL_DIR "${CMAKE_BINARY_DIR}/botan-install")
include_directories(${LIBBOTAN_INSTALL_DIR}/include/botan-3)
link_directories(${LIBBOTAN_INSTALL_DIR}/lib)


# 添加可执行文件
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})
target_include_directories(${TEST_TARGET_NAME} PRIVATE ${WORKFLOW_INCLUDE_DIR})

# 链接GoogleTest库和项目库
target_link_libraries(${TEST_TARGET_NAME} gtest gtest_main glog Libsodium botan-3)

# 启用测试
enable_testing()

# 添加测试
add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})
