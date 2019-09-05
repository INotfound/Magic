#pragma once
#include <memory>

/*#define Use(var) \
	MAGIC_ASSERT(var, #var << " Pointer is nullptr"); \
	var
*/

#define Ptr std::unique_ptr
