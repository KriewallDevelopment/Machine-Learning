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
		cout << "alphas: ";
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

		alphas.push_back((alpha1 + alpha2) / 2.0);
	}
	
	GKernel k3;
	k3.alphas = alphas;
	State combined(k3);

	if(rand() % 100 < mutationChance)
		combined.mutate();

	return combined;	
}

void GeneticSimulator::genocide(){

	cout << "GENOCIDE" << endl;

	int half = currentPopulation.size() / 2;

	currentPopulation[4].setKernelAlpha(0,1.0);
	currentPopulation[4].setKernelAlpha(0,0.0);
	currentPopulation[4].setKernelAlpha(0,0.0);
	currentPopulation[4].setKernelAlpha(0,0.0);

	currentPopulation[5].setKernelAlpha(0,0.0);
	currentPopulation[5].setKernelAlpha(0,1.0);
	currentPopulation[5].setKernelAlpha(0,0.0);
	currentPopulation[5].setKernelAlpha(0,0.0);

	currentPopulation[6].setKernelAlpha(0,0.0);
	currentPopulation[6].setKernelAlpha(0,0.0);
	currentPopulation[6].setKernelAlpha(0,1.0);
	currentPopulation[6].setKernelAlpha(0,0.0);

	currentPopulation[7].setKernelAlpha(0,0.0);
	currentPopulation[7].setKernelAlpha(0,0.0);
	currentPopulation[7].setKernelAlpha(0,0.0);
	currentPopulation[7].setKernelAlpha(0,1.0);
}
