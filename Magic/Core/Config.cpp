#include "Config.h"
#include <sstream>

namespace Magic{
    ConfigValue::ConfigValue(const std::string& name,const std::string& value,const std::string& comment)
        :m_Name(name),m_Value(value),m_Comment(comment){
    }
    const std::string& ConfigValue::getValue() const{
        return m_Value;
    }
    const std::string& ConfigValue::getComment() const{
        return m_Comment;
    }
    void ConfigValue::write(std::ostream& os){
        os << m_Comment << std::endl;
        os << m_Name << '=' << m_Value << std::endl;
    }

    void ConfigFormatter::Parse(std::string& content,ConfigKeyValue& keyValue){
        std::string normalString;
        std::string valueString;
        std::string commentString;
        bool isValue = false;
        bool isComment = false;
        uint32_t length = 0;
        for (uint32_t i = 0; i < length; i++) {
            std::string::value_type charValue = content.at(i);
            //Comment
            if (charValue == '#') {
                isComment = true;
                commentString.append(1,charValue);
            }else if (isComment == true) {
                if (charValue != '\n') {
                    commentString.append(1,charValue);
                    continue;
                }
                else {
                    isComment = false;
                    continue;
                }
            }
            //empty
            if (charValue == ' ' || charValue == '\r')
                continue;

            //normalSting and value(n = v)
            if ((charValue == '\n' || i == (length - 1)) && isValue) {
                isValue = false;
                if (i == (length - 1) && charValue != '\n' )
                    valueString.append(1, charValue);
                MagicPtr<ConfigValue> value(new ConfigValue(normalString,valueString,commentString));
                keyValue[normalString] = std::move(value);
                commentString.clear();
                normalString.clear();
                valueString.clear();
                continue;
            }else if (isValue == true) {
                valueString.append(1, charValue);
                continue;
            }else if (charValue == '=') {
                isValue = true;
                continue;
            }
            normalString.append(1, charValue);
        }
    }

    ConfigFile::ConfigFile(const std::string& path)
        :m_Path(path){
        this->m_FileStream.open(this->m_Path, std::ios_base::out);
    }
    void ConfigFile::Read(std::ostringstream& contentStream){
        if(this->m_FileStream.is_open()){
            contentStream << m_FileStream.rdbuf();
        }
    }
    void ConfigFile::write(MagicPtr<ConfigValue>& config){
        config->write(m_FileStream);
    }

    void Config::addConfigFile(MagicPtr<ConfigFile>& configFile){
        m_ConfigFile = std::move(configFile);
        std::ostringstream oss;
        m_ConfigFile->Read(oss);
        std::string content = oss.str();
        ConfigFormatter::Parse(content,m_ConfigMap);
    }
    void Config::buildConfig(){
        auto iter = m_ConfigMap.begin();
        auto end  = m_ConfigMap.end();
        for(;iter!=end;iter++){
             m_ConfigFile->write(iter->second);
        }
    }
}