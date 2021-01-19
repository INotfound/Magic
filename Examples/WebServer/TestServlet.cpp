/*
 * @Author: INotFound
 * @Date: 2020-12-17 17:57:35
 * @LastEditTime: 2020-12-21 14:02:57
 */
#include "TestServlet.h"

namespace Web{
    TestServlet::TestServlet()
        :Magic::NetWork::Http::IHttpServlet("",Magic::NetWork::Http::HttpServletType::Deafult){
    }
    bool TestServlet::handle(const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response){
        response->setStatus(Magic::NetWork::Http::HttpStatus::NOT_FOUND);
        std::string notfound{R"Template(<html>
            <head><title>404 Not Found</title></head>
            <body>
            <center><h1>404 Not Found</h1></center>
            <hr><center>Magic/0.0.2</center>
            </body>
            </html>)Template"};
        response->setBody(notfound);
        return true;
    }
}