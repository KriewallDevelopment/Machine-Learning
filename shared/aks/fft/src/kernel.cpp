#include "kernel.h"
#include "stdio.h"
#include "svm.h"
#include <limits.h>
#include <float.h>
#include <math.h>

double 	SAMPLE_DELTA 	= 0.0;
double	SAMPLE_LB 		= DBL_MAX;
double	SAMPLE_UB 		= -DBL_MAX;


extern int size;


void KERNEL_INIT(struct svm_problem prob){

	int ci = 0;
	int cj = 0;

	svm_node** vitr = prob.x;
	svm_node* pi;
	svm_node* pj;

	while(ci < size){

		cj = ci + 1;
		pi = vitr[ci];

		while(cj < size){

			pj = vitr[cj];

			double dp = linear(pi,pj);

			if(dp > SAMPLE_UB)
				SAMPLE_UB = dp;
			
			if(dp < SAMPLE_LB)
				SAMPLE_LB = dp;

			cj++;
		}

        ci++;
    }

	SAMPLE_UB = ceil(SAMPLE_UB);
	SAMPLE_LB = floor(SAMPLE_LB);

	SAMPLE_DELTA = fabs(SAMPLE_UB - SAMPLE_LB) / (1.0 * SAMPLE_N);

	printf("lb: %f ub: %f\n",SAMPLE_LB,SAMPLE_UB);
	printf("delta: %f\n",SAMPLE_DELTA);
}


double GKernel::eval(const svm_node* px, const svm_node* py){

	double val = 0.0;
	double x = dot(px,py);

	/* Are we right on the first x value? */

	if(CLOSE_TO(x,SAMPLE_LB))
		return freq[0][0];

	for(int i=0; i < SAMPLE_N - 1; i++){

		double off1 = (1.0 * i) * SAMPLE_DELTA;
		double off2 = (1.0 * (i + 1)) * SAMPLE_DELTA;

		/* Are we right on the second x value? */

		if(CLOSE_TO(off2 + SAMPLE_LB, x)){
			val = freq[i + 1][0];
			break;
		}

		if(x < (off2 + SAMPLE_LB)){

			/* Draw a straight line between y-vals and project */

			double rise = freq[i+1][0] - freq[i][0];
			double perc = (x - off1) / SAMPLE_DELTA;

			val = freq[i][0] + (rise * perc);
			break; 
		}
	}

	return val;
}
