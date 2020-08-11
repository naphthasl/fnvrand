typedef struct fract {
	int num;
	int den;
} fract;

//ADD TERM FRACT TO OUTPUT FRACT
void fract_add_fract(fract *output_fract, fract term_fract) {
	*output_fract.num = (*output_fract.num) * term_fract.den +
		(*output_fract.den) * term_fract.num;
	*output_fract.den = (*output_fract.den) * term_fract.den;
};

//SUBTRACT TERM FRACT FROM OUTPUT FRACT
void fract_sub_fract(fract *output_fract, fract term_fract) {
	*output_fract.num = (*output_fract.num) * term_fract.den -
		(*output_fract.den) * term_fract.num;
	*output_fract.den = (*output_fract.den) * term_fract.den;
};

//MULTIPLY OUTPUT FRACT BY TERM FRACT
void fract_mul_fract(fract *output_fract, fract term_fract) {
	*output_fract.num = (*output_fract.num) * term_fract.num;
	*output_fract.den = (*output_fract.den) * term_fract.den;
};

//DIVIDE OUTPUT FRACT BY TERM FRACT
void fract_div_fract(fract *output_fract, fract term_fract) {
	*output_fract.num = (*output_fract.num) * term_fract.den;
	*output_fract.den = (*output_fract.den) * term_fract.num;
};

//https://www.youtube.com/watch?v=9pRcfAeSurg
void fract_reciprocate(fract *output_fract) {
	int temp = *output_fract.num;
	*output_fract.num = *output_fract.den;
	*output_fract.den = temp;
};

//ADD TERM INTEGER TO OUTPUT FRACT
void fract_add_int(fract *output_fract, int term_int) {
	*output_fract.num = (*output_fract.num) + (*output_fract.den) * term_int;
};

//SUBTRACT TERM INTEGER FROM OUTPUT FRACT
void fract_sub_int(fract *output_fract, int term_int) {
	*output_fract.num = (*output_fract.num) - (*output_fract.den) * term_int;
};

//MULTIPLY OUTPUT FRACT BY TERM INT
void fract_mul_int(fract *output_fract, int term_int) {
	*output_fract.num = (*output_fract.num) * term_int;
};

//DIVIDE OUTPUT FRACT BY TERM INT
void fract_div_int(fract *output_fract, int term_int) {
	*output_fract.den = (*output_fract.den) * term_int;
};
