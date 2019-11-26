#include "../Magic.h"
#include <iostream>
int main() {
	Magic::Init();
	int64_t s = MAGIC_CONFIG()->at("test",12456789);
	std::cout << s;
	std::getchar();
	return 0;
}