#pragma once

#include <memory>
#include <atomic>
#include <cstdint>
#include <functional>
#include <ucontext.h>
#include "../Util/Macro.h"
#include "../Thread/Thread.h"

namespace Magic{



class Fiber{

public:
	enum State{
		INIT,	// 初始
		READY,	// 准备
		EXEC,	// 执行
		HOLD,	// 停止
		TERM,	// 结束
		EXCEPT	// 异常
	};
	~Fiber();
	Fiber(std::function<void()> callBack,bool useCaller,uint64_t stackSize = 1024 * 1024);

	
	void toCall();
	void toBack();
	void swapIn();
	void swapOut();
	State getState();
	void reset(std::function<void()> callBack);
	static uint64_t GetId();
	static Ptr<Fiber>& Init();
	static void YieldToHold();
	static void YieldToReady();

protected:
	Fiber();
	uint64_t getId();
	static Fiber* GetCurrentFiber();
	static void SetCurrentFiber(Fiber* val);
	static void MainFunc();
	static void CallerMainFunc();
private:
	uint64_t m_Id = 0;
	uint32_t m_StackSize = 0;
	std::function<void()> m_CallBack;
	State m_State = State::INIT;
	ucontext_t m_Context;
	void* m_Stack = nullptr;
};

}
