include(../plugin.pri)

DEFINES += RPD_LIBRARY

SOURCES += rpdplugin.cpp \
    qjsonparser/json.cpp

HEADERS += rpd_global.h \
    rpdplugin.h \
    qjsonparser/json.h
