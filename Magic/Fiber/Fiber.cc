#include "Fiber.h"
#include <atomic>
#include "../Util/Macro.h"
using namespace Magic;

static std::atomic<uint64_t> g_fiberId(0);
static std::atomic<uint64_t> g_fiberCount(0);
static thread_local Fiber* g_fiber = nullptr;
static thread_local std::unique_ptr<Fiber> g_threadFiber=nullptr;
static uint64_t g_stackSize = 1024 * 1024;

class MallocStackAllocator {
public:
	static void* Alloc(uint64_t size) {
		return std::malloc(size);
	}
	static void Dealloc(void* ptr) {
		return free(ptr);
	}
private:
};

using StackAllocator = MallocStackAllocator;

Fiber::Fiber() {
	g_fiberId++;
	g_fiberCount++;
	m_Id = g_fiberId;
	m_State = EXEC;
	SetCurrentFiber(this);
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
}

Fiber::Fiber(std::function<void()> callBack, uint32_t stackSize):m_CallBack(callBack){
	GetCurrentFiber();
	g_fiberId++;
	g_fiberCount++;
	m_Id = g_fiberId;
	m_StackSize = stackSize ? stackSize : g_stackSize;
	m_Stack = StackAllocator::Alloc(m_StackSize);
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
	m_Context.uc_link  = nullptr;
	m_Context.uc_stack.ss_sp   = m_Stack;
	m_Context.uc_stack.ss_size = m_StackSize;
	makecontext(&m_Context, &Fiber::MainFunc, 0);
}

Fiber::~Fiber(){
	g_fiberCount--;
	if (m_Stack) {
		MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "normalState : "<< INIT <<" currentState : "<<m_State << "  currentId: " << m_Id;
		MAGIC_ASSERT(m_State == TERM || m_State == INIT || m_State == EXCEPT,"Fiber State error");
		StackAllocator::Dealloc(m_Stack);
	}
	else{
		MAGIC_ASSERT(!m_CallBack,"");
		MAGIC_ASSERT(m_State == EXEC,"");
		Fiber* cur = g_fiber;
		if (cur == this) {
			SetCurrentFiber(nullptr);
		}
	}
	MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "Fiber::~Fiber id: " << m_Id;
}

void Fiber::reset(std::function<void()> callBack){
	MAGIC_ASSERT(m_Stack,"Stack is nullptr");
	MAGIC_ASSERT(m_State == TERM || m_State == INIT,"Fiber State error");
	m_CallBack = callBack;
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
	m_Context.uc_link = nullptr;
	m_Context.uc_stack.ss_sp = m_Stack;
	m_Context.uc_stack.ss_size = m_StackSize;
	makecontext(&m_Context, &Fiber::MainFunc, 0);
	m_State = INIT;
}

void Fiber::swapIn(){
	SetCurrentFiber(this);
	MAGIC_ASSERT(m_State != EXEC,"Current Fiber runing");
	m_State = EXEC;
	MAGIC_ASSERT(swapcontext(&(g_threadFiber->m_Context), &m_Context) == 0,"swapcontext error!");
}

void Fiber::swapOut(){
	SetCurrentFiber(g_threadFiber.get());
	MAGIC_ASSERT(swapcontext(&m_Context, &(g_threadFiber->m_Context)) == 0, "swapcontext error!");
}

uint64_t Magic::Fiber::getId()
{
	return m_Id;
}

void Magic::Fiber::SetCurrentFiber(Fiber* val)
{
	g_fiber = val;
}

Fiber* Magic::Fiber::GetCurrentFiber()
{
	if (g_fiber) {
		return g_fiber;
	}
	g_threadFiber.reset(new Fiber);
	MAGIC_ASSERT(g_fiber == g_threadFiber.get(),"");
	return g_threadFiber.get();
}

void Fiber::YieldToReady(){
	auto cur = GetCurrentFiber();
	cur->m_State = READY;
	cur->swapOut();
}

void Fiber::YieldToHold(){
	auto cur = GetCurrentFiber();
	cur->m_State = HOLD;
	cur->swapOut();
}

uint64_t Fiber::TotalFibers(){
	return uint64_t(g_fiberCount);
}

void Fiber::MainFunc(){
	auto cur = GetCurrentFiber();
	MAGIC_ASSERT(cur,"pointer nullptr");
	try
	{
		cur->m_CallBack();
		cur->m_CallBack = nullptr;
		cur->m_State = TERM;
	}
	catch (const std::exception& ex)
	{
		cur->m_State = EXCEPT;
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "Fiber Except: " << ex.what();
	}
}

uint64_t Magic::Fiber::GetId()
{
	if (g_fiber) {
		return g_fiber->getId();
	}
	return 0;
}
