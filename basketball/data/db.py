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


#conn = connectToDB()
#executeQuery(conn,"")

f = open("tourney_slots.csv", 'r')
cnt = 0;

for line in f:
	cnt = cnt + 1
	#print cnt
	lst = line.split(",",4)
	lst[3] = lst[3].strip()

	if lst[1][1] == '5':
		print lst[0] + " " + lst[1][2:]

	#query = "INSERT INTO GAMES (season, day, team, opp, score, oscore, \
	#							home, ot, fgm, fga, fgm3, fga3, ftm, fta, \
	#							orbd, drbd, ast, tovrs, stl, blk, pf) \
	#		VALUES ("+	lst[0] 	+ "," + \
	#					lst[1]	+ "," + \
	#					lst[4]	+ "," + \
	#					lst[2]	+ "," + \

	#executeQuery(conn,query)

#conn.commit()
