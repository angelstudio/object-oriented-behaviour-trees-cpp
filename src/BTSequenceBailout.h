#pragma once
#include "BTSequence.h"

class BTSequenceBailout : public BTSequence {
protected:
	/**
		While all nodes <= current are running, if there are more
		to run we will start them.
		In the event we have any previous nodes not running, ready, or success, state
		of the most dire child is propagated (and will be handled next tick).
	*/
	virtual void handleRunning() override;
public:
	BTSequenceBailout();
	~BTSequenceBailout();
};
