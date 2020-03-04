#include "Crypto.h"
#include "TimingWheel.h"
#include "Http/Session.h"
namespace Magic{
namespace Http{
    Session::~Session(){
    }
    Session::Session(bool autoGen)
        :m_LastAccessTime(std::time(0)){
        if(autoGen){
            std::stringstream ss;
            ss << GetCurrentTimeUS() << '|' << std::rand() << '|' << std::rand() << '|' << std::rand() << '|' << std::rand();
            m_Id = MD5HexString(ss.str());
        }
    }
    const std::string& Session::getId(){
        RWMutex::ReadLock lock(m_Mutex);
        return m_Id;
    }
    void Session::setId(const std::string& id){
        RWMutex::WriteLock lock(m_Mutex);
        m_Id = id;
    }
    uint64_t Session::getLastAccessTime(){
        RWMutex::ReadLock lock(m_Mutex);
        return m_LastAccessTime;
    }
    void Session::setLastAccessTime(uint64_t value){
        RWMutex::WriteLock lock(m_Mutex);
        m_LastAccessTime = value;
    }

    class SessionTimeOutTask :public ITaskNode{
        public:
            SessionTimeOutTask(const std::string& id);
            void notify() override;
        private:
            std::string m_Id;
    };
    SessionTimeOutTask::SessionTimeOutTask(const std::string& id)
        :m_Id(id){
    }
    void SessionTimeOutTask::notify(){
        SessionMgr::GetInstance()->del(m_Id);
    }
namespace Instance{

    SessionManager::SessionManager()
        :m_TimeOutMs(1200000){
    }
    void SessionManager::del(const std::string& id){
        RWMutex::WriteLock lock(m_Mutex);
        auto value = m_Data.find(id);
        if(value == m_Data.end()){
            return;
        }
        m_Data.erase(value);
    }
    void SessionManager::add(Safe<Session>& session){
        RWMutex::WriteLock lock(m_Mutex);
        TimingWheel::GetInstance()->run();
        std::string id = session->getId();
        Safe<ITaskNode> node(new SessionTimeOutTask(id));
        TimingWheel::GetInstance()->addTask(m_TimeOutMs,node);
        m_Data.emplace(id, std::move(session));
    }
    void SessionManager::setTimeOut(uint64_t timeOutMs){
        m_TimeOutMs = timeOutMs;
    }
    const Safe<Session>& SessionManager::get(const std::string& key){ 
        static const Safe<Session> emptySession;
        RWMutex::ReadLock lock(m_Mutex);
        auto value = m_Data.find(key);
        if(value != m_Data.end()){
            return value->second;
        }
        return emptySession;
    }
}
}
}