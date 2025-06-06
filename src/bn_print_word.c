/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

void bn_print_word(const BigNumWord word, char format) {
    switch(format) {
        /* bin format */
        case 'b': {
            for (int byte = (BN_WORD_SIZE/8) - 1; byte >= 0; byte--) {
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
        case 'x': {
            printf("0x");
            #if BN_WORD_SIZE == 64
                for (int i = 14; i >= 0; i -= 2) { unsigned char hex_pair = (word >> (i * 4)) & 0xFF;
                    printf("%02X", hex_pair);
                    if (i > 0) printf(" ");
                }
            #elif BN_WORD_SIZE == 32
                for (int i = 6; i >= 0; i -= 2) {
                    unsigned char hex_pair = (word >> (i * 4)) & 0xFF;
                    printf("%02X", hex_pair);
                    if (i > 0) printf(" ");
                }
            #endif
            printf("\n");
            break;
        }
        default: 
            fprintf(stderr, "this format char is not supported\n");
            break;
    }
}
