#ifndef __LINK_H__
#define __LINK_H__

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}



void regFunctions(lua_State *L);

#endif