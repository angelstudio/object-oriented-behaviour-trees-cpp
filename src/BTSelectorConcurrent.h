#pragma once
#include "BTSelector.h"

/**
	Ticking all children in a single threaded manner, this class provides
	a instance-able but extendible implementation of BTSelector.
*/
class BTSelectorConcurrent : public BTSelector {
protected:
	/**
		Returns true in every case, which will run all (ready) nodes.
	*/
	virtual bool getNodeShouldRun(BTNode* _childPtr) override;
public:
	BTSelectorConcurrent();
	~BTSelectorConcurrent();
};

