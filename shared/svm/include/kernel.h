#ifndef KERNEL_H
#define KERNEL_H

#include <vector>

/* Kernel functions and cache */

class Kernel {

typedef double (Kernel::*KERNEL_FUNCTION)(std::vector<double>,std::vector<double>);

public:

	Kernel(){};
	~Kernel(){};

	inline void setType(int t){
		type = t;
	}

	inline bool isLinear(){
		return type == 0;
	}

	inline double eval(std::vector<double> one, std::vector<double> two){
		return kern(one,two);
	}

	inline void initGamma(int val){
		gamma = 1.0 / (1.0 * val);
	}

private:

	int type;
	
	double kern(std::vector<double>, std::vector<double>);
	double cauchy(std::vector<double>, std::vector<double>);
	double wave(std::vector<double>, std::vector<double>);
	double linear(std::vector<double>,std::vector<double>);
	double klog(std::vector<double>, std::vector<double>);
	double grbf(std::vector<double>, std::vector<double>);
	double gamma;
};

#endif
