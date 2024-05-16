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
