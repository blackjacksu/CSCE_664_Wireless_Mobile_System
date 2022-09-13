Compilation:
gcc -I/usr/local/ssl -L/usr/local/ssl -o sha1test sha1test.c -lcrypto -lssl


OS: Ubuntu 20.04

Dependent: 
1. Openssl: How to installed (https://linuxpip.org/install-openssl-linux/)
2. Check openssl versin: willytwsu-shell:openssl version -a
3. 