#include <stdio.h>
#include <math.h>

int getBinaryDigits(int n)
{
    if (n == 0)
        return 1;
    return (int)(log2(n)) + 1;
}

int fast_exp(int base, int exp, int mod)
{
    int lengh = getBinaryDigits(exp);
    int remain, remain_prev = base, res = 1;

    remain = (remain_prev) % mod;
    if (exp & 1)
    {
        res *= remain;
    }
    remain_prev = remain;
    printf("%d\n", remain);

    for (int i = 1; i < lengh; i++)
    {
        if (remain_prev == 1)
        {
            remain == 1;
        }
        else
        {
            remain = (remain_prev * remain_prev) % mod;
        }
        if (remain == 0)
            break;
        if ((exp >> i) & 1)
        {
            res *= remain;
        }

        remain_prev = remain;

    }

    return res % mod;
};

int main()
{

    printf("my %d\n", fast_exp(5, 20, 7));
    printf("lib %d\n", (int)pow(5, 20) % 7);
    return 0;
}