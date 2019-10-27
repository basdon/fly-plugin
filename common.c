
/* vim: set filetype=c ts=8 noexpandtab: */

#include "common.h"

int loggedstatus[MAX_PLAYERS];
short kickdelay[MAX_PLAYERS];

void common_tp_player(AMX *amx, int playerid, struct vec4 pos)
{
	natives_NC_SetPlayerPos(amx, playerid, pos.coords);
	nc_params[0] = 2;
	nc_params[1] = playerid;
	*((float*) (nc_params + 2)) = pos.r;
	NC(n_SetPlayerFacingAngle);
	nc_params[0] = 1;
	NC(n_SetCameraBehindPlayer);
}

void common_hide_gametext_for_player(AMX *amx, int playerid)
{
	*buf32 = '_';
	*(buf32 + 1) = 0;
	NC_GameTextForPlayer(playerid, buf32a, 2, 3);
}

int common_find_vehicle_driver(AMX *amx, int vehicleid)
{
	int n = playercount;

	nc_params[0] = 1;
	while (n--) {
		nc_params[1] = players[n];
		NC(n_GetPlayerVehicleID);
		if (nc_result == vehicleid) {
			NC(n_GetPlayerVehicleSeat);
			if (nc_result == 0) {
				return players[n];
			}
		}
	}
	return INVALID_PLAYER_ID;
}

void common_crash_player(AMX *amx, int playerid)
{
	static const char
		*CRASHSTR = "Wasted~~k~SWITCH_DEBUG_CAM_ON~~k~~TOGGLE_DPAD~"
			"~k~~NETWORK_TALK~~k~~SHOW_MOUSE_POINTER_TOGGLE~";

	NC_GameTextForPlayer(playerid, buf144a, 5, 5);
}

void common_NC_Kick(int playerid)
{
	kickdelay[playerid] = 2;
}

int common_is_nan(float f)
{
	unsigned int v = *((unsigned int*) &f);
	return (v | 0x807FFFFF) == 0xFFFFFFFF && (v & 0x007FFFFF);
}