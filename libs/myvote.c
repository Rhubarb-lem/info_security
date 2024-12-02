#include "myvote.h"

int vote()
{
    while (1)
    {
        printf("Введите число соответствующее вашему выбору\n");

        printf(" 1 - Да\n\n 2 - Нет\n\n 3 - Воздержался\n\n:");
        int v;
        scanf("%d", &v);
        if (v == 1 || v == 2 || v == 3)
        {
            printf("Голос зачтен!\n");
            return v;
        }
        printf("Не правильный выбор! Введите другое значение!\n");
    }
}

void blind_sign_gen_params(long long int *N, long long int *C, long long int *D)
{

    void RSAGenParams(long long int *N, long long int *C, long long int *D);
}

BIGNUM *generate_random_number(int num_bits)
{
    BIGNUM *rnd = BN_new();
    BN_rand(rnd, num_bits, 0, 0); 
    return rnd;
}

BIGNUM *create_v_number(const char *vote_data)
{
    BIGNUM *v = BN_new();
    BN_bin2bn((unsigned char *)vote_data, strlen(vote_data), v);
    return v;
}

BIGNUM *concatenate_numbers(BIGNUM *rnd, BIGNUM *v, int v_bits)
{
    BIGNUM *n = BN_new();
    BIGNUM *shifted_rnd = BN_new();
    BN_CTX *ctx = BN_CTX_new();

  
    BN_lshift(shifted_rnd, rnd, v_bits);

    
    BN_add(n, shifted_rnd, v);

  
    BN_free(shifted_rnd);
    BN_CTX_free(ctx);

    return n;
}

BIGNUM *create_n_number(BIGNUM *rnd, BIGNUM *v, int v_bits)
{
    BIGNUM *n = BN_new();
    BIGNUM *shifted_rnd = BN_new();
    BN_CTX *ctx = BN_CTX_new();


    BN_lshift(shifted_rnd, rnd, v_bits);


    BN_add(n, shifted_rnd, v);

    BN_free(shifted_rnd);
    BN_CTX_free(ctx);

    return n;
}

long long int gen_r(long long int N)
{

    int r;
    do
    {
        r = getRand(10000);
    } while (genEuclid(r, N) != 1);
    return r;
}

BIGNUM *compute_md5_hash(BIGNUM *n, BIGNUM *N)
{
  
    int n_len = BN_num_bytes(n);
    unsigned char *n_bytes = malloc(n_len);
    BN_bn2bin(n, n_bytes);

    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, n_bytes, n_len); 
    MD5_Final(hash, &md5_ctx);


    BIGNUM *h = BN_new();
    BN_bin2bn(hash, MD5_DIGEST_LENGTH, h);

    // // Проверяем, что h < N
    // if (BN_cmp(h, N) >= 0) {
    //     printf("Ошибка: MD5(n) >= N\n");
    //     BN_free(h);
    //     h = NULL;
    // }


    free(n_bytes);
    return h;
}

BIGNUM *compute_h2_value(BIGNUM *h, BIGNUM *r, BIGNUM *d, BIGNUM *N)
{
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *r_d = BN_new();
    BN_mod_exp(r_d, r, d, N, ctx); 

    BIGNUM *result = BN_new();
    BN_mod_mul(result, h, r_d, N, ctx);

    BN_free(r_d);
    BN_CTX_free(ctx);

    return result;
}

BIGNUM *compute_s_prime(BIGNUM *h_prime, BIGNUM *C, BIGNUM *N)
{
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *s_prime = BN_new();
    BN_mod_exp(s_prime, h_prime, C, N, ctx);

    BN_CTX_free(ctx);

    return s_prime;
}

BIGNUM *compute_signature(BIGNUM *s_prime, BIGNUM *r, BIGNUM *N)
{
    BN_CTX *ctx = BN_CTX_new();

    char *r_str = BN_bn2dec(r);
    long long int r_val = atoll(r_str);
    OPENSSL_free(r_str);

    long long int r_inv = modInverse(r_val, BN_get_word(N));
    if (r_inv == -1)
    {
        printf("Ошибка: инверсия r не существует.\n");
        BN_CTX_free(ctx);
        return NULL;
    }

    BIGNUM *r_inv_bn = BN_new();
    BN_set_word(r_inv_bn, r_inv);

    BIGNUM *s = BN_new();
    BN_mod_mul(s, s_prime, r_inv_bn, N, ctx);

    BN_free(r_inv_bn);
    BN_CTX_free(ctx);

    return s;
}
void compute_md5_hash2(const unsigned char *data, size_t len, unsigned char *result)
{
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, data, len);
    MD5_Final(result, &md5_ctx);
}


int verify_ballot(BIGNUM *n, BIGNUM *s, BIGNUM *d, BIGNUM *N)
{
    BN_CTX *ctx = BN_CTX_new();

    unsigned char n_hash[MD5_DIGEST_LENGTH];                                  
    unsigned char *n_bytes = (unsigned char *)OPENSSL_malloc(BN_num_bytes(n));
    BN_bn2bin(n, n_bytes);                                                     
    
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, n_bytes, BN_num_bytes(n));
    MD5_Final(n_hash, &md5_ctx);

    OPENSSL_free(n_bytes);


    BIGNUM *hash_bn = BN_bin2bn(n_hash, MD5_DIGEST_LENGTH, NULL);

   
    BIGNUM *s_d_mod_N = BN_new();
    BN_mod_exp(s_d_mod_N, s, d, N, ctx);


    int result = (BN_cmp(hash_bn, s_d_mod_N) );




    BN_free(hash_bn);
    BN_free(s_d_mod_N);
    BN_CTX_free(ctx);

    return result;
}
