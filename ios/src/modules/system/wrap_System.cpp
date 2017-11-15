/**
 * Copyright (c) 2006-2016 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

// LOVE
#include "wrap_System.h"
#include "sdl/System.h"

namespace love
{
namespace system
{

#define instance() (Module::getInstance<System>(Module::M_SYSTEM))

int w_getOS(lua_State *L)
{
	luax_pushstring(L, instance()->getOS());
	return 1;
}

int w_getProcessorCount(lua_State *L)
{
	lua_pushinteger(L, instance()->getProcessorCount());
	return 1;
}

int w_setClipboardText(lua_State *L)
{
	const char *text = luaL_checkstring(L, 1);
	instance()->setClipboardText(text);
	return 0;
}

int w_getClipboardText(lua_State *L)
{
	luax_pushstring(L, instance()->getClipboardText());
	return 1;
}

int w_getPowerInfo(lua_State *L)
{
	int seconds = -1, percent = -1;
	const char *str;

	System::PowerState state = instance()->getPowerInfo(seconds, percent);

	if (!System::getConstant(state, str))
		str = "unknown";

	lua_pushstring(L, str);

	if (percent >= 0)
		lua_pushinteger(L, percent);
	else
		lua_pushnil(L);

	if (seconds >= 0)
		lua_pushinteger(L, seconds);
	else
		lua_pushnil(L);

	return 3;
}

int w_openURL(lua_State *L)
{
	std::string url = luax_checkstring(L, 1);
	luax_pushboolean(L, instance()->openURL(url));
	return 1;
}

int w_vibrate(lua_State *L)
{
	double seconds = luaL_optnumber(L, 1, 0.5);
	instance()->vibrate(seconds);
	return 0;
}
	
int w_authenticateLocalPlayer(lua_State *L)
{
	instance()->authenticateLocalPlayer();
	return 0;
}

int w_showAchievements(lua_State *L)
{
	instance()->showAchievements();
	return 0;
}
	
static const luaL_Reg functions[] =
{
	{ "getOS", w_getOS },
	{ "getProcessorCount", w_getProcessorCount },
	{ "setClipboardText", w_setClipboardText },
	{ "getClipboardText", w_getClipboardText },
	{ "getPowerInfo", w_getPowerInfo },
	{ "openURL", w_openURL },
	{ "vibrate", w_vibrate },
	{ "authenticateLocalPlayer", w_authenticateLocalPlayer },
	{ "showAchievements", w_showAchievements },
	{ 0, 0 }
};

extern "C" int luaopen_love_system(lua_State *L)
{
	System *instance = instance();
	if (instance == nullptr)
	{
		instance = new love::system::sdl::System();
	}
	else
		instance->retain();

	WrappedModule w;
	w.module = instance;
	w.name = "system";
	w.type = MODULE_ID;
	w.functions = functions;
	w.types = nullptr;

	return luax_register_module(L, w);
}

} // system
} // love
