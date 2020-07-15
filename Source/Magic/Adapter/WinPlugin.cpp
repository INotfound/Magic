/*
 * @Author: YHY
 * @Date: 2020-04-08 13:50:27
 * @LastEditTime: 2020-07-09 11:58:01
 */ 
#if defined(_WIN32) || defined(_WIN64)
#include "Macro.h"
#include "Plugin.h"
#include "Adapter/Win32.h"
namespace Magic{

    Plugin::~Plugin() {
        FreeLibrary(m_Handle); //判断是否为null 不是才执行此句话
    }
    Plugin::Plugin(const std::string& name, const std::string& path)
        :m_Name(name)
        ,m_Create(nullptr) {
        m_Handle = LoadLibrary(path.c_str());
        MAGIC_DEBUG() << "Loading Plugin Module: " << path.c_str();
        if (m_Handle) {
            m_IsInstance = true;
            m_Create = reinterpret_cast<create_t>(GetProcAddress(m_Handle, "create"));
            MAGIC_ASSERT(m_Create, "Not Found Function");
        }
        else {
            m_IsInstance = false;
            MAGIC_ERROR() << name.c_str() << ": " << path.c_str();
        }
    }
    std::string Plugin::getName() const {
        return m_Name;
    }
}
#endif