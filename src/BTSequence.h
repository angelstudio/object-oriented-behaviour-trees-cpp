#pragma once
#include "BTGroupNode.h"

/**
	A sequence runs each node in order, failing if a node fails, however does not check 
	prerequisites again until the sequence is completed (in contrast to concurrent sequence).
*/
class BTSequence : public BTGroupNode {
protected:
	int runningChild;
	/**
		Starts the next (runningChild) if it is ready, or sets error state if not.
		Also increments the running child value.
	*/
	void startChild();
	
	/**
		Sets state to running, and starts the first child, set runningChild.
	*/
	virtual void handleReady() override;
	/**
		Checks to see the status of the currently running child, and acts accordingly.
		Failed: Propagates fail.
		Success, last child: Propagates success.
		Success, others: Starts next child.
		Error: Propagates.
		Ready: Shouldn't happen, error is forced.
		
		Running: UPDATES CHILD.
	*/
	virtual void handleRunning() override;

public:
	BTSequence();
	~BTSequence();
};
