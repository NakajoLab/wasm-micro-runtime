

include("/home/koichi/wasm-micro-runtime/samples/native-lib/native-lib32/toolchain-x86-32.cmake")
add_library(test_rsa SHARED ./native_gmp.c)