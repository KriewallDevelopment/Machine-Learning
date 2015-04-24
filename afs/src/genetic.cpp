#include "genetic.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>

#define MIN(x,y) ((x < y) ? x : y)

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
			mutationChance = 5;
		}
		else{
		
			/* Increase mutations when we hit top of hill */

			mutationChance = MIN(mutationChance + 5, 100);
			cout << "Mutation Chance: " << mutationChance << endl;

			if(mutationChance > 75){
				genocide();
				mutationChance = 5;
			}
		}

		cout << "Current best state: " << bestFound.fitness() << endl;
		bestFound.print();
		cout << "- - - - - - - - - - - - - - - - -\n" << endl;
		flush(cout);

		if(bestFound.fitness() > 0.995)
			break;


		/* Build the next generation of States */

		int start = populationSize/2;

		for(int i=start; i<populationSize; i++){

			/* Select (heuristically?) two parents to breed */

			State one = currentPopulation[i - start];
			State two = currentPopulation[i];

			currentPopulation[i] = breed(one,two);
		}

		currentGeneration++;
	}

	return bestFound;
}

static std::vector<FEATURE> 
possible(	std::vector<FEATURE> one, 
			std::vector<FEATURE> two)
{

	std::vector<FEATURE> out;

	for(int i=0; i < two.size(); i++){

		bool present = false;

		for(int j=0; j < one.size(); j++){

			if(one[j] == two[i]){
	
				present = true;
				break;
			}
		}

		if(!present)
			out.push_back(two[i]);
	}

	return out;
}

State GeneticSimulator::breed(State one, State two){

	/* 
	 * Combine these two states in some form
	 * Maybe use more of the state that is more fit?
	 */

	State combined = one;

	std::vector<FEATURE> sel = possible(one.features,
										two.features);

	if(sel.size() != 0){
		
		int idx = rand() % sel.size();
		combined.features.push_back(two.features[idx]);
	}

	return combined;	
}

void GeneticSimulator::genocide(){

	cout << "GENOCIDE" << endl;

	int half = currentPopulation.size() / 2;
}
