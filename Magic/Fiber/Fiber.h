#pragma once

#include <memory>
#include <cstdint>
#include <functional>
#include <ucontext.h>
#include "../Thread/Thread.h"

namespace Magic {

class Fiber {
public:
	enum State {
		/// ³õÊ¼»¯×´Ì¬
		INIT,
		/// ÔÝÍ£×´Ì¬
		HOLD,
		/// Ö´ÐÐÖÐ×´Ì¬
		EXEC,
		/// ½áÊø×´Ì¬
		TERM,
		/// ¿ÉÖ´ÐÐ×´Ì¬
		READY,
		/// Òì³£×´Ì¬
		EXCEPT
	};
public:
	Fiber(std::function<void()> callBack, uint32_t stackSize = 0);
	~Fiber();
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
	Fiber();
	static void SetCurrentFiber(Fiber* val);
	static Fiber* GetCurrentFiber();
private:
	uint64_t m_Id =0;
	uint32_t m_StackSize =0;
	std::function<void()> m_CallBack;
	State m_State = INIT;
	ucontext_t m_Context;
	void* m_Stack = nullptr;
};

}