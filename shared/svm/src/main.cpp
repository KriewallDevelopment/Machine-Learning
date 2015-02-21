#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <ctime>
#include "svm.h"

using std::istringstream;
using std::vector;
using std::fstream;
using std::string;
using std::ios;
using std::cout;
using std::endl;

typedef struct record{

	Vector v;
	int score;

} Record;

static Record parseLine(string line){

	Record r;

	istringstream sstream(line);

	double dummy;
	sstream >> r.score;
	sstream >> dummy;
	r.v.push_back(dummy);
	sstream >> dummy;
	r.v.push_back(dummy);

	return r;
}

int main(){

	srand(time(0));

	SVM svm;
	vector<string> records;

	vector<Vector> trainingData;
	vector<Vector> testingData;
	vector<int> yvals;
	vector<int> svals;
	vector<int> output;

	/* Read input file */

	fstream stream;
	string file = "exampleData.csv";

    try{
        stream.open(file.c_str(), ios::in);
    }catch(...){ return 1;}

    if(!stream.good())
        return 1;

	while(!stream.eof()){

		string line;
		getline(stream, line);

		if(line.size() < 4)
			continue;

		records.push_back(line);
	}

	stream.close();

	int third = records.size() / 3;

	for(int i=0; i<third; i++){
		Record rec = parseLine(records.back());
		testingData.push_back(rec.v);
		svals.push_back(rec.score);
		records.pop_back();
	}

	for(int i=0; i<records.size(); i++){

		Record rec = parseLine(records[i]);
		trainingData.push_back(rec.v);
		int val = rec.score;
		yvals.push_back(val);
	}

	svm.setTrainingData(trainingData, yvals);
	svm.train();

	output = svm.project(testingData);

	int actualHits = 0;
	int correct = 0;
	int total = 0;
	int pones = 0;
	int nones = 0;
	int zeros = 0;

	int positiveFound = 0;

	for(int i=0; i < output.size(); i++){

		int oval = svals[i];
		int val = oval;

		if(val > 0)
			actualHits++;

		if(output[i] > 0)
			pones++;
		else if(output[i] < 0)
			nones++;
		else{
			zeros++;
			nones++;
		}

		if(output[i] == val && val > 0){

			positiveFound++;
			correct++;
		}
		else if(output[i] == val)
			correct++;
		else if(output[i] == 0 && val < 0)
			correct++;

		total++;
	}

	cout << "--------------------------------------------------" << endl;
	cout << "Predicted " << pones << " positive records and " << nones << " negative ones" << endl;
	cout << "Of the " << pones << " pridicted positives, " << positiveFound << " were actually marked +1" << endl;
	cout << "Scoring function assigned 0 to " << zeros << " records" << endl;
	cout << "--------------------------------------------------" << endl;
	cout << "Trained with " << trainingData.size() << " records" << endl;
	cout << "Evaluated " << testingData.size() << " tests with " 
		<< ((1.0 * correct)/(1.0 * total)) * 100.0 << "% accuracy" << endl;

	cout << "There were " << actualHits << " actual positives in the testing dataset" << endl;
	cout << "--------------------------------------------------" << endl;

	return 0;
}
