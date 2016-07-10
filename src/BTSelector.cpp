#include "BTSelector.h"

BTSelector::BTSelector(): runLimit(0) {
	//Super is automatically called.
}

BTSelector::~BTSelector() {
	//Super is automatically called.
}

bool BTSelector::shouldRunNew() {
	if (runLimit == 0)
		return true;
	//find the number of running nodes
	unsigned int numRunning = 0;
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0) {
			if (ptrNode->getState() == BT_RUNNING)
				numRunning++;
		}
	}
	if (numRunning < runLimit)
		return true;
	return false;
}

void BTSelector::handleReady() {
	//Updated all children ready and able to be run
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		//if we have a real pointer, are ready, and should run
		if (ptrNode != 0 && ptrNode->getState() == BT_READY && shouldRunNew()
			&& getNodeShouldRun(ptrNode)) {
			ptrNode->update();
		}
	}
	//move onto running state
	setState(BT_RUNNING);
}

void BTSelector::handleRunning() {
	/*
		- If any children are running
			- Tick those children
		- Else
			- Find the first state in order error > failed > success(default)
			- set state to the found state
	*/
	//default, continues
	bool ticked = false;
	//default, moves to success
	BTEnumNodeState direState = BT_SUCCESS;
	
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0) {
			BTEnumNodeState nodeState = ptrNode->getState();
			//Tick any running
			if (nodeState == BT_RUNNING) {
				//tick the node(s) that are running, and set flag so we don't exit
				ptrNode->update();
				ticked = true;
			} else if (nodeState == BT_ERROR) {
				//handle setting direState for later usage 
				direState = nodeState;
			} else if (nodeState == BT_FAILED) {
				//we only set the state if not already higher (the only higher is error)
				if (direState != BT_ERROR)
					direState = nodeState;
			}
		}
	}
	
	//Handle what we are doing after this
	if (!ticked)
		setState(direState);
}

void BTSelector::setRunLimit(unsigned int _runLimit) {
	runLimit = _runLimit;
}
