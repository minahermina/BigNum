#include <bignum.h>
#include <utils.h>

int
bn_set_zero(BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bn_set_zero");

    if(num->capacity > 0) {
        num->size = 1;
        num->negative = 0;
        num->words[0] = 0;
        return 0;
    }

    return -1;
}
