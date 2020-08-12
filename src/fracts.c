#include "glbl.h"
#include "fracts.h"

/*
	fract-fract operations
*/

//add term fract to output fract
void fract_add_fract(fract *output_fract, fract term_fract) {
	output_fract->num = output_fract->num * term_fract.den +
		output_fract->den * term_fract.num;
	output_fract->den = output_fract->den * term_fract.den;
};

//subtract term fract from output fract
void fract_sub_fract(fract *output_fract, fract term_fract) {
	output_fract->num = output_fract->num * term_fract.den -
		output_fract->den * term_fract.num;
	output_fract->den = output_fract->den * term_fract.den;
};

//multiply output fract by term fract
void fract_mul_fract(fract *output_fract, fract term_fract) {
	output_fract->num = output_fract->num * term_fract.num;
	output_fract->den = output_fract->den * term_fract.den;
};

//divide output fract by term fract
void fract_div_fract(fract *output_fract, fract term_fract) {
	output_fract->num = output_fract->num * term_fract.den;
	output_fract->den = output_fract->den * term_fract.num;
};

/*
	fract unary operations
*/

//set output fract to its reciprocal
void fract_reciprocate(fract *output_fract) {
	int temp = output_fract->num;
	output_fract->num = output_fract->den;
	output_fract->den = temp;
};

//simplify a fract, if possible
void fract_simplify(fract *output_fract) {
	int fract_gcd = gcd(output_fract->num, output_fract->den);
	if fract_gcd != 1 {
		output_fract->num = output_fract->num / fract_gcd;
		output_fract->den = output_fract->den / fract_gcd;
	}
};

//evaluate input fract to a double
double fract_to_double(fract input_fract) {
	double output_double;
	output_double = (double) (input_fract.num) / (double) (input_fract.den);
	return output_double;
};

/*
	fract-int operations
*/

//add term int to output fract
void fract_add_int(fract *output_fract, int term_int) {
	output_fract->num = output_fract->num + output_fract->den * term_int;
};

//subtract term int from output fract
void fract_sub_int(fract *output_fract, int term_int) {
	output_fract->num = output_fract->num - output_fract->den * term_int;
};

//multiply output fract by term int
void fract_mul_int(fract *output_fract, int term_int) {
	output_fract->num = output_fract->num * term_int;
};

//divide output fract by term int
void fract_div_int(fract *output_fract, int term_int) {
	output_fract->den = output_fract->den * term_int;
};

/*
	misc
*/

//greatest common denominator finding function
//stolen from:
//lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/
int gcd(int inta, int intb) {
    int shift;
    if (inta == 0)
		return intb;
    if (intb == 0)
		return inta;
    shift = __builtin_ctz(inta | intb);
    inta >>= __builtin_ctz(inta);
    do {
        intb >>= __builtin_ctz(intb);
        if (intb > intb) {
            int temp = intb;
            intb = inta;
            inta = temp;
        }  
        intb = intb - inta;
    } while (intb != 0);
    return inta << shift;
}
