#ifndef STATE_H
#define STATE_H

#include "svm.h"

typedef double (*KERNEL_FUNCTION)(const svm_node*, const svm_node*);


class State {

public:

	State();
	State(KERNEL_FUNCTION f);
	~State();

	State(const State &s){

		// TODO

		this->kernel = s.kernel;
		this->fitnessScore = s.fitnessScore;
		this->scoreIsCached = s.scoreIsCached;
    }

    bool operator == (const State &s) {

        // TODO

		return this->kernel == s.kernel;
    }

	State& operator = (const State &s){
		this->kernel = s.kernel;
		this->fitnessScore = s.fitnessScore;
		this->scoreIsCached = s.scoreIsCached;
		return *this;
	}

	KERNEL_FUNCTION getKernel(){
		return this->kernel;
	}

	/* Could also return double here */

	double fitness();
	
	/* Should be called a small percentage of the time */

	void mutate();

private:

	bool scoreIsCached;
	double fitnessScore;

	/* DEFINE ACTUAL STATE VARIABLES HERE */

	KERNEL_FUNCTION kernel;
	struct svm_model* model;

};
#endif
