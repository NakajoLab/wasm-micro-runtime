# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.0)
project(wasm-app)

set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../..)



if (APPLE)
    set (HAVE_FLAG_SEARCH_PATHS_FIRST 0)
    set (CMAKE_C_LINK_FLAGS "")
    set (CMAKE_CXX_LINK_FLAGS "")
endif ()

set (CMAKE_SYSTEM_PROCESSOR wasm32)
set (CMAKE_SYSROOT                  ${WAMR_ROOT_DIR}/wamr-sdk/app/libc-builtin-sysroot)

if (NOT DEFINED WASI_SDK_DIR)
    set (WASI_SDK_DIR               "/opt/wasi-sdk")
endif ()

set (CMAKE_C_FLAGS                  "-nostdlib")
set (CMAKE_C_COMPILER_TARGET        "wasm32")
set (CMAKE_C_COMPILER               "${WASI_SDK_DIR}/bin/clang")


# -Wl,--max-memory=131072 -z stack-size=8192
# 131072=128KB, 8192=8KB
# 268435456byte=256MB,131072byte=128KB
set (CMAKE_EXE_LINKER_FLAGS
    "-Wl,--max-memory=268435456 -z stack-size=131072   \
     -Wl,--no-entry,--strip-all                   \
     -Wl,--export=__main_argc_argv                \
     -Wl,--export=__heap_base,--export=__data_end \
     -Wl,--allow-undefined"
)

set(NATIVELIB_DIR "/home/koichi/wasm-micro-runtime/samples/native-lib")

# wasmにコンパイルするmain関数の含むファイルを定義
add_executable(test.wasm  main_tmp.c)

target_link_libraries(test.wasm)
# iwasmを２つ作成
