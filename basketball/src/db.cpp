#include "db.h"
#include "dto.h"
#include <mysql.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;


MYSQL* openDB(void){

	MYSQL* val = mysql_init(NULL);

	if(!val){
		cout << "MySQL init failed" << endl;
		exit(1);
	}

	val = mysql_real_connect(val,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);

	if(!val){
		cout 	<< "Failed to connect to " << DATABASE 
				<< " on " << SERVER << endl;

		exit(1);
	}

	return val;
}

void closeDB(MYSQL* conn){
	mysql_close(conn);
}

static Game parseGame(MYSQL_ROW r){

	Game g;
	
	g.season 	= atoi(r[0]);
    g.day		= atoi(r[1]);
    g.team		= atoi(r[2]);
    g.opp		= atoi(r[3]);
    g.score		= atoi(r[4]);
    g.oscore 	= atoi(r[5]);
    g.home		= r[6][0];
    g.ot		= atoi(r[7]);
    g.fgm		= atoi(r[8]);
    g.fga		= atoi(r[9]);
    g.fgm3		= atoi(r[10]);
    g.fga3		= atoi(r[11]);
    g.ftm		= atoi(r[12]);
    g.fta		= atoi(r[13]);
    g.orbd		= atoi(r[14]);
    g.drbd		= atoi(r[15]);
    g.ast 		= atoi(r[16]);
    g.tovrs 	= atoi(r[17]);
    g.stl 		= atoi(r[18]);
    g.blk 		= atoi(r[19]);
    g.pf 		= atoi(r[20]);

	return g;
}

Game getGame(MYSQL* conn, int season, int team, int day){

	Game g;
	ostringstream sstream;

	sstream 	<< "SELECT * FROM GAMES WHERE season = "
				<< season << " AND team = " << team 
				<< " AND day = " << day << ";";

	MYSQL_RES* 	result_set;
	MYSQL_ROW 	row;
	int rows;

	mysql_query(conn, sstream.str().c_str());
	result_set = mysql_store_result(conn);
	rows = mysql_num_rows(result_set);

	if((row = mysql_fetch_row(result_set)) != NULL){
		g = parseGame(row);
		cout << "game score: " << g.score << "-" << g.oscore 
				<< " " << g.home << endl;
	}
	else{
		cout << "Failed to fetch game" << endl;
		exit(1);
	}

	return g;
}

vector<Game> getGames(MYSQL* conn, int season, int team){

	vector<Game> games;
	ostringstream sstream;

	sstream 	<< "SELECT * FROM GAMES WHERE season = "
				<< season << " AND team = " << team 
				<< " ORDER BY day;";

	MYSQL_RES* 	result_set;
	MYSQL_ROW 	row;
	int rows;

	mysql_query(conn, sstream.str().c_str());
	result_set = mysql_store_result(conn);
	rows = mysql_num_rows(result_set);

	while((row = mysql_fetch_row(result_set)) != NULL){
		Game g = parseGame(row);
		games.push_back(g);
		cout << "game score: " << g.score << "-" << g.oscore 
				<< " " << g.home << endl;
	}

	return games;
}
