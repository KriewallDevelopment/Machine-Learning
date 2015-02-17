#ifndef FACTORS_H_
#define FACTORS_H_

#include <vector>

typedef std::vector<double> Vector;

#define FACTOR_COUNT	(1)

Vector getFactors(int year, int team);

double getScore(Vector,Vector);

#endif /* FACTORS_H_ */
