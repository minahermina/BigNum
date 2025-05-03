#define ARENA_ALLOCATOR_IMPLEMENTATION
#include <bignum.h>

#include <inttypes.h>
#include <assert.h>

#define ABS(x) (x < 0 ? -x : x)

void bignum_print_word(const BigNumWord word, char format){
    switch(format){

        /*bin format*/
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
        /*hex format*/
        case 'x':{
                #if BIGNUM_WORD_SIZE == 64
                    printf("0x%016" PRIX64 "\n", word);
                #elif BIGNUM_WORD_SIZE == 32
                    printf("0x%08" PRIX32 "\n", word);
                #endif
            break;
        }
        default: break;

    }

}

BigNum* bignum_new(Arena *arena)
{
    BigNum *num = NULL;
    size_t size = BIGNUM_DEFAULT_WORDS_SIZE;

    if(arena == NULL) {
        num = (BigNum*) malloc(sizeof(BigNum));
        if(num == NULL) return NULL;

        num->words = (BigNumWord*) calloc(size, sizeof(BigNumWord));
        if(num->words == NULL) {
            free(num);
            return NULL;
        }
    } else {
        num = (BigNum*) arena_alloc(arena, sizeof(BigNum));
        if(num == NULL) return NULL;

        num->words = (BigNumWord*) arena_alloc(arena, size * sizeof(BigNumWord));
        if(num->words == NULL) return NULL;

    }

    num->size = 1;
    num->capacity = size;
    num->negative = 0;

    return num;
}

void
bignum_resize(BigNum* num, size_t new_size, Arena *arena)
{
    assert(num != NULL);
    if (new_size >= num->capacity){
        num->capacity = new_size * 2;
        if (arena == NULL) {
            num->words = (BigNumWord*)realloc(num->words, (num->capacity * sizeof(BigNumWord)));
        } else {
            num->words = (BigNumWord*)arena_realloc(arena, num->words, num->size, num->capacity);
        }
    }
    assert(num->words != NULL);
    num->size = new_size;
    return ;
}

int
bignum_copy(BigNum *dest, BigNum *src, Arena *arena)
{
    size_t i;
    assert(dest != NULL);
    assert(src != NULL);

    bignum_resize(dest, src->size, arena);

    dest->negative = src->negative;
    for(i = 0; i < dest->size; ++i){
        dest->words[i] = src->words[i];
    }

    return 0;
}


BigNum*
bignum_dup(BigNum *src, Arena *arena)
{
    BigNum *num = NULL;
    assert(src != NULL);

    /*create new BigNum num*/
    num = bignum_new(arena);

    if(num == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }

    /*copy from src to num]*/
    if(bignum_copy(num, src, arena) < 0){
        return NULL;
    }

    return num;
}

void
bignum_free(BigNum *num)
{
    assert(num != NULL);
    assert(num->words != NULL);

    free(num->words);
    free(num);
}

int
bignum_append_word(BigNum *num, const BigNumWord word, Arena *arena)
{
    size_t oldsize;

    assert(num != NULL);

    if(word == 0)
        return 0;

    oldsize = num->size;
    bignum_resize(num, oldsize + 1, arena);

    assert(num->words != NULL);

    // Append the new word
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

    //shift from back to front to avoid overwriting
    for (i = oldsize; i > 0; i--) {
        num->words[i + cnt - 1] = num->words[i - 1];
    }

    // Zero out the prepended cnt words
    for (i = 0; i < cnt; i++) {
        num->words[i] = 0;
    }
    return 0;
}

BigNum*
bignum_from_int(int n, Arena* arena)
{
    BigNum *num = bignum_new(arena);

    if(num == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    if(ABS(n) != n)
        num->negative = 1;

    n = ABS(n);
    bignum_resize(num, 1, arena);
    num->words[0] = n;
    return num;
}

BigNum*
bignum_from_hex(const char *str, size_t len, Arena* arena)
{
    BigNum *num = bignum_new(arena);

    if(num == NULL){
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }

    size_t start_idx = 0, chars_per_word, word_idx, words_size, i, j;
    BigNumWord current_word, value;
    assert(str != NULL);
    assert(num != NULL);

    // Handle sign
    if (str[0] == '-') {
        num->negative = 1;
        start_idx = 1;
    }

    // Handle '0x' or '0X' prefix
    if (len > start_idx + 1 && str[start_idx] == '0' && 
        (str[start_idx + 1] == 'x' || str[start_idx + 1] == 'X')) {
        start_idx += 2;
    }

    // Skip leading zeros
    while (start_idx < len && str[start_idx] == '0') {
        start_idx++;
    }

    // If all zeros or empty, return 0
    if (start_idx >= len) {
        num->size = 1;
        num->words[0] = 0;
        return num;
    }

    chars_per_word = BIGNUM_WORD_SIZE / 4; // 16 for 64-bit, 8 for 32-bit


    /*ensure we have enough words for the hex number  */
    words_size = ((len * 4 ) / BIGNUM_WORD_SIZE) + 1;
    bignum_resize(num, words_size, arena);

    word_idx = 0;
    // Start from the rightmost digit
    for (i = len; i > start_idx; word_idx++) {
        current_word = 0;

        // Process up to chars_per_word characters curren_word (right -> left)
        for (j = 0; j < chars_per_word && i > start_idx; j++, i--) {
            char c = str[i-1];

            if (c >= '0' && c <= '9') {
                value = c - '0';
            } else if (c >= 'a' && c <= 'f') {
                value = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'F') {
                value = c - 'A' + 10;
            } else { /*invalid char*/
                bignum_free(num);
                return NULL;
            }

            // Add value at the position in the current word
            current_word |= value << (j * 4);
        }

        num->words[word_idx] = current_word;
    }

    num->size = word_idx;
    return num;
}



/* int bignum_and(BigNum *res, const BigNum *a, const BigNum *b, Arena *arena);
int bignum_or(BigNum *dv, BigNum *rem, const BigNum *a, const BigNum *d, Arena *arena);
int bignum_xor(BigNum *dv, BigNum *rem, const BigNum *a, const BigNum *d, Arena *arena);  */

/*prints number in Big-endian from highest word (MSB) to lowest word (LSB)*/
void bignum_print(BigNum* num, char format)
{
    assert(num != NULL);
    BigNumWord val;
    int i;

    // Print sign if negative
    if (num->negative) {
        printf("(negative) ");
    }

    switch(format){
        /*bin format*/
        case 'b': {
            for (i = num->size - 1; i >= 0; i--) {
                BigNumWord val = num->words[i];

                for (int byte = BIGNUM_WORD_SIZE/8 - 1; byte >= 0; byte--) {
                    unsigned char current_byte = (val >> (byte * 8)) & 0xFF;

                    for (int bit = 7; bit >= 0; bit--) {
                        printf("%d", (current_byte >> bit) & 1);
                    }

                    // space between bytes for readability
                    if (byte > 0) printf(" ");
                }
            }
            printf("\n");
            break;
        }

        /*hex format*/
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


/*printf the array*/
void
bignum_print_words(const BigNum* num, char format)
{
    assert(num != NULL);
    assert(format == 'b' || format == 'x');
    size_t i;

    BigNumWord word = num->words[i];
    switch(format){
        /*bin format*/
        case 'b': {
            for(i = 0; i < num->size; ++i){
                printf("word[%zu]:   ", i);
                word = num->words[i];
                bignum_print_word(word, 'b');
            }
            break;
        }
        /*hex format*/
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

/*NOTE: this function assume that either num1 or num2 has no leading zero words*/
int
bignum_cmp(const BigNum *num1, const BigNum *num2)
{
    size_t i;
    assert(num1 != NULL || num2 != NULL);
    if((num1->negative != num2->negative) || (num1->size != num2->size)){
        return -1;
    }

    for(i = 0; i < num1->size; ++i){
        if(num1->words[i] != num2->words[i]){
            return -1;
        }
    }

    return 1;
}

/* int
bignum_is_zero(const BigNum *num)
{

}

int
bignum_is_one(const BigNum *num)
{
}
 */
int
bignum_is_negative(const BigNum *num)
{
    return num->negative;
}
