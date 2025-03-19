#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int native_add(int op1, int op2);

//時間取得
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  
    return ts.tv_sec + ts.tv_nsec / 1e9;  // 秒単位の浮動小数点数に変換
}


//加算のネイティブライブラリ　N回測定
double addtest_native(int N){
    double start_time, end_time, result_time;
    int res=0;
    srand(time(NULL));
    int a = rand();
    int b = rand();

    start_time = get_time();
    for (volatile int i = 0; i < N; i++){
        res = native_add(a,b); 
    }
    end_time = get_time();

    result_time = end_time - start_time;
    return result_time;
}
//ネイティブ使用
int RunTestNative(){
    for(int i=1; i<=100; i++){
        int N = i*1000;
        double res =  addtest_native(N);
        printf("%d,%f\n", N, res);
    }
    return 0;
}


//インタプリタ 測定
int test_add(int op1, int op2){
    return op1 + op2;
}
//
double addtest_interpreter(int N){
    double start_time, end_time, result_time;
    int res=0;
    srand(time(NULL));
    int a = rand();
    int b = rand();

    start_time = get_time();
    for (volatile int i = 0; i < N; i++){
        res = test_add(a,b); 
    }
    end_time = get_time();

    result_time = end_time - start_time;
    return result_time;
}
int RunTestInterpreter(){
    for(int i=1; i<=100; i++){
        int N = i*1000;
        double res =  addtest_interpreter(N);
        printf("%d,%f\n", N, res);
    }
    return 0;
}


int main(void){
    
    //RunTestNative();
    RunTestInterpreter();

    return 0;
}




