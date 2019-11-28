#include "Log.h"
#include <tuple>
#include <time.h>
#include <iostream>

namespace Magic {
	class Logger;
	class LogWrap;
	class LogEvent;
	class ILogAppender;
	class LogFormatter;
	class LoggerManager;
	class ILogFormatItem;
	class FileLogAppender;
	class StdOutLogAppender;
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
		:m_LogName{ name }{
	}

	void  Logger::addILogAppender(MagicPtr<ILogAppender>& logAppender) {
		MutexLock lock{ m_Mutex };
		if (!logAppender->m_Formatter) {
			logAppender->m_Formatter.reset(new LogFormatter{ m_Formatter });
		}
		this->m_ILogAppenders.push_back(std::move(logAppender));
	}

	void  Logger::delILogAppender(MagicPtr<ILogAppender>& logAppender) {
		MutexLock lock{ m_Mutex };
		auto &vBegin = this->m_ILogAppenders.begin();
		auto &vEnd = this->m_ILogAppenders.end();
		for (; vBegin != vEnd; vBegin++) {
			if (*vBegin == logAppender) {
				this->m_ILogAppenders.erase(vBegin);
				return;
			}
		}
	}

	void Logger::setFormatter(const std::string& pattern) {
		MutexLock lock{ m_Mutex };
		this->m_Formatter = pattern;
	}

	void Logger::setLevel(LogLevel val) {
		MutexLock lock{ m_Mutex };
		this->m_Level = val;
	}

	LogLevel Logger::getLevel() const {
		return this->m_Level;
	}

	const std::string& Logger::getLogName() const {
		return this->m_LogName;
	}

	void  Logger::log(LogLevel level, MagicPtr<LogEvent>& event) {
		if (level >= m_Level) {
			MutexLock lock{ m_Mutex };
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

	LogWrap::LogWrap(const LogLevel level,MagicPtr<Logger>& logger, MagicPtr<LogEvent>&& event)
		:m_Level{ level },
		m_Logger{ logger },
		m_Event{ std::move(event) } {
	}
	std::stringstream& LogWrap::get() {
		return this->m_Event->getStream();
	}
	LogWrap::~LogWrap() {
		this->m_Logger->log(this->m_Level, this->m_Event);
	}

	LogEvent::LogEvent(uint32_t line, uint64_t time, uint64_t elapse, uint64_t fiberId,
		uint64_t threadId, const std::string& file, const std::string& logName, const std::string& threadName)
		:m_Line{ line },
		m_Time{ time },
		m_Elapse{ elapse },
		m_FiberId{ fiberId },
		m_ThreadId{ threadId },
		m_File{ file },
		m_LogName{ logName },
		m_ThreadName{ threadName } {
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
	uint64_t LogEvent::getFiberId() const {
		return m_FiberId;
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
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class LevelFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class ElapseFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class LogNameFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class ThreadIdFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class NewLineFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class DateTimeFormatItem :public ILogFormatItem {
	public:
		explicit DateTimeFormatItem(const std::string& formatString = "%Y:%m:%d %H:%M:%S");
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	private:
		std::string m_FormatString;
	};

	class FilePathFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;

	};

	class LineFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class TabFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class FiberIdFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class ThreadNameFormatItem :public ILogFormatItem {
	public:
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	};

	class StringFormatItem :public ILogFormatItem {
	public:
		explicit StringFormatItem(const std::string& str) :m_Str(str) {}
		void format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) override;
	private:
		std::string m_Str;
	};

	void MessageFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getContent().c_str();
	}

	void LevelFormatItem::format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>&) {
		os << ToString(level);
	}

	void ElapseFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getElapse();
	}

	void LogNameFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getLogName();
	}

	void ThreadIdFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getThreadId();
	}

	void NewLineFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>&) {
		os << std::endl;
	}

	DateTimeFormatItem::DateTimeFormatItem(const std::string& formatString) :m_FormatString{ formatString } {
		if (this->m_FormatString.empty()) {
			this->m_FormatString.append("%Y:%m:%d %H:%M:%S");
		}
	}

	void DateTimeFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		time_t time_secounds{ static_cast<int32_t>(event->getTime()) };
		struct tm nowTime {};
#if defined(_WIN32) || defined(_WIN64)
		localtime_s(&nowTime, &time_secounds);
#else
		localtime_r(&time_secounds, &nowTime);
#endif
		char buf[1024]{ 0 };
		strftime(buf, sizeof(buf), this->m_FormatString.c_str(), &nowTime);
		os << buf;
	}

	void FilePathFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getFile().c_str();
	}

	void LineFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getLine();
	}

	void TabFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>&) {
		os << '\t';
	}

	void FiberIdFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getFiberId();
	}

	void ThreadNameFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>& event) {
		os << event->getThreadName().c_str();
	}

	void StringFormatItem::format(std::ostream& os, const LogLevel, const MagicPtr<LogEvent>&) {
		os << this->m_Str.c_str();
	}
	//###############################*END*##################################

	LogFormatter::LogFormatter(const std::string& pattern) {
		//cmd fmt flag
		std::vector<std::tuple<std::string, std::string, uint32_t>> vec{};
		std::string nomalString{};
		std::string cmd{};
		std::string fmt{};
		uint64_t length{ pattern.size() };
		for (uint32_t i{ 0 }; i < length; i++) {
			cmd.clear();
			fmt.clear();
			if (pattern.at(i) != '%') {
				nomalString.append(1, pattern.at(i));
				continue;
			}
			uint32_t n{ i + 1 };
			uint32_t Status{};
			uint32_t Index{};
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
					vec.push_back(std::make_tuple(nomalString, std::string{}, 0));
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
			vec.push_back(std::make_tuple(nomalString, std::string{}, 0));
			nomalString.clear();
		}

		static std::map<std::string, std::function<MagicPtr<ILogFormatItem>(const std::string&)>> formatItem{
	#define Item(str,type) \
	        {#str,[](const std::string&){ return MagicPtr<ILogFormatItem>{new type{}};}}
	#define ItemEx(str,type) \
	        {#str,[](const std::string& fmt){ return MagicPtr<ILogFormatItem>{new type{fmt}};}}
			Item(m, MessageFormatItem),            //m:消息
			Item(p, LevelFormatItem),              //p:日志级别
			Item(r, ElapseFormatItem),             //r:累计毫秒数
			Item(c, LogNameFormatItem),            //c:日志名称
			Item(t, ThreadIdFormatItem),           //t:线程id
			Item(n, NewLineFormatItem),            //n:换行
			Item(f, FilePathFormatItem),           //f:文件名
			Item(l, LineFormatItem),               //l:行号
			Item(T, TabFormatItem),                //T:Tab
			Item(F, FiberIdFormatItem),            //F:协程id
			Item(N, ThreadNameFormatItem),         //N:线程名称
			ItemEx(d, DateTimeFormatItem),         //d:时间
	#undef ItemEx
	#undef Item
		};

		for (auto& value : vec) {
			uint32_t flag{ std::get<2>(value) };
			if (flag == 0) {
				this->m_Items.push_back(MagicPtr<ILogFormatItem>{new StringFormatItem{ std::get<0>(value) }});
			}
			if (flag == 1) {
				auto &iter = formatItem.find(std::get<0>(value));
				if (iter == formatItem.end()) {
					std::cout << "<(LogError)>: Not Found Item" << std::endl;
				}
				else {
					this->m_Items.push_back(iter->second(std::get<1>(value)));
				}
			}
		}
	}

	void LogFormatter::format(std::ostream& os, const LogLevel level, const MagicPtr<LogEvent>& event) {
		for (auto& v : this->m_Items) {
			v->format(os, level, event);
		}
	}

	LoggerManager::LoggerManager() {
		m_Root.reset(new Logger{});
	}

	MagicPtr<Logger>& LoggerManager::getRoot() {
		return m_Root;
	}

	MagicPtr<Logger>& LoggerManager::getLogger(const std::string& name) {
		if (name == m_Root->m_LogName) {
			return m_Root;
		}
		auto it{ m_Loggers.find(name) };
		if (it != m_Loggers.end()) {
			return it->second;
		}
		m_Loggers[name].reset(new Logger{ name });
		return  m_Loggers[name];
	}

	FileLogAppender::FileLogAppender(const std::string& path) 
		:m_Path{ path } {
		this->reOpen();
	}

	void FileLogAppender::log(LogLevel level, MagicPtr<LogEvent>& event) {
		this->m_Formatter->format(this->m_FileStream, level, event);
	}

	bool FileLogAppender::reOpen() {
		MutexLock lock{ m_Mutex };
		if (this->m_FileStream) {
			this->m_FileStream.close();
		}
		this->m_FileStream.open(this->m_Path, std::ios_base::out | std::ios_base::app);
		return this->m_FileStream.is_open();
	}

	void StdOutLogAppender::log(LogLevel level, MagicPtr<LogEvent>& event) {
		if (!this->m_Formatter) {
			std::cout << "<(LogError)> "<< std::endl;
			return;
		}
		this->m_Formatter->format(std::cout, level, event);
	}
}