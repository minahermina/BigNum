#include <bignum.h>
#include <utils.h>

void
bn_free(BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bn_free");
    MUST(num->words != NULL, "num->words pointer is NULL in bn_free");

    free(num->words);
    free(num);
}
