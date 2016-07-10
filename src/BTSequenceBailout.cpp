#include "BTSequenceBailout.h"

BTSequenceBailout::BTSequenceBailout() {
	//Super is automatically called
}

BTSequenceBailout::~BTSequenceBailout() {
	//super is automatically called
}

void BTSequenceBailout::handleRunning() {
	//if any previous (<= runningChild) node is not running, exit
	bool stopping = false;
	BTEnumNodeState direState = BT_SUCCESS;
	
	//Ticks all children leading up to the and including the last started
	for (int x = 0; x < size() && x <= runningChild; x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0) {
			//Tick this node so we can check the result.
			//Nodes start on running since startChild() ticks them from ready.
			//This is intended behaviour.
			ptrNode->update();
			BTEnumNodeState nodeState = ptrNode->getState();
			//If we are not ready, running, or succeeded, then we bail out.
			if (nodeState != BT_RUNNING && nodeState != BT_SUCCESS && nodeState != BT_READY) {
				//we are stopping
				stopping = true;
				//Log direState
				if (nodeState == BT_ERROR) {
					direState = nodeState;
				} else if (nodeState == BT_FAILED) {
					//we only need to set it to failed if we are not error already
					if (direState != BT_ERROR)
						direState = nodeState;
				}
			}
		}
	}
	
	if (stopping) {
		//Propagate direState
		setState(direState);
	} else {
		//Increment the runningChild if we wont go over our bounds and start
		if (runningChild < size()-1) {
			startChild();
		}
	}
}
