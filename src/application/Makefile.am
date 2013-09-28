ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

APPLICATION = application.c \
	      login.c \
	      signup.c \
	      session_start.c \
	      session_stop.c \
	      session_update.c \
	      session_get.c
ADAPTERHTTP = adapter/http/http.c \
	      adapter/http/update.c
	      
noinst_LIBRARIES = libapplication.a

libapplication_a_SOURCES = $(APPLICATION) $(ADAPTERHTTP)
libapplication_a_CFLAGS  = $(CFLAGS) -Wall -I ../../ -I ../../include/
