# Firebase Realtime database

## to do 
> Declare crypto algorithm (RSA, Paillier, Elgamal) :
```c
struct RSA rsa
struct Paillier paillier
struct Elgamal base
```

> Initial crypto algorithm :
1. RSA 
```c
rsa_init(struct RSA *rsa, uint64_t p, uint64_t q, uint64_t e)
```
   - p/q is prime number
   - gcd(φ(n), e) = 1 , 1 < e < φ(n)

2. Elgamal
```c
Elgamal_init(struct Elgamal *base, uint64_t p, uint64_t g)
```
   - g is prime number
   - g is generator

3. Paillier
```c
Paillier_init(struct Paillier *paillier, uint64_t p, uint64_t q, uint64_t r, uint64_t g)
```
   - p/q is prime number
   - g is generator
   - 0 < r < n and gcd(r,n) = 1

> Encrypt based on your algorithm :
1. RSA 
```c
Encrypt_RSA(struct RSA *rsa, uint32_t message)
```
   - message is plaintext
   - return unit64_t()

2. Elgamal
```c
Encrypt_Elgamal(struct Elgamal *base, uint32_t message, uint64_t Y, uint64_t x)
gen_Y(struct Elgamal *base, uint64_t y)
```
   - Y use function gen_Y to generate
   - x/y is random number
   - return (key, cipher)

3. Paillier
```c
Encrypt_Paillier(struct Paillier *paillier, uint32_t message)
```
   - return unit64_t()

> Decrypt based on your algorithm :
1. RSA 
```c
Decrypt_RSA(struct RSA *rsa, uint64_t cipher)
```
   - return unit64_t()

2. Elgamal
```c
Decrypt_Elgamal(struct Elgamal *base, uint64_t cipher, uint64_t K)
```
   - K is Encrypt_Elgamal first return
   - return unit64_t()

3. Paillier
```c
Decrypt_Paillier(struct Paillier *paillier, uint64_t cipher)
```
   - return int()

> oled_display :
```c
oled_display(uint64_t t, uint64_t h)
```
