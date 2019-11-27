#include "Config.h"
#include "nlohmann/json.hpp"
namespace Magic {
	using Json = nlohmann::json;

	Config::~Config() {
		MutexLock lock{ m_Mutex };
		if (m_IsChange){
			m_ConfigFile->close();
			std::remove(m_ConfigFile->getPath().c_str());
			m_ConfigFile->open();
			m_ConfigFile->write(m_ConfigMap);
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
	ConfigFile::ConfigFile(const std::string& path)
		:m_Path{ path } {
		this->open();
	}
	void ConfigFile::open() {
		this->m_FileStream.open(m_Path, std::ios_base::in | std::ios_base::out | std::ios_base::app);
		
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
	void ConfigFile::write(ConfigKeyValue& config) {
		m_Formatter->write(m_FileStream,config);
		m_FileStream.flush();
	}
	void ConfigFile::addFormatter(MagicPtr<IConfigFormatter>& configFormatter) {
		m_Formatter = std::move(configFormatter);
	}

	ConfigValue::ConfigValue(const std::string& name, const std::string& value, const std::string& comment)
		:m_Name{ name }, m_Value{ value }, m_Comment{ comment } {
		if (!m_Comment.empty())
			m_IsComment = true;
	}
	bool ConfigValue::isComment() const{
		return m_IsComment;
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

	void InIConfigFormatter::write(std::ostream& os, ConfigKeyValue& KeyValue) {
		auto iter{ KeyValue.begin() };
		auto end{ KeyValue.end() };
		for (; iter != end; iter++) {
			MagicPtr<ConfigValue>& value{ iter->second };
			if (!value->getComment().empty()) {
				os << "#" << value->getComment() << std::endl;
			}
			os << value->getName() << "=" << value->getValue() << std::endl;
			os << std::endl;
		}
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
	
	void JsonConfigFormatter::write(std::ostream& os, ConfigKeyValue& KeyValue) {
		MagicPtr<Json> json{ new Json{} };
		for(auto&v : KeyValue)
		{
			auto& configValue{ v.second };
			if (configValue->isComment())
				(*json)[v.first + "Comment"] = configValue->getComment();
			(*json)[v.first] = configValue->getValue();
		}
		os << (*json);
	}
	void JsonConfigFormatter::parse(const std::string& content, ConfigKeyValue& keyValue) {
		if (content.empty())
			return;
		MagicPtr<Json> json{ new Json{ Json::parse(content) } };
		auto& iter{ json->begin() };
		auto& end{ json->end() };
		for (; iter != end; iter++)
		{
			std::string keyName{ iter.key() };
			MagicPtr<ConfigValue> value{ new ConfigValue{keyName,iter.value()} };
			keyValue[keyName] = std::move(value);
		}
	}

}