#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>   // gethostbyname()
#include <arpa/inet.h>

// Debug the error of system call
#include <errno.h>

#define SERVER_PORT     3005
#define BUFFER_LENGTH    250
#define FALSE              0
#define SERVER_NAME     "localhost"
#define MAX_HOST_NAME_LENGTH 20

#define PIN_MAX_LENGTH 10000

#define DEBUG 1

typedef unsigned char byte;
/* This function takes as arguments a SHA-1 hash and the length of it.
This function returns a 4 digit PIN code that has a SHA-1 hash equal to that provided
This function returns -1 if no 4 digit PIN code is found.
*/
 
int pincrack(char *hash, int hashLength);

