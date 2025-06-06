#include <bignum.h>
#include <utils.h>

BigNum*
bn_from_bin(const char *str, size_t len, Arena* arena)
{
    BigNum *num;
    const char *hex_str;

    MUST(str != NULL, "str pointer is NULL in bn_from_bin");
    MUST(len != 0, "len is 0 in bn_from_bin");

    hex_str = bin_to_hex(str, len);
    assert(hex_str != NULL);

    num = bn_from_hex(hex_str, strlen(hex_str), arena);

    return num;
}
