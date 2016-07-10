#include "link.h"
#include "BTNode.h"
#include "BTGroupNode.h"
#include <string>
#include "basictimers.h"

int getPriority(lua_State *L) {
	BTNode* node = (BTNode*)lua_tointeger(L, 1);
	lua_pushinteger(L, (int)node->getPriority());
	return 1;
}

int setPriority(lua_State *L) {
	BTNode* node = (BTNode*)lua_tointeger(L, 1);
	int toSet = static_cast<int>(lua_tointeger(L, 2));
	node->setPriority(toSet);
	return 0;
}

int setState(lua_State *L) {
	BTNode* node = (BTNode*)lua_tointeger(L, 1);
	int toSet = static_cast<int>(lua_tointeger(L, 2));
	node->setState((BTEnumNodeState)toSet);
	return 0;
}

int nodeLog(lua_State *L) {
	std::string text(lua_tostring(L, 1));
	BTNode::log(text);
	return 0;
}

int getState(lua_State *L) {
	BTNode* node = (BTNode*)lua_tointeger(L, 1);
	lua_pushinteger(L, (int)node->getState());
	return 1;
}

int resetState(lua_State *L) {
	BTNode* node = (BTNode*)lua_tointeger(L, 1);
	node->resetState();
	return 0;
}

//BTGroupNode handlers for BTLua
//this lets us control flow in Lua

int group_resetChildren(lua_State *L) {
	BTGroupNode* node = (BTGroupNode*)lua_tointeger(L, 1);
	node->resetChildren();
	return 0;
}
int group_childrenReady(lua_State *L) {
	BTGroupNode* node = (BTGroupNode*)lua_tointeger(L, 1);
	int childrenReadyBool = 0;
	if (node->childrenReady())
		childrenReadyBool = 1;
	lua_pushboolean(L, (int)childrenReadyBool);
	return 1;
}
int group_readyChildren(lua_State *L) {
	BTGroupNode* node = (BTGroupNode*)lua_tointeger(L, 1);
	node->readyChildren();
	return 0;
}
int group_size(lua_State *L) {
	BTGroupNode* node = (BTGroupNode*)lua_tointeger(L, 1);
	lua_pushinteger(L, (int)node->size());
	return 1;
}
int group_child(lua_State *L) {
	BTGroupNode* node = (BTGroupNode*)lua_tointeger(L, 1);
	int index = static_cast<int>(lua_tointeger(L, 2));
	lua_pushinteger(L, (long)node->get(index));
	return 1;
}

/*
	Wrappers for basictimers.h.
*/
int createTimer(lua_State *L) {
	long milli = static_cast<long>(lua_tointeger(L, 1));
	basictimers_timer_id timerId = basictimers_create_timer(milli);
	lua_pushinteger(L, (long)timerId);
	return 1;
}
int resetTimer(lua_State *L) {
	basictimers_timer_id timerId = (basictimers_timer_id)lua_tointeger(L, 1);
	basictimers_reset_timer(timerId);
	return 0;
}
int timerExpired(lua_State *L) {
	basictimers_timer_id timerId = (basictimers_timer_id)lua_tointeger(L, 1);
	//push the timer status
	lua_pushinteger(L, basictimers_timer_expired(timerId));
	return 1;
}
int timerPause(lua_State *L) {
	int milli = static_cast<int>(lua_tointeger(L, 1));
	basictimers_timer_pause(milli);
	return 0;
}
int getCurrentTime(lua_State *L) {
	lua_pushnumber(L, basictimers_get_current_time());
	return 1;
}
int deleteTimer(lua_State *L) {
	basictimers_timer_id timerId = (basictimers_timer_id)lua_tointeger(L, 1);
	basictimers_delete_timer(timerId);
	return 0;
}


void regFunctions(lua_State *L) {
	lua_register(L, "getPriority", getPriority);
	lua_register(L, "setPriority", setPriority);
	lua_register(L, "setState", setState);
	lua_register(L, "getState", getState);
	lua_register(L, "nodeLog", nodeLog);
	//timer declarations
	lua_register(L, "createTimer", createTimer);
	lua_register(L, "resetTimer", resetTimer);
	lua_register(L, "timerExpired", timerExpired);
	lua_register(L, "timerPause", timerPause);
	lua_register(L, "getCurrentTime", getCurrentTime);
	lua_register(L, "deleteTimer", deleteTimer);
	
	//added when we converted to groupnode
	lua_register(L, "resetState", resetState);
	lua_register(L, "group_resetChildren", group_resetChildren);
	lua_register(L, "group_childrenReady", group_childrenReady);
	lua_register(L, "group_readyChildren", group_readyChildren);
	lua_register(L, "group_size", group_size);
	lua_register(L, "group_child", group_child);
}
