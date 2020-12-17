/*
 * @File: Core.h
 * @Author: INotFound
 * @Date: 2020-03-13 23:05:18
 * @LastEditTime : 2020-05-07 14:59:18
 */
#pragma once
#include <map>
#include <ctime>
#include <memory>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <exception>
#include <functional>
#include "asio.hpp"

#define Safe std::unique_ptr
#define Share std::shared_ptr

//typedef asio::ip::tcp::socket Socket;
static const std::string g_EmptyString;
/**/
class Noncopyable{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
/**/
template <class T>
class Singleton{
public:
    static T* GetInstance(){
        static T v;
        return &v;
    }
};
/**/
template <class T>
class SingletonPtr{
public:
    static const Safe<T>& GetInstance(){
        static Safe<T> v(new T);
        return v;
    }
};

template<class T>
class ScopedLockImpl : public Noncopyable{
public:
    ScopedLockImpl(T& mutex)
        :m_Mutex(mutex), m_Locked(false){
        lock();
    }
    ~ScopedLockImpl(){
        unlock();
    }
private:
    void lock(){
        if(!m_Locked){
            m_Mutex.lock();
            m_Locked = true;
        }
    }
    void unlock(){
        if(m_Locked){
            m_Mutex.unlock();
            m_Locked = false;
        }
    }
private:
    T& m_Mutex;
    bool m_Locked;
};

template<class T>
class ReadScopedLockImpl : public Noncopyable{
public:
    ReadScopedLockImpl(T& mutex)
        :m_Mutex(mutex), m_Locked(false){
        lock();
    }
    ~ReadScopedLockImpl(){
        unlock();
    }
private:
    void lock(){
        if(!m_Locked){
            m_Mutex.readLock();
            m_Locked = true;
        }
    }
    void unlock(){
        if(m_Locked){
            m_Mutex.unlock();
            m_Locked = false;
        }
    }
private:
    T& m_Mutex;
    bool m_Locked;
};

template<class T>
class WriteScopedLockImpl : public Noncopyable{
public:
    WriteScopedLockImpl(T& mutex)
        :m_Mutex(mutex), m_Locked(false){
        lock();
    }
    ~WriteScopedLockImpl(){
        unlock();
    }
private:
    void lock(){
        if(!m_Locked){
            m_Mutex.writeLock();
            m_Locked = true;
        }
    }
    void unlock(){
        if(m_Locked){
            m_Mutex.unlock();
            m_Locked = false;
        }
    }
private:
    T& m_Mutex;
    bool m_Locked;
};

template<class T>
std::string AsString(const T& value) {
    std::ostringstream formatStream;
    formatStream << value;
    return formatStream.str();
}

template<class T>
T StringAs(const std::string& value) {
    std::stringstream formatStream;
    formatStream.clear();
    T temp;
    formatStream << value;
    formatStream >> temp;
    return temp;
}

template<>
inline std::string StringAs<std::string>(const std::string& value) {
    return value;
}

template<>
inline int32_t StringAs<int32_t>(const std::string& value) {
    try{
        return std::stoi(value);
    }catch(const std::exception&){
        return int32_t();
    }
}

template<>
inline int64_t StringAs<int64_t>(const std::string& value) {
    try{
        return std::stol(value);
    }catch(const std::exception&){
        return int64_t();
    }
}
template<>
inline uint32_t StringAs<uint32_t>(const std::string& value) {
    try{
        return static_cast<uint32_t>(std::stoul(value));
    }catch(const std::exception&){
        return uint32_t();
    }
}
template<>
inline uint64_t StringAs<uint64_t>(const std::string& value) {
    try{
        return static_cast<uint64_t>(std::stoul(value));
    }catch(const std::exception&){
        return uint64_t();
    }
}
template<>
inline float StringAs<float>(const std::string& value) {
    try{
        return std::stof(value);
    }catch(const std::exception&){
        return float();
    }
}
template<>
inline double StringAs<double>(const std::string& value) {
    try{
        return std::stod(value);
    }catch(const std::exception&){
        return double();
    }
}

template<>
inline bool StringAs<bool>(const std::string& value) {
    bool isOk = true;
    std::string tValue(value);
    {
        auto begin = tValue.begin();
        auto end = tValue.end();
        for (; begin != end; begin++)
            *begin = std::toupper(*begin);
    }
    if (tValue == std::string("FALSE") || tValue == std::string("NO") || tValue == std::string("0"))
        isOk = false;
    return isOk;
}

namespace Magic{
    /**
     * @brief: 获取线程Id
     * @return: 返回线程Id
     */
    uint64_t GetThreadId();
    /**
     * @brief: 获取当前时间
     * @return: 返回当前时间
     */
    uint64_t GetCurrentTimeMS();
    /**
     * @brief: 获取当前时间
     * @return: 返回当前时间
     */
    uint64_t GetCurrentTimeUS();
    /**
     * @brief: 获取处理器数量
     * @return: 返回处理器数量
     */
    uint32_t GetProcessorsNumber();
    /**
     * @brief: 获取栈信息
     * @return: 返回栈信息
     */
    std::string BackTraceToString(uint32_t size = 64, uint32_t skip = 2, const std::string& prefix = "    ");

    int32_t StringCompareNoCase(const std::string& dest,const std::string& src);

    bool ReadFileList(const std::string& basePath,std::vector<std::string>& paths);

    inline std::string TimeToString(time_t ts,const std::string& format ="%Y-%m-%d %H:%M:%S"){
        struct tm nowTime;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&nowTime, &ts);
    #else
        localtime_r(&ts, &nowTime);
    #endif
        char buf[64] = {0};
        strftime(buf, sizeof(buf), format.c_str(), &nowTime);
        return buf;
    }

    inline std::string GetTimeGMTString(std::time_t t){
        struct tm* GMTime = gmtime(&t);
        char buff[512]={0};
        strftime(buff,sizeof(buff),"%a, %d %b %Y %H:%M:%S %Z",GMTime);
        return buff;
    }

    inline std::string Trim(std::string str,const std::string flag = " ") {
        if (str.empty())
            return str;
        str.erase(0, str.find_first_not_of(" "));
        str.erase(str.find_last_not_of(" ") + 1);
        return str;
    }

    inline std::string Split(const std::string& str, uint64_t index, const std::string& flag) {
        uint64_t pos = str.find(flag, index);
        if (pos == std::string::npos)
            return str.substr(index, str.size() - index);;
        return str.substr(index, pos - index);
    }

}