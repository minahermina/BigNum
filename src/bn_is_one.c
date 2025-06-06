#include <bignum.h>
#include <utils.h>

int
bn_is_one(const BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bn_is_one");

    if(num->size == 1){
        return (num->words[0] == 1);
    }

    return 0;
}
