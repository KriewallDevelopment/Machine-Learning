#include "genetic.h"
#include "state.h"
#include <stdlib.h>

State GeneticSimulator::search(int numberOfGenerations){
	
	int currentGeneration = 0;

	/* Set up initial population here and arbitrarily set bestFound */

	while(currentGeneration < numberOfGenerations){

		/* Do we have a new best score? */

		for(int i=0; i<populationSize; i++)
			if(currentPopulation[i].fitness() > bestFound.fitness())
				bestFound = currentPopulation[i];


		/* Build the next generation of States */


		for(int i=0; i<populationSize; i++){

			/* Select (heuristically?) two parents to breed */

			//random example

			State one = currentPopulation[i];
			State two = currentPopulation[populationSize - 1 - i];

			currentPopulation[i] = breed(one,two);
		}

		/* Mutate? 5% chance, e.g. */

		int r = rand() % 100;

		if(r < 5)
			currentPopulation[rand() % populationSize].mutate();


		/* Natural selection? */

		if(currentGeneration % 10 == 0)
			genocide();

		currentGeneration++;
	}

	return bestFound;
}

State GeneticSimulator::breed(State one, State two){

	/* 
	 * Combine these two states in some form
	 * Maybe use more of the state that is more fit?
	 */	
}


/* Is it even ethical to call this function? */

void GeneticSimulator::genocide(){

	const unsigned int cutoff = currentPopulation.size() / 2;

	currentPopulation.erase(currentPopulation.begin() + cutoff,
							currentPopulation.end());
}
