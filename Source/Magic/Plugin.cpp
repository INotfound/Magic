#include "Plugin.h"

namespace Magic {

	PluginManager::PluginManager() {
	}
	void PluginManager::addPlugin(Safe<Plugin>& plugin) {
		m_PluginMap[plugin->getName()] = std::move(plugin);
	}
	void PluginManager::delPlugin(Safe<Plugin>& plugin) {
		m_PluginMap[plugin->getName()].release();
	}
	Safe<Plugin>& PluginManager::at(const std::string& name) {
		return m_PluginMap.at(name);
	}
	std::map<std::string, Safe<Plugin>>& PluginManager::all() {
		return m_PluginMap;
	}

}