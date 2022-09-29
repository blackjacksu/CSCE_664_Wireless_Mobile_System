GCC = gcc
GCC_OPTIONS = -Wall -g3
GCC_LIBS =
SSL_LIBS_OPTION = -lcrypto -lssl

OPENSSL_LIBS_DIR = /usr/local/ssl
OPENSSL_HEADERS_DIR = /usr/local/ssl


all: client server libpincrack.a 

client: client.c
	$(GCC) $(GCC_OPTIONS) client.c -o client $(SSL_LIBS_OPTION)

server: server.c libpincrack.a
	$(GCC) $(GCC_OPTIONS) -L. -o server server.c -lpincrack $(SSL_LIBS_OPTION)

libpincrack.a: pincrack.c pincrack.h
	$(GCC) $(GCC_OPTIONS) -c -o libpincrack.a pincrack.c

clean:
	rm -f *.o *.bin *.a client server
