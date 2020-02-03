#if defined(_WIN32) || defined(_WIN64)
#include "Macro.h"
#include "Plugin.h"
#include "Adapter/Win32.h"
namespace Magic{
	Plugin::~Plugin() {
		FreeLibrary(m_Handle);
	}
	Plugin::Plugin(const std::string& name, const std::string& path)
		:m_Name(name)
		,m_Create(nullptr) {
		m_Handle = LoadLibrary(path.c_str());
		MAGIC_LOG(LogLevel::LogDebug) << "Loading Plugin Module: " << path.c_str();
		if (m_Handle) {
			m_IsInstance = true;
			m_Create = reinterpret_cast<create_t>(GetProcAddress(m_Handle, "create"));
			MAGIC_ASSERT(m_Create, "Not Found Function");
		}
		else {
			m_IsInstance = false;
			MAGIC_LOG(LogLevel::LogError) << name.c_str() << ": " << path.c_str();
		}
	}
	std::string Plugin::getName() const {
		return m_Name;
	}
}
#endif