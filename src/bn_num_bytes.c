#include <bignum.h>
#include <utils.h>

int
bn_num_bytes(const BigNum *num)
{
    size_t bits;
    MUST(num != NULL, "num pointer is NULL in bn_num_bytes");

    if(bn_is_zero(num)){
        return 0;
    }

    bits = bn_num_bits(num);
    return (bits  + 7) / 8;
}
