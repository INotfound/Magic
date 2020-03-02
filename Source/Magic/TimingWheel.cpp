#include "TimingWheel.h"
namespace Magic{
        ITaskNode::~ITaskNode(){
        }
namespace Instance{
        Wheel::Wheel(uint64_t tickMs,uint64_t wheelSize)
            :m_TickMs(tickMs)
            ,m_Interval(tickMs*wheelSize)
            ,m_WheelSize(wheelSize)
            ,m_Clockhand(0){
            m_Task.resize(m_WheelSize);
        }
        void Wheel::add(uint64_t ms,Safe<ITaskNode> &node){
            Mutex::Lock lock(m_Mutex);
            if(ms > m_Interval){
                if(!m_NextWheel){
                    m_NextWheel.reset(new Wheel(m_Interval,m_WheelSize));
                }
                m_NextWheel->add(ms,node);
                return;
            }
            uint64_t expiration = ms + (m_Clockhand * m_TickMs);
            m_Task[expiration / m_TickMs % m_WheelSize].push_back(std::move(node));
        }
        void Wheel::tick(std::vector<TaskList>& task){
            Mutex::Lock lock(m_Mutex);
            m_Clockhand++;
            if(m_Clockhand >= m_WheelSize){
                m_Clockhand = 0;
                if(m_NextWheel){
                    m_NextWheel->tick(task);
                }
            }
            if(m_Task[m_Clockhand].empty()){
                return;
            }
            task.push_back(std::move(m_Task[m_Clockhand]));
            m_Task[m_Clockhand].clear();
        }

        TimingWheel::TimingWheel()
            :m_IsRun(false)
            ,m_TickMs(1000)
            ,m_WheelSize(60){
        }
        void  TimingWheel::run(){
            Mutex::Lock lock(m_Mutex);
            if(m_IsRun){
                return;
            }
            m_IsRun = true;
            m_Wheels.reset(new Wheel(m_TickMs,m_WheelSize));
            m_Timer.reset(new Timer("TimingWheel",m_TickMs,[this](){
                m_Wheels->tick(m_TaskList);
                for(auto& i : m_TaskList){
                    for(auto& v : i){
                        v->notify();
                    }
                }
                m_TaskList.clear();
            }));
            m_Timer->run(); 
        }
        void  TimingWheel::stop(){
            Mutex::Lock lock(m_Mutex);
            m_IsRun = false;
            m_Timer->stop();
            m_Timer.reset();
        }
        void  TimingWheel::addTask(uint64_t timeOutMs,Safe<ITaskNode>& node){
            Mutex::Lock lock(m_Mutex);
            if(!m_Wheels){
                return;
            }
            m_Wheels->add(timeOutMs,node);
        }
        void  TimingWheel::changeTime(uint64_t tickMs,uint64_t wheelSize){
            Mutex::Lock lock(m_Mutex);
            m_TickMs = tickMs;
            m_WheelSize = wheelSize;
        }
}
}