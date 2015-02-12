#ifndef DTO_H
#define DTO_H

#include <string>

//mysql> CREATE TABLE TEAMS (
//    -> id 	INT UNSIGNED PRIMARY KEY,
//    -> name 	CHAR(20));

typedef struct _team {

	int id;
	std::string name;
	
} Team;

// CREATE TABLE `GAMES` (
//  `season` int(11) NOT NULL DEFAULT '0',
//  `day` int(11) NOT NULL DEFAULT '0',
//  `team` int(10) unsigned NOT NULL DEFAULT '0',
//  `opp` int(10) unsigned DEFAULT NULL,
//  `score` int(11) DEFAULT NULL,
//  `oscore` int(11) DEFAULT NULL,
//  `home` char(1) DEFAULT NULL,
//  `ot` int(11) DEFAULT NULL,
//  `fgm` int(11) DEFAULT NULL,
//  `fga` int(11) DEFAULT NULL,
//  `fgm3` int(11) DEFAULT NULL,
//  `fga3` int(11) DEFAULT NULL,
//  `ftm` int(11) DEFAULT NULL,
//  `fta` int(11) DEFAULT NULL,
//  `orbd` int(11) DEFAULT NULL,
//  `drbd` int(11) DEFAULT NULL,
//  `ast` int(11) DEFAULT NULL,
//  `tovrs` int(11) DEFAULT NULL,
//  `stl` int(11) DEFAULT NULL,
//  `blk` int(11) DEFAULT NULL,
//  `pf` int(11) DEFAULT NULL,
//  PRIMARY KEY (`season`,`day`,`team`),

typedef struct _game {

	int season;
	int day;
	int team;
	int opp;
	int score;
	int oscore;
	char home;
	int ot;
	int fgm;		//- field goals made
	int fga;		//- field goals attempted
	int fgm3;		//- three pointers made
	int fga3;		//- three pointers attempted
	int ftm;		//- free throws made
	int fta;		//- free throws attempted
	int orbd;		//- offensive rebounds
	int drbd;		//- defensive rebounds
	int ast;		//- assists
	int tovrs;		//- turnovers
	int stl;		//- steals
	int blk;		//- blocks
	int pf;			//- personal fouls

} Game;

// CREATE TABLE `SEEDS` (
//  `seed` int(11) DEFAULT NULL,
//  `region` char(1) DEFAULT NULL,
//  `team` int(10) unsigned NOT NULL DEFAULT '0',
//  `year` int(11) NOT NULL DEFAULT '0',
//  PRIMARY KEY (`team`,`year`),

typedef struct _seed {

	int seed;
	char region;
	int team;
	int year;

} Seed;

// CREATE TABLE `RESULTS` (
//  `year` int(11) NOT NULL DEFAULT '0',
//  `round` int(11) NOT NULL DEFAULT '0',
//  `winner` int(10) unsigned NOT NULL DEFAULT '0',
//  `wscore` int(11) DEFAULT NULL,
//  `loser` int(10) unsigned DEFAULT NULL,
//  `lscore` int(11) DEFAULT NULL,
//  `ot` int(11) DEFAULT NULL,
//  PRIMARY KEY (`year`,`round`,`winner`),

typedef struct _result {

	int year;
	int round;
	int winner;
	int wscore;
	int loser;
	int lscore;
	int ot;

} Result;

#endif
