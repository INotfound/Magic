/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : TimingWheel.hpp
 * @Date           : 2023-07-03 18:30
 ******************************************************************************
 */
#pragma once

#include <list>
#include <vector>

#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Timer.hpp"
#include "Magic/Utilty/Config.hpp"

namespace Magic{
    /**
     * @brief 任务接口类
     */
    class ITaskNode:public Noncopyable{
    public:
        /**
         * @brief 析构虚函数
         */
        virtual ~ITaskNode();

        /**
         * @brief 抽象虚函数
         */
        virtual void notify() = 0;
    };

    /**
     * @brief 函数任务
     */
    class FunctionTaskNode:public ITaskNode{
    public:
        /**
         * @brief 任务构造函数
         * @param func 函数对象
         */
        explicit FunctionTaskNode(std::function<void()> func);

        /**
         * @brief 抽象虚函数
         */
        void notify() override;

    private:
        std::function<void()> m_Function;
    };

    /**
     * @brief 时间轮
     */
    class Wheel:public Noncopyable{
    public:
        /// 任务列表
        typedef std::list<Safe<ITaskNode>> TaskList;
    public:
        /**
         * @brief 构造函数
         * @param tickMs 时间轮总时间
         * @param wheelSize 时间轮轮数
         */
        Wheel(uint64_t tickMs,uint64_t wheelSize);

        /**
         * @brief 添加时间轮任务节点函数
         * @param tickMs 执行任务的时间
         * @param taskNode 需要执行的任务
         */
        void add(uint64_t tickMs,Safe<ITaskNode>& taskNode);

        /**
         * @brief 定时执行函数
         * @param taskList 任务列表
         */
        void tick(std::vector<TaskList>& taskList);

    private:
        uint64_t m_TickMs;
        std::mutex m_Mutex;
        uint64_t m_Interval;
        uint64_t m_WheelSize;
        uint64_t m_ClockHand;
        Safe<Wheel> m_NextWheel;
        std::vector<TaskList> m_Task;
    };

    /**
     * @brief 定时轮
     */
    class TimingWheel:public Noncopyable,public std::enable_shared_from_this<TimingWheel>{
    public:
        /**
         * @brief 构造函数
         */
        explicit TimingWheel(const Safe<Config>& configuration);

        /**
         * @brief 运行定时轮函数
         * @note: 必须先执行定时轮才能添加任务,可多次执行此函数
         * @see: addTask()
         */
        void run();

        /**
         * @brief 暂停定时轮函数
         */
        void stop();

        /**
         * @brief 外部模式
         */
        void externMode();

        /**
         * @brief 改变定时路时间以及函数
         * @param tickMs 需要改变的时间(毫秒)
         * @param wheelSize 需要改变的轮数
         * @warning 必须在执行运行函数之前调用
         * @see: run()
         */
        void change(uint64_t tickMs,uint64_t wheelSize);

        /**
         * @brief 添加执行任务
         * @param timeOutMs 任务超时时间(毫秒)
         * @param taskNode 任务节点-需要添加的任务
         * @warning 必须在执行运行函数之后调用
         * @see : run()
         */
        void addTask(uint64_t timeOutMs,Safe<ITaskNode>& taskNode);

    private:
        bool m_IsRun;
        uint64_t m_TickMs;
        std::mutex m_Mutex;
        Safe<Timer> m_Timer;
        Safe<Wheel> m_Wheels;
        uint64_t m_WheelSize;
    };

    extern Safe<TimingWheel> g_TimingWheel;
}
