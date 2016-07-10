#pragma once
#include "BTSelector.h"

/**
	Runs only the node with the highest priority. In the event of
	the same priority, will run the left-most node.
	
	ONLY INCLUDES NODES WHICH ARE READY.
*/
class BTSelectorPriority : public BTSelector {
private:
	/**
		Gets the highest priority from the pool of ready children.
	*/
	int getHighestPriority();
protected:
	/**
		Returns true only for the highest (READY) node.
	*/
	virtual bool getNodeShouldRun(BTNode* _childPtr) override;
public:
	BTSelectorPriority();
	~BTSelectorPriority();
};

