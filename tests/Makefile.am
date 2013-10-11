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

AM_CFLAGS += -O0 -Wall -ggdb -I ../include -I .. -I . $(MEM_OPT_FLAGS)
AM_LDFLAGS = 

if HAVE_GCOV
AM_CFLAGS += $(COVERAGE_CFLAGS)
AM_LDFLAGS += $(COVERAGE_LDFLAGS)
endif

classTest_SOURCES = $(CLASS) classTest.c
classTest_CFLAGS  = $(AM_CFLAGS) -pg
classTest_LDFLAGS = $(AM_LDFLAGS) -pg

loggerTest_SOURCES = $(LOGGER) loggerTest.c
loggerTest_CFLAGS  = $(AM_CFLAGS) -pg
loggerTest_LDFLAGS = $(AM_LDFLAGS) -pg

socketTest_SOURCES = $(SOCKET) socketTest.c
socketTest_CFLAGS  = $(AM_CFLAGS) -pg
socketTest_LDFLAGS = $(AM_LDFLAGS) -pg

serverTest_SOURCES = $(SERVER) serverTest.c
serverTest_CFLAGS  = $(AM_CFLAGS) -pg
serverTest_LDFLAGS = $(STREAM_OBJ) $(AM_LDFLAGS) -lcrypto -pg

EXTRA_DIST = runtest.h mock suppress

if HAVE_GCOV

.PHONY: clean-gcda clean-gcno coverage-html clean-coverage

cov_dir = $(top_srcdir)/tests/coverage/$(PACKAGE_VERSION)

coverage-html: clean clean-coverage check
	@echo Collecting coverage data
	$(LCOV) -d $(top_srcdir) -c -i -o coverage.base
	$(LCOV) -d $(top_srcdir) -c -o coverage.run --gcov-tool /usr/bin/gcov
	$(LCOV) -d $(top_srcdir) \
		-a ./coverage.base -a ./coverage.run \
		-o coverage.info
	LANG=C $(GENHTML) --prefix $(top_srcdir) \
	     --output-directory $(cov_dir) \
	     --title "$(PACKAGE_NAME) $(PACKAGE_VERSION)" \
	     --legend --branch-coverage --show-details coverage.run

clean-coverage: clean-gcda clean-gcno
	$(LCOV) -d $(top_srcdir) -z
	-rm -rf coverage.info coverage.base coverage.run $(cov_dir)
  
clean-gcda:
	@echo Removing old coverage results
	-find $(top_srcdir) -name '*.gcda' -print | xargs -r rm
  
clean-gcno:
	@echo Removing old coverage results
	-find $(top_srcdir) -name '*.gcno' -print | xargs -r rm
  
clean-local: clean-coverage

endif # HAVE_GCOV
