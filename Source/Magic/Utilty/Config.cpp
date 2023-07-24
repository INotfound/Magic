/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Config.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Utilty/Config.hpp"
#include "Magic/Utilty/Logger.hpp"
// #include "rapidjson/writer.h"
// #include "rapidjson/document.h"
// #include "rapidjson/stringbuffer.h"

namespace Magic{
    std::string g_ConfigPath = "./Magic.conf";

    ConfigValue::ConfigValue(const StringView& name,const StringView& value,const StringView& comment)
        :m_IsComment(false)
        ,m_Name(name.data(),name.size())
        ,m_Value(value.data(),value.size())
        ,m_Comment(comment.data(),comment.size()){
        if(!m_Comment.empty())
            m_IsComment = true;
    }

    bool ConfigValue::isComment() const{
        return m_IsComment;
    }

    StringView ConfigValue::getName() const{
        return m_Name;
    }

    StringView ConfigValue::getValue() const{
        return m_Value;
    }

    StringView ConfigValue::getComment() const{
        return m_Comment;
    }

    IConfigFormatter::~IConfigFormatter() = default;

    ConfigFile::~ConfigFile() = default;

    ConfigFile::ConfigFile(const Safe<IConfigFormatter>& configFormatter)
        :m_Path(g_ConfigPath)
        ,m_Formatter(configFormatter){
    }

    StringView ConfigFile::getPath() const{
        return m_Path;
    }

    void ConfigFile::read(ConfigMap& keyValue){
        std::ostringstream content;

        std::ifstream readFileStream;
        readFileStream.open(m_Path,std::ios_base::in);
        if(readFileStream.is_open() && m_Formatter){
            content << readFileStream.rdbuf();
            m_Formatter->parse(content.str(),keyValue);
        }
    }

    void ConfigFile::write(ConfigMap& config){
        std::ofstream writeFileStream;
        writeFileStream.open(m_Path,std::ios_base::out);
        m_Formatter->write(writeFileStream,config);
        writeFileStream.flush();
    }

    Config::~Config(){
        this->update();
    }

    Config::Config(const Safe<ConfigFile>& configFile)
        :m_IsChange(false)
        ,m_ConfigFile(configFile){
        m_ConfigFile->read(m_ConfigMap);
    }

    void Config::update(){
        if(!m_IsChange){
            return;
        }
        m_IsChange = false;
        std::remove(m_ConfigFile->getPath().data());
        m_ConfigFile->write(m_ConfigMap);
    }

    void InIConfigFormatter::write(std::ostream& os,ConfigMap& KeyValue){
        auto iter = KeyValue.begin();
        auto end = KeyValue.end();
        for(;iter != end;iter++){
            Safe<ConfigValue>& value = iter->second;
            if(!value->getComment().empty()){
                os << "# " << value->getComment() << std::endl;
            }
            os << value->getName() << "=" << value->getValue() << std::endl;
            os << std::endl;
        }
    }

    void InIConfigFormatter::parse(const StringView& content,ConfigMap& keyValue){
        std::string valueString;
        std::string normalString;
        std::string commentString;
        bool isEmpty = true;
        bool isValue = false;
        bool isComment = false;
        uint64_t length = content.length();
        for(uint64_t i = 0;i < length;i++){
            std::string::value_type charValue = content.at(i);
            //Comment
            if(charValue == '#'){
                isComment = true;
                continue;
            }else if(isComment){
                if(charValue == ' '){
                    continue;
                }
                if(charValue != '\n'){
                    commentString.append(1,charValue);
                    continue;
                }else{
                    isComment = false;
                    continue;
                }
            }
            // normal and value
            if((charValue == '\n' || i == (length - 1)) && isValue){
                isEmpty = true;
                isValue = false;
                if(i == (length - 1) && charValue != '\n')
                    valueString.append(1,charValue);

                keyValue[normalString] = std::make_shared<ConfigValue>(normalString,valueString,commentString);
                commentString.clear();
                normalString.clear();
                valueString.clear();
                continue;
            }else if(isValue){
                if(!(charValue == ' ' && isEmpty)){
                    isEmpty = false;
                    valueString.append(1,charValue);
                }
                continue;
            }else if(charValue == '='){
                isValue = true;
                continue;
            }
            //empty
            if(charValue == ' ' || charValue == '\r' || charValue == '\n'){
                continue;
            }
            normalString.append(1,charValue);
        }
    }
}
