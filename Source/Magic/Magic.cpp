/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Magic.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include "Magic/Magic"
#include "Magic/NetWork/IoPool.hpp"

namespace Magic{
    Application::~Application(){
        if(m_Container->contain<Magic::NetWork::IoPool>()){
            m_Container->resolve<Magic::NetWork::IoPool>()->wait();
        }
    }

    Application::Application()
        :m_Container(std::make_shared<Container>()){
        Magic::Thread::SetName("Magic");
    }
}
