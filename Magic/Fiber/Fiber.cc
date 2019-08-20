#include "Fiber.h"
#include <atomic>
#include "../Util/Macro.h"

using namespace Magic;
static std::atomic<uint64_t> g_fiberId(0);
static std::atomic<uint64_t> g_fiberCount(0);
static thread_local Fiber* g_currentFiber = nullptr;

class MallocStackAllocator {
public:
	static void* Alloc(uint64_t size) {
		return std::malloc(size);
	}
	static void Dealloc(void* ptr) {
		return free(ptr);
	}
};
using StackAllocator = MallocStackAllocator;

Fiber::Fiber(std::function<void()> callBack,uint64_t stackSize){
	m_Id = g_fiberId;
	m_State = State::EXEC;
	g_fiberId++;
	g_fiberCount++;
	if (!callBack && !g_currentFiber){
		SetCurrentFiber(this);
		MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
		return;
	}
	m_StackSize = stackSize;
	m_Stack = StackAllocator::Alloc(stackSize);
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
	m_Context.uc_link = &(g_currentFiber->m_Context);
	m_Context.uc_stack.ss_sp = m_Stack;
	m_Context.uc_stack.ss_size = m_StackSize;
	makecontext(&m_Context, &Fiber::MainFunc, 0);
	SetCurrentFiber(this);
}
Fiber::~Fiber() {
	if (m_Stack){
		MAGIC_ASSERT(m_State == State::TERM 
			|| m_State == State::INIT
			|| m_State == State::EXCEPT, "Fiber State error");
		StackAllocator::Dealloc(m_Stack);
	}
	else{
		MAGIC_ASSERT(!m_CallBack, "SuperFiber error");
		MAGIC_ASSERT(m_State == State::EXEC, "CurrentState Runing");
		auto* cur = GetCurrentFiber();
		if (cur == this){
			SetCurrentFiber(nullptr);
		}
	}
	g_fiberCount--;
}
uint64_t Fiber::GetId()
{
	return uint64_t();
}
void Fiber::YieldToReady() {
	auto* cur = GetCurrentFiber();
	cur->m_State = State::READY;
	cur->Back();
}
void Fiber::YieldToHold() {
	auto* cur = GetCurrentFiber();
	cur->m_State = State::HOLD;
	cur->Back();
}

void Fiber::Call() {
	auto* super = m_Context.uc_link;
	MAGIC_ASSERT(swapcontext(super, &m_Context) == 0, "swapcontext return error(-1)")
}
void Fiber::Back() {
	auto* super = m_Context.uc_link;
	MAGIC_ASSERT(swapcontext(&m_Context, super) == 0, "swapcontext return error(-1)")
}

Fiber* Fiber::GetCurrentFiber() {
	if (g_currentFiber){
		return g_currentFiber;
	}
	MAGIC_ASSERT(g_currentFiber, "Global Current Fiber is nullptr");
	return nullptr;
}
void Fiber::SetCurrentFiber(Fiber *val) {
	g_currentFiber = val;
}
void Fiber::MainFunc() {
	auto* cur = GetCurrentFiber();
	try
	{
		cur->m_CallBack();
		cur->m_CallBack = nullptr;
		cur->m_State = State::TERM;
	}
	catch (const std::exception& exp)
	{
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "Fiber Except: " << exp.what();
	}
	cur->Back();
}