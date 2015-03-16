#ifndef STATE_H
#define STATE_H

#include "svm.h"
#include "kernel.h"

typedef double (*KERNEL_FUNCTION)(const svm_node*, const svm_node*);

class State {

public:

	State();
	State(GKernel);
	~State();

	State(const State &s){

		this->kernel = s.kernel;
		this->fitnessScore = s.fitnessScore;
		this->scoreIsCached = s.scoreIsCached;
    }

	friend bool operator < (const State& one, const State& two){
		return one.fitnessScore > two.fitnessScore;
	}

    bool operator == (const State &s) {

		return this->kernel == s.kernel;
    }

	State& operator = (const State &s){
		this->kernel = s.kernel;
		this->fitnessScore = s.fitnessScore;
		this->scoreIsCached = s.scoreIsCached;
		return *this;
	}

	GKernel getKernel(){
		return this->kernel;
	}

	void setKernelAlpha(int idx, double val){
		kernel.alphas[idx] = val;
	}

	void print();

	/* Could also return double here */

	double fitness();
	double constFitness() const;
	
	/* Should be called a small percentage of the time */

	void mutate();

private:

	bool scoreIsCached;
	double fitnessScore;

	/* DEFINE ACTUAL STATE VARIABLES HERE */

	GKernel kernel;
	struct svm_model* model;

};

#endif
