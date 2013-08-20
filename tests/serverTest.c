#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <socket.h>
#include <sys/types.h>

#include "runtest.h"
#include "logger.h"
#include "class.h"
#include "server.h"
#include "utils/signalHandling.h"
#include "mock/mock_logger.h"
#include "mock/mock_worker.h"


#define TEST_PORT	11222
#define TEST_DATA	"test"


char buffer[1025];

const char testname[] = "serverTest";

MockLogger logger = NULL;
MockWorker worker = NULL;
Server     server = NULL;

static
int
__setUp()
{
	logger = new(MockLogger, LOGGER_DEBUG);
	worker = new(MockWorker);

	server = new(Server, logger, worker, TEST_PORT, SOMAXCONN);

	ASSERT_INSTANCE_OF(Server, server);
	ASSERT_INSTANCE_OF(MockLogger, server->logger);
	ASSERT_INSTANCE_OF(MockWorker, server->worker);
	ASSERT_INSTANCE_OF(Sock, server->sock);
	ASSERT_EQUAL(TEST_PORT, server->sock->port);

	return TEST_OK;
}
int (* const setUp)() = __setUp;

static
int
__tearDown()
{
    if (NULL != server) {
        ASSERT_OBJECT(server);
        delete(server);
    }

    if (NULL != worker) {
        ASSERT_OBJECT(worker);
        delete(worker);
    }

    if (NULL != logger) {
        ASSERT_OBJECT(logger);
        delete(logger);
    }

    return TEST_OK;
}
int (* const tearDown)() = __tearDown;

static
int
testDummy()
{
	Sock con;
	pid_t pid, ppid;
	char addr[16];

	ppid = getpid();
	pid  = fork();

	switch(pid) {
		case 0:
			con = new(Sock, logger, TEST_PORT);
			sleep(1);
			socketConnect(con, "127.0.0.1", &addr);
			write(con->handle, TEST_DATA, strlen(TEST_DATA)+1);
			sleep(1);
			delete(con);
			__tearDown();
			kill(ppid, SIGINT);
			exit(EXIT_SUCCESS);

		case -1:
			return TEST_FAILED;

		default:
			init_signals();
			serverRun(server);
	}

	ASSERT_STRING_EQUAL(TEST_DATA, worker->rbuf);

	return TEST_OK;
}

const testfunc tests[] = {
    testDummy
};
const size_t count = FUNCS_COUNT(tests);

// vim: set ts=4 sw=4:
