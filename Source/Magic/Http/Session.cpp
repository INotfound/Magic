#include "Md5.h"
#include "Http/Session.h"

namespace Magic{
namespace Http{
    Session::Session(bool autoGen)
        :m_LastAccessTime(time(0)){
        if(autoGen){
            Md5 md5;
            std::stringstream ss;
            ss << GetCurrentTimeUS() << '|' << std::rand() << '|' << std::rand() << '|' << std::rand() << '|' << std::rand();
            m_Id = md5.getString(ss.str().c_str());
        }
    }
    const std::string& Session::getId() const{
        RWMutex::ReadLock lock(m_Mutex);
        return g_EmptyString;
    }
    void Session::setId(const std::string& id){
        RWMutex::WriteLock lock(m_Mutex);
        m_Id = id;
    }
    uint64_t Session::getLastAccessTime() const{
        RWMutex::ReadLock lock(m_Mutex);
        return m_LastAccessTime;
    }
    void Session::setLastAccessTime(uint64_t value){
        RWMutex::WriteLock lock(m_Mutex);
        m_LastAccessTime = value;
    }

    void SessionManager::del(const std::string& id){
        RWMutex::WriteLock lock(m_Mutex);
        auto value = m_Data.find(key);
        if(value == m_Data.end()){
            return;
        }
        m_Data.erase(value);
    }
    void SessionManager::add(Safe<Session>& session){
        RWMutex::WriteLock lock(m_Mutex);
        const std::string id = session->getId();
        m_Data.emplace(id, std::move(session));
    }
    void SessionManager::check(uint64_t time){
        uint64_t now = time(0) - time;
        std::vector<std::string> keys;
        {
            RWMutex::ReadLock lock(m_Mutex);
            for(auto &v : m_Data){
                if(v.second->getLastAccessTime() < now){
                    keys.push_back(v.first);
                }
            }
        }
        for(auto &v: keys){
            this->del(v);
        }
    }
    Safe<Session>& SessionManager::get(const std::string& key){ 
        RWMutex::ReadLock lock(m_Mutex);
        auto value = m_Data.find(key);
        if(value != m_Data.end()){
            return value->second;
        }
        return m_EmptySession;
    }
}
}