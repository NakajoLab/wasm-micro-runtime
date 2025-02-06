#include <stdio.h>
#include <stdlib.h>


int test_benchmark();

int
main(int argc, char **argv)
{
    int res ;
    
    res = test_benchmark();
    
    printf("result is %d\n", res);

    return 0;
}