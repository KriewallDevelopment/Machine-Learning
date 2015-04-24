#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "state.h"
#include "svm.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;

//extern struct svm_problem 	prob;
//extern struct svm_parameter param;
//extern struct svm_model*	pmodel;

struct svm_problem		globalProblem;
struct svm_parameter	globalParam;
struct svm_model		globalModel;
//struct svm_model*		pmodel;

extern vector<cv::Mat>	trainImages;
extern vector<cv::Mat>	testImages;

extern vector<int>		trainLabels;
extern vector<int>		testLabels;

extern int 			size;
extern double 		predict_main(int,char*[]);
extern svm_model	train_main(int, char*[]);
extern int			scale_main(int, char*[]);

char* scale_args1[] = {	"./svm-scale", 
						"training" };

char* scale_args2[] = {	"./svm-scale", 
						"testing" };


char* predict_args[] = {	"./svm-predict", 
							"scaled", 
							"scaled.model", 
							"/dev/null" };

char* train_args[] = {	"./svm-train", 
						"-t", 
						"1", 
						"-d", 
						"5", 
						"-m", 
						"1000", 
						"scaled" };

State::State(){

	scoreIsCached = false;
	fitnessScore = -1.0;
}

void State::print(){

	cout << "< ";

	for(int i=0; i < features.size(); i++){

		if(i == features.size() - 1)
			cout << features[i].name;
		else
			cout << features[i].name << ", ";
	}

	cout << " >" << endl;
}

double State::fitness(){

	if(scoreIsCached)
		return fitnessScore;

	/* Build vectors with current features */

	FEATURE general;
	general.name = "pixels";
	general.eval = &pixels;

	features.push_back(general);


	ofstream training("training");
	ofstream testing("testing");

	for(int i = 0; i < trainImages.size(); i++){

		training << trainLabels[i] << " ";

		int idx = 1;

		for(int j=features.size()-1; j > -1; j--){

			vector<double> vals = features[j].eval(trainImages[i]);

			for(int k=0; k < vals.size(); k++)
				training << idx++ << ":" << vals[k] << " ";
		}

		training << endl;
	}

	for(int i = 0; i < testImages.size(); i++){

		testing << testLabels[i] << " ";

		int idx = 1;

		for(int j=features.size()-1; j > -1; j--){

			vector<double> vals = features[j].eval(testImages[i]);

			for(int k=0; k < vals.size(); k++)
				testing << idx++ << ":" << vals[k] << " ";
		}

		testing << endl;
	}

	training.close();
	testing.close();

	features.pop_back();

	scale_main(2, scale_args1);

	//svm_model model;

	//model = train_main(8, train_args);
	//pmodel= &model;

	train_main(8, train_args);
	scale_main(2, scale_args2);

	double score = predict_main(4, predict_args);
	//svm_free_and_destroy_model(&pmodel);

	/* Cache for when we get called again */

	print();
	cout << "Scored " << score << endl;

	scoreIsCached = true;
	fitnessScore = score;

	return score;
}

void State::mutate(){

	/*
	 * Slightly change the state with a random tweak.
	 * This will get called very infrequently
	 */

	cout << "MUTATION" << endl;
}

State::~State(){

}
