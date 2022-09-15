Compilation:
gcc -I/usr/local/ssl -L/usr/local/ssl -o sha1test sha1test.c -lcrypto -lssl


OS: Ubuntu 20.04

Dependent: 
1. Openssl: How to installed (https://linuxpip.org/install-openssl-linux/)
2. Check openssl versin: willytwsu-shell:openssl version -a
3. 

Test: 
case 1234:
sh:./sha1test 1234
sh:7110eda4d09e062aa5e4a390b0a572ac0d2c0220

case 
