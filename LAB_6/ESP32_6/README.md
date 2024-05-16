# Firebase Realtime database

## to do 
> Declare crypto algorithm (RSA, Paillier, Elgamal) :
```c
struct RSA rsa
struct Paillier paillier
struct Elgamal base
```
![image](https://github.com/fontray/CPS_ESP32/blob/main/%E8%9E%A2%E5%B9%95%E6%93%B7%E5%8F%96%E7%95%AB%E9%9D%A2%202024-05-16%20135033.png)

> Initial crypto algorithm :
** RSA **
```c
rsa_init(struct RSA *rsa, uint64_t p, uint64_t q, uint64_t e)
```
* Elgamal
```c
Elgamal_init(struct Elgamal *base, uint64_t p, uint64_t g)
```
* Paillier
```c
Paillier_init(struct Paillier *paillier, uint64_t p, uint64_t q, uint64_t r, uint64_t g)
```
