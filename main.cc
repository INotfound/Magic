#include <iostream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <vector>
#include "Magic/Magic.h"

static auto& g_log = MAGIC_LOG_NAME("system");

void momo(){
	MAGIC_LOG_INFO(g_log) << Magic::BackTraceToString(10);
}

int main(){
	Magic::Init();
	momo();
	return 0;
}

