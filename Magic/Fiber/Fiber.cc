#include "Fiber.h"
#include <atomic>
#include "../Util/Assert.h"

using namespace Magic;
static auto& g_log = MAGIC_LOG_ROOT();
static std::atomic<uint64_t> g_fiberId(0);
static std::atomic<uint64_t> g_fiberCount(0);
static thread_local Fiber* g_currentFiber = nullptr;
static thread_local std::unique_ptr<Fiber> g_superFiber = nullptr;

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

Fiber::Fiber(std::function<void()> callBack,uint64_t stackSize):m_CallBack(callBack){

	if (!g_superFiber){
		g_superFiber.reset(new Fiber);
	}
	m_Id = g_fiberId;
	m_State = State::EXEC;
	g_fiberId++;
	g_fiberCount++;
	m_StackSize = stackSize;
	m_Stack = StackAllocator::Alloc(stackSize);
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
	m_Context.uc_link = nullptr;
	m_Context.uc_stack.ss_sp = m_Stack;
	m_Context.uc_stack.ss_size = m_StackSize;
	makecontext(&m_Context, &Fiber::MainFunc, 0);

}
Fiber::~Fiber() {
	if (m_Stack){
		MAGIC_ASSERT(m_State == State::TERM 
			|| m_State == State::INIT
			|| m_State == State::EXCEPT, "Fiber State error");
		StackAllocator::Dealloc(m_Stack);
	}
	else{
		MAGIC_ASSERT(!m_CallBack, "CallBack Func error");
		MAGIC_ASSERT(m_State == State::EXEC, "CurrentState Runing");
		auto* cur = GetCurrentFiber();
		if (cur == this){
			SetCurrentFiber(nullptr);
		}
	}
	g_fiberCount--;
}
uint64_t Fiber::GetId(){
	if (g_currentFiber){
		return g_currentFiber->getId();
	}
	return uint64_t(0);
}

Fiber::Fiber() {
	m_Id = g_fiberId;
	m_State = State::EXEC;
	g_fiberId++;
	g_fiberCount++;
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
}
void Fiber::toCall() {
	SetCurrentFiber(this);
	m_State = State::READY;
	MAGIC_ASSERT(swapcontext(&(g_superFiber->m_Context), &m_Context) == 0, "swapcontext return error(-1)")
}
void Fiber::ToBack() {
	auto* cur = GetCurrentFiber();
	cur->m_State = State::HOLD;
	SetCurrentFiber(g_superFiber.get());
	MAGIC_ASSERT(swapcontext(&(cur->m_Context), &(g_superFiber->m_Context)) == 0, "swapcontext return error(-1)")
}
uint64_t Fiber::getId(){
	return m_Id;
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
		MAGIC_LOG_ERROR(g_log) << "Fiber Except: " << exp.what();
	}
	SetCurrentFiber(g_superFiber.get());
	MAGIC_ASSERT(swapcontext(&(cur->m_Context), &(g_superFiber->m_Context)) == 0, "swapcontext return error(-1)")
} 
