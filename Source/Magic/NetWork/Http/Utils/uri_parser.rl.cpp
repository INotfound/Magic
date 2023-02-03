
/* #line 1 "uri_parser.rl" */
#include <sstream>
#include "Magic/NetWork/Http/Uri.h"

namespace Magic{
namespace NetWork{
namespace Http{
    
/* #line 11 "uri_parser.rl.c" */
static const int uri_parser_start = 451;
static const int uri_parser_first_final = 451;
static const int uri_parser_error = 0;

//static const int uri_parser_en_main = 451;


/* #line 131 "uri_parser.rl" */

    Uri::Uri(const std::string& uri)
        :m_Error(false)
        ,m_Port(0){
        m_Error = !this->execute(uri);
    }

    bool Uri::hasError() const{
        return m_Error;
    }

    uint32_t Uri::getPort() const{
        if(m_Port != 0){
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

    bool Uri::execute(const std::string& uri){
        int cs = 0;
        const char* mark = 0;
        
/* #line 92 "uri_parser.rl.c" */
	{
	cs = uri_parser_start;
	}

/* #line 203 "uri_parser.rl" */
        const char *p = uri.c_str();
        const char *pe = p + uri.size();
        const char* eof = pe;
        
/* #line 102 "uri_parser.rl.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 451:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st3;
		case 47: goto st458;
		case 58: goto st0;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( (*p) > 32 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr461;
		} else if ( (*p) >= 0 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( (*p) > 122 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else if ( (*p) >= 97 )
			goto tr461;
	} else
		goto st0;
	goto st452;
st452:
	if ( ++p == pe )
		goto _test_eof452;
case 452:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st3;
		case 47: goto st455;
		case 58: goto st0;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st452;
st0:
cs = 0;
	goto _out;
tr467:
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
/* #line 103 "uri_parser.rl" */
	{
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setQuery(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
tr469:
/* #line 103 "uri_parser.rl" */
	{
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setQuery(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
tr474:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st453;
tr485:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st453;
tr489:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st453;
tr493:
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st453;
tr548:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
tr559:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
tr563:
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
tr566:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
tr570:
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st453;
st453:
	if ( ++p == pe )
		goto _test_eof453;
case 453:
/* #line 293 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 37: goto tr463;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 34 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 35 ) {
		if ( (*p) > 94 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else if ( (*p) >= 91 )
			goto st0;
	} else
		goto st0;
	goto tr462;
tr462:
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
	goto st454;
st454:
	if ( ++p == pe )
		goto _test_eof454;
case 454:
/* #line 321 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 37: goto st1;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 34 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 35 ) {
		if ( (*p) > 94 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else if ( (*p) >= 91 )
			goto st0;
	} else
		goto st0;
	goto st454;
tr463:
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
/* #line 349 "uri_parser.rl.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st2;
	} else
		goto st2;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st454;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st454;
	} else
		goto st454;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st4;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st4;
	} else
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st452;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st452;
	} else
		goto st452;
	goto st0;
tr476:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st455;
tr486:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st455;
tr490:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st455;
tr494:
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st455;
st455:
	if ( ++p == pe )
		goto _test_eof455;
case 455:
/* #line 440 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st5;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st455;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st455;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st455;
	} else
		goto st455;
	goto st0;
tr482:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st456;
tr488:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st456;
tr492:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st456;
tr496:
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st456;
tr556:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st456;
tr562:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st456;
tr565:
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st456;
tr569:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st456;
tr573:
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st456;
st456:
	if ( ++p == pe )
		goto _test_eof456;
case 456:
/* #line 597 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto tr467;
		case 37: goto tr468;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto tr466;
tr466:
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
	goto st457;
st457:
	if ( ++p == pe )
		goto _test_eof457;
case 457:
/* #line 624 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto tr469;
		case 37: goto st7;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st457;
tr468:
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
/* #line 651 "uri_parser.rl.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st8;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st8;
	} else
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st457;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st457;
	} else
		goto st457;
	goto st0;
st458:
	if ( ++p == pe )
		goto _test_eof458;
case 458:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st5;
		case 47: goto st459;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st455;
st459:
	if ( ++p == pe )
		goto _test_eof459;
case 459:
	switch( (*p) ) {
		case 2: goto tr472;
		case 33: goto tr473;
		case 35: goto tr474;
		case 37: goto tr475;
		case 47: goto tr476;
		case 48: goto tr477;
		case 49: goto tr478;
		case 50: goto tr479;
		case 58: goto tr481;
		case 59: goto tr473;
		case 61: goto tr473;
		case 63: goto tr482;
		case 64: goto tr483;
		case 91: goto tr484;
		case 95: goto tr473;
		case 126: goto tr473;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr473;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr473;
		} else if ( (*p) >= 65 )
			goto tr473;
	} else
		goto tr480;
	goto st0;
tr472:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
/* #line 740 "uri_parser.rl.c" */
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 46 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 2: goto st13;
		case 48: goto st15;
		case 49: goto st27;
		case 50: goto st29;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st28;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 46 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 2: goto st17;
		case 48: goto st19;
		case 49: goto st23;
		case 50: goto st25;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st24;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 46 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 2: goto st21;
		case 48: goto st460;
		case 49: goto st463;
		case 50: goto st465;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st464;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st460;
	goto st0;
st460:
	if ( ++p == pe )
		goto _test_eof460;
case 460:
	switch( (*p) ) {
		case 35: goto tr485;
		case 47: goto tr486;
		case 58: goto tr487;
		case 63: goto tr488;
	}
	goto st0;
tr507:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st461;
tr487:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st461;
st461:
	if ( ++p == pe )
		goto _test_eof461;
case 461:
/* #line 886 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 35: goto tr489;
		case 47: goto tr490;
		case 63: goto tr492;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr491;
	goto st0;
tr491:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st462;
st462:
	if ( ++p == pe )
		goto _test_eof462;
case 462:
/* #line 903 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 35: goto tr493;
		case 47: goto tr494;
		case 63: goto tr496;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st462;
	goto st0;
st463:
	if ( ++p == pe )
		goto _test_eof463;
case 463:
	switch( (*p) ) {
		case 35: goto tr485;
		case 47: goto tr486;
		case 58: goto tr487;
		case 63: goto tr488;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st464;
	goto st0;
st464:
	if ( ++p == pe )
		goto _test_eof464;
case 464:
	switch( (*p) ) {
		case 35: goto tr485;
		case 47: goto tr486;
		case 58: goto tr487;
		case 63: goto tr488;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st460;
	goto st0;
st465:
	if ( ++p == pe )
		goto _test_eof465;
case 465:
	switch( (*p) ) {
		case 35: goto tr485;
		case 47: goto tr486;
		case 53: goto st466;
		case 58: goto tr487;
		case 63: goto tr488;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st460;
	goto st0;
st466:
	if ( ++p == pe )
		goto _test_eof466;
case 466:
	switch( (*p) ) {
		case 35: goto tr485;
		case 47: goto tr486;
		case 58: goto tr487;
		case 63: goto tr488;
	}
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st460;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 46 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 46 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 46: goto st20;
		case 53: goto st26;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 46 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st19;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 46 )
		goto st16;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 46 )
		goto st16;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 46: goto st16;
		case 53: goto st30;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 46 )
		goto st16;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st15;
	goto st0;
tr473:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st467;
st467:
	if ( ++p == pe )
		goto _test_eof467;
case 467:
/* #line 1049 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st467;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st467;
	} else
		goto st467;
	goto st0;
tr475:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
/* #line 1079 "uri_parser.rl.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st32;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st32;
	} else
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st467;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st467;
	} else
		goto st467;
	goto st0;
tr481:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st468;
tr499:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st468;
st468:
	if ( ++p == pe )
		goto _test_eof468;
case 468:
/* #line 1124 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st33;
		case 35: goto tr489;
		case 37: goto st34;
		case 47: goto tr490;
		case 61: goto st33;
		case 63: goto tr492;
		case 64: goto tr38;
		case 95: goto st33;
		case 126: goto st33;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr500;
		} else if ( (*p) >= 36 )
			goto st33;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st33;
		} else if ( (*p) >= 65 )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 33: goto st33;
		case 37: goto st34;
		case 61: goto st33;
		case 64: goto tr38;
		case 95: goto st33;
		case 126: goto st33;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st33;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st33;
		} else if ( (*p) >= 65 )
			goto st33;
	} else
		goto st33;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st35;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st35;
	} else
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st33;
	} else
		goto st33;
	goto st0;
tr38:
/* #line 37 "uri_parser.rl" */
	{
            if(mark){
                this->setUser(std::string(mark, p - mark));
            }
            mark = nullptr;
        }
	goto st469;
tr483:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 37 "uri_parser.rl" */
	{
            if(mark){
                this->setUser(std::string(mark, p - mark));
            }
            mark = nullptr;
        }
	goto st469;
st469:
	if ( ++p == pe )
		goto _test_eof469;
case 469:
/* #line 1225 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 2: goto tr472;
		case 33: goto tr501;
		case 35: goto tr474;
		case 37: goto tr502;
		case 47: goto tr476;
		case 48: goto tr503;
		case 49: goto tr504;
		case 50: goto tr505;
		case 58: goto tr507;
		case 59: goto tr501;
		case 61: goto tr501;
		case 63: goto tr482;
		case 91: goto tr484;
		case 95: goto tr501;
		case 126: goto tr501;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr501;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr501;
		} else if ( (*p) >= 65 )
			goto tr501;
	} else
		goto tr506;
	goto st0;
tr501:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st470;
st470:
	if ( ++p == pe )
		goto _test_eof470;
case 470:
/* #line 1263 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st470;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st470;
	} else
		goto st470;
	goto st0;
tr502:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
/* #line 1292 "uri_parser.rl.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st37;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st37;
	} else
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st470;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st470;
	} else
		goto st470;
	goto st0;
tr503:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st471;
st471:
	if ( ++p == pe )
		goto _test_eof471;
case 471:
/* #line 1323 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st472;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st470;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st470;
	} else
		goto st470;
	goto st0;
st472:
	if ( ++p == pe )
		goto _test_eof472;
case 472:
	switch( (*p) ) {
		case 2: goto st13;
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 47: goto tr486;
		case 48: goto st473;
		case 49: goto st481;
		case 50: goto st483;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st482;
	goto st0;
st473:
	if ( ++p == pe )
		goto _test_eof473;
case 473:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st474;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st470;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st470;
	} else
		goto st470;
	goto st0;
st474:
	if ( ++p == pe )
		goto _test_eof474;
case 474:
	switch( (*p) ) {
		case 2: goto st17;
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 47: goto tr486;
		case 48: goto st475;
		case 49: goto st477;
		case 50: goto st479;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st478;
	goto st0;
st475:
	if ( ++p == pe )
		goto _test_eof475;
case 475:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st476;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st470;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st470;
	} else
		goto st470;
	goto st0;
st476:
	if ( ++p == pe )
		goto _test_eof476;
case 476:
	switch( (*p) ) {
		case 2: goto st21;
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st470;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st470;
	} else
		goto st470;
	goto st0;
st477:
	if ( ++p == pe )
		goto _test_eof477;
case 477:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st476;
		case 47: goto tr486;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st478;
	goto st0;
st478:
	if ( ++p == pe )
		goto _test_eof478;
case 478:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st476;
		case 47: goto tr486;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st475;
	goto st0;
st479:
	if ( ++p == pe )
		goto _test_eof479;
case 479:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st476;
		case 47: goto tr486;
		case 53: goto st480;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st475;
	goto st0;
st480:
	if ( ++p == pe )
		goto _test_eof480;
case 480:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st476;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st475;
		} else if ( (*p) >= 36 )
			goto st470;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st470;
	goto st0;
st481:
	if ( ++p == pe )
		goto _test_eof481;
case 481:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st474;
		case 47: goto tr486;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st482;
	goto st0;
st482:
	if ( ++p == pe )
		goto _test_eof482;
case 482:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st474;
		case 47: goto tr486;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st473;
	goto st0;
st483:
	if ( ++p == pe )
		goto _test_eof483;
case 483:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st474;
		case 47: goto tr486;
		case 53: goto st484;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st473;
	goto st0;
st484:
	if ( ++p == pe )
		goto _test_eof484;
case 484:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st474;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st473;
		} else if ( (*p) >= 36 )
			goto st470;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st470;
	goto st0;
tr504:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st485;
st485:
	if ( ++p == pe )
		goto _test_eof485;
case 485:
/* #line 1730 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st472;
		case 47: goto tr486;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st486;
	goto st0;
tr506:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st486;
st486:
	if ( ++p == pe )
		goto _test_eof486;
case 486:
/* #line 1764 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st472;
		case 47: goto tr486;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st471;
	goto st0;
tr505:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st487;
st487:
	if ( ++p == pe )
		goto _test_eof487;
case 487:
/* #line 1798 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st472;
		case 47: goto tr486;
		case 53: goto st488;
		case 58: goto tr487;
		case 59: goto st470;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st470;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st471;
	goto st0;
st488:
	if ( ++p == pe )
		goto _test_eof488;
case 488:
	switch( (*p) ) {
		case 33: goto st470;
		case 35: goto tr485;
		case 37: goto st36;
		case 46: goto st472;
		case 47: goto tr486;
		case 58: goto tr487;
		case 61: goto st470;
		case 63: goto tr488;
		case 95: goto st470;
		case 126: goto st470;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st471;
		} else if ( (*p) >= 36 )
			goto st470;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st470;
		} else if ( (*p) >= 65 )
			goto st470;
	} else
		goto st470;
	goto st0;
tr484:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
/* #line 1864 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 58: goto st146;
		case 118: goto st225;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st39;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st39;
	} else
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 58 )
		goto st43;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st40;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st40;
	} else
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 58 )
		goto st43;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st41;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 58 )
		goto st43;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st42;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st42;
	} else
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 58 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 58 )
		goto st141;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st44;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st44;
	} else
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 58 )
		goto st48;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st45;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st45;
	} else
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 58 )
		goto st48;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st46;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st46;
	} else
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 58 )
		goto st48;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 58 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 58 )
		goto st136;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st49;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st49;
	} else
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 58 )
		goto st53;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st50;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st50;
	} else
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 58 )
		goto st53;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st51;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st51;
	} else
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 58 )
		goto st53;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st52;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st52;
	} else
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 58 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 58 )
		goto st131;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st54;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st54;
	} else
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 58 )
		goto st58;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st55;
	} else
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 58 )
		goto st58;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st56;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st56;
	} else
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 58 )
		goto st58;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st57;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st57;
	} else
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 58 )
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 58 )
		goto st126;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st59;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st59;
	} else
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 58 )
		goto st63;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st60;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st60;
	} else
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 58 )
		goto st63;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st61;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st61;
	} else
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 58 )
		goto st63;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st62;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st62;
	} else
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 58 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 58 )
		goto st113;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st64;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st64;
	} else
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 58 )
		goto st68;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st65;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st65;
	} else
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 58 )
		goto st68;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st66;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st66;
	} else
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 58 )
		goto st68;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st67;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st67;
	} else
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 58 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st96;
		case 49: goto st104;
		case 50: goto st107;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st110;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st112;
	} else
		goto st112;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 46 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 2: goto st73;
		case 48: goto st75;
		case 49: goto st92;
		case 50: goto st94;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st93;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 46 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 2: goto st77;
		case 48: goto st79;
		case 49: goto st88;
		case 50: goto st90;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st89;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st78;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st79;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 46 )
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 2: goto st81;
		case 48: goto st83;
		case 49: goto st84;
		case 50: goto st86;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st85;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 93 )
		goto st460;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 93 )
		goto st460;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 93 )
		goto st460;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st83;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 53: goto st87;
		case 93: goto st460;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st83;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 93 )
		goto st460;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st83;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 46 )
		goto st80;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 46 )
		goto st80;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st79;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 46: goto st80;
		case 53: goto st91;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st79;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 46 )
		goto st80;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st79;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 46 )
		goto st76;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st93;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 46 )
		goto st76;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st75;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 46: goto st76;
		case 53: goto st95;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st75;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 46 )
		goto st76;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st75;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st97;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st97;
	} else
		goto st97;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 58 )
		goto st100;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st98;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 58 )
		goto st100;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st99;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 58 )
		goto st100;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 58 )
		goto st83;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st101;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st101;
	} else
		goto st101;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 93 )
		goto st460;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st102;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st102;
	} else
		goto st102;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 93 )
		goto st460;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st103;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st103;
	} else
		goto st103;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 93 )
		goto st460;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st83;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st83;
	} else
		goto st83;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st105;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st97;
	} else
		goto st97;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st106;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st99;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st109;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st108;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st97;
	} else
		goto st97;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st98;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st106;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st98;
		} else if ( (*p) >= 65 )
			goto st98;
	} else
		goto st98;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st100;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st108;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st97;
	} else
		goto st97;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st101;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st101;
	} else
		goto st101;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 58 )
		goto st100;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st97;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st97;
	} else
		goto st97;
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st114;
		case 49: goto st118;
		case 50: goto st121;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st124;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st125;
	} else
		goto st125;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st115;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st115;
	} else
		goto st115;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 58 )
		goto st111;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st116;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 58 )
		goto st111;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st117;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st117;
	} else
		goto st117;
	goto st0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 58 )
		goto st111;
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st119;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st115;
	} else
		goto st115;
	goto st0;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st120;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st117;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st117;
	} else
		goto st117;
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st123;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st122;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st115;
	} else
		goto st115;
	goto st0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st116;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st120;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st116;
		} else if ( (*p) >= 65 )
			goto st116;
	} else
		goto st116;
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st122;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st115;
	} else
		goto st115;
	goto st0;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	if ( (*p) == 58 )
		goto st111;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st115;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st115;
	} else
		goto st115;
	goto st0;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st127;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st127;
	} else
		goto st127;
	goto st0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	if ( (*p) == 58 )
		goto st113;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st128;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st128;
	} else
		goto st128;
	goto st0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	if ( (*p) == 58 )
		goto st113;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st129;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st129;
	} else
		goto st129;
	goto st0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	if ( (*p) == 58 )
		goto st113;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st130;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st130;
	} else
		goto st130;
	goto st0;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	if ( (*p) == 58 )
		goto st113;
	goto st0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st132;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st132;
	} else
		goto st132;
	goto st0;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) == 58 )
		goto st126;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st133;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st133;
	} else
		goto st133;
	goto st0;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	if ( (*p) == 58 )
		goto st126;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st134;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st134;
	} else
		goto st134;
	goto st0;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	if ( (*p) == 58 )
		goto st126;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st135;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st135;
	} else
		goto st135;
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	if ( (*p) == 58 )
		goto st126;
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st137;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st137;
	} else
		goto st137;
	goto st0;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	if ( (*p) == 58 )
		goto st131;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st138;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st138;
	} else
		goto st138;
	goto st0;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 58 )
		goto st131;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st139;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st139;
	} else
		goto st139;
	goto st0;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 58 )
		goto st131;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st140;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st140;
	} else
		goto st140;
	goto st0;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	if ( (*p) == 58 )
		goto st131;
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st142;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st142;
	} else
		goto st142;
	goto st0;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) == 58 )
		goto st136;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st143;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st143;
	} else
		goto st143;
	goto st0;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	if ( (*p) == 58 )
		goto st136;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st144;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st144;
	} else
		goto st144;
	goto st0;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	if ( (*p) == 58 )
		goto st136;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st145;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st145;
	} else
		goto st145;
	goto st0;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	if ( (*p) == 58 )
		goto st136;
	goto st0;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	if ( (*p) == 58 )
		goto st147;
	goto st0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st148;
		case 49: goto st217;
		case 50: goto st220;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st223;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st224;
	} else
		goto st224;
	goto st0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st149;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st149;
	} else
		goto st149;
	goto st0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st150;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st151;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st151;
	} else
		goto st151;
	goto st0;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	switch( (*p) ) {
		case 58: goto st152;
		case 93: goto st460;
	}
	goto st0;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st153;
		case 49: goto st209;
		case 50: goto st212;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st215;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st216;
	} else
		goto st216;
	goto st0;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st154;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st154;
	} else
		goto st154;
	goto st0;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	switch( (*p) ) {
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st155;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	switch( (*p) ) {
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st156;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st156;
	} else
		goto st156;
	goto st0;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 58: goto st157;
		case 93: goto st460;
	}
	goto st0;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st158;
		case 49: goto st201;
		case 50: goto st204;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st207;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st208;
	} else
		goto st208;
	goto st0;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st159;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st159;
	} else
		goto st159;
	goto st0;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	switch( (*p) ) {
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st160;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st161;
	} else
		goto st161;
	goto st0;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 58: goto st162;
		case 93: goto st460;
	}
	goto st0;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st163;
		case 49: goto st193;
		case 50: goto st196;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st199;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st200;
	} else
		goto st200;
	goto st0;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st164;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st164;
	} else
		goto st164;
	goto st0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st165;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	switch( (*p) ) {
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st166;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st166;
	} else
		goto st166;
	goto st0;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 58: goto st167;
		case 93: goto st460;
	}
	goto st0;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st168;
		case 49: goto st185;
		case 50: goto st188;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st191;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st192;
	} else
		goto st192;
	goto st0;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st169;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st169;
	} else
		goto st169;
	goto st0;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st170;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	switch( (*p) ) {
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st171;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st171;
	} else
		goto st171;
	goto st0;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
	switch( (*p) ) {
		case 58: goto st172;
		case 93: goto st460;
	}
	goto st0;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	switch( (*p) ) {
		case 2: goto st69;
		case 48: goto st173;
		case 49: goto st177;
		case 50: goto st180;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st183;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st184;
	} else
		goto st184;
	goto st0;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st174;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	switch( (*p) ) {
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st176;
	} else
		goto st176;
	goto st0;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	switch( (*p) ) {
		case 58: goto st111;
		case 93: goto st460;
	}
	goto st0;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st178;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st179;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st176;
	} else
		goto st176;
	goto st0;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st182;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st181;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st179;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st175;
		} else if ( (*p) >= 65 )
			goto st175;
	} else
		goto st175;
	goto st0;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st181;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	switch( (*p) ) {
		case 58: goto st111;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st174;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st174;
	} else
		goto st174;
	goto st0;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st186;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st169;
	} else
		goto st169;
	goto st0;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st187;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st171;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st171;
	} else
		goto st171;
	goto st0;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st190;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st189;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st169;
	} else
		goto st169;
	goto st0;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st170;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st187;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st170;
		} else if ( (*p) >= 65 )
			goto st170;
	} else
		goto st170;
	goto st0;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st189;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st169;
	} else
		goto st169;
	goto st0;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	switch( (*p) ) {
		case 58: goto st172;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st169;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st169;
	} else
		goto st169;
	goto st0;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st194;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st164;
	} else
		goto st164;
	goto st0;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st195;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st166;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st166;
	} else
		goto st166;
	goto st0;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st198;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st197;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st164;
	} else
		goto st164;
	goto st0;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st165;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st195;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st165;
		} else if ( (*p) >= 65 )
			goto st165;
	} else
		goto st165;
	goto st0;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st197;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st164;
	} else
		goto st164;
	goto st0;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	switch( (*p) ) {
		case 58: goto st167;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st164;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st164;
	} else
		goto st164;
	goto st0;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st202;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st159;
	} else
		goto st159;
	goto st0;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st203;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st161;
	} else
		goto st161;
	goto st0;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st206;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st205;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st159;
	} else
		goto st159;
	goto st0;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st160;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st203;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st160;
		} else if ( (*p) >= 65 )
			goto st160;
	} else
		goto st160;
	goto st0;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st205;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st159;
	} else
		goto st159;
	goto st0;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	switch( (*p) ) {
		case 58: goto st162;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st159;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st159;
	} else
		goto st159;
	goto st0;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st210;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st154;
	} else
		goto st154;
	goto st0;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st211;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st156;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st156;
	} else
		goto st156;
	goto st0;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st214;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st213;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st154;
	} else
		goto st154;
	goto st0;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st155;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st211;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st155;
		} else if ( (*p) >= 65 )
			goto st155;
	} else
		goto st155;
	goto st0;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st213;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st154;
	} else
		goto st154;
	goto st0;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
	switch( (*p) ) {
		case 58: goto st157;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st154;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st154;
	} else
		goto st154;
	goto st0;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st218;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st149;
	} else
		goto st149;
	goto st0;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st219;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st151;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st151;
	} else
		goto st151;
	goto st0;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
	switch( (*p) ) {
		case 46: goto st72;
		case 53: goto st222;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st221;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st149;
	} else
		goto st149;
	goto st0;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st150;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st219;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st150;
		} else if ( (*p) >= 65 )
			goto st150;
	} else
		goto st150;
	goto st0;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
	switch( (*p) ) {
		case 46: goto st72;
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st221;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st149;
	} else
		goto st149;
	goto st0;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
	switch( (*p) ) {
		case 58: goto st152;
		case 93: goto st460;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st149;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st149;
	} else
		goto st149;
	goto st0;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st226;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st226;
	} else
		goto st226;
	goto st0;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
	if ( (*p) == 46 )
		goto st227;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st226;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st226;
	} else
		goto st226;
	goto st0;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	switch( (*p) ) {
		case 33: goto st228;
		case 36: goto st228;
		case 61: goto st228;
		case 95: goto st228;
		case 126: goto st228;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st228;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st228;
		} else if ( (*p) >= 65 )
			goto st228;
	} else
		goto st228;
	goto st0;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
	switch( (*p) ) {
		case 33: goto st228;
		case 36: goto st228;
		case 61: goto st228;
		case 93: goto st460;
		case 95: goto st228;
		case 126: goto st228;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st228;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st228;
		} else if ( (*p) >= 65 )
			goto st228;
	} else
		goto st228;
	goto st0;
tr500:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st489;
st489:
	if ( ++p == pe )
		goto _test_eof489;
case 489:
/* #line 4711 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st33;
		case 35: goto tr493;
		case 37: goto st34;
		case 47: goto tr494;
		case 61: goto st33;
		case 63: goto tr496;
		case 64: goto tr38;
		case 95: goto st33;
		case 126: goto st33;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st489;
		} else if ( (*p) >= 36 )
			goto st33;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st33;
		} else if ( (*p) >= 65 )
			goto st33;
	} else
		goto st33;
	goto st0;
tr477:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st490;
st490:
	if ( ++p == pe )
		goto _test_eof490;
case 490:
/* #line 4746 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st491;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st467;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st467;
	} else
		goto st467;
	goto st0;
st491:
	if ( ++p == pe )
		goto _test_eof491;
case 491:
	switch( (*p) ) {
		case 2: goto st13;
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 47: goto tr486;
		case 48: goto st492;
		case 49: goto st500;
		case 50: goto st502;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st501;
	goto st0;
st492:
	if ( ++p == pe )
		goto _test_eof492;
case 492:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st493;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st467;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st467;
	} else
		goto st467;
	goto st0;
st493:
	if ( ++p == pe )
		goto _test_eof493;
case 493:
	switch( (*p) ) {
		case 2: goto st17;
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 47: goto tr486;
		case 48: goto st494;
		case 49: goto st496;
		case 50: goto st498;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st497;
	goto st0;
st494:
	if ( ++p == pe )
		goto _test_eof494;
case 494:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st495;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st467;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st467;
	} else
		goto st467;
	goto st0;
st495:
	if ( ++p == pe )
		goto _test_eof495;
case 495:
	switch( (*p) ) {
		case 2: goto st21;
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st467;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st467;
	} else
		goto st467;
	goto st0;
st496:
	if ( ++p == pe )
		goto _test_eof496;
case 496:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st495;
		case 47: goto tr486;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st497;
	goto st0;
st497:
	if ( ++p == pe )
		goto _test_eof497;
case 497:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st495;
		case 47: goto tr486;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st494;
	goto st0;
st498:
	if ( ++p == pe )
		goto _test_eof498;
case 498:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st495;
		case 47: goto tr486;
		case 53: goto st499;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st494;
	goto st0;
st499:
	if ( ++p == pe )
		goto _test_eof499;
case 499:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st495;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st494;
		} else if ( (*p) >= 36 )
			goto st467;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st467;
	goto st0;
st500:
	if ( ++p == pe )
		goto _test_eof500;
case 500:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st493;
		case 47: goto tr486;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st501;
	goto st0;
st501:
	if ( ++p == pe )
		goto _test_eof501;
case 501:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st493;
		case 47: goto tr486;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st492;
	goto st0;
st502:
	if ( ++p == pe )
		goto _test_eof502;
case 502:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st493;
		case 47: goto tr486;
		case 53: goto st503;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st492;
	goto st0;
st503:
	if ( ++p == pe )
		goto _test_eof503;
case 503:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st493;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st492;
		} else if ( (*p) >= 36 )
			goto st467;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st467;
	goto st0;
tr478:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st504;
st504:
	if ( ++p == pe )
		goto _test_eof504;
case 504:
/* #line 5167 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st491;
		case 47: goto tr486;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st505;
	goto st0;
tr480:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st505;
st505:
	if ( ++p == pe )
		goto _test_eof505;
case 505:
/* #line 5202 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st491;
		case 47: goto tr486;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st490;
	goto st0;
tr479:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st506;
st506:
	if ( ++p == pe )
		goto _test_eof506;
case 506:
/* #line 5237 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st491;
		case 47: goto tr486;
		case 53: goto st507;
		case 58: goto tr499;
		case 59: goto st467;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st467;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st490;
	goto st0;
st507:
	if ( ++p == pe )
		goto _test_eof507;
case 507:
	switch( (*p) ) {
		case 33: goto st467;
		case 35: goto tr485;
		case 37: goto st31;
		case 46: goto st491;
		case 47: goto tr486;
		case 58: goto tr499;
		case 61: goto st467;
		case 63: goto tr488;
		case 64: goto tr38;
		case 95: goto st467;
		case 126: goto st467;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st490;
		} else if ( (*p) >= 36 )
			goto st467;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st467;
		} else if ( (*p) >= 65 )
			goto st467;
	} else
		goto st467;
	goto st0;
tr461:
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
	goto st508;
st508:
	if ( ++p == pe )
		goto _test_eof508;
case 508:
/* #line 5305 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st3;
		case 43: goto st508;
		case 47: goto st455;
		case 58: goto tr543;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 32 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st508;
		} else if ( (*p) >= 0 )
			goto st0;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 91 <= (*p) && (*p) <= 94 )
				goto st0;
		} else if ( (*p) > 122 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else
			goto st508;
	} else
		goto st508;
	goto st452;
tr543:
/* #line 22 "uri_parser.rl" */
	{
            this->setScheme(std::string(mark, p - mark));
            mark = nullptr;
        }
	goto st509;
st509:
	if ( ++p == pe )
		goto _test_eof509;
case 509:
/* #line 5348 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st5;
		case 47: goto st510;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st455;
st510:
	if ( ++p == pe )
		goto _test_eof510;
case 510:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st453;
		case 37: goto st5;
		case 47: goto st511;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st456;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st455;
st511:
	if ( ++p == pe )
		goto _test_eof511;
case 511:
	switch( (*p) ) {
		case 2: goto tr546;
		case 33: goto tr547;
		case 35: goto tr548;
		case 37: goto tr549;
		case 47: goto tr550;
		case 48: goto tr551;
		case 49: goto tr552;
		case 50: goto tr553;
		case 58: goto tr555;
		case 59: goto tr547;
		case 61: goto tr547;
		case 63: goto tr556;
		case 64: goto tr557;
		case 91: goto tr558;
		case 95: goto tr547;
		case 126: goto tr547;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr547;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr547;
		} else if ( (*p) >= 65 )
			goto tr547;
	} else
		goto tr554;
	goto st0;
tr546:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st229;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
/* #line 5435 "uri_parser.rl.c" */
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st230;
	goto st0;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st231;
	goto st0;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
	if ( (*p) == 46 )
		goto st232;
	goto st0;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
	switch( (*p) ) {
		case 2: goto st233;
		case 48: goto st235;
		case 49: goto st249;
		case 50: goto st251;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st250;
	goto st0;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st234;
	goto st0;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st235;
	goto st0;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
	if ( (*p) == 46 )
		goto st236;
	goto st0;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	switch( (*p) ) {
		case 2: goto st237;
		case 48: goto st239;
		case 49: goto st245;
		case 50: goto st247;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st246;
	goto st0;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st238;
	goto st0;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st239;
	goto st0;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	if ( (*p) == 46 )
		goto st240;
	goto st0;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
	switch( (*p) ) {
		case 2: goto st241;
		case 48: goto st512;
		case 49: goto st516;
		case 50: goto st518;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st517;
	goto st0;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st242;
	goto st0;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st512;
	goto st0;
st512:
	if ( ++p == pe )
		goto _test_eof512;
case 512:
	switch( (*p) ) {
		case 35: goto tr559;
		case 47: goto tr560;
		case 58: goto tr561;
		case 63: goto tr562;
	}
	goto st0;
tr550:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st513;
tr567:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	goto st513;
tr560:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st513;
tr571:
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st513;
st513:
	if ( ++p == pe )
		goto _test_eof513;
case 513:
/* #line 5605 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto tr563;
		case 37: goto st243;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto tr565;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st513;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st244;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st244;
	} else
		goto st244;
	goto st0;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st513;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st513;
	} else
		goto st513;
	goto st0;
tr584:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st514;
tr561:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st514;
st514:
	if ( ++p == pe )
		goto _test_eof514;
case 514:
/* #line 5673 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 35: goto tr566;
		case 47: goto tr567;
		case 63: goto tr569;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr568;
	goto st0;
tr568:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st515;
st515:
	if ( ++p == pe )
		goto _test_eof515;
case 515:
/* #line 5690 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 35: goto tr570;
		case 47: goto tr571;
		case 63: goto tr573;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st515;
	goto st0;
st516:
	if ( ++p == pe )
		goto _test_eof516;
case 516:
	switch( (*p) ) {
		case 35: goto tr559;
		case 47: goto tr560;
		case 58: goto tr561;
		case 63: goto tr562;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st517;
	goto st0;
st517:
	if ( ++p == pe )
		goto _test_eof517;
case 517:
	switch( (*p) ) {
		case 35: goto tr559;
		case 47: goto tr560;
		case 58: goto tr561;
		case 63: goto tr562;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st512;
	goto st0;
st518:
	if ( ++p == pe )
		goto _test_eof518;
case 518:
	switch( (*p) ) {
		case 35: goto tr559;
		case 47: goto tr560;
		case 53: goto st519;
		case 58: goto tr561;
		case 63: goto tr562;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st512;
	goto st0;
st519:
	if ( ++p == pe )
		goto _test_eof519;
case 519:
	switch( (*p) ) {
		case 35: goto tr559;
		case 47: goto tr560;
		case 58: goto tr561;
		case 63: goto tr562;
	}
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st512;
	goto st0;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
	if ( (*p) == 46 )
		goto st240;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st246;
	goto st0;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
	if ( (*p) == 46 )
		goto st240;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st239;
	goto st0;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
	switch( (*p) ) {
		case 46: goto st240;
		case 53: goto st248;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st239;
	goto st0;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
	if ( (*p) == 46 )
		goto st240;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st239;
	goto st0;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
	if ( (*p) == 46 )
		goto st236;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st250;
	goto st0;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
	if ( (*p) == 46 )
		goto st236;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st235;
	goto st0;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	switch( (*p) ) {
		case 46: goto st236;
		case 53: goto st252;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st235;
	goto st0;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
	if ( (*p) == 46 )
		goto st236;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st235;
	goto st0;
tr547:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st520;
st520:
	if ( ++p == pe )
		goto _test_eof520;
case 520:
/* #line 5836 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st520;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st520;
	} else
		goto st520;
	goto st0;
tr549:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st253;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
/* #line 5866 "uri_parser.rl.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st254;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st254;
	} else
		goto st254;
	goto st0;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st520;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st520;
	} else
		goto st520;
	goto st0;
tr555:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st521;
tr576:
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	goto st521;
st521:
	if ( ++p == pe )
		goto _test_eof521;
case 521:
/* #line 5911 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st255;
		case 35: goto tr566;
		case 37: goto st256;
		case 47: goto tr567;
		case 61: goto st255;
		case 63: goto tr569;
		case 64: goto tr263;
		case 95: goto st255;
		case 126: goto st255;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr577;
		} else if ( (*p) >= 36 )
			goto st255;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st255;
		} else if ( (*p) >= 65 )
			goto st255;
	} else
		goto st255;
	goto st0;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
	switch( (*p) ) {
		case 33: goto st255;
		case 37: goto st256;
		case 61: goto st255;
		case 64: goto tr263;
		case 95: goto st255;
		case 126: goto st255;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st255;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st255;
		} else if ( (*p) >= 65 )
			goto st255;
	} else
		goto st255;
	goto st0;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st257;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st257;
	} else
		goto st257;
	goto st0;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st255;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st255;
	} else
		goto st255;
	goto st0;
tr263:
/* #line 37 "uri_parser.rl" */
	{
            if(mark){
                this->setUser(std::string(mark, p - mark));
            }
            mark = nullptr;
        }
	goto st522;
tr557:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 37 "uri_parser.rl" */
	{
            if(mark){
                this->setUser(std::string(mark, p - mark));
            }
            mark = nullptr;
        }
	goto st522;
st522:
	if ( ++p == pe )
		goto _test_eof522;
case 522:
/* #line 6012 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 2: goto tr546;
		case 33: goto tr578;
		case 35: goto tr548;
		case 37: goto tr579;
		case 47: goto tr550;
		case 48: goto tr580;
		case 49: goto tr581;
		case 50: goto tr582;
		case 58: goto tr584;
		case 59: goto tr578;
		case 61: goto tr578;
		case 63: goto tr556;
		case 91: goto tr558;
		case 95: goto tr578;
		case 126: goto tr578;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr578;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else if ( (*p) >= 65 )
			goto tr578;
	} else
		goto tr583;
	goto st0;
tr578:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st523;
st523:
	if ( ++p == pe )
		goto _test_eof523;
case 523:
/* #line 6050 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st523;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st523;
	} else
		goto st523;
	goto st0;
tr579:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st258;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
/* #line 6079 "uri_parser.rl.c" */
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st259;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st259;
	} else
		goto st259;
	goto st0;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st523;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st523;
	} else
		goto st523;
	goto st0;
tr580:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st524;
st524:
	if ( ++p == pe )
		goto _test_eof524;
case 524:
/* #line 6110 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st525;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st523;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st523;
	} else
		goto st523;
	goto st0;
st525:
	if ( ++p == pe )
		goto _test_eof525;
case 525:
	switch( (*p) ) {
		case 2: goto st233;
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 47: goto tr560;
		case 48: goto st526;
		case 49: goto st534;
		case 50: goto st536;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st535;
	goto st0;
st526:
	if ( ++p == pe )
		goto _test_eof526;
case 526:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st527;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st523;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st523;
	} else
		goto st523;
	goto st0;
st527:
	if ( ++p == pe )
		goto _test_eof527;
case 527:
	switch( (*p) ) {
		case 2: goto st237;
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 47: goto tr560;
		case 48: goto st528;
		case 49: goto st530;
		case 50: goto st532;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st531;
	goto st0;
st528:
	if ( ++p == pe )
		goto _test_eof528;
case 528:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st529;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st523;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st523;
	} else
		goto st523;
	goto st0;
st529:
	if ( ++p == pe )
		goto _test_eof529;
case 529:
	switch( (*p) ) {
		case 2: goto st241;
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st523;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st523;
	} else
		goto st523;
	goto st0;
st530:
	if ( ++p == pe )
		goto _test_eof530;
case 530:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st529;
		case 47: goto tr560;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st531;
	goto st0;
st531:
	if ( ++p == pe )
		goto _test_eof531;
case 531:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st529;
		case 47: goto tr560;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st528;
	goto st0;
st532:
	if ( ++p == pe )
		goto _test_eof532;
case 532:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st529;
		case 47: goto tr560;
		case 53: goto st533;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st528;
	goto st0;
st533:
	if ( ++p == pe )
		goto _test_eof533;
case 533:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st529;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st528;
		} else if ( (*p) >= 36 )
			goto st523;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st523;
	goto st0;
st534:
	if ( ++p == pe )
		goto _test_eof534;
case 534:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st527;
		case 47: goto tr560;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st535;
	goto st0;
st535:
	if ( ++p == pe )
		goto _test_eof535;
case 535:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st527;
		case 47: goto tr560;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st526;
	goto st0;
st536:
	if ( ++p == pe )
		goto _test_eof536;
case 536:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st527;
		case 47: goto tr560;
		case 53: goto st537;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st526;
	goto st0;
st537:
	if ( ++p == pe )
		goto _test_eof537;
case 537:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st527;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st526;
		} else if ( (*p) >= 36 )
			goto st523;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st523;
	goto st0;
tr581:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st538;
st538:
	if ( ++p == pe )
		goto _test_eof538;
case 538:
/* #line 6517 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st525;
		case 47: goto tr560;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st539;
	goto st0;
tr583:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st539;
st539:
	if ( ++p == pe )
		goto _test_eof539;
case 539:
/* #line 6551 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st525;
		case 47: goto tr560;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st524;
	goto st0;
tr582:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st540;
st540:
	if ( ++p == pe )
		goto _test_eof540;
case 540:
/* #line 6585 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st525;
		case 47: goto tr560;
		case 53: goto st541;
		case 58: goto tr561;
		case 59: goto st523;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st523;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st524;
	goto st0;
st541:
	if ( ++p == pe )
		goto _test_eof541;
case 541:
	switch( (*p) ) {
		case 33: goto st523;
		case 35: goto tr559;
		case 37: goto st258;
		case 46: goto st525;
		case 47: goto tr560;
		case 58: goto tr561;
		case 61: goto st523;
		case 63: goto tr562;
		case 95: goto st523;
		case 126: goto st523;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st524;
		} else if ( (*p) >= 36 )
			goto st523;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st523;
		} else if ( (*p) >= 65 )
			goto st523;
	} else
		goto st523;
	goto st0;
tr558:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st260;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
/* #line 6651 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 58: goto st368;
		case 118: goto st447;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st261;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st261;
	} else
		goto st261;
	goto st0;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
	if ( (*p) == 58 )
		goto st265;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st262;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st262;
	} else
		goto st262;
	goto st0;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
	if ( (*p) == 58 )
		goto st265;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st263;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st263;
	} else
		goto st263;
	goto st0;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	if ( (*p) == 58 )
		goto st265;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st264;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st264;
	} else
		goto st264;
	goto st0;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	if ( (*p) == 58 )
		goto st265;
	goto st0;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	if ( (*p) == 58 )
		goto st363;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st266;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st266;
	} else
		goto st266;
	goto st0;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
	if ( (*p) == 58 )
		goto st270;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st267;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st267;
	} else
		goto st267;
	goto st0;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	if ( (*p) == 58 )
		goto st270;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st268;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st268;
	} else
		goto st268;
	goto st0;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
	if ( (*p) == 58 )
		goto st270;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st269;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st269;
	} else
		goto st269;
	goto st0;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
	if ( (*p) == 58 )
		goto st270;
	goto st0;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
	if ( (*p) == 58 )
		goto st358;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st271;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st271;
	} else
		goto st271;
	goto st0;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
	if ( (*p) == 58 )
		goto st275;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st272;
	} else
		goto st272;
	goto st0;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
	if ( (*p) == 58 )
		goto st275;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st273;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st273;
	} else
		goto st273;
	goto st0;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
	if ( (*p) == 58 )
		goto st275;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st274;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st274;
	} else
		goto st274;
	goto st0;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
	if ( (*p) == 58 )
		goto st275;
	goto st0;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
	if ( (*p) == 58 )
		goto st353;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st276;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st276;
	} else
		goto st276;
	goto st0;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
	if ( (*p) == 58 )
		goto st280;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st277;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st277;
	} else
		goto st277;
	goto st0;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
	if ( (*p) == 58 )
		goto st280;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st278;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st278;
	} else
		goto st278;
	goto st0;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
	if ( (*p) == 58 )
		goto st280;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st279;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st279;
	} else
		goto st279;
	goto st0;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
	if ( (*p) == 58 )
		goto st280;
	goto st0;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
	if ( (*p) == 58 )
		goto st348;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st281;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st281;
	} else
		goto st281;
	goto st0;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
	if ( (*p) == 58 )
		goto st285;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st282;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st282;
	} else
		goto st282;
	goto st0;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	if ( (*p) == 58 )
		goto st285;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st283;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st283;
	} else
		goto st283;
	goto st0;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
	if ( (*p) == 58 )
		goto st285;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st284;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st284;
	} else
		goto st284;
	goto st0;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
	if ( (*p) == 58 )
		goto st285;
	goto st0;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st286;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st286;
	} else
		goto st286;
	goto st0;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
	if ( (*p) == 58 )
		goto st290;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st287;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st287;
	} else
		goto st287;
	goto st0;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
	if ( (*p) == 58 )
		goto st290;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st288;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st288;
	} else
		goto st288;
	goto st0;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
	if ( (*p) == 58 )
		goto st290;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st289;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st289;
	} else
		goto st289;
	goto st0;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
	if ( (*p) == 58 )
		goto st290;
	goto st0;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st318;
		case 49: goto st326;
		case 50: goto st329;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st332;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st334;
	} else
		goto st334;
	goto st0;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st292;
	goto st0;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st293;
	goto st0;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
	if ( (*p) == 46 )
		goto st294;
	goto st0;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
	switch( (*p) ) {
		case 2: goto st295;
		case 48: goto st297;
		case 49: goto st314;
		case 50: goto st316;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st315;
	goto st0;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st296;
	goto st0;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st297;
	goto st0;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
	if ( (*p) == 46 )
		goto st298;
	goto st0;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
	switch( (*p) ) {
		case 2: goto st299;
		case 48: goto st301;
		case 49: goto st310;
		case 50: goto st312;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st311;
	goto st0;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st300;
	goto st0;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st301;
	goto st0;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
	if ( (*p) == 46 )
		goto st302;
	goto st0;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	switch( (*p) ) {
		case 2: goto st303;
		case 48: goto st305;
		case 49: goto st306;
		case 50: goto st308;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto st307;
	goto st0;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
	if ( 48 <= (*p) && (*p) <= 52 )
		goto st304;
	goto st0;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st305;
	goto st0;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
	if ( (*p) == 93 )
		goto st512;
	goto st0;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
	if ( (*p) == 93 )
		goto st512;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st307;
	goto st0;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
	if ( (*p) == 93 )
		goto st512;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st305;
	goto st0;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
	switch( (*p) ) {
		case 53: goto st309;
		case 93: goto st512;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st305;
	goto st0;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
	if ( (*p) == 93 )
		goto st512;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st305;
	goto st0;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
	if ( (*p) == 46 )
		goto st302;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st311;
	goto st0;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
	if ( (*p) == 46 )
		goto st302;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st301;
	goto st0;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
	switch( (*p) ) {
		case 46: goto st302;
		case 53: goto st313;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st301;
	goto st0;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
	if ( (*p) == 46 )
		goto st302;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st301;
	goto st0;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
	if ( (*p) == 46 )
		goto st298;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st315;
	goto st0;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
	if ( (*p) == 46 )
		goto st298;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st297;
	goto st0;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
	switch( (*p) ) {
		case 46: goto st298;
		case 53: goto st317;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st297;
	goto st0;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
	if ( (*p) == 46 )
		goto st298;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st297;
	goto st0;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st319;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st319;
	} else
		goto st319;
	goto st0;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
	if ( (*p) == 58 )
		goto st322;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st320;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st320;
	} else
		goto st320;
	goto st0;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
	if ( (*p) == 58 )
		goto st322;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st321;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
	if ( (*p) == 58 )
		goto st322;
	goto st0;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
	if ( (*p) == 58 )
		goto st305;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st323;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st323;
	} else
		goto st323;
	goto st0;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
	if ( (*p) == 93 )
		goto st512;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st324;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st324;
	} else
		goto st324;
	goto st0;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
	if ( (*p) == 93 )
		goto st512;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st325;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st325;
	} else
		goto st325;
	goto st0;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	if ( (*p) == 93 )
		goto st512;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st305;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st305;
	} else
		goto st305;
	goto st0;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st327;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st319;
	} else
		goto st319;
	goto st0;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st328;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st320;
	} else
		goto st320;
	goto st0;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st321;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st331;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st330;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st319;
	} else
		goto st319;
	goto st0;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st320;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st320;
	} else
		goto st320;
	goto st0;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st328;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st320;
		} else if ( (*p) >= 65 )
			goto st320;
	} else
		goto st320;
	goto st0;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st330;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st319;
	} else
		goto st319;
	goto st0;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st323;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st323;
	} else
		goto st323;
	goto st0;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
	if ( (*p) == 58 )
		goto st322;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st319;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st319;
	} else
		goto st319;
	goto st0;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st336;
		case 49: goto st340;
		case 50: goto st343;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st346;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st347;
	} else
		goto st347;
	goto st0;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st337;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st337;
	} else
		goto st337;
	goto st0;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
	if ( (*p) == 58 )
		goto st333;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st338;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st338;
	} else
		goto st338;
	goto st0;
st338:
	if ( ++p == pe )
		goto _test_eof338;
case 338:
	if ( (*p) == 58 )
		goto st333;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st339;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st339:
	if ( ++p == pe )
		goto _test_eof339;
case 339:
	if ( (*p) == 58 )
		goto st333;
	goto st0;
st340:
	if ( ++p == pe )
		goto _test_eof340;
case 340:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st341;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st337;
	} else
		goto st337;
	goto st0;
st341:
	if ( ++p == pe )
		goto _test_eof341;
case 341:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st342;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st338;
	} else
		goto st338;
	goto st0;
st342:
	if ( ++p == pe )
		goto _test_eof342;
case 342:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st339;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st343:
	if ( ++p == pe )
		goto _test_eof343;
case 343:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st345;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st344;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st337;
	} else
		goto st337;
	goto st0;
st344:
	if ( ++p == pe )
		goto _test_eof344;
case 344:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st338;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st338;
	} else
		goto st338;
	goto st0;
st345:
	if ( ++p == pe )
		goto _test_eof345;
case 345:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st342;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st338;
		} else if ( (*p) >= 65 )
			goto st338;
	} else
		goto st338;
	goto st0;
st346:
	if ( ++p == pe )
		goto _test_eof346;
case 346:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st344;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st337;
	} else
		goto st337;
	goto st0;
st347:
	if ( ++p == pe )
		goto _test_eof347;
case 347:
	if ( (*p) == 58 )
		goto st333;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st337;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st337;
	} else
		goto st337;
	goto st0;
st348:
	if ( ++p == pe )
		goto _test_eof348;
case 348:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st349;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st349;
	} else
		goto st349;
	goto st0;
st349:
	if ( ++p == pe )
		goto _test_eof349;
case 349:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st350;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st350;
	} else
		goto st350;
	goto st0;
st350:
	if ( ++p == pe )
		goto _test_eof350;
case 350:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st351;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st351;
	} else
		goto st351;
	goto st0;
st351:
	if ( ++p == pe )
		goto _test_eof351;
case 351:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st352;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st352;
	} else
		goto st352;
	goto st0;
st352:
	if ( ++p == pe )
		goto _test_eof352;
case 352:
	if ( (*p) == 58 )
		goto st335;
	goto st0;
st353:
	if ( ++p == pe )
		goto _test_eof353;
case 353:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st354;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st354;
	} else
		goto st354;
	goto st0;
st354:
	if ( ++p == pe )
		goto _test_eof354;
case 354:
	if ( (*p) == 58 )
		goto st348;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st355;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st355;
	} else
		goto st355;
	goto st0;
st355:
	if ( ++p == pe )
		goto _test_eof355;
case 355:
	if ( (*p) == 58 )
		goto st348;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st356;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st356;
	} else
		goto st356;
	goto st0;
st356:
	if ( ++p == pe )
		goto _test_eof356;
case 356:
	if ( (*p) == 58 )
		goto st348;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st357;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st357;
	} else
		goto st357;
	goto st0;
st357:
	if ( ++p == pe )
		goto _test_eof357;
case 357:
	if ( (*p) == 58 )
		goto st348;
	goto st0;
st358:
	if ( ++p == pe )
		goto _test_eof358;
case 358:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st359;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st359;
	} else
		goto st359;
	goto st0;
st359:
	if ( ++p == pe )
		goto _test_eof359;
case 359:
	if ( (*p) == 58 )
		goto st353;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st360;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st360;
	} else
		goto st360;
	goto st0;
st360:
	if ( ++p == pe )
		goto _test_eof360;
case 360:
	if ( (*p) == 58 )
		goto st353;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st361;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st361;
	} else
		goto st361;
	goto st0;
st361:
	if ( ++p == pe )
		goto _test_eof361;
case 361:
	if ( (*p) == 58 )
		goto st353;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st362;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st362;
	} else
		goto st362;
	goto st0;
st362:
	if ( ++p == pe )
		goto _test_eof362;
case 362:
	if ( (*p) == 58 )
		goto st353;
	goto st0;
st363:
	if ( ++p == pe )
		goto _test_eof363;
case 363:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st364;
	} else
		goto st364;
	goto st0;
st364:
	if ( ++p == pe )
		goto _test_eof364;
case 364:
	if ( (*p) == 58 )
		goto st358;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st365;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st365;
	} else
		goto st365;
	goto st0;
st365:
	if ( ++p == pe )
		goto _test_eof365;
case 365:
	if ( (*p) == 58 )
		goto st358;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st366;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st366;
	} else
		goto st366;
	goto st0;
st366:
	if ( ++p == pe )
		goto _test_eof366;
case 366:
	if ( (*p) == 58 )
		goto st358;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st367;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st367;
	} else
		goto st367;
	goto st0;
st367:
	if ( ++p == pe )
		goto _test_eof367;
case 367:
	if ( (*p) == 58 )
		goto st358;
	goto st0;
st368:
	if ( ++p == pe )
		goto _test_eof368;
case 368:
	if ( (*p) == 58 )
		goto st369;
	goto st0;
st369:
	if ( ++p == pe )
		goto _test_eof369;
case 369:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st370;
		case 49: goto st439;
		case 50: goto st442;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st445;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st446;
	} else
		goto st446;
	goto st0;
st370:
	if ( ++p == pe )
		goto _test_eof370;
case 370:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st371;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st371;
	} else
		goto st371;
	goto st0;
st371:
	if ( ++p == pe )
		goto _test_eof371;
case 371:
	switch( (*p) ) {
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st372;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st372;
	} else
		goto st372;
	goto st0;
st372:
	if ( ++p == pe )
		goto _test_eof372;
case 372:
	switch( (*p) ) {
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st373;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st373:
	if ( ++p == pe )
		goto _test_eof373;
case 373:
	switch( (*p) ) {
		case 58: goto st374;
		case 93: goto st512;
	}
	goto st0;
st374:
	if ( ++p == pe )
		goto _test_eof374;
case 374:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st375;
		case 49: goto st431;
		case 50: goto st434;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st437;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st438;
	} else
		goto st438;
	goto st0;
st375:
	if ( ++p == pe )
		goto _test_eof375;
case 375:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st376;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st376;
	} else
		goto st376;
	goto st0;
st376:
	if ( ++p == pe )
		goto _test_eof376;
case 376:
	switch( (*p) ) {
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st377;
	} else
		goto st377;
	goto st0;
st377:
	if ( ++p == pe )
		goto _test_eof377;
case 377:
	switch( (*p) ) {
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st378;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st378:
	if ( ++p == pe )
		goto _test_eof378;
case 378:
	switch( (*p) ) {
		case 58: goto st379;
		case 93: goto st512;
	}
	goto st0;
st379:
	if ( ++p == pe )
		goto _test_eof379;
case 379:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st380;
		case 49: goto st423;
		case 50: goto st426;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st429;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st430;
	} else
		goto st430;
	goto st0;
st380:
	if ( ++p == pe )
		goto _test_eof380;
case 380:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st381;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st381;
	} else
		goto st381;
	goto st0;
st381:
	if ( ++p == pe )
		goto _test_eof381;
case 381:
	switch( (*p) ) {
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st382;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st382;
	} else
		goto st382;
	goto st0;
st382:
	if ( ++p == pe )
		goto _test_eof382;
case 382:
	switch( (*p) ) {
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st383;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st383:
	if ( ++p == pe )
		goto _test_eof383;
case 383:
	switch( (*p) ) {
		case 58: goto st384;
		case 93: goto st512;
	}
	goto st0;
st384:
	if ( ++p == pe )
		goto _test_eof384;
case 384:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st385;
		case 49: goto st415;
		case 50: goto st418;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st421;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st422;
	} else
		goto st422;
	goto st0;
st385:
	if ( ++p == pe )
		goto _test_eof385;
case 385:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st386;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st386;
	} else
		goto st386;
	goto st0;
st386:
	if ( ++p == pe )
		goto _test_eof386;
case 386:
	switch( (*p) ) {
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st387;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st387;
	} else
		goto st387;
	goto st0;
st387:
	if ( ++p == pe )
		goto _test_eof387;
case 387:
	switch( (*p) ) {
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st388;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st388:
	if ( ++p == pe )
		goto _test_eof388;
case 388:
	switch( (*p) ) {
		case 58: goto st389;
		case 93: goto st512;
	}
	goto st0;
st389:
	if ( ++p == pe )
		goto _test_eof389;
case 389:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st390;
		case 49: goto st407;
		case 50: goto st410;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st413;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st414;
	} else
		goto st414;
	goto st0;
st390:
	if ( ++p == pe )
		goto _test_eof390;
case 390:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st391;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st391;
	} else
		goto st391;
	goto st0;
st391:
	if ( ++p == pe )
		goto _test_eof391;
case 391:
	switch( (*p) ) {
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st392;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st392;
	} else
		goto st392;
	goto st0;
st392:
	if ( ++p == pe )
		goto _test_eof392;
case 392:
	switch( (*p) ) {
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st393;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st393:
	if ( ++p == pe )
		goto _test_eof393;
case 393:
	switch( (*p) ) {
		case 58: goto st394;
		case 93: goto st512;
	}
	goto st0;
st394:
	if ( ++p == pe )
		goto _test_eof394;
case 394:
	switch( (*p) ) {
		case 2: goto st291;
		case 48: goto st395;
		case 49: goto st399;
		case 50: goto st402;
	}
	if ( (*p) < 65 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st405;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st406;
	} else
		goto st406;
	goto st0;
st395:
	if ( ++p == pe )
		goto _test_eof395;
case 395:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st396;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st396;
	} else
		goto st396;
	goto st0;
st396:
	if ( ++p == pe )
		goto _test_eof396;
case 396:
	switch( (*p) ) {
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st397;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st397;
	} else
		goto st397;
	goto st0;
st397:
	if ( ++p == pe )
		goto _test_eof397;
case 397:
	switch( (*p) ) {
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st398;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st398:
	if ( ++p == pe )
		goto _test_eof398;
case 398:
	switch( (*p) ) {
		case 58: goto st333;
		case 93: goto st512;
	}
	goto st0;
st399:
	if ( ++p == pe )
		goto _test_eof399;
case 399:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st400;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st396;
	} else
		goto st396;
	goto st0;
st400:
	if ( ++p == pe )
		goto _test_eof400;
case 400:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st401;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st397;
	} else
		goto st397;
	goto st0;
st401:
	if ( ++p == pe )
		goto _test_eof401;
case 401:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st398;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st402:
	if ( ++p == pe )
		goto _test_eof402;
case 402:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st404;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st403;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st396;
	} else
		goto st396;
	goto st0;
st403:
	if ( ++p == pe )
		goto _test_eof403;
case 403:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st397;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st397;
	} else
		goto st397;
	goto st0;
st404:
	if ( ++p == pe )
		goto _test_eof404;
case 404:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st401;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st397;
		} else if ( (*p) >= 65 )
			goto st397;
	} else
		goto st397;
	goto st0;
st405:
	if ( ++p == pe )
		goto _test_eof405;
case 405:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st403;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st396;
	} else
		goto st396;
	goto st0;
st406:
	if ( ++p == pe )
		goto _test_eof406;
case 406:
	switch( (*p) ) {
		case 58: goto st333;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st396;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st396;
	} else
		goto st396;
	goto st0;
st407:
	if ( ++p == pe )
		goto _test_eof407;
case 407:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st408;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st391;
	} else
		goto st391;
	goto st0;
st408:
	if ( ++p == pe )
		goto _test_eof408;
case 408:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st409;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st392;
	} else
		goto st392;
	goto st0;
st409:
	if ( ++p == pe )
		goto _test_eof409;
case 409:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st393;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st410:
	if ( ++p == pe )
		goto _test_eof410;
case 410:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st412;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st411;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st391;
	} else
		goto st391;
	goto st0;
st411:
	if ( ++p == pe )
		goto _test_eof411;
case 411:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st392;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st392;
	} else
		goto st392;
	goto st0;
st412:
	if ( ++p == pe )
		goto _test_eof412;
case 412:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st409;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st392;
		} else if ( (*p) >= 65 )
			goto st392;
	} else
		goto st392;
	goto st0;
st413:
	if ( ++p == pe )
		goto _test_eof413;
case 413:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st411;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st391;
	} else
		goto st391;
	goto st0;
st414:
	if ( ++p == pe )
		goto _test_eof414;
case 414:
	switch( (*p) ) {
		case 58: goto st394;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st391;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st391;
	} else
		goto st391;
	goto st0;
st415:
	if ( ++p == pe )
		goto _test_eof415;
case 415:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st416;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st386;
	} else
		goto st386;
	goto st0;
st416:
	if ( ++p == pe )
		goto _test_eof416;
case 416:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st417;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st387;
	} else
		goto st387;
	goto st0;
st417:
	if ( ++p == pe )
		goto _test_eof417;
case 417:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st388;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st418:
	if ( ++p == pe )
		goto _test_eof418;
case 418:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st420;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st419;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st386;
	} else
		goto st386;
	goto st0;
st419:
	if ( ++p == pe )
		goto _test_eof419;
case 419:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st387;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st387;
	} else
		goto st387;
	goto st0;
st420:
	if ( ++p == pe )
		goto _test_eof420;
case 420:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st417;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st387;
		} else if ( (*p) >= 65 )
			goto st387;
	} else
		goto st387;
	goto st0;
st421:
	if ( ++p == pe )
		goto _test_eof421;
case 421:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st419;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st386;
	} else
		goto st386;
	goto st0;
st422:
	if ( ++p == pe )
		goto _test_eof422;
case 422:
	switch( (*p) ) {
		case 58: goto st389;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st386;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st386;
	} else
		goto st386;
	goto st0;
st423:
	if ( ++p == pe )
		goto _test_eof423;
case 423:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st424;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st381;
	} else
		goto st381;
	goto st0;
st424:
	if ( ++p == pe )
		goto _test_eof424;
case 424:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st425;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st382;
	} else
		goto st382;
	goto st0;
st425:
	if ( ++p == pe )
		goto _test_eof425;
case 425:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st383;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st426:
	if ( ++p == pe )
		goto _test_eof426;
case 426:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st428;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st427;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st381;
	} else
		goto st381;
	goto st0;
st427:
	if ( ++p == pe )
		goto _test_eof427;
case 427:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st382;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st382;
	} else
		goto st382;
	goto st0;
st428:
	if ( ++p == pe )
		goto _test_eof428;
case 428:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st425;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st382;
		} else if ( (*p) >= 65 )
			goto st382;
	} else
		goto st382;
	goto st0;
st429:
	if ( ++p == pe )
		goto _test_eof429;
case 429:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st427;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st381;
	} else
		goto st381;
	goto st0;
st430:
	if ( ++p == pe )
		goto _test_eof430;
case 430:
	switch( (*p) ) {
		case 58: goto st384;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st381;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st381;
	} else
		goto st381;
	goto st0;
st431:
	if ( ++p == pe )
		goto _test_eof431;
case 431:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st432;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st376;
	} else
		goto st376;
	goto st0;
st432:
	if ( ++p == pe )
		goto _test_eof432;
case 432:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st433;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st377;
	} else
		goto st377;
	goto st0;
st433:
	if ( ++p == pe )
		goto _test_eof433;
case 433:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st378;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st434:
	if ( ++p == pe )
		goto _test_eof434;
case 434:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st436;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st435;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st376;
	} else
		goto st376;
	goto st0;
st435:
	if ( ++p == pe )
		goto _test_eof435;
case 435:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st377;
	} else
		goto st377;
	goto st0;
st436:
	if ( ++p == pe )
		goto _test_eof436;
case 436:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st433;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st377;
		} else if ( (*p) >= 65 )
			goto st377;
	} else
		goto st377;
	goto st0;
st437:
	if ( ++p == pe )
		goto _test_eof437;
case 437:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st435;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st376;
	} else
		goto st376;
	goto st0;
st438:
	if ( ++p == pe )
		goto _test_eof438;
case 438:
	switch( (*p) ) {
		case 58: goto st379;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st376;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st376;
	} else
		goto st376;
	goto st0;
st439:
	if ( ++p == pe )
		goto _test_eof439;
case 439:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st440;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st371;
	} else
		goto st371;
	goto st0;
st440:
	if ( ++p == pe )
		goto _test_eof440;
case 440:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st441;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st372;
	} else
		goto st372;
	goto st0;
st441:
	if ( ++p == pe )
		goto _test_eof441;
case 441:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st373;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st442:
	if ( ++p == pe )
		goto _test_eof442;
case 442:
	switch( (*p) ) {
		case 46: goto st294;
		case 53: goto st444;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st443;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st371;
	} else
		goto st371;
	goto st0;
st443:
	if ( ++p == pe )
		goto _test_eof443;
case 443:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st372;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st372;
	} else
		goto st372;
	goto st0;
st444:
	if ( ++p == pe )
		goto _test_eof444;
case 444:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st441;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st372;
		} else if ( (*p) >= 65 )
			goto st372;
	} else
		goto st372;
	goto st0;
st445:
	if ( ++p == pe )
		goto _test_eof445;
case 445:
	switch( (*p) ) {
		case 46: goto st294;
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st443;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st371;
	} else
		goto st371;
	goto st0;
st446:
	if ( ++p == pe )
		goto _test_eof446;
case 446:
	switch( (*p) ) {
		case 58: goto st374;
		case 93: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st371;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st371;
	} else
		goto st371;
	goto st0;
st447:
	if ( ++p == pe )
		goto _test_eof447;
case 447:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st448;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st448;
	} else
		goto st448;
	goto st0;
st448:
	if ( ++p == pe )
		goto _test_eof448;
case 448:
	if ( (*p) == 46 )
		goto st449;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st448;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st448;
	} else
		goto st448;
	goto st0;
st449:
	if ( ++p == pe )
		goto _test_eof449;
case 449:
	switch( (*p) ) {
		case 33: goto st450;
		case 36: goto st450;
		case 61: goto st450;
		case 95: goto st450;
		case 126: goto st450;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st450;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st450;
		} else if ( (*p) >= 65 )
			goto st450;
	} else
		goto st450;
	goto st0;
st450:
	if ( ++p == pe )
		goto _test_eof450;
case 450:
	switch( (*p) ) {
		case 33: goto st450;
		case 36: goto st450;
		case 61: goto st450;
		case 93: goto st512;
		case 95: goto st450;
		case 126: goto st450;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st450;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st450;
		} else if ( (*p) >= 65 )
			goto st450;
	} else
		goto st450;
	goto st0;
tr577:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st542;
st542:
	if ( ++p == pe )
		goto _test_eof542;
case 542:
/* #line 9498 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st255;
		case 35: goto tr570;
		case 37: goto st256;
		case 47: goto tr571;
		case 61: goto st255;
		case 63: goto tr573;
		case 64: goto tr263;
		case 95: goto st255;
		case 126: goto st255;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st542;
		} else if ( (*p) >= 36 )
			goto st255;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st255;
		} else if ( (*p) >= 65 )
			goto st255;
	} else
		goto st255;
	goto st0;
tr551:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st543;
st543:
	if ( ++p == pe )
		goto _test_eof543;
case 543:
/* #line 9533 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st544;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st520;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st520;
	} else
		goto st520;
	goto st0;
st544:
	if ( ++p == pe )
		goto _test_eof544;
case 544:
	switch( (*p) ) {
		case 2: goto st233;
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 47: goto tr560;
		case 48: goto st545;
		case 49: goto st553;
		case 50: goto st555;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st554;
	goto st0;
st545:
	if ( ++p == pe )
		goto _test_eof545;
case 545:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st546;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st520;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st520;
	} else
		goto st520;
	goto st0;
st546:
	if ( ++p == pe )
		goto _test_eof546;
case 546:
	switch( (*p) ) {
		case 2: goto st237;
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 47: goto tr560;
		case 48: goto st547;
		case 49: goto st549;
		case 50: goto st551;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 51 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st550;
	goto st0;
st547:
	if ( ++p == pe )
		goto _test_eof547;
case 547:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st548;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st520;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st520;
	} else
		goto st520;
	goto st0;
st548:
	if ( ++p == pe )
		goto _test_eof548;
case 548:
	switch( (*p) ) {
		case 2: goto st241;
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st520;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st520;
	} else
		goto st520;
	goto st0;
st549:
	if ( ++p == pe )
		goto _test_eof549;
case 549:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st548;
		case 47: goto tr560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st550;
	goto st0;
st550:
	if ( ++p == pe )
		goto _test_eof550;
case 550:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st548;
		case 47: goto tr560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st547;
	goto st0;
st551:
	if ( ++p == pe )
		goto _test_eof551;
case 551:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st548;
		case 47: goto tr560;
		case 53: goto st552;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st547;
	goto st0;
st552:
	if ( ++p == pe )
		goto _test_eof552;
case 552:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st548;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st547;
		} else if ( (*p) >= 36 )
			goto st520;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st520;
	goto st0;
st553:
	if ( ++p == pe )
		goto _test_eof553;
case 553:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st546;
		case 47: goto tr560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st554;
	goto st0;
st554:
	if ( ++p == pe )
		goto _test_eof554;
case 554:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st546;
		case 47: goto tr560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st545;
	goto st0;
st555:
	if ( ++p == pe )
		goto _test_eof555;
case 555:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st546;
		case 47: goto tr560;
		case 53: goto st556;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st545;
	goto st0;
st556:
	if ( ++p == pe )
		goto _test_eof556;
case 556:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st546;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st545;
		} else if ( (*p) >= 36 )
			goto st520;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st520;
	goto st0;
tr552:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st557;
st557:
	if ( ++p == pe )
		goto _test_eof557;
case 557:
/* #line 9954 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st544;
		case 47: goto tr560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st558;
	goto st0;
tr554:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st558;
st558:
	if ( ++p == pe )
		goto _test_eof558;
case 558:
/* #line 9989 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st544;
		case 47: goto tr560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st543;
	goto st0;
tr553:
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
	goto st559;
st559:
	if ( ++p == pe )
		goto _test_eof559;
case 559:
/* #line 10024 "uri_parser.rl.c" */
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st544;
		case 47: goto tr560;
		case 53: goto st560;
		case 58: goto tr576;
		case 59: goto st520;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st520;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st543;
	goto st0;
st560:
	if ( ++p == pe )
		goto _test_eof560;
case 560:
	switch( (*p) ) {
		case 33: goto st520;
		case 35: goto tr559;
		case 37: goto st253;
		case 46: goto st544;
		case 47: goto tr560;
		case 58: goto tr576;
		case 61: goto st520;
		case 63: goto tr562;
		case 64: goto tr263;
		case 95: goto st520;
		case 126: goto st520;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 45 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto st543;
		} else if ( (*p) >= 36 )
			goto st520;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st520;
		} else if ( (*p) >= 65 )
			goto st520;
	} else
		goto st520;
	goto st0;
	}
	_test_eof452: cs = 452; goto _test_eof; 
	_test_eof453: cs = 453; goto _test_eof; 
	_test_eof454: cs = 454; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof455: cs = 455; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof456: cs = 456; goto _test_eof; 
	_test_eof457: cs = 457; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof458: cs = 458; goto _test_eof; 
	_test_eof459: cs = 459; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof460: cs = 460; goto _test_eof; 
	_test_eof461: cs = 461; goto _test_eof; 
	_test_eof462: cs = 462; goto _test_eof; 
	_test_eof463: cs = 463; goto _test_eof; 
	_test_eof464: cs = 464; goto _test_eof; 
	_test_eof465: cs = 465; goto _test_eof; 
	_test_eof466: cs = 466; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof467: cs = 467; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof468: cs = 468; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof469: cs = 469; goto _test_eof; 
	_test_eof470: cs = 470; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof471: cs = 471; goto _test_eof; 
	_test_eof472: cs = 472; goto _test_eof; 
	_test_eof473: cs = 473; goto _test_eof; 
	_test_eof474: cs = 474; goto _test_eof; 
	_test_eof475: cs = 475; goto _test_eof; 
	_test_eof476: cs = 476; goto _test_eof; 
	_test_eof477: cs = 477; goto _test_eof; 
	_test_eof478: cs = 478; goto _test_eof; 
	_test_eof479: cs = 479; goto _test_eof; 
	_test_eof480: cs = 480; goto _test_eof; 
	_test_eof481: cs = 481; goto _test_eof; 
	_test_eof482: cs = 482; goto _test_eof; 
	_test_eof483: cs = 483; goto _test_eof; 
	_test_eof484: cs = 484; goto _test_eof; 
	_test_eof485: cs = 485; goto _test_eof; 
	_test_eof486: cs = 486; goto _test_eof; 
	_test_eof487: cs = 487; goto _test_eof; 
	_test_eof488: cs = 488; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 
	_test_eof147: cs = 147; goto _test_eof; 
	_test_eof148: cs = 148; goto _test_eof; 
	_test_eof149: cs = 149; goto _test_eof; 
	_test_eof150: cs = 150; goto _test_eof; 
	_test_eof151: cs = 151; goto _test_eof; 
	_test_eof152: cs = 152; goto _test_eof; 
	_test_eof153: cs = 153; goto _test_eof; 
	_test_eof154: cs = 154; goto _test_eof; 
	_test_eof155: cs = 155; goto _test_eof; 
	_test_eof156: cs = 156; goto _test_eof; 
	_test_eof157: cs = 157; goto _test_eof; 
	_test_eof158: cs = 158; goto _test_eof; 
	_test_eof159: cs = 159; goto _test_eof; 
	_test_eof160: cs = 160; goto _test_eof; 
	_test_eof161: cs = 161; goto _test_eof; 
	_test_eof162: cs = 162; goto _test_eof; 
	_test_eof163: cs = 163; goto _test_eof; 
	_test_eof164: cs = 164; goto _test_eof; 
	_test_eof165: cs = 165; goto _test_eof; 
	_test_eof166: cs = 166; goto _test_eof; 
	_test_eof167: cs = 167; goto _test_eof; 
	_test_eof168: cs = 168; goto _test_eof; 
	_test_eof169: cs = 169; goto _test_eof; 
	_test_eof170: cs = 170; goto _test_eof; 
	_test_eof171: cs = 171; goto _test_eof; 
	_test_eof172: cs = 172; goto _test_eof; 
	_test_eof173: cs = 173; goto _test_eof; 
	_test_eof174: cs = 174; goto _test_eof; 
	_test_eof175: cs = 175; goto _test_eof; 
	_test_eof176: cs = 176; goto _test_eof; 
	_test_eof177: cs = 177; goto _test_eof; 
	_test_eof178: cs = 178; goto _test_eof; 
	_test_eof179: cs = 179; goto _test_eof; 
	_test_eof180: cs = 180; goto _test_eof; 
	_test_eof181: cs = 181; goto _test_eof; 
	_test_eof182: cs = 182; goto _test_eof; 
	_test_eof183: cs = 183; goto _test_eof; 
	_test_eof184: cs = 184; goto _test_eof; 
	_test_eof185: cs = 185; goto _test_eof; 
	_test_eof186: cs = 186; goto _test_eof; 
	_test_eof187: cs = 187; goto _test_eof; 
	_test_eof188: cs = 188; goto _test_eof; 
	_test_eof189: cs = 189; goto _test_eof; 
	_test_eof190: cs = 190; goto _test_eof; 
	_test_eof191: cs = 191; goto _test_eof; 
	_test_eof192: cs = 192; goto _test_eof; 
	_test_eof193: cs = 193; goto _test_eof; 
	_test_eof194: cs = 194; goto _test_eof; 
	_test_eof195: cs = 195; goto _test_eof; 
	_test_eof196: cs = 196; goto _test_eof; 
	_test_eof197: cs = 197; goto _test_eof; 
	_test_eof198: cs = 198; goto _test_eof; 
	_test_eof199: cs = 199; goto _test_eof; 
	_test_eof200: cs = 200; goto _test_eof; 
	_test_eof201: cs = 201; goto _test_eof; 
	_test_eof202: cs = 202; goto _test_eof; 
	_test_eof203: cs = 203; goto _test_eof; 
	_test_eof204: cs = 204; goto _test_eof; 
	_test_eof205: cs = 205; goto _test_eof; 
	_test_eof206: cs = 206; goto _test_eof; 
	_test_eof207: cs = 207; goto _test_eof; 
	_test_eof208: cs = 208; goto _test_eof; 
	_test_eof209: cs = 209; goto _test_eof; 
	_test_eof210: cs = 210; goto _test_eof; 
	_test_eof211: cs = 211; goto _test_eof; 
	_test_eof212: cs = 212; goto _test_eof; 
	_test_eof213: cs = 213; goto _test_eof; 
	_test_eof214: cs = 214; goto _test_eof; 
	_test_eof215: cs = 215; goto _test_eof; 
	_test_eof216: cs = 216; goto _test_eof; 
	_test_eof217: cs = 217; goto _test_eof; 
	_test_eof218: cs = 218; goto _test_eof; 
	_test_eof219: cs = 219; goto _test_eof; 
	_test_eof220: cs = 220; goto _test_eof; 
	_test_eof221: cs = 221; goto _test_eof; 
	_test_eof222: cs = 222; goto _test_eof; 
	_test_eof223: cs = 223; goto _test_eof; 
	_test_eof224: cs = 224; goto _test_eof; 
	_test_eof225: cs = 225; goto _test_eof; 
	_test_eof226: cs = 226; goto _test_eof; 
	_test_eof227: cs = 227; goto _test_eof; 
	_test_eof228: cs = 228; goto _test_eof; 
	_test_eof489: cs = 489; goto _test_eof; 
	_test_eof490: cs = 490; goto _test_eof; 
	_test_eof491: cs = 491; goto _test_eof; 
	_test_eof492: cs = 492; goto _test_eof; 
	_test_eof493: cs = 493; goto _test_eof; 
	_test_eof494: cs = 494; goto _test_eof; 
	_test_eof495: cs = 495; goto _test_eof; 
	_test_eof496: cs = 496; goto _test_eof; 
	_test_eof497: cs = 497; goto _test_eof; 
	_test_eof498: cs = 498; goto _test_eof; 
	_test_eof499: cs = 499; goto _test_eof; 
	_test_eof500: cs = 500; goto _test_eof; 
	_test_eof501: cs = 501; goto _test_eof; 
	_test_eof502: cs = 502; goto _test_eof; 
	_test_eof503: cs = 503; goto _test_eof; 
	_test_eof504: cs = 504; goto _test_eof; 
	_test_eof505: cs = 505; goto _test_eof; 
	_test_eof506: cs = 506; goto _test_eof; 
	_test_eof507: cs = 507; goto _test_eof; 
	_test_eof508: cs = 508; goto _test_eof; 
	_test_eof509: cs = 509; goto _test_eof; 
	_test_eof510: cs = 510; goto _test_eof; 
	_test_eof511: cs = 511; goto _test_eof; 
	_test_eof229: cs = 229; goto _test_eof; 
	_test_eof230: cs = 230; goto _test_eof; 
	_test_eof231: cs = 231; goto _test_eof; 
	_test_eof232: cs = 232; goto _test_eof; 
	_test_eof233: cs = 233; goto _test_eof; 
	_test_eof234: cs = 234; goto _test_eof; 
	_test_eof235: cs = 235; goto _test_eof; 
	_test_eof236: cs = 236; goto _test_eof; 
	_test_eof237: cs = 237; goto _test_eof; 
	_test_eof238: cs = 238; goto _test_eof; 
	_test_eof239: cs = 239; goto _test_eof; 
	_test_eof240: cs = 240; goto _test_eof; 
	_test_eof241: cs = 241; goto _test_eof; 
	_test_eof242: cs = 242; goto _test_eof; 
	_test_eof512: cs = 512; goto _test_eof; 
	_test_eof513: cs = 513; goto _test_eof; 
	_test_eof243: cs = 243; goto _test_eof; 
	_test_eof244: cs = 244; goto _test_eof; 
	_test_eof514: cs = 514; goto _test_eof; 
	_test_eof515: cs = 515; goto _test_eof; 
	_test_eof516: cs = 516; goto _test_eof; 
	_test_eof517: cs = 517; goto _test_eof; 
	_test_eof518: cs = 518; goto _test_eof; 
	_test_eof519: cs = 519; goto _test_eof; 
	_test_eof245: cs = 245; goto _test_eof; 
	_test_eof246: cs = 246; goto _test_eof; 
	_test_eof247: cs = 247; goto _test_eof; 
	_test_eof248: cs = 248; goto _test_eof; 
	_test_eof249: cs = 249; goto _test_eof; 
	_test_eof250: cs = 250; goto _test_eof; 
	_test_eof251: cs = 251; goto _test_eof; 
	_test_eof252: cs = 252; goto _test_eof; 
	_test_eof520: cs = 520; goto _test_eof; 
	_test_eof253: cs = 253; goto _test_eof; 
	_test_eof254: cs = 254; goto _test_eof; 
	_test_eof521: cs = 521; goto _test_eof; 
	_test_eof255: cs = 255; goto _test_eof; 
	_test_eof256: cs = 256; goto _test_eof; 
	_test_eof257: cs = 257; goto _test_eof; 
	_test_eof522: cs = 522; goto _test_eof; 
	_test_eof523: cs = 523; goto _test_eof; 
	_test_eof258: cs = 258; goto _test_eof; 
	_test_eof259: cs = 259; goto _test_eof; 
	_test_eof524: cs = 524; goto _test_eof; 
	_test_eof525: cs = 525; goto _test_eof; 
	_test_eof526: cs = 526; goto _test_eof; 
	_test_eof527: cs = 527; goto _test_eof; 
	_test_eof528: cs = 528; goto _test_eof; 
	_test_eof529: cs = 529; goto _test_eof; 
	_test_eof530: cs = 530; goto _test_eof; 
	_test_eof531: cs = 531; goto _test_eof; 
	_test_eof532: cs = 532; goto _test_eof; 
	_test_eof533: cs = 533; goto _test_eof; 
	_test_eof534: cs = 534; goto _test_eof; 
	_test_eof535: cs = 535; goto _test_eof; 
	_test_eof536: cs = 536; goto _test_eof; 
	_test_eof537: cs = 537; goto _test_eof; 
	_test_eof538: cs = 538; goto _test_eof; 
	_test_eof539: cs = 539; goto _test_eof; 
	_test_eof540: cs = 540; goto _test_eof; 
	_test_eof541: cs = 541; goto _test_eof; 
	_test_eof260: cs = 260; goto _test_eof; 
	_test_eof261: cs = 261; goto _test_eof; 
	_test_eof262: cs = 262; goto _test_eof; 
	_test_eof263: cs = 263; goto _test_eof; 
	_test_eof264: cs = 264; goto _test_eof; 
	_test_eof265: cs = 265; goto _test_eof; 
	_test_eof266: cs = 266; goto _test_eof; 
	_test_eof267: cs = 267; goto _test_eof; 
	_test_eof268: cs = 268; goto _test_eof; 
	_test_eof269: cs = 269; goto _test_eof; 
	_test_eof270: cs = 270; goto _test_eof; 
	_test_eof271: cs = 271; goto _test_eof; 
	_test_eof272: cs = 272; goto _test_eof; 
	_test_eof273: cs = 273; goto _test_eof; 
	_test_eof274: cs = 274; goto _test_eof; 
	_test_eof275: cs = 275; goto _test_eof; 
	_test_eof276: cs = 276; goto _test_eof; 
	_test_eof277: cs = 277; goto _test_eof; 
	_test_eof278: cs = 278; goto _test_eof; 
	_test_eof279: cs = 279; goto _test_eof; 
	_test_eof280: cs = 280; goto _test_eof; 
	_test_eof281: cs = 281; goto _test_eof; 
	_test_eof282: cs = 282; goto _test_eof; 
	_test_eof283: cs = 283; goto _test_eof; 
	_test_eof284: cs = 284; goto _test_eof; 
	_test_eof285: cs = 285; goto _test_eof; 
	_test_eof286: cs = 286; goto _test_eof; 
	_test_eof287: cs = 287; goto _test_eof; 
	_test_eof288: cs = 288; goto _test_eof; 
	_test_eof289: cs = 289; goto _test_eof; 
	_test_eof290: cs = 290; goto _test_eof; 
	_test_eof291: cs = 291; goto _test_eof; 
	_test_eof292: cs = 292; goto _test_eof; 
	_test_eof293: cs = 293; goto _test_eof; 
	_test_eof294: cs = 294; goto _test_eof; 
	_test_eof295: cs = 295; goto _test_eof; 
	_test_eof296: cs = 296; goto _test_eof; 
	_test_eof297: cs = 297; goto _test_eof; 
	_test_eof298: cs = 298; goto _test_eof; 
	_test_eof299: cs = 299; goto _test_eof; 
	_test_eof300: cs = 300; goto _test_eof; 
	_test_eof301: cs = 301; goto _test_eof; 
	_test_eof302: cs = 302; goto _test_eof; 
	_test_eof303: cs = 303; goto _test_eof; 
	_test_eof304: cs = 304; goto _test_eof; 
	_test_eof305: cs = 305; goto _test_eof; 
	_test_eof306: cs = 306; goto _test_eof; 
	_test_eof307: cs = 307; goto _test_eof; 
	_test_eof308: cs = 308; goto _test_eof; 
	_test_eof309: cs = 309; goto _test_eof; 
	_test_eof310: cs = 310; goto _test_eof; 
	_test_eof311: cs = 311; goto _test_eof; 
	_test_eof312: cs = 312; goto _test_eof; 
	_test_eof313: cs = 313; goto _test_eof; 
	_test_eof314: cs = 314; goto _test_eof; 
	_test_eof315: cs = 315; goto _test_eof; 
	_test_eof316: cs = 316; goto _test_eof; 
	_test_eof317: cs = 317; goto _test_eof; 
	_test_eof318: cs = 318; goto _test_eof; 
	_test_eof319: cs = 319; goto _test_eof; 
	_test_eof320: cs = 320; goto _test_eof; 
	_test_eof321: cs = 321; goto _test_eof; 
	_test_eof322: cs = 322; goto _test_eof; 
	_test_eof323: cs = 323; goto _test_eof; 
	_test_eof324: cs = 324; goto _test_eof; 
	_test_eof325: cs = 325; goto _test_eof; 
	_test_eof326: cs = 326; goto _test_eof; 
	_test_eof327: cs = 327; goto _test_eof; 
	_test_eof328: cs = 328; goto _test_eof; 
	_test_eof329: cs = 329; goto _test_eof; 
	_test_eof330: cs = 330; goto _test_eof; 
	_test_eof331: cs = 331; goto _test_eof; 
	_test_eof332: cs = 332; goto _test_eof; 
	_test_eof333: cs = 333; goto _test_eof; 
	_test_eof334: cs = 334; goto _test_eof; 
	_test_eof335: cs = 335; goto _test_eof; 
	_test_eof336: cs = 336; goto _test_eof; 
	_test_eof337: cs = 337; goto _test_eof; 
	_test_eof338: cs = 338; goto _test_eof; 
	_test_eof339: cs = 339; goto _test_eof; 
	_test_eof340: cs = 340; goto _test_eof; 
	_test_eof341: cs = 341; goto _test_eof; 
	_test_eof342: cs = 342; goto _test_eof; 
	_test_eof343: cs = 343; goto _test_eof; 
	_test_eof344: cs = 344; goto _test_eof; 
	_test_eof345: cs = 345; goto _test_eof; 
	_test_eof346: cs = 346; goto _test_eof; 
	_test_eof347: cs = 347; goto _test_eof; 
	_test_eof348: cs = 348; goto _test_eof; 
	_test_eof349: cs = 349; goto _test_eof; 
	_test_eof350: cs = 350; goto _test_eof; 
	_test_eof351: cs = 351; goto _test_eof; 
	_test_eof352: cs = 352; goto _test_eof; 
	_test_eof353: cs = 353; goto _test_eof; 
	_test_eof354: cs = 354; goto _test_eof; 
	_test_eof355: cs = 355; goto _test_eof; 
	_test_eof356: cs = 356; goto _test_eof; 
	_test_eof357: cs = 357; goto _test_eof; 
	_test_eof358: cs = 358; goto _test_eof; 
	_test_eof359: cs = 359; goto _test_eof; 
	_test_eof360: cs = 360; goto _test_eof; 
	_test_eof361: cs = 361; goto _test_eof; 
	_test_eof362: cs = 362; goto _test_eof; 
	_test_eof363: cs = 363; goto _test_eof; 
	_test_eof364: cs = 364; goto _test_eof; 
	_test_eof365: cs = 365; goto _test_eof; 
	_test_eof366: cs = 366; goto _test_eof; 
	_test_eof367: cs = 367; goto _test_eof; 
	_test_eof368: cs = 368; goto _test_eof; 
	_test_eof369: cs = 369; goto _test_eof; 
	_test_eof370: cs = 370; goto _test_eof; 
	_test_eof371: cs = 371; goto _test_eof; 
	_test_eof372: cs = 372; goto _test_eof; 
	_test_eof373: cs = 373; goto _test_eof; 
	_test_eof374: cs = 374; goto _test_eof; 
	_test_eof375: cs = 375; goto _test_eof; 
	_test_eof376: cs = 376; goto _test_eof; 
	_test_eof377: cs = 377; goto _test_eof; 
	_test_eof378: cs = 378; goto _test_eof; 
	_test_eof379: cs = 379; goto _test_eof; 
	_test_eof380: cs = 380; goto _test_eof; 
	_test_eof381: cs = 381; goto _test_eof; 
	_test_eof382: cs = 382; goto _test_eof; 
	_test_eof383: cs = 383; goto _test_eof; 
	_test_eof384: cs = 384; goto _test_eof; 
	_test_eof385: cs = 385; goto _test_eof; 
	_test_eof386: cs = 386; goto _test_eof; 
	_test_eof387: cs = 387; goto _test_eof; 
	_test_eof388: cs = 388; goto _test_eof; 
	_test_eof389: cs = 389; goto _test_eof; 
	_test_eof390: cs = 390; goto _test_eof; 
	_test_eof391: cs = 391; goto _test_eof; 
	_test_eof392: cs = 392; goto _test_eof; 
	_test_eof393: cs = 393; goto _test_eof; 
	_test_eof394: cs = 394; goto _test_eof; 
	_test_eof395: cs = 395; goto _test_eof; 
	_test_eof396: cs = 396; goto _test_eof; 
	_test_eof397: cs = 397; goto _test_eof; 
	_test_eof398: cs = 398; goto _test_eof; 
	_test_eof399: cs = 399; goto _test_eof; 
	_test_eof400: cs = 400; goto _test_eof; 
	_test_eof401: cs = 401; goto _test_eof; 
	_test_eof402: cs = 402; goto _test_eof; 
	_test_eof403: cs = 403; goto _test_eof; 
	_test_eof404: cs = 404; goto _test_eof; 
	_test_eof405: cs = 405; goto _test_eof; 
	_test_eof406: cs = 406; goto _test_eof; 
	_test_eof407: cs = 407; goto _test_eof; 
	_test_eof408: cs = 408; goto _test_eof; 
	_test_eof409: cs = 409; goto _test_eof; 
	_test_eof410: cs = 410; goto _test_eof; 
	_test_eof411: cs = 411; goto _test_eof; 
	_test_eof412: cs = 412; goto _test_eof; 
	_test_eof413: cs = 413; goto _test_eof; 
	_test_eof414: cs = 414; goto _test_eof; 
	_test_eof415: cs = 415; goto _test_eof; 
	_test_eof416: cs = 416; goto _test_eof; 
	_test_eof417: cs = 417; goto _test_eof; 
	_test_eof418: cs = 418; goto _test_eof; 
	_test_eof419: cs = 419; goto _test_eof; 
	_test_eof420: cs = 420; goto _test_eof; 
	_test_eof421: cs = 421; goto _test_eof; 
	_test_eof422: cs = 422; goto _test_eof; 
	_test_eof423: cs = 423; goto _test_eof; 
	_test_eof424: cs = 424; goto _test_eof; 
	_test_eof425: cs = 425; goto _test_eof; 
	_test_eof426: cs = 426; goto _test_eof; 
	_test_eof427: cs = 427; goto _test_eof; 
	_test_eof428: cs = 428; goto _test_eof; 
	_test_eof429: cs = 429; goto _test_eof; 
	_test_eof430: cs = 430; goto _test_eof; 
	_test_eof431: cs = 431; goto _test_eof; 
	_test_eof432: cs = 432; goto _test_eof; 
	_test_eof433: cs = 433; goto _test_eof; 
	_test_eof434: cs = 434; goto _test_eof; 
	_test_eof435: cs = 435; goto _test_eof; 
	_test_eof436: cs = 436; goto _test_eof; 
	_test_eof437: cs = 437; goto _test_eof; 
	_test_eof438: cs = 438; goto _test_eof; 
	_test_eof439: cs = 439; goto _test_eof; 
	_test_eof440: cs = 440; goto _test_eof; 
	_test_eof441: cs = 441; goto _test_eof; 
	_test_eof442: cs = 442; goto _test_eof; 
	_test_eof443: cs = 443; goto _test_eof; 
	_test_eof444: cs = 444; goto _test_eof; 
	_test_eof445: cs = 445; goto _test_eof; 
	_test_eof446: cs = 446; goto _test_eof; 
	_test_eof447: cs = 447; goto _test_eof; 
	_test_eof448: cs = 448; goto _test_eof; 
	_test_eof449: cs = 449; goto _test_eof; 
	_test_eof450: cs = 450; goto _test_eof; 
	_test_eof542: cs = 542; goto _test_eof; 
	_test_eof543: cs = 543; goto _test_eof; 
	_test_eof544: cs = 544; goto _test_eof; 
	_test_eof545: cs = 545; goto _test_eof; 
	_test_eof546: cs = 546; goto _test_eof; 
	_test_eof547: cs = 547; goto _test_eof; 
	_test_eof548: cs = 548; goto _test_eof; 
	_test_eof549: cs = 549; goto _test_eof; 
	_test_eof550: cs = 550; goto _test_eof; 
	_test_eof551: cs = 551; goto _test_eof; 
	_test_eof552: cs = 552; goto _test_eof; 
	_test_eof553: cs = 553; goto _test_eof; 
	_test_eof554: cs = 554; goto _test_eof; 
	_test_eof555: cs = 555; goto _test_eof; 
	_test_eof556: cs = 556; goto _test_eof; 
	_test_eof557: cs = 557; goto _test_eof; 
	_test_eof558: cs = 558; goto _test_eof; 
	_test_eof559: cs = 559; goto _test_eof; 
	_test_eof560: cs = 560; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 462: 
	case 489: 
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	break;
	case 460: 
	case 463: 
	case 464: 
	case 465: 
	case 466: 
	case 467: 
	case 470: 
	case 471: 
	case 472: 
	case 473: 
	case 474: 
	case 475: 
	case 476: 
	case 477: 
	case 478: 
	case 479: 
	case 480: 
	case 481: 
	case 482: 
	case 483: 
	case 484: 
	case 485: 
	case 486: 
	case 487: 
	case 488: 
	case 490: 
	case 491: 
	case 492: 
	case 493: 
	case 494: 
	case 495: 
	case 496: 
	case 497: 
	case 498: 
	case 499: 
	case 500: 
	case 501: 
	case 502: 
	case 503: 
	case 504: 
	case 505: 
	case 506: 
	case 507: 
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	break;
	case 513: 
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 457: 
/* #line 103 "uri_parser.rl" */
	{
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setQuery(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 454: 
/* #line 109 "uri_parser.rl" */
	{
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setFragment(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 456: 
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
/* #line 103 "uri_parser.rl" */
	{
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setQuery(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 453: 
/* #line 18 "uri_parser.rl" */
	{ mark = p; }
/* #line 109 "uri_parser.rl" */
	{
            //std::cout << std::string(mark, fpc - mark) << std::endl;
            this->setFragment(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 461: 
	case 468: 
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
	break;
	case 459: 
	case 469: 
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
	break;
	case 514: 
	case 521: 
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 511: 
	case 522: 
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 515: 
	case 542: 
/* #line 30 "uri_parser.rl" */
	{
            if (p != mark){
                this->setPort(atoi(mark));
            }
            mark = nullptr;
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
	case 512: 
	case 516: 
	case 517: 
	case 518: 
	case 519: 
	case 520: 
	case 523: 
	case 524: 
	case 525: 
	case 526: 
	case 527: 
	case 528: 
	case 529: 
	case 530: 
	case 531: 
	case 532: 
	case 533: 
	case 534: 
	case 535: 
	case 536: 
	case 537: 
	case 538: 
	case 539: 
	case 540: 
	case 541: 
	case 543: 
	case 544: 
	case 545: 
	case 546: 
	case 547: 
	case 548: 
	case 549: 
	case 550: 
	case 551: 
	case 552: 
	case 553: 
	case 554: 
	case 555: 
	case 556: 
	case 557: 
	case 558: 
	case 559: 
	case 560: 
/* #line 44 "uri_parser.rl" */
	{
            if (mark != nullptr){
                this->setHost(std::string(mark, p - mark));
            }
        }
/* #line 19 "uri_parser.rl" */
	{ mark = p; }
/* #line 78 "uri_parser.rl" */
	{
            this->setPath(std::string(mark, p - mark));
            mark = nullptr;
        }
	break;
/* #line 10882 "uri_parser.rl.c" */
	}
	}

	_out: {}
	}

/* #line 207 "uri_parser.rl" */
        if(cs == uri_parser_error){
            return false; //error
        } else if(cs >= uri_parser_first_final){
            return true;//ok
        }
        return false;//error
    }

}
}
}
