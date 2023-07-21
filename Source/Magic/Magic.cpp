/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Magic.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include "Magic/Magic"
#include "Magic/NetWork/IoPool.hpp"
#include "Magic/Core/StringView.hpp"

static Magic::StringView g_Logo = R"(
  _____ ______       ________      ________      ___      ________
 |\   _ \  _   \    |\   __  \    |\   ____\    |\  \    |\   ____\
 \ \  \\\__\ \  \   \ \  \|\  \   \ \  \___|    \ \  \   \ \  \___|
  \ \  \\|__| \  \   \ \   __  \   \ \  \  ___   \ \  \   \ \  \
   \ \  \    \ \  \   \ \  \ \  \   \ \  \|\  \   \ \  \   \ \  \____
    \ \__\    \ \__\   \ \__\ \__\   \ \_______\   \ \__\   \ \_______\
     \|__|     \|__|    \|__|\|__|    \|_______|    \|__|    \|_______|
     @INotFound https://github.com/INotfound/Magic
)";

namespace Magic{
    Application::~Application(){
        if(m_Container->contain<Magic::NetWork::IoPool>()){
            m_Container->resolve<Magic::NetWork::IoPool>()->wait();
        }
    }

    Application::Application()
        :m_Container(std::make_shared<Container>()){
        Magic::Thread::SetName("Magic");
        std::printf("%s",g_Logo.data());
    }
}
