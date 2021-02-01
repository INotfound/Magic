/*
 * @Author: INotFound
 * @Date: 2020-12-17 11:52:32
 * @LastEditTime: 2021-02-01 23:47:05
 */
#include "Base.h"

int main(){
    Base::Initialize([](const Safe<Magic::Container>& ioc){
        Magic::g_Logger = ioc->resolve<Magic::Logger>();
    });
    return EXIT_SUCCESS;
}