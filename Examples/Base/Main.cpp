/*
 * @Author: INotFound
 * @Date: 2020-12-17 11:52:32
 * @LastEditTime: 2021-02-01 23:47:05
 */
#include "Examples/Base/Base.h"

int main(){
    Base::Initialize([](const Safe<Magic::Container>& ioc){
    });
    return EXIT_SUCCESS;
}
