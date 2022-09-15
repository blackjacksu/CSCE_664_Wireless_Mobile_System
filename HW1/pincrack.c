#include "pincrack.h"


int pincrack(char *hash, int hashLength) {

/* Here you will implement all logic: 
socket creation, communication with the server and returning 
the value to the caller of this function. 
*/

   int    sd=-1, rc, bytesReceived;
   int    pincode = 0;
   int    i;
   char   buffer[BUFFER_LENGTH];
   char   server[MAX_HOST_NAME_LENGTH];
   struct sockaddr_in serveraddr;
   struct hostent *hostp;

   do   {
        sd = socket(AF_INET, SOCK_STREAM, 0);
        //test error sd < 0
        strcpy(server, SERVER_NAME);

        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family      = AF_INET;
        serveraddr.sin_port        = htons(SERVER_PORT);
        serveraddr.sin_addr.s_addr = inet_addr(server);

        if (serveraddr.sin_addr.s_addr == (unsigned long)INADDR_NONE)      {
           hostp = gethostbyname(server);
           if (hostp == (struct hostent *)NULL) {
              printf("Host not found --> ");
              break;
           }

           memcpy(&serveraddr.sin_addr,
                  hostp->h_addr,
                  sizeof(serveraddr.sin_addr));
        }

        rc = connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        // test error rc < 0

        printf("Connect returned %d\n", rc);

        rc = send(sd, hash, hashLength, 0);
        // test error rc < 0

        printf("send returned %d\n", rc);

        bytesReceived = 0;
        while (bytesReceived < BUFFER_LENGTH) {
            rc = recv(sd, & buffer[bytesReceived],
                   BUFFER_LENGTH - bytesReceived, 0);
            // test error rc < 0 or rc == 0
            printf("bytes received %d\n", rc);
                bytesReceived += rc;

            pincode = (buffer[0] << 8) | (buffer[1] & 0xff);
            break;
        }

        printf("PIN: %d\n", pincode);


    } while (FALSE);

    //    shutdown(sd, SHUT_RDWR);

    if (sd != -1)
        close(sd);

    return pincode;
}
