/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

int
bn_append_word(BigNum *num, const BigNumWord word, Arena *arena)
{
    size_t oldsize;

    MUST(num != NULL, "num pointer is NULL in bn_append_word");

    if(word == 0)
        return 0;

    oldsize = num->size;
    bn_resize(num, oldsize + 1, arena);

    assert(num->words != NULL);

    /* Append the new word */
    num->words[oldsize] = word;

    return 0;
}
