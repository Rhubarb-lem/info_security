#pragma once
#include "mycrypto.h"
#include "myciphers.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#define DECK_SIZE 52

void initDeck(int *deck);
void shuffleDeck(int *deck);
void shufflellDeck(long long int *deck, int size);
int drawCard(long long int *deck);
void PokergenKeys(long long int P, long long int *c, long long int *d);
void mapCards(long long *a, long long *b, long long *g, long long int *deck, long long p);
void encryptThreeCards(long long *u, long long a, long long b, long long g, long long c, long long p);
long long int getrandEnCard(long long int *u);
void encryptTwoCards(long long *u, long long c, long long p);
long long encryptOneCard(long long *u, long long d, long long p);
long long decryptOneCard(long long w, long long d, long long p);
void displayCard(int num);



void encryptFullDeck(long long int *deck, long long int D, long long int P);
void erencryptAllPlayer(long long int *deck, long long int *D, int numplayer, long long P);
void initEDeck(long long int *deck, long long int p);
int searchCard(long long int card, long long int *deck);
long long int decryptCardAllPlayers(long long int card, long long int *D, long long P, int playernum);
void dealTable(long long int *table, long long int *deck);
int drawICard(long long int *deck);
void displayCardsInRow(long long int *cards, int count);




