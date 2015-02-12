#ifndef STATE_H
#define STATE_H

#include <vector>

#define EPSILON		(1e-5)


/* REPRESENTATION OF A SINGLE STATE */

class State {

public:

	State();
	~State();

	State(const State &s){

		this->weights = s.weights;
	}

	bool operator == (const State &s) {

		for(unsigned int i=0; i<weights.size(); i++)
			if(weights[i] > s.weights[i] + EPSILON || 
				weights[i] < s.weights[i] - EPSILON)
				return false;
		
		return true;
	}

	friend bool operator < (State &s1, State &s2) {

		return s1.fitness() < s2.fitness();
	}

	int fitness();
	void mutate();

private:

	bool	scoreIsCached;
	int		fitnessScore;

	/* 
	 * Our state will be a vector of weights that can
	 * be applied to the individual factors
	 */

	std::vector<double> 	weights;
};

#endif
