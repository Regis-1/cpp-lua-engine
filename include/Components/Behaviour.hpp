#pragma once

#include <cstring>

struct C_Behaviour {
	char path[64];
	int lua_ref;

	C_Behaviour(const char* path, int luaRef) : lua_ref(luaRef)
	{
		memset(this->path, '\0', 64);
		strcpy_s(this->path, path);
	}
};
