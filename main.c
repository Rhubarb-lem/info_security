#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libs/mycrypto.h"
#include "libs/myciphers.h"

int main()
{
    long long int P, g;
    ElgamalGenPG(&P, &g);

    long long int Ca, Da;
    ElgamalGenKeys(&Ca, &Da, P, g);

    long long int Cb, Db;
    ElgamalGenKeys(&Cb, &Db, P, g);

    long long int m = 56; // Сообщение
    long long int r;
    long long int e = ElgamalEncryptMessage(Db, P, g, m, &r);

    long long int dm = ElgamalDecryptMessage(Cb, P, e, r);
    printf("Сообщение m=%lld\n Зашифрованное e=%lld\n Расшифрованное dm=%lld\n", m, e, dm);

    return 0;
}