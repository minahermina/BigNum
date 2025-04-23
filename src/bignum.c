#define ARENA_ALLOCATOR_IMPLEMENTATION
#include <bignum.h>

#define BIGNUM_SIZE sizeof(BigNum)

BigNum*
BigNum_new(Arena *arena)
{
    BigNum *num = {0};
    if(arena == NULL) {
        num = (BigNum*)malloc(BIGNUM_SIZE);
    }else{
        num = (BigNum*)arena_alloc(arena, BIGNUM_SIZE);
    }
    DEBUG_BIGNUM((*num));
    return num;

}
