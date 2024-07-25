#ifndef _LOGGERUTIL_H_
#define _LOGGERUTIL_H_

// #define GLOG_NO_ABBREVIATED_SEVERITIES
// #define GLOG_USE_GLOG_EXPORT
#include <glog/logging.h>


#include <string>
#include <sys/stat.h>
#include <sys/types.h>

class LoggerUtil {
public:
    // 函数：检查目录是否存在，如果不存在则创建
    static bool CreateDirectoryIfNotExists(const std::string& dir);

    static void Init(const std::string& program_name, const std::string& log_dir = "./logs", bool also_log_to_stderr = true);
    static void Shutdown();

    static void LogInfo(const std::string& message, const char* file, int line);
    static void LogWarning(const std::string& message, const char* file, int line);
    static void LogError(const std::string& message, const char* file, int line);

    template <typename... Args>
    static void LogInfo(const char* file, int line, const char* format, Args... args)
    {
        std::string message = FormatString(format, args...);
        google::LogMessage(file, line, google::GLOG_INFO).stream() << message;
    }

    template <typename... Args>
    static void LogWarning(const char* file, int line, const char* format, Args... args)
    {
        std::string message = FormatString(format, args...);
        google::LogMessage(file, line, google::GLOG_WARNING).stream() << message;
    }

    template <typename... Args>
    static void LogError(const char* file, int line, const char* format, Args... args)
    {
        std::string message = FormatString(format, args...);
        google::LogMessage(file, line, google::GLOG_ERROR).stream() << message;
    }

private:
    template <typename... Args>
    static std::string FormatString(const std::string& format, Args... args)
    {
        size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
        if (size <= 0) {
            throw std::runtime_error("Error during formatting.");
        }
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
};

// 宏定义，简化日志调用并保留文件名和行号
#define LOG_INFO(message) LoggerUtil::LogInfo(message, __FILE__, __LINE__)
#define LOG_WARNING(message) LoggerUtil::LogWarning(message, __FILE__, __LINE__)
#define LOG_ERROR(message) LoggerUtil::LogError(message, __FILE__, __LINE__)

#define LOG_INFO_FORMAT(format, ...) LoggerUtil::LogInfo(__FILE__, __LINE__, format, __VA_ARGS__)
#define LOG_WARNING_FORMAT(format, ...) LoggerUtil::LogWarning(__FILE__, __LINE__, format, __VA_ARGS__)
#define LOG_ERROR_FORMAT(format, ...) LoggerUtil::LogError(__FILE__, __LINE__, format, __VA_ARGS__)

#endif // _LOGGERUTIL_H_