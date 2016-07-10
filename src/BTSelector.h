#pragma once
#include "BTGroupNode.h"

/**
	A selector has two primary functions, first, it must test nodes
	to see if they will be run, and secondly they must run nodes that pass.
	
	In order to better facilitate error handling, state propagation should
	be natural. What this means is that all children nodes will be run until a end
	state is achieved. The end state that is propagated will the first state found,
	in the order of Error, Failed, Success.
	
	A final note: if the state of a selector is forcibly changed, it will call
	readyChildren() as per usual, and attempt to reset child state naturally.
*/
class BTSelector : public BTGroupNode {
private:
	//the total number of nodes that are allowed to run at once, 0 for unlimited.
	unsigned int runLimit;
	/**
		Returns true if we can run new child nodes (runLimit handling).
	*/
	bool shouldRunNew();
protected:
	/**
		Pure virtual method to determine if a (child) node should run or not.
	*/
	virtual bool getNodeShouldRun(BTNode* _childPtr) = 0;
	
	/**
		This method will update any child that is both ready, and returns true from 
		getNodeShouldRun(), then sets state to running.
	*/
	virtual void handleReady() override;
	
	/**
		The main work method, want to tick any still running nodes, until we have completed
		all of them, where we propagate the (most dire) state.
		
		- If any children are running
			- Tick those children
		- Else
			- Find the first state in order error > failed > success(default)
			- set state to the found state
	*/
	virtual void handleRunning() override;
public:
	BTSelector();
	~BTSelector();
	
	/**
		Sets the total number of nodes that can be selected to run at once for this node.
		0 = unlimited, and is default (other selecters may set their own default).
	*/
	void setRunLimit(unsigned int _runLimit);
};

