
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#define COUNT 30
// native + interpreterでの実装


int
nmpz_nextprime( char* _op_str, char* _rop_str);
int
nmpz_mod(char* _op1_str,  char* _op2_str , char* _rop_str);
int
nmpz_mul( char* _num1_str, char* _num2_str , char* _result_str);
int
nmpz_sub_ui(char* _op1_str,  unsigned long int _op2_str , char* _rop_str);
int
nmpz_invert(char* _num1_str, char* _num2_str , char* _result_str);
int
nmpz_gcd(char* _op1_str,  char* _op2_str , char* _rop_str);
int
nmpz_powm(char* _op1_str,  char* _op2_str ,char* _op3_str, char* _rop_str);

//RSA暗号処理部分
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


int wrapped_nmpz_nextprime(mpz_t rop, mpz_t op, unsigned int buf){
    char *tmp_rop = malloc(buf);
 
    nmpz_nextprime(mpz_get_str(NULL,10,op),tmp_rop); 
    mpz_set_str(rop, tmp_rop, 10);
    free(tmp_rop);

    return 0;
}
int wrapped_nmpz_mod(mpz_t rop,mpz_t op1,mpz_t op2, unsigned int buf){
    char *tmp_rop = malloc(buf);

    nmpz_mod(mpz_get_str(NULL,10,op1),mpz_get_str(NULL,10,op2),tmp_rop);
    mpz_set_str(rop,tmp_rop,10);
    free(tmp_rop);

    return 0;
}
int wrapped_nmpz_mul(mpz_t rop,mpz_t op1,mpz_t op2, unsigned int buf){
    char *tmp_rop = malloc(buf);
    
    nmpz_mul(mpz_get_str(NULL,10,op1),mpz_get_str(NULL,10,op2),tmp_rop);
    mpz_set_str(rop,tmp_rop,10);
    free(tmp_rop);

    return 0;
}
int wrapped_nmpz_sub_ui(mpz_t rop,mpz_t op1,unsigned long int op2, unsigned int buf){
    char *tmp_rop = malloc(buf);
    
    nmpz_sub_ui(mpz_get_str(NULL,10,op1),op2,tmp_rop);
    mpz_set_str(rop,tmp_rop,10);
    free(tmp_rop);

    return 0;
};

int wrapped_nmpz_invert(mpz_t rop,mpz_t op1,mpz_t op2, unsigned int buf){
    char *tmp_rop = malloc(buf);
    int res;
    res = nmpz_invert(mpz_get_str(NULL,10,op1),mpz_get_str(NULL,10,op2),tmp_rop);
    mpz_set_str(rop,tmp_rop,10);
    free(tmp_rop);

    return res;
}
int wrapped_nmpz_gcd(mpz_t rop,mpz_t op1,mpz_t op2, unsigned int buf){
    char *tmp_rop = malloc(buf);
    
    nmpz_gcd(mpz_get_str(NULL,10,op1),mpz_get_str(NULL,10,op2),tmp_rop);
    mpz_set_str(rop,tmp_rop,10);
    free(tmp_rop);

    return 0;
}
int wrapped_nmpz_powm(mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod, unsigned int buf){
    char *tmp_rop = malloc(buf);
    
    nmpz_powm(mpz_get_str(NULL,10,base),mpz_get_str(NULL,10,exp),mpz_get_str(NULL,10,mod),tmp_rop);
    mpz_set_str(rop,tmp_rop,10);
    free(tmp_rop);

    return 0;
}

void generate_keys(private_key* ku, public_key* kp)
{
    char buf[BUFFER_SIZE];
    int i;
    mpz_t phi; mpz_init(phi);
    mpz_t tmp1; mpz_init(tmp1);
    mpz_t tmp2; mpz_init(tmp2);
    
    // srand(time(NULL));
    srand(42);

    mpz_set_ui(ku->e, 3);
    
    for(i = 0; i < BUFFER_SIZE; i++){
        buf[i] = rand() % 0xFF;
    }
    buf[0] |= 0xC0;
    buf[BUFFER_SIZE - 1] |= 0x01;
    mpz_import(tmp1, BUFFER_SIZE, 1, sizeof(buf[0]), 0, 0, buf);

    // printf("tmp_op is : %s\n",mpz_get_str(NULL,10,tmp1));
    // gmp_printf("tmp1(gmp) is : %Zd\n", tmp1);
 
    wrapped_nmpz_nextprime(ku->p, tmp1, 2000);

    // mpz_mod(tmp2, ku->p, ku->e);
    wrapped_nmpz_mod(tmp2, ku->p, ku->e,2000);
    // gmp_printf("Value tmp2: %Zd\n", tmp2);

     while(!mpz_cmp_ui(tmp2, 1))         
    {
        wrapped_nmpz_nextprime(ku->p, ku->p, 2000);  /* so choose the next prime */
        wrapped_nmpz_mod(tmp2, ku->p, ku->e,2000);
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
        wrapped_nmpz_nextprime(ku->q, tmp1, 2000);
        wrapped_nmpz_mod(tmp2, ku->q, ku->e,2000);
        while(!mpz_cmp_ui(tmp2, 1))
        {
            wrapped_nmpz_nextprime(ku->q, ku->q,2000);
            wrapped_nmpz_mod(tmp2, ku->q, ku->e,2000);
        }
    } while(mpz_cmp(ku->p, ku->q) == 0); /* If we have identical primes (unlikely), try again */
    /* Calculate n = p x q */
    wrapped_nmpz_mul(ku->n, ku->p, ku->q,2000);

    // /* Compute phi(n) = (p-1)(q-1) */
    // // mpz_sub_ui(tmp1, ku->p, 1);
    wrapped_nmpz_sub_ui(tmp1, ku->p, 1, 2000);
    wrapped_nmpz_sub_ui(tmp2, ku->q, 1, 2000);
    wrapped_nmpz_mul(phi, tmp1, tmp2, 2000);

    /* Calculate d (multiplicative inverse of e mod phi) */
    if(wrapped_nmpz_invert(ku->d, ku->e, phi, 2000) == 0)
    {
        wrapped_nmpz_gcd(tmp1, ku->e, phi,2000);
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
    // mpz_powm(C, M, kp.e, kp.n); 
    wrapped_nmpz_powm(C, M, kp.e, kp.n, 2000);
    return;
}

void block_decrypt(mpz_t M, mpz_t C, private_key ku)
{
    // mpz_powm(M, C, ku.d, ku.n); 
    wrapped_nmpz_powm(M, C, ku.d, ku.n, 2000); 
    return;
}

int mul_test(){
    char* num1_str = "5";
    char* num2_str = "10";
    char *result_str;
    result_str = malloc(500);
    nmpz_mul(  num1_str, num2_str,result_str );
    
    printf("The result is: %s\n", result_str);
    free(result_str);
    return 0;
}

int wrapped_mul_test(){
    mpz_t tmp1; mpz_init(tmp1);
    mpz_t tmp2; mpz_init(tmp2);
    mpz_t rop; mpz_init(rop);

    mpz_set_ui(tmp1, 3);
    mpz_set_ui(tmp2, 3);

    wrapped_nmpz_mul(rop,tmp1,tmp2,2000);
    
    gmp_printf("Value rop: %Zd\n", rop);

    mpz_clear(tmp1);
    mpz_clear(tmp2);
    mpz_clear(rop);
    return 0;
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
   
    start_clock = clock(); /* 処理開始前のクロックを取得 */
    generate_keys(&ku, &kp);
    end_clock = clock(); /* 処理終了後のクロックを取得 */
    *generate_keys_clock=(double)(end_clock - start_clock) / CLOCKS_PER_SEC;

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
    // mpz_mod(M, M, kp.n);  // M が n より小さくなるようにする
    wrapped_nmpz_mod(M, M, kp.n,2000);

    start_clock = clock();
    block_encrypt(C, M, kp);
    end_clock = clock();
    *block_encrypt_clock=(double)(end_clock - start_clock) / CLOCKS_PER_SEC;

    start_clock = clock();
    block_decrypt(DC, C, ku);
    end_clock = clock();
    *block_decrypt_clock=(double)(end_clock - start_clock) / CLOCKS_PER_SEC;

    if(mpz_cmp(M,C) == 0){
        fprintf(stderr, "Error: Encryption failed\n");
    }
    if(mpz_cmp(M,DC)!= 0){
        fprintf(stderr, "Error: Decryption failed\n");
    }
    // printf("encrypted is [%s]\n", mpz_get_str(NULL, 16, C));
    // printf("original is [%s]\n", mpz_get_str(NULL, 16, M));
    // printf("decrypted is [%s]\n", mpz_get_str(NULL, 16, DC));

    return 0;
}

int main(int argc, char **argv){

    int count = COUNT;

    double generate_keys_clock;
    double block_encrypt_clock;
    double block_decrypt_clock;

    double generate_keys_clock_sum=0;
    double block_encrypt_clock_sum=0;
    double block_decrypt_clock_sum=0;

    for(int i=0;i<count;i++){
        RSA_TEST(&generate_keys_clock,&block_encrypt_clock,&block_decrypt_clock);
        printf("generate_keys_clock %d: %f\n",i,generate_keys_clock);
        printf("block_encrypt_clock %d: %f\n",i,block_encrypt_clock);
        printf("block_decrypt_clock %d: %f\n\n",i,block_decrypt_clock);
        generate_keys_clock_sum+=generate_keys_clock;
        block_encrypt_clock_sum+=block_encrypt_clock;
        block_decrypt_clock_sum+=block_decrypt_clock;
    }

    generate_keys_clock = generate_keys_clock_sum/count;
    block_encrypt_clock = block_encrypt_clock_sum/count;
    block_decrypt_clock = block_decrypt_clock_sum/count;

    printf("----------------------\n");
    printf("generate_keys_clock AVE: %f\n",generate_keys_clock);
    printf("block_encrypt_clock AVE: %f\n",block_encrypt_clock);
    printf("block_decrypt_clock AVE: %f\n",block_decrypt_clock);

    return 0;
}


