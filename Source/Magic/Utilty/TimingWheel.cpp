/*
 * @Author: INotFound
 * @Date: 2020-03-13 21:04:59
 * @LastEditTime: 2021-02-01 22:26:12
 */
#include <utility>

#include "Magic/Utilty/TimingWheel.h"

namespace Magic{
    ITaskNode::~ITaskNode() =default;

    FunctionTaskNode::FunctionTaskNode(std::function<void()> func)
        :m_Function(std::move(func)){
    }

    void FunctionTaskNode::notify(){
        m_Function();
    }

    Wheel::Wheel(uint64_t tickMs,uint64_t wheelSize)
        :m_TickMs(tickMs)
        ,m_Interval(tickMs*wheelSize)
        ,m_WheelSize(wheelSize)
        ,m_ClockHand(0){
        m_Task.resize(m_WheelSize);
        m_Task.reserve(m_WheelSize);
    }

    void Wheel::add(uint64_t ms,Safe<ITaskNode>& node){
        Mutex::Lock lock(m_Mutex);
        if(ms > m_Interval){
            if(!m_NextWheel){
                m_NextWheel = std::make_shared<Wheel>(m_Interval,m_WheelSize);
            }
            m_NextWheel->add(ms,node);
            return;
        }
        uint64_t expiration = ms + (m_ClockHand * m_TickMs);
        m_Task[expiration / m_TickMs % m_WheelSize].push_back(std::move(node));
    }

    void Wheel::tick(std::vector<TaskList>& task){
        Mutex::Lock lock(m_Mutex);
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
        task.push_back(std::move(m_Task[m_ClockHand]));
        m_Task[m_ClockHand].clear();
    }

    TimingWheel::TimingWheel(const Safe<Config>& configuration)
        :m_IsRun(false)
        ,m_TickMs(configuration->at<uint64_t>("TimingWheel.TickMs",100))
        ,m_WheelSize(configuration->at<uint64_t>("TimingWheel.WheelSize",10)){
    }

    void  TimingWheel::run(){
        Mutex::Lock lock(m_Mutex);
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

    void  TimingWheel::stop(){
        Mutex::Lock lock(m_Mutex);
        m_IsRun = false;
        m_Timer->stop();
        m_Timer.reset();
    }

    void  TimingWheel::change(uint64_t tickMs,uint64_t wheelSize){
        Mutex::Lock lock(m_Mutex);
        m_TickMs = tickMs;
        m_WheelSize = wheelSize;
    }

    void  TimingWheel::addTask(uint64_t timeOutMs,Safe<ITaskNode>& taskNode){
        Mutex::Lock lock(m_Mutex);
        if(!m_Wheels){
            return;
        }
        m_Wheels->add(timeOutMs,taskNode);
    }
}
