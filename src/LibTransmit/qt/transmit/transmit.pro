#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T10:57:27
#
#-------------------------------------------------

QT       -= gui

TARGET = transmit
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ../../RequestHandler.cpp \
    ../../TransmitClient.cpp \
    ../../Service.cpp \
    ../../Server.cpp \
    ../../ConnectionManager.cpp \
    ../../Connection.cpp

HEADERS += \
    ../../TransmitDefines.h \
    ../../TransmitClient.h \
    ../../Service.h \
    ../../Server.h \
    ../../RequestHandler.h \
    ../../ConnectionManager.h \
    ../../Connection.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}



INCLUDEPATH +=/home/develop/libcommon/include
INCLUDEPATH +=/home/develop/boost_1_69_0
INCLUDEPATH +=/home/develop/libcommon_1.69.0/src
INCLUDEPATH +=/home/develop/libcommon_1.69.0/src/LibXML

LIBS += /home/develop/boost_1_69_0/bin.v2/libs/system/build/gcc-arm/release/link-static/threading-multi/visibility-hidden/libboost_system.a
LIBS +=/home/develop/boost_1_69_0/bin.v2/libs/thread/build/gcc-arm/release/link-static/threadapi-pthread/threading-multi/visibility-hidden/libboost_thread.a
LIBS += /home/develop/boost_1_69_0/bin.v2/libs/filesystem/build/gcc-arm/release/link-static/threading-multi/visibility-hidden/libboost_filesystem.a


