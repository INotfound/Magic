#include "Adapter.h"
#include "../Mutex.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "../Macro.h"

using namespace Magic;
static auto& g_Log = MAGIC_LOG_ROOT();

#if defined(_WIN32) || defined(_WIN64)
/***********BEGIN***********/


Semaphore::Semaphore(uint32_t count){
}

Semaphore::~Semaphore(){
}

void Semaphore::wait(){
}

void Semaphore::notify(){
}

int64_t Adapter::GetThreadIdEx(){
	GetCurrentThreadId();
}

std::string Adapter::BackTraceToString(uint32_t, uint32_t,const std::string&){
	return "";
}

/***********#END#***********/
#else
/***********BEGIN***********/


Semaphore::Semaphore(uint32_t count){
    if(sem_init(&m_Semaphore,0,count)){
        throw std::logic_error("sem_init error!");
    }
}

Semaphore::~Semaphore(){
    sem_destroy(&m_Semaphore);
}

void Semaphore::wait(){
    if(sem_wait(&m_Semaphore)){
        throw std::logic_error("Semaphore wait error!");
    }
}

void Semaphore::notify(){
    if(sem_post(&m_Semaphore)){
        throw std::logic_error("Semaphore notify error!");
    }
}


int64_t Adapter::GetThreadIdEx(){
    return syscall(SYS_gettid);
}

static void BackTrace(std::vector<std::string>& vec, uint32_t size, uint32_t skip){
	void** buffer = static_cast<void**>(std::malloc(sizeof(void*) * size));
	int32_t retValue = backtrace(buffer, size);
	char** string = backtrace_symbols(buffer, retValue);
	if (string == nullptr){
		MAGIC_LOG_ERROR(g_Log) << "backtrace_symbols error!";
	}
	uint32_t newValue = static_cast<uint32_t>(retValue);
	for (uint32_t i = skip; i < newValue; i++){
		vec.push_back(string[i]);
	}
	free(buffer);
}

std::string Adapter::BackTraceToString(uint32_t size, uint32_t skip,const std::string& prefix){
	std::vector<std::string> vecString;
	BackTrace(vecString, size, skip);
	std::stringstream ss;
	for (size_t i = 0; i <vecString.size(); i++)
	{
		ss << prefix << vecString.at(i) << std::endl;
	}
	ss << std::endl;
	return "\nBackTrace:\n" + ss.str();
}
/***********#END#***********/
#endif
