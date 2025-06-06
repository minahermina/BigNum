#include <bignum.h>
#include <utils.h>

/* prints number in Big-endian from highest word (MSB) to lowest word (LSB)*/
void bn_print(const BigNum* num, char format)
{
    MUST(num != NULL, "num pointer is NULL in bn_print");
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

                for (byte = BN_WORD_SIZE/8 - 1; byte >= 0; byte--) {
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

                #if BN_WORD_SIZE == 64
                    printf("%016" PRIX64 "\n", val);
                #elif BN_WORD_SIZE == 32
                    printf("%08" PRIX32 "\n", val);
                #endif
            }
            break;
        }
    }
}
