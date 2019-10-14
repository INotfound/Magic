#pragma once
#include <map>
#include <string>
#include <memory>
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
        static T v;
        return &v;
    }
};
/**/
template <class T>
class SingletonPtr{
public:
    static MagicPtr<T> GetInstance(){
        static MagicPtr<T> v(new T());
        return v;
    }
};

template<class T>
class ScopedLockImpl : public Noncopyable{
public:
    ScopedLockImpl(T& mutex)
        :m_Mutex(mutex),m_Locked(false){
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
        :m_Mutex(mutex),m_Locked(false){
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
        :m_Mutex(mutex),m_Locked(false){
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

/*
** enum class State{
**	Init, //Normal State
**  InitToRun, // Invoke State
**  Run //Normal State
** };
**  Only Save [Invoke State] And [Invoke State Func]
** S : Normal State
** I : Invoke State
** F : Invoke Func
*/
template<class S,class E, class F>
class StateMachine
{
private:
	typedef struct {
		S state;
		std::function<F> function;
	}StateEvent;
public:
	StateMachine(S normal) :m_CurrentState(normal) {
	}
	~StateMachine() {}
	void invoke(const E invokeState) {
		auto& tStateEvent = m_Map[invokeState];
		m_CurrentState = tStateEvent.state;
		auto& callback = tStateEvent.function;
		if(!callback){
			throw std::logic_error("Func is nullptr");
		}
		callback();
	}
	void addFunc(const E invokeState,const S toState,const std::function<F> handleFunc) {
		auto& tStateEvent = m_Map[invokeState];
		tStateEvent.state = toState;
		tStateEvent.function = handleFunc;
	}
	S& GetState() {
		return m_CurrentState;
	}
private:
	S m_CurrentState;
	std::map<E,StateEvent> m_Map;
};