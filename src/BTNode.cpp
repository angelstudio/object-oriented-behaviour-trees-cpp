#include "BTNode.h"
#include <iostream>
#include <fstream>

std::string BTNode::debugFile;

BTNode::BTNode() : state(BT_READY), priority(defaultPriority), debug("") {
}

BTNode::~BTNode() {
}

void BTNode::setState(BTEnumNodeState _state) {
	state = _state;
}
BTEnumNodeState BTNode::getState() {
	return state;
}

int BTNode::getPriority() {
	return priority;
}
void BTNode::setPriority(int _priority) {
	priority = _priority;
}

void BTNode::resetState() {
	//if we were running, we force the error in order to bail.
	//this requires handling errors to be well written.
	if (getState() == BT_RUNNING)
		setState(BT_ERROR);
	
	//Handle all the other 3 states that are not ready
	if (getState() == BT_ERROR)
		handleError();
	else if (getState() == BT_SUCCESS)
		handleSuccess();
	else if (getState() == BT_FAILED)
		handleFailed();
}

void BTNode::handleReady() {
	setState(BT_RUNNING);
}
void BTNode::handleSuccess() {
	setState(BT_READY);
}
void BTNode::handleRunning() {
	setState(BT_SUCCESS);
}
void BTNode::handleFailed() {
	setState(BT_READY);
}
void BTNode::handleError() {
	setState(BT_READY);
}

void BTNode::update() {
	//BTNode::log("Tick of "+std::to_string((long)this));
	//This is essentially just a wrapper for the handle methods.
	if (getState() == BT_READY) {
		handleReady();
	} else if (getState() == BT_RUNNING) {
		handleRunning();
	} else if (getState() == BT_SUCCESS) {
		handleSuccess();
	} else if (getState() == BT_FAILED) {
		handleFailed();
	} else {
		handleError();
	}
}

void BTNode::log(std::string _logString) {
	std::cout << _logString << std::endl;
	if (!debugFile.empty()) {
		//log to the file as well
		std::ofstream log_file(debugFile, std::ios_base::out | std::ios_base::app);
		log_file << _logString << std::endl;
		log_file.close();
	}
}

std::string BTNode::getDebug() {
	return debug;
}
void BTNode::setDebug(std::string _debug) {
	debug = _debug;
}