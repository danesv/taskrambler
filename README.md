Taskrambler
===========

When this is finished it will become a distributed task and time
management system with a web frontend.
It will come with its own HTTP server implementation and thus wont
need any third party webserver to work.
The backend is written completely in C while the frontend will be
JavaScript, CSS and HTML.

Users will be able to connect to the server
via their browsers, manage their tasks there and specify policies
who else might see their tasks, track the time spend on a specific
task, attach tasks to projects, getting reports about their work done,
etc.

For the taskmanagement stuff I think of a similar approach
as [taskwarrior](http://taskwarrior.org/projects/show/taskwarrior) is going.
This is an amazing tool and I really enjoy using it.
I started this as an experiment to implement an HTTP server, then I learned
about taskwarrior and thought, wouldn't it be great to have this
functionality in a multiuser environment.

Right now, this is a single process HTTP server implementation that performs
not to bad. And some application logic for user management. Every connection
is associated with a session and user can register, login and modify their
user data.

In the nearer future an role based access control system will be added.
Users will be able to add tasts, (for now just a text and maybe a date or
something) and they should be able to control who else might see a task.

If this works reliable I will announce version 0.2.

The main development page can be found 
[here](http://redmine.weird-web-workers.org/projects/taskrambler).

INSTALLATION
------------

This can be installed via the usual configure, make, make install
cycle. For gentoo users am ebuild is added under docs.

### API DOC

To generate the api doc a patched version of doxygen is
neccessary. A patch is included under docs.

*make docs* creates the api doc.

### TEST COVERAGE REPORT

gcov and lcov are needed to build these.

The source has to be configured with *configure --enable-gcov*.
*make coverage-html* creates the converage reports then.


USAGE
-----

### RUNNING

Simply start the installed executable.
This will usually be found under */usr/local/bin/taskrambler* after
successfull build and installation.

When installed with the ebuild the executable will be */usr/bin/taskrambler*.

After the executable is stared you should be able to see the staskrambler
user interface with your browser by openening *http://localhost:11212/*.

In the upper right corner you can see session and user related information.

### CONFIGURATION

A first configuration file is installed in your etc folder under
*taskrambler/taskrambler.conf*. The syntax of that file is described in it.

Currently there are only 5 settings that can be configured.

* ldap_base: The base for the ldap Distinguished Name (DN) for user lookups.
* ldap_host: The ldap server to connect to.
* assets_dir: Defines where taskrambler will find its assets.
* runtime_dir: The place where the user and credential database files are
  stored
* port: The port taskrambler should run on.

### PORT

Taskrambler will always connect to the port specified in the config for
HTTP connections.

A second socket will be opened on port+1 for SSL connections.

### LOGIN

Every user that can be authenticated either via LDAP or via taskramblers
own database can login via the login link in the menu.

### SIGNUP

It is possible to create a user account by clicking the signup link.
After a successfull signup you will be automatically logged in.

### CHANGE USER DATA ###

After a successfull login you have the ability to see your current account
informations and modify them via "my account"

TESTING
-------

This comes with the start of a unit test suite.
You can use *make test* to build and run the existent tests.

### REQUIREMENTS

Currently, you need valgrind to build the tests, because some memory checking is done by the way.

CONTRIBUTION
------------

I would really like to see some people possibly interested in this stuff.
I think it contains some really interesting ideas.

If you like to contribute anyway, make a fork, do your changes and generate
a pull request. Or simply contact me on georg@steffers.org.
