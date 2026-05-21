# 128-Bit Integer Library for C/C++

A header-only 128-bit integer library for C and C++ designed specifically with 32-bit microcontroller constraints in mind.

The library is written in valid C11 (using `_Generic` macros for a clean, unified, and type-generic interface) and is fully C++11+ compatible (using template overloads), compiling with zero warnings under `-Wall -Wextra`.

---

## Table of Contents
1. [Overview & Design Goals](#overview--design-goals)
2. [Types & Structures](#types--structures)
3. [Constants & Initialization](#constants--initialization)
4. [Explicit Type Conversion](#explicit-type-conversion)
5. [Type-Generic Arithmetic & Shifts](#type-generic-arithmetic--shifts)
6. [Comparison Functions](#comparison-functions)
7. [Bitwise Operations](#bitwise-operations)
8. [64-bit Multiplication & Utilities](#64-bit-multiplication--utilities)
9. [Usage Examples](#usage-examples)

---

## Overview & Design Goals

This library provides standard 128-bit integer arithmetic for systems lacking native compiler support (like `__int128` on many 32-bit microcontroller toolchains). 

### Key Features:
- **Rust-Style API**: Clear wrapping semantics (`wrapping_add`, `wrapping_sub`, etc.) so that overflow behavior is explicit and well-defined.
- **Literal Support & Auto-Promotion**: You can pass standard integer literals directly as the second argument (e.g. `wrapping_add(my_u128, 5)`). The API automatically promotes primitives to `int128`/`uint128`.
- **Microcontroller Optimized**: 
  - Reduced cross-multiplication in multiplication (minimizing temporary variables and instructions).
  - Division uses a fast, unified leading-zero count (`clz`) binary-search alignment algorithm.
- **Zero Warnings**: Compiles cleanly with `-Wall -Wextra` on GCC and Clang in C11 and C++11/14/17/20 modes.

---

## Types & Structures

The library defines two layout-compatible structs to represent signed and unsigned 128-bit integers:

```c
struct int128 {
    uint64_t low;
    uint64_t high;
};

struct uint128 {
    uint64_t low;
    uint64_t high;
};

typedef struct int128 int128;
typedef struct uint128 uint128;
```

> [!NOTE]
> Since the layouts are identical, they can be cast to each other or copied directly. However, the generic API uses their distinct types to select signed vs. unsigned behavior (e.g. for division and right-shifting).

---

## Constants & Initialization

The library defines limits and constant-creation macros:

### Constants
*   `INT128_MIN`: The minimum representation for signed 128-bit integers (`-2^127`).
    - Value: `{ .low = 0, .high = 0x8000000000000000 }`
*   `INT128_MAX`: The maximum representation for signed 128-bit integers (`2^127 - 1`).
    - Value: `{ .low = UINT64_MAX, .high = 0x7fffffffffffffff }`
*   `UINT128_MAX`: The maximum representation for unsigned 128-bit integers (`2^128 - 1`).
    - Value: `{ .low = UINT64_MAX, .high = UINT64_MAX }`

### Initialization Macros
*   `INT128_C(val)`: Creates an `int128` from a primitive integer literal or variable. If `val` is negative, it correctly performs sign-extension across the full 128-bit width.
*   `UINT128_C(val)`: Creates a `uint128` from a primitive integer literal or variable.

> [!WARNING]
> **Preprocessor Quirk for Compound Literals:**
> When passing a compound literal directly as a macro argument (e.g., `(int128){ .low = 1, .high = 0 }`), the preprocessor interprets the comma inside the braces as a macro parameter separator.
> **Fix:** Wrap the compound literal in parentheses:
> ```c
> wrapping_add(((int128){ .low = 1, .high = 0 }), 5)
> ```

---

## Explicit Type Conversion

Explicit conversions let you safely cast to and from 128-bit types:

*   `to_int128(x)`: Converts `x` to a signed `int128`.
    - **Accepts**: `int128`, `uint128`, or any standard primitive integer (`char`, `int`, `uint64_t`, etc.).
    - **Behavior**: Performs sign-extension if the argument is a signed primitive type.
*   `to_uint128(x)`: Converts `x` to an unsigned `uint128`.
    - **Accepts**: `int128`, `uint128`, or any standard primitive integer.
    - **Behavior**: Zero-extends the argument.

---

## Type-Generic Arithmetic & Shifts

These operations use C11 `_Generic` macros in C and template overloads in C++ to support a unified, clean syntax. 

The first argument (`lhs`) determines the return type and operation type (`int128` or `uint128`). The second argument (`rhs`) can be of the same type, the opposite 128-bit type, or any primitive integer type (which is promoted automatically using `to_int128`/`to_uint128`).

| Function Macro | Return Type | Description |
| :--- | :--- | :--- |
| `wrapping_add(lhs, rhs)` | Matches `lhs` | Adds `lhs` and `rhs`, wrapping on overflow/underflow. |
| `wrapping_sub(lhs, rhs)` | Matches `lhs` | Subtracts `rhs` from `lhs`, wrapping on overflow/underflow. |
| `wrapping_mul(lhs, rhs)` | Matches `lhs` | Multiplies `lhs` by `rhs`, wrapping on overflow. |
| `wrapping_div(lhs, rhs)` | Matches `lhs` | Divides `lhs` by `rhs`. Division by zero returns `UINT128_MAX` (for `uint128`) or `INT128_MAX` (for `int128`). |
| `wrapping_rem(lhs, rhs)` | Matches `lhs` | Remainder of `lhs / rhs`. Division by zero returns `0`. |
| `wrapping_neg(x)` | Matches `x` | Two's complement negation of `x`. Negating `INT128_MIN` wraps back to `INT128_MIN`. |
| `wrapping_shl(lhs, rhs)` | Matches `lhs` | Left shifts `lhs` by `rhs` bits. Shift count is masked modulo 128 (`rhs & 127`) to prevent undefined behavior. |
| `wrapping_shr(lhs, rhs)` | Matches `lhs` | Right shifts `lhs` by `rhs` bits. Shift count is masked modulo 128. For signed `int128`, this is an **arithmetic** shift (sign-preserving). For unsigned `uint128`, this is a **logical** shift (zero-filling). |

---

## Comparison Functions

To prevent hidden comparison bugs and maintain strict type-safety, comparison functions are **not** type-generic. You must call the function prefix matching your type.

### Unsigned (`uint128`) Comparisons
*   `bool uint128_eq(uint128 lhs, uint128 rhs)`: Returns `true` if `lhs == rhs`.
*   `bool uint128_less(uint128 lhs, uint128 rhs)`: Returns `true` if `lhs < rhs`.
*   `bool uint128_greater(uint128 lhs, uint128 rhs)`: Returns `true` if `lhs > rhs`.
*   `bool uint128_less_eq(uint128 lhs, uint128 rhs)`: Returns `true` if `lhs <= rhs`.
*   `bool uint128_greater_eq(uint128 lhs, uint128 rhs)`: Returns `true` if `lhs >= rhs`.

### Signed (`int128`) Comparisons
*   `bool int128_eq(int128 lhs, int128 rhs)`: Returns `true` if `lhs == rhs`.
*   `bool int128_less(int128 lhs, int128 rhs)`: Returns `true` if `lhs < rhs`.
*   `bool int128_greater(int128 lhs, int128 rhs)`: Returns `true` if `lhs > rhs`.
*   `bool int128_less_eq(int128 lhs, int128 rhs)`: Returns `true` if `lhs <= rhs`.
*   `bool int128_greater_eq(int128 lhs, int128 rhs)`: Returns `true` if `lhs >= rhs`.

---

## Bitwise Operations

Bitwise functions are layout-compatible and type-specific:

### Unsigned (`uint128`) Bitwise
*   `uint128 uint128_compl(uint128 x)`: Returns bitwise NOT (`~x`).
*   `uint128 uint128_and(uint128 lhs, uint128 rhs)`: Returns bitwise AND (`lhs & rhs`).
*   `uint128 uint128_or(uint128 lhs, uint128 rhs)`: Returns bitwise OR (`lhs | rhs`).
*   `uint128 uint128_xor(uint128 lhs, uint128 rhs)`: Returns bitwise XOR (`lhs ^ rhs`).

### Signed (`int128`) Bitwise
*   `int128 int128_compl(int128 x)`: Returns bitwise NOT (`~x`).
*   `int128 int128_and(int128 lhs, int128 rhs)`: Returns bitwise AND (`lhs & rhs`).
*   `int128 int128_or(int128 lhs, int128 rhs)`: Returns bitwise OR (`lhs | rhs`).
*   `int128 int128_xor(int128 lhs, int128 rhs)`: Returns bitwise XOR (`lhs ^ rhs`).

---

## 64-bit Multiplication & Utilities

These underlying functions are exposed for cases where you want to produce a 128-bit product from 64-bit inputs or count leading zeros directly:

*   `uint128 uint128_mul64(uint64_t lhs, uint64_t rhs)`: Multiplies two unsigned 64-bit values to yield a full 128-bit product without overflow.
*   `int128 int128_mul64(int64_t lhs, int64_t rhs)`: Multiplies two signed 64-bit values to yield a full 128-bit product (performs correct sign-extension of the product).
*   `int uint128_clz(uint128 val)`: Counts the leading zero bits of a 128-bit integer. Returns `128` if `val` is zero. Highly optimized using compiler-intrinsic `clz` where available, otherwise falls back to a branchless tree-search.

---

## Usage Examples

### C11 Implementation Example
```c
#include <stdio.h>
#include "int128.h"

int main() {
    // 1. Initialize using constants or macros
    uint128 a = UINT128_C(10);
    
    // 2. Perform arithmetic with literal promotion
    uint128 b = wrapping_add(a, 5); // b is 15
    
    // 3. Wrapping behavior
    uint128 c = wrapping_sub(b, 20); // c wraps to UINT128_MAX - 4
    
    // 4. Comparison
    if (uint128_less(c, a)) {
        printf("c wraps below a\n");
    }

    // 5. Explicit conversion from signed to unsigned
    int128 neg_val = INT128_C(-1);
    uint128 converted = to_uint128(neg_val); // converted becomes UINT128_MAX
    
    if (uint128_eq(converted, UINT128_MAX)) {
        printf("Conversion preserved bit pattern correctly!\n");
    }

    return 0;
}
```

### C++11/C++17 Implementation Example
```cpp
#include <iostream>
#include "int128.h"

int main() {
    int128 x = INT128_C(-5);
    
    // Auto-promotion via C++ template overloads
    int128 y = wrapping_add(x, 10); // y = 5
    
    if (int128_greater(y, x)) {
        std::cout << "y (" << (long long)y.low << ") is greater than x\n";
    }

    return 0;
}
```
