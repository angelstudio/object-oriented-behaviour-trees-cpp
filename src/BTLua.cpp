#include "BTLua.h"
#include "link.h"

//define static members
lua_State* BTLua::L = 0;
int BTLua::instanceCount = 0;

BTLua::BTLua() : luaHandleReady(0), luaHandleRunning(0), luaHandleSuccess(0),
		luaHandleFailed(0), luaHandleError(0), luaConstructor(0), luaDestructor(0) {
	//Super is automatically called
	instanceCount++;
	//initialize L for usage if not already
	if (BTLua::L == 0) {
		//load state
		BTLua::L = luaL_newstate();
		//load lua libraries
		luaL_openlibs(L);
		//our (general) lua methods are linked in link.h
		regFunctions(L);
		
		//Create our BT_NODES table, which will be used for storage of node info.
		lua_newtable(L);
		
		//Global table for our pleasure (local storage for nodes)
		lua_setglobal(L, "BT_NODES");
		
		//set the states as globals
		lua_pushnumber(L, (int)BT_READY);
		lua_setglobal(L, "BT_READY");
		lua_pushnumber(L, (int)BT_RUNNING);
		lua_setglobal(L, "BT_RUNNING");
		lua_pushnumber(L, (int)BT_SUCCESS);
		lua_setglobal(L, "BT_SUCCESS");
		lua_pushnumber(L, (int)BT_FAILED);
		lua_setglobal(L, "BT_FAILED");
		lua_pushnumber(L, (int)BT_ERROR);
		lua_setglobal(L, "BT_ERROR");
	}
	//run constructor script
	if (luaConstructor != 0)
		runScript(luaConstructor);
}

BTLua::~BTLua() {
	//Super is automatically called
	//run destructor script
	if (luaDestructor != 0)
		runScript(luaDestructor);
	instanceCount--;
	if (instanceCount <= 0) {
		instanceCount = 0;
		lua_close(L);
		L = 0;
	}
	
	//If we were running large amount of multiple nodes we will need to consider
	//redesigning this.
	delete luaConstructor;
	delete luaDestructor;
	delete luaHandleReady;
	delete luaHandleRunning;
	delete luaHandleSuccess;
	delete luaHandleFailed;
	delete luaHandleError;
	delete luaGetPriority;
}

bool BTLua::runScript(std::string* _luaScript) {
	bool result = false;
	int status = 0;
	if (_luaScript != 0) {
		if (READ_SCRIPTS_FROM_FILE) {
			//should contain a file path
			status = luaL_loadfile(L, _luaScript->c_str());
		} else {
			//should contain a the complete script
			status = luaL_loadstring(L, _luaScript->c_str());
		}
	} else {
		log("ERROR WITH LUA LOADING: NO SCRIPT PASSED");
		return result;
	}
	if (status == LUA_OK) {
		lua_pushinteger(L, (long)(this));
		lua_setglobal(L, "sourceAddress");
		result = lua_pcall(L, 0, LUA_MULTRET, 0) == 0;
	} else {
		log("ERROR WITH LUA LOADING");
	}
	
	return result;
}

void BTLua::handleReady() {
	if (luaHandleReady == 0) {
		BTGroupNode::handleReady();
	} else {
		//run the script in lua
		if (runScript(luaHandleReady)) {
			//success
		} else {
			//set error state
			BTGroupNode::log("Error with script for object "+((long)this));
			setState(BT_ERROR);
		}
	}
}
void BTLua::handleRunning() {
	if (luaHandleRunning == 0) {
		BTGroupNode::handleRunning();
	} else {
		//run the script in lua
		if (runScript(luaHandleRunning)) {
			//success
		} else {
			//set error state
			BTGroupNode::log("Error with script for object "+((long)this));
			setState(BT_ERROR);
		}
	}
}
void BTLua::handleSuccess() {
	if (luaHandleSuccess == 0) {
		BTGroupNode::handleSuccess();
	} else {
		//ready children for next run.
		readyChildren();
		//run the script in lua
		if (runScript(luaHandleSuccess)) {
			//success
		} else {
			//set error state
			BTGroupNode::log("Error with script for object "+((long)this));
			setState(BT_ERROR);
		}
	}
}
void BTLua::handleFailed() {
	if (luaHandleFailed == 0) {
		BTGroupNode::handleFailed();
	} else {
		//ready children for next run.
		readyChildren();
		//run the script in lua
		if (runScript(luaHandleFailed)) {
			//success
		} else {
			//set error state
			BTGroupNode::log("Error with script for object "+((long)this));
			setState(BT_ERROR);
		}
	}
}
void BTLua::handleError() {
	if (luaHandleError == 0) {
		BTGroupNode::handleError();
	} else {
		//ready children for next run.
		readyChildren();
		//run the script in lua
		if (runScript(luaHandleError)) {
			//success
		} else {
			//set error state
			BTGroupNode::log("Error with script for object "+((long)this));
			setState(BT_ERROR);
		}
	}
}

void BTLua::setHandleReady(std::string* _luaScript) {
	luaHandleReady = _luaScript;
}
void BTLua::setHandleRunning(std::string* _luaScript) {
	luaHandleRunning = _luaScript;
}
void BTLua::setHandleSuccess(std::string* _luaScript) {
	luaHandleSuccess = _luaScript;
}
void BTLua::setHandleFailed(std::string* _luaScript) {
	luaHandleFailed = _luaScript;
}
void BTLua::setHandleError(std::string* _luaScript) {
	luaHandleError = _luaScript;
}

void BTLua::setConstructor(std::string* _luaScript) {
	luaConstructor = _luaScript;
}
void BTLua::setDestructor(std::string* _luaScript) {
	luaDestructor = _luaScript;
}

void BTLua::setGetPriority(std::string* _luaScript) {
	luaGetPriority = _luaScript;
}

int BTLua::getPriority() {
	//call the lua script for this
	if (luaGetPriority != 0)
		runScript(luaGetPriority);
	//call super
	return BTGroupNode::getPriority();
}
