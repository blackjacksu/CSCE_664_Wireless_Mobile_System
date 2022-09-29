### Compile

```bash

cd HW2
make
```

### Run server

```bash
sudo ./server 1234
```

### Run client with your hash

```bash
./client localhost 1234 7110eda4d09e062aa5e4a390b0a572ac0d2c0220
1234
```



### Test case
./sha1test 1234
7110eda4d09e062aa5e4a390b0a572ac0d2c0220

./sha1test 0003
5caa47a3e35d5c847e2666a8f9c82c5287640e40

./sha1test 0050
95dd84c617999229b78c5f962ffb585b2d6b24af

./sha1test 4321
d5f12e53a182c062b6bf30c1445153faff12269a

## Terminal-1 run server
sudo ./server 1234
Connection: 127.0.0.1:42526
No certificates.
Client msg: "d5f12e53a182c062b6bf30c1445153faff12269a"
Hash: d5f12e53a182c062b6bf30c1445153faff12269a, Len: 40, SHA_DIGEST_LENGTH: 20
Digest: d5f12e53a182c062b6bf30c1445153faff12269a
Hash: d5f12e53a182c062b6bf30c1445153faff12269a, Length: 40
PIN: 4321



## Terminal-2 run client
./client localhost 1234 d5f12e53a182c062b6bf30c1445153faff12269a

SSL Client 0.1
~~~~~~~~~~~~~~~

Client SSL lib init complete
Connected with TLS_AES_256_GCM_SHA384 encryption

Server certificate:
Subject: /C=US/ST=TX/L=College Station/O=TAMU/OU=CSCE664/CN=Radu Stoleru
Issuer: /C=US/ST=TX/L=College Station/O=TAMU/OU=CSCE664/CN=Radu Stoleru
PIN: 4321

### OS requirement: Ubuntu 20.04

Dependent: 
1. Openssl: How to installed (https://linuxpip.org/install-openssl-linux/)
2. Check openssl versin: willytwsu-shell:openssl version -a
