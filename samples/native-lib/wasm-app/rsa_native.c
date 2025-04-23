//これはテスト用

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <stdint.h>

int
nmpz_mul( int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int
wrapped_nmpz_mul(mpz_t op1, mpz_t op2)
{
    mpz_t rop;
    mpz_init(rop);

    printf("(wasm rop) %d \n", rop->_mp_alloc);
    printf("(wasm rop) %d \n", rop->_mp_size);
    gmp_printf(" (wasm rop: result) :  %Zd\n\n", rop);
    
    printf("(wasm op2) %d \n", op2->_mp_alloc);
    printf("(wasm op2) %d \n\n", op2->_mp_size);
    // printf("(wasm) %p \n", op1->_mp_d);
    // gmp_printf(" (native) value:  %Zd\n", op1);
    
    // limbのサイズを計算
    // mpz_realloc2()
    // int limb_size = op1->_mp_size + op2->_mp_size;
    // limb 32ビット
    // rop->_mp_alloc =1;
    // rop->_mp_size = 0;
    mpz_realloc(rop, 0);
    printf("wasm : mp_d rop : %p \n", rop->_mp_d);
    // 符号
    // if(mpz_cmp_ui(op1,0) || mpz_cmp_ui(op2,0)){
    //     rop->_mp_alloc = 0;
    //     rop->_mp_size = 0;
    //     mpz_set_ui(rop, 0);
    // }
    // if(op1->_mp_size < 0 && op2->_mp_size > 0){
    //     rop->_mp_size = -rop->_mp_size;
    // }
        
    // rop->_mp_d = (mp_limb_t *)malloc(sizeof(mp_limb_t) * rop->_mp_alloc);

    nmpz_mul(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    printf("(wasm rop: _mp_alloc) %d \n", rop->_mp_alloc);
    printf("(wasm rop: _mp_size) %d \n", rop->_mp_size);
    gmp_printf(" (wasm rop: result) :  %Zd\n\n", rop);

    // 1 limb の最大値（unsigned の最大値）
    // mp_limb_t max_val = (mp_limb_t)(-1);  // すべてのビットが 1
    // printf("Max value of 1 limb = %llu\n", (unsigned long long)max_val);

    return 0;
}

int main() {
    // 1. 初期値を設定
    mpz_t op1, op2, rop;
    mpz_init_set_str(op1, "4294967295", 10);
    mpz_init_set_str(op2, "1", 10);
    mpz_init_set_str(rop, "0", 10);

    // *計算処理
    wrapped_nmpz_mul(op1,op2);

    mpz_mul(rop, op1, op2);
    gmp_printf("  %Zd * %Zd =  %Zd\n", op1, op2, rop);
    
    mpz_clear(op1);
    mpz_clear(op2);
    mpz_clear(rop);

    return 0;
}
