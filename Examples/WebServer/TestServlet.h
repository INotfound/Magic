/*
 * @Author: INotFound
 * @Date: 2020-12-17 17:57:31
 * @LastEditTime: 2020-12-21 13:55:26
 */
#pragma once
#include <Magic/NetWork/Http/HttpServlet.h>

namespace Web{
    class TestServlet :public Magic::NetWork::Http::IHttpServlet{
    public:
        TestServlet();
        bool handle(const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response) override;
    };
}
