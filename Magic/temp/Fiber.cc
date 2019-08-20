#include "Fiber.h"
#include <atomic>
#include "../Util/Macro.h"
using namespace Magic;

static std::atomic<uint64_t> g_fiberId(0);
static std::atomic<uint64_t> g_fiberCount(0);
static thread_local Fibers* g_currentfiber = nullptr;
static thread_local std::unique_ptr<Fibers> g_superFiber=nullptr;
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

Fibers::Fibers() {
	g_fiberCount++;
	m_Id = g_fiberId;
	m_State = EXEC;
	SetCurrentFiber(this);
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
}

Fibers::Fibers(std::function<void()> callBack, uint32_t stackSize):m_CallBack(callBack){
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
	makecontext(&m_Context, &Fibers::MainFunc, 0);
}

Fibers::~Fibers(){
	g_fiberCount--;
	if (m_Stack) {
		MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "normalState : "<< INIT <<" currentState : "<<m_State << "  currentId: " << m_Id;
		MAGIC_ASSERT(m_State == TERM || m_State == INIT || m_State == EXCEPT,"Fibers State error");
		StackAllocator::Dealloc(m_Stack);
	}
	else{
		MAGIC_ASSERT(!m_CallBack,"");
		MAGIC_ASSERT(m_State == EXEC,"");
		Fibers* cur = g_currentfiber;
		if (cur == this) {
			SetCurrentFiber(nullptr);
		}
	}
	MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "Fibers::~Fibers id: " << m_Id;
}

void Fibers::reset(std::function<void()> callBack){
	MAGIC_ASSERT(m_Stack,"Stack is nullptr");
	MAGIC_ASSERT(m_State == TERM || m_State == INIT,"Fibers State error");
	m_CallBack = callBack;
	MAGIC_ASSERT(getcontext(&m_Context) == 0, "getcontext return error(-1)");
	m_Context.uc_link = nullptr;
	m_Context.uc_stack.ss_sp = m_Stack;
	m_Context.uc_stack.ss_size = m_StackSize;
	makecontext(&m_Context, &Fibers::MainFunc, 0);
	m_State = INIT;
}

void Fibers::swapIn(){
	SetCurrentFiber(this);
	MAGIC_ASSERT(m_State != EXEC,"Current Fibers runing");
	m_State = EXEC;
	MAGIC_ASSERT(swapcontext(&(g_superFiber->m_Context), &m_Context) == 0,"swapcontext error!");
}

void Fibers::swapOut(){
	SetCurrentFiber(g_superFiber.get());
	MAGIC_ASSERT(swapcontext(&m_Context, &(g_superFiber->m_Context)) == 0, "swapcontext error!");
}

uint64_t Magic::Fibers::getId()
{
	return m_Id;
}

void Magic::Fibers::SetCurrentFiber(Fibers* val)
{
	g_currentfiber = val;
}

Fibers* Magic::Fibers::GetCurrentFiber()
{
	if (g_currentfiber) {
		return g_currentfiber;
	}
	g_superFiber.reset(new Fibers);
	MAGIC_ASSERT(g_currentfiber == g_superFiber.get(),"");
	return g_superFiber.get();
}

void Fibers::YieldToReady(){
	auto cur = GetCurrentFiber();
	cur->m_State = READY;
	cur->swapOut();
}

void Fibers::YieldToHold(){
	auto cur = GetCurrentFiber();
	cur->m_State = HOLD;
	cur->swapOut();
}

uint64_t Fibers::TotalFibers(){
	return uint64_t(g_fiberCount);
}

void Fibers::MainFunc(){
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
		MAGIC_LOG_ERROR(MAGIC_LOG_ROOT()) << "Fibers Except: " << ex.what();
	}
	cur->swapOut();
}

uint64_t Magic::Fibers::GetId()
{
	if (g_currentfiber) {
		return g_currentfiber->getId();
	}
	return 0;
}
