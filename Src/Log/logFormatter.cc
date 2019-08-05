 #include "logFormatter.h"

#include <map>
#include <tuple>
#include <time.h>
#include <functional>
#include "logEvent.h"

using namespace Magic;
//format item interface
class MessageFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getContent().c_str();
    }
};

class LevelFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << LogLevel::toString(level);
    }
};

class ElapseFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getElapse();
    }
};

class LogNameFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getLogName();
    }
};

class ThreadIdFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getThreadId();
    }
};

class NewLineFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << std::endl;
    }
};

class DateTimeFormatItem :public ILogFormatItem{
public:
    explicit DateTimeFormatItem(const std::string &formatString ="%Y:%m:%d %H:%M:%S"):m_FormatString(formatString){
        if(this->m_FormatString.empty()){
            this->m_FormatString.append("%Y:%m:%d %H:%M:%S");
        }
    }
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        time_t time_secounds = event->getTime();
        struct tm nowTime;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&nowTime,&time_secounds);
#else
        localtime_r(&time_secounds,&nowTime);
#endif
		char buf[1024] = { 0 };
        strftime(buf,sizeof(buf),this->m_FormatString.c_str(),&nowTime);
        os << buf;
    }
private:
    std::string m_FormatString;
};

class FilePathFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getFile().c_str(); 
    }
};

class LineFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getLine(); 
    }
};

class TabFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << '\t';
    }
};

class FiberIdFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getFiberId();
    }
};

class ThreadNameFormatItem :public ILogFormatItem{
public:
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << event->getThreadName().c_str();
    }
};

class StringFormatItem :public ILogFormatItem{
public:
    explicit StringFormatItem(const std::string & str):m_Str(str){}
    void format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event) override{
        os << this->m_Str.c_str(); 
    }
private:
    std::string m_Str; 
};



LogFormatter::LogFormatter(const std::string& pattern){
    //cmd fmt flag
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nomalString;
    std::string cmd;
    std::string fmt;
    uint32_t length = pattern.size();
    for (uint32_t i = 0; i < length; i++){
        cmd.clear();
        fmt.clear();
        if (pattern.at(i) != '%'){
            nomalString.append(1, pattern.at(i));
            continue;
        }
        uint32_t n = i + 1;
        uint32_t Status = 0;
        uint32_t Index = 0;
        if (pattern.at(n) == '%' && n <= length){
            nomalString.append(1, '%');
            i = n;
            continue;
        }
        while (n < length){
            if (!Status && !std::isalpha(pattern.at(n)) 
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

    static std::map<std::string,std::function<std::unique_ptr<ILogFormatItem>(const std::string&)>> formatItem ={
#define Item(str,type) \
        {#str,[](const std::string& fmt){ return std::unique_ptr<ILogFormatItem>(new type);}}
#define ItemEx(str,type) \
        {#str,[](const std::string& fmt){ return std::unique_ptr<ILogFormatItem>(new type(fmt));}}

        Item(m, MessageFormatItem),            //m:消息
        Item(p, LevelFormatItem),                    //p:日志级别
        Item(r, ElapseFormatItem),                  //r:累计毫秒数
        Item(c, LogNameFormatItem),            //c:日志名称
        Item(t, ThreadIdFormatItem),              //t:线程id
        Item(n, NewLineFormatItem),              //n:换行
        Item(f, FilePathFormatItem),                //f:文件名
        Item(l, LineFormatItem),                        //l:行号
        Item(T, TabFormatItem),                        //T:Tab
        Item(F, FiberIdFormatItem),                  //F:协程id
        Item(N, ThreadNameFormatItem),       //N:线程名称
        ItemEx(d, DateTimeFormatItem),         //d:时间
#undef ItemEx
#undef Item
    };

    for(auto &value : vec){

        uint32_t flag = std::get<2>(value);
        if(flag == 0){
            this->m_Items.push_back(std::move(std::unique_ptr<ILogFormatItem>(new StringFormatItem(std::get<0>(value)))));
        }
        if(flag == 1){
            auto iter = formatItem.find(std::get<0>(value));
            if(iter == formatItem.end()){
                std::cout << "<(ERROR)>: Not Found Item" << std::endl;
            }else {
                this->m_Items.push_back(std::move(iter->second(std::get<1>(value))));
            }
        }
    }
}

void LogFormatter::format(std::ostream &os, LogLevel::Level level,std::unique_ptr<LogEvent>& event){
	for(auto &v :this->m_Items){
		v->format(os,level,event);
	}
}
