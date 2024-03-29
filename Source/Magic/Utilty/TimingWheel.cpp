/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : TimingWheel.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include <utility>

#include "Magic/Utilty/TimingWheel.hpp"

namespace Magic{
    Safe<TimingWheel> g_TimingWheel;

    ITaskNode::~ITaskNode() = default;

    FunctionTaskNode::FunctionTaskNode(std::function<void()> func)
        :m_Function(std::move(func)){
    }

    void FunctionTaskNode::notify(){
        m_Function();
    }

    Wheel::Wheel(uint64_t tickMs,uint64_t wheelSize)
        :m_TickMs(tickMs)
        ,m_Interval(tickMs * wheelSize)
        ,m_WheelSize(wheelSize)
        ,m_ClockHand(0){
        m_Task.resize(m_WheelSize);
        m_Task.reserve(m_WheelSize);
    }

    void Wheel::add(uint64_t ms,Safe<ITaskNode>& node){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(ms > m_Interval){
            if(!m_NextWheel){
                m_NextWheel = std::make_shared<Wheel>(m_Interval,m_WheelSize);
            }
            m_NextWheel->add(ms,node);
            return;
        }
        ms = ms > m_TickMs ? ms : m_TickMs;
        uint64_t expiration = ms + (m_ClockHand * m_TickMs);
        m_Task[expiration / m_TickMs % m_WheelSize].emplace_back(std::move(node));
    }

    void Wheel::tick(std::vector<TaskList>& task){
        std::lock_guard<std::mutex> locker(m_Mutex);
        m_ClockHand++;
        if(m_ClockHand >= m_WheelSize){
            m_ClockHand = 0;
            if(m_NextWheel){
                m_NextWheel->tick(task);
            }
        }
        if(m_Task[m_ClockHand].empty()){
            return;
        }
        task.emplace_back(std::move(m_Task[m_ClockHand]));
        m_Task[m_ClockHand].resize(0);
    }

    TimingWheel::TimingWheel(const Safe<Config>& configuration)
        :m_IsRun(false)
        ,m_TickMs(configuration->at<uint64_t>("TimingWheel.TickMs",1000))
        ,m_WheelSize(configuration->at<uint64_t>("TimingWheel.WheelSize",60)){
    }

    void TimingWheel::run(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(m_IsRun){
            return;
        }
        m_IsRun = true;
        m_Wheels = std::make_shared<Wheel>(m_TickMs,m_WheelSize);
        m_Timer = std::make_shared<Timer>("TimingWheel",m_TickMs,[this](){
            std::vector<Wheel::TaskList> taskList;
            m_Wheels->tick(taskList);
            for(auto& i : taskList){
                for(auto& v : i){
                    v->notify();
                }
            }
        });
        m_Timer->run();
    }

    void TimingWheel::stop(){
        std::lock_guard<std::mutex> locker(m_Mutex);
        m_IsRun = false;
        m_Timer->stop();
        m_Timer.reset();
    }

    void TimingWheel::externMode(){
        if(!g_TimingWheel)
            g_TimingWheel = this->shared_from_this();
    }

    void TimingWheel::change(uint64_t tickMs,uint64_t wheelSize){
        std::lock_guard<std::mutex> locker(m_Mutex);
        m_TickMs = tickMs;
        m_WheelSize = wheelSize;
    }

    void TimingWheel::addTask(uint64_t timeOutMs,Safe<ITaskNode>& taskNode){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!m_Wheels){
            return;
        }
        m_Wheels->add(timeOutMs,taskNode);
    }
}
