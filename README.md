# BigNum Library

## ⚠️ Warning

> - **Note:** This project is not yet organized.
> - **Note:** Some core functionalities of this library are also not yet finished.

## Description

**BigNum** is a C99 library for arbitrary-precision arithmetic, optimized for large integers with support for 32-bit or 64-bit word sizes.
It is inspired by the [OpenSSL BIGNUM library](https://docs.openssl.org/1.0.2/man3/bn/)  & [libtommath](https://github.com/libtom/libtommath) and aims to provide utilities for manipulating large numbers that works in most **Unix-like systems**.
## Purpose

The BigNum library is being implemented as the main component for future projects, particularly for implementing the some cryptography algorithms.

## Dependencies

- **GNU Make**  
  Make sure **GNU Make** is installed on your system.

  ### Installation

  - **OpenBSD:**
    ```sh
    doas pkg_add gmake
    ```

  - **Debian-based Linux systems:**
    ```sh
    sudo apt update
    sudo apt install make
    ```

> Note: On most Linux systems, `make` refers to GNU Make by default.

## Building the Library

To build the BigNum library, run the appropriate make command for your system:

- **Linux:** `make release`
- **BSD systems:** `gmake release`

This will create `build/libbignum.so` - the shared library file that you'll link with your applications.

## Using the Library

### Include the Header

In your C file (e.g., `main.c`), include the BigNum header file in the same directory:

```c
#include "bignum.h"
```

### Example Usage

```c
#include "bignum.h"

int main()
{
    Arena arena = {0};
    arena_init(&arena, ARENA_REGION_DEFAULT_CAPACITY);

    char *num1_chars = "984128321312346744073709551615";
    char *num2_chars = "9129391293129312399129391293912939";

    BigNum *num1 = bn_from_dec(num1_chars, strlen(num1_chars), &arena);

    BigNum *num2 = bn_from_dec(num2_chars, strlen(num2_chars), &arena);
    BigNum *res = bn_new(&arena);

    bn_add(res, num1, num2, &arena);
    bn_print(&res, 'x');

    return 0;
}
```

## Compilation Steps

### 1. Set up Environment

```sh
# 1. export LD_LIBRARY_PATH="/path/to/libbignum.so:$LD_LIBRARY_PATH"
# For example to compile main.c in this project you must:
export LD_LIBRARY_PATH="$(pwd)/build/:$LD_LIBRARY_PATH"
# This exports the directory path containing libbignum.so to enable linking to the library.

# 2. Compile Your Program
gcc main.c -L$(pwd)/build -lbignum -o main
```

## Memory Management

The library provides two memory management options:

- **Arena-based:** Pass an `Arena` pointer to functions for automatic memory management within the arena scope
- **Manual:** Pass `NULL` instead of an arena pointer, but remember to call `bn_free()` on BigNum instances when done

## Current Status

- Functions like `bn_gcd()`, `bn_div()`, `bn_mul()`, `bn_mod()`, `bn_mod_pow()` are implemented in naive slow way as i wanted them just to work.

## Future Plans
- Check the `TODOs` section in `include/bignum.h`

