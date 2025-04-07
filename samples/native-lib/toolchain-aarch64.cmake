# toolchain-aarch64.cmake
message(STATUS "cmake is called")
# 明示的にターゲットプラットフォームを指定
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# クロスコンパイラを指定（Debian/Ubuntu の例）
set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)

# リンカ・アーカイバなど（任意）
# set(CMAKE_AR aarch64-linux-gnu-ar)
# set(CMAKE_RANLIB aarch64-linux-gnu-ranlib)
# set(CMAKE_STRIP aarch64-linux-gnu-strip)

# 32bit/64bit を CMake に伝える
set(CMAKE_SIZEOF_VOID_P 8)

# 追加のインクルードパスが必要な場合はここで設定
# set(CMAKE_SYSROOT /path/to/aarch64/sysroot)

# optional: static linking したいとき
# set(CMAKE_EXE_LINKER_FLAGS "-static")

message(STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")