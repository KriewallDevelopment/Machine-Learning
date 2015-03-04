#ifndef GENETIC_H
#define GENETIC_H

#include "state.h"
#include <vector>
#include <ctime>
#include <stdlib.h>

class GeneticSimulator {

public:

	GeneticSimulator(){
		srand(time(0));
	};

	void addToPopulation(State s){
		currentPopulation.push_back(s);
	}

	/* Finds and returns the most fit state across numberOfGenerations */

	State search(int numberOfGenerations);


	/* Define how to combine two states */

	State breed(State,State);


	/* optional */

	void genocide();

private:

	State bestFound;
	std::vector<State> currentPopulation;
};

#endif
