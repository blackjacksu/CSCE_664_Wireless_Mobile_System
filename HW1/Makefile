GCC = gcc
GCC_OPTIONS = -Wall -g3
GCC_LIBS =
SHA_LIBS_OPTION = -lcrypto -lssl

OPENSSL_LIBS_DIR = /usr/local/ssl
OPENSSL_HEADERS_DIR = /usr/local/ssl


all: client server sha1test libpincrack.a pincracktest

client: client.c
	$(GCC) $(GCC_OPTIONS) client.c -o client

server: server.c
	$(GCC) $(GCC_OPTIONS) server.c -o server

pincracktest: pincracktest.c libpincrack.a
	$(GCC) $(GCC_OPTIONS) -L. -o pincracktest pincracktest.c -lpincrack $(SHA_LIBS_OPTION)

sha1test: sha1test.c
	$(GCC) $(GCC_OPTIONS) -I$(OPENSSL_HEADERS_DIR) -L$(OPENSSL_LIBS_DIR) -o sha1test sha1test.c $(SHA_LIBS_OPTION)

libpincrack.a: pincrack.c
	$(GCC) $(GCC_OPTIONS) -c -o libpincrack.a pincrack.c

clean:
	rm -f *.o *.bin *.a sha1test client server pincracktest
