#ifndef GENETIC_H
#define GENETIC_H

#include "state.h"
#include <vector>
#include <ctime>
#include <stdlib.h>

class GeneticSimulator {

public:

	GeneticSimulator(int p){
		populationSize = p;
		srand(time(0));
	};

	/* Finds and returns the most fit state across numberOfGenerations */

	State search(int numberOfGenerations);


	/* Define how to combine two states */

	State breed(State,State);


	/* optional */

	void genocide();

private:

	int populationSize;

	State bestFound;
	std::vector<State> currentPopulation;
};

#endif
