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

static MYSQL* conn = NULL;

static void openDB(void){

	conn = mysql_init(NULL);

	if(!conn){
		cout << "MySQL init failed" << endl;
		exit(1);
	}

	conn = mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);

	if(!conn){
		cout 	<< "Failed to connect to " << DATABASE 
				<< " on " << SERVER << endl;

		exit(1);
	}
}

void closeDB(){
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

Game getGame(int season, int team, int day){

	if(!conn)
		openDB();

	Game g;
	ostringstream sstream;

	sstream 	<< "SELECT * FROM GAMES WHERE season = "
				<< season << " AND team = " << team 
				<< " AND day = " << day << ";";

	MYSQL_RES* 	result_set;
	MYSQL_ROW 	row;

	mysql_query(conn, sstream.str().c_str());
	result_set = mysql_store_result(conn);

	if((row = mysql_fetch_row(result_set)) != NULL){
		g = parseGame(row);
	}
	else{
		cout << "Failed to fetch game" << endl;
		exit(1);
	}

	return g;
}

vector<Game> getGames(int season, int team){

	if(!conn)
			openDB();

	vector<Game> games;
	ostringstream sstream;

	sstream 	<< "SELECT * FROM GAMES WHERE season = "
				<< season << " AND team = " << team 
				<< " ORDER BY day;";

	MYSQL_RES* 	result_set;
	MYSQL_ROW 	row;

	mysql_query(conn, sstream.str().c_str());
	result_set = mysql_store_result(conn);

	while((row = mysql_fetch_row(result_set)) != NULL){
		Game g = parseGame(row);
		games.push_back(g);
	}

	return games;
}

static Seed parseSeed(MYSQL_ROW r){

	Seed s;
	s.seed = 	atoi(r[0]);
	s.region = 	r[1][0];
	s.team = 	atoi(r[2]);
	s.year = 	atoi(r[3]);
	return s;
}

Seed getSeed(int team, int year){

	if(!conn)
			openDB();

	Seed s;
	ostringstream sstream;

	sstream 	<< "SELECT * FROM SEEDS WHERE team = "
				<< team << " AND year = " << year << ";";

	MYSQL_RES* 	result_set;
	MYSQL_ROW 	row;

	mysql_query(conn, sstream.str().c_str());
	result_set = mysql_store_result(conn);

	if((row = mysql_fetch_row(result_set)) != NULL){
		s = parseSeed(row);
	}
	else{
		cout << "Failed to fetch seed" << endl;
		exit(1);
	}

	return s;
}

vector<Seed> getSeeds(int year){

	if(!conn)
			openDB();

	vector<Seed> seeds;
	ostringstream sstream;

	sstream 	<< "SELECT * FROM SEEDS WHERE year = "
				<< year << " ORDER BY region, seed;";

	MYSQL_RES* 	result_set;
	MYSQL_ROW 	row;

	mysql_query(conn, sstream.str().c_str());
	result_set = mysql_store_result(conn);

	while((row = mysql_fetch_row(result_set)) != NULL){
		Seed s = parseSeed(row);
		seeds.push_back(s);
		cout << "Seed: " << s.seed << " - " << s.team
						<< " " << s.region << endl;
	}

	return seeds;
}

static Result parseResult(MYSQL_ROW r){

	Result res;
	res.year = 		atoi(r[0]);
	res.round = 	atoi(r[1]);
	res.winner = 	atoi(r[2]);
	res.wscore = 	atoi(r[3]);
	res.loser = 	atoi(r[4]);
	res.lscore = 	atoi(r[5]);
	res.ot = 		atoi(r[6]);
	res.region = 	r[7][0];
	return res;
}

std::vector<Result> getResults(int year, int round){

	if(!conn)
				openDB();

		vector<Result> results;
		ostringstream sstream;

		sstream 	<< "SELECT r.*, s.region FROM RESULTS r, SEEDS s "
					<< "WHERE r.winner = s.team AND r.year = s.year "
					<< "AND r.year = " << year << " AND round = " << round
					<< " ORDER BY r.round, s.region;";

		MYSQL_RES* 	result_set;
		MYSQL_ROW 	row;

		mysql_query(conn, sstream.str().c_str());
		result_set = mysql_store_result(conn);

		while((row = mysql_fetch_row(result_set)) != NULL){
			Result r = parseResult(row);
			results.push_back(r);
		}

		return results;
}
