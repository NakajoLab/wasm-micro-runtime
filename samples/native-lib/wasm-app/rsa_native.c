//これはテスト用

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <stdint.h>

#include <math.h>
#include <time.h>

int nmpz_mul( int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);
int nmpz_add(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
  int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
  int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);
int nmpz_mod(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);
int nmpz_gcd(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);
int nmpz_invert(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);
int nmpz_nextprime( int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1);
int nmpz_powm(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2,
    int _mp_alloc_3, int _mp_size_3, unsigned long int *_mp_d_3);
int nmpz_sub_ui(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    unsigned long int _mp_d_2);

int wrapped_nmpz_mul(mpz_t rop, mpz_t op1, mpz_t op2)
{
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + abs(op2->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_mul(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    return 0;
}

int wrapped_nmpz_add(mpz_t rop, mpz_t op1, mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
      mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }
  
    int alloc_size = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);
    
    nmpz_add(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
  
    return 0;
  }

  int wrapped_nmpz_mod(mpz_t rop, mpz_t op1, mpz_t op2)
  {
      if(op1->_mp_alloc % 2 != 0){
          mpz_realloc(op1, op1->_mp_alloc+1);
      }
      if(op2->_mp_alloc % 2 != 0){
          mpz_realloc(op2, op2->_mp_alloc+1);
      }
  
      int alloc_size = abs(op2->_mp_size) + 2;
      if(alloc_size  % 2 != 0){
          alloc_size++;
      }
      mpz_realloc(rop, alloc_size);
  
      nmpz_mod(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
          op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
          op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
      
      return 0;
  }

int wrapped_nmpz_gcd(mpz_t rop,mpz_t op1,mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_gcd(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    return 0;
  }

int wrapped_nmpz_invert(mpz_t rop, mpz_t op1, mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = abs(op2->_mp_size) + 1 + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_invert(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    return 0;
  }

int wrapped_nmpz_nextprime(mpz_t rop, mpz_t op1){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + abs(op1->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_nextprime(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d);
    
    return 0;
}

int wrapped_nmpz_powm(mpz_t rop,  mpz_t op1,  mpz_t op2,  mpz_t op3){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }
    if(op3->_mp_alloc % 2 != 0){
        mpz_realloc(op3, op3->_mp_alloc+1);
    }

    int alloc_size = abs(op3->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_powm(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d,
        op3->_mp_alloc, op3->_mp_size, op3->_mp_d);
    
    return 0;
}

int wrapped_nmpz_sub_ui(mpz_t rop,  mpz_t op1,  unsigned long int op2){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_sub_ui(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2
    );
    
    return 0;
}



int MulCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 1000; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        if (rand() % 2) {
            mpz_neg(op2, op2);
        }
        mpz_set(op3, op1);
        mpz_set(op4, op2);

        wrapped_nmpz_mul(rop, op1, op2);
        mpz_mul(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("mul check ok\n");
    return 0;
}

int AddCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 1000; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000 ;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        if (rand() % 2) {
            mpz_neg(op2, op2);
        }
        mpz_set(op3, op1);
        mpz_set(op4, op2);
    
        wrapped_nmpz_add(rop, op1, op2);
        mpz_add(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("add check ok\n");
    return 0;
}

int ModCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 1000; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        
        if (mpz_cmp_ui(op2, 0) == 0) {
            mpz_set_ui(op2, 1);  
        }
        mpz_set(op3, op1);
        mpz_set(op4, op2);
        
        wrapped_nmpz_mod(rop, op1, op2);
        mpz_mod(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("mod check ok\n");
    return 0;
}

int GcdCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 1000; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        if (rand() % 2) {
            mpz_neg(op2, op2);
        }
        mpz_set(op3, op1);
        mpz_set(op4, op2);

        wrapped_nmpz_gcd(rop, op1, op2);
        mpz_gcd(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("gcd check ok\n");
    return 0;
}

int InvertCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 1000; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        
        if (mpz_cmp_ui(op2, 0) == 0) {
            mpz_set_ui(op2, 1);  
        }
        mpz_set(op3, op1);
        mpz_set(op4, op2);

        wrapped_nmpz_invert(rop, op1, op2);
        mpz_invert(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("invert check ok\n");
    return 0;
}

int NextprimeCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 100; i++){
        mpz_t op1, op2, rop;
        mpz_t op3, op4, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 100;
        int rand2 = rand() % 100;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        if (rand() % 2) {
            mpz_neg(op2, op2);
        }
        mpz_set(op3, op1);
        mpz_set(op4, op2);

        wrapped_nmpz_nextprime(rop, op1);
        mpz_nextprime(rt, op3);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("nextprime check ok\n");
    return 0;
}

int PowmCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 100; i++){
        mpz_t op1, op2, op3, rop;
        mpz_t op4, op5, op6, rt;
        mpz_init(op1); mpz_init(op2); mpz_init(op3); mpz_init(op4); mpz_init(op5); mpz_init(op6); 
        mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 1000;
        int rand2 = rand() % 1000;
        int rand3 = rand() % 1000;
        mpz_urandomb(op1, state, rand1);
        mpz_urandomb(op2, state, rand2);
        mpz_urandomb(op3, state, rand3);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        
        if (mpz_cmp_ui(op3, 0) == 0) {
            mpz_set_ui(op3, 1);  
        }
        mpz_set(op4, op1);
        mpz_set(op5, op2);
        mpz_set(op6, op3);

        wrapped_nmpz_powm(rop, op1, op2, op3);
        mpz_powm(rt, op4, op5 , op6);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            gmp_printf("wasm op2 =%Zd\n", op2);
            gmp_printf("wasm op4 =%Zd\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力 
            return -1;
        }
        mpz_clear(op1); mpz_clear(op2); mpz_clear(op3); mpz_clear(op4); mpz_clear(op5);mpz_clear(op6);mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("powm check ok\n");
    return 0;
}

int SubuiCheck(){
    gmp_randstate_t state;
    gmp_randinit_default(state);  
    gmp_randseed_ui(state, time(NULL));  
    srand(time(NULL));
    
    for(int i=0; i < 100; i++){
        mpz_t op1, rop;
        unsigned long int op2;
        mpz_t op3, rt;
        unsigned long int op4;
        mpz_init(op1);  mpz_init(op3);  mpz_init(rop); mpz_init(rt);

        int rand1 = rand() % 100;
        
        mpz_urandomb(op1, state, rand1);
        if (rand() % 2) {
            mpz_neg(op1, op1);
        }
        mpz_set(op3, op1);
        op2 = ((unsigned long int)rand() << 14) | rand();
        op4 = op2;

        wrapped_nmpz_sub_ui(rop, op1, op2);
        mpz_sub_ui(rt, op3, op4);
        
        int alloc_tmp = rt->_mp_alloc;
        int size_tmp = rt->_mp_size;
        
        mpz_realloc(rt, rop->_mp_alloc);
        if(rt->_mp_size != rop->_mp_size){
            rt->_mp_size = rop->_mp_size;
            rt->_mp_d[abs(rop->_mp_size)-1] = 0;
        }

        if(mpz_cmp(rop, rt) != 0){
            gmp_printf("wasm rop =%Zd\n", rop);
            gmp_printf("wasm rt  =%Zd\n", rt);
            gmp_printf("wasm op1 =%Zd\n", op1);
            gmp_printf("wasm op3 =%Zd\n", op3);
            printf("wasm op2 =%lu\n", op2);
            printf("wasm op4 =%lu\n", op4);
            printf("\x1b[31merror!\x1b[0m\n");// 赤文字で出力
            return -1;
        }
        mpz_clear(op1);  mpz_clear(op3); mpz_clear(rop); mpz_clear(rt);
    }
    gmp_randclear(state);
    printf("sub ui check ok\n");
    return 0;
}

int main() {
    // 乗算テスト
    MulCheck();
    // 加算テスト
    AddCheck();
    // 除算テスト
    ModCheck();
    // 最大公約数テスト
    GcdCheck();
    // 逆元テスト
    InvertCheck();
    // 素数テスト
    NextprimeCheck();
    // 指数剰余テスト
    PowmCheck();
    // 減算テスト
    SubuiCheck();
    return 0;
}