/************************************************************************************************
 Advanced Digital System Design
 Testbench for SVM accelerator
 Authors: He Li & Ahmid Tahmid
 Specification: to test the function in HLS, we first use small test data, NSVM = 2,
				if the function is correct, then we can compute NSVM = 2000 data on FPGA
 ************************************************************************************************/

#include <stdio.h>
#include <math.h>
#include "fp_type.h"
#include "fp_hyp_cordic.h"
#include "fp_tanh.h"
#include "Fixed_point.h"

#include "testData.h"
#include "result.h"
#include "gold.h"

#define NSVM 2 //2000 // Number of tests for the SVM block
int main() {
	// Tedt, SVM IP and SVM Golden Files
	FILE *test_tanh, *test_svm, *gold_svm;
	// Variables to test CORDIC block
	data_t outputx, outputy;
	// Variables to test of the tanh block
	data_t output;
	// Variables to test of the svm
	data_t x[N];

	// Accumulator for the result of the sum on the support vectors
	float acc_sv;
	// Accumulator for the dot product and the parameter c
	float acc_dp;
	// Register for the coefficient of each support vector
	data_t reg_coeff;

	/*
	 * Opening files for the TB
	 */
	test_tanh = fopen("test_tanh_fp.dat", "w");
	test_svm = fopen("test_svm_fp.dat", "w");
	gold_svm = fopen("gold_svm_fp.dat", "w");

	/*
	 * Test of the svm block
	 */
	int index_test;
	for (index_test = 0; index_test < NSVM; index_test++) {
		fprintf(test_svm, " SVM Test %d\n", index_test);
		fprintf(gold_svm, " SVM Test %d\n", index_test);

		// Assign value for the input data
		int j;
		for (j = 0; j < N; j++) {
			x[j] = testData[index_test][j];
		}

		// Compute the output value with the SVM IP
		Fixed_point(x, &output);
		fprintf(test_svm, " Computed Result { %d }\n\n", output);

		// Print the gold output value
		output = gold[index_test];
		fprintf(gold_svm, " Computed Result { %d }\n\n", output);
	}

	fclose(test_tanh);
	fclose(test_svm);
	fclose(gold_svm);

	if (system("diff -w test_svm_fp.dat gold_svm_fp.dat")) {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
		return 1;
	} else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	}

	/*
		// Test of the hyperbolic CORDIC block

		fprintf(test_tanh, " Hyperbolic CORDIC Test\n");
		fp_hyp_cordic(+2048, &outputx, &outputy);
		fprintf(test_tanh, " Input {+0.5} Signed Result { %d, %d }\n", outputx, outputy);
		fp_hyp_cordic(-2048, &outputx, &outputy);
		fprintf(test_tanh, " Input {-0.5} Signed Result { %d, %d }\n", outputx, outputy);
		fp_hyp_cordic(+4096, &outputx, &outputy);
		fprintf(test_tanh, " Input {+1.0} Signed Result { %d, %d }\n", outputx, outputy);
		fp_hyp_cordic(-4096, &outputx, &outputy);
		fprintf(test_tanh, " Input {-1.0} Signed Result { %d, %d }\n", outputx, outputy);
		fp_hyp_cordic(+4096 + 2048, &outputx, &outputy);
		fprintf(test_tanh, " Input {+1.5} Signed Result { %d, %d }\n", outputx, outputy);
		fp_hyp_cordic(-4096 - 2048, &outputx, &outputy);
		fprintf(test_tanh, " Input {-1.5} Signed Result { %d, %d }\n", outputx, outputy);
		fprintf(test_tanh, "\n");




		fprintf(test_tanh, " Hyperbolic Tangent Test\n");
		fp_tanh(+2048, &output);
		fprintf(test_tanh, " Input {+0.5} Signed Result { %d }\n", output);
		fp_tanh(4096, &output);
		fprintf(test_tanh, " Input {+1.0} Signed Result { %d }\n", output);
		fp_tanh(4096 + 2048, &output);
		fprintf(test_tanh, " Input {+1.5} Signed Result { %d }\n", output);
		fp_tanh(-4096 - 2048, &output);
		fprintf(test_tanh, " Input {-1.5} Signed Result { %d }\n", output);
		fp_tanh(32767, &output);
		fprintf(test_tanh, " Input {SATURATION} Signed Result { %d }\n", output);
		fp_tanh(10840, &output);
		fprintf(test_tanh, " Input {atanh(0.99)} Signed Result { %d }\n\n", output);


		//Test of the tanh block
		int beta;
		for (beta = 0; beta <= 32767; beta++) {
			fp_tanh(beta, &output);
			fprintf(test_tanh, " Fixed_point Input { %d } Signed Result { %d }\n", beta, output);
		}
	*/

}
