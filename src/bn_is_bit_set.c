#include <bignum.h>
#include <utils.h>

int
bn_is_bit_set(const BigNum *num, int n)
{
    size_t word_idx, bit_idx;
    MUST(num != NULL, "num pointer is NULL in bn_is_bit_set");

    if (bn_get_indices(num, n, &word_idx, &bit_idx) != 0) {
        return -1;
    }
    return (num->words[word_idx] & ((BigNumWord)1 << bit_idx)) ? 1 : 0;
}
