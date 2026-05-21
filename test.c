#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "int128.h"

void int128_tests()
{
	// Left Shift
	int128 a = wrapping_add(INT128_C(0), (uint64_t)1 << 63);
	int128 b = wrapping_shl(a, 1);
	assert(b.high == 1);

	a = (int128){ .low = 0, .high = 1 };
	b = wrapping_shr(a, 1);
	assert(b.low == ((uint64_t)1 << 63));

	// Left shift of more than 63 bits?
	assert(int128_eq(wrapping_shl(INT128_C(1), 64),
			 (int128){ .low = 0, .high = 1 }));

	// Right Shift
	// For int128, this should be an arithmetic shift, meaning the sign bit
	// is preserved.
	a = INT128_C(-8);
	assert(int128_less(wrapping_shr(a, 1), INT128_C(0)));

	// Right shift of more than 63 bits?
	assert(int128_eq(wrapping_shr(((int128){ .low = 0, .high = 1 }), 64),
			 INT128_C(1)));
	assert(int128_eq(wrapping_shr(((int128){ .low = UINT64_MAX,
						 .high = 0xfffffffffffffffe }),
				       64),
			 INT128_C(-2)));
	assert(int128_eq(wrapping_shr(((int128){ .low = UINT64_MAX,
						 .high = 0xbfffffffffffffff }),
				       126),
			 INT128_C(-2)));

	// Negation Sanity Check
	assert(int128_eq(wrapping_neg(INT128_MAX),
			 wrapping_add(INT128_MIN, INT128_C(1))));

	// Addition Sanity Check
	a = INT128_C(UINT64_MAX);
	b = wrapping_add(a, INT128_C(1));
	assert(int128_eq(b, (int128){ .low = 0, .high = 1 }));

	// Does addition wrap on overflow?
	b = wrapping_add(INT128_MAX, INT128_C(1));
	assert(int128_less(b, INT128_MAX));
	assert(int128_eq(b, INT128_MIN));

	// Subtraction Sanity Check
	b = wrapping_sub(((int128){ .low = 0, .high = 1 }), INT128_C(1));
	assert(int128_eq(b, INT128_C(UINT64_MAX)));

	// Does subtraction wrap on underflow?
	b = wrapping_sub(INT128_MIN, INT128_C(1));
	assert(int128_eq(b, INT128_MAX));

	// Multiplication Sanity Check
	a = (int128){ .low = 0, .high = 100000000 };
	b = wrapping_mul(a, INT128_C(2));
	assert(int128_eq(b, (int128){ .low = 0, .high = 200000000 }));

	b = wrapping_mul(INT128_MAX, INT128_C(0));
	assert(int128_eq(b, INT128_C(0)));

	// Does multiplication wrap on overflow?
	b = wrapping_mul(INT128_MAX, INT128_C(-10));
	assert(int128_eq(b, INT128_C(10)));

	b = wrapping_mul(wrapping_neg(INT128_MAX), INT128_C(-10));
	assert(int128_eq(b, INT128_C(-10)));

	// Make sure multiplying by -1 is equal to wrapping_neg.
	assert(int128_eq(wrapping_neg(INT128_MAX),
			 wrapping_mul(INT128_MAX, INT128_C(-1))));
	assert(int128_eq(wrapping_neg(INT128_MIN),
			 wrapping_mul(INT128_MIN, INT128_C(-1))));
	assert(int128_eq(wrapping_neg(INT128_MIN),
			 wrapping_mul(INT128_C(-1), INT128_MIN)));

	// Division Sanity Check
	a = (int128){ .low = 0, .high = 200000000 };
	b = wrapping_div(a, INT128_C(2));
	assert(int128_eq(b, (int128){ .low = 0, .high = 100000000 }));

	assert(int128_eq(wrapping_div(INT128_MIN, INT128_C(-123456789)),
			 INT128_C(0)));

	assert(int128_eq(wrapping_div(INT128_MIN, INT128_MIN), INT128_C(1)));

	a = INT128_C(1);
	b = INT128_C(-1);
	assert(int128_eq(wrapping_div(b, b), a));
	assert(int128_eq(wrapping_div(b, a), b));
	assert(int128_eq(wrapping_div(a, b), b));
	assert(int128_eq(wrapping_div(a, a), a));

	assert(int128_eq(wrapping_div(INT128_C(1), INT128_C(2)), INT128_C(0)));

	// Remainder Sanity Check
	a = wrapping_div(wrapping_sub(INT128_MAX, INT128_C(1)), INT128_C(2));
	b = INT128_C(1234567890);
	assert(int128_eq(wrapping_rem(a, b), INT128_C(1122498603)));

	a = wrapping_div(wrapping_sub(INT128_MAX, INT128_C(1)), INT128_C(2));
	b = INT128_C(-1234567890);

	assert(int128_eq(wrapping_rem(a, b), INT128_C(1122498603)));
	a = wrapping_neg(a);
	assert(int128_eq(wrapping_rem(a, b), INT128_C(-1122498603)));

	assert(int128_eq(wrapping_rem(INT128_C(-1), INT128_MIN), INT128_C(-1)));
	assert(int128_eq(wrapping_rem(INT128_MIN, INT128_MIN), INT128_C(0)));
	assert(int128_eq(wrapping_rem(INT128_C(1), INT128_C(1)), INT128_C(0)));
	assert(int128_eq(wrapping_rem(INT128_C(1), INT128_C(2)), INT128_C(1)));
}

void uint128_tests()
{
	// Multiplication Sanity Check
	uint128 a = (uint128){ .low = 0, .high = 100000000 };
	uint128 b = wrapping_mul(a, UINT128_C(2));
	assert(uint128_eq(b, (uint128){ .low = 0, .high = 200000000 }));

	// Multiplication wraparound check
	a = UINT128_C(1);
	for (int i = 0; i < 38; i += 1) {
		uint128 tmp = wrapping_mul(a, UINT128_C(10));
		assert(uint128_greater(tmp, a));
		a = tmp;
	}

	b = wrapping_mul(a, UINT128_C(10));

	uint128 correct = (uint128){
		.low = 0x5f65568000000000,
		.high = 0xf050fe938943acc4,
	};

	assert(uint128_eq(b, correct));

	// Right Shift Sanity Check
	a = (uint128){ .low = 0, .high = (uint64_t)1 << 63 };
	assert(uint128_eq(wrapping_shr(a, 1),
			  (uint128){ .low = 0, .high = (uint64_t)1 << 62 }));

	assert(uint128_less(UINT128_C(2), UINT128_C(5)));
}

void regression_tests()
{
	printf("Running regression and literal tests...\n");

	// 1. Comparison regression test (int128_less bug with large low word)
	int128 comp_a = INT128_C(5);
	int128 comp_b = { .low = 0xffffffffffffffffULL, .high = 0 };
	assert(int128_less(comp_a, comp_b) == true);
	assert(int128_greater(comp_b, comp_a) == true);

	// 2. Shift right regression test (0 shifted right arithmetically)
	int128 shift_zero = INT128_C(0);
	int128 shift_res = wrapping_shr(shift_zero, 1);
	assert(shift_res.low == 0 && shift_res.high == 0);

	// 3. Division infinite loop regression test
	uint128 div_max = UINT128_MAX;
	uint128 div_one = UINT128_C(1);
	uint128 div_res = wrapping_div(div_max, div_one);
	assert(uint128_eq(div_res, UINT128_MAX));

	// 4. Literal support in wrapping macros
	uint128 u_val = UINT128_C(10);
	uint128 u_res = wrapping_add(u_val, 5); // int literal
	assert(u_res.low == 15 && u_res.high == 0);

	u_res = wrapping_sub(u_val, 20LL); // long long literal
	assert(u_res.low == (uint64_t)-10 && u_res.high == UINT64_MAX);

	int128 s_val = INT128_C(-5);
	int128 s_res = wrapping_add(s_val, 10);
	assert(s_res.low == 5 && s_res.high == 0);

	s_res = wrapping_add(s_val, -10);
	assert(s_res.low == (uint64_t)-15 && s_res.high == UINT64_MAX);

	printf("All regression and literal tests passed!\n");
}

int main()
{
	uint128_tests();
	int128_tests();
	regression_tests();
	printf("All tests completed successfully!\n");
	return 0;
}
