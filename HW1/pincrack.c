#include "pincrack.h"


int pincrack(char *hash, int hashLength) {

/* Here you will implement all logic: 
socket creation, communication with the server and returning 
the value to the caller of this function. 
*/

    SHA_CTX shactx;
    byte digest[SHA_DIGEST_LENGTH];
    char digest_buf[3]; // including the nul using sprintf
    char *pin;
    int i; // Used for iterated index for printf char array
    int j; // Used to enumerate the PIN code
    int ret = -1; // Return fail code (-1) 
    bool is_matched = true;

    if (hashLength != SHA_DIGEST_LENGTH * 2)
    {
        // No result for this
        return ret;
    }

    pin = (char *) malloc(sizeof(char) * 4);
    memset(pin, 0, sizeof(char) * 4);
#if DEBUG
    printf("Hash: %s, Len: %d, SHA_DIGEST_LENGTH: %d\n", hash, hashLength, SHA_DIGEST_LENGTH);
#endif
    for (j = 0; j < PIN_MAX_LENGTH; j++)
    {
        is_matched = true;
        memset(digest, 0, sizeof(digest));
        sprintf(pin, "%04d", j);
        SHA1_Init(&shactx);
        SHA1_Update(&shactx, pin, 4);
        SHA1_Final(digest, &shactx);
#if DEBUG
        printf("digest: %s, pin: %s, j: %d\n", digest, pin, j);
        for (i = 0; i < SHA_DIGEST_LENGTH; i++)
	        printf("%02x", digest[i]);
        putchar('\n');
#endif
        // check every byte if the digest and hash matched
        // The hash, in ASCII format, has to be converted to hex format before compare with digest
        for (i = 0; i < SHA_DIGEST_LENGTH; i++)
        {
            memset(digest_buf, 0, sizeof(char) * 2);
            sprintf(digest_buf, "%02x", digest[i]);
            if (digest_buf[0] != hash[i * 2] || digest_buf[1] != hash[i * 2 + 1])
            {
                is_matched = false;
                break;
            }
        }

        if (is_matched)
        {
            // Found the PIN code for the hash
            ret = j;
#if DEBUG
            printf("Digest: ");
            for (i = 0; i < SHA_DIGEST_LENGTH; i++)
	            printf("%02x", digest[i]);
            putchar('\n');
            printf("Hash: %s, Length: %d\n", hash, hashLength);
#endif
            break;
        }
    }

    return ret;
}
