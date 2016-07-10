#include "BTSequence.h"

BTSequence::BTSequence() : runningChild(0) {
	//Super is automatically called.
}

BTSequence::~BTSequence() {
	//Super is automatically called.
}

void BTSequence::startChild() {
	//if we are in bounds, have a real pointer, and are ready, run, start, else error.
	if (runningChild+1 < size() && get(runningChild+1) != 0 && get(runningChild+1)->getState() == BT_READY) {
		runningChild++;
		get(runningChild)->update();
	} else {
		//we are exiting with error
		setState(BT_ERROR);
	}
}

void BTSequence::handleReady() {
	setState(BT_RUNNING);
	//set runningChild to the first
	runningChild = -1;
	//if we have children we can start
	if (size() > 0) {
		//if we have a ready child we can start, otherwise we have an error.
		startChild();
	}
}
void BTSequence::handleRunning() {
	//if we are not running a child we can exit here with success.
	if (runningChild == -1) {
		setState(BT_SUCCESS);
		return;
	}
	//Checks status of running child and act accordingly.
	BTEnumNodeState childState = get(runningChild)->getState();
	if (childState == BT_FAILED) {
		//Failed: Propagates.
		setState(childState);
	} else if (childState == BT_SUCCESS) {
		//Success, last child: Propagates success.
		//Success, others: Starts next child.
		if (runningChild == size()-1) {
			setState(childState);
		} else {
			startChild();
		}
	} else if (childState == BT_ERROR) {
		//Error: Propagates.
		setState(childState);
	} else if (childState == BT_RUNNING) {
		//Running: UPDATES CHILD.
		get(runningChild)->update();
	} else {
		//Ready: Shouldn't happen, error is forced.
		setState(BT_ERROR);
	}
}
