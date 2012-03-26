ACLOCAL_AMFLAGS = -I m4

MSG    = message.c \
	 message/has_keep_alive.c \
	 message/header_size_get.c \
	 message/header_to_string.c \
	 message/get_version.c \
	 message/has_valid_version.c
MSGQ   = message/queue.c
REQ    = request.c \
	 request/has_valid_method.c
RESP   = response.c \
	 response/304.c \
	 response/404.c \
	 response/403.c \
	 response/login_form.c \
	 response/asset.c \
	 response/randval.c \
	 response/session.c
PARSER = parser.c \
	 parser/parse.c \
	 parser/new_message.c \
	 parser/header.c \
	 parser/body.c \
	 parser/request_vars.c \
	 parser/post_vars.c
WRITER = writer.c \
	 writer/write.c
WORKER = worker.c \
	 worker/process.c \
	 worker/write.c \
	 worker/get_asset.c \
	 worker/add_common_header.c
HEADER = header.c \
	 header/to_string.c

noinst_LIBRARIES = libhttp.a

libhttp_a_SOURCES = $(MSG) $(MSGQ) $(REQ) $(RESP) $(PARSER) $(WRITER) \
		    $(WORKER) $(HEADER)
libhttp_a_CFLAGS  = -Wall -I ../../include/
