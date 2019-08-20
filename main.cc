#include <iostream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "Magic/Magic.h"

void momo(){
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo Begin";
	Magic::Fiber::YieldToHold();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo End";

}

int main(){
	Magic::Init();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "Main Begin";
	std::unique_ptr<Magic::Fiber> superfiber(new Magic::Fiber());

	std::unique_ptr<Magic::Fiber> subfiber(new Magic::Fiber(&momo));
	subfiber->YieldToReady();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "Main after swapIn";
	subfiber->YieldToReady();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "Main End";
	return 0;
}

