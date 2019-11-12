#include "Magic/Magic.h"
#include <cctype>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

void func() {
	MAGIC_LOG_DEBUG(MAGIC_LOG_ROOT()) << "ProcessorsNumber:" << Magic::GetProcessorsNumber();
	MAGIC_CONFIG()->at<uint32_t>("test", 123456789, "this is test!");
	uint64_t i = MAGIC_CONFIG()->at<uint64_t>("test1", 1234);
	std::cout << i;
}

int main(void) {
	Magic::Init();
	Magic::Thread thread{"Test",&func};
	std::getchar();
	return -1;
}





