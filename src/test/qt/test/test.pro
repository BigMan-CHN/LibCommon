TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../test.cpp \
    ../../MControl.cpp

HEADERS += \
    ../../MControl.h

INCLUDEPATH +=/home/MPS/libcommon/src/LibXML
INCLUDEPATH +=/home/MPS/jrtp/lib/include
INCLUDEPATH +=/home/MPS/jrtp/lib/include/jthread
INCLUDEPATH +=/home/MPS/jrtp/lib/include/jrtplib3

INCLUDEPATH +=/home/MPS/mps/include/mps_include
##INCLUDEPATH +=/home/MPS/mps/include/mp4v2
#INCLUDEPATH +=/home/MPS/mps/include/mp4v2-1.9.1
INCLUDEPATH +=/home/MPS/mps/include/soundtouch
INCLUDEPATH +=/home/MPS/mps/include/live555

INCLUDEPATH +=/home/MPS/libcommon/include
INCLUDEPATH +=/home/MPS/boost_1_39_0
INCLUDEPATH +=/home/MPS/libcommon/src
INCLUDEPATH +=/home/MPS/flv/include



INCLUDEPATH +=/home/MPS/live/BasicUsageEnvironment/include
INCLUDEPATH +=/home/MPS/live/groupsock/include
INCLUDEPATH +=/home/MPS/live/liveMedia/include
INCLUDEPATH +=/home/MPS/live/UsageEnvironment/include

LIBS += -lpthread

LIBS += /home/MPS/libcommon/src/LibXML/libxml.a
LIBS += /home/MPS/libcommon/src/LibTransmit/libtransmit.a
LIBS += /home/MPS/jrtp/lib/lib/libjrtp.a
LIBS += /home/MPS/jrtp/lib/lib/libjthread.a

LIBS += /home/MPS/flv/src/libflv.a


LIBS += /home/MPS/live/liveMedia/libliveMedia.a
LIBS += /home/MPS/live/BasicUsageEnvironment/libBasicUsageEnvironment.a
LIBS += /home/MPS/live/groupsock/libgroupsock.a
LIBS += /home/MPS/live/UsageEnvironment/libUsageEnvironment.a


LIBS += /home/MPS/boost_1_39_0/bin.v2/libs/system/build/gcc-arm/release/link-static/threading-multi/libboost_system-gcc-mt-1_39.a
LIBS += /home/MPS/boost_1_39_0/bin.v2/libs/thread/build/gcc-arm/release/link-static/threading-multi/libboost_thread-gcc-mt-1_39.a
LIBS += /home/MPS/boost_1_39_0/bin.v2/libs/filesystem/build/gcc-arm/release/link-static/threading-multi/libboost_filesystem-gcc-mt-1_39.a
LIBS += /home/MPS/boost_1_39_0/bin.v2/libs/program_options/build/gcc-arm/release/link-static/threading-multi/libboost_program_options-gcc-mt-1_39.a

LIBS += /home/MPS/mp4v2-2.0.0/lib/libmp4v2.a
