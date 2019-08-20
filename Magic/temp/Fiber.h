#pragma once

#include <memory>
#include <cstdint>
#include <functional>
#include <ucontext.h>
#include "../Thread/Thread.h"

namespace Magic {

class Fibers {
public:
	enum State {
		INIT,	// ³õÊ¼»¯×´Ì¬
		HOLD,	// ÔÝÍ£×´Ì¬
		EXEC,	// Ö´ÐÐÖÐ×´Ì¬
		TERM,	// ½áÊø×´Ì¬
		READY,	// ¿ÉÖ´ÐÐ×´Ì¬
		EXCEPT	// Òì³£×´Ì¬
	};
public:
	Fibers(std::function<void()> callBack, uint32_t stackSize = 0);
	~Fibers();
	void reset(std::function<void()> callBack);
	void swapIn();
	void swapOut();
	uint64_t getId();
	static void YieldToReady();
	static void YieldToHold();
	static uint64_t TotalFibers();
	static void MainFunc();
	static uint64_t GetId();
private:
	Fibers();
	static void SetCurrentFiber(Fibers* val);
	static Fibers* GetCurrentFiber();
private:
	uint64_t m_Id =0;
	uint32_t m_StackSize =0;
	std::function<void()> m_CallBack;
	State m_State = INIT;
	ucontext_t m_Context;
	void* m_Stack = nullptr;
};

}