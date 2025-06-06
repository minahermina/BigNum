#include<bignum.h>
#include<utils.h>
/*
 * (num1 = q * num2 + remain)
 *
 * TODO: Optimize this slow udiv function
 */
int
bn_udiv(BigNum *q, BigNum *remain, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    BigNum temp1, sub_res, *temp_q;
    size_t cnt;
    int ucompare_result;

    /* TODO: implement a util to init a pre-existed BigNum */
    bn_init(&temp1, arena);
    bn_init(&sub_res, arena);

    MUST(q      != NULL, "q pointer is NULL in bn_udiv");
    MUST(num1   != NULL, "num1 pointer is NULL in bn_udiv");
    MUST(num2   != NULL, "num2 pointer is NULL in bn_udiv");
    MUST(remain != NULL, "remain pointer is NULL in bn_udiv");
    MUST(bn_is_zero(num2) == 0, "num2 is zero, division by zero is undefined in bn_udiv");

    /* num1 == num2, q = 0, remain = num1 */
    if(bn_ucompare(num1, num2) == 1){
        bn_copy(remain, num1, arena);

        if(bn_set_zero(q) < 0){
            return -1;
        }
    }

    bn_copy(&sub_res, num1, arena);
    bn_set_zero(q);

    cnt = 0;
    ucompare_result = bn_ucompare(&sub_res, num2);
    while(ucompare_result == 0 || ucompare_result == 2){
        bn_copy(&temp1, &sub_res, arena);
        if(bn_usubtract(&sub_res, &temp1, num2, arena) < 0){
            return -1;
        }
        ucompare_result = bn_ucompare(&sub_res, num2);
        cnt++;
    }

    /* now remain = sub_res, remain = cnt */
    bn_copy(remain, &sub_res, arena);

    temp_q = bn_from_int(cnt, arena);
    bn_copy(q, temp_q, arena);
    return 0;
}
