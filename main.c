#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libs/mycrypto.h"
#include "libs/myciphers.h"

int main()
{
    // long long int P, g;
    // ElgamalGenPG(&P, &g);

    // long long int Ca, Da;
    // ElgamalGenKeys(&Ca, &Da, P, g);

    // long long int Cb, Db;
    // ElgamalGenKeys(&Cb, &Db, P, g);

    // char m = 56; // Сообщение
    // short r;
    // //short e = ElgamalEncryptMessage(Db, P, g, m, &r);
    // char message[] = "Hello!";
    // short emessage[7], rmessage[7];
    // char dmessage[7] ;

    // ElgamalEncryptMessage(Db, P, g, message, rmessage, emessage);
    // ElgamalDecryptMessage(Cb, P, emessage, rmessage, dmessage);
    // ElgamalEncryptFile(Db, P, g, "message.png", "emessaage.png");
    //  ElgamalDecryptFile( Cb,  P,  "emessaage.png", "dmessaage.png" );
    // for(int i = 0; i<7; i++){
    //     emessage[i] = ElgamalEncryptMessage(Db, P, g, message[i], &rmessage[i]);
    // }
    // for(int i = 0; i<7; i++){
    //     dmessage[i] = ElgamalDecryptMessage(Cb, P, emessage[i], rmessage[i]);
    // }
    // char dm = ElgamalDecryptMessage(Cb, P, e, r);
    // printf("Сообщение m=%s\n Расшифрованное dm=%s\n", message,  dmessage);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  srand(time(NULL));
    // long long int C, D, N;
    // RSAGenParams(&N, &C, &D);
    // // long long int m = 15;
    // // unsigned long long int e = RSAEncryptM(N, m , D);
    // // printf("%lld", e);
    // // long long int dm = RSADecryptM(N, e, C);
    // RSAEncryptFile(N, D, "message2.jpg", "emessage2.jpg");
    // RSADecryptFile(N, C, "emessage2.jpg", "dmessage2.jpg");

    // printf("Сообщение m=%lld\n Расшифрованное dm=%lld\n", m,  dm);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    long long int P = ShamirGenP();
    long long int Ca, Da, Cb, Db;
    ShamirGenCD(P, &Ca, &Da);
    ShamirGenCD(P, &Cb, &Db);
    // long long int m = 15;
    // long long int x;
    // x = ShamirEncryptM(P, Ca, m);
    // x = ShamirEncryptM(P, Cb, x);
    // x = ShamirEncryptM(P, Da, x);
    // x = ShamirEncryptM(P, Db, x);
    // printf("Сообщение m=%lld\n Расшифрованное dm=%lld\n", m, x);
    // ShamirEncryptFile(P, Ca, "message3.jpg", "1emessage3.jpg" );
    // ShamirDecryptFile(P, Cb, "1emessage3.jpg", "2emessage3.jpg" );
    // ShamirEncryptFile(P, Da, "2emessage3.jpg", "3emessage3.jpg" );
    // ShamirDecryptFile(P, Db, "3emessage3.jpg", "dmessage3.jpg" );
    ShamirEncryptFile(P, Ca, "message3.jpg", "1emessage3.jpg");
    ShamirEncryptFile(P, Cb, "1emessage3.jpg", "2emessage3.jpg");
    ShamirEncryptFile(P, Da, "2emessage3.jpg", "3emessage3.jpg");
    ShamirEncryptFile(P, Db, "3emessage3.jpg", "dmessage3.jpg");
    // unsigned char k = getRand(255);
    // unsigned char m = 15, e, dm;
    // e = VernamEncryptM(m, k);
    // dm =  VernamDecryptM(e, k);
    //     printf("Сообщение m=%d\n Расшифрованное dm=%d\n", m, dm);
    // VernamEncryptFile("message4.jpg", "keyfile", "emessage4.jpg");

    // VernamDecryptFile("emessage4.jpg", "keyfile", "dmessage4.jpg");

    return 0;
}