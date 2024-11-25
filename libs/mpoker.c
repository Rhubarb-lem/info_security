#include "mpoker.h"
void initDeck(int *deck)
{

    for (int i = 1; i <= DECK_SIZE; i++)
    {
        deck[i - 1] = i;
    }
}
void initEDeck(long long int *deck, long long int p)
{
    srand(time(NULL));
    for (int i = 1; i <= DECK_SIZE; i++)
    {
        deck[i] = getRand(p);
    }
}
int searchCard(long long int card, long long int *deck)
{
    int i = 0;
    while (i < 52)
    {
        if (deck[i] == card)
        {
            deck[i] = -1;
            return i;
        }
        i++;
    }
    printf("Card not found!\n");
    return -1;
}
void shuffleDeck(int *deck)
{

    // for (int i = 0; i < 52; i++)
    // {
    //     printf("%d\n", deck[i]);
    // }
    srand(time(NULL));
    int t;
    for (int i = 0; i < DECK_SIZE; i++)
    {
        int j = getRand(DECK_SIZE - 1);
        // printf("j - %d - %d\n", j, deck[j]);
        t = deck[i];
        deck[i] = deck[j];
        deck[j] = t;
    }
}
void shufflellDeck(long long int *deck, int size)
{

    // for (int i = 0; i < size; i++)
    // {
    //     printf("%d\n", deck[i]);
    // }
    srand(time(NULL));
    long long int t;
    for (int i = 0; i < size; i++)
    {
        int j = getRand(size) - 1;

        t = deck[i];
        deck[i] = deck[j];
        deck[j] = t;
        // printf("j - %lld - %lld\n", j, deck[j]);
        // printf("i - %lld - %lld\n", i, deck[i]);

        // printf("------\n");
    }
}
int drawCard(long long int *deck)
{
    int i = 0;
    long long int card;
    while (i < 52)
    {
        // printf("%d\n", deck[i]);
        if (deck[i] != -1)
        {

            card = deck[i];
            deck[i] = -1;

            return card;
        }
        i++;
    }
    printf("Deck is empty!\n");
    return 0;
}
int drawICard(long long int *deck)
{
    int i = 0;
    long long int card;
    while (i < 52)
    {
        // printf("%d\n", deck[i]);
        if (deck[i] != -1)
        {

            card = deck[i];
            deck[i] = -1;

            return i;
        }
        i++;
    }
    printf("Deck is empty!\n");
    return 0;
}
void PokergenKeys(long long int P, long long int *c, long long int *d)
{

    srand(time(NULL));

    do
    {
        *c = getRand(10000);
    } while (genEuclid(*c, P - 1) != 1);
    *d = modInverse(*c, P - 1);
}
void mapCards(long long *a, long long *b, long long *g, long long int *deck, long long p)
{
    srand(time(NULL));

    a[0] = drawCard(deck);
    a[1] = getRand(p - 1);

    b[0] = drawCard(deck);
    // printf("vvvvv\n");
    b[1] = getRand(p - 1);

    g[0] = drawCard(deck);
    g[1] = getRand(p - 1);
}
void mapDeck(long long *cardMapping, int *deck, long long p)
{
    srand(time(NULL));
    for (int i = 0; i < DECK_SIZE; i++)
    {
        cardMapping[i] = getRand(p - 1); // Каждой карте сопоставляется случайное число.
    }
}
void encryptThreeCards(long long *u, long long a, long long b, long long g, long long c, long long p)
{
    int i, t;
    u[0] = fastExp(a, c, p);
    u[1] = fastExp(b, c, p);
    u[2] = fastExp(g, c, p);
    shufflellDeck(u, 3);
}

long long int getrandEnCard(long long int *u)
{
    srand(time(NULL));
    int i;
    long long int t;
    while (1)
    {
        i = getRand(3) - 1;
        if (u[i] != -1)
        {
            t = u[i];
            // printf("ua - %lld\n", u[i]);
            u[i] = -1;

            return t;
        }
    }
}
void encryptTwoCards(long long *u, long long c, long long p)
{

    if (u[0] == -1)
    {
        u[0] = u[1];
        u[1] = -1;
    }
    if (u[1] == -1)
    {
        u[1] = u[2];
        u[2] = -1;
    }
    long long int t;
    u[0] = fastExp(u[0], c, p);
    u[1] = fastExp(u[1], c, p);
    srand(time(NULL));
    if (getRand(1) == 2)
    {
        t = u[0];
        u[0] = u[1];
        u[1] = t;
    }
}
long long encryptOneCard(long long *u, long long d, long long p)
{
    if (u[0] == -1)
    {
        u[0] = u[1];
        u[1] = -1;
    }
    return fastExp(u[0], d, p);
}
void encryptDeck(long long *deckMapping, long long c, long long p)
{
    for (int i = 0; i < DECK_SIZE; i++)
    {
        deckMapping[i] = fastExp(deckMapping[i], c, p);
    }
}

long long decryptOneCard(long long w, long long d, long long p)
{
    return fastExp(w, d, p);
}

void encryptFullDeck(long long int *deck, long long int C, long long int P)
{
    for (int i = 0; i < 52; i++) // 47??
    {
        deck[i] = fastExp(deck[i], C, P);
    }
}
void erencryptAllPlayer(long long int *deck, long long int *C, int numplayer, long long P)
{
    for (int i = 0; i < numplayer; i++)
    {
        encryptFullDeck(deck, C[i], P);
        shufflellDeck(deck, 52);
    }
}
long long int decryptCardAllPlayers(long long int card, long long int *D, long long P, int playernum)
{
    for (int i = 0; i < playernum; i++)
    {
        card = fastExp(card, D[i], P);
    }
    return card;
}
void dealTable(long long int *table, long long int *deck)
{

    for (int j = 0; j < 5; j++)
    {
        table[j] = drawICard(deck);
    }
}

void displayCard(int num)
{
    setlocale(LC_ALL, "");
    wchar_t kind;
    char rang;
    switch (num / 13)
    {
    case 0:
        kind = 9824; // ♠
        break;
    case 1:
        kind = 9825; // ♡
        break;
    case 2:
        kind = 9826; // ♢
        break;
    case 3:
        kind = 9827; // ♣
        break;

    default:
        break;
    }
    switch (num % 13)
    {
    case 0:
        rang = '2';
        break;
    case 1:
        rang = '3';
        break;
    case 2:
        rang = '4';
        break;
    case 3:
        rang = '5';
        break;
    case 4:
        rang = '6';
        break;
    case 5:
        rang = '7';
        break;
    case 6:
        rang = '8';
        break;
    case 7:
        rang = '9';
        break;
    case 8:
        rang = '0';
        break;
    case 9:
        rang = 'J';
        break;
    case 10:
        rang = 'Q';
        break;
    case 11:
        rang = 'K';
        break;
    case 12:
        rang = 'A';
        break;

    default:
        break;
    }

    // wprintf(L"%lc\n", kind);
    if (rang != '0')
    {
        wprintf(L"┌───────┐\n│ %lc     │\n", kind);
        wprintf(L"│ %c     │\n", rang);
        // wprintf(L"│         │\n");
        // wprintf(L"│         │\n");
        wprintf(L"│     %c │\n", rang);
        wprintf(L"│     %lc │\n└───────┘\n", kind);
    }
    else
    {

        wprintf(L"┌───────┐\n│ %lc     │\n", kind);
        wprintf(L"│ 10    │\n", rang);
        // wprintf(L"│         │\n");
        // wprintf(L"│         │\n");
        wprintf(L"│    10 │\n", rang);
        wprintf(L"│     %lc │\n└───────┘\n", kind);
    }
}

void displayCardsInRow(long long int *cards, int count) {
    setlocale(LC_ALL, ""); 
    
    for (int row = 0; row < 6; row++) { 
        for (int i = 0; i < count; i++) {
            wchar_t kind;
            char rang;
            long long int num = cards[i];
            //wprintf(L"num - %lld\n", num);

            
            switch (num / 13) {
            case 0: kind = 9824; break; // ♠
            case 1: kind = 9825; break; // ♡
            case 2: kind = 9826; break; // ♢
            case 3: kind = 9827; break; // ♣
            default: kind = '?'; break;
            }

            
            switch (num % 13) {
            case 0: rang = '2'; break;
            case 1: rang = '3'; break;
            case 2: rang = '4'; break;
            case 3: rang = '5'; break;
            case 4: rang = '6'; break;
            case 5: rang = '7'; break;
            case 6: rang = '8'; break;
            case 7: rang = '9'; break;
            case 8: rang = '0'; break;
            case 9: rang = 'J'; break;
            case 10: rang = 'Q'; break;
            case 11: rang = 'K'; break;
            case 12: rang = 'A'; break;
            default: rang = '?'; break;
            }

            
            if (row == 0) {
                wprintf(L"┌───────┐ ");
            } else if (row == 1) {
                wprintf(L"│ %lc     │ ", kind);
            } else if (row == 2) {
                if (rang != '0') {
                    wprintf(L"│ %c     │ ", rang);
                } else {
                    wprintf(L"│ 10    │ ");
                }
            } else if (row == 3) {
                if (rang != '0') {
                    wprintf(L"│     %c │ ", rang);
                } else {
                    wprintf(L"│    10 │ ");
                }
            } else if (row == 4) {
                wprintf(L"│     %lc │ ", kind);
            } else if (row == 5) {
                wprintf(L"└───────┘ "); 
            }
        }
        wprintf(L"\n"); 
    }
}



