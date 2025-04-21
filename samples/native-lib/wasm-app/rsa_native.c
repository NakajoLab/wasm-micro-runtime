//これはテスト用

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>

int
nmpz_mul(size_t count1, void* buffer1);
int
wrapped_nmpz_mul(mpz_t op1)
{
    size_t count1, count2, count_rop;
    
    // バイト列に変換
    void *buffer1 = mpz_export(NULL, &count1, 1, 1, 1, 0, op1);
    printf("Exported %zu bytes:\n", count1);
    nmpz_mul(count1,buffer1);
    // mpz_t imported;
    // mpz_init(imported);
    // mpz_import(imported, count1, 1, 1, 1, 0, buffer1);
    // gmp_printf("Imported1:  %Zd\n", imported);

    // void *buffer2 = mpz_export(NULL, &count2, 1, 1, 1, 0, op2);
    // ネイティブコード呼び出し
    // void *rop_buffer = nmpz_mul(buffer1, count1, buffer2, count2, &count_rop);
    
    // mpz_import(rop, count_rop, 1, 1, 1, 0, rop_buffer);

    free(buffer1);
    // free(buffer2);
    // free(rop_buffer);
    return 0;
}

int main() {
    // 1. 初期値を設定
    mpz_t op1, op2, rop, imported;
    mpz_init_set_str(op1, "105", 10);
    mpz_init_set_str(op2, "4", 10);
    mpz_init_set_str(rop, "0", 10);

    mpz_init(imported);

    // *計算処理
    wrapped_nmpz_mul(op1);
    

    mpz_clear(op1);
    mpz_clear(op2);
    mpz_clear(rop);
    mpz_clear(imported);

    return 0;




    // 2. mpz_export でバイト列に変換
    
    // printf("Exported %zu bytes:\n", count);
    // for (size_t i = 0; i < count; ++i)
    //     printf("%02X ", ((unsigned char *)buffer)[i]);
    // printf("\n");

    // 3. mpz_import で元の数値に戻す
    // mpz_import(imported, count, 1, 1, 1, 0, buffer);

    // // 4. 確認（復元された値が元と一致しているか）
    // if (mpz_cmp(original, imported) == 0) {
    //     printf("Success! Imported value matches original.\n");
    // } else {
    //     printf("Error! Values do not match.\n");
    // }

    // // 5. 表示（人間向け）
    // gmp_printf("Original:  %Zd\n", original);
    // gmp_printf("Imported:  %Zd\n", imported);

    // 6. 後始末
    // free(buffer);
    // mpz_clear(original);
    // mpz_clear(imported);

    // return 0;
}
