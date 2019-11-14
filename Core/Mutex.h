#pragma once
#include "Core.h"
#include "Adapter/Adapter.h"

namespace Magic{

class RWMutex : public Noncopyable {
public:
	typedef ReadScopedLockImpl<RWMutex> ReadLock;
	typedef WriteScopedLockImpl<RWMutex> WriteLock;

	RWMutex();
	~RWMutex();
	void readLock();
	void writeLock();
	void unlock();
private:
	rwlock_t m_RWLock{};
};

class Spinlock : Noncopyable {
public:
	typedef ScopedLockImpl<Spinlock> Lock;

	Spinlock();

	~Spinlock();

	void lock();

	void unlock();

private:
	spinlock_t m_Mutex{};
};

class Mutex : public Noncopyable {
public:
	typedef ScopedLockImpl<Mutex> Lock;
	Mutex();
	~Mutex();
	void lock();
	void unlock();
private:
	mutex_t m_Mutex{};
};

class Semaphore : public Noncopyable{
public:
    Semaphore(uint32_t count =0);

    ~Semaphore();

    void wait();

    void notify();
private:
	sem_t m_Semaphore{};
};

typedef Magic::Mutex MutexType;
typedef Magic::Mutex::Lock MutexLock;
}