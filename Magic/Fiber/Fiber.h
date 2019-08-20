#pragma once
#include <memory>
#include <cstdint>
#include <functional>
#include <ucontext.h>
#include "../Thread/Thread.h"

namespace Magic{
enum State{
	INIT,	// ³õÊ¼»¯×´Ì¬
	HOLD,	// ÔÝÍ£×´Ì¬
	EXEC,	// Ö´ÐÐÖÐ×´Ì¬
	TERM,	// ½áÊø×´Ì¬
	READY,	// ¿ÉÖ´ÐÐ×´Ì¬
	EXCEPT	// Òì³£×´Ì¬
};

class Fiber{
public:
	Fiber(std::function<void()> callBack = nullptr, uint64_t stackSize = 1024 * 1024);
	~Fiber();
	static uint64_t GetId();
	static void YieldToReady();
	static void YieldToHold();
private:
	void Call();
	void Back();
	static Fiber* GetCurrentFiber();
	static void SetCurrentFiber(Fiber* val);
	static void MainFunc();
private:
	uint64_t m_Id = 0;
	uint32_t m_StackSize = 0;
	std::function<void()> m_CallBack;
	State m_State = INIT;
	ucontext_t m_Context;
	void* m_Stack = nullptr;
};


}