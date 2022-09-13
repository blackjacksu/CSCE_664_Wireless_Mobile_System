#include "pincrack.h"


int pincrack(char *hash, int hashLength) {

/* Here you will implement all logic: 
socket creation, communication with the server and returning 
the value to the caller of this function. 
*/

    SHA_CTX shactx;
    byte digest[SHA_DIGEST_LENGTH];
    char *pin;
    int i; // Used for iterated index for printf char array
    int j; // Used to enumerate the PIN code
    int ret = -1; // Return fail code (-1) 

    pin = (char *) malloc(sizeof(char) * 4);
    memset(pin, 0, sizeof(char) * 4);

    printf("Hash: %s, Len: %d\n", hash, hashLength);

    for (j = 0; j < HASH_MAX_LENGTH; j++)
    {
        memset(digest, 0, SHA_DIGEST_LENGTH);
        sprintf(pin, "%d", j);
        SHA1_Init(&shactx);
        SHA1_Update(&shactx, pin, 4);
        SHA1_Final(digest, &shactx);
        printf("digest: %s, pin: %s, j: %d\n", digest, pin, j);

        if (strcmp(digest, hash) == 0)
        {
            // Found the PIN code for the hash
            ret = j;
            printf("Digest: ");
            for (i = 0; i < SHA_DIGEST_LENGTH; i++)
	            printf("%02x", digest[i]);
            putchar('\n');
            printf("Hash: %s, Length: %d\n", hash, hashLength);
            break;
        }
    }

    return ret;
}
