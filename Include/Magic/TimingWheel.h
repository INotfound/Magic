#pragma once
#include <list>
#include <vector>

#include "Core.h"
#include "Timer.h"
#include "Mutex.h"

namespace Magic{
    class ITaskNode{
    public:
        virtual ~ITaskNode();
        virtual void notify() = 0;
    };
namespace Instance{
    class Wheel{
    public:
        typedef std::list<Safe<ITaskNode>> TaskList;
    public:
        Wheel(uint64_t tickMs,uint64_t wheelSize);
        void add(uint64_t ms,Safe<ITaskNode> &node);
        void tick(std::vector<TaskList>& task);
    private:
        Mutex m_Mutex;
        uint64_t m_TickMs;
        uint64_t m_Interval;
        uint64_t m_WheelSize;
        uint64_t m_Clockhand;
        Safe<Wheel> m_NextWheel;
        std::vector<TaskList> m_Task;
    };

    class TimingWheel{
    public:
        TimingWheel();
        void run();
        void stop();
        void changeTime(uint64_t tickMs,uint64_t wheelSize);
        void addTask(uint64_t timeOutMs,Safe<ITaskNode>& node);

    private:
        bool m_IsRun;
        Mutex m_Mutex;
        uint64_t m_TickMs;
        uint64_t m_WheelSize;
        Safe<Timer> m_Timer;
        Safe<Wheel> m_Wheels;
        std::vector<Wheel::TaskList> m_TaskList;
    };
}
    typedef SingletonPtr<Instance::TimingWheel> TimingWheel;
}