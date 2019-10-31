#include "Magic/Magic.h"
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
	MAGIC_CONFIG()->at<uint32_t>("test", 123456789, "this is test!");
	uint32_t i = MAGIC_CONFIG()->at<uint32_t>("test1", 1234);
	std::cout << i;
}

int main(void) {
	Magic::Init();
	StateMachine<TestState, TestStateEvent,void()> State(TestState::Init);
	State.addFunc(TestStateEvent::InitToRun, TestState::Run, func);
	State.invoke(TestStateEvent::InitToRun);

	std::getchar();
	return -1;
}





