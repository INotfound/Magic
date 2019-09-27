#include "Magic/Magic.h"
#include <vector>
#include <string>
#include <iostream>
Magic::RWMutex mutex;
//Magic::Mutex mutex1;
void run(){
	Magic::RWMutex::WriteLock lock(mutex);
	//Magic::Mutex::Lock lock(mutex1);
	std::cout << "hello world1" << std::endl;
	std::cout.flush();
}

int main(void){
    Magic::Init();
	std::vector<MagicPtr<Magic::Thread>> vec;
	{
		for (size_t i = 0; i < 500; i++)
		{
			vec.push_back(MagicPtr<Magic::Thread>(new Magic::Thread("test_" + std::to_string(i), run)));
		}
	}
	getchar();
    return 0;
}