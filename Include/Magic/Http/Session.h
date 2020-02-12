#pragma once
#include <unordered_map>
#include "Core.h"
#include "Mutex.h"
#include "Session.h"
namespace Magic{
namespace Http{
    class Session{
    public:
        ~Session();
        Session(bool autoGen =true);
        const std::string& getId() const;
        void setId(const std::string& id);
        uint64_t getLastAccessTime() const;
        void setLastAccessTime(uint64_t value);
    private:
        RWMutex m_Mutex;
        std::string m_Id;
        uint64_t m_LastAccessTime;
    };

    class SessionManager{
    public:
        void del(const std::string& id);
        void add(Safe<Session>& session);
        void check(uint64_t time = 3600);
        Safe<Session>& get(const std::string& key);
    private:
        RWMutex m_Mutex;
        Safe<Session> m_EmptySession;
        std::unordered_map<std::string,Safe<Session>> m_Data;
    };
}
}