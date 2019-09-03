#pragma once
#include <string>
#include <cassert>
#include "Util.h"
#include "../Log/Log.h"

#define MAGIC_ASSERT(exp,msg) \
	if(!(exp)) { \
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "ASSERTION: " << #exp \
			<< "\n" << msg \
			<< "\nBackTrace:\n" \
			<< Magic::BackTraceToString(); \
		assert(exp); \
	}
