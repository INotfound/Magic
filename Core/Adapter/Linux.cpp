#if defined(linux) || defined(__linux__)
#include "Linux.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "../Util.h"
#include "../Mutex.h"
#include "../Macro.h"
#include "../Plugin.h"

namespace Magic {
	Plugin::~Plugin() {
		dlclose(m_Handle);
	}
	Plugin::Plugin(const std::string& name, const std::string& path)
		:m_Name(name) {
		m_Handle = dlopen(name.c_str(), RTLD_LAZY);
		MAGIC_LOG(LogLevel::LogInfo) << "Loading Plugin Module: " << name.c_str();
		if (m_Handle) {
			m_IsInstance = true;
			m_Create = reinterpret_cast<create_t>(dlsym(m_Handle, "Create"));
			MAGIC_ASSERT(m_Create, "Not Found Function");
		}
		else {
			m_IsInstance = false;
			MAGIC_LOG(LogLevel::LogError) << "Plugin Module: " << name.c_str();
		}
	}
	std::string Plugin::getName() const {
		return m_Name;
	}
	RWMutex::RWMutex() {
		pthread_rwlock_init(&m_RWLock, nullptr);
	}
	RWMutex::~RWMutex() {
		pthread_rwlock_destroy(&m_RWLock);
	}
	void RWMutex::readLock() {
		pthread_rwlock_rdlock(&m_RWLock);
	}
	void RWMutex::writeLock() {
		pthread_rwlock_wrlock(&m_RWLock);
	}
	void RWMutex::unlock() {
		pthread_rwlock_unlock(&m_RWLock);
	}

	Spinlock::Spinlock() {
		pthread_spin_init(&m_Mutex, PTHREAD_PROCESS_PRIVATE);
	}
	Spinlock::~Spinlock() {
		pthread_spin_destroy(&m_Mutex);
	}
	void Spinlock::lock() {
		pthread_spin_lock(&m_Mutex);
	}
	void Spinlock::unlock() {
		pthread_spin_unlock(&m_Mutex);
	}

	Mutex::Mutex() {
		pthread_mutex_init(&m_Mutex, nullptr);
	}
	Mutex::~Mutex() {
		pthread_mutex_destroy(&m_Mutex);
	}
	void Mutex::lock() {
		pthread_mutex_lock(&m_Mutex);
	}
	void Mutex::unlock() {
		pthread_mutex_unlock(&m_Mutex);
	}

	Semaphore::Semaphore(uint32_t count) {
		if (sem_init(&m_Semaphore, 0, count)) {
			throw std::logic_error("sem_init error!");
		}
	}
	Semaphore::~Semaphore() {
		sem_destroy(&m_Semaphore);
	}
	void Semaphore::wait() {
		if (sem_wait(&m_Semaphore)) {
			throw std::logic_error("Semaphore wait error!");
		}
	}
	void Semaphore::notify() {
		if (sem_post(&m_Semaphore)) {
			throw std::logic_error("Semaphore notify error!");
		}
	}


	uint32_t GetProcessorsNumber() {
		unsigned int count{ 1 };
		count = sysconf(_SC_NPROCESSORS_CONF);
		return count;
	}

	uint64_t GetThreadId() {
		return syscall(SYS_gettid);
	}

	static void BackTrace(std::vector<std::string>& vec, uint32_t size, uint32_t skip) {
		void** buffer = static_cast<void**>(std::malloc(sizeof(void*) * size));
		int32_t retValue{ backtrace(buffer, size) };
		char** string = backtrace_symbols(buffer, retValue);
		if (string == nullptr) {
			MAGIC_LOG(Magic::LogLevel::LogError) << "backtrace_symbols error!";
		}
		uint32_t newValue{ static_cast<uint32_t>(retValue) };
		for (uint32_t i{ skip }; i < newValue; i++) {
			vec.push_back(string[i]);
		}
		free(buffer);
	}

	std::string BackTraceToString(uint32_t size, uint32_t skip, const std::string& prefix) {
		std::vector<std::string> vecString{};
		BackTrace(vecString, size, skip);
		std::stringstream ss{};
		for (size_t i{ 0 }; i < vecString.size(); i++)
		{
			ss << prefix << vecString.at(i) << std::endl;
		}
		ss << std::endl;
		return "\nBackTrace:\n" + ss.str();
	}
}
#endif
