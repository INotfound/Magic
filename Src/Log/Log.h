#include "logger.h"
#include "logWrap.h"
#include "logLevel.h"
#include "logEvent.h"
#include "logAppender.h"
#include "logFormatter.h"
#include "../Util/Util.h"

#define  MAGIC_LOG_LEVEL(logger,level) \
                if(level >= logger->getLevel()) \
                    Magic::LogWrap(logger,level,std::make_shared<Magic::LogEvent>(__LINE__,time(0),1,0,Magic::getThreadId(),__FILE__,logger->getLogName(),"threadName")).get()
                
#define MAGIC_LOG_DEBUG(logger) MAGIC_LOG_LEVEL(logger,Magic::LogLevel::DEBUG)

#define MAGIC_LOG_INFO(logger) MAGIC_LOG_LEVEL(logger,Magic::LogLevel::INFO)

#define MAGIC_LOG_WARN(logger) MAGIC_LOG_LEVEL(logger,Magic::LogLevel::WARN)

#define MAGIC_LOG_ERROR(logger) MAGIC_LOG_LEVEL(logger,Magic::LogLevel::ERROR)

#define MAGIC_LOG_FATAL(logger) MAIGC_LOG_LEVEL(logger,Magic::LogLevel::FATAL)
