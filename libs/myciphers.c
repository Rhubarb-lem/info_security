#include "myciphers.h"

// long long int shamir(){
//     long long int P =  getPrimeRand();

// }

void ElgamalGenPG(long long int *P, long long int *g)
{
    srand(time(NULL));
    *P = getPrimeRand();
    *g = getRnadg(*P);
    // printf("P=%lld G=%lld\n", *P, *g);
}

void ElgamalGenKeys(long long int *C, long long int *D, long long int P, long long int g)
{
    srand(time(NULL));
    *C = getRand(P - 1);
    *D = fastExp(g, *C, P);
    // printf("C=%lld D=%lld\n", *C, *D);
}

// char ElgamalEncryptMessage(long long int D, long long int P, long long int g, char m, long long int *r)
// {
//     srand(time(NULL));
//     long long int k = getRand(P - 2);
//     *r = fastExp(g, k, P);
//     long long int e = (m * fastExp(D, k, P)) % P;
//     return e;
// }
// char ElgamalDecryptMessage(long long int C, long long int P, long long int e, long long int r)
// {
//    long long int dm = (e * fastExp(r, P - 1 - C, P)) % P;
//     return dm;
// }

long long int ElgamalEncryptMessage(long long int D, long long int P, long long int g, long long int m, long long int *r)
{
    srand(time(NULL));
    long long int k = getRand(P - 2);             // Генерируем случайное k
    *r = fastExp(g, k, P);                        // r = g^k mod P
    long long int e = (m * fastExp(D, k, P)) % P; // e = m * D^k mod P
    // printf("Зашифрованное сообщение: e=%lld, r=%lld\n", e, *r);
    return e; // Возвращаем зашифрованное сообщение
}

long long int ElgamalDecryptMessage(long long int C, long long int P, long long int e, long long int r)
{
    long long int r_inv = fastExp(r, P - 1 - C, P); // r_inv = r^(P-1-C) mod P
    long long int dm = (e * r_inv) % P;             // dm = e * r_inv mod P
    // printf("Расшифрованное сообщение: dm=%lld\n", dm);
    return dm; // Возвращаем расшифрованное сообщение
}