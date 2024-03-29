/*
 * @Author: INotFound
 * @Date: 2020-12-17 17:57:31
 * @LastEditTime: 2020-12-21 13:55:26
 */
#pragma once
#include <Magic/NetWork/Http/HttpServlet.hpp>

namespace Web{
    using namespace Magic::NetWork::Http;
    class TestServlet :public IHttpServlet{
    public:
        TestServlet();
        void test(const Safe<HttpSocket>& httpSocket);
    };
}
