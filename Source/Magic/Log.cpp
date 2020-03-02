#include "Log.h"
#include <tuple>
#include <time.h>
#include <iostream>
#include "Adapter.h"
namespace Magic {

    static const std::string g_HtmlTemplate{R"Template(
        <?xml version="1.0" encoding="utf-8" standalone="yes"?>
        <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
        <html>
        <head>
            <title>Logger</title>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <style type="text/css" id="logCss">
                body {
                    background: #18242b;
                    color: #afc6d1;
                    margin-right: 20px;
                    margin-left: 20px;
                    font-size: 14px;
                    font-family: Arial, sans-serif, sans;
                }
                a {
                    text-decoration: none;
                }

                a:link {
                    color: #a0b2bb;
                }

                a:active {
                    color: #f59504;
                }

                a:visited {
                    color: #adc7d4;
                }

                a:hover {
                    color: #e49115;
                }

                h1 {
                    text-align: center;
                }

                h2 {
                    color: #ebe5e5;
                }

                .LogDebug,
                .LogWarn,
                .LogError,
                .LogFatal,
                .LogInfo

                .LogDebug {
                    background-color: #0f1011;
                    color: #a8c1ce;
                }

                .LogInfo {
                    background-color: #294453;
                    color: #a8c1ce;
                }

                .LogWarn {
                    background-color: #7993a0;
                    color: #1b2329;
                }

                .LogError {
                    background-color: #ff952b;
                    color: #1d2930;
                }

                .LogFatal {
                    background-color: #fc0808;
                    color: #19242b;
                }
            </style>
        </head>

        <body>
            <h1> Log File </h1>
            <script type="text/JavaScript">
                function objHide(obj) {
                    obj.style.display="none"
                }
                function objShow(obj) {
                    obj.style.display="block"
                }
                function selectType() {
                    var sel = document.getElementById("typeSelect");
                    const hideList = new Set(['LogDebug', 'LogInfo', 'LogWarn', 'LogError', 'LogFatal']);
                    if (sel.value === 'a') {
                        hideList.forEach(element => {
                            var list = document.querySelectorAll('.' + element);
                            list.forEach(objShow);
                        });
                    } else {
                        var ss = hideList;
                        ss.delete(sel.value);
                        ss.forEach(element => {
                            var list = document.querySelectorAll('.' + element);
                            list.forEach(objHide);
                        });
                        var showList = document.querySelectorAll('.' + sel.value);
                        showList.forEach(objShow);
                    }
                }
            </script>
            <select id="typeSelect" onchange="selectType()">
                <option value='a' selected="selected">All</option>
                <option value='LogDebug'>Debug</option>
                <option value='LogInfo'>Info</option>
                <option value='LogWarn'>Warn</option>
                <option value='LogError'>Error</option>
                <option value='LogFatal'>Fatal</option>
            </select>
        )Template" 
    };

    const char* ToString(const LogLevel  level) {
        switch (level)
        {
#define LEVEL(Name)\
        case LogLevel::Name: \
            return #Name;
            LEVEL(LogDebug)
            LEVEL(LogInfo)
            LEVEL(LogWarn)
            LEVEL(LogError)
            LEVEL(LogFatal)
#undef LEVEL
        }
        return "<(LogError)>";
    }

    Logger::Logger(const std::string& name)
        :m_Level(LogLevel::LogDebug)
        ,m_LogName(name){
    }

    void  Logger::addILogAppender(Safe<ILogAppender>& logAppender) {
        Mutex::Lock lock(m_Mutex);
        if (!logAppender->m_Formatter) {
            logAppender->m_Formatter.reset(new LogFormatter(m_Formatter));
        }
        this->m_ILogAppenders.push_back(std::move(logAppender));
    }

    void  Logger::delILogAppender(Safe<ILogAppender>& logAppender) {
        Mutex::Lock lock(m_Mutex);
        auto vBegin = this->m_ILogAppenders.begin();
        auto vEnd = this->m_ILogAppenders.end();
        for (; vBegin != vEnd; vBegin++) {
            if (*vBegin == logAppender) {
                this->m_ILogAppenders.erase(vBegin);
                return;
            }
        }
    }

    void Logger::setFormatter(const std::string& pattern) {
        Mutex::Lock lock(m_Mutex);
        this->m_Formatter = pattern;
    }

    void Logger::setLevel(LogLevel val) {
        Mutex::Lock lock(m_Mutex);
        this->m_Level = val;
    }

    LogLevel Logger::getLevel() const {
        return this->m_Level;
    }

    const std::string& Logger::getLogName() const {
        return this->m_LogName;
    }

    void  Logger::log(LogLevel level,const Safe<LogEvent>& event) {
        if (level >= m_Level) {
            Mutex::Lock lock(m_Mutex);
            if (!this->m_ILogAppenders.empty()) {
                for (auto& v : this->m_ILogAppenders) {
                    v->log(level, event);
                }
            }
            else if (LoggerMgr::GetInstance()->getRoot()) {
                LoggerMgr::GetInstance()->getRoot()->log(level, event);
            }
        }
    }

    LogWrap::LogWrap(const LogLevel level, Safe<LogEvent>&& event ,const Safe<Logger>& logger)
        :m_Level(level)
        ,m_Event(std::move(event))
        ,m_Logger(logger){
    }
    std::stringstream& LogWrap::get() {
        return this->m_Event->getStream();
    }
    LogWrap::~LogWrap() {
        this->m_Logger->log(this->m_Level, this->m_Event);
    }

    LogEvent::LogEvent(uint32_t line, uint64_t time, uint64_t elapse,uint64_t threadId, 
        const std::string& file, const std::string& logName, const std::string& threadName)
        :m_Line(line),
        m_Time(time),
        m_Elapse(elapse),
        m_ThreadId(threadId),
        m_File(file),
        m_LogName(logName),
        m_ThreadName(threadName) {
    }
    uint32_t LogEvent::getLine()    const {
        return m_Line;
    }
    uint64_t LogEvent::getTime()    const {
        return m_Time;
    }
    uint64_t LogEvent::getElapse()  const {
        return m_Elapse;
    }
    uint64_t LogEvent::getThreadId()                const {
        return m_ThreadId;
    }
    const std::string  LogEvent::getContent()       const {
        return m_StringStream.str();
    }
    const std::string& LogEvent::getFile()          const {
        return m_File;
    }
    const std::string& LogEvent::getLogName()       const {
        return m_LogName;
    }
    const std::string& LogEvent::getThreadName()    const {
        return m_ThreadName;
    }
    std::stringstream& LogEvent::getStream() {
        return m_StringStream;
    }

//###############################BEGIN##################################
    ILogFormatItem::~ILogFormatItem() {}

    class MessageFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class LevelFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class ElapseFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class LogNameFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class ThreadIdFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class NewLineFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class DateTimeFormatItem :public ILogFormatItem {
    public:
        explicit DateTimeFormatItem(const std::string& formatString = "%Y:%m:%d %H:%M:%S");
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    private:
        std::string m_FormatString;
    };

    class FilePathFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;

    };

    class LineFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class TabFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class ThreadNameFormatItem :public ILogFormatItem {
    public:
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    };

    class StringFormatItem :public ILogFormatItem {
    public:
        explicit StringFormatItem(const std::string& str) :m_String(str) {}
        void format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) override;
    private:
        std::string m_String;
    };

    void MessageFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getContent().c_str();
    }

    void LevelFormatItem::format(std::ostream& os, const LogLevel level, const Safe<LogEvent>&) {
        os << ToString(level);
    }

    void ElapseFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getElapse();
    }

    void LogNameFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getLogName();
    }

    void ThreadIdFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getThreadId();
    }

    void NewLineFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>&) {
        os << std::endl;
    }

    DateTimeFormatItem::DateTimeFormatItem(const std::string& formatString) :m_FormatString(formatString) {
        if (this->m_FormatString.empty()) {
            this->m_FormatString.append("%Y:%m:%d %H:%M:%S");
        }
    }

    void DateTimeFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        time_t time_secounds = static_cast<int32_t>(event->getTime());
        os << Magic::TimeToString(time_secounds,this->m_FormatString).c_str();
    }

    void FilePathFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getFile().c_str();
    }

    void LineFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getLine();
    }

    void TabFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>&) {
        os << '\t';
    }

    void ThreadNameFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>& event) {
        os << event->getThreadName().c_str();
    }

    void StringFormatItem::format(std::ostream& os, const LogLevel, const Safe<LogEvent>&) {
        os << this->m_String.c_str();
    }
    //###############################*END*##################################

    LogFormatter::LogFormatter(const std::string& pattern) {
        //cmd fmt flag
        std::string cmd;
        std::string fmt;
        std::string nomalString;
        std::vector<std::tuple<std::string, std::string, uint32_t>> vec;
        uint64_t length = pattern.size();
        for (uint32_t i = 0; i < length; i++) {
            cmd.clear();
            fmt.clear();
            if (pattern.at(i) != '%') {
                nomalString.append(1, pattern.at(i));
                continue;
            }
            uint32_t n = i + 1;
            uint32_t Status = 0;
            uint32_t Index = 0;
            if (n < length && pattern.at(n) == '%') {
                nomalString.append(1, '%');
                i = n;
                continue;
            }
            while (n < length) {
                if (!Status && !isalpha(pattern.at(n))
                    && pattern.at(n) != '{' && pattern.at(n) != '}') {
                    cmd = pattern.substr(i + 1, n - i - 1);
                    break;
                }
                if (Status == 0) {
                    if (pattern.at(n) == '{') {
                        cmd = pattern.substr(i + 1, n - i - 1);
                        Status = 1;
                        Index = n;
                        n++;
                        continue;
                    }
                }
                if (Status == 1) {
                    if (pattern.at(n) == '}') {
                        fmt = pattern.substr(Index + 1, n - Index - 1);
                        Status = 0;
                        n++;
                        break;
                    }
                }
                n++;
                if (n == length) {
                    if (cmd.empty()) {
                        cmd = pattern.substr(i + 1);
                    }
                }
            }
            if (Status == 0) {
                if (!nomalString.empty()) {
                    vec.push_back(std::make_tuple(nomalString, std::string(), 0));
                    nomalString.clear();
                }
                vec.push_back(std::make_tuple(cmd, fmt, 1));
                i = n - 1;
            }
            else if (Status == 1) {
                std::cout << "pattern error:" << pattern << "-" << pattern.substr(i) << std::endl;
            }
        }
        if (!nomalString.empty()) {
            vec.push_back(std::make_tuple(nomalString, std::string(), 0));
            nomalString.clear();
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

        for (auto& value : vec) {
            uint32_t flag = std::get<2>(value);
            if (flag == 0) {
                this->m_Items.push_back(Safe<ILogFormatItem>(new StringFormatItem(std::get<0>(value))));
            }
            if (flag == 1) {
                auto iter = formatItem.find(std::get<0>(value));
                if (iter == formatItem.end()) {
                    std::cout << "<(LogError)>: Not Found Item" << std::endl;
                }
                else {
                    this->m_Items.push_back(iter->second(std::get<1>(value)));
                }
            }
        }
    }

    void LogFormatter::format(std::ostream& os, const LogLevel level, const Safe<LogEvent>& event) {
        for (auto& v : this->m_Items) {
            v->format(os, level, event);
        }
    }

    LoggerManager::LoggerManager() {
        m_Root.reset(new Logger);
    }

    const Safe<Logger>& LoggerManager::getRoot() {
        return m_Root;
    }

    const Safe<Logger>& LoggerManager::getLogger(const std::string& name) {
        if (name == m_Root->m_LogName) {
            return m_Root;
        }
        auto it = m_Loggers.find(name);
        if (it != m_Loggers.end()) {
            return it->second;
        }
        m_Loggers[name].reset(new Logger(name));
        return  m_Loggers[name];
    }

    FileLogAppender::FileLogAppender(const std::string& path) 
        :m_Path(path) {
        Mutex::Lock lock(m_Mutex);
        this->m_FileStream.open(this->m_Path, std::ios_base::out | std::ios_base::app);
    }

    void FileLogAppender::log(LogLevel level,const Safe<LogEvent>& event) {
        this->m_Formatter->format(this->m_FileStream, level, event);
    }

    HtmlLogAppender::HtmlLogAppender(const std::string& path)
        :m_Path(path) {
        Mutex::Lock lock(m_Mutex);
        if (IS_FILE(path.c_str()) != 0) {
            this->m_FileStream.open(this->m_Path, std::ios_base::out | std::ios_base::app);
            this->m_FileStream << g_HtmlTemplate.c_str();
        }
        else {
            this->m_FileStream.open(this->m_Path, std::ios_base::out | std::ios_base::app);
        }
    }

    void HtmlLogAppender::log(LogLevel level,const Safe<LogEvent>& event){
        this->m_FileStream << "<div class=\"" << ToString(level) << "\">";
        this->m_Formatter->format(this->m_FileStream, level, event);
        this->m_FileStream << "</div>";
        this->m_FileStream.flush();
    }

    void StdOutLogAppender::log(LogLevel level,const Safe<LogEvent>& event) {
        if (!this->m_Formatter) {
            std::cout << "<(LogError)> "<< std::endl;
            return;
        }
        this->m_Formatter->format(std::cout, level, event);
    }

}