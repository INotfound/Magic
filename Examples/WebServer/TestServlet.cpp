/*
 * @Author: INotFound
 * @Date: 2020-12-17 17:57:35
 * @LastEditTime: 2020-12-21 14:02:57
 */
#include "TestServlet.h"

namespace Web{
    TestServlet::TestServlet(){
    }

    void TestServlet::test(const Safe<HttpSocket>& httpSocket,const Safe<HttpRequest>& request,const Safe<HttpResponse>& response){
        std::string helloMagic{R"Template(<html>
            <head><title>Hello Magic</title></head>
            <body>
            <center><h1>Hello Magic</h1></center>
            <hr><center>Magic/2.0.0</center>
            </body>
            </html>)Template"};
        response->setBody(helloMagic)
            ->setStatus(HttpStatus::OK);
        httpSocket->sendResponse(response);
    }
}