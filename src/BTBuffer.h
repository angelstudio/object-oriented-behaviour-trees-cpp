#pragma once
#include "BTGroupNode.h"
#include "BTSequence.h"

/**
	Use to stop propagation of fail states up the tree. This node will always
	run then return success, never BT_ERROR.
	
	This class works similar to a sequence. The first node is the
	primary node, and the main work. This node is run in the first running tick,
	until it returns a end state. If the primary node returns success, then the buffer is 
	finished, but if it is not successful then we will tick the next nodes in order, one
	at a time, until they are all finished, then return success.
*/
class BTBuffer : public BTSequence {
protected:
	/**
		Buffers the state of the first child, and on fail propagates the state
		of the other children as a sequence.
		
		This class works similar to a sequence. The first node is the
		primary node, and the main work. This node is run in the first running tick,
		until it returns a end state. If the primary node returns success, then the buffer is 
		finished, but if it is not successful then we will tick the next nodes in order, one
		at a time, until they are all finished, then return success.
	*/
	virtual void handleRunning() override;
	
public:
	BTBuffer();
	~BTBuffer();
};
