#ifndef INT128_H
#define INT128_H
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <type_traits>
#endif

#define INT128_MAX \
	((int128){ .low = UINT64_MAX, .high = UINT64_C(0x7fffffffffffffff) })
#define INT128_MIN ((int128){ .low = 0, .high = UINT64_C(0x8000000000000000) })
#define UINT128_MAX ((uint128){ .low = UINT64_MAX, .high = UINT64_MAX })

#define UINT128_C(x) ((uint128){ .low = (uint64_t)(x), .high = 0 })
#define INT128_C(x)                                                 \
	(((x) < 0) ? (int128){ .low = (uint64_t)(x), .high = UINT64_MAX } : \
		     (int128){ .low = (uint64_t)(x), .high = 0 })

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

// Returns lhs < rhs.
static inline bool uint128_less(uint128 lhs, uint128 rhs)
{
	if (lhs.high == rhs.high) {
		return lhs.low < rhs.low;
	}

	return lhs.high < rhs.high;
}

// Returns lhs > rhs.
static inline bool uint128_greater(uint128 lhs, uint128 rhs)
{
	if (lhs.high == rhs.high) {
		return lhs.low > rhs.low;
	}

	return lhs.high > rhs.high;
}

// Returns lhs < rhs.
static inline bool int128_less(int128 lhs, int128 rhs)
{
	if ((int64_t)lhs.high == (int64_t)rhs.high) {
		return lhs.low < rhs.low;
	}

	return (int64_t)lhs.high < (int64_t)rhs.high;
}

// Returns lhs > rhs.
static inline bool int128_greater(int128 lhs, int128 rhs)
{
	if ((int64_t)lhs.high == (int64_t)rhs.high) {
		return lhs.low > rhs.low;
	}

	return (int64_t)lhs.high > (int64_t)rhs.high;
}

// Returns lhs == rhs.
static inline bool int128_eq(int128 lhs, int128 rhs)
{
	return lhs.low == rhs.low && lhs.high == rhs.high;
}

// Returns lhs == rhs.
static inline bool uint128_eq(uint128 lhs, uint128 rhs)
{
	return lhs.low == rhs.low && lhs.high == rhs.high;
}

// Returns lhs <= rhs.
static inline bool uint128_less_eq(uint128 lhs, uint128 rhs)
{
	return uint128_less(lhs, rhs) || uint128_eq(lhs, rhs);
}

// Returns lhs >= rhs.
static inline bool uint128_greater_eq(uint128 lhs, uint128 rhs)
{
	return uint128_greater(lhs, rhs) || uint128_eq(lhs, rhs);
}

// Returns lhs >= rhs.
static inline bool int128_greater_eq(int128 lhs, int128 rhs)
{
	return int128_greater(lhs, rhs) || int128_eq(lhs, rhs);
}

// Returns lhs <= rhs.
static inline bool int128_less_eq(int128 lhs, int128 rhs)
{
	return int128_less(lhs, rhs) || int128_eq(lhs, rhs);
}

// Adds two int128s, wrapping on overflow.
static inline int128 int128_wrapping_add(int128 lhs, int128 rhs)
{
	int128 result = {
		.low = lhs.low + rhs.low,
		.high = lhs.high + rhs.high,
	};

	if (result.low < lhs.low) {
		result.high += 1;
	}

	return result;
}

// Adds two uint128s, wrapping on overflow.
static inline uint128 uint128_wrapping_add(uint128 lhs, uint128 rhs)
{
	uint128 result = {
		.low = lhs.low + rhs.low,
		.high = lhs.high + rhs.high,
	};

	if (result.low < lhs.low) {
		result.high += 1;
	}

	return result;
}

// Returns -x, wrapping on overflow.
static inline int128 int128_wrapping_neg(int128 x)
{
	return (int128){
		.low = ~x.low + 1,
		.high = ~x.high + (x.low == 0),
	};
}

// Returns -x, wrapping on overflow.
static inline uint128 uint128_wrapping_neg(uint128 x)
{
	return (uint128){
		.low = ~x.low + 1,
		.high = ~x.high + (x.low == 0),
	};
}

// Returns ~x.
static inline int128 int128_compl(int128 x)
{
	return (int128){
		.low = ~x.low,
		.high = ~x.high,
	};
}

// Returns ~x.
static inline uint128 uint128_compl(uint128 x)
{
	return (uint128){
		.low = ~x.low,
		.high = ~x.high,
	};
}

// Returns lhs | rhs.
static inline int128 int128_or(int128 lhs, int128 rhs)
{
	return (int128){
		.low = lhs.low | rhs.low,
		.high = lhs.high | rhs.high,
	};
}

// Returns lhs & rhs.
static inline int128 int128_and(int128 lhs, int128 rhs)
{
	return (int128){
		.low = lhs.low & rhs.low,
		.high = lhs.high & rhs.high,
	};
}

// Returns lhs | rhs.
static inline uint128 uint128_or(uint128 lhs, uint128 rhs)
{
	return (uint128){
		.low = lhs.low | rhs.low,
		.high = lhs.high | rhs.high,
	};
}

// Returns lhs & rhs.
static inline uint128 uint128_and(uint128 lhs, uint128 rhs)
{
	return (uint128){
		.low = lhs.low & rhs.low,
		.high = lhs.high & rhs.high,
	};
}

// Returns lhs ^ rhs.
static inline int128 int128_xor(int128 lhs, int128 rhs)
{
	return (int128){
		.low = lhs.low ^ rhs.low,
		.high = lhs.high ^ rhs.high,
	};
}

// Returns lhs ^ rhs.
static inline uint128 uint128_xor(uint128 lhs, uint128 rhs)
{
	return (uint128){
		.low = lhs.low ^ rhs.low,
		.high = lhs.high ^ rhs.high,
	};
}

// Returns lhs << rhs.
static inline uint128 uint128_wrapping_shl(uint128 lhs, int rhs)
{
	rhs &= 127;
	if (rhs == 0) {
		return lhs;
	}
	if (rhs >= 64) {
		return (uint128){ .low = 0, .high = lhs.low << (rhs - 64) };
	}

	return (uint128){
		.low = lhs.low << rhs,
		.high = (lhs.high << rhs) | (lhs.low >> (64 - rhs)),
	};
}

// Returns lhs << rhs.
static inline int128 int128_wrapping_shl(int128 lhs, int rhs)
{
	uint128 a = { .low = lhs.low, .high = lhs.high };
	uint128 result = uint128_wrapping_shl(a, rhs);
	return (int128){ .low = result.low, .high = result.high };
}

// Returns lhs >> rhs.
static inline uint128 uint128_wrapping_shr(uint128 lhs, int rhs)
{
	rhs &= 127;
	if (rhs == 0) {
		return lhs;
	}
	if (rhs >= 64) {
		return (uint128){ .low = lhs.high >> (rhs - 64), .high = 0 };
	}

	return (uint128){
		.low = (lhs.low >> rhs) | (lhs.high << (64 - rhs)),
		.high = (lhs.high >> rhs),
	};
}

// Returns lhs >> rhs.
// On signed int128 this is an arithmetic shift, so the sign bit is used for
// shifted in bits.
static inline int128 int128_wrapping_shr(int128 lhs, int rhs)
{
	rhs &= 127;
	if (rhs == 0) {
		return lhs;
	}
	if ((int64_t)lhs.high >= 0) {
		uint128 a = { .low = lhs.low, .high = lhs.high };
		uint128 result = uint128_wrapping_shr(a, rhs);
		return (int128){ .low = result.low, .high = result.high };
	}

	if (rhs >= 64) {
		return (int128){
			.low = (uint64_t)((int64_t)lhs.high >> (rhs - 64)),
			.high = (uint64_t)((int64_t)lhs.high >> 63),
		};
	}

	return (int128){
		.low = (lhs.low >> rhs) | (lhs.high << (64 - rhs)),
		.high = (uint64_t)((int64_t)lhs.high >> rhs),
	};
}

// Subtracts two int128s, wrapping on underflow.
static inline int128 int128_wrapping_sub(int128 lhs, int128 rhs)
{
	int128 result = {
		.low = lhs.low - rhs.low,
		.high = lhs.high - rhs.high,
	};

	if (lhs.low < rhs.low) {
		result.high -= 1;
	}

	return result;
}

// Subtracts two uint128s, wrapping on underflow.
static inline uint128 uint128_wrapping_sub(uint128 lhs, uint128 rhs)
{
	uint128 result = {
		.low = lhs.low - rhs.low,
		.high = lhs.high - rhs.high,
	};

	if (lhs.low < rhs.low) {
		result.high -= 1;
	}

	return result;
}

// Multiply two unsigned 64 bit integers and get the 128 bit result.
static inline uint128 uint128_mul64(uint64_t lhs, uint64_t rhs)
{
	// Split the low 64 bits of lhs and rhs into its high and low 32 bits.
	uint64_t left_lo32 = lhs & UINT32_MAX;
	uint64_t left_hi32 = lhs >> 32;
	uint64_t right_lo32 = rhs & UINT32_MAX;
	uint64_t right_hi32 = rhs >> 32;

	// Compute each component of the product as the sum of multiple 32 bit
	// products.
	uint64_t lo_lo = left_lo32 * right_lo32;
	uint64_t lo_hi = left_lo32 * right_hi32;
	uint64_t hi_lo = left_hi32 * right_lo32;
	uint64_t hi_hi = left_hi32 * right_hi32;

	uint64_t carry =
		((lo_lo >> 32) + (lo_hi & UINT32_MAX) + (hi_lo & UINT32_MAX)) >>
		32; // we want the high bits of that sum

	// Assemble the final product from these components, adding the carry
	// to the high 64 bits.
	return (uint128){
		.low = lo_lo + (lo_hi << 32) + (hi_lo << 32),
		.high = hi_hi + (lo_hi >> 32) + (hi_lo >> 32) + carry,
	};
}

// Multiply two signed 64 bit integers and get the 128 bit result.
static inline int128 int128_mul64(int64_t lhs, int64_t rhs)
{
	uint128 tmp = uint128_mul64((uint64_t)lhs, (uint64_t)rhs);
	int128 result = {
		.low = tmp.low,
		.high = tmp.high,
	};

	if (lhs < 0) {
		result.high -= (uint64_t)rhs;
	}

	if (rhs < 0) {
		result.high -= (uint64_t)lhs;
	}

	return result;
}

// Returns lhs * rhs.
static inline uint128 uint128_wrapping_mul(uint128 lhs, uint128 rhs)
{
	uint128 i = uint128_mul64(lhs.low, rhs.low);
	uint64_t j = lhs.low * rhs.high;
	uint64_t k = lhs.high * rhs.low;
	i.high += j + k;
	return i;
}

// Returns lhs * rhs.
static inline int128 int128_wrapping_mul(int128 lhs, int128 rhs)
{
	uint128 a = { .low = lhs.low, .high = lhs.high };
	uint128 b = { .low = rhs.low, .high = rhs.high };
	uint128 tmp = uint128_wrapping_mul(a, b);
	return (int128){ .low = tmp.low, .high = tmp.high };
}

static inline int uint64_clz(uint64_t val)
{
#if defined(__GNUC__) || defined(__clang__)
	if (val == 0) return 64;
	return __builtin_clzll(val);
#else
	if (val == 0) return 64;
	int n = 0;
	if ((val & UINT64_C(0xFFFFFFFF00000000)) == 0) { n += 32; val <<= 32; }
	if ((val & UINT64_C(0xFFFF000000000000)) == 0) { n += 16; val <<= 16; }
	if ((val & UINT64_C(0xFF00000000000000)) == 0) { n += 8;  val <<= 8;  }
	if ((val & UINT64_C(0xF000000000000000)) == 0) { n += 4;  val <<= 4;  }
	if ((val & UINT64_C(0xC000000000000000)) == 0) { n += 2;  val <<= 2;  }
	if ((val & UINT64_C(0x8000000000000000)) == 0) { n += 1;  val <<= 1;  }
	return n;
#endif
}

static inline int uint128_clz(uint128 val)
{
	if (val.high != 0) {
		return uint64_clz(val.high);
	}
	return 64 + uint64_clz(val.low);
}

static inline uint128 uint128_divmod(uint128 lhs, uint128 rhs, uint128 *rem)
{
	if (uint128_eq(rhs, UINT128_C(0))) {
		if (rem) {
			*rem = UINT128_C(0);
		}
		return UINT128_MAX;
	}

	if (uint128_greater(rhs, lhs)) {
		if (rem) {
			*rem = lhs;
		}
		return UINT128_C(0);
	}

	if (uint128_eq(lhs, rhs)) {
		if (rem) {
			*rem = UINT128_C(0);
		}
		return UINT128_C(1);
	}

	int lhs_clz = uint128_clz(lhs);
	int rhs_clz = uint128_clz(rhs);
	int shift = rhs_clz - lhs_clz;

	uint128 divisor = uint128_wrapping_shl(rhs, shift);
	uint128 quotient = UINT128_C(0);

	for (int i = 0; i <= shift; i++) {
		quotient = uint128_wrapping_shl(quotient, 1);
		if (uint128_greater_eq(lhs, divisor)) {
			lhs = uint128_wrapping_sub(lhs, divisor);
			quotient.low |= 1;
		}
		divisor = uint128_wrapping_shr(divisor, 1);
	}

	if (rem) {
		*rem = lhs;
	}
	return quotient;
}

// Returns lhs / rhs.
static inline uint128 uint128_wrapping_div(uint128 lhs, uint128 rhs)
{
	return uint128_divmod(lhs, rhs, NULL);
}

// Returns lhs % rhs.
static inline uint128 uint128_wrapping_rem(uint128 lhs, uint128 rhs)
{
	uint128 rem;
	uint128_divmod(lhs, rhs, &rem);
	return rem;
}

// Returns lhs / rhs.
static inline int128 int128_wrapping_div(int128 lhs, int128 rhs)
{
	// signed integers aren't symmetric. INT128_MIN == (-INT128_MAX) - 1
	if (int128_eq(lhs, rhs) && int128_eq(lhs, INT128_MIN)) {
		return INT128_C(1);
	}

	if (int128_eq(lhs, INT128_MIN)) {
		return INT128_C(0);
	}

	bool result_negative = false;
	if (int128_less(lhs, INT128_C(0))) {
		result_negative = !result_negative;
		lhs = int128_wrapping_neg(lhs);
	}

	if (int128_less(rhs, INT128_C(0))) {
		result_negative = !result_negative;
		rhs = int128_wrapping_neg(rhs);
	}

	uint128 a = { .low = lhs.low, .high = lhs.high };
	uint128 b = { .low = rhs.low, .high = rhs.high };
	uint128 c = uint128_wrapping_div(a, b);
	int128 result = { .low = c.low, .high = c.high };

	if (result_negative) {
		return int128_wrapping_neg(result);
	}

	return result;
}

// Returns lhs % rhs.
static inline int128 int128_wrapping_rem(int128 lhs, int128 rhs)
{
	// signed integers aren't symmetric. INT128_MIN == (-INT128_MAX) - 1
	if (int128_eq(lhs, INT128_MIN)) {
		return INT128_C(0);
	}

	if (int128_eq(rhs, INT128_MIN)) {
		return lhs;
	}

	bool result_negative = false;
	if (int128_less(lhs, INT128_C(0))) {
		result_negative = true;
		lhs = int128_wrapping_neg(lhs);
	}

	if (int128_less(rhs, INT128_C(0))) {
		rhs = int128_wrapping_neg(rhs);
	}

	uint128 a = { .low = lhs.low, .high = lhs.high };
	uint128 b = { .low = rhs.low, .high = rhs.high };
	uint128 c = uint128_wrapping_rem(a, b);
	int128 result = { .low = c.low, .high = c.high };

	if (result_negative && int128_greater(result, INT128_C(0))) {
		return int128_wrapping_neg(result);
	}

	return result;
}

// --- C11 and C++ generic APIs ---

#ifdef __cplusplus
// C++ overloaded conversions
inline int128 to_int128(int128 x) { return x; }
inline int128 to_int128(uint128 x) { return (int128){ .low = x.low, .high = x.high }; }

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value, int128>::type
to_int128(T x) {
	int64_t val = (int64_t)x;
	return (val < 0) ? (int128){ .low = (uint64_t)val, .high = UINT64_MAX } : (int128){ .low = (uint64_t)val, .high = 0 };
}

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && !std::is_signed<T>::value, int128>::type
to_int128(T x) {
	return (int128){ .low = (uint64_t)x, .high = 0 };
}

inline uint128 to_uint128(uint128 x) { return x; }
inline uint128 to_uint128(int128 x) { return (uint128){ .low = x.low, .high = x.high }; }

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value, uint128>::type
to_uint128(T x) {
	int64_t val = (int64_t)x;
	return (val < 0) ? (uint128){ .low = (uint64_t)val, .high = UINT64_MAX } : (uint128){ .low = (uint64_t)val, .high = 0 };
}

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && !std::is_signed<T>::value, uint128>::type
to_uint128(T x) {
	return (uint128){ .low = (uint64_t)x, .high = 0 };
}

// C++ overloaded wrapping operations
template <typename T>
inline int128 wrapping_add(int128 lhs, T rhs) { return int128_wrapping_add(lhs, to_int128(rhs)); }
template <typename T>
inline uint128 wrapping_add(uint128 lhs, T rhs) { return uint128_wrapping_add(lhs, to_uint128(rhs)); }

template <typename T>
inline int128 wrapping_sub(int128 lhs, T rhs) { return int128_wrapping_sub(lhs, to_int128(rhs)); }
template <typename T>
inline uint128 wrapping_sub(uint128 lhs, T rhs) { return uint128_wrapping_sub(lhs, to_uint128(rhs)); }

template <typename T>
inline int128 wrapping_mul(int128 lhs, T rhs) { return int128_wrapping_mul(lhs, to_int128(rhs)); }
template <typename T>
inline uint128 wrapping_mul(uint128 lhs, T rhs) { return uint128_wrapping_mul(lhs, to_uint128(rhs)); }

template <typename T>
inline int128 wrapping_div(int128 lhs, T rhs) { return int128_wrapping_div(lhs, to_int128(rhs)); }
template <typename T>
inline uint128 wrapping_div(uint128 lhs, T rhs) { return uint128_wrapping_div(lhs, to_uint128(rhs)); }

template <typename T>
inline int128 wrapping_rem(int128 lhs, T rhs) { return int128_wrapping_rem(lhs, to_int128(rhs)); }
template <typename T>
inline uint128 wrapping_rem(uint128 lhs, T rhs) { return uint128_wrapping_rem(lhs, to_uint128(rhs)); }

inline int128 wrapping_neg(int128 x) { return int128_wrapping_neg(x); }
inline uint128 wrapping_neg(uint128 x) { return uint128_wrapping_neg(x); }

inline int128 wrapping_shl(int128 lhs, int rhs) { return int128_wrapping_shl(lhs, rhs); }
inline uint128 wrapping_shl(uint128 lhs, int rhs) { return uint128_wrapping_shl(lhs, rhs); }

inline int128 wrapping_shr(int128 lhs, int rhs) { return int128_wrapping_shr(lhs, rhs); }
inline uint128 wrapping_shr(uint128 lhs, int rhs) { return uint128_wrapping_shr(lhs, rhs); }

#else // C11 generic implementations

static inline int128 to_int128_impl(int128 x) { return x; }
static inline int128 to_int128_uint128(uint128 x) { return (int128){ .low = x.low, .high = x.high }; }
static inline int128 to_int128_signed(int64_t x) {
	return (x < 0) ? (int128){ .low = (uint64_t)x, .high = UINT64_MAX } : (int128){ .low = (uint64_t)x, .high = 0 };
}
static inline int128 to_int128_unsigned(uint64_t x) {
	return (int128){ .low = x, .high = 0 };
}

#define to_int128(x) _Generic((x), \
	int128: to_int128_impl, \
	uint128: to_int128_uint128, \
	char: to_int128_signed, \
	signed char: to_int128_signed, \
	short: to_int128_signed, \
	int: to_int128_signed, \
	long: to_int128_signed, \
	long long: to_int128_signed, \
	unsigned char: to_int128_unsigned, \
	unsigned short: to_int128_unsigned, \
	unsigned int: to_int128_unsigned, \
	unsigned long: to_int128_unsigned, \
	unsigned long long: to_int128_unsigned \
)(x)

static inline uint128 to_uint128_impl(uint128 x) { return x; }
static inline uint128 to_uint128_int128(int128 x) { return (uint128){ .low = x.low, .high = x.high }; }
static inline uint128 to_uint128_signed(int64_t x) {
	return (x < 0) ? (uint128){ .low = (uint64_t)x, .high = UINT64_MAX } : (uint128){ .low = (uint64_t)x, .high = 0 };
}
static inline uint128 to_uint128_unsigned(uint64_t x) {
	return (uint128){ .low = x, .high = 0 };
}

#define to_uint128(x) _Generic((x), \
	uint128: to_uint128_impl, \
	int128: to_uint128_int128, \
	char: to_uint128_signed, \
	signed char: to_uint128_signed, \
	short: to_uint128_signed, \
	int: to_uint128_signed, \
	long: to_uint128_signed, \
	long long: to_uint128_signed, \
	unsigned char: to_uint128_unsigned, \
	unsigned short: to_uint128_unsigned, \
	unsigned int: to_uint128_unsigned, \
	unsigned long: to_uint128_unsigned, \
	unsigned long long: to_uint128_unsigned \
)(x)

#define wrapping_add(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_add(to_int128(lhs), to_int128(rhs)), \
	uint128: uint128_wrapping_add(to_uint128(lhs), to_uint128(rhs)) \
)

#define wrapping_sub(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_sub(to_int128(lhs), to_int128(rhs)), \
	uint128: uint128_wrapping_sub(to_uint128(lhs), to_uint128(rhs)) \
)

#define wrapping_mul(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_mul(to_int128(lhs), to_int128(rhs)), \
	uint128: uint128_wrapping_mul(to_uint128(lhs), to_uint128(rhs)) \
)

#define wrapping_div(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_div(to_int128(lhs), to_int128(rhs)), \
	uint128: uint128_wrapping_div(to_uint128(lhs), to_uint128(rhs)) \
)

#define wrapping_rem(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_rem(to_int128(lhs), to_int128(rhs)), \
	uint128: uint128_wrapping_rem(to_uint128(lhs), to_uint128(rhs)) \
)

#define wrapping_neg(x) _Generic((x), \
	int128: int128_wrapping_neg(to_int128(x)), \
	uint128: uint128_wrapping_neg(to_uint128(x)) \
)

#define wrapping_shl(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_shl(to_int128(lhs), (rhs)), \
	uint128: uint128_wrapping_shl(to_uint128(lhs), (rhs)) \
)

#define wrapping_shr(lhs, rhs) _Generic((lhs), \
	int128: int128_wrapping_shr(to_int128(lhs), (rhs)), \
	uint128: uint128_wrapping_shr(to_uint128(lhs), (rhs)) \
)

#endif

#endif // INT128_H
