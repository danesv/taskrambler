/**
 * \file
 * Disconnect from parent process and let all standard file handles
 * point to /dev/null.
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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

#include <stdio.h>      // for printf() and fprintf()
#include <unistd.h>     // for getopt
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>


#define WORKDIR		"/"
#define UMASK		0

void daemonize(void) {
    pid_t pid;

    if (0 > ((pid = fork()))) {
        perror("deamoinze[fork]");
        exit(EXIT_FAILURE);
    } else if (0 != pid) {
        exit(EXIT_SUCCESS);
    }

    // make new child session leader
    setsid();

    if (0 > ((pid = fork()))) {
        perror("deamoinze[fork]");
        exit(EXIT_FAILURE);
    } else if (0 != pid) {
        exit(EXIT_SUCCESS);
    }

    // set umask and change to working directory to /
    umask(UMASK);
    if (-1 == chdir("/")) {
		perror("daemonize");
		exit(EXIT_FAILURE);
    }

    // we should close all open filedescriptors now.
    // But I assume that this function is called at the very start of the
    // program and no more filedescriptors are open than the standard
    // ones.

    // connect all standard streams to /dev/null
    stderr = freopen("/dev/null", "w", stderr);
    stdin  = freopen("/dev/null", "r", stdin);
    stdout = freopen("/dev/null", "w", stdout);
}

// vim: set ts=4 sw=4:
