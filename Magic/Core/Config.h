#pragma once
#include <fstream>
#include <sstream>
#include "Core.h"
namespace Magic {

class Config;
class ConfigFile;
class ConfigValue;
class ConfigFormatter;

typedef std::map<std::string,MagicPtr<ConfigValue>> ConfigKeyValue;


template<class T>
std::string asString(const T& value) {
	std::ostringstream formatStream;
	formatStream << value;
	return formatStream.str();
}
template<class T>
T stringAs(const std::string& value) {
	std::stringstream formatStream;
	formatStream.clear();
	T temp;
	formatStream << value;
	formatStream >> temp;
	return temp;
}
template<>
inline std::string stringAs<std::string>(const std::string& value) {
	return value;
}
template<>
inline bool stringAs<bool>(const std::string& value) {
	bool isOk = true;
	std::string tValue = value;
	{
		auto begin = tValue.begin();
		auto end = tValue.end();
		for (; begin != end; begin++)
			*begin = std::toupper(*begin);
	}
	if (tValue == std::string("FALSE") || tValue == std::string("NO") || tValue == std::string("0"))
		isOk = false;
	return isOk;
}

class ConfigValue{
public:
    ConfigValue(const std::string& name,const std::string& value,const std::string& comment);
    const std::string& getValue() const;
    const std::string& getComment() const;
    void write(std::ostream& os);
private:
    std::string m_Name;
    std::string m_Value;
    std::string m_Comment;
};

class ConfigFormatter{
public:
    static void Parse(std::string& content,ConfigKeyValue& keyValue);

};

class ConfigFile{
public:
    ConfigFile(const std::string& path,const std::string& welcome);
    void Read(std::ostringstream& content);
    void write(MagicPtr<ConfigValue>& config);
private:
    std::string m_Path;
    std::fstream m_FileStream;
};


class Config{
public:
    Config(){};
    void addConfigFile(MagicPtr<ConfigFile>& configFile);
    void reBuild();
    template<class T>
    T at(const std::string& defaultName,const T& defaultValue,const std::string& defaultComment=""){
        auto iter = m_ConfigMap.find(defaultName);
        if(iter != m_ConfigMap.end()){
            return stringAs<T>(iter->second->getValue());
        }

        MagicPtr<ConfigValue> value(new ConfigValue(defaultName,asString<T>(defaultValue),defaultComment));
		m_ConfigFile->write(value);
        m_ConfigMap[defaultName] = std::move(value);
        return stringAs<T>(m_ConfigMap[defaultName]->getValue());
    }

private:
    MagicPtr<ConfigFile> m_ConfigFile;
    ConfigKeyValue m_ConfigMap;
 };
}