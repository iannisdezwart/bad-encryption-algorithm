# bad-encryption-algorithm
The learning process of developing an encryption algorithm

---

To use the best encryption algorithm in the world, clone thise repo and select the version you want to use. (Don't even bother using version 1-4, they are utter rubbish...)

There are 5 versions (`1.hpp`, `2.hpp`, `3.hpp`, `4.hpp`, `5.hpp`).
Each version is supposed to be better than its ancestor.
Change line 3 (containing `#include "5.hpp"`) in `encrypt.cpp` according to the version you want to use.

Then compile the file:

```
$ g++ encrypt.cpp -o encrypt
```

---

You can then execute it to encrypt something:

```
$ ./encrypt
1							# We choose option 1 (encrypt)
Hello, World!						# We input the plaintext in ascii
MyVerySecretKey						# We input the key in ascii
> f2 17 07 d1 67 d6 3c 2b 0b 23 21 9a 41 36 de 60	# The output is the hex representation of the ciphertext
```

Yay, it worked!

---

Let's decrypt the message:

```
$ ./encrypt
2							# We choose option 2 (decrypt)
f2 17 07 d1 67 d6 3c 2b 0b 23 21 9a 41 36 de 60		# We input the hex encoded ciphertext
MyVerySecretKey						# We input the key in ascii
> 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 00 00 00	# The output is the hex representation of "Hello, World!"
```

Wow, that actually worked???

---

Long story short: use AES.