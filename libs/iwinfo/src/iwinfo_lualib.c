/*
 * iwinfo - Wireless Information Library - Lua Bindings
 *
 *   Copyright (C) 2009 Jo-Philipp Wich <xm@subsignal.org>
 *
 * The iwinfo library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * The iwinfo library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with the iwinfo library. If not, see http://www.gnu.org/licenses/.
 */

#include "iwinfo_lualib.h"

/* Determine type */
static int iwinfo_L_type(lua_State *L)
{
	const char *ifname = luaL_checkstring(L, 1);

	if( wl_probe(ifname) )
		lua_pushstring(L, "wl");

	else if( madwifi_probe(ifname) )
		lua_pushstring(L, "madwifi");

	else if( wext_probe(ifname) )
		lua_pushstring(L, "wext");

	else
		lua_pushnil(L);

	return 1;
}

/* Wrapper for assoclist */
static int iwinfo_L_assoclist(lua_State *L, int (*func)(const char *, char *, int *))
{
	int i, len;
	char rv[IWINFO_BUFSIZE];
	char macstr[18];
	const char *ifname = luaL_checkstring(L, 1);
	struct iwinfo_assoclist_entry *e;

	lua_newtable(L);
	memset(rv, 0, sizeof(rv));

	if( !(*func)(ifname, rv, &len) )
	{
		for( i = 0; i < len; i += sizeof(struct iwinfo_assoclist_entry) )
		{
			e = (struct iwinfo_assoclist_entry *) &rv[i];

			sprintf(macstr, "%02X:%02X:%02X:%02X:%02X:%02X",
				e->mac[0], e->mac[1], e->mac[2],
				e->mac[3], e->mac[4], e->mac[5]);

			lua_newtable(L);

			lua_pushnumber(L, e->signal);
			lua_setfield(L, -2, "signal");
			
			lua_pushnumber(L, e->noise);
			lua_setfield(L, -2, "noise");

			lua_setfield(L, -2, macstr);
		}
	}

	return 1;
}

/* Broadcom */
LUA_WRAP_INT(wl,channel)
LUA_WRAP_INT(wl,frequency)
LUA_WRAP_INT(wl,bitrate)
LUA_WRAP_INT(wl,signal)
LUA_WRAP_INT(wl,noise)
LUA_WRAP_INT(wl,quality)
LUA_WRAP_INT(wl,quality_max)
LUA_WRAP_INT(wl,mbssid_support)
LUA_WRAP_STRING(wl,mode)
LUA_WRAP_STRING(wl,ssid)
LUA_WRAP_STRING(wl,bssid)
LUA_WRAP_STRING(wl,enctype)
LUA_WRAP_ASSOCLIST(wl)

/* Madwifi */
LUA_WRAP_INT(madwifi,channel)
LUA_WRAP_INT(madwifi,frequency)
LUA_WRAP_INT(madwifi,bitrate)
LUA_WRAP_INT(madwifi,signal)
LUA_WRAP_INT(madwifi,noise)
LUA_WRAP_INT(madwifi,quality)
LUA_WRAP_INT(madwifi,quality_max)
LUA_WRAP_INT(madwifi,mbssid_support)
LUA_WRAP_STRING(madwifi,mode)
LUA_WRAP_STRING(madwifi,ssid)
LUA_WRAP_STRING(madwifi,bssid)
LUA_WRAP_STRING(madwifi,enctype)
LUA_WRAP_ASSOCLIST(madwifi)

/* Wext */
LUA_WRAP_INT(wext,channel)
LUA_WRAP_INT(wext,frequency)
LUA_WRAP_INT(wext,bitrate)
LUA_WRAP_INT(wext,signal)
LUA_WRAP_INT(wext,noise)
LUA_WRAP_INT(wext,quality)
LUA_WRAP_INT(wext,quality_max)
LUA_WRAP_INT(wext,mbssid_support)
LUA_WRAP_STRING(wext,mode)
LUA_WRAP_STRING(wext,ssid)
LUA_WRAP_STRING(wext,bssid)
LUA_WRAP_STRING(wext,enctype)
LUA_WRAP_ASSOCLIST(wext)

/* Broadcom table */
static const luaL_reg R_wl[] = {
	LUA_REG(wl,channel),
	LUA_REG(wl,frequency),
	LUA_REG(wl,bitrate),
	LUA_REG(wl,signal),
	LUA_REG(wl,noise),
	LUA_REG(wl,quality),
	LUA_REG(wl,quality_max),
	LUA_REG(wl,mode),
	LUA_REG(wl,ssid),
	LUA_REG(wl,bssid),
	LUA_REG(wl,enctype),
	LUA_REG(wl,assoclist),
	LUA_REG(wl,mbssid_support),
	{ NULL, NULL }
};

/* Madwifi table */
static const luaL_reg R_madwifi[] = {
	LUA_REG(madwifi,channel),
	LUA_REG(madwifi,frequency),
	LUA_REG(madwifi,bitrate),
	LUA_REG(madwifi,signal),
	LUA_REG(madwifi,noise),
	LUA_REG(madwifi,quality),
	LUA_REG(madwifi,quality_max),
	LUA_REG(madwifi,mode),
	LUA_REG(madwifi,ssid),
	LUA_REG(madwifi,bssid),
	LUA_REG(madwifi,enctype),
	LUA_REG(madwifi,assoclist),
	LUA_REG(madwifi,mbssid_support),
	{ NULL, NULL }
};

/* Wext table */
static const luaL_reg R_wext[] = {
	LUA_REG(wext,channel),
	LUA_REG(wext,frequency),
	LUA_REG(wext,bitrate),
	LUA_REG(wext,signal),
	LUA_REG(wext,noise),
	LUA_REG(wext,quality),
	LUA_REG(wext,quality_max),
	LUA_REG(wext,mode),
	LUA_REG(wext,ssid),
	LUA_REG(wext,bssid),
	LUA_REG(wext,enctype),
	LUA_REG(wext,assoclist),
	LUA_REG(wext,mbssid_support),
	{ NULL, NULL }
};

/* Common */
static const luaL_reg R_common[] = {
	{ "type", iwinfo_L_type },
	{ NULL, NULL }
};


LUALIB_API int luaopen_iwinfo(lua_State *L) {
	luaL_register(L, IWINFO_META, R_common);

	luaL_newmetatable(L, IWINFO_WL_META);
	luaL_register(L, NULL, R_wl);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_setfield(L, -2, "wl");

	luaL_newmetatable(L, IWINFO_MADWIFI_META);
	luaL_register(L, NULL, R_madwifi);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_setfield(L, -2, "madwifi");

	luaL_newmetatable(L, IWINFO_WEXT_META);
	luaL_register(L, NULL, R_wext);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_setfield(L, -2, "wext");

	return 1;
}
