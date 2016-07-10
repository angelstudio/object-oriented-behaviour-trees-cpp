#pragma once
#include "BTNode.h"
#include <vector>
/**
	This class extends the default node, and provides functionality for common procedures of child handling.
	
	Destructor must clean up children as well.
*/
class BTGroupNode : public BTNode {
private:
	//Vector holding the child nodes
	std::vector<BTNode*> children;

protected:
	/**
		Ready our children. If we fail at readying our state becomes a error.
	*/
	virtual void handleSuccess() override;
	/**
		Ready our children. If we fail at readying our state becomes a error.
	*/
	virtual void handleFailed() override;
	/**
		Ready our children. If we fail at readying our state becomes a error.
	*/
	virtual void handleError() override;

public:
	/**
		Calls resetState() on all of our children.
	*/
	void resetChildren();
	
	/**
		Gets if all our children are ready or not.
		
		Returns false if any children are not ready.
	*/
	bool childrenReady();
	
	/**
		Attempts to prepare children and current state for next run.
		
		Resets children, checks if they are now ready, and sets state
		accordingly. This method is safe inside handleError() etc,
		is designed as a general group node handler.
	*/
	void readyChildren();
	
	/**
		Adds pointer to a new child, to the children vector.
	*/
	void add(BTNode* _child);

	/**
		Get the number of children currently linked.
	*/
	int size();

	/**
		Get a pointer to the child at the specified index.
	*/
	BTNode* get(int _index);

	/**
		Resets the trees state, from this node, downwards (leaf-first left-right).
	*/
	void resetState() override;
	BTGroupNode();
	
	/**
		This destructor cleans up all children as well.
	*/
	virtual ~BTGroupNode();
};
