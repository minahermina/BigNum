#include <bignum.h>
#include <utils.h>


int
bn_is_zero(const BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bn_is_zero");

    if(num->size == 1){
        return (num->words[0] == 0);
    }

    return 0;
}
