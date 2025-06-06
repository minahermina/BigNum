#include <bignum.h>
#include <utils.h>

BigNum*
bn_from_int(int n, Arena* arena)
{
    BigNum *num = bn_new(arena);

    MUST(num != NULL, "Allocating memory in bn_from_int");

    if(ABS(n) != n)
        num->negative = 1;

    n = ABS(n);
    bn_resize(num, 1, arena);
    num->words[0] = n;
    return num;
}
