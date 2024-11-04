#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libs/mycrypto.h"
#include "libs/myciphers.h"
#include "libs/mysigns.h"

int main()
{

  // long long a = 10, p = 117, x = 100;
  // long long y = fastExp(a, x, p);
  // long long x2 = babygiantStep(a, p , y);
  // printf("x = %lld, x2 = %lld\n", x, x2);

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
  // long long int P = ShamirGenP();
  // long long int Ca, Da, Cb, Db;
  // ShamirGenCD(P, &Ca, &Da);
  // ShamirGenCD(P, &Cb, &Db);
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
  // ShamirEncryptFile(P, Ca, "message3.jpg", "1emessage3.jpg");
  // ShamirEncryptFile(P, Cb, "1emessage3.jpg", "2emessage3.jpg");
  // ShamirEncryptFile(P, Da, "2emessage3.jpg", "3emessage3.jpg");
  // ShamirEncryptFile(P, Db, "3emessage3.jpg", "dmessage3.jpg");
  // unsigned char k = getRand(255);
  // unsigned char m = 15, e, dm;
  // e = VernamEncryptM(m, k);
  // dm =  VernamDecryptM(e, k);
  //     printf("Сообщение m=%d\n Расшифрованное dm=%d\n", m, dm);
  // VernamEncryptFile("message4.jpg", "keyfile", "emessage4.jpg");

  // VernamDecryptFile("emessage4.jpg", "keyfile", "dmessage4.jpg");

  // srand(time(NULL));
  // long long int C, D, N;
  // RSAGenParams(&N, &C, &D);
  // // char *message = "Aboba!\n";
  // // BIGNUM *sign = BN_new();
  // // RSASign(message, N, C, sign);
  // // int check = RSASigncheck(message, N, D, sign);
  // // printf("Check result: %d\n", check);
  // RSASignFile("message2.jpg", N, C, "signmessage2");

  // int check = RSASigncheckFile("message2.jpg", N, D, "signmessage2");
  //  printf("Check result: %d\n", check);
  /////////////////////////////////////////////////////////////////
  // long long P, G;
  // ElgamalGenPG(&P, &G);

  // long long x, y, r;
  // ElgamalGenKeys(&x, &y, P, G);
  // long long s;
  // // ElgamalSign("Hello!", x, P, G, &r, &s);
  // // int check2 = ElgamalcheckSign("Hello!", y, P, G, r, s);
  // ElgamalSignFile("message3.jpg", x, P, G, "signmessage3");
  // int check2 = ElgamalCheckFileSignature("message3.jpg", y, P, G, "signmessage3");

  //  printf("Check result: %d\n", check2);
  const char *message = "Test message for GOST signature";

  // Создание переменных для ключей и подписи
  BIGNUM *private_key = BN_new();
  BIGNUM *public_key = BN_new();
  BIGNUM *signature_r = BN_new();
  BIGNUM *signature_s = BN_new();

  // Генерация ключей
  if (GOSTGenerateKeys(private_key, public_key) != 0)
  {
    fprintf(stderr, "Ошибка при генерации ключей.\n");
    return 1;
  }

  // Подписание сообщения
  if (GOSTSign(message, private_key, public_key, signature_r, signature_s) != 0)
  {
    fprintf(stderr, "Ошибка при подписании сообщения.\n");
    return 1;
  }
  printf("Подпись успешно создана.\n");

  // Проверка подписи
  if (GOSTVerify(message, public_key, signature_r, signature_s) == 1)
  {
    printf("Подпись успешно проверена и подтверждена.\n");
  }
  else
  {
    printf("Ошибка проверки подписи.\n");
  }

  // Очистка памяти
  BN_free(private_key);
  BN_free(public_key);
  BN_free(signature_r);
  BN_free(signature_s);

  return 0;
}