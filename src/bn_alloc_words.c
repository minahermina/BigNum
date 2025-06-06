#include <bignum.h>
#include <utils.h>

BigNumWord*
bn_alloc_words(size_t size, Arena *arena)
{
    BigNumWord *words;

    if(arena == NULL) {
        words = (BigNumWord*) calloc(size, sizeof(BigNumWord));
        if(words == NULL){
            free(words);
            MUST(words != NULL, "Allocating memory in bn_alloc_words");
        }

    } else {
        words = (BigNumWord*) arena_alloc(arena, size * sizeof(BigNumWord));
        if(words == NULL){
            MUST(words != NULL, "Allocating memory in bn_alloc_words");
        }

    }
    return words;
}
