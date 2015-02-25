#include "aks.h"
#include "svm.h"


static double dotProduct(const svm_node* v1, const svm_node* v2){

	double sum = 0;

    while(px->index != -1 && py->index != -1)
    {

        if(px->index == py->index)
        {
            sum += px->value * py->value;
            ++px;
            ++py;
        }
        else
        {
            if(px->index > py->index)
                ++py;
            else
                ++px;
        }
    }

    return sum;
}

double aks_kernel(const svm_node* v1, const svm_node* v2){

	return dotProduct(v1,v2);
}
