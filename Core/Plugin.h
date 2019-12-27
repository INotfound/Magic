#pragma once
#include "Core.h"
#include "Adapter.h"

/*

class IPlugin {
public:
	~IPlugin();
};

extern "C"
{
	_declspec(dllexport) void* Create()
	{
		return new IPlugin;
	}
}

*/
namespace Magic {

	typedef void* (*create_t)();
	class Plugin {
	public:
		~Plugin();
		Plugin(const std::string& name,const std::string& path);
		std::string getName() const;
		template<class T>
		MagicPtr<T> getInstance() {
			return MagicPtr<T>(reinterpret_cast<T*>(m_Create()));
		}
	private:
		plugin_t m_Handle{};
		create_t m_Create{};
		bool m_IsInstance{};
		std::string m_Name{};
	};
	class PluginManager {
	public:
		PluginManager();
		void addPlugin(MagicPtr<Plugin>&);
		void delPlugin(MagicPtr<Plugin>&);
		MagicPtr<Plugin>& at(const std::string& name);
		std::map<std::string, MagicPtr<Plugin>>& all();
	private:
		std::map<std::string, MagicPtr<Plugin>> m_PluginMap;
	};

}