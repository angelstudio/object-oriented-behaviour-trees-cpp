#include "BTBuffer.h"

BTBuffer::BTBuffer() {
	//Super is automatically called.
}

BTBuffer::~BTBuffer() {
	//Super is automatically called.
}

void BTBuffer::handleRunning() {
	//we didnt run a child at the start
	if (runningChild == -1) {
		setState(BT_SUCCESS);
		return;
	} else if (runningChild != 0) {
		//we are not on the first child! We can run as a normal sequence
		BTSequence::handleRunning();
		return;
	}
	
	//We are running our first child now, if there is one.
	
	//Checks status of running child and act accordingly.
	BTEnumNodeState childState = get(runningChild)->getState();
	if (childState == BT_RUNNING) {
		//still on our first child, update it
		get(runningChild)->update();
	} else {
		//if we exited with success we move on
		if (childState == BT_SUCCESS) {
			setState(BT_SUCCESS);
		} else {
			//we didn't exit with success, run the rest of our nodes until we are done...
			if (runningChild+1 < size()) {
				//we have more to run (more then 1 child)
				//start the next one
				startChild();
				return;
			} else {
				//we are done, no other children, set state to success
				setState(BT_SUCCESS);
			}
		}
	}
}
