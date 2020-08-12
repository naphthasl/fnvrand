#include "glbl.h"

#ifndef FRACT_HEADER_FILE
#define FRACT_HEADER_FILE
	typedef struct fract {
		int num;
		int den;
	} fract;
	
	//fract-fract operations
	void fract_add_fract(fract *output_fract, fract term_fract);
	void fract_sub_fract(fract *output_fract, fract term_fract);
	void fract_mul_fract(fract *output_fract, fract term_fract);
	void fract_div_fract(fract *output_fract, fract term_fract);
	
	//fract unary operations
	void fract_reciprocate(fract *output_fract);
	void fract_simplify(fract *output_fract);
	double fract_to_double(fract input_fract);
	
	//fract-int operations
	void fract_add_int(fract *output_fract, int term_int);
	void fract_sub_int(fract *output_fract, int term_int);
	void fract_mul_int(fract *output_fract, int term_int);
	void fract_div_int(fract *output_fract, int term_int);
	
	//misc
	int gcd(int inta, int intb);
#endif
