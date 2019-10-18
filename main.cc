#include "Magic/Core/Core.h"
#include "Magic/Magic.h"
#include <bits/stdint-uintn.h>
#include <cctype>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

enum class MusicState {
	UnReady,
	Ready,
	Playing,
	Pause,
	Stop
};

enum class PlayerEvent {
	Init,
	Select,
	Play,
	Pause,
	Stop
};

enum class TestState {
	Init,
	Run
};

enum class TestStateEvent {
	InitToRun,
	RunToInit
};
class ConfigValue{
public:
    ConfigValue(const std::string& value,const std::string& comment)
        :m_Value(value),m_Comment(comment){
        }
    void write(std::ostream& os);
private:
    std::string m_Value;
    std::string m_Comment;
};

class ConfigFile{
public:
    ConfigFile(const std::string& path){
    }
    void write(const ConfigValue& config);
private:
    std::string m_FilePath;
    std::ofstream m_File;
};

class Config{
public:
    Config(){}
    template<class T>
    T& at(const std::string& name,const std::string& defaultValue,const std::string& defaultComment){
        return m_ConfigMap[name];
    }
private:
    template<class T>
    std::string asString(const T& value){
        m_FormatStream.clear();
        m_FormatStream << value;
        return m_FormatStream.str();
    }
    template<class T>
    T stringAs(const std::string& value){
        m_FormatStream.clear();
        T temp;
        m_FormatStream << value;
        m_FormatStream >> temp;
        return temp;
    }
    template<>
    inline std::string stringAs<std::string>(const std::string& value){
        return value;
    }
    template<>
    bool stringAs<bool>(const std::string& value){
        bool isOk = true;
        std::string tValue = value;
        {
            auto begin = tValue.begin();
            auto end   = tValue.end();
            for (;begin!=end;begin++)
                *begin = std::toupper(*begin);
        }
        if(tValue==std::string("FALSE") || tValue==std::string("NO") ||  tValue==std::string("0"))
            isOk = false;
        return isOk;
    }
private:
    std::stringstream m_FormatStream;
    std::map<std::string,MagicPtr<ConfigValue>> m_ConfigMap;
 };





void parse(std::string& str){
    std::map<std::string,std::string> keyValue;
    std::string normalString;
    std::string valueString;
	bool isInfo = false;
	bool isValue = false;
	bool isComment = false;
    uint32_t length = 0;
	for (uint32_t i = 0; i < length; i++) {
		std::string::value_type charValue = str.at(i);
		//Comment
		if (charValue == '#') {
			isComment = true;
		}else if (isComment == true) {
			if (charValue != '\n') {
				continue;
			}
			else {
				isComment = false;
				continue;
			}
		}
		//empty
		if (charValue == ' ' || charValue == '\r')
			continue;

		//normalSting and value(n = v)
 		if ((charValue == '\n' || i == (length - 1)) && isValue) {
			isValue = false;
			if (i == (length - 1) && charValue != '\n' )
				valueString.append(1, charValue);
			keyValue[normalString] = valueString;
			normalString.clear();
			valueString.clear();
			continue;
		}else if (isValue == true) {
			valueString.append(1, charValue);
			continue;
		}else if (charValue == '=') {
			isValue = true;
			continue;
		}

		normalString.append(1, charValue);
	}
}

void func() {
	MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "ProcessorsNumber:" << Magic::GetProcessorsNumber();

	std::string str = "nihao=23333\nhhh=2333333\nnihaoya=hhhhh\n";
	parse(str);
}

int main(void) {
	Magic::Init();
	StateMachine<TestState, TestStateEvent,void()> State(TestState::Init);
	State.addFunc(TestStateEvent::InitToRun, TestState::Run, func);
	State.invoke(TestStateEvent::InitToRun);

	getchar();
	return -1;
}






////Magic::RWMutex mutex;
////Magic::Mutex mutex1;
//Magic::Spinlock spinlock;
//void run(){
//	//Magic::RWMutex::WriteLock lock(mutex);
//	//Magic::Mutex::Lock lock(mutex1);
//	{
//		Magic::Spinlock::Lock lock(spinlock);
//		MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "hello world1";
//	}
//}
//
//int main(void){
//    Magic::Init();
//	std::vector<MagicPtr<Magic::Thread>> vec;
//	{
//		for (size_t i = 0; i < 500; i++)
//		{
//			vec.push_back(MagicPtr<Magic::Thread>(new Magic::Thread("test_" + std::to_string(i), run)));
//		}
//	}
//	getchar();
//    return 0;
//}
//
