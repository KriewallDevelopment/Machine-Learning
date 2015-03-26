#include "genetic.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fftw3.h>

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
			//	genocide();
			//	mutationChance = 5;
			}
		}

		cout << "Current best state: " << bestFound.fitness() << endl;
		cout << "freq: ";
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

	fftw_complex freq[SAMPLE_N];

	GKernel k1 = one.getKernel();
	GKernel k2 = two.getKernel();

	fftw_complex out[SAMPLE_N], out1[SAMPLE_N], out2[SAMPLE_N];
	fftw_plan p1, p2, q;

	/* CONVOLUTION! Which is multiplication in frequency domain */

	p1 = fftw_plan_dft_1d(SAMPLE_N,k1.freq,out1,FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p1);
	p2 = fftw_plan_dft_1d(SAMPLE_N,k2.freq,out2,FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p2);

	fftw_destroy_plan(p1);
	fftw_destroy_plan(p2);

	for(int i=0; i < SAMPLE_N; i++){

		double a = out1[i][0];
		double b = out1[i][1];
		double c = out2[i][0];
		double d = out2[i][1];

		out[i][0] = (a * c) - (b * d);
		out[i][1] = (a * d) + (b * c);
   	}

	q = fftw_plan_dft_1d(SAMPLE_N, out, freq, FFTW_BACKWARD, FFTW_ESTIMATE);
   	fftw_execute(q);

	/* Normalize */

	for (int i = 0; i < SAMPLE_N; i++) {
		freq[i][0] *= 1./SAMPLE_N;
		freq[i][1] *= 1./SAMPLE_N;
   	}

	fftw_destroy_plan(q);
	fftw_cleanup();

	GKernel k3;
	k3.setArray(freq);
	State combined(k3);

	if(rand() % 100 < mutationChance)
		combined.mutate();

	return combined;	
}

void GeneticSimulator::genocide(){

}
