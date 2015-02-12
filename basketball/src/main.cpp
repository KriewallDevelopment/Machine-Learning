#include "genetic.h"
#include "state.h"
#include "db.h"
#include <mysql.h>
#include <vector>

int main(){

	MYSQL* conn = openDB();

	std::vector<Game> games = getGames(conn, 2003, 1104);

	closeDB(conn);

	return 0;
}
