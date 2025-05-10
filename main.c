#include <stdio.h>
#include <bignum.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

int
main(){
    Arena arena = {0};
    arena_init(&arena, 4096);

    char str1[] = "0x63C";
    char str2[] = "0x0C7800";
    BigNum *num1 = bignum_from_hex(str1, strlen(str1), &arena);
    BigNum *num2 =  bignum_from_hex(str2, strlen(str2), &arena);
    BigNum *res =  bignum_new(&arena);

    printf("---num1: ---\n");
    bignum_print_words(num1, 'x');
    printf("\n");

    printf("----num2: --------\n");
    bignum_print_words(num2, 'x');
    printf("\n");


    printf("----res: --------\n");
    bignum_lshift(res, num1, 9, &arena);
    bignum_print_words(res, 'x');
    printf("\n");

    if(bignum_compare(res, num2) < 0){
        printf("error");
    }



    /* printf("---res: ---\n");
    bignum_print_words(res, 'b');
    printf("\n");
    bignum_print_words(res, 'x');
    */




    return 0;
}
