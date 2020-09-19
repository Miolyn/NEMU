#include "FLOAT.h"
// #include "lib-common/FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {

	return (FLOAT)((ll)a * (ll)b >> 16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */
	int sa = sign_bit(a);
	int sb = sign_bit(b);
	int s = int_no_sign(sa) * int_no_sign(sb);
	int noSa = a * int_no_sign(sa);
	int noSb = b * int_no_sign(sb);
	int res = noSa / noSb;
	int mod = noSa % noSb;
	int i;
	for(i = 0; i < 16; i++){
		res <<= 1;
		mod <<= 1;
		if(mod >= noSb){
			mod -= noSb;
			res |= 1;
		}
	}
	return res * s;
}

FLOAT fF2F(float a) {
	int b = *(int *)&a;
	int sign = b >> 31;
	int exp = (b >> 23) & 0xff;
	FLOAT k = b & 0x7fffff;
	if (exp != 0) k += 1 << 23;
	exp -= 150;
	// exp = exp - 150
	// k >>= -16 + 150 -exp = 134 - exp = 127 + 7 - exp
	if (exp < -16) k >>= -16 - exp;
	// exp - 150 + 16 = exp - 134 = (exp - 127) - 7
	if (exp > -16) k <<= exp + 16;
	return sign == 0 ? k : -k;
}
FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */
	int t = *(int*)&a;
	int s = sign_bit(t);
	t = (t << 1) >> 1;
	int Ex = (t >> 23) & 0xff;
	FLOAT res = t & 0x7ffff;
	int e = Ex - 0x7f;
	if(!Ex){
		if(!res) return 0;
		else e = 1 - Ex;
	} else if(!(Ex ^ 0xff)){
		return (-1) ^ ((!s) << 31);
	}else res |= (1 << 23);
	// now point is at l:23
	// (s)(31) (30)--(23).(22)--(16).(15)...(0)
	// if(e > 7){
	// 	res <<= e - 7;
	// } else{
	// 	e *= -1;
	// 	res >>= e - 7;
	// }
	if(e > 7){
		res <<= e - 7;
	} else{
		if(e > 0) res >>= 7 - e;
		else res >>= -7 - e;
	}
	// res >>= 7;

	return (res * int_sign(s));
}


FLOAT Fabs(FLOAT a) {
	// nemu_assert(0);
	return a * int_no_sign(sign_bit(a));
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

