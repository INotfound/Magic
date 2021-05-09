#include <sstream>
#include "Magic/NetWork/Http/Uri.h"

namespace Magic{
namespace NetWork{
namespace Http{
    %%{
        # See RFC 3986: http://www.ietf.org/rfc/rfc3986.txt

        machine uri_parser;

        gen_delims = ":" | "/" | "?" | "#" | "[" | "]" | "@";
        sub_delims = "!" | "$" | "&" | "'" | "(" | ")" | "*" | "+" | "," | ";" | "=";
        reserved = gen_delims | sub_delims;
        unreserved = alpha | digit | "-" | "." | "_" | "~";
        pct_encoded = "%" xdigit xdigit;

        action marku { mark = fpc; }
        action markh { mark = fpc; }

        action save_scheme
        {
            this->setScheme(std::string(mark, fpc - mark));
            mark = NULL;
        }

        scheme = (alpha (alpha | digit | "+" | "-" | ".")*) >marku %save_scheme;

        action save_port
        {
            if (fpc != mark){
                this->setPort(atoi(mark));
            }
            mark = NULL;
        }
        action save_userinfo
        {
            if(mark){
                this->setUser(std::string(mark, fpc - mark));
            }
            mark = NULL;
        }
        action save_host
        {
            if (mark != NULL){
                this->setHost(std::string(mark, fpc - mark));
            }
        }

        userinfo = (unreserved | pct_encoded | sub_delims | ":")*;
        dec_octet = digit | [1-9] digit | "1" digit{2} | 2 [0-4] digit | "25" [0-5];
        IPv4address = dec_octet "." dec_octet "." dec_octet "." dec_octet;
        h16 = xdigit{1,4};
        ls32 = (h16 ":" h16) | IPv4address;
        IPv6address = (                         (h16 ":"){6} ls32) |
                    (                    "::" (h16 ":"){5} ls32) |
                    ((             h16)? "::" (h16 ":"){4} ls32) |
                    (((h16 ":"){1} h16)? "::" (h16 ":"){3} ls32) |
                    (((h16 ":"){2} h16)? "::" (h16 ":"){2} ls32) |
                    (((h16 ":"){3} h16)? "::" (h16 ":"){1} ls32) |
                    (((h16 ":"){4} h16)? "::"              ls32) |
                    (((h16 ":"){5} h16)? "::"              h16 ) |
                    (((h16 ":"){6} h16)? "::"                  );
        IPvFuture = "v" xdigit+ "." (unreserved | sub_delims | ":")+;
        IP_literal = "[" (IPv6address | IPvFuture) "]";
        reg_name = (unreserved | pct_encoded | sub_delims)*;
        host = IP_literal | IPv4address | reg_name;
        port = digit*;

        authority = ( (userinfo %save_userinfo "@")? host >markh %save_host (":" port >markh %save_port)? ) >markh;

        action save_segment
        {
            mark = NULL;
        }

        action save_path
        {
            this->setPath(std::string(mark, fpc - mark));
            mark = NULL;
        }


    #    pchar = unreserved | pct_encoded | sub_delims | ":" | "@";
    # add (any -- ascii) support chinese
        pchar         = ( (any -- ascii ) | unreserved | pct_encoded | sub_delims | ":" | "@" ) ;
        segment = pchar*;
        segment_nz = pchar+;
        segment_nz_nc = (pchar - ":")+;

        action clear_segments
        {
        }

        path_abempty = (("/" segment))? ("/" segment)*;
        path_absolute = ("/" (segment_nz ("/" segment)*)?);
        path_noscheme = segment_nz_nc ("/" segment)*;
        path_rootless = segment_nz ("/" segment)*;
        path_empty = "";
        path = (path_abempty | path_absolute | path_noscheme | path_rootless | path_empty);

        action save_query
        {
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setQuery(std::string(mark, fpc - mark));
            mark = NULL;
        }
        action save_fragment
        {
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setFragment(std::string(mark, fpc - mark));
            mark = NULL;
        }

        query = (pchar | "/" | "?")* >marku %save_query;
        fragment = (pchar | "/" | "?")* >marku %save_fragment;

        hier_part = ("//" authority path_abempty > markh %save_path) | path_absolute | path_rootless | path_empty;

        relative_part = ("//" authority path_abempty) | path_absolute | path_noscheme | path_empty;
        relative_ref = relative_part ( "?" query )? ( "#" fragment )?;

        absolute_URI = scheme ":" hier_part ( "?" query )? ;
        # Obsolete, but referenced from HTTP, so we translate
        relative_URI = relative_part ( "?" query )?;

        URI = scheme ":" hier_part ( "?" query )? ( "#" fragment )?;
        URI_reference = URI | relative_ref;
        main := URI_reference;
        write data;
    }%%

    Uri::Uri()
        :m_Port(0){
    }

    bool Uri::execute(const std::string& uri){
        int cs = 0;
        const char* mark = 0;
        %% write init;
        const char *p = uri.c_str();
        const char *pe = p + uri.size();
        const char* eof = pe;
        %% write exec;
        if(cs == uri_parser_error){
            return false; //error
        } else if(cs >= uri_parser_first_final){
            return true;//ok
        }
        return false;//error
    }
    uint32_t Uri::getPort() const{
        if(m_Port){
            return m_Port;
        }
        if(m_Scheme == "http"
            || m_Scheme == "ws"){
            return 80;
        } else if(m_Scheme == "https"
                || m_Scheme == "wss"){
            return 443;
        }
        return m_Port;
    }

    const std::string& Uri::getUser() const{ 
        return m_User;
    }
    const std::string& Uri::getHost() const{
        return m_Host;
    }

    const std::string& Uri::getPath() const{
        static std::string s_default_path = "/";
        return m_Path.empty() ? s_default_path : m_Path;
    }
    const std::string& Uri::getQuery() const{ 
        return m_Query;
    }
    const std::string& Uri::getScheme() const{ 
        return m_Scheme;
    }
    const std::string& Uri::getFragment() const{
        return m_Fragment;
    }

    void Uri::setPort(uint32_t val){ 
        m_Port = val;
    }
    void Uri::setHost(const std::string& val){
        m_Host = val;
    }
    void Uri::setPath(const std::string& val){
        m_Path = val;
    }
    void Uri::setUser(const std::string& val){
        m_User = val;
    }
    void Uri::setQuery(const std::string& val){
        m_Query = val;
    }
    void Uri::setScheme(const std::string& val){
        m_Scheme = val;
    }
    void Uri::setFragment(const std::string& val){
        m_Fragment = val;
    }

    bool Uri::isDefaultPort() const{
        if(m_Port == 0){
            return true;
        }
        if(m_Scheme == "http"
                || m_Scheme == "ws"){
            return m_Port == 80;
        } else if(m_Scheme == "https"
                || m_Scheme == "wss"){
            return m_Port == 443;
        }
        return false;
    }
}
}
}
