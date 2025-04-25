#include <stdio.h>
#include <bignum.h>

int
main(){
    /* Arena arena = {0};
    arena_init(&arena, 2048);
    BigNum *num = bignum_new(&arena);
    printf("Hello");
    return 0; */
    int a = 0x1234;
    char *p = (char*)&a;
    printf("%x%x", *p, *(p+1));

}
