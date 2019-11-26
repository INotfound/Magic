#pragma once
#include <sstream>
#include <fstream>

#include "Core.h"
#include "Mutex.h"

namespace Magic {
	class Config;
	class ConfigFile;
	class ConfigValue;
	class IConfigFormatter;
	class InIConfigFormatter;

	typedef std::map<std::string, MagicPtr<ConfigValue>> ConfigKeyValue;

	class Config{
	public:
		~Config();
		Config();
		void addConfigFile(MagicPtr<ConfigFile>& configFile);
		template<class T>
		T at(const std::string& defaultName, const T& defaultValue, const std::string defaultComment = "") {
			MutexLock lock{ m_Mutex };
			auto iter{ m_ConfigMap.find(defaultName) };
			if (iter != m_ConfigMap.end()) {
				return StringAs<T>(iter->second->getValue());
			}
			//New Config Item
			m_IsChange = true;
			MagicPtr<ConfigValue> value{ new ConfigValue{ defaultName, AsString<T>(defaultValue), defaultComment } };
			m_ConfigMap[defaultName] = std::move(value);
			return defaultValue;
		}
		template<class T>
		T revise(const std::string& defaultName, const T& defaultValue, const std::string& defaultComment = "") {
			MutexLock lock{ m_Mutex };
			m_IsChange = true;
			MagicPtr<ConfigValue> value{ new ConfigValue{defaultName,AsString<T>(defaultValue),defaultComment } };
			m_ConfigMap[defaultName].reset();
			m_ConfigMap[defaultName] = std::move(value);
			return defaultValue;
		}

	private:
		MutexType m_Mutex{};
		bool m_IsChange{ false };
		ConfigKeyValue m_ConfigMap{};
		MagicPtr<ConfigFile> m_ConfigFile{};
	};

	class ConfigFile {
	public:
		~ConfigFile();
		ConfigFile(const std::string& path);
		void open();
		void close();
		const std::string& getPath() const;
		void read(ConfigKeyValue& keyValue);
		void write(ConfigKeyValue& config);
		void addFormatter(MagicPtr<IConfigFormatter>& configFormatter);
	private:
		std::string m_Path{};
		std::fstream m_FileStream{};
		MagicPtr<IConfigFormatter> m_Formatter{};
	};

	class ConfigValue {
	public:
		ConfigValue(const std::string& name, const std::string& value, const std::string& comment);
		const std::string& getName() const;
		const std::string& getValue() const;
		const std::string& getComment() const;
	private:
		std::string m_Name{};
		std::string m_Value{};
		std::string m_Comment{};
	};

	class IConfigFormatter {
	public:
		~IConfigFormatter() {};
		virtual void write(std::ostream& os, ConfigKeyValue& KeyValue) = 0;
		virtual void parse(const std::string& content, ConfigKeyValue& keyValue) = 0;
	};

	class InIConfigFormatter :public IConfigFormatter {
	public:
		void write(std::ostream& os, ConfigKeyValue& KeyValue) override;
		void parse(const std::string& content, ConfigKeyValue& keyValue) override;
	};
}