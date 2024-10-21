#include "myciphers.h"

// long long int shamir(){
//     long long int P =  getPrimeRand();

// }

void ElgamalGenPG(long long int *P, long long int *g)
{
    srand(time(NULL));
    *P = getPrimeRand();
    *g = getRnadg(*P);
    // printf("P=%lld G=%lld\n", *P, *g);
}

void ElgamalGenKeys(long long int *C, long long int *D, long long int P, long long int g)
{
    srand(time(NULL));
    *C = getRand(P - 1);
    *D = fastExp(g, *C, P);
    // printf("C=%lld D=%lld\n", *C, *D);
}

short ElgamalEncryptM(long long int D, long long int P, long long int g, char m, short *r)
{
    srand(time(NULL));
    long long int k = getRand(P - 2);
    *r = fastExp(g, k, P);
    short e = (m * fastExp(D, k, P)) % P;

    return e;
}

char ElgamalDecryptM(long long int C, long long int P, short e, short r)
{
    short r_inv = fastExp(r, P - 1 - C, P);
    char dm = (e * r_inv) % P;

    return dm;
}

void ElgamalEncryptMessage(long long int D, long long int P, long long int g, char *message, short *rmessage, short *emessage)
{
    int len = strlen(message);
    for (int i = 0; i < len; i++)
    {
        emessage[i] = ElgamalEncryptM(D, P, g, message[i], &rmessage[i]);
    }
}

void ElgamalDecryptMessage(long long int C, long long int P, short *emessage, short *rmessage, char *dmessage)
{
    int len = (sizeof(emessage) / sizeof(emessage[0]) + 1);
    for (int i = 0; i <= len; i++)
    {
        dmessage[i] = ElgamalDecryptM(C, P, emessage[i], rmessage[i]);
    }
}

void ElgamalEncryptFile(long long int D, long long int P, long long int g, const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        return;
    }

    char message;
    short rmessage;
    short emessage;

    while (fread(&message, sizeof(char), 1, input_file) == 1)
    {
        emessage = ElgamalEncryptM(D, P, g, message, &rmessage);
        fwrite(&emessage, sizeof(short), 1, output_file);
        fwrite(&rmessage, sizeof(short), 1, output_file); // Записываем r
    }

    fclose(input_file);
    fclose(output_file);
}

void ElgamalDecryptFile(long long int C, long long int P, const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        return;
    }

    short emessage;
    short rmessage;
    char dmessage;

    while (fread(&emessage, sizeof(short), 1, input_file) == 1 &&
           fread(&rmessage, sizeof(short), 1, input_file) == 1)
    {
        dmessage = ElgamalDecryptM(C, P, emessage, rmessage);
        fwrite(&dmessage, sizeof(char), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

void RSAGenParams(long long int *N, long long int *C, long long int *D)
{
    srand(time(NULL));
    long long int P = getPrimeRand(), Q = getPrimeRand();
    // long long int P = 3, Q = 11;
    while (P == Q)
    {
        Q = getPrimeRand();
    }

    *N = P * Q;
    long long int fi = (P - 1) * (Q - 1);
    do
    {
        *D = getRand(fi);
    } while (genEuclid(*D, fi) != 1);
    *C = modInverse(*D, fi);
}
long long int RSAEncryptM(long long int N, long long int M, long long int D)
{
    return fastExp(M, D, N);
}
long long int RSADecryptM(long long int N, long long int e, long long int C)
{
    return fastExp(e, C, N);
}
void RSAEncryptMessage(long long int N, long long int D, long long int *message, long long int *emessage)
{
    int len = (sizeof(message) / sizeof(message[0]) + 1);

    for (int i = 0; i < len; i++)
    {
        emessage[i] = RSAEncryptM(N, message[i], D);
    }
}

void RSADecryptMessage(long long int N, long long int C, long long int *emessage, long long int *dmessage)
{
    int len = (sizeof(emessage) / sizeof(emessage[0]) + 1);
    for (int i = 0; i <= len; i++)
    {
        dmessage[i] = RSADecryptM(N, emessage[i], C);
    }
}
void RSAEncryptFile(long long int N, long long int D, const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        return;
    }

    char message;
    long long int emessage;

    while (fread(&message, sizeof(char), 1, input_file) == 1)
    {
        emessage = RSAEncryptM(N, (long long int)message, D);
        fwrite(&emessage, sizeof(long long int), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

void RSADecryptFile(long long int N, long long int C, const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        return;
    }

    long long int emessage;
    char dmessage;

    while (fread(&emessage, sizeof(long long int), 1, input_file) == 1)
    {
        dmessage = (char)RSADecryptM(N, emessage, C);
        fwrite(&dmessage, sizeof(char), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

long long int ShamirGenP()
{
    srand(time(NULL));
    return getPrimeRand();
}

void ShamirGenCD(long long int P, long long int *C, long long int *D)
{
    srand(time(NULL));

    do
    {
        *C = getRand(10000);
    } while (genEuclid(*C, P - 1) != 1);

    *D = modInverse(*C, P - 1);
    // while (genEuclid(*D, P - 1) != 1)
    //     ;
    // {
    //     *C = getRand(10000);
    // }
}

long long int ShamirEncryptM(long long int P, long long int cdExp, long long int MX)
{

    return fastExp(MX, cdExp, P);
}
void ShamirEncryptMessage(long long int P, long long int cdExp, long long int *inMX, long long int *outMX)
{
    int len = (sizeof(inMX) / sizeof(inMX[0]) + 1);

    for (int i = 0; i < len; i++)
    {
        outMX[i] = ShamirEncryptM(P, cdExp, inMX[i]);
    }
}
void ShamirEncryptFile(long long int P, long long int cdExp, const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        return;
    }

    long long int message = 0;
    long long int emessage = 0;
    size_t bytes_read;

    
    while ((bytes_read = fread(&message, 1, sizeof(long long int), input_file)) > 0)
    {
        
        if (bytes_read < sizeof(long long int))
        {
            memset(((char *)&message) + bytes_read, 0, sizeof(long long int) - bytes_read);
        }
        emessage = ShamirEncryptM(P, cdExp, message);
        fwrite(&emessage, sizeof(long long int), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

void ShamirDecryptFile(long long int P, long long int cdExp, const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        return;
    }

    long long int emessage = 0;
    long long int dmessage = 0;
    size_t bytes_read;

    
    while ((bytes_read = fread(&emessage, sizeof(long long int), 1, input_file)) > 0)
    {
        dmessage = ShamirEncryptM(P, cdExp, emessage);
        fwrite(&dmessage, sizeof(long long int), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}
void VernamGenKarr(unsigned char *k)
{
    srand(time(NULL));
    int len = (sizeof(k) / sizeof(k) + 1);
    for (int i = 0; i < len; i++)
    {
        k[i] = getRand(255);
    }
}
unsigned char VernamEncryptM(unsigned char m, unsigned char k)
{
    return m ^ k;
}
unsigned char VernamDecryptM(unsigned char e, unsigned char k)
{
    return e ^ k;
}
void VernamEncryptMessage(unsigned char *m, unsigned char *k, unsigned char *e)
{
    int len = (sizeof(m) / sizeof(m) + 1);
    for (int i = 0; i < len; i++)
    {
        e[i] = VernamEncryptM(m[i], k[i]);
    }
}
void VernamDecryptMessage(unsigned char *e, unsigned char *k, unsigned char *dm)
{
    int len = (sizeof(e) / sizeof(e) + 1);
    for (int i = 0; i < len; i++)
    {
        dm[i] = VernamEncryptM(e[i], k[i]);
    }
}
void VernamEncryptFile(const char *input_filename, const char *key_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *key_file = fopen(key_filename, "wb");
    if (!key_file) {
        perror("Не удалось открыть файл ключа");
        fclose(input_file);
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        fclose(key_file);
        return;
    }

    srand(time(NULL)); 

    unsigned char byte, key_byte, encrypted_byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file) == 1) {
        key_byte = (unsigned char) (rand() % 256); 
        encrypted_byte = byte ^ key_byte; 
        fwrite(&key_byte, sizeof(unsigned char), 1, key_file); 
        fwrite(&encrypted_byte, sizeof(unsigned char), 1, output_file); 
    }

    fclose(input_file);
    fclose(key_file);
    fclose(output_file);
}
void VernamDecryptFile(const char *input_filename, const char *key_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Не удалось открыть входной файл");
        return;
    }

    FILE *key_file = fopen(key_filename, "rb");
    if (!key_file) {
        perror("Не удалось открыть файл ключа");
        fclose(input_file);
        return;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Не удалось открыть выходной файл");
        fclose(input_file);
        fclose(key_file);
        return;
    }

    unsigned char encrypted_byte, key_byte, decrypted_byte;
    while (fread(&encrypted_byte, sizeof(unsigned char), 1, input_file) == 1 &&
           fread(&key_byte, sizeof(unsigned char), 1, key_file) == 1) {
        decrypted_byte = encrypted_byte ^ key_byte; 
        fwrite(&decrypted_byte, sizeof(unsigned char), 1, output_file); 
    }

    fclose(input_file);
    fclose(key_file);
    fclose(output_file);
}