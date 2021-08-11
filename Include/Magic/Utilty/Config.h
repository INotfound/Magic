/*
 * @Author: INotFound
 * @Date: 2020-09-28 14:07:31
 * @LastEditTime: 2021-02-01 22:19:30
 */
#pragma once
#include <sstream>
#include <fstream>

#include "Magic/Core/Core.h"
#include "Magic/Utilty/Mutex.h"
#include "Magic/Utilty/String.h"

namespace Magic {
    /**
     * @brief: 配置项类
     */
    class ConfigValue {
    public:
        /**
         * @brief: 构造函数
         * @param name 键名称
         * @param value 值名称
         * @param comment 注释
         */
        ConfigValue(const std::string& name, const std::string& value, const std::string& comment = "");
        /**
         * @brief: 是否是注释函数
         * @return: 返回True则是，返回False则否
         */
        bool isComment() const;
        /**
         * @brief: 获取配置项名称函数
         * @return: 返回配置项名称
         */
        const std::string& getName() const;
        /**
         * @brief: 获取配置项值函数
         * @return: 返回配置项值
         */
        const std::string& getValue() const;
        /**
         * @brief: 获取配置项注释函数
         * @return: 返回配置项注释
         */
        const std::string& getComment() const;
    private:
        bool m_IsComment;
        std::string m_Name;
        std::string m_Value;
        std::string m_Comment;
    };
    typedef std::unordered_map<std::string, Safe<ConfigValue>> ConfigMap;
    /**
     * @brief: 配置格式化接口类 
     */
    class IConfigFormatter {
    public:
        virtual ~IConfigFormatter();
        /**
         * @brief: 将配置键值对写入配置文件函数
         * @param os 输出流
         * @param keyValue 配置键值对容器
         */
        virtual void write(std::ostream& os, ConfigMap& KeyValue) = 0;
        /**
         * @brief: 解析配置文件函数
         * @param content 配置文件正文
         * @param keyValue 配置文件键值对容器
         */
        virtual void parse(const std::string& content, ConfigMap& keyValue) = 0;
    };    
    /**
     * @brief: 配置文件类
     */
    class ConfigFile {
    public:
        /**
         * @brief: 析构函数
         */
        ~ConfigFile();
        /**
         * @brief: 构造函数
         */
        ConfigFile();
        /**
         * @brief: 关闭配置文件函数
         */
        void close();
        /**
         * @brief: 打开配置文件函数
         */
        void open();
        /**
         * @brief: 获取配置文件路径函数
         * @return: 返回配置文件路径
         */
        const std::string& getPath() const;
        /**
         * @brief: 设置配置文件路径函数
         */
        void setFilePath(const std::string& path);
        /**
         * @brief: 读取配置文件项函数
         * @param keyValue 配置文件键值对 
         */
        void read(ConfigMap& keyValue);
        /**
         * @brief: 写入配置文件项函数
         * @param config 配置文件 
         */
        void write(ConfigMap& config);
        /**
         * @brief: 添加配置文件格式化器函数
         * @param configFormatter 配置文件格式化器
         */
        void addFormatter(const Safe<IConfigFormatter>& configFormatter);
    private:
        std::string m_Path;
        std::fstream m_FileStream;
        Safe<IConfigFormatter> m_Formatter;
    };
    /**
     * @brief: 配置类
     */
    class Config{
    public:
        /**
         * @brief: 析构函数
         */
        ~Config();
        /**
         * @brief: 构造函数
         */
        Config();
        /**
         * @brief: 添加配置文件函数
         * @param configFile 配置文件
         */
        void addConfigFile(const Safe<ConfigFile>& configFile);
        /**
         * @brief: 获取配置项值函数
         * @param defaultName 配置键
         * @param defaultValue 配置值
         * @param defaultComment 配置注释
         * @return: 返回配置项值
         */
        template<class T>
        T at(const std::string& defaultName, const T& defaultValue, const std::string defaultComment = "");
        /**
         * @brief: 修改配置项值函数
         * @param defaultName 配置键
         * @param defaultValue 配置值
         * @param defaultComment 配置注释
         * @return: 返回配置项值
         */
        template<class T>
        T revise(const std::string& defaultName, const T& defaultValue, const std::string& defaultComment = "");
    private:
        /**
         * @brief: 配置更新函数
         */
        void update();
    private:
        bool m_IsChange;
        RWMutex m_Mutex;
        ConfigMap m_ConfigMap;
        Safe<ConfigFile> m_ConfigFile;
    };

    class InIConfigFormatter :public IConfigFormatter {
    public:
        ~InIConfigFormatter() {};
        void write(std::ostream& os, ConfigMap& KeyValue) override;
        void parse(const std::string& content, ConfigMap& keyValue) override;
    };

    class JsonConfigFormatter :public IConfigFormatter {
    public:
        ~JsonConfigFormatter() {};
        void write(std::ostream& os, ConfigMap& KeyValue) override;
        void parse(const std::string& content, ConfigMap& keyValue) override;
    };

    template<class T>
    T Config::at(const std::string& defaultName, const T& defaultValue, const std::string defaultComment) {
        RWMutex::WriteLock lock(m_Mutex);
        auto iter = m_ConfigMap.find(defaultName);
        if (iter != m_ConfigMap.end()) {
            return StringAs<T>(iter->second->getValue());
        }
        m_IsChange = true;
        m_ConfigMap[defaultName] = std::make_shared<ConfigValue>(defaultName, AsString<T>(defaultValue), defaultComment);
        this->update();
        return defaultValue;
    }
    template<class T>
    T Config::revise(const std::string& defaultName, const T& defaultValue, const std::string& defaultComment) {
        RWMutex::WriteLock lock(m_Mutex);
        m_IsChange = true;
        m_ConfigMap[defaultName].reset();
        m_ConfigMap[defaultName] = std::make_shared<ConfigValue>(defaultName, AsString<T>(defaultValue), defaultComment);
        this->update();
        return defaultValue;
    }
}
