#include <bignum.h>
#include <utils.h>

int
bn_is_negative(const BigNum *num)
{
    MUST(num != NULL, "num is NULL in bn_is_negative");
    return num->negative;
}
