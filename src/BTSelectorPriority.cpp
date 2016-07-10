#include "BTSelectorPriority.h"


BTSelectorPriority::BTSelectorPriority() {
	//Super is automatically called.
	//set number of concurrent we allow
	setRunLimit(1);
}


BTSelectorPriority::~BTSelectorPriority() {
	//Super is automatically called.
}

//private
int BTSelectorPriority::getHighestPriority() {
	//only consider ready ones
	int result = 0;
	bool first = true;
	for (int x = 0; x < size(); x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0 && ptrNode->getState() == BT_READY) {
			if (first) {
				first = false;
				result = ptrNode->getPriority();
			} else {
				//we compare with the previous (highest) value
				int p = ptrNode->getPriority();
				if (p > result)
					result = p;
			}
		}
	}
	return result;
}

bool BTSelectorPriority::getNodeShouldRun(BTNode* _childPtr) {
	//walk our children to find the first child with the given highest priority
	int highest = getHighestPriority();
	BTNode* lookingFor = 0;
	for (int x = 0; x < size() && lookingFor == 0; x++) {
		BTNode* ptrNode = get(x);
		if (ptrNode != 0 && ptrNode->getState() == BT_READY) {
			if (ptrNode->getPriority() == highest) {
				lookingFor = ptrNode;
				break;
			}
		}
	}
	return _childPtr != 0 && lookingFor == _childPtr;
}