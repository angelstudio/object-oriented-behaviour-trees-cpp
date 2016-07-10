#include "BTSequenceSelector.h"

BTSequenceSelector::BTSequenceSelector() {
	//Super is automatically called
}

BTSequenceSelector::~BTSequenceSelector() {
	//super is automatically called
}

void BTSequenceSelector::handleRunning() {
	//we didnt run a child at the start
	if (runningChild == -1) {
		setState(BT_SUCCESS);
		return;
	}
	BTEnumNodeState childState = get(runningChild)->getState();
	//if we succeeded we bail out of the sequence, otherwise the same as a sequence.
	if (childState == BT_SUCCESS) {
		setState(childState);
	} else {
		BTSequence::handleRunning();
	}
}
