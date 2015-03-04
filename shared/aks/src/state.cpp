#include <stdio.h>
#include "state.h"
#include "svm.h"

extern struct svm_problem 	prob;
extern struct svm_parameter param;
extern KERNEL_FUNCTION aks_kernel;
extern int size;

State::State(){

	scoreIsCached = false;
	fitnessScore = -1.0;
}

State::State(KERNEL_FUNCTION f){

	scoreIsCached = false;
	fitnessScore = -1.0;
	kernel = f;
}

double State::fitness(){

	if(scoreIsCached)
		return fitnessScore;

	aks_kernel = kernel;
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

        if(((int)yitr[counter]) == 1 && ((int)d) == 1)
            correct++;
        else if(((int)yitr[counter]) == 2 && ((int)d) == 2)
            correct++;

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


}

State::~State(){

}
