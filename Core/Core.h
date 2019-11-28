#pragma once
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <stdint.h>
#include <functional>

#define MagicPtr std::unique_ptr

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
		static T v{};
        return &v;
    }
};
/**/
template <class T>
class SingletonPtr{
public:
    static MagicPtr<T> GetInstance(){
		static MagicPtr<T> v{ new T{} };
        return v;
    }
};

template<class T>
class ScopedLockImpl : public Noncopyable{
public:
    ScopedLockImpl(T& mutex)
		:m_Mutex{ mutex }, m_Locked{ false }{
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
	T& m_Mutex{};
	bool m_Locked{ false };
};

template<class T>
class ReadScopedLockImpl : public Noncopyable{
public:
    ReadScopedLockImpl(T& mutex)
		:m_Mutex{ mutex }, m_Locked{ false }{
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
	T& m_Mutex{};
	bool m_Locked{ false };
};

template<class T>
class WriteScopedLockImpl : public Noncopyable{
public:
    WriteScopedLockImpl(T& mutex)
		:m_Mutex{ mutex }, m_Locked{ false }{
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
	T& m_Mutex{};
	bool m_Locked{ false };
};

template<class T>
std::string AsString(const T& value) {
	std::ostringstream formatStream{};
	formatStream << value;
	return formatStream.str();
}

template<class T>
T StringAs(const std::string& value) {
	std::stringstream formatStream{};
	formatStream.clear();
	T temp{};
	formatStream << value;
	formatStream >> temp;
	return temp;
}

template<>
inline std::string StringAs<std::string>(const std::string& value) {
	return value;
}

template<>
inline bool StringAs<bool>(const std::string& value) {
	bool isOk{ true };
	std::string tValue{ value };
	{
		auto &begin = tValue.begin();
		auto &end = tValue.end();
		for (; begin != end; begin++)
			*begin = std::toupper(*begin);
	}
	if (tValue == std::string{ "FALSE" } || tValue == std::string{ "NO" } || tValue == std::string{ "0" })
		isOk = false;
	return isOk;
}