#include "bracket.h"
#include "db.h"
#include <vector>

using std::vector;


Bracket::Bracket(int y){

	rounds.resize(NUM_ROUNDS);
	year = y;

	/* Populate round 1 games for projection */

	vector<Result> results = getResults(y,1);

	for(int i=0; i<results.size(); i++){
		Projection p;
		p.team1 = results[i].winner;
		p.team2 = results[i].loser;
		p.prob = 1.0;
		rounds[0].push_back(p);
	}
}

void Bracket::populate(){

}

vector<Projection> Bracket::getRound(int r){
	return rounds[r-1];
}

void Bracket::setRound(vector<Projection> v, int r){
	rounds[r] = v;
}

void Bracket::simulateRound(int round){

}

Bracket::~Bracket(){

}
