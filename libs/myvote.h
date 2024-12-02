#pragma once
#include "mycrypto.h"
#include "myciphers.h"
#include "mysigns.h"
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/bn.h>

int vote();
void blind_sign_gen_params(long long int *N, long long int *C, long long int *D);
BIGNUM *generate_random_number(int num_bits);
BIGNUM *create_v_number(const char *vote_data);
BIGNUM *concatenate_numbers(BIGNUM *rnd, BIGNUM *v, int v_bits);
BIGNUM *create_n_number(BIGNUM *rnd, BIGNUM *v, int v_bits);
long long int gen_r(long long int N);
BIGNUM *compute_md5_hash(BIGNUM *n, BIGNUM *N);
BIGNUM *compute_h2_value(BIGNUM *h, BIGNUM *r, BIGNUM *d, BIGNUM *N);
BIGNUM *compute_s_prime(BIGNUM *h_prime, BIGNUM *C, BIGNUM *N);
BIGNUM *compute_signature(BIGNUM *s_prime, BIGNUM *r, BIGNUM *N);
int verify_ballot(BIGNUM *n, BIGNUM *s, BIGNUM *d, BIGNUM *N) ;
void compute_md5_hash2(const unsigned char *data, size_t len, unsigned char *result);

