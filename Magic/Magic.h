#pragma once
#include "Core/Log.h"
#include "Core/Util.h"
#include "Core/Macro.h"
#include "Core/Mutex.h"
#include "Core/Thread.h"
#include "Core/Config.h"
namespace Magic{

void Core() {
	Magic::Thread::SetName("Main");
	MagicPtr<ILogAppender> logAppender(new FileLogAppender("system.log"));
	MagicPtr<Magic::ConfigFile> configFile(new Magic::ConfigFile("config.ini","#Configuration"));
	MAGIC_LOG_ROOT()->addILogAppender(logAppender);
	MAGIC_CONFIG()->addConfigFile(configFile);
}

}
