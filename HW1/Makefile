GCC = gcc
GCC_OPTIONS = -Wall -g3
GCC_LIBS =
SHA_LIBS_OPTION = -lcrypto -lssl

OPENSSL_LIBS_DIR = /usr/local/ssl
OPENSSL_HEADERS_DIR = /usr/local/ssl


all: client server sha1test libpincrack.a pincracktest

pincracktest_dll: pincracktest.c pincrack.c pincrack.h
	$(GCC) $(GCC_OPTIONS)  -o pincracktest_dll -L. pincracktest.c -lpincrack

client: client.c
	$(GCC) $(GCC_OPTIONS) client.c -o client

server: server.c libpincrack.a
	$(GCC) $(GCC_OPTIONS) -L. -o server server.c -lpincrack $(SHA_LIBS_OPTION)

pincracktest: pincracktest.c pincrack.c pincrack.h
	$(GCC) $(GCC_OPTIONS)  -o pincracktest -L. pincracktest.c -lpincrack

sha1test: sha1test.c
	$(GCC) $(GCC_OPTIONS) -I$(OPENSSL_HEADERS_DIR) -L$(OPENSSL_LIBS_DIR) -o sha1test sha1test.c $(SHA_LIBS_OPTION)

libpincrack.a: pincrack.c pincrack.h
	$(GCC) $(GCC_OPTIONS) -c -o libpincrack.a pincrack.c

libpincrack.so: pincrack.c pincrack.h
	$(GCC) $(GCC_OPTIONS) -shared -c -o libpincrack.so pincrack.c

clean:
	rm -f *.o *.bin *.a sha1test client server pincracktest
