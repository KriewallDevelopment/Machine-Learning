#include "svm.h"
#include "stdhdr.h"
#include <vector>
#include <cmath>
#include <cfloat>
#include <iostream>

using std::vector;
using std::fill;
using std::exp;
using std::cout;
using std::endl;

static double epsilon = 1e-8;
static double tau = 1e-12;

static double planeEpsilon = 1e-4;

static void printVector(Vector v){

	cout << "VECTOR DUMP: ";

	for(int i=0; i<v.size(); i++)
		cout << v[i] << " ";

	cout << endl;
}

static Vector addVectors(Vector v1, Vector v2){

	Vector ret;

	for(int i=0; i<v1.size(); i++)
		ret.push_back(v1[i] + v2[i]);

	return ret;
}

static void scalarMult(Vector &v, double s){

	for(int i=0; i < v.size(); i++)
		v[i] *= s;
}

void SVM::drawPlane(int w, int h){

	const int HW = w / 2;
	const int HH = h / 2;

	glBegin(GL_POINTS);

	for(int i=0; i<w; i++){
		for(int j=0; j<h; j++){
			Vector v;
			v.push_back(i - HW);
			v.push_back(j - HH);

			double score = objective(v);
			
			if(score < planeEpsilon && score > -planeEpsilon)
				glColor3f(0.0,0.0,0.0);
			else if(score < 0.0)
				glColor3f(0.5,0.5,1.0);
			else
				glColor3f(1.0,0.5,0.5);
			
			glVertex2i(i,j);
		}
	}

	glEnd();
}

void SVM::train(){

	/* Scale the data for faster computation */

	//scale(-1,1);
	k.initGamma(trainingVectors[0].size());
	executeSMO();
}

vector<int> SVM::project(vector<Vector> tests){

	vector<int> out;

	for(int i=0; i<tests.size(); i++){

        double score = objective(tests[i]);

        int result;

        if(score > 0.0){
            result = 1;
		}
        else if(score < 0.0){
            result = -1;
		}
        else{
            result = 0;
		}

        out.push_back(result);
    }

	return out;
}

void SVM::scale(int lb, int ub){

	std::size_t size = trainingVectors[0].size();

	double maxs[size];
	double mins[size];

	for(int i=0; i<size; i++){
		maxs[i] = DBL_MIN;
		mins[i] = DBL_MAX;
	}

	for(int i=0; i < trainingVectors.size(); i++){

		Vector v = trainingVectors[i];

		for(int j=0; j < size; j++){
		
			if(v[j] > maxs[j])
				maxs[j] = v[j];
			else if(v[j] < mins[j])
				mins[j] = v[j];
		}
	}

	for(int i=0; i < trainingVectors.size(); i++){

		Vector v = trainingVectors[i];

		for(int j=0; j < size; j++){

			if(maxs[j] == mins[j])
				v[j] = 0;
			else
				v[j] = (1.0 * lb) + 
					((v[j] / maxs[j] - mins[j]) * (1.0 * (ub - lb)));
		}
	}
}

void SVM::selectSet(int& one, int& two){

	one = -1;
	two = -1;

	double ub, lb;

	for(int i=0; i<yValues.size(); i++){

		GET_BOUNDS(i,lb,ub);

		double y = yValues[i];
		double a = alphas[i];
		double g = gradient[i];

		double current = y * g;

		if(one == -1 && (y * a) < ub){
			one = i;
			continue;
		}
		else if(one == -1)
			continue;

		double omax = yValues[one] * gradient[one];
	
		if(current > omax && (y * a) < ub)
			one = i;
	}

	for(int i=0; i<yValues.size(); i++){

		GET_BOUNDS(i,lb,ub);

		double y = yValues[i];
		double a = alphas[i];
		double g = gradient[i];

		double current = y * g;

		if(two == -1 && (y * a) > lb){
			two = i;
			continue;
		}
		else if(two == -1)
			continue;

		double omin = yValues[two] * gradient[two];
	
		if(current < omin && (y * a) > lb)
			two = i;
	}
}

void SVM::executeSMO(){

	/* Initialize Lagrange Multipliers to 0 to begin optimization */

	int trainingCount = trainingVectors.size();
	int idx1, idx2;
	double lambda;

	alphas.resize(trainingCount);
	gradient.resize(trainingCount);
	fill(alphas.begin(), alphas.end(), 0.0);
	fill(gradient.begin(), gradient.end(), 1.0);

	C = 0.5;
	C1 = C2 = C;
	//C1 = 10.0;
	//C2 = 0.001;

	int maxIterations = 10000;
	int count = 0;

	cout << "Building Model..." << endl;

	while(count < maxIterations){

		count++;

		if(count % (maxIterations / 5) == 0){
			cout << " " << (int)(((count*1.0)/(maxIterations*1.0))*100.0) 
				 << "% ";
			cout.flush();
		}
		else if(count % (maxIterations / 25) == 0){
			cout << ".";
			cout.flush();
		}

		selectSet(idx1, idx2);

		if (idx2 == -1 || idx1 == -1)
			break;

		/* Check optimality criterion */

		if(yValues[idx1]*gradient[idx1] <= yValues[idx2]*gradient[idx2])
			break;

		Vector v1 = trainingVectors[idx1];
		Vector v2 = trainingVectors[idx2];

		/* Calculate lambda */

		double ilb;
		double iub;
		double jlb;
		double jub;

		GET_BOUNDS(idx1, ilb, iub);
		GET_BOUNDS(idx2, jlb, jub);

		double val1, val2, val3;

		val1 = iub - (yValues[idx1] * alphas[idx1]);
		val2 = (yValues[idx2] * alphas[idx2]) - jlb;

		val3 = 	(yValues[idx1]*gradient[idx1]	- 
				 yValues[idx2]*gradient[idx2])	/
				(k.eval(v1,v1) + k.eval(v2,v2) - 2 * k.eval(v1,v2));

		lambda = MIN(MIN(val1,val2), val3);

		/* Update gradient vector */

		for(int i=0; i<gradient.size(); i++){
			gradient[i] = gradient[i] - 
						(lambda * yValues[i] * 
						k.eval(v1,trainingVectors[i]) + 
						(lambda * yValues[i] *
						k.eval(v2,trainingVectors[i])));
		}

		/* Update alphas */

		alphas[idx1] += yValues[idx1] * lambda;
		alphas[idx2] -= yValues[idx2] * lambda;
	}

	w.resize(2,0.0);

	for(int i=0; i<alphas.size(); i++){
		
		Vector x = trainingVectors[i];
		scalarMult(x, alphas[i] * yValues[i]);
		w = addVectors(w,x);
	}

	cout << endl;

	if(k.isLinear())
		cout << "W: <" << w[0] << "," << w[1] << ">" << endl;
	else
		cout << "W has infinite dimensions" << endl;


	/* How many support vectors do we have? */	

	int svs = 0;

	for(int i=0;i<alphas.size(); i++)
		if(alphas[i] != 0.0)
			svs++;

	/* Compute b */

	double sum = 0.0;

	for(int i=0;i<alphas.size(); i++){

		if(alphas[i] != 0.0){
		
			if(k.isLinear())
				sum += (dot(trainingVectors[i],w) - yValues[i]);
			else
				sum += (infDP(trainingVectors[i]) - yValues[i]);
		}
	}

	b = (1.0 / (1.0 * svs)) * sum;
	cout << "b: " << b << endl;

	cout << "Identified " << svs << " support vectors." << endl;
	cout << "Analyzing results..." << endl;
}

