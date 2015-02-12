#ifndef STATE_H
#define STATE_H

class State {

public:

	State();
	~State();

	State(const State &s){

		// TODO
    }

    bool operator == (const State &s) {

        // TODO
    }

	/* Could also return double here */

	int fitness();
	
	/* Should be called a small percentage of the time */

	void mutate();

private:

	bool scoreIsCached;
	int fitnessScore;

	/* DEFINE ACTUAL STATE VARIABLES HERE */
};

#endif
