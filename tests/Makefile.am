ACLOCAL_AMFLAGS = -I m4

#TESTS_ENVIRONMENT = valgrind --error-exitcode=123 --leak-check=full --quiet
#TESTS = cclassTest loggerTest socketTest serverTest
#check_PROGRAMS = cclassTest loggerTest socketTest serverTest
#
#COMMON = runtest.c ../src/interface/class.c
#CCLASS = $(COMMON) mock/class.c
#LOGGER = $(COMMON) ../src/logger.c ../src/interface/logger.c \
#	 ../src/logger/stderr.c ../src/logger/syslog.c
#SOCKET = $(LOGGER) ../src/socket.c ../src/socket/listen.c \
#	 ../src/socket/accept.c ../src/socket/connect.c
#SERVER = $(SOCKET) ../src/server.c ../src/server/run.c \
#	 ../src/server/close_conn.c ../src/utils/signalHandling.c
#
#cclassTest_SOURCES = $(CCLASS) cclassTest.c
#cclassTest_CFLAGS  = -Wall -ggdb -O0 -finline-functions -I ../include -I .. -I .
#
#loggerTest_SOURCES = $(LOGGER) loggerTest.c
#loggerTest_CFLAGS  = -Wall -ggdb -O0 -I ../include -I .. -I .
#
#socketTest_SOURCES = $(SOCKET) socketTest.c
#socketTest_CFLAGS  = -Wall -ggdb -O0 -I ../include -I .. -I .
#
#serverTest_SOURCES = $(SERVER) serverTest.c
#serverTest_CFLAGS  = -Wall -ggdb -O0 -I ../include -I .. -I .

EXTRA_DIST = runtest.h mock/class.h
