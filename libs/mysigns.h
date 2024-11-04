#pragma once
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/bn.h>
#include "mycrypto.h"
#include "myciphers.h"

void compute_md5(const char *str, unsigned char *result);
void RSASign(const char *message, long long N, long long c, BIGNUM *signature);
int RSASigncheck(const char *message, long long N, long long d, BIGNUM *signature);
void compute_md5_file(FILE *file, unsigned char *result);
int RSASigncheckFile(const char *file_path, long long N_val, long long d_val, const char *signature_path);
int RSASignFile(const char *file_path, long long N_val, long long c_val, const char *signature_path);

int ElgamalSign(const char *message, long long X_val, long long P_val, long long G_val, long long *r, long long *s);
int ElgamalcheckSign(const char *message, long long Y_val, long long P_val, long long G_val, long long r, long long s);
int ElgamalSignFile(const char *file_path, long long X_val, long long P_val, long long G_val, const char *signature_file);
int ElgamalCheckFileSignature(const char *file_path, long long Y_val, long long P_val, long long G_val, const char *signature_file) ;
