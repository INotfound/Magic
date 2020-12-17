/*
 * @File: Config.cpp
 * @Author: INotFound
 * @Date: 2020-03-04 23:46:59
 * @LastEditTime: 2020-03-14 00:16:27
 */
#include "Config.h"
#include "Macro.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

namespace Magic {
    ConfigFile::~ConfigFile() {
        this->close();
    }
    ConfigFile::ConfigFile(const std::string& path)
        :m_Path(path) {
        this->open();
    }
    void ConfigFile::open() {
        m_FileStream.open(m_Path, std::ios_base::in | std::ios_base::out | std::ios_base::app);
    }
    void ConfigFile::close() {
        if (m_FileStream.is_open()) {
            m_FileStream.close();
        }
    }
    const std::string& ConfigFile::getPath() const {
        return m_Path;
    }
    void ConfigFile::read(ConfigMap& keyValue) {
        std::ostringstream content;
        if (m_FileStream.is_open()) {
            content << m_FileStream.rdbuf();
            m_Formatter->parse(content.str(), keyValue);
        }
    }
    void ConfigFile::write(ConfigMap& config) {
        m_Formatter->write(m_FileStream,config);
        m_FileStream.flush();
    }
    void ConfigFile::addFormatter(Safe<IConfigFormatter>& configFormatter) {
        m_Formatter = std::move(configFormatter);
    }

    ConfigValue::ConfigValue(const std::string& name, const std::string& value, const std::string& comment)
        :m_IsComment(false),m_Name(name), m_Value(value), m_Comment(comment) {
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

    void InIConfigFormatter::write(std::ostream& os, ConfigMap& KeyValue) {
        auto iter = KeyValue.begin();
        auto end  = KeyValue.end();
        for (; iter != end; iter++) {
            Safe<ConfigValue>& value = iter->second;
            if (!value->getComment().empty()) {
                os << "#" << value->getComment() << std::endl;
            }
            os << value->getName() << "=" << value->getValue() << std::endl;
            os << std::endl;
        }
    }
    void InIConfigFormatter::parse(const std::string& content, ConfigMap& keyValue) {
        std::string valueString;
        std::string normalString;
        std::string commentString;
        bool isEmpty = true;
        bool isValue = false;
        bool isComment = false;
        uint64_t length = content.length();
        for (uint64_t i = 0; i < length; i++){
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
            // normal and value
            if ((charValue == '\n' || i == (length - 1)) && isValue) {
                isEmpty = true;
                isValue = false;
                if (i == (length - 1) && charValue != '\n')
                    valueString.append(1, charValue);
                Safe<ConfigValue> value(new ConfigValue(normalString,valueString,commentString));
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

    void JsonConfigFormatter::write(std::ostream& os, ConfigMap& KeyValue) {
        Safe<rapidjson::Document> json(new rapidjson::Document);
        rapidjson::Document::AllocatorType& allocator = json->GetAllocator();
        json->SetObject();
        for(auto&v : KeyValue)
        {            
            if (v.second->isComment()) {
                std::string commentName(v.first + "Comment");
                rapidjson::Value jsonName(commentName.c_str(), allocator);
                rapidjson::Value jsonValue(v.second->getComment().c_str() , allocator);
                json->AddMember(jsonName, jsonValue, allocator);
            }
            rapidjson::Value jsonName(v.first.c_str(), allocator);
            rapidjson::Value jsonValue(v.second->getValue().c_str() , allocator);
            json->AddMember(jsonName, jsonValue, allocator);
        }
        Safe<rapidjson::StringBuffer> buffer(new rapidjson::StringBuffer);
        Safe<rapidjson::Writer<rapidjson::StringBuffer>> writer(new rapidjson::Writer<rapidjson::StringBuffer>(*buffer));
        json->Accept(*writer);
        os << buffer->GetString();
    }
    void JsonConfigFormatter::parse(const std::string& content, ConfigMap& keyValue) {
        if (content.empty())
            return;
        Safe<rapidjson::Document> json(new rapidjson::Document);
        json->Parse(content.c_str());
        auto iter = json->MemberBegin();
        auto end = json->MemberEnd();
        for (; iter != end; iter++)
        {
            std::string keyName(iter->name.GetString());
            Safe<ConfigValue> value(new ConfigValue(keyName,iter->value.GetString()));
            keyValue[keyName] = std::move(value);
        }
    }
namespace Instance{
    Config::~Config() {
        this->update();
    }
    Config::Config()
        :m_IsChange(false){
    }
    void Config::update(){
        if (!m_IsChange){
            return;
        }
        RWMutex::WriteLock lock(m_Mutex);
        m_ConfigFile->close();
        std::remove(m_ConfigFile->getPath().c_str());
        m_ConfigFile->open();
        m_ConfigFile->write(m_ConfigMap);
        MAGIC_DEBUG() << "Update configuration items";
    }
    void Config::addConfigFile(Safe<ConfigFile>& configFile) {
        RWMutex::WriteLock lock(m_Mutex);
        m_ConfigFile = std::move(configFile);
        m_ConfigFile->read(m_ConfigMap);
    }
}
}