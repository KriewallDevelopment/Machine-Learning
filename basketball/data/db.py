#!/usr/bin/env python

import MySQLdb as mysql
import sys

HOST = ''
USER = ''
PASS = ''
DB = ''

def connectToDB():

	conn = None

	try:
		conn = mysql.connect(HOST, USER, PASS, DB)
	except mysql.Error, e:
		print e
		return None
	
	return conn

def executeQuery(conn, query):

	try:
		cursor = conn.cursor()
		cursor.execute(query)
		return cursor.rowcount
	except mysql.Error, e:
		print e
		print query
		return -1

def executeForResult(conn, query):
	try:
		cursor = conn.cursor()
		cursor.execute(query)
		return cursor.fetchall()
	except mysql.Error, e:
		print e
		print query
		return -1

def wordExists(conn, word):

	q = "SELECT word_id FROM WORD_TBL WHERE word='"+word+"';"
	r = executeQuery(conn, q)

	if r == 0:
		return False
	return True

def insertWord(conn, word):
	word = conn.escape_string(word)
	q = "INSERT INTO WORD_TBL (word) VALUES ('"+word+"');"
	executeQuery(conn,q)

def insertPOS(conn,word,pos,weight):
	word = conn.escape_string(word)
	pos = conn.escape_string(pos)
	q = "INSERT INTO POS_TBL (word_id,pos,weight) VALUES ((SELECT id FROM WORD_TBL WHERE word='"+word+"'),'"+pos+"',"+str(weight)+");"
	executeQuery(conn,q)

def disconnectFromDB(conn):
	conn.commit()
	conn.close()


conn = connectToDB()
executeQuery(conn,"")

f = open("regular_season_detailed_results.csv", 'r')
cnt = 0;

for line in f:
	cnt = cnt + 1
	print cnt
	lst = line.split(",",34)
	lst[33] = lst[33].strip()

	if(lst[6] == 'A'):
		lst[6] = 'H'
	elif(lst[6] == 'H'):
		lst[6] = 'A'

	query = "INSERT INTO GAMES (season, day, team, opp, score, oscore, \
								home, ot, fgm, fga, fgm3, fga3, ftm, fta, \
								orbd, drbd, ast, tovrs, stl, blk, pf) \
			VALUES ("+	lst[0] 	+ "," + \
						lst[1]	+ "," + \
						lst[4]	+ "," + \
						lst[2]	+ "," + \
						lst[5]	+ "," + \
						lst[3]	+ ",'" + \
						lst[6]	+ "'," + \
						lst[7]	+ "," + \
						lst[21]	+ "," + \
						lst[22]	+ "," + \
						lst[23]	+ "," + \
						lst[24]	+ "," + \
						lst[25]	+ "," + \
						lst[26]	+ "," + \
						lst[27]	+ "," + \
						lst[28]	+ "," + \
						lst[29]	+ "," + \
						lst[30]	+ "," + \
						lst[31]	+ "," + \
						lst[32]	+ "," + \
						lst[33]	+ ");"

	executeQuery(conn,query)

conn.commit()
