#include "kernel.h"
#include "stdio.h"
#include "svm.h"

const int NUM_KERNELS = 6;

extern double* dots;

double GKernel::eval(const svm_node* px, const svm_node* py){

	double val = 0.0;

	if(!CLOSE_TO(alphas[0],0.0))
		val += alphas[0] * linear(px,py);

	if(!CLOSE_TO(alphas[1],0.0))
		val += alphas[1] * poly(px,py);

	if(!CLOSE_TO(alphas[2],0.0))
		val += alphas[2] * rbf(px,py);

	if(!CLOSE_TO(alphas[3],0.0))
		val += alphas[3] * log(px,py);

	if(!CLOSE_TO(alphas[4],0.0))
		val += alphas[4] * sigmoid(px,py);

	if(!CLOSE_TO(alphas[5],0.0))
		val += alphas[5] * wave(px,py);

	return val;
}
