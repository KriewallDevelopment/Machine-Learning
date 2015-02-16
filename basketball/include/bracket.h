#ifndef BRACKET_H
#define BRACKET_H

#include <vector>
#include <dto.h>

#define NUM_ROUNDS	(6)


typedef struct _projection {

	int team1;
	int team2;
	double prob;

	bool operator==(const struct _projection& rhs) const {
	    return team1 == rhs.team1 &&
	    		team2 == rhs.team2 &&
	    		((prob < 0.0 && rhs.prob < 0.0) ||
	    		(prob > 0.0 && rhs.prob > 0.0));
	}

} Projection;

inline bool correctProjection(Projection guess, Projection actual){
	return guess == actual;
}


/*
 * NOTE THAT WX and YZ are the regions that play
 * each other in the final four!
 */

class Bracket {

public:

	/* Populates only the first round by default */

	Bracket(int year);
	~Bracket();

	/* This will populate the all the games and the winners */

	void populate();

	/* Generate the (r + 1) round based on the winners in round r */

	void simulateRound(int r);

	std::vector<Projection> getRound(int round);
	void setRound(std::vector<Projection>, int round);

private:

	int year;
	std::vector< std::vector<Projection> > rounds;
};

#endif
