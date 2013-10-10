EXTRA_DIST = openssl.cnf taskrambler.conf.in mime.types

taskrambler.pem:
	openssl req -new -nodes -out taskrambler.csr -config openssl.cnf

taskrambler.crt:
	openssl req -new -x509 -key taskrambler.pem -out taskrambler.crt -config openssl.cnf

all-local: taskrambler.pem taskrambler.crt

clean-local:
	rm -f taskrambler.csr taskrambler.pem taskrambler.crt
