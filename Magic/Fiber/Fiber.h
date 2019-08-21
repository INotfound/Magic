#pragma once
#include <memory>
#include <cstdint>
#include <functional>
#include <ucontext.h>
#include "../Thread/Thread.h"

namespace Magic{
enum State{
	INIT,	// 初始
	HOLD,	// 停止
	EXEC,	// 执行
	TERM,	// 结束
	READY,	// 准备
	EXCEPT	// 异常
};

class Fiber{
public:
	Fiber(std::function<void()> callBack = nullptr, uint64_t stackSize = 1024 * 1024);
	~Fiber();
	static uint64_t GetId();
	static void YieldToCall();
	static void YieldToBack();
private:
	void Init();
	void call();
	void back();
	uint64_t getId();
	static Fiber* GetCurrentFiber();
	static void SetCurrentFiber(Fiber* val);
	static void MainFunc();
private:
	uint64_t m_Id = 0;
	uint32_t m_StackSize = 0;
	std::function<void()> m_CallBack;
	State m_State = State::INIT;
	ucontext_t m_Context;
	void* m_Stack = nullptr;
};


}