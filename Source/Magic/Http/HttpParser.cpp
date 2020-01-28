#include "Http/HttpParser.h"
#include <cstring>
namespace Magic{
namespace Http{

    void OnRequestUri(void *data,const char*at,size_t length){
    }
    void OnRequestPath(void *data,const char*at,size_t length){
        HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
        parser->getData()->setUrlPath(std::string(at,length));
    }
    void OnRequestQuery(void *data,const char*at,size_t length){
        HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
        parser->getData()->setQuery(std::string(at,length));
    }
    void OnRequestMethod(void *data,const char*at,size_t length){
        HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
        HttpMethod method = CharsToHttpMethod(at);
        if(method == HttpMethod::INVALID_METHOD){
            //TODO:
            parser->setError(true);
            return;
        }
        parser->getData()->setMethod(method);
    }
    void OnRequestVersion(void *data,const char*at,size_t length){
        HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
        uint8_t ver = 0;
        if(std::strncmp(at,"HTTP/1.1",length) == 0){
            ver = 0x11;
        }else if(std::strncmp(at,"HTTP/1.0",length) == 0){
            ver = 0x10;
        }else{
            //TODO:
            parser->setError(true);
            return;
        }
        parser->getData()->setVersion(ver);
    }
    void OnRequestFragment(void *data,const char*at,size_t length){
        HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
        parser->getData()->setFragment(std::string(at,length));
    }
    void OnRequestHeaderDone(void *data,const char*at,size_t length){
        //HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
    }
    void OnRequestHttpField(void *data,const char*field,size_t flen,const char* value,size_t vlen){
        HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
        if(flen == 0){
            //TODO
            parser->setError(true);
            return;
        }
        std::string key(field, flen);
        std::string val(value, vlen);
        parser->getData()->setHeader(key, val);
    }

    HttpRequestParser::HttpRequestParser()
        :m_Error{false}{
        m_Data.reset(new HttpRequest(true));
        http_parser_init(&m_Parser);
        m_Parser.request_uri    = OnRequestUri;
        m_Parser.request_path   = OnRequestPath;
        m_Parser.query_string   = OnRequestQuery;
        m_Parser.request_method = OnRequestMethod;
        m_Parser.http_version   = OnRequestVersion;
        m_Parser.fragment       = OnRequestFragment;
        m_Parser.http_field     = OnRequestHttpField;
        m_Parser.header_done    = OnRequestHeaderDone;
        m_Parser.data = this;

    }
    bool HttpRequestParser::hasError(){
        return m_Error || http_parser_has_error(&m_Parser);
    }
    bool HttpRequestParser::isFinished(){
        return http_parser_finish(&m_Parser);
    }
    void HttpRequestParser::setError(bool val){
        m_Error = val;
    }

    MagicPtr<HttpRequest>& HttpRequestParser::getData(){
        return m_Data;
    }
    uint32_t HttpRequestParser::execute(char* data,uint32_t length){
        size_t offset = http_parser_execute(&m_Parser,data,length,0);
        std::memmove(data,data + offset,length - offset);
        return offset;
    }


    void OnResponseReason(void *data,const char*at,size_t length){
        HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
        parser->getData()->setReason(std::string(at,length));
    }
    void OnResponseStatus(void *data,const char*at,size_t length){
        HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
        HttpStatus status = static_cast<HttpStatus>(atol(at));
        parser->getData()->setStatus(status);
    }
    void OnResponseVersion(void *data,const char*at,size_t length){
        HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
        uint8_t ver = 0;
        if(std::strncmp(at,"HTTP/1.1",length) == 0){
            ver = 0x11;
        }else if(std::strncmp(at,"HTTP/1.0",length) == 0){
            ver = 0x10;
        }else{
            //TODO:
            parser->setError(true);
            return;
        }
        parser->getData()->setVersion(ver);
    }
    void OnResponseLastChunk(void *data,const char*at,size_t length){
        //HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
    }
    void OnResponseChunkSize(void *data,const char*at,size_t length){
        //HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
    }
    void OnResponseHeaderOne(void *data,const char*at,size_t length){
    }
    void OnResponseHttpField(void *data,const char*field,size_t flen,const char* value,size_t vlen){
        HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
        if(flen == 0){
            //TODO
            parser->setError(true);
            return;
        }
        parser->getData()->setHeader(std::string(field,flen),std::string(value,vlen));
    }
    HttpResponseParser::HttpResponseParser()
        :m_Error{false}{
        m_Data.reset(new HttpResponse(true));
        httpclient_parser_init(&m_Parser);
        m_Parser.reason_phrase  = OnResponseReason;
        m_Parser.status_code    = OnResponseStatus;
        m_Parser.http_version   = OnResponseVersion;
        m_Parser.last_chunk     = OnResponseLastChunk;
        m_Parser.chunk_size     = OnResponseChunkSize;
        m_Parser.header_done    = OnResponseHeaderOne;
        m_Parser.http_field     = OnResponseHttpField;
        m_Parser.data = this;
    }
    bool HttpResponseParser::hasError(){
        return m_Error || httpclient_parser_has_error(&m_Parser);;
    }
    bool HttpResponseParser::isFinished(){
        return httpclient_parser_finish(&m_Parser);
    }
    void HttpResponseParser::setError(bool val){
        m_Error = val;
    }
    MagicPtr<HttpResponse>& HttpResponseParser::getData(){
        return m_Data;
    }
    uint32_t HttpResponseParser::execute(char* data,uint32_t length){
        size_t offset = httpclient_parser_execute(&m_Parser,data,length,0);
        std::memmove(data,data + offset,length - offset);
        return offset;
    }


}
}