#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T10:47:41
#
#-------------------------------------------------

QT       -= gui

TARGET = xml
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ../../xmlParser.cpp \
    ../../LibXML.cpp

HEADERS += \
    ../../xmlParser.h \
    ../../LibXML.h \
    ../../Defines.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


INCLUDEPATH +=/home/develop/libcommon/include
INCLUDEPATH +=/home/develop/boost_1_69_0

LIBS += /home/develop/boost_1_69_0/bin.v2/libs/system/build/gcc-arm/release/link-static/threading-multi/visibility-hidden/libboost_system.a
LIBS +=/home/develop/boost_1_69_0/bin.v2/libs/thread/build/gcc-arm/release/link-static/threadapi-pthread/threading-multi/visibility-hidden/libboost_thread.a
LIBS += /home/develop/boost_1_69_0/bin.v2/libs/filesystem/build/gcc-arm/release/link-static/threading-multi/visibility-hidden/libboost_filesystem.a
