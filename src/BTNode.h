#pragma once
#include <string>

// Enum of possible node states.
enum BTEnumNodeState { BT_READY, BT_SUCCESS, BT_RUNNING, BT_FAILED, BT_ERROR };

/**
	This is the base class of all nodes in the tree.
*/
class BTNode {
private:
	//The default priority for all nodes. Priority is used internally by other nodes.
	static const int defaultPriority = 100;
	//the node's current state.
	BTEnumNodeState state;
	//this node's current priority
	int priority;
	std::string debug;

protected:
	//Default handlers for states (used in update).
	/**
		Sets the state to BT_RUNNING.
		Should be overridden to initiate whatever function this node will perform.
		Rules:
		- Must set the state to BT_RUNNING or BT_ERROR.
		- Can be overridden to check preconditions that may result in (not normal) failure.
	*/
	virtual void handleReady();
	/**
		Sets state to BT_READY.
		Should be overridden to reset anything necessary for the node, and reset to ready.
		Rules:
		- Must set state to BT_READY or BT_ERROR if it cannot setup.
	*/
	virtual void handleSuccess();
	/**
		Sets state to BT_SUCCESS.
		Should be overridden to handle the "work" segment of node.
		Can last a indeterminate number of ticks.
		Rules:
		- Must set the state to BT_SUCCESS, BT_ERROR, or BT_FAILED
		- Perform work, such as handling actions, error checking, etc.
	*/
	virtual void handleRunning();
	/**
		Sets the state to BT_READY.
		Should be overridden to handle a "normal" failure, such as a decorator which
		is designed to bail out when the player is no longer in sight is triggered.
		Rules:
		- Must set the state to BT_ERROR or BT_READY.
	*/
	virtual void handleFailed();
	/**
		Sets the state to BT_READY.
		Should be overridden to handle a "exceptional" failure, which may generally
		include forced bail outs, interruptions, or unexpected errors such as preconditions
		no longer being met and a state reset being required.
		Rules:
		- ONLY other state (then itself) is BT_READY.
	*/
	virtual void handleError();

public:
	static std::string debugFile;
	BTNode();
	virtual ~BTNode();
	
	/**
		Set the current state of the node.
	*/
	void setState(BTEnumNodeState _state);
	
	/**
		Set the current priority of the node.
	*/
	void setPriority(int _priority);
	
	/**
		Run every tick. While we could use a pure virtual, enforced structure
		for consistency, and to better allow bail out using resetState().
	*/
	void update();
	
	/**
		Setup (reset) the state of the tree. May force bail out.
		The aim here is to force ready of the node, in the minimum amount of ticks.
		
		NOTE THAT THIS CAN BE CALLED MULTIPLE TIMES. If there is a operation that must 
		only be performed once, then handle accordingly.
	*/
	virtual void resetState();

	/**
		Get the state of this node.
	*/
	BTEnumNodeState getState();
	
	/**
		Get the priority of this node.
	*/
	virtual int getPriority();
	
	static void log(std::string _logString);
	
	std::string getDebug();
	void setDebug(std::string _debug);
};
