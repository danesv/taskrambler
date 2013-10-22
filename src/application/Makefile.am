ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

APPLICATION = application.c \
	      login.c \
	      logout.c \
	      signup.c \
	      session_start.c \
	      session_stop.c \
	      session_get.c \
	      session_cleanup.c
ADAPTERHTTP = adapter/http/http.c \
	      adapter/http/update.c
CONTROLLER = controller/authenticate/create.c \
	     controller/authenticate/delete.c \
 	     controller/currentuser/read.c \
	     controller/randval/read.c \
	     controller/sessinfo/read.c \
	     controller/user/create.c \
	     controller/version/read.c
	      
AM_CFLAGS += -I../../include/

noinst_LTLIBRARIES = libapplication.la

libapplication_la_SOURCES = $(APPLICATION) $(ADAPTERHTTP) $(CONTROLLER)
libapplication_la_CFLAGS  = $(AM_CFLAGS)
