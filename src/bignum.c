#define ARENA_ALLOCATOR_IMPLEMENTATION
#include <bignum.h>
#include <inttypes.h>
#include <assert.h>

#define ABS(x) (x < 0 ? -x : x)

typedef unsigned long WORD;
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

    num->size = 0;
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

void
bignum_free(BigNum *num)
{
    assert(num != NULL);
    assert(num->words != NULL);

    free(num->words);
    free(num);
}

BigNum*
bignum_from_int(int n, Arena* arena)
{
    BigNum *num = bignum_new(arena);
    if(ABS(n) != n)
        num->negative = 1;

    n = ABS(n);
    bignum_append_word(num, (BIGNUM_WORD)n, arena);
    return num;
}

BigNum*
bignum_from_hex(const char *str, size_t len, Arena* arena)
{
    BigNum *num = bignum_new(arena);
    size_t start_idx = 0, chars_per_word, word_idx, words_size, i, j;
    BIGNUM_WORD current_word, value;
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

/*prints number in Big-endian from highest word (MSB) to lowest word (LSB)*/
void bignum_print(BigNum* num, char format)
{
    assert(num != NULL);
    BIGNUM_WORD val;
    int i;

    // Print sign if negative
    if (num->negative) {
        printf("(negative) ");
    }

    switch(format){
        /*bin format*/
        case 'b': {
            for (i = num->size - 1; i >= 0; i--) {
                BIGNUM_WORD val = num->words[i];

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

int8_t
bignum_append_word(BigNum* num, BIGNUM_WORD word, Arena *arena)
{
    size_t newsize = num->size + 1;
    assert(num != NULL);
    if(arena == NULL) {
        /*TODO: implement a simple realloc macro for resizing BigNum (just copy it from the arena_allocator.h))*/
        num->words = (BIGNUM_WORD*) realloc(num->words, newsize);
        assert(num->words != NULL);
    }else {
        num->words = (BIGNUM_WORD*) arena_realloc(arena, num->words, num->size, newsize);
    }
    assert(num->words != NULL);

    num->words[num->size] = word;
    num->size = newsize;
    return 0;
}


const char*
bignum_int2hex(BIGNUM_WORD number)
{
    static char buffer[2048] = {'\0'};
    #if BIGNUM_WORD_SIZE == 64
        snprintf(buffer, sizeof(buffer), "%" PRIX64 "", number);
    #elif BIGNUM_WORD_SIZE  == 32
        snprintf(buffer, sizeof(buffer), "%" PRIX32 "", number);
    #else
        #error "Unsupporetd architecture"
    #endif

    return buffer;
}
