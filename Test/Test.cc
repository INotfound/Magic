#include "../Magic.h"
#include <iostream>

int main() {
	Magic::Init();

	int64_t s = MAGIC_CONFIG()->revise("test",124567891,"233333333");
	std::cout << s;
	std::getchar();
	return 0;
}