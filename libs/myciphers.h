#pragma once
#include"mycrypto.h"

void ElgamalGenPG(long long int *P, long long int *g);
void ElgamalGenKeys(long long int *C, long long int *D, long long int P, long long int g);
char ElgamalEncryptMessage(long long int D, long long int P, long long int g, char m, long long int *r);
char  ElgamalDecryptMessage(long long int C, long long int P,  long long int e, long long int r);
