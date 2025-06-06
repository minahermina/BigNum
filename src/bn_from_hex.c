#include <bignum.h>
#include <utils.h>

BigNum*
bn_from_hex(const char *str, size_t len, Arena* arena)
{
    BigNum *num = bn_new(arena);

    MUST(num != NULL, "Allocating memory in bn_from_hex");
    MUST(str != NULL, "str pointer is NULL in bn_from_hex");

    size_t start_idx = 0, chars_per_word, word_idx, words_size, i, j;
    BigNumWord current_word, value;

    /* Handle sign */
    if (str[0] == '-') {
        num->negative = 1;
        start_idx = 1;
    }

    /* Handle '0x' or '0X' prefix */
    if (len > start_idx + 1 && str[start_idx] == '0' && 
        (str[start_idx + 1] == 'x' || str[start_idx + 1] == 'X')) {
        start_idx += 2;
    }

    /* Skip leading zeros */
    while (start_idx < len && str[start_idx] == '0') {
        start_idx++;
    }

    /* If all zeros or empty, return 0 */
    if (start_idx >= len) {
        num->size = 1;
        num->words[0] = 0;
        return num;
    }

    chars_per_word = BN_WORD_SIZE / 4; /* 16 for 64-bit, 8 for 32-bit */


    /* ensure we have enough words for the hex number */
    words_size = ((len * 4 ) / BN_WORD_SIZE) + 1;
    bn_resize(num, words_size, arena);

    word_idx = 0;
    /* Start from the rightmost digit */
    for (i = len; i > start_idx; word_idx++) {
        current_word = 0;

        /* Process up to chars_per_word characters curren_word (right -> left) */
        for (j = 0; j < chars_per_word && i > start_idx; j++, i--) {
            char c = str[i-1];

            if (c >= '0' && c <= '9') {
                value = c - '0';
            } else if (c >= 'a' && c <= 'f') {
                value = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'F') {
                value = c - 'A' + 10;
            } else { /*invalid char*/
                if(arena == NULL) bn_free(num);
                return NULL;
            }

            /* Add value at the position in the current word */
            current_word |= value << (j * 4);
        }

        num->words[word_idx] = current_word;
    }

    num->size = word_idx;
    return num;
}
