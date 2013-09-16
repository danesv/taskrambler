ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

MSG    = message.c \
	 message/has_keep_alive.c \
	 message/header_size_get.c \
	 message/header_to_string.c \
	 message/get_version.c \
	 message/has_valid_version.c
REQ    = request.c \
	 request/has_valid_method.c
RESP   = response.c \
	 response/304.c \
	 response/404.c \
	 response/403.c \
	 response/500.c \
	 response/login_form.c \
	 response/asset.c \
	 response/randval.c \
	 response/session.c
PARSER = parser.c \
	 parser/parse.c \
	 parser/new_message.c \
	 parser/p_header.c \
	 parser/p_body.c \
	 parser/p_request_vars.c \
	 parser/p_post_vars.c
WRITER = writer.c \
	 writer/write.c
WORKER = worker.c \
	 worker/process.c \
	 worker/answer.c \
	 worker/get_asset.c \
	 worker/add_common_header.c \
	 worker/add_computed_header.c
HEADER = header.c \
	 header/to_string.c

noinst_LIBRARIES = libhttp.a

libhttp_a_SOURCES = $(MSG) $(REQ) $(RESP) $(PARSER) $(WRITER) \
		    $(WORKER) $(HEADER) interface/i_http_intro.c
libhttp_a_CFLAGS  = $(CFLAGS) -Wall -I ../../include/
