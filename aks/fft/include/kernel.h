#ifndef KERNEL_H
#define KERNEL_H

#include "svm.h"
#include <cmath>
#include <vector>
#include <fftw3.h>

inline bool CLOSE_TO(double one, double two){
	return fabs(one - two) < 0.00001;
}


extern double 		SAMPLE_DELTA;
extern double		SAMPLE_LB;
extern double		SAMPLE_UB;
static const int	SAMPLE_N = 100;

extern void KERNEL_INIT(struct svm_problem);


class GKernel {

public:

	GKernel& operator = (const GKernel &k){

		for(int i=0; i<SAMPLE_N; i++){
			freq[i][0] = k.freq[i][0];
			freq[i][1] = k.freq[i][1];
		}

		return *this;
	}

	bool operator == (const GKernel &k){

		bool equal = true;

		for(int i=0; i < 4; i++){
			if(!CLOSE_TO(freq[i][0],k.freq[i][0])){
				equal = false;
				break;
			}
		}

		return equal;
	}

	void setArray(fftw_complex* arr){

		for(int i=0; i<SAMPLE_N; i++){
			freq[i][0] = arr[i][0];
			freq[i][1] = arr[i][1];
		}
	}

	double eval(const svm_node*, const svm_node*);

	fftw_complex freq[SAMPLE_N];
};

inline double dot(const svm_node *px, const svm_node *py){

    double sum = 0;
    while(px->index != -1 && py->index != -1){
        if(px->index == py->index){
            sum += px->value * py->value;
            ++px;
            ++py;
        }
        else {
            if(px->index > py->index)
                ++py;
            else
                ++px;
        }
    }
    return sum;
}

inline double linear(const svm_node* px, const svm_node* py){
	return dot(px,py);
}

inline double rbf(const svm_node* px, const svm_node* py){
	return exp(-0.5 * (dot(px,px)+dot(py,py) - 2*dot(px,py)));
}

inline double poly(const svm_node* px, const svm_node* py){
    return pow(0.5 * dot(px,py) + 1.0, 2.0);
}

inline double log(const svm_node* px, const svm_node* py){

	double sum = 0.0;
	double exponent = 3;

	while(px->index != -1 && py->index != -1){
	    if(px->index == py->index){
	    	double diff = px->value - py->value;
	        sum += diff * diff;
	        ++px;
	        ++py;
	    }
	    else {
	        if(px->index > py->index)
	            ++py;
	        else
	            ++px;
	    }
	}

	return -1.0 * log(pow(sum, 0.5*exponent));
}

#endif
