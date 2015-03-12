#include "genetic.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::flush;

State GeneticSimulator::search(int numberOfGenerations){

	int currentGeneration = 0;
	int populationSize = currentPopulation.size();

	/* arbitrarily set bestFound */

	if(populationSize < 2)
		return bestFound;

	bestFound = currentPopulation[0];

	while(currentGeneration < numberOfGenerations){

		populationSize = currentPopulation.size();

		cout << "- - - - - - - - - - - - - - - - -\n" << endl;
		cout << "Generation " << currentGeneration;
		cout << ", Population size: " << populationSize << endl;

		double perc = 0.0;
		double inc = 100.0 / populationSize;

		cout << perc;

		for(int i=0; i<populationSize; i++){
			cout << "...";
			flush(cout);
			currentPopulation[i].fitness();
			perc += inc;
			cout << perc;
			flush(cout);
		}
		cout << endl;

		std::sort(currentPopulation.begin(),currentPopulation.end());

		/* Do we have a new best score? */

		if(currentPopulation[0].fitness() > bestFound.fitness()){
			bestFound = currentPopulation[0];
		}

		cout << "Current best state: " << bestFound.fitness() << endl;
		cout << "- - - - - - - - - - - - - - - - -\n" << endl;
		flush(cout);

		if(bestFound.fitness() > 0.99)
			break;


		/* Build the next generation of States */

		int start = populationSize/2;

		for(int i=start; i<populationSize; i++){

			/* Select (heuristically?) two parents to breed */

			State one = currentPopulation[i - start];
			State two = currentPopulation[i];

			currentPopulation[i] = breed(one,two);
		}

		/* Mutate? 5% chance, e.g. */

		int r = rand() % 100;

		if(r < 5)
			currentPopulation[rand() % populationSize].mutate();


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

	vector<double> alphas;
	GKernel k1 = one.getKernel();
	GKernel k2 = two.getKernel();

	for(int i=0; i<4; i++){

		double alpha1 = k1.alphas[i];
		double alpha2 = k2.alphas[i];
		double avg = 0.0;

		switch(rand() % 4){
		
		case 0:
			avg = (alpha1 + alpha2) /2.0; break;
		
		case 1:
			avg = alpha1 + alpha2; break;
	
		case 2:
			avg = fabs(alpha1 - alpha2);

		case 3:
			avg = alpha1 * alpha2;
		}

		alphas.push_back(avg);
	}
	
	GKernel k3;
	k3.alphas = alphas;
	State combined(k3);

	return combined;	
}


/* Is it even ethical to call this function? */

void GeneticSimulator::genocide(){

//	const unsigned int cutoff = currentPopulation.size() / 2;

//	currentPopulation.erase(currentPopulation.begin() + cutoff,
//							currentPopulation.end());
}
