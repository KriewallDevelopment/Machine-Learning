#include "kernel.h"
#include <vector>
#include <cmath>
#include <iostream>

using std::vector;
using std::exp;
using std::cout;
using std::endl;
using std::fill;

static double mult = 0.0;


static double dot(vector<double> one, vector<double> two){

	double sum = 0.0;

	for(int i=0; i<one.size(); i++)
		sum += one[i] * two[i];

	return sum;	
}

double Kernel::kern(vector<double> one, vector<double> two){

	switch(type){

	case 0:
		return linear(one,two);

	case 1:
		//return pow(gamma * dot(one,two) + 1.0, 3.0);
		return pow(gamma * dot(one,two) + 0.0, 3.0);

	case 2:
		return grbf(one,two);
	}
}

double Kernel::linear(vector<double> one, vector<double> two){
	return dot(one,two);
}

double Kernel::wave(vector<double> one, vector<double> two){

	const int max = one.size();
	double angle = 0.0, val1 = 0.0, val2 = 0.0, diff = 0.0;
    vector<double> out;
	
	for (int i = 0; i < max; i++)
	{
		diff += (one[i] - two[i]) * (one[i] - two[i]);
		angle += one[i] * two[i];
		val1 += one[i] * one[i];
		val2 += two[i] * two[i];
	}
	
	val1 = sqrt(val1);
	val2 = sqrt(val2);
	diff = sqrt(diff);
	
	angle /= (val1 * val2);

	return (angle/diff) * sin(diff/angle);
}
double Kernel::klog(vector<double> one, vector<double> two){

	const int max = one.size();
	double diff = 0.0;
	double exponent = 7;

	for (int i = 0; i < max; i++)
		diff += (one[i] - two[i]) * (one[i] - two[i]);

	return -1.0 * log(pow(diff, 0.5*exponent));
}
double Kernel::grbf(vector<double> one, vector<double> two){

	const int max = one.size();
	double sum = 0.0;	

	for (int i = 0; i < max; i++)
		sum += (one[i] - two[i]) * (one[i] - two[i]);

	sum = sqrt(sum);

	return exp(-gamma * sum);
}
double Kernel::cauchy(vector<double> one, vector<double> two){

    const int max = one.size();
    double sum = 0.0;
	double gamma1 = gamma;

    vector<double> out;

    for(int i=0; i < max; i++)
        out.push_back(one[i] - two[i]);

    for(int i=0; i<max; i++)
        sum += (out[i] * out[i]);

    return 1.0 / ((sum * gamma1));
}
