### Compile

```bash

cd HW1
make
```

### Run server

```bash
./server
```

### Run pincracktest

```bash
./pincracktest 7110eda4d09e062aa5e4a390b0a572ac0d2c0220
1234
```



### Test case
./sha1test 1234
7110eda4d09e062aa5e4a390b0a572ac0d2c0220

/pincracktest 7110eda4d09e062aa5e4a390b0a572ac0d2c0220
Connect returned 0
send returned 40
bytes received 2
PIN: 1234
PIN found: 1234

./sha1test 0050
95dd84c617999229b78c5f962ffb585b2d6b24af

./pincracktest 95dd84c617999229b78c5f962ffb585b2d6b24af
Connect returned 0
send returned 40
bytes received 2
PIN: 50
PIN found: 50

### OS requirement: Ubuntu 20.04

Dependent: 
1. Openssl: How to installed (https://linuxpip.org/install-openssl-linux/)
2. Check openssl versin: willytwsu-shell:openssl version -a
