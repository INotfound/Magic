#include <iostream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "Magic/Magic.h"

void momo1() {
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo1 Begin";
	Magic::Fiber::YieldToHold();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo1 End";
}

void momo(){
	Magic::Fiber::Init();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "_____________________";
	Ptr<Magic::Fiber> subfiber(new Magic::Fiber(&momo1,true));
	subfiber->toCall();
	subfiber->toCall();
}

int main(){
	Magic::Init();
	MAGIC_LOG_ROOT();
	Ptr<Magic::Thread> thread(new Magic::Thread("test",momo));
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "Start Fiber";
	thread->join();
	Ptr<Magic::Thread> threadx;
	IsPointer(threadx);
	getchar();
	return 0;
}

