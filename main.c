#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libs/mycrypto.h"
#include "libs/myciphers.h"
#include "libs/mysigns.h"
#include "libs/mpoker.h"
#define DECKSIZE 52
#define CARDS_PER_PLAYER 2
#define NUMPLAYERS 20

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
  // printf("♠\n");
  //   int deck[52];
  //   initDeck(deck);
  //   for (int i = 0; i < 52; i++)
  //   {
  //     printf("%d\n", deck[i]);
  //   }
  //   printf("============\n");
  //   shuffleDeck(deck);
  //   for (int i = 0; i < 52; i++)
  //   {
  //     printf("%d\n", deck[i]);
  //   }
  //   // Генерируем параметры системы
  //   long long int p = getPrimeRand();
  //   long long int ca, da, cb, db;
  //   // Алиса генерирует ключи
  //   PokergenKeys(p, &ca, &da);
  //   // Боб генерирует ключи
  //   PokergenKeys(p, &cb, &db);
  //   // Алиса выбирает три случайных карты и сопоставляет им три случайных числа
  //   long long int a[2], b[2], g[2];
  //   printf("aaaaaaaaaaaaa\n");
  //   mapCards(a, b, g, deck, p); // мапить всю колоду

  //   printf("%lld - %lld\n", a[0], a[1]);
  //   printf("%lld - %lld\n", b[0], b[1]);
  //   printf("%lld - %lld\n", g[0], g[1]);
  //   // Алиса шифрует карты, перемешивает их и отправляет Бобу
  //   long long int u[3];
  //   encryptThreeCards(u, a[1], b[1], g[1], ca, p);
  //   // Боб выбирает случайную карту и отправляет ее Алисе. Алиса ее расшифовывает
  //  // long long int A_card = fastExp(getrandEnCard(u), da, p); // decryptOneCard
  //   long long int A_card = decryptOneCard(getrandEnCard(u), da, p); // decryptOneCard
  //   printf("Карта Алисы - %lld\n", A_card);
  //   // Боб шифрует две оставшиеся карты, возможно перемешивает их и отправляет Алисе
  //   encryptTwoCards(u, cb, p);
  //   // Алиса выбирает случайную карту, шифрует ее и отправляет Бобу
  //   long long int w = encryptOneCard(u, da, p);
  //   // Боб расшифровывает карту
  //   long long int B_card = decryptOneCard(w, da, p);
  //   printf("Карта Боба - %lld\n", B_card);
  //////////////////////////////////////////////////////////

  long long int deck[DECK_SIZE], edeck[DECK_SIZE];
  long long int P = getPrimeRand();
  initEDeck(deck, P);
  for (int i = 0; i < DECK_SIZE; i++)
  {
    edeck[i] = deck[i];
  }
  long long int c[NUMPLAYERS], d[NUMPLAYERS];
  long long int pcards[NUMPLAYERS][2];

  for (int i = 0; i < NUMPLAYERS; i++)
  {
    PokergenKeys(P, &c[i], &d[i]);
  }
  erencryptAllPlayer(edeck, c, NUMPLAYERS, P);
  for (int i = 0; i < NUMPLAYERS; i++)
  {
    pcards[i][0] = edeck[i];
    edeck[i] = -1;
    pcards[i][1] = edeck[DECK_SIZE - i - 1];
    edeck[DECK_SIZE - i] = -1;
  }
  for (int i = 0; i < NUMPLAYERS; i++)
  {
    pcards[i][0] = decryptCardAllPlayers(pcards[i][0], d, P, NUMPLAYERS);
    // printf("%d\n", pcards[i][0]);
    pcards[i][1] = decryptCardAllPlayers(pcards[i][1], d, P, NUMPLAYERS);
  }
  long long int discards[2];
  setlocale(LC_ALL, "");
  for (int i = 0; i < NUMPLAYERS; i++)
  {
    wprintf(L"Player %d cards:\n", i);
    // printf("Player %d cards:\n", i);
    pcards[i][0] = searchCard(pcards[i][0], deck);
    
    pcards[i][1] = searchCard(pcards[i][1], deck);
    
    discards[0] = pcards[i][0];
    discards[1] = pcards[i][1];
    wprintf(L"%lld\n", discards[0]);
    wprintf(L"%lld\n", discards[1]);
    displayCardsInRow(discards, 2);
  }
  shufflellDeck(deck, 52);
  long long int table[5];
  dealTable(table, deck);

  wprintf(L"Table:\n");
  displayCardsInRow(table, 5);

  return 0;
}