/*
 * @Author: INotFound
 * @Date: 2020-09-28 14:07:31
 * @LastEditTime: 2021-02-01 22:19:30
 */
#pragma once

#include <sstream>
#include <fstream>

#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/String.hpp"

namespace Magic{
    /**
     * @brief 配置项类
     */
    class ConfigValue:public Noncopyable{
    public:
        /**
         * @brief 构造函数
         * @param name 键名称
         * @param value 值名称
         * @param comment 注释
         */
        ConfigValue(const Magic::StringView& name,const Magic::StringView& value,const Magic::StringView& comment = "");

        /**
         * @brief 是否是注释函数
         * @return: 返回True则是，返回False则否
         */
        bool isComment() const;

        /**
         * @brief 获取配置项名称函数
         * @return: 返回配置项名称
         */
        Magic::StringView getName() const;

        /**
         * @brief 获取配置项值函数
         * @return: 返回配置项值
         */
        Magic::StringView getValue() const;

        /**
         * @brief 获取配置项注释函数
         * @return: 返回配置项注释
         */
        Magic::StringView getComment() const;

    private:
        bool m_IsComment;
        std::string m_Name;
        std::string m_Value;
        std::string m_Comment;
    };

    typedef std::unordered_map<std::string,Safe<ConfigValue>> ConfigMap;

    /**
     * @brief 配置格式化接口类
     */
    class IConfigFormatter:public Noncopyable{
    public:
        virtual ~IConfigFormatter();

        /**
         * @brief 将配置键值对写入配置文件函数
         * @param os 输出流
         * @param keyValue 配置键值对容器
         */
        virtual void write(std::ostream& os,ConfigMap& KeyValue) = 0;

        /**
         * @brief 解析配置文件函数
         * @param content 配置文件正文
         * @param keyValue 配置文件键值对容器
         */
        virtual void parse(const Magic::StringView& content,ConfigMap& keyValue) = 0;
    };

    /**
     * @brief 配置文件类
     */
    class ConfigFile:public Noncopyable{
    public:
        /**
         * @brief 析构函数
         */
        ~ConfigFile();

        /**
         * @brief 构造函数
         * @param configFormatter 配置文件格式化器
         */
        explicit ConfigFile(const Safe<IConfigFormatter>& configFormatter);

        /**
         * @brief 获取配置文件路径函数
         * @return: 返回配置文件路径
         */
        Magic::StringView getPath() const;

        /**
         * @brief 读取配置文件项函数
         * @param keyValue 配置文件键值对 
         */
        void read(ConfigMap& keyValue);

        /**
         * @brief 写入配置文件项函数
         * @param config 配置文件 
         */
        void write(ConfigMap& config);

    private:
        std::string m_Path;
        Safe<IConfigFormatter> m_Formatter;
    };

    /**
     * @brief 配置类
     */
    class Config:public Noncopyable{
    public:
        /**
         * @brief 析构函数
         */
        ~Config();

        /**
         * @brief 构造函数
         * @param configFile 配置文件
         */
        explicit Config(const Safe<ConfigFile>& configFile);

        /**
         * @brief 获取配置项值函数
         * @param defaultName 配置键
         * @param defaultValue 配置值
         * @param defaultComment 配置注释
         * @return: 返回配置项值
         */
        template<typename T>
        T at(const Magic::StringView& defaultName,const T& defaultValue,const Magic::StringView& defaultComment = "");

        /**
         * @brief 修改配置项值函数
         * @param defaultName 配置键
         * @param defaultValue 配置值
         * @param defaultComment 配置注释
         * @return: 返回配置项值
         */
        template<typename T>
        T revise(const Magic::StringView& defaultName,const T& defaultValue,const Magic::StringView& defaultComment = "");

    private:
        /**
         * @brief 配置更新函数
         */
        void update();

    private:
        bool m_IsChange;
        std::mutex m_Mutex;
        ConfigMap m_ConfigMap;
        Safe<ConfigFile> m_ConfigFile;
    };

    class InIConfigFormatter:public IConfigFormatter{
    public:
        ~InIConfigFormatter() override = default;

        void write(std::ostream& os,ConfigMap& KeyValue) override;

        void parse(const Magic::StringView& content,ConfigMap& keyValue) override;
    };

    template<typename T>
    T Config::at(const Magic::StringView& defaultName,const T& defaultValue,const Magic::StringView& defaultComment){
        std::lock_guard<std::mutex> locker(m_Mutex);
        std::string key(defaultName.data(),defaultName.size());
        auto iter = m_ConfigMap.find(key);
        if(iter != m_ConfigMap.end()){
            try{
                return StringAs<T>(iter->second->getValue());
            }catch(...){
                return defaultValue;
            }
        }
        m_IsChange = true;
        m_ConfigMap[key] = std::make_shared<ConfigValue>(defaultName,AsString<T>(defaultValue),defaultComment);
        this->update();
        return defaultValue;
    }

    template<typename T>
    T Config::revise(const Magic::StringView& defaultName,const T& defaultValue,const Magic::StringView& defaultComment){
        std::lock_guard<std::mutex> locker(m_Mutex);
        m_IsChange = true;
        std::string key(defaultName.data(),defaultName.size());
        m_ConfigMap[key] = std::make_shared<ConfigValue>(defaultName,AsString<T>(defaultValue),defaultComment);
        this->update();
        return defaultValue;
    }

    /*
     * g_ConfigPath 配置文件默认路径
     */
    extern std::string g_ConfigPath;
}
