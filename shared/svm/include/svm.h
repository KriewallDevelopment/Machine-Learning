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

	inline void setKernelType(int type){
		k.setType(type);
	}

	inline void setTrainingData(std::vector< Vector > data, 
								std::vector<int> yvals){

		dimension = yvals.size();
		trainingVectors = data;
		yValues = yvals;
	};

	void train();
	std::vector<int> project(std::vector<Vector>);
	
	void drawPlane(int,int);


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

	inline double infDP(Vector v){

		double sum = 0.0;

		for(int i=0; i < dimension; i++){
				sum += (yValues[i] * alphas[i] *
						k.eval(v, trainingVectors[i]));
		}

		return sum;
	}

	inline double objective(Vector v){

        double sum = 0.0;

		if(k.isLinear()){
			return dot(w,v) - b;
		}
		else{
			sum = infDP(v);
		}

        //return sin(sum);
		return sum - b;
    }

	inline double Q(int one, int two){
		return yValues[one] * yValues[two] * 
				k.eval(trainingVectors[one],trainingVectors[two]);
	}

	inline double getC(int idx){
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

	/* Linear data */

	Vector w;
	double b;
};

#endif
