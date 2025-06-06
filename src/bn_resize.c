/*
 * Copyright (C) 2025 Mina Albert Saeed
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <bignum.h>
#include <utils.h>

void
bn_resize(BigNum* num, size_t newsize, Arena *arena)
{
    MUST(num != NULL, "num pointer is NULL in bn_resize");

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

