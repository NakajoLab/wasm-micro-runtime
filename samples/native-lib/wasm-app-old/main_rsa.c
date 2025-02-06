#include <stdio.h>
#include <stdlib.h>


int test_rsa();

int
main(int argc, char **argv)
{
    
    
    int res ;

    // 暗号処理　＋　GMP　すべてネイティブとして処理
    res = test_rsa();
    
    printf("res is %d\n", res);

    return 0;
}