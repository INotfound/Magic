/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Logger.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include <tuple>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/Logger.hpp"

namespace Magic{
    Safe<Magic::Logger> g_Logger;

    const char* ToString(LogLevel level){
        switch(level){
        #define LEVEL(Name)      \
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

    LogEvent::LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t threadId,const Magic::StringView& file,const Magic::StringView& logName,const Magic::StringView& threadName)
        :m_Line(line)
        ,m_Time(time)
        ,m_Elapse(elapse)
        ,m_ThreadId(threadId)
        ,m_File(file.data(),file.size())
        ,m_LogName(logName.data(),logName.size())
        ,m_ThreadName(threadName.data(),threadName.size()){
    }

    uint32_t LogEvent::getLine() const{
        return m_Line;
    }

    uint64_t LogEvent::getTime() const{
        return m_Time;
    }

    uint64_t LogEvent::getElapse() const{
        return m_Elapse;
    }

    uint64_t LogEvent::getThreadId() const{
        return m_ThreadId;
    }

    const std::string LogEvent::getContent() const{
        return m_StringStream.str();
    }

    Magic::StringView LogEvent::getFile() const{
        return m_File;
    }

    Magic::StringView LogEvent::getLogName() const{
        return m_LogName;
    }

    Magic::StringView LogEvent::getThreadName() const{
        return m_ThreadName;
    }

    std::stringstream& LogEvent::getStream(){
        return m_StringStream;
    }

//###############################BEGIN##################################
    ILogFormatItem::~ILogFormatItem() = default;

    class MessageFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            os << event->getContent().data();
        }
    };

    class LevelFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel level,const Safe<LogEvent>& /*event*/) override{
            os << ToString(level);
        }
    };

    class ElapseFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            os << event->getElapse();
        }
    };

    class LogNameFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            os << event->getLogName();
        }
    };

    class ThreadIdFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            os << event->getThreadId();
        }
    };

    class NewLineFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& /*event*/) override{
            os << std::endl;
        }
    };

    class DateTimeFormatItem:public ILogFormatItem{
    public:
        explicit DateTimeFormatItem(const Magic::StringView& formatString = "%Y:%m:%d %H:%M:%S")
            :m_FormatString(formatString.data(),formatString.size()){
            if(this->m_FormatString.empty()){
                this->m_FormatString.append("%Y:%m:%d %H:%M:%S");
            }
        }

        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            auto time_secounds = static_cast<std::time_t>(event->getTime());
            os << TimeToString(time_secounds,m_FormatString);
        }

    private:
        std::string m_FormatString;
    };

    class FilePathFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            Magic::StringView filePath = event->getFile();
            std::size_t pathPos = filePath.rfind('/');
            if(pathPos == std::string::npos){
                pathPos = filePath.rfind('\\');
                pathPos = (pathPos == std::string::npos) ? 0 : pathPos + 1;
            }else{
                pathPos++;
            }
            os << filePath.substr(pathPos);
        }
    };

    class LineFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            os << event->getLine();
        }
    };

    class TabFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& /*event*/) override{
            os << '\t';
        }
    };

    class ThreadNameFormatItem:public ILogFormatItem{
    public:
        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& event) override{
            os << event->getThreadName().data();
        }
    };

    class StringFormatItem:public ILogFormatItem{
    public:
        explicit StringFormatItem(const Magic::StringView& str)
            :m_String(str.data(),str.size()){
        }

        void format(std::ostream& os,LogLevel /*level*/,const Safe<LogEvent>& /*event*/) override{
            os << this->m_String.data();
        }

    private:
        std::string m_String;
    };
    //###############################*END*##################################

    LogFormatter::LogFormatter(const Magic::StringView& pattern){
        //cmd fmt flag
        std::string normalString;
        std::vector<std::tuple<std::string,std::string,uint32_t>> vec;
        uint64_t length = pattern.size();
        for(uint32_t i = 0;i < length;i++){
            Magic::StringView cmd;
            Magic::StringView fmt;
            if(pattern.at(i) != '%'){
                normalString.append(1,pattern.at(i));
                continue;
            }
            uint32_t n = i + 1;
            uint32_t Status = 0;
            uint32_t Index = 0;
            if(n < length && pattern.at(n) == '%'){
                normalString.append(1,'%');
                i = n;
                continue;
            }
            while(n < length){
                if(!Status && !isalpha(pattern.at(n)) && pattern.at(n) != '{' && pattern.at(n) != '}'){
                    cmd = pattern.substr(i + 1,n - i - 1);
                    break;
                }
                if(Status == 0){
                    if(pattern.at(n) == '{'){
                        cmd = pattern.substr(i + 1,n - i - 1);
                        Status = 1;
                        Index = n;
                        n++;
                        continue;
                    }
                }
                if(Status == 1){
                    if(pattern.at(n) == '}'){
                        fmt = pattern.substr(Index + 1,n - Index - 1);
                        Status = 0;
                        n++;
                        break;
                    }
                }
                n++;
                if(n == length){
                    if(cmd.empty()){
                        cmd = pattern.substr(i + 1);
                    }
                }
            }
            if(Status == 0){
                if(!normalString.empty()){
                    vec.emplace_back(normalString,std::string(),0);
                    normalString.clear();
                }
                vec.emplace_back(std::string(cmd.data(),cmd.size()),std::string(fmt.data(),fmt.size()),1);
                i = n - 1;
            }else if(Status == 1){
                std::cout << "Pattern Error:" << pattern << "-" << pattern.substr(i) << std::endl;
            }
        }
        if(!normalString.empty()){
            vec.emplace_back(normalString,std::string(),0);
            normalString.clear();
        }

        static std::map<std::string,std::function<Safe<ILogFormatItem>(const Magic::StringView&)>> formatItem{
        #define Item(str,type) \
           {#str,[](const Magic::StringView&){ return Safe<ILogFormatItem>(new (type)); }}
        #define ItemEx(str,type) \
           {#str,[](const Magic::StringView& fmt){ return Safe<ILogFormatItem>(new type(fmt)); }}
            Item(T,TabFormatItem),                //T:Tab
            Item(l,LineFormatItem),               //l:行数
            Item(p,LevelFormatItem),              //p:级别
            Item(r,ElapseFormatItem),             //r:\t
            Item(m,MessageFormatItem),            //m:消息
            Item(c,LogNameFormatItem),            //c:日志名
            Item(n,NewLineFormatItem),            //n:新一行
            Item(t,ThreadIdFormatItem),           //t:线程Id
            Item(f,FilePathFormatItem),           //f:文件路径
            Item(N,ThreadNameFormatItem),         //N:线程名
            ItemEx(d,DateTimeFormatItem),         //d:时间
        #undef ItemEx
        #undef Item
        };

        for(auto& value : vec){
            uint32_t flag = std::get<2>(value);
            if(flag == 0){
                this->m_Items.push_back(std::make_shared<StringFormatItem>(std::get<0>(value)));
            }
            if(flag == 1){
                auto iter = formatItem.find(std::get<0>(value));
                if(iter == formatItem.end()){
                    std::cout << "<(Error)>: Not Found Item" << std::endl;
                }else{
                    this->m_Items.push_back(iter->second(std::get<1>(value)));
                }
            }
        }
    }

    void LogFormatter::format(std::ostream& os,const LogLevel level,const Safe<LogEvent>& event){
        for(auto& v : this->m_Items){
            v->format(os,level,event);
        }
    }

    Logger::Logger(const Safe<Config>& configuration)
        :m_Level(LogLevel::Debug)
        ,m_LogName(configuration->at<std::string>("Logger.Name","Root"))
        ,m_Formatter(configuration->at<std::string>("Logger.Formatter","[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T[%c]%T%f:%l%T%m%n")){
    }

    void Logger::externMode(){
        if(!g_Logger){
            g_Logger = this->shared_from_this();
        }
    }

    void Logger::setLevel(LogLevel val){
        std::lock_guard<std::mutex> locker(m_Mutex);
        this->m_Level = val;
    }

    LogLevel Logger::getLevel() const{
        return this->m_Level;
    }

    Magic::StringView Logger::getLogName() const{
        return this->m_LogName;
    }

    void Logger::setFormatter(const Magic::StringView& pattern){
        std::lock_guard<std::mutex> locker(m_Mutex);
        this->m_Formatter = std::string(pattern.data(),pattern.size());
    }

    void Logger::log(LogLevel level,const Safe<LogEvent>& event){
        if(level >= m_Level){
            std::lock_guard<std::mutex> locker(m_Mutex);
            if(!this->m_ILogAppenders.empty()){
                for(auto& v : this->m_ILogAppenders){
                    v->log(level,event);
                }
            }
        }
    }

    void Logger::addILogAppender(const Safe<ILogAppender>& logAppender){
        std::lock_guard<std::mutex> locker(m_Mutex);
        if(!logAppender->m_Formatter){
            logAppender->m_Formatter = std::make_shared<LogFormatter>(m_Formatter);
        }
        this->m_ILogAppenders.push_back(logAppender);
    }

    LogWrap::LogWrap(LogLevel level,Safe<LogEvent>&& event,const Safe<Logger>& logger)
        :m_Level(level)
        ,m_Event(std::move(event))
        ,m_Logger(logger){
    }

    std::stringstream& LogWrap::get(){
        return this->m_Event->getStream();
    }

    LogWrap::~LogWrap(){
        this->m_Logger->log(this->m_Level,this->m_Event);
    }

    FileLogAppender::FileLogAppender(const Safe<Config>& configuration)
        :m_Path(configuration->at<std::string>("Logger.FilePath","./log.txt")){
        std::lock_guard<std::mutex> locker(m_Mutex);
        this->m_FileStream.open(this->m_Path,std::ios_base::out | std::ios_base::app);
    }

    void FileLogAppender::log(LogLevel level,const Safe<LogEvent>& event){
        this->m_Formatter->format(this->m_FileStream,level,event);
    }

#if defined(_WIN32) || defined(_WIN64)
    void StdOutLogAppender::log(LogLevel level,const Safe<LogEvent>& event){
        if (!this->m_Formatter){
            std::cout << "<(Error)> "<< std::endl;
            return;
        }
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
        GetConsoleScreenBufferInfo(handle,&ConsoleScreenBufferInfo);
        switch (level) {
        case LogLevel::Info:
            SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | ConsoleScreenBufferInfo.wAttributes);
            break;
        case LogLevel::Warn:
            SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            break;
        case LogLevel::Error:
            SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | FOREGROUND_RED);
            break;
        case LogLevel::Fatal:
            SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | FOREGROUND_RED);
            break;
        case LogLevel::Debug:
            SetConsoleTextAttribute(handle,FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
            break;
        }
        this->m_Formatter->format(std::cout, level, event);
        SetConsoleTextAttribute(handle,ConsoleScreenBufferInfo.wAttributes);
    }
#endif

#if (defined(linux) || defined(__linux__))
    void StdOutLogAppender::log(LogLevel level,const Safe<LogEvent>& event){
        if(!this->m_Formatter){
            std::cout << "<(Error)> " << std::endl;
            return;
        }
        switch(level){
            case LogLevel::Info:
                std::cout << "\033[0m";
                break;
            case LogLevel::Warn:
                std::cout << "\033[34;1m";
                break;
            case LogLevel::Error:
                std::cout << "\033[31;1m";
                break;
            case LogLevel::Fatal:
                std::cout << "\033[31;1m";
                break;
            case LogLevel::Debug:
                std::cout << "\033[33;1m";
                break;
        }
        this->m_Formatter->format(std::cout,level,event);
        std::cout << "\033[0m";
        std::cout.flush();
    }
#endif

}
