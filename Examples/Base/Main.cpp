/*
 * @Author: INotFound
 * @Date: 2020-12-17 11:52:32
 * @LastEditTime: 2021-02-01 23:47:05
 */
#include "Base.h"

int main(){
    Safe<Magic::Application> application = std::make_shared<Magic::Application>();
    application->initialize();
    return EXIT_SUCCESS;
}
