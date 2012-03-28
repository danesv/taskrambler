ACLOCAL_AMFLAGS = -I m4

TESTS_ENVIRONMENT = valgrind --error-exitcode=123 --leak-check=full --quiet
TESTS = classTest loggerTest socketTest serverTest
check_PROGRAMS = classTest loggerTest socketTest serverTest

COMMON = runtest.c
CLASS  = $(COMMON) \
	 ../src/class/interface.c \
	 ../src/class/interface/i_class.c \
	 mock/mock_class.c

LOGGER = $(CLASS) \
	 ../src/logger/logger.c \
	 ../src/logger/stderr.c \
	 ../src/logger/syslog.c \
	 ../src/logger/interface/i_logger.c \
	 mock/mock_logger.c

SOCKET = $(LOGGER) \
	 ../src/socket/socket.c \
	 ../src/socket/listen.c \
	 ../src/socket/accept.c \
	 ../src/socket/connect.c

STREAM_OBJ = ./stream/stream.o \
	     ./stream/read.o \
	     ./stream/write.o \
	     ./stream/reader.o \
	     ./stream/writer.o

SERVER = $(SOCKET) \
	 ../src/server/server.c \
	 ../src/server/run.c \
	 ../src/server/close_conn.c \
	 ../src/server/handle_accept.c \
	 ../src/server/poll.c \
	 ../src/server/read.c \
	 ../src/server/write.c \
	 ../src/utils/signalHandling.c \
	 ../src/utils/memory.c \
	 mock/mock_worker.c

classTest_SOURCES = $(CLASS) classTest.c
classTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -I ../include -I .. -I .
classTest_LDFLAGS = -lgcov

loggerTest_SOURCES = $(LOGGER) loggerTest.c
loggerTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -I ../include -I .. -I .
loggerTest_LDFLAGS = -lgcov

socketTest_SOURCES = $(SOCKET) socketTest.c
socketTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -I ../include -I .. -I .
socketTest_LDFLAGS = -lgcov

serverTest_SOURCES = $(SERVER) serverTest.c
serverTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -I ../include -I .. -I .
serverTest_LDFLAGS = $(STREAM_OBJ) -lgcov

EXTRA_DIST = runtest.h mock/mock_class.h mock/mock_logger.h

.PHONY: stream
stream:
	$(MAKE) -C stream $(AM_MAKEFLAGS)

$(check_PROGRAMS): stream
check-build: $(check_PROGRAMS)

all-local: stream

SUBDIRS = stream
