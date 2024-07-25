cmake_minimum_required(VERSION 3.10)
project(king_server)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

find_package(PkgConfig REQUIRED)

add_subdirectory("external/workflow")

set(WORKFLOW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/workflow/_include CACHE INTERNAL "Include directory for project workflow")
add_subdirectory("external/gtest")
add_subdirectory("tests")

set(BUILD_TESTING OFF)
add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES)
add_definitions(-DGLOG_USE_GLOG_EXPORT)
add_subdirectory("external/glog")

# set(RAPIDJSON_BUILD_EXAMPLES OFF)
# set(RAPIDJSON_BUILD_TESTS OFF)
# set(RAPIDJSON_BUILD_DOC OFF)
# add_subdirectory("external/rapidjson" EXCLUDE_FROM_ALL)
include_directories("external/rapidjson/include")

include(FetchContent)
# Specify the download directory
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

FetchContent_Declare(
  sqlite3
  URL https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip
  SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/sqlite3
)

FetchContent_MakeAvailable(sqlite3)

add_library(sqlite3 STATIC
  ${sqlite3_SOURCE_DIR}/sqlite3.c
  ${sqlite3_SOURCE_DIR}/sqlite3.h
  ${sqlite3_SOURCE_DIR}/sqlite3ext.h
)

target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

# Set the include directory and library variables for compatibility
set(SQLite3_INCLUDE_DIR ${sqlite3_SOURCE_DIR})
set(SQLite3_LIBRARY sqlite3)

include_directories("external/sqlite_orm/include")
# 查找libsodium
set(LIBSODIUM_INSTALL_DIR "${CMAKE_BINARY_DIR}/libsodium-install")
set(ENV{PKG_CONFIG_PATH} "${LIBSODIUM_INSTALL_DIR}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(LIBSODIUM libsodium)
if (LIBSODIUM_FOUND)
    message(STATUS "libsodium found")
else()
    message(STATUS "libsodium not found")
endif()

if (NOT LIBSODIUM_FOUND)
    # 下载libsodium
    set(LIBSODIUM_URL "https://download.libsodium.org/libsodium/releases/libsodium-1.0.20.tar.gz")
    set(LIBSODIUM_TAR "${CMAKE_SOURCE_DIR}/external/libsodium-1.0.20.tar.gz")
    set(LIBSODIUM_SRC_DIR "${CMAKE_BINARY_DIR}/libsodium-1.0.20")
    set(LIBSODIUM_INSTALL_DIR "${CMAKE_BINARY_DIR}/libsodium-install")

    if (NOT EXISTS ${LIBSODIUM_TAR})
        message(STATUS "Downloading libsodium...")
        file(DOWNLOAD ${LIBSODIUM_URL} ${LIBSODIUM_TAR})
    endif()

    if (NOT EXISTS ${LIBSODIUM_SRC_DIR})
        message(STATUS "Extracting libsodium...")
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${LIBSODIUM_TAR} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()

    # 编译和安装libsodium到指定目录
    execute_process(COMMAND ./configure --prefix=${LIBSODIUM_INSTALL_DIR} WORKING_DIRECTORY ${LIBSODIUM_SRC_DIR})
    execute_process(COMMAND make WORKING_DIRECTORY ${LIBSODIUM_SRC_DIR})
    execute_process(COMMAND make check WORKING_DIRECTORY ${LIBSODIUM_SRC_DIR})
    execute_process(COMMAND make install WORKING_DIRECTORY ${LIBSODIUM_SRC_DIR})
endif()

set(LIBSODIUM_INCLUDE_DIR "${LIBSODIUM_INSTALL_DIR}/include")
set(LIBSODIUM_LIBRARY "${LIBSODIUM_INSTALL_DIR}/lib/libsodium.a")
# 包含libsodium
include_directories(${LIBSODIUM_INCLUDE_DIR})
link_libraries(${LIBSODIUM_LIBRARY})



file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)


target_link_libraries(${PROJECT_NAME} workflow-shared glog sqlite3)

# # 添加自定义目标以在构建后运行测试
# add_custom_target(run_tests
#     COMMAND ${CMAKE_CTEST_COMMAND}
#     DEPENDS ${TEST_TARGET_NAME}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )


