#define ARENA_ALLOCATOR_IMPLEMENTATION
#include <bignum.h>
#include <utils.h>

BigNum*
bn_new(Arena *arena)
{
    BigNum *num = NULL;
    size_t size = BN_DEFAULT_WORDS_SIZE;

    if(arena == NULL) {
        num = (BigNum*) malloc(sizeof(BigNum));
        MUST(num != NULL, "Allocating Memory in bn_new");

        num->words = bn_alloc_words(size, arena);
    } else {
        num = (BigNum*) arena_alloc(arena, sizeof(BigNum));
        MUST(num != NULL, "Allocating Memory in bn_new");

        num->words = bn_alloc_words(size, arena );
    }

    num->size = 0;
    num->capacity = size;
    num->negative = 0;

    return num;
}
