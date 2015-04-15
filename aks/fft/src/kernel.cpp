#include "kernel.h"
#include "svm.h"
#include <stdio.h>
#include <stdlib.h>
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

			double dp = (dot(pi,pi) + dot(pj,pj)) - (2 * dot(pi,pj));

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

	SAMPLE_DELTA = fabs(SAMPLE_UB - SAMPLE_LB) / ((1.0 * SAMPLE_N) - 1.0);

	printf("lb: %f ub: %f\n",SAMPLE_LB,SAMPLE_UB);
	printf("delta: %f\n",SAMPLE_DELTA);
}


double GKernel::eval(const svm_node* px, const svm_node* py){

	double x = (dot(px,px) + dot(py,py)) - (2 * dot(px,py));
	int idx = ((int)floor(((x - SAMPLE_LB) / SAMPLE_DELTA)));

	/* Are we right on the first x value? */

	if(CLOSE_TO(x,SAMPLE_LB + (idx * SAMPLE_DELTA)))
		return freq[idx][0];

	if(idx + 1 == SAMPLE_N){
		fprintf(stderr, "INDEX OUT OF BOUNDS\n");
		exit(EXIT_FAILURE);		
	}


	double off1 = (1.0 * idx) * SAMPLE_DELTA;
	double off2 = (1.0 * (idx + 1)) * SAMPLE_DELTA;

	/* Are we right on the second x value? */

	if(CLOSE_TO(off2 + SAMPLE_LB, x))
		return freq[idx + 1][0];

	/* Draw a straight line between y-vals and project */

	double rise = freq[idx+1][0] - freq[idx][0];
	double perc = (x - off1) / SAMPLE_DELTA;

	return freq[idx][0] + (rise * perc);
}
