#include "factors.h"
#include "db.h"

/* Each factor should be given a score between 1 and 100 */

Vector getFactors(int year, int team){

	Vector factors(FACTOR_COUNT);

	factors[0] = 100.0 - (6.25 * ((double)getSeed(team,year).seed));

	return factors;
}


/* Score a team based on weights and factors */

double getScore(Vector factors, Vector weights){

	return factors[0];
}
