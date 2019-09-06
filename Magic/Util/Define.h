#pragma once
#include <memory>
#define Ptr std::unique_ptr
#ifndef RELEASE

#define UsePointer(Var,Func) \
	Func; \
	(*Var)

#else

#define UsePointer(Var,Func) \
	(*Var)

#endif
