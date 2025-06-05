/*
    Copyright (C) 2025  Mina Albert Saeed mina.albert.saeed@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#define ARENA_ALLOCATOR_IMPLEMENTATION
#include <bignum.h>
#include <utils.h>

#include <inttypes.h>
#include <assert.h>
#include <string.h>


#define ABS(x) (x < 0 ? -x : x)


BigNum*
bignum_new(Arena *arena)
{
    BigNum *num = NULL;
    size_t size = BIGNUM_DEFAULT_WORDS_SIZE;

    if(arena == NULL) {
        num = (BigNum*) malloc(sizeof(BigNum));
        MUST(num != NULL, "Allocating Memory in bignum_new");

        num->words = bignum_alloc_words(size, arena);
    } else {
        num = (BigNum*) arena_alloc(arena, sizeof(BigNum));
        MUST(num != NULL, "Allocating Memory in bignum_new");

        num->words = bignum_alloc_words(size, arena );
    }

    num->size = 0;
    num->capacity = size;
    num->negative = 0;

    return num;
}

BigNumWord*
bignum_alloc_words(size_t size, Arena *arena)
{
    BigNumWord *words;

    if(arena == NULL) {
        words = (BigNumWord*) calloc(size, sizeof(BigNumWord));
        if(words == NULL){
            free(words);
            MUST(words != NULL, "Allocating memory in bignum_alloc_words");
        }

    } else {
        words = (BigNumWord*) arena_alloc(arena, size * sizeof(BigNumWord));
        if(words == NULL){
            MUST(words != NULL, "Allocating memory in bignum_alloc_words");
        }

    }
    return words;
}

void
bignum_init(BigNum *num, Arena *arena)
{
    size_t size;
    MUST(num != NULL, "num pointer is NULL in bignum_init");

    size = BIGNUM_DEFAULT_WORDS_SIZE;
    num->words = bignum_alloc_words(size, arena);

    num->size = 0;
    num->capacity = size;
}

BigNum*
bignum_zero(Arena *arena)
{
    BigNum *num;

    num = bignum_new(arena);

    MUST(num != NULL, "num pointer is NULL in bignum_zero");

    num->size = 1;
    num->words[0] = 0;
    return num;
}

int
bignum_set_zero(BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bignum_set_zero");

    if(num->capacity > 0) {
        num->size = 1;
        num->negative = 0;
        num->words[0] = 0;
        return 0;
    }

    return -1;
}

void
bignum_resize(BigNum* num, size_t newsize, Arena *arena)
{
    MUST(num != NULL, "num pointer is NULL in bignum_resize");

    if (newsize >= num->capacity){
        num->capacity = newsize * 2;

        if (arena == NULL) {
            num->words = (BigNumWord*) realloc(num->words, (num->capacity * sizeof(BigNumWord)));
        } else {
            num->words = (BigNumWord*) arena_realloc(arena, num->words, num->size, num->capacity);
        }
    }

    assert(num->words != NULL);
    num->size = newsize;
    return ;
}

void
bignum_copy(BigNum *dest, const BigNum *src, Arena *arena)
{
    size_t i;
    MUST(dest != NULL, "dest pointer is NULL in bignum_copy");
    MUST(src != NULL, "src pointer is NULL in bignum_copy");

    /*dest == src ? do nothing*/
    if(bignum_compare(src, dest) == 2){
        return;
    }

    bignum_resize(dest, src->size, arena);

    dest->negative = src->negative;
    for(i = 0; i < dest->size; ++i){
        dest->words[i] = src->words[i];
    }
}


BigNum*
bignum_dup(BigNum *src, Arena *arena)
{
    BigNum *num = NULL;
    MUST(src != NULL, "src pointer is NULL in bignum_dup");

    /* create new BigNum num */
    num = bignum_new(arena);

    MUST(num != NULL, "Allocating memory in bignum_dup");

    bignum_copy(num, src, arena);

    return num;
}

void
bignum_free(BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bignum_free");
    MUST(num->words != NULL, "num->words pointer is NULL in bignum_free");

    free(num->words);
    free(num);
}

int
bignum_append_word(BigNum *num, const BigNumWord word, Arena *arena)
{
    size_t oldsize;

    MUST(num != NULL, "num pointer is NULL in bignum_append_word");

    if(word == 0)
        return 0;

    oldsize = num->size;
    bignum_resize(num, oldsize + 1, arena);

    assert(num->words != NULL);

    /* Append the new word */
    num->words[oldsize] = word;

    return 0;
}

int
bignum_prepend_zero_words(BigNum *num, size_t cnt, Arena *arena)
{
    size_t oldsize, i;
    assert(num != NULL);

    if (cnt == 0)
        return 0;

    oldsize = num->size;

    bignum_resize(num, oldsize + cnt, arena);

    /* shift from back to front to avoid overwriting */
    for (i = oldsize; i > 0; i--) {
        num->words[i + cnt - 1] = num->words[i - 1];
    }

    /* Zero out the prepended cnt words */
    for (i = 0; i < cnt; i++) {
        num->words[i] = 0;
    }
    return 0;
}

BigNum*
bignum_from_int(int n, Arena* arena)
{
    BigNum *num = bignum_new(arena);

    MUST(num != NULL, "Allocating memory in bignum_from_int");

    if(ABS(n) != n)
        num->negative = 1;

    n = ABS(n);
    bignum_resize(num, 1, arena);
    num->words[0] = n;
    return num;
}


BigNum*
bignum_from_bin(const char *str, size_t len, Arena* arena)
{
    BigNum *num;
    const char *hex_str;

    MUST(str != NULL, "str pointer is NULL in bignum_from_bin");
    MUST(len != 0, "len is 0 in bignum_from_bin");

    hex_str = bin_to_hex(str, len);
    assert(hex_str != NULL);

    num = bignum_from_hex(hex_str, strlen(hex_str), arena);

    return num;
}


BigNum*
bignum_from_dec(const char *str, size_t len, Arena* arena)
{
    BigNum *num;
    const char *hex_str;

    MUST(str != NULL, "str pointer is NULL in bignum_from_dec");
    MUST(len != 0, "len is 0 in bignum_from_dec");

    hex_str = dec_to_hex(str, len);
    assert(hex_str != NULL);

    num = bignum_from_hex(hex_str, strlen(hex_str), arena);


    return num;
}

BigNum*
bignum_from_hex(const char *str, size_t len, Arena* arena)
{
    BigNum *num = bignum_new(arena);

    MUST(num != NULL, "Allocating memory in bignum_from_hex");
    MUST(str != NULL, "str pointer is NULL in bignum_from_hex");

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

    chars_per_word = BIGNUM_WORD_SIZE / 4; /* 16 for 64-bit, 8 for 32-bit */


    /* ensure we have enough words for the hex number */
    words_size = ((len * 4 ) / BIGNUM_WORD_SIZE) + 1;
    bignum_resize(num, words_size, arena);

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
                if(arena == NULL) bignum_free(num);
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



void bignum_print_word(const BigNumWord word, char format){

    switch(format){
        /* bin format */
        case 'b':{
            for (int byte = (BIGNUM_WORD_SIZE/8) - 1; byte >= 0; byte--) {
                unsigned char current_byte = (word >> (byte * 8)) & 0xFF;
                for (int bit = 7; bit >= 0; bit--) {
                    printf("%d", (current_byte >> bit) & 1);
                }
                printf(" ");
            }
            printf("\n");
            break;
        }
        /* hex format */
        case 'x':{
                #if BIGNUM_WORD_SIZE == 64
                    printf("0x%016" PRIX64 "\n", word);
                #elif BIGNUM_WORD_SIZE == 32
                    printf("0x%08" PRIX32 "\n", word);
                #endif
            break;
        }
        default: 
            fprintf(stderr, "this format char is not supported");
            break;
    }

}

/* prints number in Big-endian from highest word (MSB) to lowest word (LSB)*/
void bignum_print(const BigNum* num, char format)
{
    MUST(num != NULL, "num pointer is NULL in bignum_print");
    BigNumWord val;
    unsigned char current_byte;
    int i, byte, bit;

    if (num->negative == 1) {
        printf("(negative)\n");
    }

    switch(format){
        /* bin format */
        case 'b': {
            for (i = num->size - 1; i >= 0; i--) {
                val = num->words[i];

                for (byte = BIGNUM_WORD_SIZE/8 - 1; byte >= 0; byte--) {
                    current_byte = (val >> (byte * 8)) & 0xFF;

                    for (bit = 7; bit >= 0; bit--) {
                        printf("%d", (current_byte >> bit) & 1);
                    }

                    /* space between bytes for readability */
                    if (byte > 0) printf(" ");
                }
            }
            printf("\n");
            break;
        }

        /* hex format */
        case 'x' :{
            printf("0x");
            for (i = num->size - 1; i >= 0; i--) {
                val = num->words[i];

                #if BIGNUM_WORD_SIZE == 64
                    printf("%016" PRIX64 "\n", val);
                #elif BIGNUM_WORD_SIZE == 32
                    printf("%08" PRIX32 "\n", val);
                #endif
            }
            break;
        }
    }
}


/* printf the array */
void
bignum_print_words(const BigNum* num, char format)
{
    MUST(num != NULL, "num pointer is NULL in bignum_print_words");
    MUST(format == 'b' || format == 'x', "format is not \'b\' neither \'x\' in bignum_print_words");
    size_t i = 0;

    if (num->negative == 1) {
        printf("(negative)\n");
    }

    BigNumWord word = num->words[i];
    switch(format){
        /* bin format */
        case 'b': {
            for(i = 0; i < num->size; ++i){
                printf("word[%zu]:   ", i);
                word = num->words[i];
                bignum_print_word(word, 'b');
            }
            break;
        }
        /* hex format */
        case 'x': {
            for(i = 0; i < num->size; ++i){
                printf("word[%zu]:  ", i);
                word = num->words[i];
                bignum_print_word(word, 'x');
            }
            break;
        }
    }
}

/*
 *  NOTE: this function assume that either num1 or num2 has no leading zero words
 * -1 --> indicates error
 *  0 --> num1 > num2
 *  1 --> num1 < num2
 *  2 --> num1 = num2
*/

int
bignum_ucompare(const BigNum *num1, const BigNum *num2)
{
    size_t i;
    MUST(num1 != NULL, "num1 pointer is NULL in bignum_ucompare");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_ucompare");

    if(num1->size > num2->size){
        return 0;
    }
    else if(num1->size < num2->size){
        return  1;
    }

    /* num1->size == num2->size */
    for(i = 0; i < num1->size; ++i){
        if(num1->words[i] > num2->words[i]){
            return  0;
        }

        if(num1->words[i] < num2->words[i]){
            return  1;
        }
    }

    return 2;
}

int
bignum_compare(const BigNum *num1, const BigNum *num2)
{
    int ucompare_result;

    MUST(num1 != NULL, "num1 pointer is NULL in bignum_compare");
    MUST(num2 != NULL, "num2 pointer is NULL in bignum_compare");

    /* signs differ */
    if (bignum_is_negative(num1) != bignum_is_negative(num2)) {
        return bignum_is_negative(num1) ? 1 : 0;
    }

    ucompare_result = bignum_ucompare(num1, num2);

    /* both signs are (-) */
    if (bignum_is_negative(num1)) {
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

int
bignum_is_zero(const BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bignum_is_zero");

    if(num->size == 1){
        return (num->words[0] == 0);
    }

    return 0;
}

int
bignum_is_one(const BigNum *num)
{
    MUST(num != NULL, "num pointer is NULL in bignum_is_one");

    if(num->size == 1){
        return (num->words[0] == 1);
    }

    return 0;
}

int
bignum_num_bits_word(BigNumWord word)
{
    int bits;
    if(word == 0){
        return 0;
    }

    bits = 0;
    while(word > 0){
        bits++;
        word >>= 1;
    }
    return bits;
}

int
bignum_num_bits(const BigNum *num)
{
    size_t top_index;
    BigNumWord top_word;
    int bits;

    MUST(num != NULL, "num pointer is NULL in bignum_num_bits");

    if (bignum_is_zero(num)) {
        return 0;
    }

    top_index = num->size - 1;
    top_word = num->words[top_index];

    bits = bignum_num_bits_word(top_word);

    return bits + (top_index * BIGNUM_WORD_SIZE);
}

int
bignum_num_bytes(const BigNum *num)
{
    size_t bits;
    MUST(num != NULL, "num pointer is NULL in bignum_num_bytes");

    if(bignum_is_zero(num)){
        return 0;
    }

    bits = bignum_num_bits(num);
    return (bits  + 7) / 8;
}

int
bignum_is_negative(const BigNum *num)
{
    return num->negative;
}

/*
 * Helper function to validate inputs and calc word and bit indices.
 */
static int
bignum_get_indices(const BigNum *num, int n, size_t *word_idx, size_t *bit_idx)
{
    int bits;

    bits = bignum_num_bits(num);
    if (n < 0 || n > bits) {
        return -1;
    }

    *word_idx = n / BIGNUM_WORD_SIZE;
    *bit_idx = n % BIGNUM_WORD_SIZE;
    return 0;
}

int
bignum_set_bit(BigNum *num, int n)
{
    size_t word_idx, bit_idx;
    MUST(num != NULL, "num pointer is NULL in bignum_set_bit");

    if (bignum_get_indices(num, n, &word_idx, &bit_idx) != 0) {
        return -1;
    }
    num->words[word_idx] |= ((BigNumWord)1 << bit_idx);
    return 0;
}

int
bignum_unset_bit(BigNum *num, int n)
{
    size_t word_idx, bit_idx;
    MUST(num != NULL, "num pointer is NULL in bignum_unset_bit");

    if (bignum_get_indices(num, n, &word_idx, &bit_idx) != 0) {
        return -1;
    }
    num->words[word_idx] &= ~((BigNumWord)1 << bit_idx);
    return 0;
}

int
bignum_is_bit_set(const BigNum *num, int n)
{
    size_t word_idx, bit_idx;
    MUST(num != NULL, "num pointer is NULL in bignum_is_bit_set");

    if (bignum_get_indices(num, n, &word_idx, &bit_idx) != 0) {
        return -1;
    }
    return (num->words[word_idx] & ((BigNumWord)1 << bit_idx)) ? 1 : 0;
}
