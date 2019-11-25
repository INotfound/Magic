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
		//Config
		MagicPtr<Magic::ConfigFile> configFile{
			new Magic::ConfigFile{"Config.ini","#Configuration!!!"}
		};
		MagicPtr<Magic::IConfigFormatter> configFormatter{
			new Magic::InIConfigFormatter{}
		};
		configFile->addFormatter(configFormatter);
		MAGIC_CONFIG()->addConfigFile(configFile);

		//Log
		MagicPtr<ILogAppender> logFileAppender{
			new FileLogAppender{MAGIC_CONFIG()->at<std::string>("LogPath", "System.log")}
		};
		MagicPtr<ILogAppender> logStdOutAppender{ new StdOutLogAppender };

		MAGIC_LOG_ROOT()->setFormatter(
			MAGIC_CONFIG()->at<std::string>("LogFormater", "[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%c]%T%f:%l%T%m%n")
		);

		MAGIC_LOG_ROOT()->addILogAppender(logFileAppender);
		MAGIC_LOG_ROOT()->addILogAppender(logStdOutAppender);
	}
}
