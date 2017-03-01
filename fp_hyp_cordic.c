#include "fp_type.h"
#include "fp_hyp_cordic.h"

#define nb_iter 12
#define K  0x1351        // 1.2075 initial value in fixed-point 3.12 (16 bits)
//#define K  0x1348        // 1.2073 initial value in fixed-point 3.12 (16 bits)

void fp_hyp_cordic(data_t input, data_t *outputx, data_t *outputy) {
	// Token for the repetition of the 4th iteration
	char token4 = 0;

	// Approximation of cosh(beta) and sinh(beta)
	data_t x = K;
	data_t y = 0;
	data_t x_new;
	data_t beta = input;
	// Stored values
	const data_t atanh[12] = { 0x08C9, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064,
			0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001 };

	// CORDIC Iteration
	int i,j;
	fp_hyp_cordic: for (i = 1; i <= nb_iter; i++) {
		// force the 3i+1 th iteration to be repeated
		if (((i%3)==1) &&( i!=1 )){
		//if (i == 4 && token4 == 0)
			for (j=1;j<=2;j++){
				// beta<0 anti-clockwise rotation
				if (beta < 0) {
					x_new = x - (y >> i);
					y -= x >> i;
					beta += atanh[i - 1];
				}
				// beta>0 clockwise rotation
				else {
					x_new = x + (y >> i);
					y += (x >> i);
					beta -= atanh[i - 1];
				}
				x = x_new;
			}
		}
		else {
			if (beta < 0) {
				x_new = x - (y >> i);
				y -= x >> i;
				beta += atanh[i - 1];
			}
			// beta>0 clockwise rotation
			else {
				x_new = x + (y >> i);
				y += (x >> i);
				beta -= atanh[i - 1];
			}
			x = x_new;
			}
		}

	// Assignments of the outputs
	*outputx = x;
	*outputy = y;
}
