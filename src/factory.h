#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <string>
#include "BTNode.h"
#include <string>

extern std::string global_script_directory;

/**
	Builds a tree from a simple text based file structure.
*/
BTNode* factory(std::string _filePath);

/**
	Hard casts to BT node, and updates the status using the address.
	
	Theoretically this should work.
*/
void updateDebugFile(std::string _debugPath);

/**
	Builds a debug file by dynamic casting nodes from root, and dumping.
*/
void buildDebugFile(BTNode* _root, std::string _debugPath);

#endif