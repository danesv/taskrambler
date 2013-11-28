/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2012  Georg Hopp
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

#include "server.h"
#include "logger.h"
#include "http/worker.h"
#include "auth.h"
#include "application/application.h"
#include "application/adapter/http.h"
#include "interface/subject.h"
#include "config/config.h"
#include "config/value.h"

#include "class.h"
#include "logger.h"

#include "utils/signalHandling.h"
#include "utils/memory.h"
#include "utils/mime_type.h"

#define DEFAULT_SECS	10
//#define DEFAULT_USECS	(1000000 / HZ * 2)
//#define DEFAULT_SECS	1
#define DEFAULT_USECS	0


void nullhandler() {}

void daemonize(void);

Logger logger;
Config config;

int
main()
{
	long             psize  = sysconf(_SC_PAGESIZE);
	struct rlimit    limit = {RLIM_INFINITY, RLIM_INFINITY};

	pid_t            pid;
	int              status;
	int              shm;
	struct randval * value;

	logger = new(LoggerSyslog, LOGGER_DEBUG);
	config = new(Config, CONFIGDIR "/taskrambler.conf");

	if (NULL == config) {
		loggerLog(logger, LOGGER_INFO,
				"unable to load configuration file: %s\n",
				CONFIGDIR "/taskrambler.conf");

		if (! INSTANCE_OF(LoggerStderr, logger)) {
			fprintf(stderr,
					"unable to load configuration file: %s\n",
					CONFIGDIR "/taskrambler.conf");
		}

		return 1;
	}

	setrlimit(RLIMIT_CPU, &limit);

	getrlimit(RLIMIT_NOFILE, &limit);
	limit.rlim_cur = limit.rlim_max;
	setrlimit(RLIMIT_NOFILE, &limit);

	init_signals();
	//daemonize();

	shm = shm_open("/fooshm", O_RDWR|O_CREAT, S_IRWXU);
	if (-1 == ftruncate(shm, psize)) {
		doShutdown = 1;
	}

	switch((pid = fork())) {
		case -1:
			break;

		case 0:
			{
				sigset_t block_these, pause_mask;
				struct sigaction s;
				struct itimerval interval;

				value = mmap (0, sizeof(struct randval), PROT_READ|PROT_WRITE,
						MAP_SHARED, shm, 0);
				value->timestamp = 0;
				value->value     = 0;

				close(shm);

				// Block SIGALRM
				sigemptyset(&block_these);
				sigaddset(&block_these, SIGALRM);
				sigprocmask(SIG_BLOCK, &block_these, &pause_mask);

				// Set up handler for SIGALRM
				sigemptyset(&s.sa_mask);
				sigaddset(&s.sa_mask, SIGINT);
				s.sa_flags = 0;
				s.sa_handler = nullhandler;
				if (sigaction(SIGALRM, &s, NULL) < 0) {
					perror("sigaction SIGALRM");
					exit (1);
				}

				interval.it_value.tv_sec     = DEFAULT_SECS;
				interval.it_value.tv_usec    = DEFAULT_USECS;
				interval.it_interval.tv_sec  = DEFAULT_SECS;
				interval.it_interval.tv_usec = DEFAULT_USECS;

				setitimer(ITIMER_REAL, &interval, NULL);

				// child
				while(!doShutdown) {
					value->timestamp = time(NULL);
					value->value     = rand() % 100;
					sigsuspend(&pause_mask);
				}
			}
			break;

		default:
			{
				Storage                users;
				Storage                passwords;
				Auth                   auth;
				Application            application;
				Router                 router;
				ApplicationAdapterHttp adapterHttp;
				HttpWorker             worker;
				Server                 server;

				ConfigValue	ldap_base   =
					configGet(config, CSTRA("ldap_base"));
				ConfigValue	ldap_host   =
					configGet(config, CSTRA("ldap_host"));
				ConfigValue	runtime_dir =
					configGet(config, CSTRA("runtime_dir"));
				ConfigValue	port        =
					configGet(config, CSTRA("port"));

				char user_storage[512];
				char password_storage[512];

				strcpy(user_storage, (runtime_dir->value).string);
				strcpy(password_storage, (runtime_dir->value).string);
				strcat(user_storage, "/users.db");
				strcat(password_storage, "/passwords.db");

				value = mmap (0, sizeof(int), PROT_READ|PROT_WRITE,
						MAP_SHARED, shm, 0);

				shm_unlink("/fooshm");
				close(shm);

				auth = new(Auth);
				authCreate(
						auth,
						AUTH_LDAP,
						(ldap_host->value).string,
						CONFSTRA(ldap_base));

				users       = new(Storage, user_storage);
				passwords   = new(Storage, password_storage);

				if (NULL == users || NULL == passwords) {
					puts("error opening database files...\n");
					doShutdown = 1;
				}

				authCreate(auth, AUTH_STORAGE, passwords);

				application = new(
						Application,
						value,
						users,
						passwords,
						"14de9e60-d497-4754-be72-f3bed52541fc",
						auth);

				router      = new(Router, application);
				adapterHttp = new(ApplicationAdapterHttp, application, router);

				worker = new(HttpWorker, "taskrambler");
				subjectAttach(worker, adapterHttp);

				server = new(
						Server,
						logger,
						worker,
						(int)(port->value).number,
						SOMAXCONN);

				if (NULL != server && !doShutdown) {
					serverRun(server);
				}
				else {
					kill(pid, SIGINT);
				}

				do {
					pid_t w;

					w = waitpid(pid, &status, 0);

					while (w == -1) {
						switch(errno) {
							case EINTR: w = waitpid(pid, &status, 0);
										break;
							case ECHILD: perror("no child");
										 // DROP THROUGH
							default: w = 0;
						}
					}

					if (0 < w) {
						if (WIFEXITED(status)) {
							loggerLog(logger, LOGGER_INFO,
									"child exited, status=%d\n",
									WEXITSTATUS(status));
						} else if (WIFSIGNALED(status)) {
							loggerLog(logger, LOGGER_INFO,
									"killed by signal %d\n",
									WTERMSIG(status));
						} else if (WIFSTOPPED(status)) {
							loggerLog(logger, LOGGER_INFO,
									"stopped by signal %d\n",
									WSTOPSIG(status));
						} else if (WIFCONTINUED(status)) {
							loggerLog(logger, LOGGER_INFO, "continued\n");
						}
					}
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));

				delete(server);
				delete(worker);
				delete(adapterHttp);
				delete(router);
				delete(application);
				delete(passwords);
				delete(users);
				delete(auth);

				clearMimeTypes();
				assetPoolCleanup();
			}

			break;
	}

	delete(config);
	delete(logger);
	memCleanup();

	return 0;
}

// vim: set ts=4 sw=4:
