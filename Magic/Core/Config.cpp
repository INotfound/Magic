#include "Config.h"
#include <sstream>
#include <io.h>
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
		if (!m_Comment.empty())
		{
			os << std::endl << m_Comment << std::endl;
		}
		else {
			os << std::endl;
		}
        os << m_Name << '=' << m_Value;
    }

    void ConfigFormatter::Parse(std::string& content,ConfigKeyValue& keyValue){
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
                commentString.append(1,charValue);
				continue;
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
			//empty
			if (charValue == ' ' || charValue == '\r' || charValue == '\n')
				continue;

            normalString.append(1, charValue);
        }
    }

    ConfigFile::ConfigFile(const std::string& path,const std::string &welcome)
        :m_Path(path){
		bool isFile = false;
		if (access(m_Path.c_str(), 0) == -1) {
			isFile = true;
		}
        this->m_FileStream.open(this->m_Path, std::ios_base::in | std::ios_base::out | std::ios_base::app);
		if (isFile)
			this->m_FileStream << welcome.c_str();
		this->m_FileStream.flush();
    }
    void ConfigFile::Read(std::ostringstream& contentStream){
        if(this->m_FileStream.is_open()){
            contentStream << m_FileStream.rdbuf();
        }
    }
    void ConfigFile::write(MagicPtr<ConfigValue>& config){
        config->write(m_FileStream);
		m_FileStream.flush();
    }

    void Config::addConfigFile(MagicPtr<ConfigFile>& configFile){
        m_ConfigFile = std::move(configFile);
        std::ostringstream oss;
        m_ConfigFile->Read(oss);
        std::string content = oss.str();
        ConfigFormatter::Parse(content,m_ConfigMap);
    }
    void Config::reBuild(){
        auto iter = m_ConfigMap.begin();
        auto end  = m_ConfigMap.end();
        for(;iter!=end;iter++){
             m_ConfigFile->write(iter->second);
        }
    }
}