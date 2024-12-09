#include "myauth.h"
//#define T 10
long int server_gen_module()
{
    // srand(time(NULL)); 
    long int P = getPrimeRand(), Q = getPrimeRand();

    while (P == Q)
    {
        Q = getPrimeRand();
    }

    return P * Q;
}

void client_gen_keys(long int N, long int *s, long int *v)
{
    do
    {
        *s = getRand(N);
    } while (genEuclid(*s, N) != 1);
    *v = (*s * *s) % N; // fastexp?
}

long int client_proof_rand_x(long int N, long int *r)
{
    //srand(time(NULL));

    *r = getRand(N - 1);
    return (*r * *r) % N;
}

int server_gen_rand_bit()
{
    return rand() % 2;
}

unsigned long long mod(long long a, unsigned long long N)
{
    return (a % N + N) % N;
}

long int client_proof_rand_y(long int N, long int r, long int s, int e)
{
    if (e == 0)
    {
        return r;
    }
    else
    {
        return (r * s) % N;
    }
}

int server_check_proof(int e, long int x, long int y, long int v, long int N)
{
    unsigned long long yy = mod((unsigned long long)y * y, N);
    unsigned long long xv = mod((unsigned long long)x * v, N);
    if (e == 0)
    {
        return yy == x;
    }
    else
    {
        return yy == xv;
    }
}
