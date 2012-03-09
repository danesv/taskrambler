#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtest.h"
#include "logger.h"
#include "cclass.h"
#include "socket.h"


#define TEST_PORT	11212


int    level  = -1;
char * msg    = NULL;

static void
logfnct_mock(int _level, const char * _msg)
{
    level = _level;
    msg   = malloc(strlen(_msg) + 1);
    strcpy(msg, _msg);
}

const char testname[] = "socketTest";
LOGGER logger = NULL;
SOCK   sock   = NULL;

static
int
__setUp()
{
	logger = new(LOGGER, NULL);
	logger_add(logger, logfnct_mock);

	sock = new(SOCK, logger, TEST_PORT);

	ASSERT_INSTANCE_OF(SOCK, sock);
	ASSERT_INSTANCE_OF(LOGGER, sock->logger);
	ASSERT_EQUAL(TEST_PORT, sock->port);
	ASSERT_NOT_EQUAL(0, sock->handle);

	return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    level = -1;

    if (NULL != msg) {
        free(msg);
        msg = NULL;
    }

    if (NULL != logger) {
        ASSERT_OBJECT(logger);
        delete(&logger);
    }

    if (NULL != sock) {
        ASSERT_OBJECT(sock);
        delete(&sock);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testListen()
{
	sock_listen(sock, 10);

	return TEST_OK;
}

static
int
testAccept()
{
	SOCK acc, con;
	char addr[16];
	pid_t pid;

	sock_listen(sock, 10);

	pid = fork();

	switch(pid) {
		case 0:
			con = new(SOCK, logger, TEST_PORT);
			sleep(1);
			sock_connect(con, "127.0.0.1");
			delete(&con);
			__tearDown();
			exit(EXIT_SUCCESS);

		case -1:
			return TEST_FAILED;

		default:
			acc = sock_accept(sock, addr);
	}

    if (NULL != acc) {
        ASSERT_OBJECT(acc);
        delete(&acc);
    }

	return TEST_OK;
}

const testfunc tests[] = {
    testListen,
	testAccept
};
const size_t count = FUNCS_COUNT(tests);

// vim: set ts=4 sw=4:
