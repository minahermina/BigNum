#define ARENA_ALLOCATOR_IMPLEMENTATION
#include <bignum.h>
#include <inttypes.h>
#include <assert.h>

#define BIGNUM_SIZE sizeof(BigNum)
#define BIGNUM_SIZE sizeof(BigNum)
#define ABS(x) (x < 0 ? -x : x)

typedef unsigned long WORD;

BigNum*
bignum_new(Arena *arena)
{
    BigNum *num = {0};
    size_t size = BIGNUM_DEFAULT_WORDS_SIZE;

    if(arena == NULL) {
        num = (BigNum*)malloc(BIGNUM_SIZE);
        num->words = (BIGNUM_WORD*)malloc(size);
    } else{
        num = (BigNum*)arena_alloc(arena, BIGNUM_SIZE);
        num->words = (BIGNUM_WORD*) arena_alloc(arena, size);
    }

    assert(num != NULL);
    assert(num->words != NULL);
    num->size = 0;
    num->capacity = size;
    num->negative = 0;

    return num;
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
int8_t
bignum_add_word(BigNum* num, BIGNUM_WORD word, Arena *arena)
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
    static char buffer[256] = {'\0'};
    #if BIGNUM_WORD_SIZE == 64
        snprintf(buffer, sizeof(buffer), "%" PRIX64 "", number);
    #elif BIGNUM_WORD_SIZE  == 32
        snprintf(buffer, sizeof(buffer), "%" PRIX32 "", number);
    #else
        #error "Unsupporetd architecture"
    #endif


    return buffer;
}
