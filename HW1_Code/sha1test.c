#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

typedef unsigned char byte;

int main(int argc, char *argv[]) {
    SHA_CTX shactx;
    byte digest[SHA_DIGEST_LENGTH];
    int i;

    if(argc != 2) {
        printf("Usage: sha1test <4 character string>\n");
        exit(-1);
    }

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, argv[1], 4);
    SHA1_Final(digest, &shactx);

    for (i=0; i<SHA_DIGEST_LENGTH; i++)
	printf("%02x", digest[i]);
    putchar('\n');

    return 0;
}


