#pragma once
#include <unordered_map>
#include "Any.h"
#include "Core.h"
#include "Mutex.h"
#include "Session.h"
namespace Magic{
namespace Http{
    class Session{
    public:
        ~Session();
        Session(bool autoGen =true);
        const std::string& getId();
        void setId(const std::string& id);
        uint64_t getLastAccessTime();
        void setLastAccessTime(uint64_t value);

        template<class T>
        const T& getData(const std::string& key){
            RWMutex::ReadLock lock(m_Mutex);
            static const T empty = T();
            auto value = m_Data.find(key);
            if(value == m_Data.end()){
                return empty;
            }
            //Here may throw exceptions
            return value->second->AnyCast<T>();
        }
        template<class T>
        void setData(const std::string& key,const T& value){
            RWMutex::WriteLock lock(m_Mutex);
            m_Data.emplace(key,value);
        }
    private:
        RWMutex m_Mutex;
        std::string m_Id;
        uint64_t m_LastAccessTime;
        std::unordered_map<std::string,Safe<Any>> m_Data;
    };

    class SessionManager{
    public:
        void del(const std::string& id);
        void add(Safe<Session>& session);
        void check(uint64_t time = 3600);
        const Safe<Session>& get(const std::string& key);
    private:
        RWMutex m_Mutex;
        std::unordered_map<std::string,Safe<Session>> m_Data;
    };
    typedef Singleton<SessionManager> SessionMgr;
}
}