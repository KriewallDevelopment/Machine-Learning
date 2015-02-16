#include "genetic.h"
#include "state.h"
#include "bracket.h"
#include "db.h"
#include "dto.h"
#include <mysql.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main(){

	Bracket b(2001);

	vector<Projection> round1 = b.getRound(1);

	for(int i=0; i < round1.size(); i++){

		cout << round1[i].team1 << endl;
	}

	closeDB();

	return 0;
}
