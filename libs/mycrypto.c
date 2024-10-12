#include "mycrypto.h"

void bstree_add(struct bstree *tree, int key, int value)
{
    if (tree == NULL)
        return;
    struct bstree *parent, *node;
    while (tree != NULL)
    {
        parent = tree;
        if (key < tree->key)
            tree = tree->left;
        else if (key > tree->key)
            tree = tree->right;
        else
            return;
    }
    node = bstree_create(key, value);
    if (key < parent->key)
        parent->left = node;
    else
        parent->right = node;
}
int bstree_lookup(struct bstree *tree, int key)
{
    while (tree != NULL)
    {
        if (key == tree->key)
            return tree->value;
        else if (key < tree->key)
            tree = tree->left;
        else
            tree = tree->right;
    }
    if (tree)
        return tree->value;

    return -1;
}

struct bstree *bstree_create(int key, int value)
{
    struct bstree *node;
    node = malloc(sizeof(*node));
    if (node != NULL)
    {
        node->key = key;
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

long long int getBinaryDigits(long long int n)
{
    if (n == 0)
        return 1;
    return (long long int)(log2(n)) + 1;
}

long long int fastExp(long long int base, long long int exp, long long int mod)
{
    long long int lengh = getBinaryDigits(exp);

    long long int remain, remain_prev = base, res = 1;

    remain = (remain_prev) % mod;
    if (exp & 1)
    {
        res *= remain;
    }
    remain_prev = remain;

    for (int i = 1; i < lengh; i++)
    {
        if (remain_prev == 1)
        {
            remain = 1;
        }
        else
        {
            remain = (remain_prev * remain_prev) % mod;
        }
        if (remain == 0)
            break;
        if ((exp >> i) & 1)
        {
            res = (res * remain) % mod;
        }

        remain_prev = remain;
    }

    return res % mod;
}
long long int genEuclid(long long int a, long long int b)
{
    long long int u[3], v[3], T[3];
    long long int q;
    if (a >= b)
    {
        u[0] = a;
        u[1] = 1;
        u[2] = 0;
        v[0] = b;
        v[1] = 0;
        v[2] = 1;
    }
    else
    {
        u[0] = b;
        u[1] = 0;
        u[2] = 1;
        v[0] = a;
        v[1] = 1;
        v[2] = 0;
    }
    q = u[0] / v[0];

    T[0] = u[0] - q * v[0];
    T[1] = u[1] - q * v[1];
    T[2] = u[2] - q * v[2];

    while (T[0] != 1)
    {

        u[0] = v[0];
        u[1] = v[1];
        u[2] = v[2];

        v[0] = T[0];
        v[1] = T[1];
        v[2] = T[2];

        q = u[0] / v[0];
        if (u[0] - q * v[0] == 0)
            break;
        T[0] = u[0] - q * v[0];
        T[1] = u[1] - q * v[1];
        T[2] = u[2] - q * v[2];
    }
    // printf("%d\n", T[0]);
    printf("%lld  %lld\n", T[1], T[2]);
    return T[0];
}
long long int getRand(unsigned int order)
{
    long long int r;
    // srand(time(NULL));
    r = rand();
    return (r % order) + 1;
}
int FermaCheck(long long int p)
{
    if (p == 2)
        return 1;
    for (int i = 0; i < 100; i++)
    {

        long long a = getRand(p - 2) + 2;
        if (genEuclid(a, p) != 1)
            return 0;

        if (fastExp(a, p - 1, p) != 1)
            return 0;
    }
    return 1;
}
long long int getPrimeRand()
{
    srand(time(0));
    long long int randP;
    long int flag = 0;
    while (flag == 0)
    {
        randP = getRand(10000);
        flag = FermaCheck(randP);
    }
    return randP;
}
long long int getRnadg(long long int p)
{
    long long int q = (p - 1) / 2;
    long long int g;
    while (1)
    {
        g = getRand(100);
        if ((1 < g) && (g < p - 1) && (fastExp(g, q, p) != 1))
            return g;
    }
}
long long int getMyOpenKey(long long int p, long long int g, long long int mysecretKey)
{

    if (((1 > g) || (g > (p - 1))))
    {
        printf("Error: wrong system parametrs!\n");
        return -1;
    }
    if ((p < 1) || (g < 1) || (mysecretKey < 1))
    {
        printf("Error: wrong system parametrs!\n");
        return -1;
    }
    printf("key - %lld, g - %lld, p %lld", mysecretKey, p, g);
    long long int myopenKey = fastExp(g, mysecretKey, p);
    return myopenKey;
}
long long int getSharSecKey(long long int p, long long int openKey, long long int mysecretKey)
{
    if ((openKey < 1) || (mysecretKey < 1) || (p < 1))
    {
        printf("Error: wrong parametrs\n");
        return 0;
    }

    long long int Zab = fastExp(openKey, mysecretKey, p);

    return Zab;
}
long long int genDiffieHellman(long long int p, long long int g, long long int Xa, long long int Xb)
{

    if (!((1 < g) && (g < (p - 1))))
    {
        printf("Error: wrong system parametrs!\n");
        return -1;
    }

    // long longint q = (p - 1) / 2;

    long long int Ya = fastExp(g, Xa, p);
    long long int Yb = fastExp(g, Xb, p);

    long long int Zab = fastExp(Yb, Xa, p);
    long long int Zba = fastExp(Ya, Xb, p);

    // printf("%lld\n", Zab);
    // printf("%lld\n", Zba);

    if (Zab != Zba)
    {
        printf("Error: wrong shared secret key calculation!\n");
        return -1;
    }
    printf("%lld\n", Zab);
    return Zab;
}

long long int babygiantStep(long long int a, long long int p, long long int y)
{

    long long int m, k = sqrt(p) + 1;
    m = k;
    long long int A[m], B[k];
    for (int i = 1; i < k; i++)
    {
        A[i] = (y % p * fastExp(a, i, p)) % p;
        B[i] = fastExp(a, m * (i), p);
    }
    int i = 0, j = 0;
    bst *tree = bstree_create(A[i], i);

    for (i = 0; i < m; ++i)
    {
        bstree_add(tree, A[i], i);
    }
    i = 0;
    for (j = 0; j < k; ++j)
    {

        i = bstree_lookup(tree, B[j]);
        if (i != -1)
        {

            free(tree);
            return (j + 1) * m - i;
            // return (i+1)*m-j+1;
        }
    }
    return 0;
}
//  long long int p = getPrimeRand();

// long long int g = getRnadg(p);

// long long int xa = getRand(1000), xb = getRand(1000);

// printf("xa = %lld, xb = %lld\n", xa, xb);
// long long int ya = getMyOpenKey(p, g, xa), yb = getMyOpenKey(p, g, xb);
// printf("Ya = %lld, Yb = %lld\n", ya, yb);
// long long int zab = getSharSecKey(p, yb, xa), zba = getSharSecKey(p, ya, xb);
// printf("Zab = %lld, Zba = %lld\n", zab, zba);

//  genDiffieHellman(p, g, xa, xb);