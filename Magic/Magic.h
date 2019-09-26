#pragma once
#include "Core/Log.h"
#include "Core/Util.h"
#include "Core/Macro.h"
#include "Core/Thread.h"

namespace Magic{

void Init(){
    Ptr<ILogAppender> logAppender(new FileLogAppender("Root.log"));
	MAGIC_LOG_ROOT()->addILogAppender(logAppender);
	Magic::Thread::SetName("Main");
}

}
