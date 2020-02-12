
/* vim: set filetype=c ts=8 noexpandtab: */

#include "common.h"
#include "kneeboard.h"
#include "missions.h"
#include <string.h>

int kneeboard_ptxt_header[MAX_PLAYERS];
int kneeboard_ptxt_distance[MAX_PLAYERS];
int kneeboard_ptxt_info[MAX_PLAYERS];

static int last_distance[MAX_PLAYERS];

void kneeboard_create_player_text(int playerid)
{
	B144("_");

	nc_params[1] = playerid;

	NC_PARS(4);
	nc_paramf[2] = 525.0f;
	nc_paramf[3] = 240.0f;
	nc_params[4] = buf144a;
	kneeboard_ptxt_distance[playerid] = nc_params[2] =
		NC(n_CreatePlayerTextDraw);
	nc_paramf[3] = 0.25f;
	nc_paramf[4] = 1.0f;
	NC(n_PlayerTextDrawLetterSize);
	nc_paramf[3] = 615.0f;
	nc_paramf[4] = 100.0f;
	NC(n_PlayerTextDrawTextSize);
	NC_PARS(3);
	nc_params[3] = 1;
	NC(n_PlayerTextDrawFont);
	NC(n_PlayerTextDrawSetShadow);
	NC(n_PlayerTextDrawUseBox);
	nc_params[3] = 0x00000077; /*should be same as PANEL_BG*/
	NC(n_PlayerTextDrawBoxColor);

	NC_PARS(4);
	nc_paramf[2] = 525.0f;
	nc_paramf[3] = 250.0f;
	nc_params[4] = buf144a;
	kneeboard_ptxt_info[playerid] = nc_params[2] =
		NC(n_CreatePlayerTextDraw);
	nc_paramf[3] = 0.25f;
	nc_paramf[4] = 1.0f;
	NC(n_PlayerTextDrawLetterSize);
	nc_paramf[3] = 615.0f;
	nc_paramf[4] = 100.0f;
	NC(n_PlayerTextDrawTextSize);
	NC_PARS(3);
	nc_params[3] = 1;
	NC(n_PlayerTextDrawFont);
	NC(n_PlayerTextDrawSetShadow);
	NC(n_PlayerTextDrawUseBox);
	nc_params[3] = 0;
	NC(n_PlayerTextDrawBoxColor);

	B144("~w~Kneeboard");
	NC_PARS(3);
	nc_paramf[2] = 532.0f;
	nc_paramf[3] = 230.0f;
	nc_params[4] = buf144a;
	kneeboard_ptxt_header[playerid] = nc_params[2] =
		NC(n_CreatePlayerTextDraw);
	nc_paramf[3] = 0.5f;
	nc_paramf[4] = 1.7f;
	NC(n_PlayerTextDrawLetterSize);
	NC_PARS(3);
	nc_params[3] = 1;
	NC(n_PlayerTextDrawSetOutline);
	nc_params[3] = 0;
	NC(n_PlayerTextDrawFont);
}

void kneeboard_reset_show(int playerid)
{
	NC_PARS(3);
	nc_params[1] = playerid;
	nc_params[2] = kneeboard_ptxt_info[playerid];
	nc_params[3] = buf144a;
	B144("~w~Not on a job, type ~y~/w~w~ to start working.");
	NC(n_PlayerTextDrawSetString);
	nc_params[2] = kneeboard_ptxt_distance[playerid];
	B144("~n~~n~~n~~n~~n~");
	NC(n_PlayerTextDrawSetString);
	NC_PARS(2);
	nc_params[2] = kneeboard_ptxt_info[playerid];
	NC(n_PlayerTextDrawShow);
	nc_params[2] = kneeboard_ptxt_distance[playerid];
	NC(n_PlayerTextDrawShow);
	nc_params[2] = kneeboard_ptxt_header[playerid];
	NC(n_PlayerTextDrawShow);
}

void kneeboard_update_distance(int playerid, struct vec3 *playerpos)
{
	int dist;

	dist = missions_get_distance_to_next_cp(playerid, playerpos);
	if (dist != -1) {
		if (dist > 1000) {
			dist /= 100;
			dist *= 100;
		}
		if (dist != last_distance[playerid]) {
			last_distance[playerid] = dist;
			NC_PARS(3);
			nc_params[1] = playerid;
			nc_params[2] = kneeboard_ptxt_distance[playerid];
			nc_params[3] = buf4096a;
			if (dist < 1000) {
				csprintf(buf4096,
					"~n~~n~~n~~n~~n~"
					"~w~Distance: %dm~n~",
					dist);
			} else {
				csprintf(buf4096,
					"~n~~n~~n~~n~~n~"
					"~w~Distance: %.1fKm~n~",
					(float) dist / 1000.0f);

			}
			NC(n_PlayerTextDrawSetString);
		}
	}
}