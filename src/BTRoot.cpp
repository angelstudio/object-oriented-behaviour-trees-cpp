#include "BTRoot.h"

BTRoot::BTRoot() {
	//Super is automatically called.
}

BTRoot::~BTRoot() {
	//Super is automatically called.
}

void BTRoot::handleRunning() {
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0)
			ptrNode->update();
	}
	setState(BT_SUCCESS);
}

void BTRoot::handleSuccess() {
	setState(BT_READY);
}

void BTRoot::handleError() {
	setState(BT_READY);
}

void BTRoot::handleFailed() {
	setState(BT_READY);
}