#ifndef _DECISION_TREE_H
#define _DECISION_TREE_H

#include <vector>

typedef std::vector<double> FeatureVector;

typedef struct _edge{

	double probability;
	Node* endNode;

} Edge;

typedef struct _node {

	double expectedValue;
	std::vector<Edge> edges;

} Node;

class DecisionTree {

};

#endif
