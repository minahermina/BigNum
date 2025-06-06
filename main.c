#include <stdio.h>
#include <bignum.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>

int
main(int argc, char **argv){
    Arena arena = {0};
    arena_init(&arena, 4096);


    if(argc < 3){
        fprintf(stderr, "NO. or arguments is less than 2\n");
        return -1;
    }

    char *arg1 = argv[1];
    char *arg2 = argv[2];
    BigNum *num1 = bn_from_dec(arg1, strlen(arg1), &arena);
    BigNum *num2 =  bn_from_dec(arg2, strlen(arg2), &arena);

    BigNum remain, q;
    bn_init(&remain, &arena);
    bn_init(&q, &arena);

    printf("---num1: ---\n");
    bn_print_words(num1, 'x');
    printf("\n");

    printf("----num2: --------\n");
    bn_print_words(num2, 'x');
    printf("\n");


    bn_div(&q, &remain, num1, num2, &arena);

    printf("----q: --------\n");
    bn_print_words(&q, 'x');
    printf("\n");

    printf("----remain: --------\n");
    bn_print_words(&remain, 'x');
    printf("\n");
/*
    printf("----q: --------\n");
    bn_print_words(q, 'x');
    printf("\n");

*/




    return 0;
}
