#include "state.h"

State::State(){

	scoreIsCached = false;
	fitnessScore = -1;
}

int State::fitness(){

	if(scoreIsCached)
		return fitnessScore;


	/* Score this state! */

	int score = 0;


	/* Cache for when we get called again */

	scoreIsCached = true;
	fitnessScore = score;

	return score;
}

void State::mutate(){

	/*
	 * Slightly change the state with a random tweak.
	 * This will get called very infrequently
	 */


}

State::~State(){

}
