/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

/* printf the array */
void
bn_print_words(const BigNum* num, char format)
{
    MUST(num != NULL, "num pointer is NULL in bn_print_words");
    MUST(format == 'b' || format == 'x', "format is not \'b\' neither \'x\' in bn_print_words");
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
                bn_print_word(word, 'b');
            }
            break;
        }
        /* hex format */
        case 'x': {
            for(i = 0; i < num->size; ++i){
                printf("word[%zu]:  ", i);
                word = num->words[i];
                bn_print_word(word, 'x');
            }
            break;
        }
    }
}
