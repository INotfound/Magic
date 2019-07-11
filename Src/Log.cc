#include "Log.h"
#include <map>
#include <functional>
#include <time.h>

namespace Log
{

//format item interface
class MessageFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getContext().c_str();
    }
};

class LevelFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << LogLevel::toString(level); 
    }
};

class ElapseFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getElapse();
    }
};

class LogNameFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getLogName();
    }
};

class ThreadIdFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getThreadId();
    }
};

class NewLineFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << std::endl;
    }
};

class DateTimeFormatItem :public ILogFormatItem{
public:
    DateTimeFormatItem(const std::string &formatString ="%Y:%m:%d %H:%M:%S"):FormatString(formatString){
        if(FormatString.empty()){
            FormatString.append("%Y:%m:%d %H:%M:%S");
        }
    }
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        time_t time_secounds = event->getTime();
        struct tm nowTime;
        //localtime_r(&time_secounds,&nowTime);
        localtime_s(&nowTime,&time_secounds);
		char buf[1024] = { 0 };
        strftime(buf,sizeof(buf),FormatString.c_str(),&nowTime);
        os << buf;
    }
private:
    std::string FormatString;
};

class FilePathFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getFile().c_str(); 
    }
};

class LineFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getLine(); 
    }
};

class TabFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << '\t';
    }

};

class FiberIdFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getFiberId();
    }
};

class ThreadNameFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << event->getThreadName().c_str();
    }
};

class StringFormatItem :public ILogFormatItem{
public:
    StringFormatItem(const std::string & str):Str(str){}
    void format(std::ostream &os, LogLevel::Level level,std::shared_ptr<LogEvent> event) override{
        os << Str.c_str(); 
    }
private:
    std::string Str; 
};

const char* LogLevel::toString(LogLevel::Level level){
    switch(level){
#define LEVEL(Name)\
        case LogLevel::Name: \
            return #Name;
        
        LEVEL(DEBUG)
        LEVEL(INFO)
        LEVEL(WARN)
        LEVEL(ERROR)
        LEVEL(FATAL)
        default:
            break;
#undef LEVEL
    }
	return "<(ERROR)>";
}
LogEvent::LogEvent(uint32_t line,uint64_t time,uint64_t elapse,uint64_t fiberId,uint64_t threadId,const std::string& file,const std::string& logName,const std::string& context,const std::string& threadName)
    :Line(line),Time(time),Elapse(elapse),FiberId(fiberId),ThreadId(threadId),File(file),LogName(logName),Context(context),ThreadName(threadName){
    
    }

uint32_t LogEvent::getLine() const{ return this->Line; }
uint64_t LogEvent::getTime() const{ return this->Time; }
uint64_t LogEvent::getElapse() const{ return this->Elapse; }
uint64_t LogEvent::getFiberId() const{ return this->FiberId; }
uint64_t LogEvent::getThreadId() const{ return this->ThreadId; }
const std::string& LogEvent::getLogName() const{return this->LogName;}
const std::string& LogEvent::getFile() const{ return this->File; }
const std::string& LogEvent::getContext() const{ return this->Context; }
const std::string& LogEvent::getThreadName() const{ return this->ThreadName; }

Logger::Logger(const std::string &name) 
    :LogName(name){
}

void Logger::addILogAppender(std::shared_ptr<ILogAppender> logAppender){
    if(!logAppender->Formatter){
        logAppender->Formatter = this->Formatter;
    }
    this->ILogAppenders.push_back(logAppender);
}

void Logger::delILogAppender(std::shared_ptr<ILogAppender> logAppender){
    for(auto i=this->ILogAppenders.begin();i!=this->ILogAppenders.end();i++){
        if(*i == logAppender){
            this->ILogAppenders.erase(i);
        }
    }
}

void Logger::log(LogLevel::Level level, std::shared_ptr<LogEvent> event){
    for (auto &i : this->ILogAppenders){
        i->log(level, event);
    }
}
void Logger::setFormatter(std::shared_ptr<LogFormatter> formatter){
    this->Formatter = formatter;
}

const std::string& Logger::getLogName()const{
    return this->LogName;
}
void Logger::debug(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::DEBUG, event);
}
void Logger::info(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::INFO, event);
}
void Logger::warn(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::WARN, event);
}
void Logger::error(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::ERROR, event);
}
void Logger::fatal(std::shared_ptr<LogEvent> event){
    this->log(LogLevel::FATAL, event);
}

void LogFormatter::initFormat(const std::string &pattern){
    //cmd fmt type
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nomalString;
    std::string cmd;
    std::string fmt;
    uint32_t length = pattern.size();
    for (uint32_t i = 0; i < length; i++){
        cmd.clear();
        fmt.clear();
        if (pattern[i] != '%'){
            nomalString.append(1, pattern[i]);
            continue;
        }
        uint32_t n = i + 1;
        uint32_t Status = 0;
        uint32_t Index = 0;
        if (pattern[n] == '%' && n <= length){
            nomalString.append(1, '%');
            i = n;
            continue;
        }
        while (n < length){
            if (!Status && !std::isalpha(pattern[n]) 
                    && pattern[n] != '{' && pattern[n] != '}'){
                cmd = pattern.substr(i + 1, n - i - 1);
                break;
            }
            if (Status == 0){
                if (pattern[n] == '{'){
                    cmd = pattern.substr(i + 1, n - i - 1);
                    Status = 1;
                    Index = n;
                    n++;
                    continue;
                }
            }
            if (Status == 1){
                if (pattern[n] == '}'){
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
            if (!nomalString.empty()){
                vec.push_back(std::make_tuple(nomalString, std::string(), 0));
                nomalString.clear();
            }
            vec.push_back(std::make_tuple(cmd, fmt, 1));
            i = n - 1;
        }
        else if (Status == 1){
            std::cout << "pattern error:" << pattern << "-" << pattern.substr(i) << std::endl;
        }
    }
    if (!nomalString.empty()){
        vec.push_back(std::make_tuple(nomalString, std::string(), 0));
        nomalString.clear();
    }

    std::map<std::string,std::function<std::shared_ptr<ILogFormatItem>(const std::string&)>> formatItem ={
#define Item(str,type) \
        {#str,[](const std::string& fmt){ return std::shared_ptr<ILogFormatItem>(new type);}}
#define ItemEx(str,type) \
        {#str,[](const std::string& fmt){ return std::shared_ptr<ILogFormatItem>(new type(fmt));}}

        Item(m, MessageFormatItem),           //m:消息
        Item(p, LevelFormatItem),             //p:日志级别
        Item(r, ElapseFormatItem),            //r:累计毫秒数
        Item(c, LogNameFormatItem),           //c:日志名称
        Item(t, ThreadIdFormatItem),          //t:线程id
        Item(n, NewLineFormatItem),           //n:换行
        Item(f, FilePathFormatItem),          //f:文件名
        Item(l, LineFormatItem),              //l:行号
        Item(T, TabFormatItem),               //T:Tab
        Item(F, FiberIdFormatItem),           //F:协程id
        Item(N, ThreadNameFormatItem),        //N:线程名称
        ItemEx(d, DateTimeFormatItem),          //d:时间
#undef ItemEx
#undef Item
    };

    for(auto &value : vec){

        uint32_t flag = std::get<2>(value);
        if(flag == 0){
            this->Items.push_back(std::shared_ptr<ILogFormatItem>(new StringFormatItem(std::get<0>(value))));
        }
        if(flag == 1){
            auto iter = formatItem.find(std::get<0>(value));
            if(iter == formatItem.end()){
                std::cout << "ERROR: Not Found Item" << std::endl;
            }else {
                this->Items.push_back(iter->second(std::get<1>(value)));
            }
        }
    }
}

void StdOutLogAppender::log(LogLevel::Level level,std::shared_ptr<LogEvent> event){
    if(!this->Formatter){
        std::cout << "error : this->Formatter" << std::endl;
        return;
    }
    this->Formatter->format(std::cout,level, event);
}
FileLogAppender::FileLogAppender(const std::string &path){
    this->reOpen();
}
void FileLogAppender::log(LogLevel::Level level, std::shared_ptr<LogEvent> event){
    this->Formatter->format(this->OutPutFile, level, event);
}

bool FileLogAppender::reOpen(){
    if (this->OutPutFile){
        OutPutFile.close();
    }
    this->OutPutFile.open(this->Path);
    return this->OutPutFile.is_open();
}

std::string LogFormatter::format(std::ostream &os, LogLevel::Level level, std::shared_ptr<LogEvent> event){
    for(auto &i : this->Items){
        i->format(os,level,event);
    }
	return std::string("");
}


} // namespace Log
