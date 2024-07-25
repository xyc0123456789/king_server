#ifndef _LOGGERUTIL_CPP_
#define _LOGGERUTIL_CPP_

#include "util/LoggerUtil.h"
#include <iostream>
#include <memory>
#include <sstream>
#include "glog/logging.h"


// 函数：检查目录是否存在，如果不存在则创建
bool LoggerUtil::CreateDirectoryIfNotExists(const std::string& dir)
{
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        // 目录不存在，创建目录
        if (mkdir(dir.c_str(), 0777) == -1) {
            std::cerr << "Error creating directory: " << dir << std::endl;
            return false;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        // 路径存在，但不是目录
        std::cerr << "Path exists but is not a directory: " << dir << std::endl;
        return false;
    }
    return true;
}

void LoggerUtil::Init(const std::string& program_name, const std::string& log_dir, bool also_log_to_stderr)
{
    FLAGS_log_dir = log_dir;
    LoggerUtil::CreateDirectoryIfNotExists(log_dir);
    google::InitGoogleLogging(program_name.c_str());
    FLAGS_alsologtostderr = also_log_to_stderr;
    FLAGS_minloglevel = google::GLOG_INFO;
    FLAGS_logbufsecs = 0; // 立即写入日志
    FLAGS_timestamp_in_logfile_name = false; // 禁用日志文件名中的时间戳
    FLAGS_colorlogtostderr = true;
    FLAGS_stop_logging_if_full_disk = true;

    // 输出不同级别的日志
    LOG(INFO) << "This is an info log.";
    LOG(WARNING) << "This is a warning log.";
    LOG(ERROR) << "This is an error log.";
    // LOG(FATAL) << "This is a fatal log."; // 这会终止程序

    // 条件日志
    int num_cookies = 15;
    LOG_IF(INFO, num_cookies > 10) << "Log_if demo";

    // 每隔10次输出一次日志
    for (int i = 0; i < 30; ++i) {
        LOG_EVERY_N(INFO, 10) << "LOG_EVERY_N demo :Got " << google::COUNTER;
    }
}

void LoggerUtil::Shutdown()
{
    google::ShutdownGoogleLogging();
}

void LoggerUtil::LogInfo(const std::string& message, const char* file, int line)
{
    google::LogMessage(file, line, google::GLOG_INFO).stream() << message;
}

void LoggerUtil::LogWarning(const std::string& message, const char* file, int line)
{
    google::LogMessage(file, line, google::GLOG_WARNING).stream() << message;
}

void LoggerUtil::LogError(const std::string& message, const char* file, int line)
{
    google::LogMessage(file, line, google::GLOG_ERROR).stream() << message;
}



#endif // _LOGGERUTIL_CPP_