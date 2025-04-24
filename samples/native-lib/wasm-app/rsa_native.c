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
wrapped_nmpz_mul(mpz_t rop, mpz_t op1, mpz_t op2)
{
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + abs(op2->_mp_size) + 1;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_mul(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    return 0;
}

#include <time.h>
int main() {
    gmp_randstate_t state;
    gmp_randinit_default(state);  // 乱数生成器の初期化
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));  
    // *計算処理
    for(int i=0; i < 1000; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1);
        mpz_init(op2);
        mpz_init(op3);
        mpz_init(op4);
        mpz_init(rop);
        mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        mpz_set(op3, op1);
        mpz_set(op4, op2);

        wrapped_nmpz_mul(rop, op1, op2);
        mpz_mul(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[rop->_mp_size-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力
            
        }
        mpz_clear(op1);
        mpz_clear(op2);
        mpz_clear(op3);
        mpz_clear(op4);
        mpz_clear(rop);
        mpz_clear(rt);
    }

    gmp_randclear(state);

    return 0;
}




int main_bk() {
    
    // 1. 初期値を設定　4294967295
    mpz_t op1, op2, rop;
    mpz_t t1, t2, rt;
    mpz_init_set_str(op1, "4294967295", 10);
    mpz_init_set_str(op2, "1000000000000000000000000000", 10);
    mpz_init_set_str(rop, "0", 10);

    // *計算処理
    wrapped_nmpz_mul(rop,op1, op2);
    gmp_printf("  %Zd * %Zd =  %Zd(native call)\n", op1, op2, rop);

    
    // printf(" (rop->_mp_size) :  %d\n", rop->_mp_size);
    // gmp_printf("  %Zd * %Zd =  %Zd\n", op1, op2, t);
    
    printf("wasm size limb: %d\n", mp_bits_per_limb);
    printf("GMP LIMB BITS = %d\n", GMP_LIMB_BITS);

    mpz_clear(op1);
    mpz_clear(op2);
    mpz_clear(rop);

    return 0;
}
