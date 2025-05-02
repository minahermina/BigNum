#ifndef BIGNUM
#define BIGNUM

#include <stdlib.h>
#include <stdint.h>
#include <arena_allocator.h>

#if defined(__x86_64__)
    typedef uint64_t BigNumWord;
    #define BIGNUM_WORD_SIZE 64
    #define BIGNUM_MASK1 ((BigNumWord)-1)
#elif defined(__i386__) || defined(__i686__)
    typedef uint32_t BigNumWord;
    #define BIGNUM_WORD_SIZE 32
    #define BIGNUM_MASK1 ((BigNumWord)-1)
#endif


#define DEBUG_BIGNUM(num) \
do {\
    printf("--------------------------------\n");  \
    printf("BigNum: %p\n", &(num));                \
    printf("data: %p\n", (num).words);             \
    printf("size: %zu\n", (num).size);             \
    printf("capacity: %zu\n", (num).capacity);     \
    printf("negative: %d\n", (num).negative);      \
} while(0)\

typedef struct {
    BigNumWord *words;
    size_t size;
    size_t capacity;
    int negative;
} BigNum;


const char* bignum_int2hex(BIGNUM_WORD number);

BigNum* bignum_new(Arena *arena);
void bignum_free(BigNum *num);
BigNum* bignum_dup(BigNum *src, Arena *arena);
void bignum_print_words(BigNum* num, char format);
void bignum_print(BigNum* num, char format);
int bignum_append_word(BigNum* num, const BigNumWord word, Arena *arena); /*done*/
void bignum_resize(BigNum* num, size_t new_size, Arena *arena);          /*done*/
int bignum_copy(BigNum *dest, BigNum *src, Arena *arena);               /*done*/
void bignum_print_words(const BigNum* num, char format);                      /*done*/

/*Conversion*/
int *bignum_bn2dec(const BigNum *num, char *dest, size_t len);
int *bignum_bn2hex(const BigNum *num, char *dest, size_t len);
int *bignum_bn2bin(const BigNum *num, char *dest, size_t len);
BigNum *bignum_from_int(int n, Arena* arena);
BigNum *bignum_from_bin(const char *str, size_t len);
BigNum *bignum_from_dec(const char *str, size_t len, Arena* arena);
BigNum *bignum_from_hex(const char *str, size_t len, Arena* arena);

/* Basic Arithmetic */
int bignum_add(BigNum *res, const BigNum *a, const BigNum *b);
int bignum_sub(BigNum *res, const BigNum *a, const BigNum *b);
int bignum_mul(BigNum *res, const BigNum *a, const BigNum *b, Arena *arena);
int bignum_div(BigNum *dv, BigNum *rem, const BigNum *a, const BigNum *d, Arena *arena);
int bignum_mod(BigNum *rem, const BigNum *a, const BigNum *m, Arena *arena);

int bignum_lshift(BigNum *res, BigNum *num, size_t nbits, Arena *arena);
BigNumWord bignum_lshift_word(BigNumWord word, size_t nbits);
/* Modular Arithmetic */
int bignum_mod_exp(BigNum *res, const BigNum *a, const BigNum *p, const BigNum *m, Arena *arena);
int bignum_mod_inverse(BigNum *res, const BigNum *a, const BigNum *n, Arena *arena);

int bignum_cmp(const BigNum *a, const BigNum *b);
int bignum_is_zero(const BigNum *a);
int bignum_is_one(const BigNum *a);
int bignum_is_negative(const BigNum *a);

#endif /*BigNum*/
