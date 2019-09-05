#pragma once
#include <memory>
#include <string>
#include <cassert>

#include "Util.h"
#include "Define.h"

#include "../Log/Log.h"

#define MAGIC_ASSERT(exp,msg) \
	if(!(exp)) { \
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "ASSERTION: " << #exp \
			<< "\n" << msg \
			<< "\nBackTrace:\n" \
			<< Magic::BackTraceToString(); \
		assert(exp); \
	}

#define Use(Var) UsePointer(Var,MAGIC_ASSERT(Var != nullptr, #Var << " => Pointer is nullptr"))
