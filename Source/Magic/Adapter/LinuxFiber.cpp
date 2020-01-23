#if defined(linux) || defined(__linux__)
#include <atomic>
#include "Fiber.h"
#include "Macro.h"
#include "Scheduler.h"
#include "Adapter/Linux.h"

namespace Magic{
    static std::atomic<uint64_t> g_FiberId{0};
    static std::atomic<uint64_t> g_FiberCount{0};
    static MagicPtr<Fiber> g_RootFiber{nullptr};
    static thread_local Fiber* g_MainFiber{nullptr};
    static thread_local Fiber* g_ThreadFiber{nullptr};


    class StackAllocator{
        public:
        static void* Alloc(uint64_t size){
            return std::malloc(size);
        }
        static void Dealloc(void* vp,uint64_t size){
            return free(vp);
        }
    };
 
    Fiber::~Fiber(){
        g_FiberCount--;
        if(m_Stack){
            MAGIC_ASSERT(m_State == State::TERM
                || m_State == State::INIT,"State Error");
            StackAllocator::Dealloc(m_Stack, m_StackSize);
        }else{
            MAGIC_ASSERT(!m_CallBack,"CallBack Error");
            MAGIC_ASSERT(m_State == State::EXEC,"State Error");
            Fiber* currentFiber = g_ThreadFiber;
            if(currentFiber == this){
                SetThis(nullptr);
            }
        }
        MAGIC_LOG(LogLevel::LogDebug) << "Fiber::~Fiber Id: " << m_Id;
    }
    Fiber::Fiber(std::function<void()> callBack,bool useSelf,uint64_t stackSize)
        :m_StackSize{stackSize}
        ,m_CallBack{callBack}{
            
        g_FiberId++;
        m_Id = g_FiberId;
        m_Stack = StackAllocator::Alloc(m_StackSize);
        MAGIC_ASSERT(!getcontext(&m_Context), "getcontext");
        m_Context.uc_link = nullptr;
        m_Context.uc_stack.ss_sp = m_Stack;
        m_Context.uc_stack.ss_size = m_StackSize;
        if(useSelf){
            makecontext(&m_Context, &Fiber::MainCallerFunc, 0);
        }else{
            makecontext(&m_Context, &Fiber::MainFunc, 0);
        }
        m_State = State::INIT;
    }

    void Fiber::call() {
        SetThis(this);
        m_State = State::EXEC;
        MAGIC_ASSERT(!swapcontext(&g_MainFiber->m_Context, &m_Context), "swapcontext"); 
    }

    void Fiber::back() {
        SetThis(g_MainFiber);
        MAGIC_ASSERT(!swapcontext(&m_Context, &g_MainFiber->m_Context), "swapcontext");  
    }

    void Fiber::swapIn(){
        SetThis(this);
        m_State = State::EXEC;
        MAGIC_ASSERT(!swapcontext(&Scheduler::GetMainFiber()->m_Context, &m_Context), "swapcontext");  
    }
    void Fiber::swapOut(){
        SetThis(Scheduler::GetMainFiber());
        MAGIC_ASSERT(!swapcontext(&m_Context, &Scheduler::GetMainFiber()->m_Context), "swapcontext");  
    }

    void Fiber::reset(std::function<void()> callBack){
        MAGIC_ASSERT(m_Stack,"Stack Error");
        MAGIC_ASSERT(m_State == State::TERM
            || m_State == State::INIT,"State Error");
        m_CallBack = callBack; 
        MAGIC_ASSERT(!getcontext(&m_Context), "getcontext");       
        m_Context.uc_link = nullptr;
        m_Context.uc_stack.ss_sp = m_Stack;
        m_Context.uc_stack.ss_size = m_StackSize;
        makecontext(&m_Context, &Fiber::MainFunc, 0);
        m_State = State::INIT;
    }

    Fiber* Fiber::Root(){
        if(g_ThreadFiber){
            return g_ThreadFiber;
        }
        if(!g_RootFiber.get()){
            g_RootFiber.reset(new Fiber);
            g_MainFiber = g_RootFiber.get();
            MAGIC_LOG(LogLevel::LogDebug) << "Root";
        }
        return g_MainFiber;
    }

    Fiber* Fiber::GetThis(){
        return g_ThreadFiber;
    }
    uint64_t Fiber::GetId(){
        if(g_ThreadFiber){
            return g_ThreadFiber->m_Id;
        }
        return 0;
    }
    void Fiber::YieldToHold(){
        Fiber* currentFiber = GetThis();
        currentFiber->m_State = State::HOLD;
        currentFiber->swapOut();
    }
    void Fiber::YieldToReady(){
        Fiber* currentFiber = GetThis();
        currentFiber->m_State = State::READY;
        currentFiber->swapOut();
    }
    uint64_t Fiber::TotalFiber(){
        return g_FiberCount;
    }

    void Fiber::SetThis(Fiber * fiber){
        g_ThreadFiber = fiber;
    }
    Fiber::Fiber(){
        SetThis(this);        
        m_State = State::EXEC;
        MAGIC_ASSERT(!getcontext(&m_Context), "getcontext");
        g_FiberCount++;
    }
    void Fiber::MainFunc(){
        Fiber* currentFiber = GetThis();
        MAGIC_ASSERT(currentFiber, "nullptr");
        currentFiber->m_CallBack();
        currentFiber->m_CallBack = nullptr;
        currentFiber->m_State = State::TERM;
        currentFiber->swapOut();
    }
    void Fiber::MainCallerFunc(){
        Fiber* currentFiber = GetThis();
        MAGIC_ASSERT(currentFiber, "nullptr");
        currentFiber->m_CallBack();
        currentFiber->m_CallBack = nullptr;
        currentFiber->m_State = State::TERM;
        currentFiber->back();
    }
}
#endif