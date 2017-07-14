#include <iostream>
#include "lua.hpp"

extern "C"
{
	static int l_cppfunction(lua_State *L)
	{
		double arg = luaL_checknumber(L, 1);
		lua_pushnumber(L, arg * 0.5);
		return 1;
	}
}

using namespace std;

int main(int argc, const char *argv[])
{
	lua_State *L = luaL_newstate();
	cout << "-- load lua standard library --" << endl;
	luaL_openlibs(L);
	cout << "|| load file, but do not execute ||" << endl;
	if (luaL_loadfile(L, "luascript.lua"))
	{
		cerr << "-- load fail --" << endl;
		cerr << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);
	}

	cout << "-- write data(cppvar) from c++ to lua --" << endl;
	lua_pushnumber(L, 1.1);
	lua_setglobal(L, "cppvar");

	cout << "-- start execute lua script --" << endl << endl;
	if (lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		cerr << "-- execute error --" << endl;
		cerr << lua_tostring(L, -1) << endl;
		lua_pop(L, -1);
	}

	cout << "-- read data(luavar) from lua --" << endl;
	lua_getglobal(L, "luavar");
	double luavar = lua_tonumber(L, -1);
	lua_pop(L, 1);
	cout << "|| data read from lua, luavar = " << luavar << " ||" << endl << endl;

	cout << "-- execute function(myluafunction) from lua --" << endl;
	lua_getglobal(L, "myluafunction");
	lua_pushnumber(L, 5);
	lua_pcall(L, 1, 1, 0);
	cout << "|| result is: " << lua_tostring(L, -1) << " ||" << endl << endl;
	lua_pop(L, 1);

	cout << "-- excute function from c++ --" << endl;
	cout << "|| register c++ function in lua first ||" << endl;
	lua_pushcfunction(L, l_cppfunction);
	lua_setglobal(L, "cppfunction");

	cout << "|| execute lua function to call c++ function ||" << endl;
	lua_getglobal(L, "myfunction");
	lua_pushnumber(L, 5);
	lua_pcall(L, 1, 1, 0);
	cout << "|| result is: " << lua_tonumber(L, -1) << " ||" << endl << endl;
	lua_pop(L, 1);

	cout << "-- dispose lua resource --" << endl;
	lua_close(L);
	getc(stdin);
	return 0;
}