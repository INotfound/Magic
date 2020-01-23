#pragma once
#include "Core.h"
#include "Adapter.h"
#include <functional>

namespace Magic{
    class Fiber{
    public:
        enum class State{
            INIT,
            HOLD,
            EXEC,
            TERM,
            READY
        };
        ~Fiber();
        Fiber(std::function<void()> callBack,bool useSelf = false,uint64_t stackSize = 1024*1024);
        void call();
        void back();
        void swapIn();
        void swapOut();
        void reset(std::function<void()> callBack);

        static Fiber* Root();
        static Fiber* GetThis();
        static uint64_t GetId(); 
        static void YieldToHold();
        static void YieldToReady();
        static uint64_t TotalFiber();
        static void SetThis(Fiber * fiber); 
    private:
        Fiber();
        static void MainFunc();
        static void MainCallerFunc();
    private:
        uint64_t m_Id{};
        fiber_t  m_Context{};        
        uint64_t m_StackSize{};
        void *   m_Stack{nullptr};        
        State    m_State{State::INIT};
        std::function<void()> m_CallBack{};
    };
}