/*
 * @Author: INotFound
 * @Date: 2020-02-18 11:38:37
 * @LastEditTime: 2021-02-01 23:55:58
 */
#include <tuple>
#include <time.h>
#include <iostream>

#include "Magic/Core/Core.h"
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/String.h"

namespace Magic{
    Safe<Magic::Logger> g_Logger;

    const char* ToString(LogLevel  level){
        switch (level)
       {
#define LEVEL(Name)\
        case LogLevel::Name: \
            return #Name;
            LEVEL(Debug)
            LEVEL(Info)
            LEVEL(Warn)
            LEVEL(Error)
            LEVEL(Fatal)
#undef LEVEL
        }
        return "<(Error)>";
    }
    
    LogEvent::LogEvent(uint32_t line, uint64_t time, uint64_t elapse,uint64_t threadId, 
        const std::string& file, const std::string& logName, const std::string& threadName)
        :m_Line(line),
        m_Time(time),
        m_Elapse(elapse),
        m_ThreadId(threadId),
        m_File(file),
        m_LogName(logName),
        m_ThreadName(threadName){
    }
    
    uint32_t LogEvent::getLine()    const{
        return m_Line;
    }

    uint64_t LogEvent::getTime()    const{
        return m_Time;
    }

    uint64_t LogEvent::getElapse()  const{
        return m_Elapse;
    }

    uint64_t LogEvent::getThreadId()                const{
        return m_ThreadId;
    }

    const std::string LogEvent::getContent()       const{
        return m_StringStream.str();
    }

    const std::string& LogEvent::getFile()          const{
        return m_File;
    }

    const std::string& LogEvent::getLogName()       const{
        return m_LogName;
    }

    const std::string& LogEvent::getThreadName()    const{
        return m_ThreadName;
    }

    std::stringstream& LogEvent::getStream(){
        return m_StringStream;
    }

//###############################BEGIN##################################
    ILogFormatItem::~ILogFormatItem(){}

    class MessageFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class LevelFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class ElapseFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class LogNameFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class ThreadIdFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class NewLineFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class DateTimeFormatItem :public ILogFormatItem{
    public:
        explicit DateTimeFormatItem(const std::string& formatString = "%Y:%m:%d %H:%M:%S");
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    private:
        std::string m_FormatString;
    };

    class FilePathFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;

    };

    class LineFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class TabFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class ThreadNameFormatItem :public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    };

    class StringFormatItem :public ILogFormatItem{
    public:
        explicit StringFormatItem(const std::string& str) :m_String(str){}
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& event) override;
    private:
        std::string m_String;
    };

    void MessageFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        os << event->getContent().c_str();
    }

    void LevelFormatItem::format(std::ostream& os,LogLevel level,const Safe<LogEvent>&){
        os << ToString(level);
    }

    void ElapseFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        os << event->getElapse();
    }

    void LogNameFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        os << event->getLogName();
    }

    void ThreadIdFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        os << event->getThreadId();
    }

    void NewLineFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>&){
        os << std::endl;
    }

    DateTimeFormatItem::DateTimeFormatItem(const std::string& formatString) :m_FormatString(formatString){
        if (this->m_FormatString.empty()){
            this->m_FormatString.append("%Y:%m:%d %H:%M:%S");
        }
    }

    void DateTimeFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        time_t time_secounds = static_cast<int32_t>(event->getTime());
        os << Magic::TimeToString(time_secounds,this->m_FormatString).c_str();
    }

    void FilePathFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        const std::string& filePath = event->getFile();
        std::size_t pathPos = filePath.rfind('/');
        if(pathPos == std::string::npos) {
            pathPos = filePath.rfind('\\');
            pathPos = (pathPos == std::string::npos) ? 0 : pathPos + 1;
        }else {
            pathPos++;
        }
        os << filePath.substr(pathPos);
    }

    void LineFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        os << event->getLine();
    }

    void TabFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>&){
        os << '\t';
    }

    void ThreadNameFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>& event){
        os << event->getThreadName().c_str();
    }

    void StringFormatItem::format(std::ostream& os,LogLevel,const Safe<LogEvent>&){
        os << this->m_String.c_str();
    }
    //###############################*END*##################################

    LogFormatter::LogFormatter(const std::string& pattern){
        //cmd fmt flag
        std::string cmd;
        std::string fmt;
        std::string normalString;
        std::vector<std::tuple<std::string, std::string, uint32_t>> vec;
        uint64_t length = pattern.size();
        for (uint32_t i = 0; i < length; i++){
            cmd.clear();
            fmt.clear();
            if (pattern.at(i) != '%'){
                normalString.append(1, pattern.at(i));
                continue;
            }
            uint32_t n = i + 1;
            uint32_t Status = 0;
            uint32_t Index = 0;
            if (n < length && pattern.at(n) == '%'){
                normalString.append(1, '%');
                i = n;
                continue;
            }
            while (n < length){
                if (!Status && !isalpha(pattern.at(n))
                    && pattern.at(n) != '{' && pattern.at(n) != '}'){
                    cmd = pattern.substr(i + 1, n - i - 1);
                    break;
                }
                if (Status == 0){
                    if (pattern.at(n) == '{'){
                        cmd = pattern.substr(i + 1, n - i - 1);
                        Status = 1;
                        Index = n;
                        n++;
                        continue;
                    }
                }
                if (Status == 1){
                    if (pattern.at(n) == '}'){
                        fmt = pattern.substr(Index + 1, n - Index - 1);
                        Status = 0;
                        n++;
                        break;
                    }
                }
                n++;
                if (n == length){
                    if (cmd.empty()){
                        cmd = pattern.substr(i + 1);
                    }
                }
            }
            if (Status == 0){
                if (!normalString.empty()){
                    vec.emplace_back(normalString, std::string(), 0);
                    normalString.clear();
                }
                vec.emplace_back(cmd, fmt, 1);
                i = n - 1;
            }
            else if (Status == 1){
                std::cout << "pattern error:" << pattern << "-" << pattern.substr(i) << std::endl;
            }
        }
        if (!normalString.empty()){
            vec.emplace_back(normalString, std::string(), 0);
            normalString.clear();
        }

        static std::map<std::string, std::function<Safe<ILogFormatItem>(const std::string&)>> formatItem{
    #define Item(str,type) \
           {#str,[](const std::string&){ return Safe<ILogFormatItem>(new type); }}
    #define ItemEx(str,type) \
           {#str,[](const std::string& fmt){ return Safe<ILogFormatItem>(new type(fmt)); }}
            Item(T, TabFormatItem),                //T:Tab
            Item(l, LineFormatItem),               //l:�к�
            Item(p, LevelFormatItem),              //p:��־����
            Item(r, ElapseFormatItem),             //r:�ۼƺ�����
            Item(m, MessageFormatItem),            //m:��Ϣ
            Item(c, LogNameFormatItem),            //c:��־����
            Item(n, NewLineFormatItem),            //n:����
            Item(t, ThreadIdFormatItem),           //t:�߳�id
            Item(f, FilePathFormatItem),           //f:�ļ���
            Item(N, ThreadNameFormatItem),         //N:�߳�����
            ItemEx(d, DateTimeFormatItem),         //d:ʱ��
    #undef ItemEx
    #undef Item
        };

        for (auto& value : vec){
            uint32_t flag = std::get<2>(value);
            if (flag == 0){
                this->m_Items.push_back(std::make_shared<StringFormatItem>(std::get<0>(value)));
            }
            if (flag == 1){
                auto iter = formatItem.find(std::get<0>(value));
                if (iter == formatItem.end()){
                    std::cout << "<(Error)>: Not Found Item" << std::endl;
                }
                else{
                    this->m_Items.push_back(iter->second(std::get<1>(value)));
                }
            }
        }
    }

    void LogFormatter::format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event){
        for (auto& v : this->m_Items){
            v->format(os, level, event);
        }
    }
    
    Logger::Logger(const Safe<Config>& configuration)
        :m_Level(LogLevel::Debug)
        ,m_LogName(configuration->at<std::string>("Logger.Name","Root"))
        ,m_Formatter(configuration->at<std::string>("Logger.Formatter","[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T[%c]%T%f:%l%T%m%n")){
    }

    void  Logger::addILogAppender(const Safe<ILogAppender>& logAppender){
        Mutex::Lock lock(m_Mutex);
        if(!g_Logger){
            g_Logger = this->shared_from_this();
        }
        if(!logAppender->m_Formatter){
            logAppender->m_Formatter = std::make_shared<LogFormatter>(m_Formatter);
        }
        this->m_ILogAppenders.push_back(logAppender);
    }

    void Logger::setFormatter(const std::string& pattern){
        Mutex::Lock lock(m_Mutex);
        this->m_Formatter = pattern;
    }

    void Logger::setLevel(LogLevel val){
        Mutex::Lock lock(m_Mutex);
        this->m_Level = val;
    }

    LogLevel Logger::getLevel() const{
        return this->m_Level;
    }

    const std::string& Logger::getLogName() const{
        return this->m_LogName;
    }

    void  Logger::log(LogLevel level,const Safe<LogEvent>& event){
        if (level >= m_Level){
            Mutex::Lock lock(m_Mutex);
            if (!this->m_ILogAppenders.empty()){
                for (auto& v : this->m_ILogAppenders){
                    v->log(level, event);
                }
            }
        }
    }

    LogWrap::LogWrap(LogLevel level, Safe<LogEvent>&& event ,const Safe<Logger>& logger)
        :m_Level(level)
        ,m_Event(std::move(event))
        ,m_Logger(logger){
    }
    
    std::stringstream& LogWrap::get(){
        return this->m_Event->getStream();
    }
    
    LogWrap::~LogWrap(){
        this->m_Logger->log(this->m_Level, this->m_Event);
    }

    FileLogAppender::FileLogAppender(const Safe<Config>& configuration) 
        :m_Path(configuration->at<std::string>("Logger.FilePath","./log.txt")){
        Mutex::Lock lock(m_Mutex);
        this->m_FileStream.open(this->m_Path, std::ios_base::out | std::ios_base::app);
    }

    void FileLogAppender::log(LogLevel level,const Safe<LogEvent>& event){
        this->m_Formatter->format(this->m_FileStream, level, event);
    }

    void StdOutLogAppender::log(LogLevel level,const Safe<LogEvent>& event){
        if (!this->m_Formatter){
            std::cout << "<(Error)> "<< std::endl;
            return;
        }
        this->m_Formatter->format(std::cout, level, event);
    }
    
}
