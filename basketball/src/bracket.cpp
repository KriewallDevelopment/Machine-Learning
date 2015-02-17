#include "bracket.h"
#include "db.h"
#include <vector>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;

static int matchups[8] = {  0, 7, 5, 3, 2, 4, 6, 1	};

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

	vector<Projection> orders(32);

	for(int i=0; i<4; i++){
		for(int j=0; j<8; j++){

			int currentIdx = (i*8) + j;
			int targetIdx;
			int seed = getSeed(rounds[0][currentIdx].team1,y).seed;

			if(seed > 8)
				seed = 17 - seed;

			targetIdx = (i * 8) + matchups[seed - 1];
			orders[targetIdx] = rounds[0][currentIdx];
		}
	}

	rounds[0] = orders;
}

void Bracket::populate(){

}

vector<Projection> Bracket::getRound(int r){
	return rounds[r-1];
}

void Bracket::setRound(vector<Projection> v, int r){
	rounds[r-1] = v;
}

void Bracket::simulateRound(int round){

	round = round - 1;

	cout << "max: " << (int)pow(2,5-round) << endl;

	for(int i=0; i < (int)pow(2,(5-round)); i+=2){
		Projection p;
		p.team1 = (rounds[round][i].prob > 0.0) ?
				rounds[round][i].team1 : rounds[round][i].team2;
		p.team2 = (rounds[round][i+1].prob > 0.0) ?
				rounds[round][i+1].team1 : rounds[round][i+1].team2;
		p.prob = 1.0;
		rounds[round+1].push_back(p);
	}
}

Bracket::~Bracket(){

}

