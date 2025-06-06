#include <bignum.h>
#include <utils.h>

int
bn_subtract(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    int compare_result, ret;
    MUST(res  != NULL, "res pointer is NULL in bn_subtract");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_subtract");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_subtract");

    /*
     * +num1 - -num2 =  num1 + num2;
     * -num1 - +num2 =  -(num1 + num2);
     */
    if(bn_is_negative(num1) != bn_is_negative(num2)){
        ret = bn_uadd(res, num1, num2, arena);
        res->negative = num1->negative;
    } else{
        compare_result = bn_ucompare(num1, num2);
        /*
         * -5 - -3 = (5-3) * -1 = -2
         */
        if(compare_result == 0) {
            ret = bn_usubtract(res, num1, num2, arena);
            res->negative = num1->negative;
        }
        /*
         * -5 - -7 = (7-5) * opposite sign of (-5) = -2
         */
        else if(compare_result == 1){
            ret = bn_usubtract(res, num2, num1, arena);
            res->negative = !(num1->negative);
        }
        /*
         * num1 == num2
         */
        else {
            ret = bn_set_zero(res);
            res->negative = 0;
        }
    }

    return ret;
}
