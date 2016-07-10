#pragma once
#include "BTSelector.h"

/**
	Randomly selects a child based on their priorities.
	
	To explain, two children with priority of 10 will have
	a 50% chance each, 1 and 4 will have 25% and 75% respectively, etc.
*/
class BTSelectorRandom : public BTSelector {
private:
	static bool haveSetRandomSeed;
protected:
	/**
		Chooses a node at a weighted random position, and returns true for that that.
	*/
	virtual bool getNodeShouldRun(BTNode* _childPtr) override;
public:
	BTSelectorRandom();
	~BTSelectorRandom();
};

