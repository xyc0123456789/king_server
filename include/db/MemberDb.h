#ifndef _MEMBERDB_H_
#define _MEMBERDB_H_

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include "sqlite_orm/sqlite_orm.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "util/JsonUtil.h"
#include "util/UniqueIdGenerator.h"

using namespace sqlite_orm;

const int64_t LEVEL_BASE = 10000;

struct MemberDTO
{
    int64_t id;
    std::string name;
    std::string email;
    std::string password;
    int64_t level;
    std::string ext_data;
    int64_t ctime;
    int64_t utime;
    bool enable;
};

class MemberService
{
public:
    static MemberService &getInstance()
    {
        static MemberService instance;
        return instance;
    }

    static std::string serializeMember(const MemberDTO &member)
    {
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

        document.AddMember("id", member.id, allocator);
        document.AddMember("name", rapidjson::Value(member.name.c_str(), allocator), allocator);
        document.AddMember("email", rapidjson::Value(member.email.c_str(), allocator), allocator);
        document.AddMember("password", rapidjson::Value(member.password.c_str(), allocator), allocator);
        document.AddMember("level", member.level, allocator);
        document.AddMember("ext_data", rapidjson::Value(member.ext_data.c_str(), allocator), allocator);
        document.AddMember("ctime", member.ctime, allocator);
        document.AddMember("utime", member.utime, allocator);
        document.AddMember("enable", member.enable, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        return buffer.GetString();
    }

    static std::string serializeMemberVector(const std::vector<MemberDTO> &members)
    {
        rapidjson::Document document;
        document.SetArray(); // 设置为数组而不是对象
        rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

        for (const auto &member : members)
        {
            rapidjson::Value memberValue(rapidjson::kObjectType);
            memberValue.AddMember("id", member.id, allocator);
            memberValue.AddMember("name", rapidjson::Value(member.name.c_str(), allocator), allocator);
            memberValue.AddMember("email", rapidjson::Value(member.email.c_str(), allocator), allocator);
            memberValue.AddMember("password", rapidjson::Value(member.password.c_str(), allocator), allocator);
            memberValue.AddMember("level", member.level, allocator);
            memberValue.AddMember("ext_data", rapidjson::Value(member.ext_data.c_str(), allocator), allocator);
            memberValue.AddMember("ctime", member.ctime, allocator);
            memberValue.AddMember("utime", member.utime, allocator);
            memberValue.AddMember("enable", member.enable, allocator);
            document.PushBack(memberValue, allocator);
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        return buffer.GetString();
    }

    static std::optional<MemberDTO> deserializeMember(const std::string &jsonString)
    {
        rapidjson::Document document;
        if (document.Parse(jsonString.c_str()).HasParseError())
        {
            std::cerr << "JSON parse error at offset " << document.GetErrorOffset() << std::endl;
            return std::nullopt;
        }

        if (!document.IsObject())
        {
            return std::nullopt;
        }

        MemberDTO member{};

        if (!(JsonUtil::get_if(document, "id", member.id) &&
              JsonUtil::get_if(document, "name", member.name) &&
              JsonUtil::get_if(document, "email", member.email) &&
              JsonUtil::get_if(document, "password", member.password) &&
              JsonUtil::get_if(document, "level", member.level) &&
              JsonUtil::get_if(document, "ext_data", member.ext_data) &&
              JsonUtil::get_if(document, "ctime", member.ctime) &&
              JsonUtil::get_if(document, "utime", member.utime) &&
              JsonUtil::get_if(document, "enable", member.enable)))
        {
            return std::nullopt;
        }

        return member;
    }

    void init()
    {
        storage.sync_schema();
    }

    void addMember(const MemberDTO &member)
    {
        storage.insert(member);
    }

    void addMember(const std::string& name, const std::string& email, const std::string& password)
    {
        MemberDTO member;
        member.id = UniqueIdGenerator::generateUniqueId();
        member.name = name;
        member.email = email;
        member.password = password;
        member.level = LEVEL_BASE;
        member.ext_data = "{}";
        member.ctime = static_cast<int64_t>(std::time(nullptr));
        member.utime = static_cast<int64_t>(std::time(nullptr));
        member.enable = true;

        storage.insert(member);
    }


    std::vector<MemberDTO> getAllMembers()
    {
        return storage.get_all<MemberDTO>();
    }

    std::optional<MemberDTO> getMemberById(int64_t id)
    {
        return storage.get_optional<MemberDTO>(id);
    }

    std::optional<MemberDTO> getMemberByName(std::string name)
    {
        auto results = storage.get_all<MemberDTO>(sqlite_orm::where(sqlite_orm::c(&MemberDTO::name) == name));

        // Check if any results were found
        if (!results.empty()) {
            return results.front(); // Return the first result
        } else {
            return std::nullopt; // Return an empty optional if no results were found
        }
    }

    void updateMember(const MemberDTO &member)
    {
        storage.update(member);
    }

    void updateMemberByName(const std::string& name, const MemberDTO& updatedMember)
    {
        auto members = storage.get_all<MemberDTO>(sqlite_orm::where(sqlite_orm::c(&MemberDTO::name) == name));
        if (!members.empty())
        {
            auto& member = members.front();
            member.email = updatedMember.email;
            member.password = updatedMember.password;
            member.level = updatedMember.level;
            member.ext_data = updatedMember.ext_data;
            member.ctime = updatedMember.ctime;
            member.utime = updatedMember.utime;
            member.enable = updatedMember.enable;
            storage.update(member);
        }
    }

    void deleteMember(int64_t id)
    {
        storage.remove<MemberDTO>(id);
    }

    void deleteMemberByName(const std::string& name)
    {
        auto members = storage.get_all<MemberDTO>(sqlite_orm::where(sqlite_orm::c(&MemberDTO::name) == name));
        for (const auto& member : members)
        {
            storage.remove<MemberDTO>(member.id);
        }
    }

private:
    MemberService() : storage(make_storage("db.sqlite",
                                           make_table("members",
                                                      make_column("id", &MemberDTO::id, primary_key().autoincrement()),
                                                      make_column("name", &MemberDTO::name),
                                                      make_column("email", &MemberDTO::email),
                                                      make_column("password", &MemberDTO::password),
                                                      make_column("ext_data", &MemberDTO::ext_data),
                                                      make_column("ctime", &MemberDTO::ctime),
                                                      make_column("utime", &MemberDTO::utime),
                                                      make_column("enable", &MemberDTO::enable))))
    {
        init();
    }

    MemberService(const MemberService &) = delete;
    MemberService &operator=(const MemberService &) = delete;

    decltype(make_storage("db.sqlite",
                          make_table("members",
                                     make_column("id", &MemberDTO::id, primary_key().autoincrement()),
                                     make_column("name", &MemberDTO::name),
                                     make_column("email", &MemberDTO::email),
                                     make_column("password", &MemberDTO::password),
                                     make_column("ext_data", &MemberDTO::ext_data),
                                     make_column("ctime", &MemberDTO::ctime),
                                     make_column("utime", &MemberDTO::utime),
                                     make_column("enable", &MemberDTO::enable)))) storage;
};

#endif // _MEMBERDB_H_