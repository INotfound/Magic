LIBS+= -lpthread

HEADERS += \
    Magic/Log/Log.h \
    Magic/Log/logAppender.h \
    Magic/Log/logEvent.h \
    Magic/Log/logFormatter.h \
    Magic/Log/logLevel.h \
    Magic/Log/logWrap.h \
    Magic/Log/logger.h \
    Magic/Log/loggerManager.h \
    Magic/Thread/Thread.h \
    Magic/Util/Noncopyable.h \
    Magic/Util/Singleton.h \
    Magic/Util/Util.h

SOURCES += \
    Magic/Log/logAppender.cc \
    Magic/Log/logEvent.cc \
    Magic/Log/logFormatter.cc \
    Magic/Log/logLevel.cc \
    Magic/Log/logWrap.cc \
    Magic/Log/logger.cc \
    Magic/Log/loggerManager.cc \
    Magic/Thread/Thread.cc \
    Magic/Util/Util.cc \
    main.cc
