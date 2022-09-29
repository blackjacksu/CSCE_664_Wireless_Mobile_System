#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "pincrack.h"
#include <stdbool.h>
#define FAIL    -1

#define DEBUG   1
// Create the SSL socket and intialize the socket address structure
int OpenListener(int port)
{
    int sd;
    struct sockaddr_in addr;
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
int isRoot()
{
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
SSL_CTX* InitServerCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLS_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{
    char buf[1024] = {0};
    int sd, bytes;
    const char* ServerResponse="<\\Body>\
                               <Name>aticleworld.com</Name>\
                 <year>1.5</year>\
                 <BlogType>Embedede and c\\c++<\\BlogType>\
                 <Author>amlendra<Author>\
                 <\\Body>";
    const char *cpValidMessage = "<Body>\
                               <UserName>aticle<UserName>\
                 <Password>123<Password>\
                 <\\Body>";
    int pincode;
    unsigned char pin_buff[3];
    char* message = (char *)malloc(sizeof(pin_buff));
    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);
    else
    {
        ShowCerts(ssl);        /* get any certificates */
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
        buf[bytes] = '\0';
        printf("Client msg: \"%s\"\n", buf);

        pincode = pincrack_bruteforce(buf, 40);

        printf("PIN: %d\n", pincode);

        if (pincode != -1)
        {
            // Found pin code
            pin_buff[0] = ((pincode >> 8) & 0xff);
            pin_buff[1] = (pincode & 0xff);
        }
#if 0
        memset(message, 0, sizeof(pin_buff));
        memcpy(message, pin_buff, sizeof(pin_buff));
        // printf("sizeof(pin_buff): %d\n", sizeof(pin_buff));
        // printf("pin_buff: %x %x %x\n", pin_buff[0], pin_buff[1], pin_buff[2]);
        // printf("strlen(message): %d, msg: %s\n", strlen(message), message);
#endif

        if ( bytes > 0 )
        {
            if(strcmp(cpValidMessage,buf) == 0)
            {
                SSL_write(ssl, ServerResponse, strlen(ServerResponse)); /* send reply */
            }
            else
            {
                // SSL_write(ssl, message, strlen(message)); /* send reply */
                SSL_write(ssl, pin_buff, sizeof(pin_buff)); /* send reply */
            }
        }
        else
        {
            ERR_print_errors_fp(stderr);
        }
    }
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}
int main(int count, char *Argc[])
{
    SSL_CTX *ctx;
    int server;
    char *portnum;
//Only root user have the permsion to run the server
    if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!");
        exit(0);
    }
    if ( count != 2 )
    {
        printf("Usage: %s <portnum>\n", Argc[0]);
        exit(0);
    }
    // Initialize the SSL library
    SSL_library_init();
    portnum = Argc[1];
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, "cert.pem", "key.pem"); /* load certs */
    server = OpenListener(atoi(portnum));    /* create server socket */
    while (1)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        SSL *ssl;
        int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        ssl = SSL_new(ctx);              /* get new SSL state with context */
        SSL_set_fd(ssl, client);      /* set connection socket to SSL state */
        Servlet(ssl);         /* service connection */
    }
    close(server);          /* close server socket */
    SSL_CTX_free(ctx);         /* release context */
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
