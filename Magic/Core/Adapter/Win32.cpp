#if defined(_WIN32) || defined(_WIN64)
#include "Win32.h"

#include "../Util.h"
#include "../Mutex.h"
#include "../Macro.h"

namespace Magic {

	static auto& g_Log = MAGIC_LOG_ROOT();

	RWMutex::RWMutex() {
		InitializeSRWLock(m_RWLock.lock);
	}
	RWMutex::~RWMutex() {
	}
	void RWMutex::readLock() {
		AcquireSRWLockShared(m_RWLock.lock);
		m_RWLock.flag = RW::Read;
	}
	void RWMutex::writeLock() {
		AcquireSRWLockExclusive(m_RWLock.lock);
		m_RWLock.flag = RW::Wirte;
	}
	void RWMutex::unlock() {
		switch (m_RWLock.flag)
		{
		case RW::Read:
			ReleaseSRWLockShared(m_RWLock.lock);
			break;
		case RW::Wirte:
			ReleaseSRWLockExclusive(m_RWLock.lock);
			break;
		}
	}
	
	Spinlock::Spinlock() {
		InitializeCriticalSectionAndSpinCount(&m_Mutex, 0);
	}

	Spinlock::~Spinlock() {
		DeleteCriticalSection(&m_Mutex);
	}

	void Spinlock::lock() {
		EnterCriticalSection(&m_Mutex);
	}

	void Spinlock::unlock() {
		LeaveCriticalSection(&m_Mutex);
	}

	Mutex::Mutex() {
		m_Mutex = CreateMutex(nullptr, false, nullptr);
	}
	Mutex::~Mutex() {
		CloseHandle(m_Mutex);
	}
	void Mutex::lock() {
		WaitForSingleObject(m_Mutex, INFINITE);
	}
	void Mutex::unlock() {
		ReleaseMutex(m_Mutex);
	}


	Semaphore::Semaphore(uint32_t count) {
		m_Semaphore = CreateSemaphore(nullptr, count, 1, nullptr);
	}

	Semaphore::~Semaphore() {
		CloseHandle(m_Semaphore);
	}

	void Semaphore::wait() {
		WaitForSingleObject(m_Semaphore, INFINITE);
	}

	void Semaphore::notify() {
		ReleaseSemaphore(m_Semaphore, 1, nullptr);
	}

	int64_t GetThreadId() {
		return GetCurrentThreadId();
	}

	std::string BackTraceToString(uint32_t, uint32_t, const std::string&) {
		return "";
	}

}
#endif

