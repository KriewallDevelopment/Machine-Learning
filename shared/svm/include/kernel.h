#ifndef KERNEL_H
#define KERNEL_H

#include <vector>

/* Kernel functions and cache */

class Kernel {

public:

	Kernel(){};
	~Kernel(){};

	inline double valAt(std::vector<double> one, std::vector<double> two){
		return kern(one,two);
	}

	inline void initGamma(int val){
		gamma = 1.0 / (1.0 * val);
	}

private:

	double cauchy(std::vector<double>, std::vector<double>);
	double kern(std::vector<double>, std::vector<double>);
	double wave(std::vector<double>, std::vector<double>);
	double klog(std::vector<double>, std::vector<double>);
	double grbf(std::vector<double>, std::vector<double>);
	double gamma;
};

#endif
