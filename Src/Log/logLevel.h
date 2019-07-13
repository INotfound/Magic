#pragma once

namespace Magic{
class LogLevel{
public:
enum Level{
	DEBUG 	=1,
	INFO	=2,
	WARN	=3,
	ERROR	=4,
	FATAL	=5
};
static const char* toString(LogLevel::Level level);
};

}