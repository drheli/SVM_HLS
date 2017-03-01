/*******************************************************************************
 Advanced Digital System Design
 Tanh with round convergene
 Authors: He Li & Ahmid Tahmid
 *******************************************************************************/

#include <stdio.h>

#include "fp_hyp_cordic.h"
#include "fp_tanh.h"

void fp_tanh(data_t input, data_t *output) {
	// Flag to indicate that the result is negative
	char is_neg;
	// Trigonometric index to extend the range
	int index_trigo;
	// Input angle
	data_t beta = input;
	// Output of the hyperbolic CORDIC block
	data_t outputcosh, outputsinh;
	// Result to be assigned to the output
	data_t result, result_num, result_den;

	// Central symmetry of the tanh function
	if (beta < 0) {
		is_neg = 1;
		beta = -beta;
	} else {
		is_neg = 0;
	}

	if(beta >= 24576) {
		result = 4096;
	}
	else {
		while beta > 4096*1.13 {
			num = num + 1;
			beta = beta - 4096*log(2);
		}
	}
	
	// Call to the hyperbolic CORDIC block
		fp_hyp_cordic(beta, &outputcosh, &outputsinh);
	// Trigonometric rules application
	if (num) {
		result_num = 0.5*(outputsinh + outputcosh)*2^(num) +0.5*(outputsinh - outputcosh)*2^(-num) ;
		result_den = (0.5*(outputsinh + outputcosh)*2^(num) +0.5*(-outputsinh + outputcosh)*2^(-num)) >>12 ;
	}
	else { 
		result_num = outputcosh;
		result_den = outputsinh;
	}
	
		result = result_num / result_den;
		
		
	// Central symmetry correction
	result = is_neg ? -result : result;

	// Output assignment
	*output = result;
}

	// Mapping of the range to the operational range of the hyperbolic CORDIC block
/* 	if (beta >= 24576) {
		result = 4096; // Saturation effect
	} else {
		if (beta >= 20480) {
			index_trigo = 5;
		} else if (beta >= 16384) {
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
		result_num = (sinh[index_trigo] * outputcosh
				+ cosh[index_trigo] * outputsinh);
		result_den = (cosh[index_trigo] * outputcosh
				+ sinh[index_trigo] * outputsinh) >> 12;
		result = result_num / result_den;
	} */
