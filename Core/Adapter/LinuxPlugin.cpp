#if defined(linux) || defined(__linux__)
#include "Linux.h"
#include "../Macro.h"
#include "../Plugin.h"

namespace Magic{
    Plugin::~Plugin() {
		dlclose(m_Handle);
	}
	Plugin::Plugin(const std::string& name, const std::string& path)
		:m_Name{ name } {
		m_Handle = dlopen(path.c_str(), RTLD_LAZY);
		MAGIC_LOG(LogLevel::LogDebug) << "Loading Plugin Module: " << name.c_str();
		if (m_Handle) {
			m_IsInstance = true;
			m_Create = reinterpret_cast<create_t>(dlsym(m_Handle, "create"));
			MAGIC_ASSERT(m_Create, "Not Found Function");
		}
		else {
			m_IsInstance = false;
			MAGIC_LOG(LogLevel::LogError) << dlerror();
		}
	}
	std::string Plugin::getName() const {
		return m_Name;
	}
}
#endif