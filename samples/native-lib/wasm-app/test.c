/**********************************************************************
 *                                                                    *
 * Created by Adam Brockett                                           *
 *                                                                    *
 * Copyright (c) 2010                                                 *
 *                                                                    *
 * Redistribution and use in source and binary forms, with or without *
 * modification is allowed.                                           *
 *                                                                    *
 * But if you let me know you're using my code, that would be freaking*
 * sweet.                                                             *
 *                                                                    *
 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#define COUNT 5

// interpreterでの実装

#define MODULUS_SIZE 4096                   /* This is the number of bits we want in the modulus 1024 2048 4096 8192 */
#define BLOCK_SIZE (MODULUS_SIZE/8)         /* This is the size of a block that gets en/decrypted at once */
#define BUFFER_SIZE ((MODULUS_SIZE/8) / 2)  /* This is the number of bytes in n and p */

typedef struct {
    mpz_t n; /* Modulus */
    mpz_t e; /* Public Exponent */
} public_key;

typedef struct {
    mpz_t n; /* Modulus */
    mpz_t e; /* Public Exponent */
    mpz_t d; /* Private Exponent */
    mpz_t p; /* Starting prime p */
    mpz_t q; /* Starting prime q */
} private_key;

void print_hex(char* arr, int len)
{
    int i;
    for(i = 0; i < len; i++)
        printf("%02x", (unsigned char) arr[i]); 
}

/* NOTE: Assumes mpz_t's are initted in ku and kp */
void generate_keys(private_key* ku, public_key* kp)
{
    char buf[BUFFER_SIZE];
    int i;
    mpz_t phi; mpz_init(phi);
    mpz_t tmp1; mpz_init(tmp1);
    mpz_t tmp2; mpz_init(tmp2);

    // srand(time(NULL));
    srand(42);

    /* Insetead of selecting e st. gcd(phi, e) = 1; 1 < e < phi, lets choose e
     * first then pick p,q st. gcd(e, p-1) = gcd(e, q-1) = 1 */
    // We'll set e globally.  I've seen suggestions to use primes like 3, 17 or 
    // 65537, as they make coming calculations faster.  Lets use 3.
    mpz_set_ui(ku->e, 3); 
    // gmp_printf("Value ku->e: %Zd\n", ku->e);
    /* Select p and q */
    /* Start with p */
    // Set the bits of tmp randomly
    for(i = 0; i < BUFFER_SIZE; i++)
        buf[i] = rand() % 0xFF; 
    // Set the top two bits to 1 to ensure int(tmp) is relatively large
    buf[0] |= 0xC0;
    // Set the bottom bit to 1 to ensure int(tmp) is odd (better for finding primes)
    buf[BUFFER_SIZE - 1] |= 0x01;
    // Interpret this char buffer as an int
    mpz_import(tmp1, BUFFER_SIZE, 1, sizeof(buf[0]), 0, 0, buf);
    // gmp_printf("Value tmp1: %Zd\n", tmp1);
    // Pick the next prime starting from that random number
    mpz_nextprime(ku->p, tmp1);
    // gmp_printf("Value ku->p: %Zd\n", ku->p);
    /* Make sure this is a good choice*/
    mpz_mod(tmp2, ku->p, ku->e);        /* If p mod e == 1, gcd(phi, e) != 1 */
    // gmp_printf("Value tmp2: %Zd\n", tmp2);
    while(!mpz_cmp_ui(tmp2, 1))         
    {
        mpz_nextprime(ku->p, ku->p);    /* so choose the next prime */
        mpz_mod(tmp2, ku->p, ku->e);
    }

    /* Now select q */
    do {
        for(i = 0; i < BUFFER_SIZE; i++)
            buf[i] = rand() % 0xFF; 
        // Set the top two bits to 1 to ensure int(tmp) is relatively large
        buf[0] |= 0xC0;
        // Set the bottom bit to 1 to ensure int(tmp) is odd
        buf[BUFFER_SIZE - 1] |= 0x01;
        // Interpret this char buffer as an int
        mpz_import(tmp1, (BUFFER_SIZE), 1, sizeof(buf[0]), 0, 0, buf);
        // Pick the next prime starting from that random number
        mpz_nextprime(ku->q, tmp1);
        mpz_mod(tmp2, ku->q, ku->e);
        while(!mpz_cmp_ui(tmp2, 1))
        {
            mpz_nextprime(ku->q, ku->q);
            mpz_mod(tmp2, ku->q, ku->e);
        }
    } while(mpz_cmp(ku->p, ku->q) == 0); /* If we have identical primes (unlikely), try again */

    /* Calculate n = p x q */
    mpz_mul(ku->n, ku->p, ku->q);

    /* Compute phi(n) = (p-1)(q-1) */
    mpz_sub_ui(tmp1, ku->p, 1);
    mpz_sub_ui(tmp2, ku->q, 1);
    mpz_mul(phi, tmp1, tmp2);

    /* Calculate d (multiplicative inverse of e mod phi) */
    if(mpz_invert(ku->d, ku->e, phi) == 0)
    {
        mpz_gcd(tmp1, ku->e, phi);
        printf("gcd(e, phi) = [%s]\n", mpz_get_str(NULL, 16, tmp1));
        printf("Invert failed\n");
    }

    /* Set public key */
    mpz_set(kp->e, ku->e);
    mpz_set(kp->n, ku->n);

    /* メモリの解放 */
    mpz_clear(phi);
    mpz_clear(tmp1);
    mpz_clear(tmp2);

    return;
}

void block_encrypt(mpz_t C, mpz_t M, public_key kp)
{
    /* C = M^e mod n */
    mpz_powm(C, M, kp.e, kp.n); 
    return;
}

void block_decrypt(mpz_t M, mpz_t C, private_key ku)
{
    mpz_powm(M, C, ku.d, ku.n); 
    return;
}
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  
    return ts.tv_sec + ts.tv_nsec / 1e9;  // 秒単位の浮動小数点数に変換
}

int
RSA_TEST(double *generate_keys_clock, double *block_encrypt_clock, double *block_decrypt_clock)
{
    int i;
    mpz_t M;  mpz_init(M);
    mpz_t C;  mpz_init(C);
    mpz_t DC;  mpz_init(DC);
    private_key ku;
    public_key kp;

    // Initialize public key
    mpz_init(kp.n);
    mpz_init(kp.e); 
    // Initialize private key
    mpz_init(ku.n); 
    mpz_init(ku.e); 
    mpz_init(ku.d); 
    mpz_init(ku.p); 
    mpz_init(ku.q); 

    clock_t start_clock, end_clock;
    
    double start_time = get_time(); /* 処理開始前のクロックを取得 */
    generate_keys(&ku, &kp);
    double end_time = get_time(); /* 処理終了後のクロックを取得 */
    *generate_keys_clock = end_time - start_time;

    // 0≤M<nの条件に違反するため以下のコードは不可．
    // char buf[6*BLOCK_SIZE]; 
    // for(i = 0; i < 6*BLOCK_SIZE; i++)
    //     buf[i] = rand() % 0xFF;
    // mpz_import(M, (6*BLOCK_SIZE), 1, sizeof(buf[0]), 0, 0, buf);

    size_t n_size = (mpz_sizeinbase(kp.n, 2) - 1) / 8;  // n のビット長をバイトに変換
    char buf[n_size]; // `n` のサイズに合わせたバッファ
    for(i = 0; i < n_size; i++){
        buf[i] = rand() % 0xFF;
    }
    mpz_import(M, n_size, 1, sizeof(buf[0]), 0, 0, buf);
    mpz_mod(M, M, kp.n);  // M が n より小さくなるようにする
    // mpz_set_ui(M, 500);

    start_time = get_time();
    block_encrypt(C, M, kp);
    end_time = get_time();
    *block_encrypt_clock = end_time - start_time;

    start_time = get_time();
    block_decrypt(DC, C, ku);
    end_time = get_time();
    *block_decrypt_clock = end_time - start_time;

    if(mpz_cmp(M,C) == 0){
        fprintf(stderr, "Error: Encryption failed\n");
    }
    if(mpz_cmp(M,DC)!= 0){
        fprintf(stderr, "Error: Decryption failed\n");
    }
    // printf("encrypted is [%s]\n", mpz_get_str(NULL, 16, C));
    // printf("original is [%s]\n", mpz_get_str(NULL, 16, M));
    // printf("decrypted is [%s]\n", mpz_get_str(NULL, 16, DC));

    mpz_clears(M, C, DC, kp.n, kp.e, ku.n, ku.e, ku.d, ku.p, ku.q, NULL);

    return 0;
}

int main(int argc, char **argv){
    int count = COUNT;
    double generate_keys_clock, block_encrypt_clock, block_decrypt_clock;
    double generate_keys_clock_sum = 0, block_encrypt_clock_sum = 0, block_decrypt_clock_sum = 0;

    for (int i = 0; i < count; i++) {
        RSA_TEST(&generate_keys_clock, &block_encrypt_clock, &block_decrypt_clock);
        printf("generate_keys_clock %d: %f\n", i, generate_keys_clock);
        printf("block_encrypt_clock %d: %f\n", i, block_encrypt_clock);
        printf("block_decrypt_clock %d: %f\n\n", i, block_decrypt_clock);
        generate_keys_clock_sum += generate_keys_clock;
        block_encrypt_clock_sum += block_encrypt_clock;
        block_decrypt_clock_sum += block_decrypt_clock;
    }

    printf("----------------------\n");
    printf("generate_keys_clock AVE: %f\n", generate_keys_clock_sum / count);
    printf("block_encrypt_clock AVE: %f\n", block_encrypt_clock_sum / count);
    printf("block_decrypt_clock AVE: %f\n", block_decrypt_clock_sum / count);

    return 0;
}
