#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "state.h"
#include "svm.h"

using std::vector;
using std::cout;
using std::endl;

extern struct svm_problem 	prob;
extern struct svm_parameter param;
extern GKernel aks_kernel_obj;
extern int size;

State::State(){

	scoreIsCached = false;
	fitnessScore = -1.0;
}

State::State(GKernel k){

	scoreIsCached = false;
	fitnessScore = -1.0;
	kernel = k;
}

void State::print(){

	for(int i=0; i < kernel.alphas.size(); i++)
		cout << kernel.alphas[i] << " ";

	cout << endl;
}

double State::fitness(){

	if(scoreIsCached)
		return fitnessScore;

	aks_kernel_obj = kernel;
	model = svm_train(&prob, &param);

	/* Score this state! */

	double score = 0.0;

	int counter = 0;
    int correct = 0;

    svm_node** vitr = prob.x;
    double* yitr = prob.y;
    svm_node* px;

    while(counter < size){

        px = vitr[counter];

        double d = svm_predict(model, px);

        if(((int)yitr[counter]) == ((int)d))
            correct++;
        //if(((int)yitr[counter]) == 1 && ((int)d) == 1)
        //    correct++;
        //else if(((int)yitr[counter]) == 2 && ((int)d) == 2)
        //    correct++;

        counter++;
    }

	score = (1.0 * correct)/(1.0 * counter);
	svm_free_and_destroy_model(&model);

	/* Cache for when we get called again */

	scoreIsCached = true;
	fitnessScore = score;

	return score;
}

void State::mutate(){

	/*
	 * Slightly change the state with a random tweak.
	 * This will get called very infrequently
	 */

	cout << "MUTATION" << endl;

	int affectedIndex = rand() % kernel.alphas.size();
	double oldVal = kernel.alphas[affectedIndex];

	if(CLOSE_TO(oldVal,0.0)){
		kernel.alphas[affectedIndex] = rand() * 1.0;
	}
	else{

		int i;
		double sign = (rand() % 2 == 1) ? 1.0 : -1.0;
		//double sign = 1.0;

		switch(rand() % 6){
		case 0:
			kernel.alphas[affectedIndex] = sign * pow(oldVal,2.0);
			break;
		case 1:
			kernel.alphas[affectedIndex] = sign * pow(oldVal,0.5);
			break;
		case 2:
			kernel.alphas[affectedIndex] = 0.0;
			break;
		case 3:
			kernel.alphas[affectedIndex] = rand() * sign;
			break;
		case 4:
			for(i=0;i<kernel.alphas.size();i++)
				kernel.alphas[i] *= 100.0;
			break;
		case 5:
			for(i=0;i<kernel.alphas.size();i++)
				kernel.alphas[i] /= 100.0;
			break;
		}
	}
}

State::~State(){

}
