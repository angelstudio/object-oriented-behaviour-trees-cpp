#pragma once
#include "BTGroupNode.h"

/**
	Ticks all children in order without consideration of states, all in a single run tick.
	
	If you are using this heavily it is probably an indicator of poorly designed trees.
*/
class BTRoot : public BTGroupNode {
protected:
	/**
		Ticks all children then moves to success.
	*/
	virtual void handleRunning() override;
	
	/**
		Don't do anything to our children.
	*/
	virtual void handleSuccess() override;
	
	/**
		Don't do anything to our children.
	*/
	virtual void handleError() override;
	
	/**
		Don't do anything to our children.
	*/
	virtual void handleFailed() override;
	
public:
	BTRoot();
	~BTRoot();
};
