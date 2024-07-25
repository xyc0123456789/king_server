#ifndef _JSONBUILDER_H_
#define _JSONBUILDER_H_

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

class JSONBuilder {
public:
    JSONBuilder() {
        doc_.SetObject();
        allocator_ = &doc_.GetAllocator();
    }

    JSONBuilder& AddMember(const std::string& key, const std::string& value) {
        doc_.AddMember(Value().SetString(key.c_str(), *allocator_), Value().SetString(value.c_str(), *allocator_), *allocator_);
        return *this;
    }

    JSONBuilder& AddMember(const std::string& key, int value) {
        doc_.AddMember(Value().SetString(key.c_str(), *allocator_), Value().SetInt(value), *allocator_);
        return *this;
    }

    JSONBuilder& AddMember(const std::string& key, bool value) {
        doc_.AddMember(Value().SetString(key.c_str(), *allocator_), Value().SetBool(value), *allocator_);
        return *this;
    }

    JSONBuilder& AddMember(const std::string& key, double value) {
        doc_.AddMember(Value().SetString(key.c_str(), *allocator_), Value().SetDouble(value), *allocator_);
        return *this;
    }

    std::string Build() const {
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        doc_.Accept(writer);
        return buffer.GetString();
    }

private:
    Document doc_;
    Document::AllocatorType* allocator_;
};

#endif // _JSONBUILDER_H_