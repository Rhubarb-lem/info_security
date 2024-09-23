#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lab1/mycrypto.h"

int main()
{

    long long int y = fastExp(5, 16, 23);
    printf("my %lld\n", y);
    printf("%lld\n", babygiantStep(5, 23, 3));
    // printf("lib %d\n", (int)pow(5, 20) % 7);
    // genEuclid(30, 12);
    // DiffieHellman
    //  long long int p = getPrimeRand();
    // //long long int p = 14479;
    // long long int g = getRnadg(p);
    // // long long int g = 5;
    // srand(time(NULL));
    // long long int xa = getRand(1000), xb = getRand(1000);
    // //long long int xa = 248, xb = 82;
    // printf("xa = %lld, xb = %lld\n", xa, xb);
    // long long int ya = getMyOpenKey(p, g, xa), yb = getMyOpenKey(p, g, xb);
    // printf("Ya = %lld, Yb = %lld\n", ya, yb);
    // long long int zab = getSharSecKey(p, yb, xa), zba = getSharSecKey(p, ya, xb);
    // printf("Zab = %lld, Zba = %lld\n", zab, zba);

    // genDiffieHellman(p, g, xa, xb);
    return 0;
}