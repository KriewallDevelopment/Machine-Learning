#ifndef KERNEL_H
#define KERNEL_H

#include "svm.h"
#include "state.h"
#include <cmath>

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

inline double rbf(const svm_node* px, const svm_node* py){
	return exp(-0.5 * (dot(px,px)+dot(py,py) - 2*dot(px,py)));
}

inline double poly(const svm_node* px, const svm_node* py){
    return pow(0.5 * dot(px,py) + 1.0, 3.0);
}

#endif
