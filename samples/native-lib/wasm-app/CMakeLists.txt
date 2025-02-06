cmake_minimum_required(VERSION 3.14)
project(wasm-app)

# Emscripten の emcc コマンドをチェック
find_program(EMCC_EXECUTABLE emcc)

if (NOT EMCC_EXECUTABLE)
    message(FATAL_ERROR "コマンドemccが使用できません.次のコマンドを実行してください.\ncd ~/emsdk\nsource ./emsdk_env.sh\n")
endif ()

# emcc をコンパイラとして設定
set(CMAKE_C_COMPILER "${EMCC_EXECUTABLE}")
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

# Emscripten用のフラグ
set(CMAKE_C_FLAGS "-s STANDALONE_WASM")

# インクルードディレクトリをターゲットごとに適用
set(GMP_INCLUDE_DIR "$ENV{HOME}/emscripten-gmp/include")
set(GMP_LIB_DIR "$ENV{HOME}/emscripten-gmp/lib")

# GMP ライブラリを探す

include_directories(~/emscripten-gmp/include)
link_directories(~/emscripten-gmp/lib)

# テスト用の実行ファイル
add_executable(test test.c)
add_executable(test2 test2.c)

message(STATUS "GMP_LIB_WASM : ${GMP_LIB_WASM}")

# GMP ライブラリをリンク
target_link_libraries(test ${GMP_LIB_DIR}/libgmp.a)
target_link_libraries(test2 ${GMP_LIB_DIR}/libgmp.a)

# オプションのリンカフラグを設定
set(CMAKE_EXE_LINKER_FLAGS "-s STANDALONE_WASM -Wl,--allow-undefined")

add_custom_target(run
    DEPENDS test  
    COMMAND ./iwasm
        --native-lib=./libtest_rsa.so
        --native-lib=./libtest_add.so
        --native-lib=./libtest_sqrt.so
        --native-lib=./libtest_hello.so
        --native-lib=./libtest_hello2.so
        wasm-app/test.wasm
        --stack-size=262144 --heap-size=268435456
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run2
    DEPENDS test2  
    COMMAND ./iwasm
        --native-lib=./libtest_rsa.so
        --native-lib=./libtest_add.so
        --native-lib=./libtest_sqrt.so
        --native-lib=./libtest_hello.so
        --native-lib=./libtest_hello2.so
        wasm-app/test2.wasm
        --stack-size=262144 --heap-size=268435456
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

#メモ
# エクスポート設定
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--export=main")

# -Wl,--max-memory=131072 -z stack-size=8192がデフォルト 131072=128KB, 8192=8KB
# 268435456byte=256MB,131072byte=128KB に変更
# set (CMAKE_EXE_LINKER_FLAGS
#     "-Wl,--max-memory=268435456 -z stack-size=131072   \
#      -Wl,--no-entry,--strip-all                   \
#      -Wl,--export=__main_argc_argv                \
#      -Wl,--export=__heap_base,--export=__data_end \
#      -Wl,--allow-undefined"
# )

# set(NATIVELIB_DIR "/home/koichi/wasm-micro-runtime/samples/native-lib")
