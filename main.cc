#include "Magic/Core/Config.h"
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


void func() {
	MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "ProcessorsNumber:" << Magic::GetProcessorsNumber();
    MagicPtr<Magic::ConfigFile> configFile(new Magic::ConfigFile("config.ini"));
    Magic::MagicConfig::GetInstance()->addConfigFile(configFile);
    int port = Magic::MagicConfig::GetInstance()->at<int>("NetWork",2020, "#this test");
    std::cout << "get network port: " << port << std::endl;
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
