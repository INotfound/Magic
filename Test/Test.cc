/*
 * @file: Test.cpp
 * @Author: INotFound
 * @Date: 2020-03-12 01:59:04
 * @LastEditTime: 2020-03-19 23:25:06
 */
#include <iostream>
#include "Magic.h"

int main() {
    Magic::Init("Build_Test");
    Magic::Http::WebSocketServer server("0.0.0.0",8181,1,6000);
    server.run();
    getchar();
    return 0;
}


