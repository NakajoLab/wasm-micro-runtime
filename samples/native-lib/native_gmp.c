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
#include <gmp.h>

#include "wasm_export.h"

// ネイティブライブラリの登録
static int
nmpz_mul_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2)
{
    mpz_t op1,op2,rop;   
    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);

    op1->_mp_alloc = (_mp_alloc_1)/2;
    op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
    op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);

    op2->_mp_alloc = (_mp_alloc_2)/2;
    op2->_mp_size = (_mp_size_2 < 0) ? -(-_mp_size_2+1)/2 : (_mp_size_2+1)/2;
    op2->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
    
    if(abs(_mp_size_1)%2 != 0){
        op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    if(abs(_mp_size_2)%2 != 0){
        op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    
    int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
    rop-> _mp_alloc = alloc/2;
    rop->_mp_size = 0;
    rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
    
    mpz_mul(rop,op1,op2);
    
    int rop_size = rop->_mp_size;
    *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;
    
    return 0;
}

static int
nmpz_add_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2)
{
    mpz_t op1,op2,rop;   
    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    
    op1->_mp_alloc = (_mp_alloc_1)/2;
    op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
    op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);

    op2->_mp_alloc = (_mp_alloc_2)/2;
    op2->_mp_size = (_mp_size_2 < 0) ? -(-_mp_size_2+1)/2 : (_mp_size_2+1)/2;
    op2->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
    
    if(abs(_mp_size_1)%2 != 0){
        op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    if(abs(_mp_size_2)%2 != 0){
        op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    
    int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
    rop-> _mp_alloc = alloc/2;
    rop->_mp_size = 0;
    rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
    
    mpz_add(rop,op1,op2);
    
    int rop_size = rop->_mp_size;
    *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;
    
    return 0;
}

static int
nmpz_mod_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2)
{
    mpz_t op1,op2,rop;   
    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);

    op1->_mp_alloc = (_mp_alloc_1)/2;
    op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
    op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);

    op2->_mp_alloc = (_mp_alloc_2)/2;
    op2->_mp_size = (_mp_size_2 < 0) ? -(-_mp_size_2+1)/2 : (_mp_size_2+1)/2;
    op2->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
    
    if(abs(_mp_size_1)%2 != 0){
        op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    if(abs(_mp_size_2)%2 != 0){
        op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    
    int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
    rop-> _mp_alloc = alloc/2;
    rop->_mp_size = 0;
    rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
    
    mpz_mod(rop,op1,op2);
    
    int rop_size = rop->_mp_size;
    *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;
    
    return 0;
}

static int
nmpz_gcd_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2){
        mpz_t op1,op2,rop;   
        wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
        
        op1->_mp_alloc = (_mp_alloc_1)/2;
        op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
        op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
    
        op2->_mp_alloc = (_mp_alloc_2)/2;
        op2->_mp_size = (_mp_size_2 < 0) ? -(-_mp_size_2+1)/2 : (_mp_size_2+1)/2;
        op2->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
        
        if(abs(_mp_size_1)%2 != 0){
            op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        if(abs(_mp_size_2)%2 != 0){
            op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        
        int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
        rop-> _mp_alloc = alloc/2;
        rop->_mp_size = 0;
        rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
        
        mpz_gcd(rop,op1,op2);
        
        int rop_size = rop->_mp_size;
        *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;

        return 0;
}

static int
nmpz_invert_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2){
        mpz_t op1,op2,rop;   
        wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
        
        op1->_mp_alloc = (_mp_alloc_1)/2;
        op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
        op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
    
        op2->_mp_alloc = (_mp_alloc_2)/2;
        op2->_mp_size = (_mp_size_2 < 0) ? -(-_mp_size_2+1)/2 : (_mp_size_2+1)/2;
        op2->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
        
        if(abs(_mp_size_1)%2 != 0){
            op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        if(abs(_mp_size_2)%2 != 0){
            op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        
        int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
        rop-> _mp_alloc = alloc/2;
        rop->_mp_size = 0;
        rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
        
        mpz_invert(rop,op1,op2);
        
        int rop_size = rop->_mp_size;
        *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;

        return 0;
}

static int
nmpz_nextprime_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1)
{
    mpz_t op1,rop;   
        wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
        
        op1->_mp_alloc = (_mp_alloc_1)/2;
        op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
        op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);  
        
        if(abs(_mp_size_1)%2 != 0){
            op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        
        int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
        rop-> _mp_alloc = alloc/2;
        rop->_mp_size = 0;
        rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
        
        mpz_nextprime(rop,op1);
        
        int rop_size = rop->_mp_size;
        *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;

        return 0;
}

static int
nmpz_powm_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2,
    int _mp_alloc_3, int _mp_size_3, uint32_t *_mp_d_3){
    mpz_t op1,op2,op3,rop;   
        wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
        
        op1->_mp_alloc = (_mp_alloc_1)/2;
        op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
        op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
    
        op2->_mp_alloc = (_mp_alloc_2)/2;
        op2->_mp_size = (_mp_size_2 < 0) ? -(-_mp_size_2+1)/2 : (_mp_size_2+1)/2;
        op2->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
        
        op3->_mp_alloc = (_mp_alloc_3)/2;
        op3->_mp_size = (_mp_size_3 < 0) ? -(-_mp_size_3+1)/2 : (_mp_size_3+1)/2;
        op3->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_3);

        if(abs(_mp_size_1)%2 != 0){
            op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        if(abs(_mp_size_2)%2 != 0){
            op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        if(abs(_mp_size_3)%2 != 0){
            op3->_mp_d[abs(op3->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
        }
        
        int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
        rop-> _mp_alloc = alloc/2;
        rop->_mp_size = 0;
        rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
        
        mpz_powm(rop,op1,op2,op3);
        
        int rop_size = rop->_mp_size;
        *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;
        
        return 0;
}

static int
nmpz_sub_ui_wrapper(wasm_exec_env_t exec_env,
    uint32_t *_mp_alloc_rop, uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
    uint32_t _mp_d_2)
{
    mpz_t op1, rop;   
    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    
    op1->_mp_alloc = (_mp_alloc_1)/2;
    op1->_mp_size = (_mp_size_1 < 0) ? -(-_mp_size_1+1)/2 : (_mp_size_1+1)/2;
    op1->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);  
    
    if(abs(_mp_size_1)%2 != 0){
        op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
    }
    
    int alloc=*(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
    rop-> _mp_alloc = alloc/2;
    rop->_mp_size = 0;
    rop->_mp_d     = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
    
    mpz_sub_ui(rop, op1, _mp_d_2);

    int rop_size = rop->_mp_size;
    *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size * 2;

    return 0;
}


/* clang-format off */
#define REG_NATIVE_FUNC(func_name, signature) \
    { #func_name, func_name##_wrapper, signature, NULL }

static NativeSymbol native_symbols[] = {
    REG_NATIVE_FUNC(nmpz_mul, "(iiiiiiiii)i"),
    REG_NATIVE_FUNC(nmpz_add, "(iiiiiiiii)i"),
    REG_NATIVE_FUNC(nmpz_mod, "(iiiiiiiii)i"),
    REG_NATIVE_FUNC(nmpz_gcd,"(iiiiiiiii)i"),
    REG_NATIVE_FUNC(nmpz_invert,"(iiiiiiiii)i"),
    REG_NATIVE_FUNC(nmpz_nextprime, "(iiiiii)i"),
    REG_NATIVE_FUNC(nmpz_powm,"(iiiiiiiiiiii)i"),
    REG_NATIVE_FUNC(nmpz_sub_ui, "(iiiiiii)i"),
};
/* clang-format on */

uint32_t
get_native_lib(char **p_module_name, NativeSymbol **p_native_symbols)
{
    *p_module_name = "env";
    *p_native_symbols = native_symbols;
    return sizeof(native_symbols) / sizeof(NativeSymbol);
}