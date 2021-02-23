/*
 * @Author: INotFound
 * @Date: 2021-02-01 22:04:06
 * @LastEditTime: 2021-02-01 23:34:43
 */
#include "Magic/Core/Container.h"

namespace Magic{
    Safe<Container> g_Container = std::make_shared<Container>();
    const Safe<Container>& Configure(const std::function<void(const Safe<Container>&)>& configure){
        configure(g_Container);
        return g_Container;
    }
}
