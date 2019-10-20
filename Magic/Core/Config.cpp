#include "Config.h"
#include <cstdio>
#include <sstream>
#include "Adapter/Adapter.h"
namespace Magic {

	ConfigValue::ConfigValue(const std::string& name, const std::string& value, const std::string& comment)
		:m_Name(name), m_Value(value), m_Comment(comment) {
	}
	const std::string& ConfigValue::getValue() const {
		return m_Value;
	}
	const std::string& ConfigValue::getComment() const {
		return m_Comment;
	}
	void ConfigValue::write(std::ostream& os) {
		if (!m_Comment.empty())
		{
			os << std::endl << m_Comment << std::endl;
		}
		else {
			os << std::endl;
		}
		os << m_Name << '=' << m_Value;
	}

	void ConfigFormatter::Parse(const std::string& content, ConfigKeyValue& keyValue) {
		std::string normalString;
		std::string valueString;
		std::string commentString;
		bool isValue = false;
		bool isComment = false;
		uint32_t length = content.length();
		for (uint32_t i = 0; i < length; i++) {
			std::string::value_type charValue = content.at(i);
			//Comment
			if (charValue == '#') {
				isComment = true;
				commentString.append(1, charValue);
				continue;
			}
			else if (isComment == true) {
				if (charValue != '\n') {
					commentString.append(1, charValue);
					continue;
				}
				else {
					isComment = false;
					continue;
				}
			}

			//normalSting and value(n = v)
			if ((charValue == '\n' || i == (length - 1)) && isValue) {
				isValue = false;
				if (i == (length - 1) && charValue != '\n')
					valueString.append(1, charValue);
				MagicPtr<ConfigValue> value(new ConfigValue(normalString, valueString, commentString));
				keyValue[normalString] = std::move(value);
				commentString.clear();
				normalString.clear();
				valueString.clear();
				continue;
			}
			else if (isValue == true) {
				valueString.append(1, charValue);
				continue;
			}
			else if (charValue == '=') {
				isValue = true;
				continue;
			}
			//empty
			if (charValue == ' ' || charValue == '\r' || charValue == '\n')
				continue;

			normalString.append(1, charValue);
		}
	}

	ConfigFile::~ConfigFile() {
		this->close();
	}
	ConfigFile::ConfigFile(const std::string& path, const std::string& welcome)
		:m_Path(path),m_Welcome(welcome){
		this->open();
	}
	const std::string& ConfigFile::getPath() const {
		return m_Path;
	}
	void ConfigFile::read(std::ostringstream& contentStream) {
		if (this->m_FileStream.is_open()) {
			contentStream << m_FileStream.rdbuf();
		}
	}
	void ConfigFile::write(MagicPtr<ConfigValue>& config) {
		config->write(m_FileStream);
		m_FileStream.flush();
	}
	void ConfigFile::open() {
		bool isFile = false;
		if (access(m_Path.c_str(), 0) == -1) {
			isFile = true;
		}
		this->m_FileStream.open(this->m_Path, std::ios_base::in | std::ios_base::out | std::ios_base::app);
		if (isFile)
			this->m_FileStream << m_Welcome.c_str();
		this->m_FileStream.flush();
	}
	void ConfigFile::close() {
		if (this->m_FileStream.is_open()) {
			this->m_FileStream.close();
		}
	}

	Config::~Config() {
		MutexLock lock(m_Mutex);
		if (m_IsChange)
		{
			m_ConfigFile->close();
			std::remove(m_ConfigFile->getPath().c_str());
			m_ConfigFile->open();
			auto iter = m_ConfigMap.begin();
			auto end = m_ConfigMap.end();
			for (; iter != end; iter++) {
				m_ConfigFile->write(iter->second);
			}
		}
	}
	Config::Config()
		:m_IsChange(false){
	}
	void Config::addConfigFile(MagicPtr<ConfigFile>& configFile) {
		MutexLock lock(m_Mutex);
		m_ConfigFile = std::move(configFile);
		std::ostringstream oss;
		m_ConfigFile->read(oss);
		std::string content = oss.str();
		ConfigFormatter::Parse(content, m_ConfigMap);
	}

}