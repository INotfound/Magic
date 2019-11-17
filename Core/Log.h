#pragma once
#include <list>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "Core.h"
#include "Util.h"
#include "Mutex.h"

namespace Magic{
	enum class LogLevel;
	class Logger;
	class LogWrap;
	class LogEvent;
	class ILogAppender;
	class LogFormatter;
	class LoggerManager;
	class ILogFormatItem;
	class FileLogAppender;
	class StdOutLogAppender;

	enum class LogLevel {
		LogDebug = 1,
		LogInfo = 2,
		LogWarn = 3,
		LogError = 4,
		LogFatal = 5
	};
	const char* ToString(const LogLevel level);

	class Logger {
		friend class LoggerManager;
	public:
		explicit Logger(const std::string& name = "root");
		void setLevel(LogLevel);
		LogLevel getLevel() const;
		const std::string& getLogName() const;
		void setFormatter(const std::string& pattern);
		void log(LogLevel level, MagicPtr<LogEvent>& event);
		void addILogAppender(MagicPtr<ILogAppender>& logAppender);
		void delILogAppender(MagicPtr<ILogAppender>& logAppender);
	private:
		MutexType m_Mutex{};
		std::string m_LogName{};
		std::string m_Formatter{};
		LogLevel m_Level{ LogLevel::LogDebug };
		std::list<MagicPtr<ILogAppender>> m_ILogAppenders{};
	};

	class LogWrap {
	public:
		LogWrap(const LogLevel level ,MagicPtr<Logger>& logger, MagicPtr<LogEvent>&& event);
		std::stringstream& get();
		~LogWrap();
	private:
		LogLevel m_Level{};
		MagicPtr<Logger>& m_Logger;
		MagicPtr<LogEvent> m_Event{};
	};

	class LogEvent {
	public:
		LogEvent(uint32_t line, uint64_t time, uint64_t elapse, uint64_t fiberId,
			uint64_t threadId, const std::string& file, const std::string& LogName, const std::string& threadName);
		uint32_t getLine()                  const;
		uint64_t getTime()                  const;
		uint64_t getElapse()                const;
		uint64_t getFiberId()               const;
		uint64_t getThreadId()              const;
		const std::string  getContent()     const;
		const std::string& getFile()        const;
		const std::string& getLogName()     const;
		const std::string& getThreadName()  const;
		std::stringstream& getStream();
	private:
		uint32_t m_Line{};
		uint64_t m_Time{};
		uint64_t m_Elapse{};
		uint64_t m_FiberId{};
		uint64_t m_ThreadId{};
		std::string m_File{};
		std::string m_LogName{};
		std::string m_ThreadName{};
		std::stringstream m_StringStream{};
	};

	class ILogAppender {
		friend class Logger;
	public:
		virtual ~ILogAppender() {};
		virtual void log(LogLevel level, MagicPtr<LogEvent>& event) = 0;
	protected:
		MagicPtr<LogFormatter> m_Formatter{};
	};

	class LogFormatter {
	public:
		explicit LogFormatter(const std::string& pattern);
		void format(std::ostream& os, const LogLevel  level, const MagicPtr<LogEvent>& event);
	private:
		std::vector<MagicPtr<ILogFormatItem>> m_Items{};
	};

	class LoggerManager {
	public:
		LoggerManager();
		MagicPtr<Logger>& getRoot();
		MagicPtr<Logger>& getLogger(const std::string& name);
	private:
		MagicPtr<Logger> m_Root{};
		std::map<std::string, MagicPtr<Logger>> m_Loggers{};
	};
	typedef Singleton<LoggerManager>        LoggerMgr;

	class ILogFormatItem {
	public:
		virtual ~ILogFormatItem();
		virtual void format(std::ostream& os, const LogLevel  level, const MagicPtr<LogEvent>& event) = 0;
	};

	class FileLogAppender :public ILogAppender {
	public:
		explicit FileLogAppender(const std::string& path);
		void log(LogLevel level, MagicPtr<LogEvent>& event) override;
		bool reOpen();
	private:
		MutexType m_Mutex{};
		std::string m_Path{};
		std::ofstream m_FileStream{};
	};

	class StdOutLogAppender :public ILogAppender {
	public:
		void log(LogLevel level, MagicPtr<LogEvent>& event) override;
	};
}