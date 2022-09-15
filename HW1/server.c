#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>    
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include "pincrack.h"

#define SERVER_PORT     3005
#define BUFFER_LENGTH    250
#define FALSE              0

#define SERVER_DEBUG 0

int pincrack_bruteforce(char *hash, int hashLength);

int  main() {
   int    sd=-1, sd2=-1;
   int    rc, length, on=1;
   char   buffer[BUFFER_LENGTH];
   char   pin_buff[3];
   fd_set read_fd;
   struct timeval timeout;
   struct sockaddr_in serveraddr;
   int i;
   int pincode;

   sd = socket(AF_INET, SOCK_STREAM, 0);
   // test error: sd < 0)      

   memset(&serveraddr, 0, sizeof(serveraddr));
   serveraddr.sin_family      = AF_INET;
   serveraddr.sin_port        = htons(SERVER_PORT);
   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

   rc = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
   // test error rc < 0

   rc = listen(sd, 10);
   // test error rc< 0

   printf("Ready for client connect().\n");

   do {

      sd2 = accept(sd, NULL, NULL);
      // test error sd2 < 0

      timeout.tv_sec  = 0;
      timeout.tv_usec = 0;

      FD_ZERO(&read_fd);
      FD_SET(sd2, &read_fd);

      rc = select(1, &read_fd, NULL, NULL, &timeout);
      // test error rc < 0

      length = BUFFER_LENGTH;
 
      rc = recv(sd2, buffer, sizeof(buffer), 0);
      // test error rc < 0 or rc == 0 or   rc < sizeof(buffer
      printf("server received %d bytes\n", rc);
#if SERVER_DEBUG
      for (i  = 0; i < sizeof(buffer); i++)
      {
         printf("%x", buffer[i]);
      }
      printf("\n");
#endif
      pincode = pincrack_bruteforce(buffer, 40);

      printf("PIN: %d\n", pincode);

      pin_buff[0] = (pincode >> 8) & 0xff;
      pin_buff[1] = pincode & 0xff;

      rc = send(sd2, pin_buff, 2, 0);
      // test error rc < 0
      printf("server returned %d bytes\n", rc);

   } while (1);

   if (sd != -1)
      close(sd);
   if (sd2 != -1)
      close(sd2);
}

int pincrack_bruteforce(char *hash, int hashLength) {

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
#if 0
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
