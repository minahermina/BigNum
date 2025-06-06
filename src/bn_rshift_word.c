#include<bignum.h>
#include<utils.h>

BigNumWord
bn_rshift_word(const BigNumWord word, size_t nbits)
{
    if(nbits == 0)
        return word;

    if(nbits > BN_WORD_SIZE - 1)
        return 0;

    return (word >> nbits);
}
