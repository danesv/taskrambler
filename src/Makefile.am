ACLOCAL_AMFLAGS = -I m4

IFACE   = interface/stream_reader.c interface/logger.c \
          interface/stream_writer.c interface/http_intro.c \
          interface/subject.c interface/observer.c
SOCKET  = socket.c socket/accept.c socket/connect.c socket/listen.c
STREAM  = stream.c stream/read.c stream/write.c
HASH    = hash.c hash/add.c hash/get.c hash/delete.c \
          hash/each.c interface/hashable.c hash_value.c
SERVER  = server.c server/run.c server/close_conn.c server/poll.c \
          server/handle_accept.c server/read.c server/write.c
LOGGER  = logger.c logger/stderr.c logger/syslog.c
CB      = cbuf.c cbuf/read.c cbuf/write.c \
          cbuf/get_line.c cbuf/set_data.c cbuf/get_data.c \
          cbuf/addr_index.c cbuf/get_free.c cbuf/get_read.c cbuf/get_write.c \
          cbuf/inc_read.c cbuf/inc_write.c cbuf/is_empty.c cbuf/memchr.c \
          cbuf/skip_non_alpha.c cbuf/is_locked.c cbuf/lock.c cbuf/release.c \
          cbuf/empty.c
MSG     = http/message.c \
          http/message/has_keep_alive.c \
          http/message/header_size_get.c \
          http/message/header_to_string.c \
          http/message/get_version.c \
          http/message/has_valid_version.c
MSGQ    = http/message/queue.c
REQ     = http/request.c \
          http/request/has_valid_method.c
RESP    = http/response.c \
          http/response/304.c \
          http/response/404.c \
          http/response/403.c \
          http/response/login_form.c \
          http/response/asset.c \
          http/response/randval.c \
          http/response/session.c
PARSER  = http/parser.c \
          http/parser/parse.c \
          http/parser/new_message.c \
          http/parser/header.c \
          http/parser/body.c \
          http/parser/request_vars.c \
          http/parser/post_vars.c
WRITER  = http/writer.c \
          http/writer/write.c
WORKER  = http/worker.c \
          http/worker/process.c \
          http/worker/write.c \
          http/worker/get_asset.c \
          http/worker/add_common_header.c
HEADER  = http/header.c \
	  http/header/to_string.c
SESSION = session.c session/add.c session/get.c session/delete.c
UTILS   = utils/hash.c \
          utils/memory.c \
          utils/http.c \
          utils/daemonize.c \
          utils/signalHandling.c
AUTH    = interface/auth.c auth/ldap.c credential.c


AM_CFLAGS = -Wall -I ../include/

bin_PROGRAMS = taskrambler

taskrambler_SOURCES = taskrambler.c \
			$(IFACE) $(SOCKET) $(SERVER) $(LOGGER) $(MSG) $(REQ) \
			$(WRITER) $(RESP) $(HEADER) $(PARSER) $(WORKER) $(CB) \
			$(UTILS) $(MSGQ) $(SESSION) $(STREAM) $(HASH) $(AUTH)
taskrambler_CFLAGS  = -Wall -I ../include/
taskrambler_LDFLAGS = -L./class -lclass -lrt -lssl -lldap

SUBDIRS = class
