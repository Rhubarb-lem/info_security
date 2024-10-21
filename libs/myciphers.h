#pragma once
#include "mycrypto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void ElgamalGenPG(long long int *P, long long int *g);
void ElgamalGenKeys(long long int *C, long long int *D, long long int P, long long int g);
short ElgamalEncryptM(long long int D, long long int P, long long int g, char m, short *r);
char ElgamalDecryptM(long long int C, long long int P, short e, short r);
void ElgamalEncryptMessage(long long int D, long long int P, long long int g, char *message, short *rmessage, short *emessage);
void ElgamalDecryptMessage(long long int C, long long int P, short *emessage, short *rmessage, char *dmessage);
void ElgamalEncryptFile(long long int D, long long int P, long long int g, const char *input_filename, const char *output_filename);
void ElgamalDecryptFile(long long int C, long long int P, const char *input_filename, const char *output_filename);

void RSAGenParams(long long int *N, long long int *C, long long int *D);
long long int RSAEncryptM(long long int N, long long int M, long long int D);
long long int RSADecryptM(long long int N, long long int e, long long int C);
void RSAEncryptMessage(long long int N, long long int D, long long int *message, long long int *emessage);
void RSADecryptMessage(long long int N, long long int C, long long int *emessage, long long int *dmessage);
void RSAEncryptFile(long long int N, long long int D, const char *input_filename, const char *output_filename);
void RSADecryptFile(long long int N, long long int C, const char *input_filename, const char *output_filename);

long long int ShamirGenP();
void ShamirGenCD(long long int P, long long int *C, long long int *D);
long long int ShamirEncryptM(long long int P, long long int cdExp, long long int MX);
void ShamirEncryptMessage(long long int P, long long int cdExp, long long int *inMX, long long int *outMX);
void ShamirEncryptFile(long long int P, long long int cdExp, const char *input_filename, const char *output_filename);
void ShamirDecryptFile(long long int P, long long int cdExp, const char *input_filename, const char *output_filename);

void VernamGenKarr(unsigned char *k);
unsigned char VernamEncryptM(unsigned char m, unsigned char k);
unsigned char VernamDecryptM(unsigned char e, unsigned char k);
void VernamEncryptMessage(unsigned char *m, unsigned char *k, unsigned char *e);
void VernamEncryptMessage(unsigned char *e, unsigned char *k, unsigned char *dm);
void VernamEncryptFile(const char *input_filename, const char *key_filename, const char *output_filename);
void VernamDecryptFile(const char *input_filename, const char *key_filename, const char *output_filename);