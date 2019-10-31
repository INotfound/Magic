#pragma once
#include "Core/Log.h"
#include "Core/Util.h"
#include "Core/Macro.h"
#include "Core/Mutex.h"
#include "Core/Thread.h"
#include "Core/Config.h"
namespace Magic {
	void Init(){
		Magic::Thread::SetName("Main");
		MagicPtr<Magic::ConfigFile> configFile(
			new Magic::ConfigFile("config.ini", "#Configuration!!!")
		);
		MAGIC_CONFIG()->addConfigFile(configFile);

		MAGIC_LOG_ROOT()->setFormatter(
			MAGIC_CONFIG()->at<std::string>("LogFormater","[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%c]%T%f:%l%T%m%n")
		);
		MagicPtr<ILogAppender> logFileAppender(
			new FileLogAppender(MAGIC_CONFIG()->at<std::string>("LogRootPath", "system.log"))
		);
		MagicPtr<ILogAppender> logStdOutAppender(new StdOutLogAppender);
		MAGIC_LOG_ROOT()->addILogAppender(logFileAppender);
		MAGIC_LOG_ROOT()->addILogAppender(logStdOutAppender);
	}
}
