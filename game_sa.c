
/* vim: set filetype=c ts=8 noexpandtab: */

#include "common.h"
#include "game_sa.h"
#include <string.h>

unsigned int vehicleflags[VEHICLE_MODEL_TOTAL];

void game_sa_init()
{
	memset(vehicleflags, 0, sizeof(vehicleflags));
#define SETFLAGS(M,F) vehicleflags[M - 400] = F
	SETFLAGS(MODEL_LEVIATHN, HELI);
	SETFLAGS(MODEL_HUNTER, HELI);
	SETFLAGS(MODEL_SEASPAR, HELI);
	SETFLAGS(MODEL_SKIMMER, PLANE);
	SETFLAGS(MODEL_RCBARON, PLANE);
	SETFLAGS(MODEL_RCRAIDER, HELI);
	SETFLAGS(MODEL_SPARROW, HELI);
	SETFLAGS(MODEL_RUSTLER, PLANE);
	SETFLAGS(MODEL_MAVERICK, HELI);
	SETFLAGS(MODEL_VCNMAV, HELI);
	SETFLAGS(MODEL_POLMAV, HELI);
	SETFLAGS(MODEL_RCGOBLIN, HELI);
	SETFLAGS(MODEL_BEAGLE, PLANE);
	SETFLAGS(MODEL_CROPDUST, PLANE);
	SETFLAGS(MODEL_STUNT, PLANE);
	SETFLAGS(MODEL_SHAMAL, PLANE);
	SETFLAGS(MODEL_HYDRA, PLANE);
	SETFLAGS(MODEL_CARGOBOB, HELI);
	SETFLAGS(MODEL_NEVADA, PLANE);
	SETFLAGS(MODEL_RAINDANC, HELI);
	SETFLAGS(MODEL_AT400, PLANE);
	SETFLAGS(MODEL_ANDROM, PLANE);
	SETFLAGS(MODEL_DODO, PLANE);
}

int game_is_air_vehicle(int model)
{
	return model && (vehicleflags[model - 400] & (HELI | PLANE));
}

int game_is_heli(int model)
{
	return model && (vehicleflags[model - 400] & HELI);
}

int game_is_plane(int model)
{
	return model && (vehicleflags[model - 400] & PLANE);
}

void game_random_carcol(int model, int *col1, int *col2)
{
	char amount;
	short position;

	model -= 400;
	if (0 <= model && model < VEHICLE_MODEL_TOTAL) {
		amount = carcoldata[model].amount;
		position = carcoldata[model].position;
		if (amount > 1) {
			position += NC_random(amount) * 2;
		}
		if (col1 != NULL) {
			*col1 = carcols[position];
		}
		if (col2 != NULL) {
			*col2 = carcols[position + 1];
		}
	}
}
