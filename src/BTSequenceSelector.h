#pragma once
#include "BTSequence.h"

/**
	Runs until a child succeeds, propagates failure and error.
*/
class BTSequenceSelector : public BTSequence {
protected:
	/**
		Runs until a child succeeds, propagates failure and error.
	*/
	virtual void handleRunning() override;
public:
	BTSequenceSelector();
	~BTSequenceSelector();
};
