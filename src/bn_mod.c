#include<bignum.h>
#include<utils.h>
/*
 * Computing q in bn_udiv and bn_div introduce unnecessary overhead.
 * so refactoring these functions to be more general and avoid computing q when not needed.  
 
*/
int
bn_mod(BigNum *res, const BigNum *num1, const BigNum *num2, Arena *arena)
{
    MUST(res != NULL, "res pointer is NULL in bn_mod");
    MUST(num1 != NULL, "num1 pointer is NULL in bn_mod");
    MUST(num2 != NULL, "num2 pointer is NULL in bn_mod");

    BigNum *q = bn_new(arena);
    if(bn_div(q, res, num1, num2, arena) < 0){
        return -1;
    }

    if(arena == NULL){
        bn_free(q);
    }
    return 0;
}
