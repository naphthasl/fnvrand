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
