ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

TESTS_ENVIRONMENT = valgrind \
		    --error-exitcode=123 \
		    --leak-check=full \
		    --suppressions=./suppress/external.supp \
		    --quiet
TESTS = classTest loggerTest socketTest serverTest
check_PROGRAMS = classTest loggerTest socketTest serverTest

COMMON = runtest.c
CLASS  = $(COMMON) \
	 ../src/class/interface.c \
	 ../src/class/interface/i_class.c \
	 ../src/utils/memory.c \
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
	 ../src/socket/connect.c \
	 ../src/socket/nonblock.c

STREAM = ../src/stream/stream.c \
	 ../src/stream/read.c \
	 ../src/stream/write.c \
	 ../src/stream/interface/reader.c \
	 ../src/stream/interface/writer.c

SERVER = $(SOCKET) $(STREAM) \
	 ../src/server/server.c \
	 ../src/server/run.c \
	 ../src/server/close_conn.c \
	 ../src/server/handle_accept.c \
	 ../src/server/poll.c \
	 ../src/server/read.c \
	 ../src/server/write.c \
	 ../src/utils/signalHandling.c \
	 mock/mock_worker.c

classTest_SOURCES = $(CLASS) classTest.c
classTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -pg -I ../include -I .. -I .
classTest_LDFLAGS = -lgcov -pg

loggerTest_SOURCES = $(LOGGER) loggerTest.c
loggerTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -pg -I ../include -I .. -I .
loggerTest_LDFLAGS = -lgcov -pg

socketTest_SOURCES = $(SOCKET) socketTest.c
socketTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -pg -I ../include -I .. -I .
socketTest_LDFLAGS = -lgcov -pg

serverTest_SOURCES = $(SERVER) serverTest.c
serverTest_CFLAGS  = -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -pg -I ../include -I .. -I .
serverTest_LDFLAGS = $(STREAM_OBJ) -lgcov -lcrypto -pg

EXTRA_DIST = runtest.h mock/mock_class.h mock/mock_logger.h
