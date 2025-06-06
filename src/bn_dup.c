#include <bignum.h>
#include <utils.h>

BigNum*
bn_dup(BigNum *src, Arena *arena)
{
    BigNum *num = NULL;
    MUST(src != NULL, "src pointer is NULL in bn_dup");

    /* create new BigNum num */
    num = bn_new(arena);

    MUST(num != NULL, "Allocating memory in bn_dup");

    bn_copy(num, src, arena);

    return num;
}
