#include "Util.h"
#include "Fiber.h"

uint64_t Magic::GetFiberId(){
	return Fiber::GetId();
}