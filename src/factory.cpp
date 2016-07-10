#include "factory.h"
#include "BTGroupNode.h"
#include "BTSequence.h"
#include "BTSequenceBailout.h"
#include "BTLua.h"
#include "BTSelector.h"
#include "BTSelectorPriority.h"
#include "BTSelectorRandom.h"
#include "BTSelectorConcurrent.h"
#include "BTRoot.h"
#include "BTBuffer.h"
#include "BTSequenceSelector.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <cstdlib>

extern "C" {
	#include <string.h>
	#include <sys/types.h>
	#include <sys/stat.h>
}

//START trim lib
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

std::string global_script_directory = "scripts";

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}
// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}
// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}
// trim from start (copying)
static inline std::string ltrimmed(std::string s) {
	ltrim(s);
	return s;
}
// trim from end (copying)
static inline std::string rtrimmed(std::string s) {
	rtrim(s);
	return s;
}
// trim from both ends (copying)
static inline std::string trimmed(std::string s) {
	trim(s);
	return s;
}
//END trim lib

using namespace std;

/**
	Counts the number of tabs a the beginning of the string.
*/
int countTabs(string _input) {
	int result = 0;
	for (unsigned int x = 0; x < _input.length(); x++) {
		if (_input.at(x) != '\t')
			break;
		result++;
	}
	return result;
}

bool validLua(string* _script) {
	int status;
	lua_State *L;
	
	L = luaL_newstate();
	luaL_openlibs(L);
	status = luaL_loadstring(L, _script->c_str());
	
	return status == 0;
}

/**
	Rewrites the given file with the given text.
*/
void truncTextFileString(string _path, string _text) {
	ofstream file;
	if (!_path.empty()) {
		file.open(_path, ios_base::out | ios_base::trunc );
		if (file.is_open()) {
			file << _text.c_str();
		}
	}
}

void updateDebugFile(string _debugPath) {
	try {
		string result = "";
		//Walk the text file, stripping tabs and trimming the string.
		//4 lines per node: address as int, string name, and our fields PRIORITY and STATE
		string line;
		ifstream file (_debugPath.c_str());
		if (file.is_open()) {
			int fields = 0;
			BTNode* nodeAddress;
			while (getline(file, line)) {
				//count the number of tabs and remove them for later
				int tabDepth = countTabs(line);
				line = line.substr(tabDepth, -1);
				string tabPart = "";
				for (int x = 0; x < tabDepth; x++)
					tabPart = tabPart + "\t";
				
				fields++;
				if (fields == 1) {
					result = result + tabPart + line + "\n";
					//get the address
					nodeAddress = 0;
					char* sEnd;
					nodeAddress = dynamic_cast<BTNode*>((BTNode*)strtoul(line.c_str(), &sEnd, 10));
				} else if (fields == 2) {
					//We dont do anything with the node name
					result = result + tabPart + line + "\n";
				} else if (fields == 3) {
					//write priority 
					if (nodeAddress != 0) {
						result = result + tabPart + to_string((int)(nodeAddress->getPriority())) + "\n";
					} else {
						result = result + tabPart + line + "\n";
					}
				} else if (fields == 4) {
					//write state
					if (nodeAddress != 0) {
						result = result + tabPart + to_string((int)(nodeAddress->getState())) + "\n";
					} else {
						result = result + tabPart + line + "\n";
					}
					fields = 0;
				}
			}
			file.close();
			//write out result.
			truncTextFileString(_debugPath, result);
		}
	} catch(int e) {
		//Log we had a problem.
		BTNode::log("We were unable to update the debug file \""+_debugPath+"\", exception occurred.");
	}
}

/**
	Loads and verifies a lua file into a map.
	
	Note that the script is loaded into memory for each node.
	
	If memory becomes a problem this will need to be changed.
	
	BTLua deletes the string on destruction.
*/
string* loadScript(string _filePath) {
	string* result = new string();
	string line;
	ifstream file (_filePath.c_str());
	if (file.is_open()) {
		while (getline(file, line)) {
			*result += line+"\n";
		}
		file.close();
	}
	
	if (!(result->empty()) && validLua(result)) {
		//is valid script
		//do we want to sent the path or the value?
		if (READ_SCRIPTS_FROM_FILE) {
			//We set the value to the file path.
			delete result;
			result = new string(_filePath);
		}
		return result;
	} else {
		BTNode::log("Failed to load "+_filePath+", continuing (node will be added).");
		delete result;
		return 0;
	}
}

/**
	Create a node.
*/
BTNode* createNode(string _nodeName) {
	BTNode* result = 0;
	
	/*
		FORMAT of _nodeName contains a key, and bunch of values to map to a map.
		
		Separate with ;, indicate pairs with :
		Spaces between ; are trimmed
		e.g.
		NodeKey; key1:value1; key2:value2;
	*/
	
	string nodeKey(_nodeName);
	map<string,string> params;
	
	char* tempS = (char*)nodeKey.c_str();
	char * pch = strtok(tempS, ";");
	if (pch != NULL) {
		//the first part of the split
		string firstPart(pch);
		//do the rest (assigning to map)
		pch = strtok(NULL, ";");
		while (pch != NULL) {
			//put it into the map un-separated
			string disLine(pch);
			trim(disLine);
			if (!disLine.empty()) {
				params[disLine] = "";
			}
			pch = strtok(NULL, ";");
		}
		nodeKey = firstPart;
		trim(nodeKey);
	}
	
	map<string,string> tParams;
	
	//For any params we can now safely split the key pairs up by : token
	for (std::map<string,string>::iterator it=params.begin(); it!=params.end(); ++it) {
		tempS = (char*)((it->first).c_str());
		pch = strstr(tempS, ":");
		if (pch != NULL) {
			string val((char*)(pch+sizeof(char)));
			char t = pch[0];
			pch[0] = '\0';
			string key(tempS);
			pch[0] = t;
			
			trim(key);
			trim(val);
			if (!key.empty() && !val.empty())
				tParams[key] = val;
		}
	}
	
	params.clear();
	params = tParams;
	
	if (params.empty()) {
		BTNode::log("Had no parameters for "+nodeKey);
	} else {
		BTNode::log("Had "+to_string(params.size())+" parameters for "+nodeKey);
	/*	for (std::map<string,string>::iterator it=params.begin(); it!=params.end(); ++it)
		BTNode::log(it->first + ":"+it->second);*/
	}
	
	if (nodeKey.empty()) {
		//Does nothing, returns 0
	} else if (nodeKey == "lookup") {
		//Must have a "path" parameter (relative path to load)
		if (params["path"].empty()) {
			BTNode::log("Could not lookup node, no path provided");
		} else {
			BTNode::log("--------------attempting to lookup "+params["path"]+"--------------");
			result = factory(params["path"]);
			BTNode::log("--------------end lookup "+params["path"]+"--------------");
		}
	} else if (nodeKey == "BTSequence") {
		result = new BTSequence();
	} else if (nodeKey == "BTSequenceBailout") {
		result = new BTSequenceBailout();
	} else if (nodeKey == "BTSelectorConcurrent") {
		result = new BTSelectorConcurrent();
	} else if (nodeKey == "BTSelectorPriority") {
		result = new BTSelectorPriority();
	} else if (nodeKey == "BTSelectorRandom") {
		result = new BTSelectorRandom();
	} else if (nodeKey == "BTRoot") {
		result = new BTRoot();
	} else if (nodeKey == "BTBuffer") {
		result = new BTBuffer();
	} else if (nodeKey == "BTSequenceSelector") {
		result = new BTSequenceSelector();
	} else {
		//we are going to attempt to load a BTLua node, using the scripts folder.
		struct stat info;
		string pathname = global_script_directory+"/"+nodeKey+"/";
		if( stat( pathname.c_str(), &info ) != 0 ) {
			//we can't access the folder
		} else if( info.st_mode & S_IFDIR ) {
			// S_ISDIR() doesn't exist on my windows 
			//we have a folder for this script
			//create the node
			BTLua* temp = new BTLua();
			//attempt to load for every script. sets to 0 for each if it failed.
			temp->setHandleReady(loadScript(pathname+"ready.lua"));
			temp->setHandleRunning(loadScript(pathname+"running.lua"));
			temp->setHandleSuccess(loadScript(pathname+"success.lua"));
			temp->setHandleFailed(loadScript(pathname+"failed.lua"));
			temp->setHandleError(loadScript(pathname+"error.lua"));
			temp->setConstructor(loadScript(pathname+"constructor.lua"));
			temp->setDestructor(loadScript(pathname+"destructor.lua"));
			temp->setGetPriority(loadScript(pathname+"getpriority.lua"));
			
			//we pass on the parameters as a table... nested deep: BT_NODES[<pointertothisasint>]['params']
			//get the table BT_NODES into the stack
			lua_getglobal(temp->L, "BT_NODES");
			lua_pushinteger(temp->L, (long)temp);
			lua_newtable(temp->L);
			lua_pushstring(temp->L, "params");
			lua_newtable(temp->L);
			
			//For every parameter we add as rows in the table BT_NODES[<pointertothis>]['params'].
			for (std::map<string,string>::iterator it=params.begin(); it!=params.end(); ++it) {
				lua_pushstring(temp->L, it->first.c_str());
				lua_pushstring(temp->L, it->second.c_str());
				lua_rawset(temp->L, -3);
			}
			//set the table "params" as field of table for this node
			lua_rawset(temp->L, -3);
			//set the table for this node as a field of the global BT_NODES
			lua_rawset(temp->L, -3);
			//Pop it back off the stack
			lua_setglobal(temp->L, "BT_NODES");
			
			result = temp;
		}
	}
	
	//Reserved parameters
	if (result != 0){
		if (!params["priority"].empty()) {
			result->setPriority(atoi(params["priority"].c_str()));
			BTNode::log("Set the priority to "+params["priority"]);
		}
		if (!params["runLimit"].empty()) {
			//runlimit for BTSelector
			((BTSelector*)result)->setRunLimit(atoi(params["runLimit"].c_str()));
			BTNode::log("Set the run limit to "+params["runLimit"]);
		}
	}
	
	//Dereference all pointers in params
	params.clear();
	
	return result;
}

BTNode* factory(string _filePath) {
	string logMessage;
	//the root node
	BTNode* result = 0;
	//The last node we added to the tree
	BTNode* last = 0;
	
	//history of levels
	stack<BTGroupNode*> history;
	
	string line;
	ifstream file (_filePath);
	int level = 0;
	int lineNum = 0;
	if (file.is_open()) {
		while (getline(file, line)) {
			lineNum++;
			//Working logic
			//strip anything after //
			char* tempS = (char*)line.c_str();
			char * pch = strstr(tempS, "//");
			if (pch != NULL) {
				char t = pch[0];
				pch[0] = '\0';
				string stripped(tempS);
				pch[0] = t;
				line = stripped;
			}
			//count the number of tabs at the start.
			int nextLevel = countTabs(line);
			line = line.substr(nextLevel, -1);
			//remove any whitespace
			trim(line);
			if (nextLevel > level) {
				BTNode::log("["+to_string(lineNum)+"] Moving up "+to_string(nextLevel-level)+" levels.");
				//we have advanced a level, set top of stack to last node we made
				for (int x = 0; x < (nextLevel-level); x++)
					history.push((BTGroupNode*)last);
			} else if (nextLevel < level) {
				BTNode::log("["+to_string(lineNum)+"] Moving down "+to_string(level-nextLevel)+" levels.");
				//we have moved down a level, pop
				for (int x = 0; x < (level-nextLevel); x++)
					history.pop();
			}
			if (nextLevel != level)
				level = nextLevel;
			
			//Add a node to the tree.
			//create node first
			BTNode* temp = 0;
			if (!line.empty())
				temp = createNode(line);
			//we only want to add live nodes
			BTNode::log("["+to_string(lineNum)+"] Attempting to create \""+line+"\"");
			if (temp != 0) {
				BTNode::log("["+to_string(lineNum)+"] success");
				last = temp;
				
				//dump node name and parameters for lookup later
				last->setDebug(line);
				
				//if we are the first node...
				if (result == 0) {
					BTNode::log("["+to_string(lineNum)+"] Created root node.");
					result = last;
				} else {
					//we are not the first node.
					//if we don't have a group node to add to we are done, delete the stray node.
					if (history.empty()) {
						BTNode::log("["+to_string(lineNum)+"] Deleted created node because it was on another root. Exiting.");
						delete last;
						break;
					} else {
						BTNode::log("["+to_string(lineNum)+"] Added as child node.");
						//we are still adding.
						history.top()->add(last);
					}
				}
			} else {
				BTNode::log("["+to_string(lineNum)+"] failure");
			}
		}
		file.close();
	} else {
		BTNode::log("Unable to create tree from "+_filePath);
	}
	
	return result;
}

/**
	Recursively dumps node info to a file.
*/
void getDebugForNode(BTNode* _node, string &_output, int _level) {
	//first, we dump the generic things
	//address, nodename, priority, state.
	string tabLevel = "";
	for (int x = 0; x < _level; x++)
		tabLevel += "\t";
	_output += tabLevel + to_string((long)_node) + "\n";
	_output += tabLevel + _node->getDebug() + "\n";
	_output += tabLevel + to_string(_node->getPriority()) + "\n";
	_output += tabLevel + to_string(_node->getState()) + "\n";
	
	//now, find any children
	BTGroupNode* g = dynamic_cast<BTGroupNode*>(_node);
	if (g != NULL) {
		//for every child we do the same
		for (int x = 0; x < g->size(); x++) {
			getDebugForNode(g->get(x), _output, _level+1);
		}
	}
}

void buildDebugFile(BTNode* _root, string _debugPath) {
	string dbg = "";
	//recursively dump the info
	getDebugForNode(_root, dbg, 0);
	//write to file (path, text)
	truncTextFileString(_debugPath, dbg);
}
