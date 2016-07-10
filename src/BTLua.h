#ifndef __BTLUA_H__
#define __BTLUA_H__

#include "BTNode.h"
#include "BTGroupNode.h"
#include "basictimers.h"
#include <string>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>
}

/**
	1 or 0, if we are 1 then we the string* vars should store file paths,
	otherwise they need to store the entire script.
*/
#define READ_SCRIPTS_FROM_FILE 1

class BTLua : public BTGroupNode {
private:
	static int instanceCount;
	std::string* luaHandleReady;
	std::string* luaHandleRunning;
	std::string* luaHandleSuccess;
	std::string* luaHandleFailed;
	std::string* luaHandleError;
	std::string* luaConstructor;
	std::string* luaDestructor;
	std::string* luaGetPriority;
protected:
	//Override for lua powered methods
	void handleReady() override;
	void handleRunning() override;
	void handleSuccess() override;
	void handleFailed() override;
	void handleError() override;
public:
	//Static lua object that is initialized on construction the first object.
	static lua_State *L;
	
	BTLua();
	~BTLua();
	
	//set the lua script strings for usage (after construction)
	void setHandleReady(std::string* _luaScript);
	void setHandleRunning(std::string* _luaScript);
	void setHandleSuccess(std::string* _luaScript);
	void setHandleFailed(std::string* _luaScript);
	void setHandleError(std::string* _luaScript);
	
	void setConstructor(std::string* _luaScript);
	void setDestructor(std::string* _luaScript);
	
	void setGetPriority(std::string* _luaScript);
	
	int getPriority() override;
	
	/**
		Runs the given lua script in the (set up) lua environment.
	*/
	bool runScript(std::string* _luaScript);
};

#endif
