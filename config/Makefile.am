EXTRA_DIST = openssl.cnf taskrambler.conf.in mime.types

.PHONY: update-config

edit = sed \
       -e 's,@trdatadir\@,$(trdatadir),g' \
       -e 's,@trstatedir\@,$(trstatedir),g'

taskrambler.conf: taskrambler.conf.in
	$(edit) taskrambler.conf.in >taskrambler.conf

taskrambler.pem:
	openssl req -new -nodes -out taskrambler.csr -config openssl.cnf

taskrambler.crt:
	openssl req -new -x509 -key taskrambler.pem -out taskrambler.crt -config openssl.cnf

all-local: taskrambler.pem taskrambler.crt taskrambler.conf

clean-local:
	rm -f taskrambler.csr taskrambler.pem taskrambler.crt taskrambler.conf
