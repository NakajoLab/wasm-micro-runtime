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

// https://github.com/gilgad13/rsa-gmp

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "wasm_export.h"

#define MODULUS_SIZE 1024                   /* This is the number of bits we want in the modulus 1024 2048 4096 8192 */
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
    // Pick the next prime starting from that random number
    mpz_nextprime(ku->p, tmp1);
    /* Make sure this is a good choice*/
    mpz_mod(tmp2, ku->p, ku->e);        /* If p mod e == 1, gcd(phi, e) != 1 */
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

    return;
}

void block_encrypt(mpz_t C, mpz_t M, public_key kp)
{
    /* C = M^e mod n */
    mpz_powm(C, M, kp.e, kp.n); 
    return;
}

int encrypt(char cipher[], char message[], int length, public_key kp)
{
    /* Its probably overkill, but I implemented PKCS#1v1.5 paging
     * Encoded message block is of the form:
     * EMB = 00 || 02 || PS || 00 || D
     * Where || is concatenation, D is the message, and PS is a string of
     * (block_size-|D|-3) non-zero, randomly generated bytes
     * |D| must be less than block_size - 11, which means we have at least 8
     * bytes of PS
     */
    int block_count = 0;
    int prog = length;
    char mess_block[BLOCK_SIZE];
    mpz_t m; mpz_init(m);
    mpz_t c; mpz_init(c);

    while(prog > 0)
    {
        int i = 0;
        int d_len = (prog >= (BLOCK_SIZE - 11)) ? BLOCK_SIZE - 11 : prog;

        /* Construct the header */
        mess_block[i++] = 0x00;
        mess_block[i++] = 0x02;
        while(i < (BLOCK_SIZE - d_len - 1))
            mess_block[i++] = (rand() % (0xFF - 1)) + 1; 
        mess_block[i++] = 0x00;

        /* Copy in the message */
        memcpy(mess_block + i, message + (length - prog), d_len);
        
        // Convert bytestream to integer 
        mpz_import(m, BLOCK_SIZE, 1, sizeof(mess_block[0]), 0, 0, mess_block);
        // Perform encryption on that block
        block_encrypt(c, m, kp);

        // Calculate cipher write offset to take into account that we want to
        // pad with zeros in the front if the number we get back has fewer bits
        // than BLOCK_SIZE
        int off = block_count * BLOCK_SIZE;         // Base offset to start of this block
        off += (BLOCK_SIZE - (mpz_sizeinbase(c, 2) + 8 - 1)/8); // See manual for mpz_export
        
        // Pull out bytestream of ciphertext
        mpz_export(cipher + off, NULL, 1, sizeof(char), 0, 0, c);

        block_count++;
        prog -= d_len;
    }
    return block_count * BLOCK_SIZE;
} 

void block_decrypt(mpz_t M, mpz_t C, private_key ku)
{
    mpz_powm(M, C, ku.d, ku.n); 
    return;
}

int decrypt(char* message, char* cipher, int length, private_key ku)
{
    int msg_idx = 0;
    char buf[BLOCK_SIZE];
    *(long long*)buf = 0ll;
    mpz_t c; mpz_init(c);
    mpz_t m; mpz_init(m);

    int i;
    for(i = 0; i < (length / BLOCK_SIZE); i++)
    {
        // Pull block into mpz_t
        mpz_import(c, BLOCK_SIZE, 1, sizeof(char), 0, 0, cipher + i * BLOCK_SIZE);
        // Decrypt block
        block_decrypt(m, c, ku);

        // Calculate message write offset to take into account that we want to
        // pad with zeros in the front if the number we get back has fewer bits
        // than BLOCK_SIZE
        int off = (BLOCK_SIZE - (mpz_sizeinbase(m, 2) + 8 - 1)/8); // See manual for mpz_export
        // Convert back to bitstream
        mpz_export(buf + off, NULL, 1, sizeof(char), 0, 0, m);

        // Now we just need to lop off top padding before memcpy-ing to message
        // We know the first 2 bytes are 0x00 and 0x02, so manually skip those
        // After that, increment forward till we see a zero byte
        int j;
        for(j = 2; ((buf[j] != 0) && (j < BLOCK_SIZE)); j++);
        j++;        // Skip the 00 byte

        /* Copy over the message part of the plaintext to the message return var */
        memcpy(message + msg_idx, buf + j, BLOCK_SIZE - j);

        msg_idx += BLOCK_SIZE - j; 
    } 
    return msg_idx;
}

int main_rsa()
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
    /* 処理開始前のクロックを取得 */
    start_clock = clock();
    generate_keys(&ku, &kp);
    /* 処理終了後のクロックを取得 */
    end_clock = clock();

    /* 計測時間を表示 */
    printf(
        "clock generate_keys:%f\n", (double)(end_clock - start_clock) / CLOCKS_PER_SEC
    );
    // printf("---------------Private Key-----------------");
    // printf("kp.n is [%s]\n", mpz_get_str(NULL, 16, kp.n));
    // printf("kp.e is [%s]\n", mpz_get_str(NULL, 16, kp.e));
    // printf("---------------Public Key------------------");
    // printf("ku.n is [%s]\n", mpz_get_str(NULL, 16, ku.n));
    // printf("ku.e is [%s]\n", mpz_get_str(NULL, 16, ku.e));
    // printf("ku.d is [%s]\n", mpz_get_str(NULL, 16, ku.d));
    // printf("ku.p is [%s]\n", mpz_get_str(NULL, 16, ku.p));
    // printf("ku.q is [%s]\n", mpz_get_str(NULL, 16, ku.q));

    char buf[6*BLOCK_SIZE]; 
    for(i = 0; i < 6*BLOCK_SIZE; i++)
        buf[i] = rand() % 0xFF;

    mpz_import(M, (6*BLOCK_SIZE), 1, sizeof(buf[0]), 0, 0, buf);
    // printf("original is [%s]\n", mpz_get_str(NULL, 16, M)); 
    block_encrypt(C, M, kp);
    // printf("encrypted is [%s]\n", mpz_get_str(NULL, 16, C));
    block_decrypt(DC, C, ku);
    // printf("decrypted is [%s]\n", mpz_get_str(NULL, 16, DC));
    return 0;
}

static int
test_rsa_wrapper(wasm_exec_env_t exec_env)
{
    main_rsa();
    return 0;
}

/* clang-format off */
#define REG_NATIVE_FUNC(func_name, signature) \
    { #func_name, func_name##_wrapper, signature, NULL }

static NativeSymbol native_symbols[] = {
    REG_NATIVE_FUNC(test_rsa, "()i")
};
/* clang-format on */

uint32_t
get_native_lib(char **p_module_name, NativeSymbol **p_native_symbols)
{
    *p_module_name = "env";
    *p_native_symbols = native_symbols;
    return sizeof(native_symbols) / sizeof(NativeSymbol);
}