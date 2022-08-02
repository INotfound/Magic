/*
 * @Author: INotFound
 * @Date: 2020-12-17 16:24:54
 * @LastEditTime: 2020-12-17 17:56:19
 */
#include <iostream>
#include "Examples/WebServer/WebServer.h"
int main(){
    auto ioc = Web::Initialize([](const Safe<Magic::Container>& ioc){});
    ioc->resolve<Magic::NetWork::Http::HttpServer>()->run();
    return EXIT_SUCCESS;
}
