# bad-encryption-algorithm
The learning process of developing an encryption algorithm

---

First compile one of the versions of the algorithm:

```
$ g++ 5.cpp -o 5
```

---

You can then execute it to encrypt something:

```
$ ./5
< 1							# We choose option 1 (encrypt)
< Hello, World!						# We input the plaintext in ascii
< MyVerySecretKey					# We input the key in ascii
> f2 17 07 d1 67 d6 3c 2b 0b 23 21 9a 41 36 de 60	# This is the hex representation of the ciphertext
```

---

Let's decrypt this message

```
$ ./5
< 2							# We choose option 2 (decrypt)
< f2 17 07 d1 67 d6 3c 2b 0b 23 21 9a 41 36 de 60	# We input the hex encoded ciphertext
< MyVerySecretKey					# We input the key in ascii
> 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 00 00 00	# This is the hex representation of "Hello, World!"
```
