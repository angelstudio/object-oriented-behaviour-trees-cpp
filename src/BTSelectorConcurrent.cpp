#include "BTSelectorConcurrent.h"


BTSelectorConcurrent::BTSelectorConcurrent() {
	//Super is automatically called.
	//set number of concurrent we allow
	setRunLimit(0);//unlimited
}


BTSelectorConcurrent::~BTSelectorConcurrent() {
	//Super is automatically called.
}

bool BTSelectorConcurrent::getNodeShouldRun(BTNode* _childPtr) {
	return true;
}