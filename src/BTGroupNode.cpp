#include "BTGroupNode.h"

BTGroupNode::BTGroupNode() {
	//Super BTNode called already.
}

BTGroupNode::~BTGroupNode() {
	//cleanup children
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		delete ptrNode;
	}
	//now all child nodes should be deleted, tada
}

void BTGroupNode::add(BTNode* _child) {
	children.push_back(_child);
}

int BTGroupNode::size() {
	return children.size();
}

BTNode* BTGroupNode::get(int _index) {
	return children[_index];
}

void BTGroupNode::resetChildren() {
	//reset the state of all children
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		ptrNode->resetState();
	}
}

void BTGroupNode::resetState() {
	//reset all children
	resetChildren();
	//reset the state of this node
	BTNode::resetState();
}

bool BTGroupNode::childrenReady() {
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode->getState() != BT_READY)
			return false;
	}
	return true;
}

void BTGroupNode::readyChildren() {
	//reset all of our children
	resetChildren();
	//set our state to ready if children are all ready, otherwise set to error.
	if (childrenReady()) {
		setState(BT_READY);
	} else {
		setState(BT_ERROR);
	}
}

void BTGroupNode::handleSuccess() {
	readyChildren();
}
void BTGroupNode::handleFailed() {
	readyChildren();
}
void BTGroupNode::handleError() {
	readyChildren();
}
