/****************************************
 Advanced Digital System Design
 Fixed_point SVM
 Authors: He Li & Ahmid Tahmid
 ****************************************/

#include <stdio.h>

#include "fp_hyp_cordic.h"
#include "fp_tanh.h"
//#include "fp_tanh_round.h"
#include "Fixed_point.h"

void Fixed_point(data_t x[N],            // Input of the classifier
		data_t *output              // Output of the classifier
		) {

#pragma HLS ARRAY_PARTITION variable=x complete dim=1


#pragma HLS INTERFACE s_axilite port=output
#pragma HLS INTERFACE s_axilite port=x
#pragma HLS INTERFACE s_axilite port=return



	// Accumulator for the result of the sum on the support vectors
	float acc_sv = 0;
	// Accumulator for the dot product and the parameter c
	int acc_dp = 0;
	// Result of the classification
	data_t result;
	// Register for the output of the fp_tanh function
	data_t reg_tanh;

	// Sum on the support vectors
	int index_sv;
	svm_loop: for (index_sv = 0; index_sv < NSV; index_sv++) {
		acc_dp = 0;
		int index_dp;
		dot_product_loop: for (index_dp = 0; index_dp < N; index_dp++) {
			acc_dp += (sv_x[index_sv][index_dp] * x[index_dp]) >> 12;
		}
		acc_dp = acc_dp << 1; // Multiplication by 2
		fp_tanh(acc_dp, &reg_tanh);
		float a = 4096;
		acc_sv += (sv_alpha[index_sv] * ((float)reg_tanh) / a);
	}

	// Result computation from the sign of the sum
	acc_sv = acc_sv + b;
	printf("acc_svm + bias: %f\n", acc_sv);
	result = ((acc_sv) >= 0) ? 0 : 1;

	// Output assignment
	*output = result;
}
