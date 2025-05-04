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
 nmpz_mul_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                  uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                  int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                  int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_mul(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_tdiv_q_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                  uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                  int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                  int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_tdiv_q(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_add_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                  uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                  int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                  int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_add(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_sub_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                  uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                  int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                  int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_sub(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 
 static int
 nmpz_mod_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                  uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                  int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                  int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_mod(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_gcd_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                  uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                  int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                  int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_gcd(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_invert_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                     uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
                     int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
                     int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     int res = mpz_invert(rop, op1, op2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return res;
 }
 
 static int
 nmpz_nextprime_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                        uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
                        int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1)
 {
     mpz_t op1, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_nextprime(rop, op1);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_powm_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                   uint32_t *_mp_size_rop, uint32_t *_mp_d_rop, int _mp_alloc_1,
                   int _mp_size_1, uint32_t *_mp_d_1, int _mp_alloc_2,
                   int _mp_size_2, uint32_t *_mp_d_2, int _mp_alloc_3,
                   int _mp_size_3, uint32_t *_mp_d_3)
 {
     mpz_t op1, op2, op3, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     op3->_mp_alloc = (_mp_alloc_3) / 2;
     op3->_mp_size =
         (_mp_size_3 < 0) ? -(-_mp_size_3 + 1) / 2 : (_mp_size_3 + 1) / 2;
     op3->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_3);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_3) % 2 != 0) {
         op3->_mp_d[abs(op3->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_powm(rop, op1, op2, op3);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_sub_ui_wrapper(wasm_exec_env_t exec_env, uint32_t *_mp_alloc_rop,
                     uint32_t *_mp_size_rop, uint32_t *_mp_d_rop,
                     int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
                     uint32_t _mp_d_2)
 {
     mpz_t op1, rop;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop);
     rop->_mp_alloc = alloc / 2;
     rop->_mp_size = 0;
     rop->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop);
 
     mpz_sub_ui(rop, op1, _mp_d_2);
 
     int rop_size = abs(rop->_mp_size);
     int flag = (rop->_mp_size < 0) ? -1 : 1;
     if (rop->_mp_d[abs(rop->_mp_size) - 1] >> 32 == 0) {
         rop_size = rop_size * 2 - 1;
     }
     else {
         rop_size = rop_size * 2;
     }
 
     if(flag == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = rop_size;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop) = -rop_size;
     }
 
     return 0;
 }
 
 static int
 nmpz_gcdext_wrapper(wasm_exec_env_t exec_env, 
     uint32_t *_mp_alloc_rop1, uint32_t *_mp_size_rop1, uint32_t *_mp_d_rop1,
     uint32_t *_mp_alloc_rop2, uint32_t *_mp_size_rop2, uint32_t *_mp_d_rop2,
     uint32_t *_mp_alloc_rop3, uint32_t *_mp_size_rop3, uint32_t *_mp_d_rop3,
      int _mp_alloc_1, int _mp_size_1, uint32_t *_mp_d_1,
     int _mp_alloc_2, int _mp_size_2, uint32_t *_mp_d_2)
 {
     mpz_t op1, op2, rop1, rop2, rop3;
     wasm_module_inst_t inst = wasm_runtime_get_module_inst(exec_env);
 
     op1->_mp_alloc = (_mp_alloc_1) / 2;
     op1->_mp_size =
         (_mp_size_1 < 0) ? -(-_mp_size_1 + 1) / 2 : (_mp_size_1 + 1) / 2;
     op1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_1);
 
     op2->_mp_alloc = (_mp_alloc_2) / 2;
     op2->_mp_size =
         (_mp_size_2 < 0) ? -(-_mp_size_2 + 1) / 2 : (_mp_size_2 + 1) / 2;
     op2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_2);
 
     if (abs(_mp_size_1) % 2 != 0) {
         op1->_mp_d[abs(op1->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
     if (abs(_mp_size_2) % 2 != 0) {
         op2->_mp_d[abs(op2->_mp_size) - 1] &= 0x00000000FFFFFFFFUL;
     }
 
     int alloc1 =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop1);
     rop1->_mp_alloc = alloc1 / 2;
     rop1->_mp_size = 0;
     rop1->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop1);
 
     int alloc2 =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop2);
     rop2->_mp_alloc = alloc2 / 2;
     rop2->_mp_size = 0;
     rop2->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop2);
 
     int alloc3 =
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_alloc_rop3);
     rop3->_mp_alloc = alloc3 / 2;
     rop3->_mp_size = 0;
     rop3->_mp_d = wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_d_rop3);
 
     mpz_gcdext(rop1,rop2,rop3, op1, op2);
 
     int rop_size1 = abs(rop1->_mp_size);
     int flag1 = (rop1->_mp_size < 0) ? -1 : 1;
     if (rop1->_mp_d[abs(rop1->_mp_size) - 1] >> 32 == 0) {
         rop_size1 = rop_size1 * 2 - 1;
     }
     else {
         rop_size1 = rop_size1 * 2;
     }
     if(flag1 == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop1) = rop_size1;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop1) = -rop_size1;
     }
 
     int rop_size2 = abs(rop2->_mp_size);
     int flag2 = (rop2->_mp_size < 0) ? -1 : 1;
     if (rop2->_mp_d[abs(rop2->_mp_size) - 1] >> 32 == 0) {
         rop_size2 = rop_size2 * 2 - 1;
     }
     else {
         rop_size2 = rop_size2 * 2;
     }
     if(flag2 == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop2) = rop_size2;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop2) = -rop_size2;
     }
 
     int rop_size3 = abs(rop3->_mp_size);
     int flag3 = (rop3->_mp_size < 0) ? -1 : 1;
     if (rop3->_mp_d[abs(rop3->_mp_size) - 1] >> 32 == 0) {
         rop_size3 = rop_size3 * 2 - 1;
     }
     else {
         rop_size3 = rop_size3 * 2;
     }
     if(flag3 == 1){
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop3) = rop_size3;
     }else{
         *(int *)wasm_runtime_addr_app_to_native(inst, (uint64_t)_mp_size_rop3) = -rop_size3;
     }
 
     return 0;
 }
 
 /* clang-format off */
  #define REG_NATIVE_FUNC(func_name, signature) \
      { #func_name, func_name##_wrapper, signature, NULL }
  
  static NativeSymbol native_symbols[] = {
      REG_NATIVE_FUNC(nmpz_mul, "(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_tdiv_q, "(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_add, "(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_sub, "(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_mod, "(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_gcd,"(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_invert,"(iiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_nextprime, "(iiiiii)i"),
      REG_NATIVE_FUNC(nmpz_powm,"(iiiiiiiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_sub_ui, "(iiiiiii)i"),
      REG_NATIVE_FUNC(nmpz_gcdext,"(iiiiiiiiiiiiiii)i"),
  };
 /* clang-format on */
 
 uint32_t
 get_native_lib(char **p_module_name, NativeSymbol **p_native_symbols)
 {
     *p_module_name = "env";
     *p_native_symbols = native_symbols;
     return sizeof(native_symbols) / sizeof(NativeSymbol);
 }