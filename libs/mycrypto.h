#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct bstree
{
    int value;
    int key;
    struct bstree *left;
    struct bstree *right;
} bst;

struct bstree *bstree_create(int key, int value);
void bstree_add(struct bstree *tree, int key, int value);
int bstree_lookup(struct bstree *tree, int key);

long long int getBinaryDigits(long long int n);
long long int fastExp(long long int base, long long int exp, long long int mod);
long long int genEuclid(long long int a, long long int b);
long long int getRand(unsigned int order);
int FermaCheck(long long int p);
long long int getPrimeRand();
long long int getRnadg(long long int p);
long long int getMyOpenKey(long long int p, long long int g, long long int mysecretKey);
long long int getSharSecKey(long long int p, long long int openKey, long long int mysecretKey);
long long int genDiffieHellman(long long int p, long long int g, long long int Xa, long long int Xb);
long long int babygiantStep(long long int a, long long int p, long long int y);
long long int gcdExtended(long long int a, long long int b, long long int *x, long long int *y);
long long int modInverse(long long int d, long long int phi);
