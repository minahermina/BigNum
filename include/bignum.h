#ifndef BIGNUM
#define BIGNUM

#include <stdlib.h>
#include <stdint.h>

#include <arena_allocator.h>


#define DEBUG_BIGNUM(num) \
do {\
    printf("BigNum: %p\n", &(num));       \
    printf("data: %p\n", (num).digits);       \
    printf("size: %d\n", (num).size);         \
    printf("capacity: %d\n", (num).capacity); \
    printf("negative: %d\n", (num).negative); \
} while(0)\

typedef struct {
    uint8_t *digits;
    uint16_t size;
    uint16_t capacity;
    int negative;
} BigNum;

BigNum* BigNum_new(Arena *arena);
uint8_t BigNum_copy(BigNum *dest, BigNum *src, Arena *arena);
BigNum* BigNum_dup(BigNum *src, Arena *arena);
char *BigNum_2dec(const BigNum *num);
char *BigNum_2hex(const BigNum *num);

/* Basic Arithmetic */
int BigNum_add(BigNum *r, const BigNum *a, const BigNum *b);
int BigNum_sub(BigNum *r, const BigNum *a, const BigNum *b);
int BigNum_mul(BigNum *r, const BigNum *a, const BigNum *b, Arena *arena);
int BigNum_div(BigNum *dv, BigNum *rem, const BigNum *a, const BigNum *d, Arena *arena);
int BigNum_mod(BigNum *rem, const BigNum *a, const BigNum *m, Arena *arena);

/* Modular Arithmetic */
int BN_mod_exp(BigNum *r, const BigNum *a, const BigNum *p, const BigNum *m, Arena *arena);
int BN_mod_inverse(BigNum *r, const BigNum *a, const BigNum *n, Arena *arena);

int BN_cmp(const BigNum *a, const BigNum *b);
int BN_is_zero(const BigNum *a);
int BN_is_one(const BigNum *a);
int BN_is_negative(const BigNum *a);

#endif /*BigNum*/
