#ifndef SVM_H
#define SVM_H

#include <vector>
#include <inttypes.h>
#include <cmath>
#include <iostream>
#include "kernel.h"

using std::cout;
using std::endl;
using std::vector;

typedef vector<double>	Vector;


inline double dot(Vector v1, Vector v2){

	const int max = v1.size();
	double sum = 0.0;

	for(int i=0; i<max; i++)
		sum += (v1[i] * v2[i]);

	return sum;
}

inline double squaredNormOfDifference(Vector v1, Vector v2){

	const int max = v1.size();
	Vector out;

	for(int i=0; i < max; i++)
		out.push_back(v1[i] - v2[i]);

	return dot(out,out);
}


/*
 * Use a Support Vector Machine (SVM) to classify inputs.
 * This custom SVM is tailored to the expected data.
 */


class SVM {

public:

	/* 
	 * Construct an event to score the property 
	 * for the given target value.
	 */

	SVM(){};
	~SVM(){};

	inline void setTrainingData(std::vector< Vector > data, 
								std::vector<int> yvals){

		dimension = yvals.size();
		trainingVectors = data;
		yValues = yvals;
	};

	void train();
	std::vector<int> project(std::vector<Vector>);


private:

	inline double MIN(double a, double b){
		return (a < b) ? a : b;
	}

	inline void GET_BOUNDS(int idx, double &lb, double &ub){

		if(yValues[idx] > 0){
			lb = 0.0;
			ub = getC(idx);
		}
		else {
			lb = -getC(idx);
			ub = 0.0;
		}
	}

	inline double objective(Vector v){

        double sum = 0.0;

        for(int i=0; i < dimension; i++){
            if(alphas[i] > 0.0)
                sum += (yValues[i] * alphas[i] *
                        k.valAt(v, trainingVectors[i]));
        }

        return sin(sum);
    }

	inline double Q(int one, int two){
		//cout << "Q access at: " << one << ", " << two << endl;
		return yValues[one] * yValues[two] * 
				k.valAt(trainingVectors[one],trainingVectors[two]);
		//return access(one,two);
	}

	inline double getC(int idx){
		//return C;
		return (yValues[idx] > 0) ? C1 : C2;
	}

	// SVM data

	void scale(int,int);
	void executeSMO();
	void selectSet(int&, int&);

	Kernel k;
	double C;
	double C1;
	double C2;
	uint32_t dimension;

	std::vector< Vector > 	trainingVectors;
	std::vector< int >		yValues;
	Vector 					alphas;
	Vector 					gradient;
};

#endif
