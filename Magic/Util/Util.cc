#include "Util.h"
#include <sstream>
#include <unistd.h>
#include <execinfo.h>
#include <sys/syscall.h>
#include "../Log/Log.h"
#include "../Thread/Thread.h"
#include "../Fiber/Fiber.h"
static auto& g_log = MAGIC_LOG_ROOT();

void Magic::Init(){
	std::unique_ptr<ILogAppender> fileAppender(new FileLogAppender("Root.log"));
	g_log->addILogAppender(fileAppender);
	Magic::Thread::SetName("Main");
}

uint64_t Magic::GetThreadId(){
    return static_cast<uint64_t>(syscall(SYS_gettid));
}

uint64_t Magic::GetFiberId(){
    return Fiber::GetId();
}

void Magic::BackTrace(std::vector<std::string>& vec, uint32_t size, uint32_t skip){
	void** buffer = static_cast<void**>(std::malloc(sizeof(void*) * size));
	int32_t retValue = backtrace(buffer, size);
	char** string = backtrace_symbols(buffer, retValue);
	if (string == nullptr){
		MAGIC_LOG_ERROR(g_log) << "backtrace_symbols error!";
	}
	uint32_t newValue = static_cast<uint32_t>(retValue);
	for (uint32_t i = skip; i < newValue; i++){
		vec.push_back(string[i]);
	}
	free(buffer);
}

std::string Magic::BackTraceToString(uint32_t size, uint32_t skip,const std::string& prefix){
	std::vector<std::string> vecString;
	BackTrace(vecString, size, skip);
	std::stringstream ss;
	for (size_t i = 0; i <vecString.size(); i++)
	{
		ss << prefix << vecString.at(i) << std::endl;
	}
	ss << std::endl;
	return ss.str();
}

