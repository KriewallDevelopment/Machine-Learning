#ifndef DATABASE_H
#define DATABASE_H

#include "dto.h"
#include <mysql.h>
#include <vector>
#include <string>


void closeDB();

std::string getTeamName(int);

/* Game data access */

Game getGame(int season, int team, int day);
std::vector<Game> getGames(int season, int team);


/* Bracket data access */

Seed getSeed(int team, int year);
std::vector<Seed> getSeeds(int year);

std::vector<Result> getResults(int year, int round);

#endif
