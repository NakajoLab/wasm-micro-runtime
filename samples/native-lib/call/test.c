// #include <stdio.h>

// void test() {
//     volatile double sum = 0.0;  // volatileをつけて最適化を防ぐ
//     for (volatile int i = 0; i < 500000000; i++) {  // ループ回数を増やす
//         sum += 1.0 / (i + 1);  // 計算負荷を上げる
//     }
//     printf("sum = %f\n", sum);  // 計算結果を出力（最適化防止）
// }

// int main() {
//     test();
//     return 0;
// }

//gprofでの測定
// gcc -pg -o test test.c
// ./test
// gprof test gmon.out > profile.txt
// cat profile.txt
//または
// gprof myprog gmon.out


//rdtscでの測定
#include <stdio.h>
#include <stdint.h>

static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void empty_function() {}

int main() {
    const int N = 10000000;  // 1千万回呼び出し
    uint64_t start, end, loop_cycles, function_cycles;

    // ループのみ
    start = rdtsc();
    for (int i = 0; i < N; i++);
    end = rdtsc();
    loop_cycles = end - start;

    // 関数呼び出しあり
    start = rdtsc();
    for (int i = 0; i < N; i++) {
        empty_function();
    }
    end = rdtsc();
    function_cycles = end - start;

    // 1回の関数呼び出しオーバーヘッド (CPUクロック単位)
    double overhead = (double)(function_cycles - loop_cycles) / N;

    printf("Loop cycles: %lu\n", loop_cycles);
    printf("Function call cycles: %lu\n", function_cycles);
    printf("Function call overhead: %f cycles per call\n", overhead);

    return 0;
}
