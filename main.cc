#include "Magic/Magic.h"
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

void parse(std::string str) {
	std::string infoString;
	std::string normalString;
	std::string valueString;
	std::map<std::string, std::string> keyValue;
	std::map<std::string, std::map<std::string, std::string>> configMap;
	uint32_t length = str.length();
	uint32_t i = 0;
	bool isInfo = false;
	bool isValue = false;
	bool isComment = false;
	for (; i < length; i++) {
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
		//title(info)String
		if (charValue == ']') {
			isInfo = false;
			continue;
		}else if (isInfo == true) {
			infoString.append(1, charValue);
			continue;
		}else if (charValue == '[') {
			isInfo = true;
			if (!infoString.empty()) {
				configMap[infoString] = keyValue;
				infoString.clear();
				keyValue.clear();
			}
			continue;
		}
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

		if (charValue == '\n')
			continue;
		normalString.append(1, charValue);
	}
	if (!infoString.empty()) {
		configMap[infoString] = keyValue;
		infoString.clear();
		keyValue.clear();
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
