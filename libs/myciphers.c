#include "myciphers.h"

// long long int shamir(){
//     long long int P =  getPrimeRand();

// }

void ElgamalGenPG(long long int *P, long long int *g)
{
    srand(time(NULL));
    *P = getPrimeRand();
    *g = getRnadg(*P);
}

void ElgamalGenKeys(long long int *C, long long int *D, long long int P, long long int g)
{
    srand(time(NULL));
    *C = getRand(P - 1);
    *D = fastExp(g, *C, P);
}

char ElgamalEncryptMessage(long long int D, long long int P, long long int g, char m, long long int *r)
{
    srand(time(NULL));
    long long int k = getRand(P - 2);
    *r = fastExp(g, k, P);
    char e = m * fastExp(D, k, P);
    return e;
}
char ElgamalDecryptMessage(long long int C, long long int P, long long int e, long long int r)
{
    char dm = e * fastExp(r, P - 1 - C, P);
    return e;
}