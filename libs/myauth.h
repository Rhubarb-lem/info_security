#pragma once
#include "mycrypto.h"
#include "myciphers.h"
#include <stdio.h>

long int server_gen_module(); 

void client_gen_keys(long int N, long int *s, long int *v); 

long int client_proof_rand_x(long int N, long int *r); 

int server_gen_rand_bit(); 

long int client_proof_rand_y(long int N, long int r, long int s, int e); 

int server_check_proof(int e, long int x, long y, long int v, long int N);

unsigned long long mod(long long a, unsigned long long N);
