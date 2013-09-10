libHttpServer
=============

This is a partly finished implementation of HTTP/1.1.
It is especially intendet to write C only REST server,
well, at least thats the idea.

INSTALLATION
------------

Currently there is no finished installation process.
Further more, packages are required for building this thing,
that in fact are only requered for the testing part.

### BUILD REQUIREMENTS

You need the following to build this thing.

- A GNU Autobuild (autoconf, automake, etc.) installation.
- GCC as well as binutils.

### BUILDING

Call ./bootstrap and then run ./configure.
After that run make. After that you will hopefully find a binary called
*taskrambler* under *src/*.
This is a test application for the library. It's name comes from the
first intention what i would like to do with it. Some kind of task management
system.

### INSTALLING

No care has been taken that this thing installs correctly. Anyway
simply call *./src/taskrambler* will run this. After started this you
should be able to connect with any browser on localhost port 11212.

### PLAY AROUND

Any asset under assets could be exchanged. It should be possible to
change the main.html there and put additionally html files and images
under assets and everything should be deliverd.

Anyway, there are a few rules.

* html assets will always be loaded from assets/html.
* all other assets are loaded directly from the assets directory.

So, right now this can be used as a HTTP server for static content
and without virtual hosts support.


USAGE
-----

### RUNNING

Simply start the executable *src/taskrambler* after successfull build.
Currently this will stay in the foreground and does logging via syslog.

### PORT

The server will listen on port 11212 for HTTP requests and on port 11213
for SSL requerst.

Currently SSL request are broken and disabled completely.

You can use telnet or simply start a browser and connect on localhost:11212.

This should show up a very simplistic page.
It should eneable you to start a session, get values from the server
or login.

### LOGIN

You can't use the login without changing the code. It's implemented using
ldap and currently not configurable. You have to change the configuration
in the code and compile again.

TESTING
-------

This comes with an incomplete unit test suite.
You can use *make test* to build and run the existent tests.

### REQUIREMENTS

Currently, you need valgrind to build the tests, because some memory checking is done by the way.

CONTRIBUTION
------------

I would really like to see some people possibly interested in this stuff.
I think it contains some really interesting ideas.

Well, sadly i have to say that this is a spare time project. Documentation
is sparse and Tests too.

If you like to contribute anyway, make a fork, do your changes and generate
a pull request.

I will look at these as soon as possible.
