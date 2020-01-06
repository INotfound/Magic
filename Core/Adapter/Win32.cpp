#if defined(_WIN32) || defined(_WIN64)
#include "Win32.h"

#include "../Util.h"
#include "../Mutex.h"
#include "../Macro.h"
#include "../Plugin.h"

namespace Magic {
	
	Plugin::~Plugin() {
		FreeLibrary(m_Handle);
	}
	Plugin::Plugin(const std::string& name, const std::string& path)
		:m_Name{ name } {
		m_Handle = LoadLibrary(path.c_str());
		MAGIC_LOG(LogLevel::LogDebug) << "Loading Plugin Module: " << path.c_str();
		if (m_Handle) {
			m_IsInstance = true;

			m_Create = reinterpret_cast<create_t>(GetProcAddress(m_Handle, "create"));
			MAGIC_ASSERT(m_Create, "Not Found Function");
		}
		else {
			m_IsInstance = false;
			MAGIC_LOG(LogLevel::LogError) << "Plugin Module: " << path.c_str();
		}
	}
	std::string Plugin::getName() const {
		return m_Name;
	}
	RWMutex::RWMutex() {
		InitializeSRWLock(&m_RWLock.lock);
	}
	RWMutex::~RWMutex() {
	}
	void RWMutex::readLock() {
		AcquireSRWLockShared(&m_RWLock.lock);
		m_RWLock.flag = WinRWLock::RW::Read;
	}
	void RWMutex::writeLock() {
		AcquireSRWLockExclusive(&m_RWLock.lock);
		m_RWLock.flag = WinRWLock::RW::Wirte;
	}
	void RWMutex::unlock() {
		switch (m_RWLock.flag)
		{
		case WinRWLock::RW::Read:
			ReleaseSRWLockShared(&m_RWLock.lock);
			break;
		case WinRWLock::RW::Wirte:
			ReleaseSRWLockExclusive(&m_RWLock.lock);
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

	uint32_t GetProcessorsNumber() {
		SYSTEM_INFO info{};
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
	}

	uint64_t GetThreadId() {
		return GetCurrentThreadId();
	}

	std::string BackTraceToString(uint32_t, uint32_t, const std::string&) {
		return "Plase use WinDbg!!!";
	}

}
#endif