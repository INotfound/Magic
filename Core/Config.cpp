#include "Config.h"

namespace Magic {

	Config::~Config() {
		MutexLock lock{ m_Mutex };
		if (m_IsChange){
			auto iter{ m_ConfigMap.begin() };
			auto end{ m_ConfigMap.end() };

			m_ConfigFile->close();
			std::remove(m_ConfigFile->getPath().c_str());
			m_ConfigFile->open();

			for (; iter != end; iter++){
				m_ConfigFile->write(iter->second);
			}
		}
	}
	Config::Config() {
	}
	void Config::addConfigFile(MagicPtr<ConfigFile>& configFile) {
		MutexLock lock{ m_Mutex };
		m_ConfigFile = std::move(configFile);
		m_ConfigFile->read(m_ConfigMap);
	}

	ConfigFile::~ConfigFile() {
		this->close();
	}
	ConfigFile::ConfigFile(const std::string& path, const std::string& welcome)
		:m_Path{ path }, m_Welcome{ welcome } {
		this->open();
	}
	void ConfigFile::open() {
		bool isFile{ false };
		if (access(m_Path.c_str(), 0) == -1)
			isFile = true;
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
	const std::string& ConfigFile::getPath() const {
		return m_Path;
	}
	void ConfigFile::read(ConfigKeyValue& keyValue) {
		std::ostringstream content{};
		if (this->m_FileStream.is_open()) {
			content << m_FileStream.rdbuf();
			m_Formatter->parse(content.str(), keyValue);
		}
	}
	void ConfigFile::write(MagicPtr<ConfigValue>& config) {
		m_Formatter->write(config,m_FileStream);
		m_FileStream.flush();
	}
	void ConfigFile::addFormatter(MagicPtr<IConfigFormatter>& configFormatter) {
		m_Formatter = std::move(configFormatter);
	}

	ConfigValue::ConfigValue(const std::string& name, const std::string& value, const std::string& comment)
		:m_Name{ name }, m_Value{ value }, m_Comment{ comment } {

	}
	const std::string& ConfigValue::getName() const {
		return m_Name;
	}
	const std::string& ConfigValue::getValue() const {
		return m_Value;
	}
	const std::string& ConfigValue::getComment() const {
		return m_Comment;
	}

	void InIConfigFormatter::parse(const std::string& content, ConfigKeyValue& keyValue) {
		std::string normalString{};
		std::string valueString{};
		std::string commentString{};
		bool isEmpty{ true };
		bool isValue{ false };
		bool isComment{ false };
		uint64_t length{ content.length() };
		for (uint64_t i{0}; i < length; i++){
			std::string::value_type charValue{ content.at(i) };
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
			// normal and value
			if ((charValue == '\n' || i == (length - 1)) && isValue) {
				isEmpty = true;
				isValue = false;
				if (i == (length - 1) && charValue != '\n')
					valueString.append(1, charValue);
				MagicPtr<ConfigValue> value{ new ConfigValue{normalString,valueString,commentString} };
				keyValue[normalString] = std::move(value);
				commentString.clear();
				normalString.clear();
				valueString.clear();
				continue;
			}
			else if (isValue == true) {
				if (!(charValue == ' ' && isEmpty)) {
					isEmpty = false;
					valueString.append(1, charValue);
				}
				continue;
			}
			else if (charValue == '=') {
				isValue = true;
				continue;
			}
			//empty
			if (charValue == ' ' || charValue == '\r' || charValue == '\n') {
				continue;
			}
			normalString.append(1, charValue);
		}
	}
	void InIConfigFormatter::write(MagicPtr<ConfigValue>& configValue, std::ostream& os) {
		os << std::endl;
		if (!configValue->getComment().empty()) {
			os << std::endl << "#" << configValue->getComment() << std::endl;
		}
		else {
			os << std::endl;
		}
		os << configValue->getName() << "=" << configValue->getValue();
	}
}