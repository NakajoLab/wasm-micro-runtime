#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>
// 暗号処理：wasmインタプリタ
// GMP：ネイティブ
#define MODULUS_SIZE 1024                   /* This is the number of bits we want in the modulus 1024 2048 4096 8192 */
#define BLOCK_SIZE (MODULUS_SIZE/8)         /* This is the size of a block that gets en/decrypted at once */
#define BUFFER_SIZE ((MODULUS_SIZE/8) / 2)  /* This is the number of bytes in n and p */



// mpz_tの定義
typedef unsigned long int	mp_limb_t;
typedef struct
{
  int _mp_alloc;		/* Number of *limbs* allocated and pointed
				   to by the _mp_d field.  */
  int _mp_size;			/* abs(_mp_size) is the number of limbs the
				   last field points to.  If _mp_size is
				   negative this is a negative number.  */
  mp_limb_t *_mp_d;		/* Pointer to the limbs.  */
} __mpz_struct;
typedef __mpz_struct mpz_t[1];

// mpz_ptrの定義
typedef __mpz_struct *mpz_ptr;


// GMPのネイティブライブラリ
int  
mpz_init (mpz_t x);
int
mpz_set_ui(mpz_ptr x, unsigned long int y);
int 
random_get(void *buf, unsigned int buf_len);

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


int
main(int argc, char **argv)
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

    // generate_keys(&ku, &kp);

    // rand();

    printf("res is success\n");

    return 0;
}