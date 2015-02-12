#ifndef DATABASE_H
#define DATABASE_H

#include "dto.h"
#include <mysql.h>
#include <vector>


/* Connections */

MYSQL* 	openDB(void);
void	closeDB(MYSQL*);

Game getGame(MYSQL*, int season, int team, int day);
std::vector<Game> getGames(MYSQL*, int season, int team);

#endif
