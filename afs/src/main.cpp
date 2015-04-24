#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <vector>
#include "svm.h"
#include "seg.h"
#include "mnist.h"
#include "state.h"
#include "genetic.h"

using namespace std;

using cv::Mat;
using cv::Rect;
using cv::Size;
using cv::Scalar;

int NUM_GENERATIONS = 5;

vector<Mat> trainImages;
vector<Mat> testImages;

vector<int> trainLabels;
vector<int> testLabels;

void perform_afs(vector<FEATURE> features){

	trainImages = getImages(TRAIN_IMAGES, 1000);
	testImages = getImages(TEST_IMAGES, 10000);

	trainLabels = getLabels(TRAIN_LABELS, 1000);
	testLabels = getLabels(TEST_LABELS, 10000);

	GeneticSimulator sim;
	State best;

	for(int i=0; i < features.size(); i++){

		State s;
		s.features.push_back(features[i]);
		sim.addToPopulation(s);
	}

	best = sim.search(NUM_GENERATIONS);

	printf("Genetic search completed. ");
	printf("Best vector scored %f\n", best.fitness());
	best.print();
}

int main(int argc, char **argv){

	vector<FEATURE> features;
	FEATURE f;

	f.name = string("skew");
	f.eval = &skew;
	features.push_back(f);

	f.name = string("bounds");
	f.eval = &bounds;
	features.push_back(f);

	f.name = string("corners");
	f.eval = &corners;
	features.push_back(f);

	f.name = string("on_pixels");
	f.eval = &on_pixels;
	features.push_back(f);

	perform_afs(features);

	return 0;
}
