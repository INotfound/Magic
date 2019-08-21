#include <iostream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "Magic/Magic.h"

void momo(){
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo Begin";
	Magic::Fiber::YieldToBack();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo End";
}
void momo1() {
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo1 Begin";
	Magic::Fiber::YieldToBack();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "momo1 End";
}
int main(){
	Magic::Init();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "Main Begin";
	std::unique_ptr<Magic::Fiber> superfiber(new Magic::Fiber());
	std::unique_ptr<Magic::Fiber> subfiber(new Magic::Fiber(&momo));
	std::unique_ptr<Magic::Fiber> subfiber1(new Magic::Fiber(&momo1));
	Magic::Fiber::YieldToCall();
	Magic::Fiber::YieldToCall();
	MAGIC_LOG_INFO(MAGIC_LOG_ROOT()) << "Main End";
	return 0;
}

