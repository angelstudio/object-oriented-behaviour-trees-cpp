#include "BTSelectorRandom.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool BTSelectorRandom::haveSetRandomSeed = false;

double fRand(double fMin, double fMax) {
	using namespace std;
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

BTSelectorRandom::BTSelectorRandom() {
	//Super is automatically called.
	//set the seed if we haven't already
	if (!haveSetRandomSeed) {
		srand(static_cast<unsigned int>(time(NULL)));
		haveSetRandomSeed = true;
	}
	//set number of concurrent we allow (defaulted to 1)
	setRunLimit(1);
}

BTSelectorRandom::~BTSelectorRandom() {
	//Super is automatically called.
}

//Selects one node at a weighted random, and returns true for it.
bool BTSelectorRandom::getNodeShouldRun(BTNode* _childPtr) {
	//Take the sum of all (ready) child nodes
	double nodeSum = 0;
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0 && ptrNode->getState() == BT_READY) {
			nodeSum += (double)(ptrNode->getPriority());
		}
	}
	//exit we if had none
	if (nodeSum == 0)
		return false;
	//get a value from 1 to the sum of these nodes
	double randSum = fRand(1, nodeSum);
	
	//Walk our child nodes back to front, subtracting priorities.
	//Our success state is at nodeSum < randSum
	for (int x = size()-1; x >= 0; x--) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0 && ptrNode->getState() == BT_READY) {
			nodeSum -= (double)(ptrNode->getPriority());
			if (nodeSum < randSum) {
				//ptrNode is the one we are looking for
				return ptrNode == _childPtr;
			}
		}
	}
	
	return false;
}
