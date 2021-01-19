/*
 * @Author: INotFound
 * @Date: 2020-10-21 19:35:46
 * @LastEditTime: 2020-10-24 00:03:15
 */
#include "Core/TimingWheel.h"
#include "Utilty/CryptoUtils.h"
#include "NetWork/Http/HttpSession.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief: Http会话超时类
     */
    class HttpSessionTimeOutTask :public ITaskNode{
        public:
            HttpSessionTimeOutTask(const std::string& id,const Safe<HttpSessionManager>& sessionMgr);
            void notify() override;
        private:
            std::string m_Id;
            Safe<HttpSessionManager> m_SessionMgr;
    };

    HttpSession::~HttpSession(){
    }

    HttpSession::HttpSession(bool autoGen)
        :m_LastAccessTime(std::time(0)){
        if(autoGen){
            std::stringstream ss;
            ss << GetCurrentTimeUS() << '|' << std::rand() << '|' << std::rand() << '|' << std::rand() << '|' << std::rand();
            m_Id = MD5HexString(ss.str());
        }
    }

    const std::string& HttpSession::getId(){
        RWMutex::ReadLock lock(m_Mutex);
        return m_Id;
    }

    void HttpSession::setId(const std::string& id){
        RWMutex::WriteLock lock(m_Mutex);
        m_Id = id;
    }
    
    uint64_t HttpSession::getLastAccessTime(){
        RWMutex::ReadLock lock(m_Mutex);
        return m_LastAccessTime;
    }

    void HttpSession::setLastAccessTime(uint64_t value){
        RWMutex::WriteLock lock(m_Mutex);
        m_LastAccessTime = value;
    }

    HttpSessionTimeOutTask::HttpSessionTimeOutTask(const std::string& id,const Safe<HttpSessionManager>& sessionMgr)
        :m_Id(id)
        ,m_SessionMgr(sessionMgr){
    }

    void HttpSessionTimeOutTask::notify(){
        m_SessionMgr->del(m_Id);
    }

    HttpSessionManager::HttpSessionManager(const Safe<Config>& configuration)
        :m_TimeOutMs(configuration->at<uint64_t>("NetWork.HttpSession.TimeOutMs",1200000)){
    }
    
    void HttpSessionManager::del(const std::string& id){
        RWMutex::WriteLock lock(m_Mutex);
        auto value = m_Data.find(id);
        if(value == m_Data.end()){
            return;
        }
        m_Data.erase(value);
    }

    void HttpSessionManager::setTimeOut(uint64_t timeOutMs){
        m_TimeOutMs = timeOutMs;
    }

    void HttpSessionManager::add(Safe<HttpSession>& httpSession){
        RWMutex::WriteLock lock(m_Mutex);
        m_TimingWheel->run();
        std::string id = httpSession->getId();
        Safe<ITaskNode> node = std::make_shared<HttpSessionTimeOutTask>(id,this->shared_from_this());
        m_TimingWheel->addTask(m_TimeOutMs,node);
        m_Data.emplace(id, httpSession);
    }
    
    const Safe<HttpSession>& HttpSessionManager::get(const std::string& key){ 
        static const Safe<HttpSession> emptySession;
        RWMutex::ReadLock lock(m_Mutex);
        auto value = m_Data.find(key);
        if(value != m_Data.end()){
            return value->second;
        }
        return emptySession;
    }

    void HttpSessionManager::setTimingWheel(const Safe<TimingWheel>& timingWheel){
        m_TimingWheel = timingWheel;
    }
}
}
}