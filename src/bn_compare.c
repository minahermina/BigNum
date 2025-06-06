#include <bignum.h>
#include <utils.h>

int
bn_compare(const BigNum *num1, const BigNum *num2)
{
    int ucompare_result;

    MUST(num1 != NULL, "num1 pointer is NULL in bn_compare");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_compare");

    /* signs differ */
    if (bn_is_negative(num1) != bn_is_negative(num2)) {
        return bn_is_negative(num1) ? 1 : 0;
    }

    ucompare_result = bn_ucompare(num1, num2);

    /* both signs are (-) */
    if (bn_is_negative(num1)) {
        /* reverse ucompare_result */
        if(ucompare_result == 0 ) return 1;
        else if(ucompare_result == 1 ) return 0;
    } 
    /* both signs are (+) */
    else {
        return ucompare_result;
    }

    /* both numbers are equal */
    return 2;
}
