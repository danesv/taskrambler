#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtest.h"
#include "class.h"
#include "socket.h"
#include "logger.h"
#include "mock/mock_logger.h"


#define TEST_PORT	11222


const char testname[] = "socketTest";

MockLogger logger = NULL;
Sock       sock   = NULL;

static
int
__setUp()
{
	logger = new(MockLogger, LOGGER_DEBUG);

	sock = new(Sock, logger, TEST_PORT);

	ASSERT_INSTANCE_OF(Sock, sock);
	ASSERT_INSTANCE_OF(MockLogger, sock->log);
	ASSERT_EQUAL(TEST_PORT, sock->port);
	ASSERT_NOT_EQUAL(0, sock->handle);

	return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    if (NULL != logger) {
        ASSERT_OBJECT(logger);
        delete(logger);
    }

    if (NULL != sock) {
        ASSERT_OBJECT(sock);
        delete(sock);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testListen()
{
	socketListen(sock, 10);

	return TEST_OK;
}

static
int
testAcceptConnect()
{
	Sock acc, con;
	char addr[16];
	pid_t pid;

	socketListen(sock, 10);

	pid = fork();

	switch(pid) {
		case 0:
			con = new(Sock, logger, TEST_PORT);
			sleep(1);
			socketConnect(con, "127.0.0.1", &addr);
			delete(con);
			__tearDown();
			exit(EXIT_SUCCESS);

		case -1:
			return TEST_FAILED;

		default:
			acc = socketAccept(sock, &addr);
	}

    if (NULL != acc) {
        ASSERT_OBJECT(acc);
        delete(acc);
    }

	return TEST_OK;
}

const testfunc tests[] = {
    testListen,
	testAcceptConnect
};
const size_t count = FUNCS_COUNT(tests);

// vim: set ts=4 sw=4:
