cmake_minimum_required(VERSION 3.14)
project(wasm-app)

# Emscripten の emcc コマンドをチェック
find_program(EMCC_EXECUTABLE emcc)
# 環境変数PATHから探す
# EMCC_EXECUTABLEにemccのフルパスが入る

pkg_check_modules(GMP_WASM REQUIRED IMPORTED_TARGET GLOBAL gmp-wasm)

if (NOT EMCC_EXECUTABLE)
    message(FATAL_ERROR "コマンドemccが使用できません.次のコマンドを実行してください.\ncd ~/emsdk\nsource ./emsdk_env.sh\n")
endif ()

# emcc をコンパイラとして設定
set(CMAKE_C_COMPILER "${EMCC_EXECUTABLE}")
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

# Emscripten用のフラグ
# set(CMAKE_C_FLAGS "-s STANDALONE_WASM")
set(CMAKE_C_FLAGS "-s STANDALONE_WASM -O3 -fomit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --cache /tmp/emcache")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${GMP_WASM_INCLUDE_DIRS}")
# オプションのリンカフラグを設定
set(CMAKE_EXE_LINKER_FLAGS "-s STANDALONE_WASM -Wl,--allow-undefined ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=48388608")

# テスト用の実行ファイル
# add_executable(test test.c)
# add_executable(test2 test2.c)
add_executable(multiply multiply.c)
add_executable(multiply_native multiply_native.c)
add_executable(divide divide.c)
add_executable(rsa rsa.c)
add_executable(rsa_native rsa_native.c)

target_compile_options(multiply PRIVATE -o multiply)
target_compile_options(multiply_native PRIVATE -o multiply_native)
target_compile_options(divide PRIVATE -o divide)
target_compile_options(rsa PRIVATE -o rsa)
target_compile_options(rsa_native PRIVATE -o rsa_native)

# GMP ライブラリをリンク
target_link_libraries(multiply PkgConfig::GMP_WASM)
target_link_libraries(multiply_native PkgConfig::GMP_WASM)
target_link_libraries(divide PkgConfig::GMP_WASM)
target_link_libraries(rsa PkgConfig::GMP_WASM)
target_link_libraries(rsa_native PkgConfig::GMP_WASM)

get_property(INCLUDE_DIRS TARGET multiply PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Final include directories: ${INCLUDE_DIRS}")

# インタプリタ
add_custom_target(run
    DEPENDS  rsa iwasm test_rsa test_add test_sqrt test_hello test_hello2
    COMMAND ./iwasm
        --native-lib=./libtest_rsa.so
        --native-lib=./libtest_add.so
        --native-lib=./libtest_sqrt.so
        --native-lib=./libtest_hello.so
        --native-lib=./libtest_hello2.so
        wasm-app/rsa.wasm
        --stack-size=262144 --heap-size=268435456
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
# インタプリタ＋ネイティブ
add_custom_target(run2
    DEPENDS  rsa_native iwasm test_rsa test_add test_sqrt test_hello test_hello2
    COMMAND ./iwasm
        --native-lib=./libtest_rsa.so
        --native-lib=./libtest_add.so
        --native-lib=./libtest_sqrt.so
        --native-lib=./libtest_hello.so
        --native-lib=./libtest_hello2.so
        wasm-app/rsa_native.wasm
        --stack-size=262144 --heap-size=268435456
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
# 積 インタプリタ
add_custom_target(mul
    DEPENDS  multiply iwasm test_rsa test_add test_sqrt test_hello test_hello2
    COMMAND ./iwasm
        --native-lib=./libtest_rsa.so
        --native-lib=./libtest_add.so
        --native-lib=./libtest_sqrt.so
        --native-lib=./libtest_hello.so
        --native-lib=./libtest_hello2.so
        wasm-app/multiply.wasm
        --stack-size=262144 --heap-size=268435456
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
# 積　インタプリタ＋ネイティブ
add_custom_target(mul2
    DEPENDS  multiply_native iwasm test_rsa test_add test_sqrt test_hello test_hello2
    COMMAND ./iwasm
        --native-lib=./libtest_rsa.so
        --native-lib=./libtest_add.so
        --native-lib=./libtest_sqrt.so
        --native-lib=./libtest_hello.so
        --native-lib=./libtest_hello2.so
        wasm-app/multiply_native.wasm
        --stack-size=8*1024*1024 --heap-size=64*1024*1024
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)



