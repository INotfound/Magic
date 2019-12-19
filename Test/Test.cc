#include "../Magic.h"
#include <iostream>

int main() {
	Magic::Init("test");
	int64_t s = MAGIC_CONFIG()->at<int64_t>("test",124567891,"233333333");
	std::cout << s;
	std::getchar();
	return 0;
}