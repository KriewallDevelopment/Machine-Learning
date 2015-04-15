#ifndef KERNEL_H
#define KERNEL_H

#include "svm.h"
#include <cmath>
#include <vector>

extern const int NUM_KERNELS;
extern double* dots;

inline bool CLOSE_TO(double one, double two){
	return fabs(one - two) < 0.00001;
}

class GKernel {

public:

	GKernel& operator = (const GKernel &k){

		this->alphas = k.alphas;

		return *this;
	}

	bool operator == (const GKernel &k){

		bool equal = true;

		for(int i=0; i < 4; i++){
			if(!CLOSE_TO(alphas[i],k.alphas[i])){
				equal = false;
				break;
			}
		}

		return equal;
	}

	double eval(const svm_node*, const svm_node*);
	std::vector<double> alphas;
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
	return exp(-0.5 * (dots[px->index] + dots[py->index] - 2*dot(px,py)));
}

inline double poly(const svm_node* px, const svm_node* py){
    return pow(0.5 * dot(px,py) + 1.0, 2.0);
}

inline double sigmoid(const svm_node* px, const svm_node* py){
	return tanh(0.5 * dot(px,py));
}

inline double wave(const svm_node* px, const svm_node* py){

	double theta = dot(px,py) / (sqrt(dot(px,px)) * sqrt(dot(py,py)));
	double sum = 0.0;

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

	return (theta / sum) * sin( sum / theta );
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

	return -1.0 * log(pow(sum, exponent) + 1.0);
}

#endif
