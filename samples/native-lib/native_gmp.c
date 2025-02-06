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
nmpz_nextprime_wrapper(wasm_exec_env_t exec_env,  uint32_t op_str_addr, uint32_t rop_str_addr)
{
    mpz_t rop, op;
    mpz_init(rop);
    mpz_init(op);
    
    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *rop_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)rop_str_addr);
    char *op_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op_str_addr);
    
    mpz_set_str(op, op_str, 10);

    mpz_nextprime(rop,op);

    mpz_get_str(rop_str, 10, rop);

    mpz_clear(rop);
    mpz_clear(op);
    
    return 0;
}

static int
nmpz_mod_wrapper(wasm_exec_env_t exec_env,  uint32_t op1_str_addr, uint32_t op2_str_addr , uint32_t rop_str_addr)
{
    mpz_t rop, op1,op2;
    mpz_init(rop);
    mpz_init(op1);
    mpz_init(op2);

    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *rop_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)rop_str_addr);
    char *op1_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op1_str_addr);
    char *op2_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op2_str_addr);

    mpz_set_str(op1, op1_str, 10);
    mpz_set_str(op2, op2_str, 10);

    mpz_mod(rop,op1,op2);

    mpz_get_str(rop_str, 10, rop);

    mpz_clear(rop);
    mpz_clear(op1);
    mpz_clear(op2);
    
    return 0;  
}

static int
nmpz_mul_wrapper(wasm_exec_env_t exec_env,  uint32_t num1_str_addr, uint32_t num2_str_addr , uint32_t result_str_addr)
{
    mpz_t num1, num2, result;
    mpz_init(num1);  // mpz_t型の変数を初期化
    mpz_init(num2);
    mpz_init(result);

    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *num1_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)num1_str_addr);
    char *num2_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)num2_str_addr);
    char *result_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)result_str_addr);

    // 文字列として渡された数値をmpz_t型に変換
    mpz_set_str(num1, num1_str, 10);  // 10は10進数
    mpz_set_str(num2, num2_str, 10);
    // 掛け算を実行
    mpz_mul(result, num1, num2);
    // 結果を文字列に変換
    mpz_get_str(result_str, 10, result);  // 10進数で結果を文字列として取得
    // 使用が終わったmpz_t型の変数を解放
    mpz_clear(num1);
    mpz_clear(num2);
    mpz_clear(result);
    return 0;
}

static int
nmpz_sub_ui_wrapper(wasm_exec_env_t exec_env,  uint32_t op1_str_addr, unsigned long int op2 , uint32_t rop_str_addr)
{
    mpz_t rop, op1;
    mpz_init(rop);
    mpz_init(op1);

    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *rop_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)rop_str_addr);
    char *op1_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op1_str_addr);

    mpz_set_str(op1, op1_str, 10);

    mpz_sub_ui(rop,op1,op2);

    mpz_get_str(rop_str, 10, rop);

    mpz_clear(rop);
    mpz_clear(op1);

    return 0;  
}

static int
nmpz_invert_wrapper(wasm_exec_env_t exec_env,  uint32_t op1_str_addr, uint32_t op2_str_addr , uint32_t rop_str_addr){
    int res;
    mpz_t rop, op1,op2;
    mpz_init(rop);
    mpz_init(op1);
    mpz_init(op2);

    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *rop_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)rop_str_addr);
    char *op1_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op1_str_addr);
    char *op2_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op2_str_addr);

    mpz_set_str(op1, op1_str, 10);
    mpz_set_str(op2, op2_str, 10);

    res = mpz_invert(rop,op1,op2);

    mpz_get_str(rop_str, 10, rop);

    mpz_clear(rop);
    mpz_clear(op1);
    mpz_clear(op2);
    
    return res;  
}

static int
nmpz_gcd_wrapper(wasm_exec_env_t exec_env,  uint32_t op1_str_addr, uint32_t op2_str_addr , uint32_t rop_str_addr){
    mpz_t rop, op1,op2;
    mpz_init(rop);
    mpz_init(op1);
    mpz_init(op2);

    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *rop_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)rop_str_addr);
    char *op1_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op1_str_addr);
    char *op2_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op2_str_addr);

    mpz_set_str(op1, op1_str, 10);
    mpz_set_str(op2, op2_str, 10);

    mpz_gcd(rop,op1,op2);

    mpz_get_str(rop_str, 10, rop);

    mpz_clear(rop);
    mpz_clear(op1);
    mpz_clear(op2);
    
    return 0;  
}
static int
nmpz_powm_wrapper(wasm_exec_env_t exec_env,  uint32_t op1_str_addr, uint32_t op2_str_addr , uint32_t op3_str_addr, uint32_t rop_str_addr){
    mpz_t rop, op1,op2,op3;
    mpz_init(rop);
    mpz_init(op1);
    mpz_init(op2);
    mpz_init(op3);

    wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
    char *rop_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)rop_str_addr);
    char *op1_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op1_str_addr);
    char *op2_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op2_str_addr);
    char *op3_str     = wasm_runtime_addr_app_to_native(inst, (uint64_t)op3_str_addr); 

    mpz_set_str(op1, op1_str, 10);
    mpz_set_str(op2, op2_str, 10);
    mpz_set_str(op3, op3_str, 10);

    mpz_powm(rop,op1,op2,op3);

    mpz_get_str(rop_str, 10, rop);

    mpz_clear(rop);
    mpz_clear(op1);
    mpz_clear(op2);
    mpz_clear(op3);
    
    return 0;  
}
/* clang-format off */
#define REG_NATIVE_FUNC(func_name, signature) \
    { #func_name, func_name##_wrapper, signature, NULL }

static NativeSymbol native_symbols[] = {
    REG_NATIVE_FUNC(nmpz_nextprime, "(ii)i"),
    REG_NATIVE_FUNC(nmpz_mod, "(iii)i"),
    REG_NATIVE_FUNC(nmpz_mul, "(iii)i"),
    REG_NATIVE_FUNC(nmpz_sub_ui, "(iii)i"),
    REG_NATIVE_FUNC(nmpz_invert,"(iii)i"),
    REG_NATIVE_FUNC(nmpz_gcd,"(iii)i"),
    REG_NATIVE_FUNC(nmpz_powm,"(iiii)i"),
};
/* clang-format on */

uint32_t
get_native_lib(char **p_module_name, NativeSymbol **p_native_symbols)
{
    *p_module_name = "env";
    *p_native_symbols = native_symbols;
    return sizeof(native_symbols) / sizeof(NativeSymbol);
}