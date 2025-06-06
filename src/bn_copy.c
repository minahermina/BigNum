#include <bignum.h>
#include <utils.h>

void
bn_copy(BigNum *dest, const BigNum *src, Arena *arena)
{
    size_t i;
    MUST(dest != NULL, "dest pointer is NULL in bn_copy");
    MUST(src != NULL, "src pointer is NULL in bn_copy");

    /*dest == src ? do nothing*/
    if(bn_compare(src, dest) == 2){
        return;
    }

    bn_resize(dest, src->size, arena);

    dest->negative = src->negative;
    for(i = 0; i < dest->size; ++i){
        dest->words[i] = src->words[i];
    }
}
