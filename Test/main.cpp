/*
 * @Author: INotFound
 * @Date: 2020-12-17 00:35:45
 * @LastEditTime: 2021-01-18 06:00:25
 */
#include "Magic/NetWork/Http/Uri.h"
#include "Magic/NetWork/Http/Http.h"
#include "Magic/NetWork/Http/HttpParser.h"

int main(int argc,char** argv){
    Magic::NetWork::Http::Uri uri;
    uri.execute("mysql://admin@0.0.0.0/xxx?password=12345678901a");
    std::printf("%s\n",uri.getUser().c_str());
    std::printf("%s\n",uri.getHost().c_str());
    std::printf("%s\n",&(uri.getPath().c_str()[1]));
    std::printf("%s\n",uri.getQuery().c_str());
    std::printf("%s\n",uri.getScheme().c_str());
    std::printf("%s\n",uri.getFragment().c_str());

    Magic::NetWork::Http::HttpRequestParser parser;
    std::string httpRequestContext("GET /bg-upper.png HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: 127.0.0.1:8180\r\nAccept: */*\r\nRange: bytes=0-10\r\n\r\n");
    char context[] = "GET /bg-upper.png HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: 127.0.0.1:8180\r\nAccept: */*\r\nRange: bytes=0-10\r\n\r\n";
    parser.execute(context,httpRequestContext.size());
    auto request = parser.getData();
    std::cout << request->isRange() << std::endl;
    std::cout << request->getRangeStop() << std::endl;
    std::cout << request->getRangeStart() << std::endl;
    
    Magic::NetWork::Http::HttpResponseParser responseParser;
    char responseContext[] = "HTTP/1.1 206 Partial Content\r\nAccept-Ranges: bytes\r\nContent-Range: bytes 20-2000/4932\r\nContent-Length: 2001\r\n\r\n";
    std::string httpResponseContext("HTTP/1.1 206 Partial Content\r\nAccept-Ranges: bytes\r\nContent-Range: bytes 20-2000/4932\r\nContent-Length: 2001\r\n\r\n");
    responseParser.execute(responseContext,httpResponseContext.length());
    auto response = responseParser.getData();
    
    std::cout << "-------------" << std::endl;
    std::cout << response->isRange() << std::endl;
    std::cout << "Stop: " << response->getRangeStop() << std::endl;
    std::cout << "Start: " << response->getRangeStart() << std::endl;
    return 0;
}
