/**
 * \file
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

#include <signal.h>     // for signal() and signal names

volatile int doShutdown;

void terminate(int signum)
{
    signal(signum, SIG_IGN);
    /*
     * \todo do logging here
     */
    doShutdown = 1;
}

void init_signals(void)
{
    signal(SIGTERM, terminate);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, terminate);
    signal(SIGQUIT, terminate);
    signal(SIGABRT, terminate);
    signal(SIGALRM, SIG_IGN);
    signal(SIGURG, SIG_IGN);

    signal(SIGPIPE, SIG_IGN);
}
