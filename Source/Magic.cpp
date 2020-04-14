/*
 * @file: Magic.cpp
 * @Author: INotFound
 * @Date: 2020-02-23 21:36:52
 * @LastEditTime: 2020-03-15 17:57:29
 */
#include "Magic.h"
namespace Magic {
    void Init(const std::string& Name){
        std::srand(time(0));
        Magic::Thread::SetName("Main");
        //Config
        Safe<Magic::ConfigFile> configFile(
            new Magic::ConfigFile(Name + ".conf")
        );
        Safe<Magic::IConfigFormatter> jsonConfigFormatter(
                    new Magic::JsonConfigFormatter
        );
        configFile->addFormatter(jsonConfigFormatter);
        Magic::Config::GetInstance()->addConfigFile(configFile);

        //Log
        Safe<ILogAppender> logHtmlAppender(
            new HtmlLogAppender(Magic::Config::GetInstance()->at<std::string>("LogPath", Name + ".html"))
        );
        Safe<ILogAppender> logStdOutAppender(new StdOutLogAppender);

        MAGIC_LOG_ROOT()->setFormatter(
            Magic::Config::GetInstance()->at<std::string>("LogFormater", "[%p]%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T[%c]%T%f:%l%T%m%n")
        );

        MAGIC_LOG_ROOT()->addILogAppender(logHtmlAppender);
        MAGIC_LOG_ROOT()->addILogAppender(logStdOutAppender);
        MAGIC_INFO() << "Magic initialized successfully";
    }
}
