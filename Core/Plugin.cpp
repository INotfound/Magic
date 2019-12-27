#include "Plugin.h"

namespace Magic {

	PluginManager::PluginManager() {
	}
	void PluginManager::addPlugin(MagicPtr<Plugin>& plugin) {
		m_PluginMap[plugin->getName()] = std::move(plugin);
	}
	void PluginManager::delPlugin(MagicPtr<Plugin>& plugin) {
		m_PluginMap[plugin->getName()].release();
	}
	MagicPtr<Plugin>& PluginManager::at(const std::string& name) {
		return m_PluginMap.at(name);
	}
	std::map<std::string, MagicPtr<Plugin>>& PluginManager::all() {
		return m_PluginMap;
	}

}