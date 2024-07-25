#ifndef _JSONUTIL_H_
#define _JSONUTIL_H_

#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <type_traits>

template <typename T>
struct ValueExtractor
{
    static bool Extract(rapidjson::Value &v, T &out)
    {
        // Default implementation that does nothing
        return false;
    }
};

template <>
struct ValueExtractor<int64_t>
{
    static bool Extract(rapidjson::Value &v, int64_t &out)
    {
        if (v.IsInt64())
        {
            out = v.GetInt64();
            return true;
        }
        return false;
    }
};

template <>
struct ValueExtractor<std::string>
{
    static bool Extract(rapidjson::Value &v, std::string &out)
    {
        if (v.IsString())
        {
            out = v.GetString();
            return true;
        }
        return false;
    }
};

template <>
struct ValueExtractor<bool>
{
    static bool Extract(rapidjson::Value &v, bool &out)
    {
        if (v.IsBool())
        {
            out = v.GetBool();
            return true;
        }
        return false;
    }
};

class JsonUtil
{
public:
    template <typename T>
    static bool get_if(rapidjson::Value &v, const char *key, T &out)
    {
        if (v.HasMember(key))
        {
            return ValueExtractor<T>::Extract(v[key], out);
        }
        return false;
    }

    static std::string ConvertToJSONString(const rapidjson::Document& doc) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }

private:
};

#endif // _JSONUTIL_H_