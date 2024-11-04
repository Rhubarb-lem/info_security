#include "mysigns.h"

// Генерация ключей ГОСТ
int GOSTGenerateKeys(BIGNUM *private_key, BIGNUM *public_key) {
    // Создание группы для ГОСТ
    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_id_GostR3410_94_CryptoPro_A_ParamSet);
    if (!group) {
        fprintf(stderr, "Ошибка при создании группы для ГОСТ\n");
        return 0;
    }

    // Создание нового EC_KEY объекта
    EC_KEY *eckey = EC_KEY_new();
    if (!eckey) {
        fprintf(stderr, "Ошибка при создании EC_KEY\n");
        EC_GROUP_free(group);
        return 0;
    }

    // Установка группы для ключа
    EC_KEY_set_group(eckey, group);

    // Генерация ключа
    if (EC_KEY_generate_key(eckey) != 1) {
        fprintf(stderr, "Ошибка при генерации ключа\n");
        EC_KEY_free(eckey);
        EC_GROUP_free(group);
        return 0;
    }

    // Получение закрытого и открытого ключей
    const BIGNUM *priv_key_bn = EC_KEY_get0_private_key(eckey);
    const EC_POINT *pub_key_point = EC_KEY_get0_public_key(eckey);

    // Копирование ключей в переданные переменные
    BN_copy(private_key, priv_key_bn);
    EC_POINT_point2bn(group, pub_key_point, POINT_CONVERSION_UNCOMPRESSED, public_key, NULL);

    // Очистка
    EC_KEY_free(eckey);
    EC_GROUP_free(group);

    return 1;
}

// Генерация хэша ГОСТ для сообщения
void compute_gost_hash(const char *message, unsigned char *result)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_get_digestbyname("md_gost94");
    if (!md)
    {
        fprintf(stderr, "Не удалось найти хеш-функцию ГОСТ\n");
        return;
    }

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, message, strlen(message));
    EVP_DigestFinal_ex(mdctx, result, NULL);
    EVP_MD_CTX_free(mdctx);
}

// Функция подписания сообщения с помощью ГОСТ Р 34.10-94
int GOSTSign(const char *message, BIGNUM *private_key, BIGNUM *public_key, BIGNUM *signature_r, BIGNUM *signature_s)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    compute_gost_hash(message, hash);

    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_id_GostR3410_94_CryptoPro_A_ParamSet);
    EC_POINT *Q = EC_POINT_new(group);
    EC_KEY *eckey = EC_KEY_new();

    EC_KEY_set_group(eckey, group);
    EC_KEY_set_private_key(eckey, private_key);
    EC_KEY_set_public_key(eckey, Q);

    ECDSA_SIG *sig = ECDSA_do_sign(hash, sizeof(hash), eckey);
    if (!sig)
    {
        fprintf(stderr, "Ошибка подписи\n");
        return 0;
    }

    const BIGNUM *r = ECDSA_SIG_get0_r(sig);
    const BIGNUM *s = ECDSA_SIG_get0_s(sig);
    BN_copy(signature_r, r);
    BN_copy(signature_s, s);

    ECDSA_SIG_free(sig);
    EC_KEY_free(eckey);
    EC_POINT_free(Q);
    EC_GROUP_free(group);

    return 1;
}

// Функция проверки подписи ГОСТ
int GOSTVerify(const char *message, BIGNUM *public_key, BIGNUM *signature_r, BIGNUM *signature_s)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    compute_gost_hash(message, hash);

    EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_id_GostR3410_94_CryptoPro_A_ParamSet);
    EC_KEY *eckey = EC_KEY_new();
    EC_POINT *Q = EC_POINT_new(group);

    EC_KEY_set_group(eckey, group);
    EC_POINT_bn2point(group, public_key, Q, NULL);
    EC_KEY_set_public_key(eckey, Q);

    ECDSA_SIG *sig = ECDSA_SIG_new();
    ECDSA_SIG_set0(sig, BN_dup(signature_r), BN_dup(signature_s));

    int result = ECDSA_do_verify(hash, sizeof(hash), sig, eckey);

    ECDSA_SIG_free(sig);
    EC_KEY_free(eckey);
    EC_POINT_free(Q);
    EC_GROUP_free(group);

    return result;
}

// Подпись файла ГОСТ
int GOSTSignFile(const char *file_path, BIGNUM *private_key, BIGNUM *public_key, const char *signature_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Не удалось открыть файл");
        return 0;
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    compute_md5_file(file, hash);
    fclose(file);

    BIGNUM *signature_r = BN_new();
    BIGNUM *signature_s = BN_new();
    if (!GOSTSign((const char *)hash, private_key, public_key, signature_r, signature_s))
    {
        return 0;
    }

    FILE *sig_file = fopen(signature_path, "w");
    if (!sig_file)
    {
        perror("Не удалось открыть файл подписи");
        return 0;
    }

    char *r_str = BN_bn2dec(signature_r);
    char *s_str = BN_bn2dec(signature_s);
    fprintf(sig_file, "%s\n%s\n", r_str, s_str);

    OPENSSL_free(r_str);
    OPENSSL_free(s_str);
    fclose(sig_file);
    BN_free(signature_r);
    BN_free(signature_s);

    return 1;
}

// Проверка подписи файла ГОСТ
int GOSTVerifyFile(const char *file_path, BIGNUM *public_key, const char *signature_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Не удалось открыть файл");
        return 0;
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    compute_md5_file(file, hash);
    fclose(file);

    BIGNUM *signature_r = BN_new();
    BIGNUM *signature_s = BN_new();

    FILE *sig_file = fopen(signature_path, "r");
    if (!sig_file)
    {
        perror("Не удалось открыть файл подписи");
        return 0;
    }

    char r_str[64], s_str[64];
    fscanf(sig_file, "%63s\n%63s\n", r_str, s_str);
    fclose(sig_file);

    BN_dec2bn(&signature_r, r_str);
    BN_dec2bn(&signature_s, s_str);

    int result = GOSTVerify((const char *)hash, public_key, signature_r, signature_s);

    BN_free(signature_r);
    BN_free(signature_s);

    return result;
}

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

int ElgamalSignFile(const char *file_path, long long X_val, long long P_val, long long G_val, const char *signature_file)
{
    BN_CTX *ctx = BN_CTX_new();
    unsigned char md[MD5_DIGEST_LENGTH];

    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
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
    do
    {
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
    long long s = atoll(s_str);
    OPENSSL_free(s_str);

    FILE *sig_file = fopen(signature_file, "w");
    if (!sig_file)
    {
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

int ElgamalCheckFileSignature(const char *file_path, long long Y_val, long long P_val, long long G_val, const char *signature_file)
{
    BN_CTX *ctx = BN_CTX_new();
    unsigned char md[MD5_DIGEST_LENGTH];

    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Failed to open file");
        return 0;
    }
    compute_md5_file(file, md);
    fclose(file);

    BIGNUM *hash_bn = BN_new();
    BN_bin2bn(md, MD5_DIGEST_LENGTH, hash_bn);

    long long r, s;
    FILE *sig_file = fopen(signature_file, "r");
    if (!sig_file)
    {
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