#include "logLevel.h"

using namespace Magic;

const char* LogLevel::toString(LogLevel::Level level){
	switch(level){
#define LEVEL(Name)\
        case LogLevel::Name: \
            return #Name;
        
        LEVEL(DEBUG)
        LEVEL(INFO)
        LEVEL(WARN)
        LEVEL(ERROR)
        LEVEL(FATAL)
#undef LEVEL
    }
	return "<(ERROR)>";
}
