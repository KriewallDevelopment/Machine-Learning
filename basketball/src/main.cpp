#include "genetic.h"
#include "state.h"
#include "bracket.h"
#include "db.h"
#include "factors.h"
#include "dto.h"
#include <mysql.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

#define YEAR	(2014)

int main(){

	Bracket b(YEAR);

	vector<Projection> round1 = b.getRound(1);
	vector<Projection> round1Solution = b.getRound(1);

	Vector weights;

	for(int i=0; i < round1.size(); i++){

		if(i != 0 && i % 8 == 0)
			cout << endl;

		Vector team1Factors = getFactors(YEAR, round1[i].team1);
		Vector team2Factors = getFactors(YEAR, round1[i].team2);

		cout << getTeamName(round1[i].team1) << " / " << getTeamName(round1[i].team2) << endl;

		double score1 = getScore(team1Factors,weights);
		double score2 = getScore(team2Factors,weights);

		if(score1 > score2){
			round1[i].prob = 1.0;
		}
		else
			round1[i].prob = -1.0;
	}

	b.setRound(round1,1);
	b.simulateRound(1);

	vector<Projection> round2 = b.getRound(2);
	cout << "ROUND 2" << endl;
	for(int i=0; i<round2.size(); i++){
		cout << getTeamName(round2[i].team1) << " / " << getTeamName(round2[i].team2) << endl;
	}

	double total = 0.0;
	double correct = 0.0;

	for(int i=0; i < round1.size(); i++){

		if(round1[i] == round1Solution[i]){
			correct += 1.0;
		}

		total += 1.0;
	}

	cout << "Scored " << (correct / total) * 100.0
			<< "% on round 1 (" << YEAR << " season)" << endl;

	closeDB();

	return 0;
}
