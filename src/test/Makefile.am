SUBDIRS = .

noinst_PROGRAMS = test
test_SOURCES = \
 MControl.cpp \
 test.cpp

INCLUDES = \
 -I${top_srcdir}/include \
 -I${top_srcdir}/src \
 -I${top_srcdir}/src/test \
 -I/home/MPS/boost_1_39_0

test_LDADD = \
 -L/usr/local/lib -lpthread \
 -lboost_regex-gcc43-mt \
 -lboost_system-gcc43-mt \
 -lboost_date_time-gcc43-mt \
 -lboost_filesystem-gcc43-mt \
 -lboost_program_options-gcc43-mt \
 -lboost_thread-gcc43-mt \
 -L$(top_srcdir)/src/LibXML -lxml \
 -L$(top_srcdir)/src/LibTransmit -ltransmit

MAINTAINERCLEANFILES = Makefile.in
