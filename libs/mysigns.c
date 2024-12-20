#include "mysigns.h"

void compute_md5(const char *str, unsigned char *result)
{
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, str, strlen(str));
    MD5_Final(result, &md5_ctx);
}

void RSASign(const char *message, long long N_val, long long c_val, BIGNUM *signature)
{
    unsigned char md[MD5_DIGEST_LENGTH];
    compute_md5(message, md);

    BIGNUM *N = BN_new();
    BIGNUM *c = BN_new();

    char N_str[20], c_str[20];
    snprintf(N_str, sizeof(N_str), "%lld", N_val);
    snprintf(c_str, sizeof(c_str), "%lld", c_val);

    BN_dec2bn(&N, N_str);
    BN_dec2bn(&c, c_str);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);

    BN_CTX *ctx = BN_CTX_new();

    BN_mod_exp(signature, hash_bn, c, N, ctx);

    BN_free(hash_bn);
    BN_free(N);
    BN_free(c);
    BN_CTX_free(ctx);
}
int RSASigncheck(const char *message, long long N_val, long long d_val, BIGNUM *signature)
{
    BIGNUM *N = BN_new();
    BIGNUM *d = BN_new();

    char N_str[20], d_str[20];
    snprintf(N_str, sizeof(N_str), "%lld", N_val);
    snprintf(d_str, sizeof(d_str), "%lld", d_val);

    BN_dec2bn(&N, N_str);
    BN_dec2bn(&d, d_str);

    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *hashcheck = BN_new();
    BN_mod_exp(hashcheck, signature, d, N, ctx);

    unsigned char md[MD5_DIGEST_LENGTH];
    compute_md5(message, md);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);

    int result = (BN_cmp(hashcheck, hash_bn) == 0);

    BN_free(N);
    BN_free(d);
    BN_free(hashcheck);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return result;
}
void compute_md5_file(FILE *file, unsigned char *result)
{
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);

    unsigned char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        MD5_Update(&md5_ctx, buffer, bytes_read);
    }

    MD5_Final(result, &md5_ctx);
}
int RSASignFile(const char *file_path, long long N_val, long long c_val, const char *signature_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Failed to open file");
        return 0;
    }

    unsigned char md[MD5_DIGEST_LENGTH];
    compute_md5_file(file, md);
    fclose(file);

    BIGNUM *N = BN_new();
    BIGNUM *c = BN_new();
    BIGNUM *signature = BN_new();

    char N_str[20], c_str[20];
    snprintf(N_str, sizeof(N_str), "%lld", N_val);
    snprintf(c_str, sizeof(c_str), "%lld", c_val);

    BN_dec2bn(&N, N_str);
    BN_dec2bn(&c, c_str);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);

    BN_CTX *ctx = BN_CTX_new();

    BN_mod_exp(signature, hash_bn, c, N, ctx);

    FILE *sig_file = fopen(signature_path, "wb");
    if (!sig_file)
    {
        perror("Failed to open file");
        BN_free(N);
        BN_free(c);
        BN_free(signature);
        BN_free(hash_bn);
        BN_CTX_free(ctx);
        return 0;
    }

    int sig_len = BN_num_bytes(signature);
    unsigned char *sig_buf = malloc(sig_len);
    BN_bn2bin(signature, sig_buf);
    fwrite(sig_buf, 1, sig_len, sig_file);
    fclose(sig_file);

    free(sig_buf);
    BN_free(N);
    BN_free(c);
    BN_free(signature);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return 1;
}

int RSASigncheckFile(const char *file_path, long long N_val, long long d_val, const char *signature_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Failed to open file");
        return 0;
    }

    unsigned char md[MD5_DIGEST_LENGTH];
    compute_md5_file(file, md);
    fclose(file);

    BIGNUM *N = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM *signature = BN_new();

    char N_str[20], d_str[20];
    snprintf(N_str, sizeof(N_str), "%lld", N_val);
    snprintf(d_str, sizeof(d_str), "%lld", d_val);

    BN_dec2bn(&N, N_str);
    BN_dec2bn(&d, d_str);

    FILE *sig_file = fopen(signature_path, "rb");
    if (!sig_file)
    {
        perror("Failed to open file");
        BN_free(N);
        BN_free(d);
        return 0;
    }

    fseek(sig_file, 0, SEEK_END);
    long sig_len = ftell(sig_file);
    fseek(sig_file, 0, SEEK_SET);
    unsigned char *sig_buf = malloc(sig_len);
    fread(sig_buf, 1, sig_len, sig_file);
    fclose(sig_file);

    BN_bin2bn(sig_buf, sig_len, signature);
    free(sig_buf);

    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *hashcheck = BN_new();
    BN_mod_exp(hashcheck, signature, d, N, ctx);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);

    int result = (BN_cmp(hashcheck, hash_bn) == 0);

    BN_free(N);
    BN_free(d);
    BN_free(signature);
    BN_free(hashcheck);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return result;
}
int ElgamalSign(const char *message, long long X_val, long long P_val, long long G_val, long long *r, long long *s)
{
    BN_CTX *ctx = BN_CTX_new();
    unsigned char md[MD5_DIGEST_LENGTH];
    compute_md5(message, md);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);

    
    BIGNUM *P = BN_new();
    BIGNUM *X = BN_new();
    BIGNUM *K = BN_new();
    BIGNUM *G = BN_new();
    BIGNUM *R = BN_new();

    BN_set_word(P, P_val);
    BN_set_word(X, X_val);
    BN_set_word(G, G_val);

    long long k;
    do
    {
        k = getRand(P_val - 1);
    } while (genEuclid(k, P_val) != 1);
    BN_set_word(K, k);

    BN_mod_exp(R, G, K, P, ctx); 

    
    char *r_str = BN_bn2dec(R);
    *r = atoll(r_str);
    OPENSSL_free(r_str);

    
    BIGNUM *P_minus_1 = BN_new();
    BN_sub(P_minus_1, P, BN_value_one()); 

    BIGNUM *XR = BN_new();
    BIGNUM *U = BN_new();

    
    BIGNUM *K_inv = BN_mod_inverse(NULL, K, P_minus_1, ctx);
    if (K_inv == NULL)
    {
        
        BN_CTX_free(ctx);
        return 0;
    }

    BN_mul(XR, X, R, ctx);        
    BN_sub(U, hash_bn, XR);       
    BN_mod(U, U, P_minus_1, ctx); 

    BIGNUM *S = BN_new();
    BN_mod_mul(S, K_inv, U, P_minus_1, ctx); 

    char *s_str = BN_bn2dec(S);
    *s = atoll(s_str);
    OPENSSL_free(s_str);

    BN_free(P);
    BN_free(X);
    BN_free(G);
    BN_free(R);
    BN_free(XR);
    BN_free(U);
    BN_free(K);
    BN_free(K_inv);
    BN_free(P_minus_1);
    BN_free(S);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return 1;
}


int ElgamalcheckSign(const char *message, long long Y_val, long long P_val, long long G_val, long long r, long long s)
{
    BN_CTX *ctx = BN_CTX_new();
    unsigned char md[MD5_DIGEST_LENGTH];
    compute_md5(message, md);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);


    BIGNUM *P = BN_new();
    BIGNUM *Y = BN_new();
    BIGNUM *G = BN_new();
    BIGNUM *R = BN_new();
    BIGNUM *S = BN_new();

    BN_set_word(P, P_val);
    BN_set_word(Y, Y_val);
    BN_set_word(G, G_val);
    BN_set_word(R, r);
    BN_set_word(S, s);


    BIGNUM *G_hash = BN_new();
    BIGNUM *Yr = BN_new();
    BIGNUM *Rs = BN_new();
    BIGNUM *check = BN_new();

    BN_mod_exp(G_hash, G, hash_bn, P, ctx); 
    BN_mod_exp(Yr, Y, R, P, ctx);           
    BN_mod_exp(Rs, R, S, P, ctx);           

    BN_mod_mul(check, Yr, Rs, P, ctx); 


    int result = (BN_cmp(G_hash, check) == 0);


    BN_free(P);
    BN_free(Y);
    BN_free(G);
    BN_free(R);
    BN_free(S);
    BN_free(G_hash);
    BN_free(Yr);
    BN_free(Rs);
    BN_free(check);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return result;
}

int ElgamalSignFile(const char *file_path, long long X_val, long long P_val, long long G_val, const char *signature_file) {
    BN_CTX *ctx = BN_CTX_new();
    unsigned char md[MD5_DIGEST_LENGTH];

    
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }
    compute_md5_file(file, md);
    fclose(file);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);


    BIGNUM *P = BN_new();
    BIGNUM *X = BN_new();
    BIGNUM *K = BN_new();
    BIGNUM *G = BN_new();
    BIGNUM *R = BN_new();

    BN_set_word(P, P_val);
    BN_set_word(X, X_val);
    BN_set_word(G, G_val);

    long long k;
    do {
        k = getRand(P_val - 1);
    } while (genEuclid(k, P_val) != 1);
    BN_set_word(K, k);

    BN_mod_exp(R, G, K, P, ctx); 

    char *r_str = BN_bn2dec(R);
    long long r = atoll(r_str);
    OPENSSL_free(r_str);

    BIGNUM *P_minus_1 = BN_new();
    BN_sub(P_minus_1, P, BN_value_one());

    BIGNUM *XR = BN_new();
    BIGNUM *U = BN_new();
    BIGNUM *K_inv = BN_mod_inverse(NULL, K, P_minus_1, ctx);
    if (K_inv == NULL) {
        BN_CTX_free(ctx);
        return 0;
    }

    BN_mul(XR, X, R, ctx);
    BN_sub(U, hash_bn, XR);
    BN_mod(U, U, P_minus_1, ctx);

    BIGNUM *S = BN_new();
    BN_mod_mul(S, K_inv, U, P_minus_1, ctx);

    char *s_str = BN_bn2dec(S);
    long long s = atoll(s_str);
    OPENSSL_free(s_str);


    FILE *sig_file = fopen(signature_file, "w");
    if (!sig_file) {
        perror("Failed to open signature file");
        return 0;
    }
    fprintf(sig_file, "%lld\n%lld\n", r, s);
    fclose(sig_file);


    BN_free(P);
    BN_free(X);
    BN_free(G);
    BN_free(R);
    BN_free(XR);
    BN_free(U);
    BN_free(K);
    BN_free(K_inv);
    BN_free(P_minus_1);
    BN_free(S);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return 1;
}

int ElgamalCheckFileSignature(const char *file_path, long long Y_val, long long P_val, long long G_val, const char *signature_file) {
    BN_CTX *ctx = BN_CTX_new();
    unsigned char md[MD5_DIGEST_LENGTH];

    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }
    compute_md5_file(file, md);
    fclose(file);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);


    long long r, s;
    FILE *sig_file = fopen(signature_file, "r");
    if (!sig_file) {
        perror("Failed to open signature file");
        return 0;
    }
    fscanf(sig_file, "%lld\n%lld\n", &r, &s);
    fclose(sig_file);


    BIGNUM *P = BN_new();
    BIGNUM *Y = BN_new();
    BIGNUM *G = BN_new();
    BIGNUM *R = BN_new();
    BIGNUM *S = BN_new();

    BN_set_word(P, P_val);
    BN_set_word(Y, Y_val);
    BN_set_word(G, G_val);
    BN_set_word(R, r);
    BN_set_word(S, s);


    BIGNUM *G_hash = BN_new();
    BIGNUM *Yr = BN_new();
    BIGNUM *Rs = BN_new();
    BIGNUM *check = BN_new();

    BN_mod_exp(G_hash, G, hash_bn, P, ctx);
    BN_mod_exp(Yr, Y, R, P, ctx);
    BN_mod_exp(Rs, R, S, P, ctx);

    BN_mod_mul(check, Yr, Rs, P, ctx);

    int result = (BN_cmp(G_hash, check) == 0);


    BN_free(P);
    BN_free(Y);
    BN_free(G);
    BN_free(R);
    BN_free(S);
    BN_free(G_hash);
    BN_free(Yr);
    BN_free(Rs);
    BN_free(check);
    BN_free(hash_bn);
    BN_CTX_free(ctx);

    return result;
}