/****************************************
 Advanced Digital System Design
 Tanh with separate partition to converge
 Authors: He Li & Ahmid Tahmid
 ****************************************/

#include <stdio.h>

#include "fp_hyp_cordic.h"
#include "fp_tanh.h"

void fp_tanh(data_t input, data_t *output) {
	// the result is positive or negative
	char is_neg;
	// Trigonometric index to show the range
	int index_trigo;
	// Input angle
	data_t beta = input;
	// Output of the hyperbolic CORDIC block
	data_t outputcosh, outputsinh;
	// Stored cosh value
	const data_t cosh[5] = { 0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE};
	//const data_t cosh[6] = { 0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE, 0x4A35B };
	// Stored sinh value
	const data_t sinh[5] = { 0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3};
	//const data_t sinh[6] = { 0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3, 0x4A340 };
	// Result of tanh, sinh and cosh
	data_t result, result_cosh, result_sinh;

	// Central symmetry of the tanh function
	if (beta < 0) {
		is_neg = 1;
		beta = -beta;
	} else {
		is_neg = 0;
	}

	// Implement approximate range of the hyperbolic CORDIC block
	if (beta >= 20480) {
		result = 4096; // Saturation effect
	} else {
		if (beta >= 16384) {
			index_trigo = 4;
		} else if (beta >= 12288) {
			index_trigo = 3;
		} else if (beta >= 8192) {
			index_trigo = 2;
		} else if (beta >= 4096) {
			index_trigo = 1;
		} else {
			index_trigo = 0;
		}
		beta = beta - index_trigo * 4096;

		// Call to the hyperbolic CORDIC block
		fp_hyp_cordic(beta, &outputcosh, &outputsinh);

		// Trigonometric rules application
		result_cosh = (sinh[index_trigo] * outputcosh
				+ cosh[index_trigo] * outputsinh);
		result_sinh = (cosh[index_trigo] * outputcosh
				+ sinh[index_trigo] * outputsinh) >> 12;
		result = result_cosh / result_sinh;
	}
	// Central symmetry correction
	result = is_neg ? -result : result;

	// Output assignment
	*output = result;
}
