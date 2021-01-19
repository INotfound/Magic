/*
 * @Author: INotFound
 * @Date: 2020-09-16 16:30:35
 * @LastEditTime: 2020-12-29 02:49:37
 */
#include "Magic.h"

namespace Magic{
    static Safe<Container> g_Container = std::make_shared<Container>();
    const Safe<Container>& Configure(const std::function<void(const Safe<Container>&)>& configure){
        Thread::SetName("Master");
        configure(g_Container);
        return g_Container;
    }
}