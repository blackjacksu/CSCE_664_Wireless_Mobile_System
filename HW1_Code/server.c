#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>    
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT     3005
#define BUFFER_LENGTH    250
#define FALSE              0

int  main() {
   int    sd=-1, sd2=-1;
   int    rc, length, on=1;
   char   buffer[BUFFER_LENGTH];
   fd_set read_fd;
   struct timeval timeout;
   struct sockaddr_in serveraddr;

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

      rc = send(sd2, buffer, sizeof(buffer), 0);
      // test error rc < 0
      printf("server returned %d bytes\n", rc);

   } while (1);

   if (sd != -1)
      close(sd);
   if (sd2 != -1)
      close(sd2);
}


