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



INCLUDEPATH +=/home/xuyf/develop/libcommon/include
INCLUDEPATH +=/home/xuyf/develop/boost_1_69_0
INCLUDEPATH +=/home/xuyf/develop/libcommon_1.69.0/src
INCLUDEPATH +=/home/xuyf/develop/libcommon_1.69.0/src/LibXML

LIBS += /home/xuyf/develop/boost_1_69_0/bin.v2/libs/system/build/gcc-arm/release/link-static/threading-multi/visibility-hidden/libboost_system.a
LIBS +=/home/xuyf/develop/boost_1_69_0/bin.v2/libs/thread/build/gcc-arm/release/link-static/threadapi-pthread/threading-multi/visibility-hidden/libboost_thread.a
LIBS += /home/xuyf/develop/boost_1_69_0/bin.v2/libs/filesystem/build/gcc-arm/release/link-static/threading-multi/visibility-hidden/libboost_filesystem.a
