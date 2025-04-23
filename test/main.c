#include<stdio.h>
#include<arena_allocator.h>
#include<bignum.h>

int
main(){
    Arena arena = {0};
    arena_init(&arena, 2048);
    BigNum *num = NULL;
    num = BigNum_new(&arena);
    printf("Hello");
    return 0;
}
