#include <stdio.h>
#include <bignum.h>
#include <string.h>

int
main(int argc, char **argv){
    Arena arena = {0};
    arena_init(&arena, 2 * 4096);


    if(argc < 3){
        fprintf(stderr, "NO. or arguments is less than 2\n");
        return -1;
    }

    char *arg1 = argv[1];
    char *arg2 = argv[2];
    BigNum *num1 = bn_from_dec(arg1, strlen(arg1), &arena);
    BigNum *num2 =  bn_from_dec(arg2, strlen(arg2), &arena);

    BigNum res;
    bn_init(&res, &arena);

    printf("---num1: ---\n");
    bn_print_words(num1, 'x');
    printf("\n");

    printf("----num2: --------\n");
    bn_print_words(num2, 'x');
    printf("\n");


    bn_subtract(&res, num1, num2, &arena);

    printf("----res: --------\n");
    bn_print_words(&res, 'x');
    printf("\n");

/*
    printf("----q: --------\n");
    bn_print_words(q, 'x');
    printf("\n");

*/




    return 0;
}
