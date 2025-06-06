#include <bignum.h>
#include <utils.h>

void
bn_init(BigNum *num, Arena *arena)
{
    size_t size;
    MUST(num != NULL, "num pointer is NULL in bn_init");

    size = BN_DEFAULT_WORDS_SIZE;
    num->words = bn_alloc_words(size, arena);

    num->size = 0;
    num->capacity = size;
}
