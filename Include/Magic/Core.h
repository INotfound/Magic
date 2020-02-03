#pragma once
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <stdint.h>
#include <algorithm>
#include <functional>

#include "asio.hpp"

#define Safe std::unique_ptr
#define Share std::shared_ptr

typedef asio::ip::tcp::socket Socket; 
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
    static Safe<T> GetInstance(){
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
	return std::stoi(value);
}

template<>
inline int64_t StringAs<int64_t>(const std::string& value) {
	return std::stol(value);
}
template<>
inline uint32_t StringAs<uint32_t>(const std::string& value) {
	return static_cast<uint32_t>(std::stoul(value));
}
template<>
inline uint64_t StringAs<uint64_t>(const std::string& value) {
	return static_cast<uint64_t>(std::stoul(value));
}
template<>
inline float StringAs<float>(const std::string& value) {
	return std::stof(value);
}
template<>
inline double StringAs<double>(const std::string& value) {
	return std::stod(value);
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
    uint64_t GetThreadId();

    uint32_t GetProcessorsNumber();

    std::string BackTraceToString(uint32_t size = 64, uint32_t skip = 2, const std::string& prefix = "    ");

    int32_t StringCompareNoCase(const std::string& dest,const std::string& src);
}