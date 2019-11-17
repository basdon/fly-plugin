
/* vim: set filetype=c ts=8 noexpandtab: */

#include "common.h"
#include "airport.h"
#include "anticheat.h"
#include "cmd.h"
#include "game_sa.h"
#include "playerdata.h"
#include "math.h"
#include "missions.h"
#include "money.h"
#include "nav.h"
#include "vehicles.h"
#include <string.h>
#include <time.h>

#define TRACKER_PORT 7766

const static char *SATISFACTION_TEXT_FORMAT = "Passenger~n~Satisfaction: %d%%";

struct MISSIONPOINT {
	unsigned short id;
	float x, y, z;
	unsigned int type;
	char name[MAX_MSP_NAME + 1];
	unsigned short currentlyactivemissions;
	struct AIRPORT *ap;
	struct MISSIONPOINT *next;
};

struct MISSION {
	int id;
	int stage;
	int missiontype;
	struct MISSIONPOINT *startpoint, *endpoint;
	float distance, actualdistanceM;
	int passenger_satisfaction;
	struct dbvehicle *veh;
	/**
	The vehicleid of the vehicle the mission was started with.
	*/
	int vehicleid;
	/**
	Reincarnation value of vehicleid when the mission was started.

	Use in combination with vehicleid to check if vehicle at the end is
	the same vehicle the mission was started with.
	*/
	int vehicle_reincarnation;
	time_t starttime;
	short lastvehiclehp, damagetaken;
	float lastfuel, fuelburned;
	short weatherbonus;
};

static struct MISSION *activemission[MAX_PLAYERS];
/*whether one flightdata data packet with afk flag has been sent already*/
static char tracker_afk_packet_sent[MAX_PLAYERS];
/**
Tracker socket handle.
*/
static int tracker;
/**
Player textdraw handles for passenger satisfaction indicator.
*/
static int ptxt_satisfaction[MAX_PLAYERS];

int missions_is_player_on_mission(int playerid)
{
	return activemission[playerid] != NULL;
}

void missions_create_tracker_socket()
{
	tracker = NC_ssocket_create(SOCKET_UDP);
	if (tracker == SOCKET_INVALID_SOCKET) {
		logprintf("failed to create flighttracker socket");
	} else {
		amx_SetUString(buf144, "127.0.0.1", 144);
		NC_ssocket_connect(tracker, buf144a, TRACKER_PORT);
		*buf32 = 0x04594C46;
		NC_ssocket_send(tracker, buf32a, 4);
	}
}

void missions_destroy_tracker_socket()
{
	if (tracker != SOCKET_INVALID_SOCKET) {
		*buf32 = 0x05594C46;
		NC_ssocket_send(tracker, buf32a, 4);
	}
}

void missions_init()
{
	struct AIRPORT *ap;
	struct MISSIONPOINT *msp;
	int apid, lastapid, i, cacheid, *f = nc_params + 2;

	for (i = 0; i < MAX_PLAYERS; i++) {
		activemission[i] = NULL;
	}

	/*load missionpoints*/
	amx_SetUString(buf144,
		"SELECT a,i,x,y,z,t,name "
		"FROM msp "
		"ORDER BY a ASC,i ASC",
		144);
	cacheid = NC_mysql_query(buf144a);
	i = NC_cache_get_row_count();
	lastapid = -1;
	nc_params[3] = buf144a;
	while (i--) {
		NC_PARS(2);
		nc_params[1] = i;
		apid = (*f = 0, NC(n_cache_get_field_i));
		if (apid != lastapid) {
			lastapid = apid;
			ap = airports + apid;
			msp = malloc(sizeof(struct MISSIONPOINT));
			ap->missionpoints = msp;
		} else {
			msp->next = malloc(sizeof(struct MISSIONPOINT));
			msp = msp->next;
		}
		msp->next = NULL;
		msp->ap = ap;
		msp->currentlyactivemissions = 0;
		msp->id = (unsigned short) (*f = 1, NC(n_cache_get_field_i));
		msp->x = (*f = 2, NCF(n_cache_get_field_f));
		msp->y = (*f = 3, NCF(n_cache_get_field_f));
		msp->z = (*f = 4, NCF(n_cache_get_field_f));
		msp->type = (*f = 5, NC(n_cache_get_field_i));
		NC_PARS(3);
		*f = 6; NC(n_cache_get_field_s);
		amx_GetUString(msp->name, buf32, MAX_MSP_NAME + 1);
		ap->missiontypes |= msp->type;
		ap->num_missionpts++;
	}
	NC_cache_delete(cacheid);

	/*end unfinished dangling flights*/
	amx_SetUString(buf144,
		"UPDATE flg "
		"SET state="EQ(MISSION_STATE_SERVER_ERR)" "
		"WHERE state="EQ(MISSION_STATE_INPROGRESS)"",
		144);
	NC_mysql_tquery_nocb(buf144a);
}

void missions_add_distance(int playerid, float distance_in_m)
{
	if (activemission[playerid] != NULL) {
		activemission[playerid]->actualdistanceM += distance_in_m;
	}
}

/**
Append a pay description and colorized, formatted number to given buffer.

@return amount of characters appended
*/
static
int missions_append_pay(char *buf, char *description, int amount)
{
	char tmp[12], *ptmp;
	int len, p = sprintf(buf, "%s", description);
	if (amount < 0) {
		if (amount < -2147483647) {
			amount = -2147483647;
		}
		amount = -amount;
		buf[p++] = '{'; buf[p++] = 'F'; buf[p++] = '6'; buf[p++] = '0';
		buf[p++] = '0'; buf[p++] = '0'; buf[p++] = '0'; buf[p++] = '}';
		buf[p++] = '-';
	} else {
		if (amount > 2147483647) {
			amount = 2147483647;
		}
		buf[p++] = '{'; buf[p++] = '0'; buf[p++] = '0'; buf[p++] = 'F';
		buf[p++] = '6'; buf[p++] = '0'; buf[p++] = '0'; buf[p++] = '}';
	}
	buf[p++] = '$';
	sprintf(tmp, "%d", amount);
	ptmp = tmp;
	len = strlen(tmp);
	while (len-- > 0) {
		buf[p++] = *(ptmp++);
		if (len % 3 == 0 && len) {
			buf[p++] = ',';
		}
	}
	buf[p++] = '\n';
	return p;
}

void missions_freepoints()
{
	int i = numairports;
	struct AIRPORT *ap = airports;
	struct MISSIONPOINT *msp, *tmp;

	while (i--) {
		msp = ap->missionpoints;
		while (msp != NULL) {
			tmp = msp->next;
			free(msp);
			msp = tmp;
		}
		ap++;
	}
}

/**
@return speed in units/s
*/
static
float missions_get_vehicle_maximum_speed(int model)
{
	/* max horizontal speed is about 81.6 */
	switch (model) {
	case MODEL_DODO: return 140.0f / 270.0f * 81.6f;
	case MODEL_BEAGLE: return 125.0f / 270.0f * 81.6f;
	case MODEL_NEVADA: return 185.0f / 270.0f * 81.6f;
	case MODEL_MAVERICK:
	case MODEL_VCNMAV: return 154.0f / 270.0f * 81.6f;
	case MODEL_RAINDANC: return 113.0f / 270.0f * 81.6f;
	case MODEL_LEVIATHN: return 108.0f / 270.0f * 81.6f;
	case MODEL_POLMAV: return 154.0f / 270.0f * 81.6f;
	case MODEL_SPARROW: return 106.0f / 270.0f * 81.6f;
	case MODEL_HUNTER: return 190.0f / 270.0f * 81.6f;
	case MODEL_CARGOBOB: return 126.0f / 270.0f * 81.6f;
	case MODEL_RUSTLER: return 235.0f / 270.0f * 81.6f;
	case MODEL_SKIMMER: return 135.0f / 270.0f * 81.6f;
	default:
		logprintf("mission_get_vehicle_maximum_speed: "
			"unk model: %d", model);
	case MODEL_SHAMAL:
	case MODEL_HYDRA:
	case MODEL_ANDROM:
	case MODEL_AT400:
		return 81.6f;
	}
}

/**
Gets the pay multiplier for missions done with given vehicle model.
*/
static
float missions_get_vehicle_paymp(int model)
{
	const float heli_mp = 1.18f;

	switch (model) {
	case MODEL_DODO: return 270.0f / 140.0f;
	case MODEL_SHAMAL: return 1.0f;
	case MODEL_BEAGLE: return 270.0f / 125.0f;
	case MODEL_AT400: return 1.15f;
	case MODEL_ANDROM: return 1.1f;
	case MODEL_NEVADA: return 270.0f / 185.0f;
	case MODEL_MAVERICK:
	case MODEL_VCNMAV: return heli_mp * 270.0f / 154.0f;
	case MODEL_RAINDANC: return heli_mp * 270.0f / 113.0f;
	case MODEL_LEVIATHN: return heli_mp * 270.0f / 108.0f;
	case MODEL_POLMAV: return heli_mp * 270.0f / 154.0f;
	case MODEL_SPARROW: return heli_mp * 270.0f / 106.0f;
	case MODEL_HUNTER: return heli_mp * 270.0f / 190.0f;
	case MODEL_CARGOBOB: return heli_mp * 270.0f / 126.0f;
	case MODEL_HYDRA: return 0.9f;
	case MODEL_RUSTLER: return 270.0f / 235.0f;
	case MODEL_SKIMMER: return 270.0f / 135.0f;
	default:
		logprintf("mission_get_vehicle_paymp: unk model: %d", model);
		return 1.0f;
	}
}

/**
Calculates airport tax for given missiontype at given airport.
*/
static
int calculate_airport_tax(struct AIRPORT *ap, int missiontype)
{
	struct MISSIONPOINT *msp;
	struct RUNWAY *rnw;
	int costpermsp, tax = 500, chargernws = 0;

	switch (missiontype) {
	case MISSION_TYPE_PASSENGER_WATER:
		/* skimmer, also for runways (lights maintenance etc) */
		costpermsp = 15;
		chargernws = 1;
		break;
	case MISSION_TYPE_PASSENGER_S:
		/*dodo is excluded from runway cost*/
		costpermsp = 20;
		break;
	case MISSION_TYPE_PASSENGER_M:
		costpermsp = 30;
		chargernws = 1;
		break;
	case MISSION_TYPE_PASSENGER_L:
		costpermsp = 50;
		chargernws = 1;
		break;
	case MISSION_TYPE_CARGO_S:
	case MISSION_TYPE_CARGO_M:
	case MISSION_TYPE_CARGO_L:
		costpermsp = 40;
		chargernws = 1;
		break;
	case MISSION_TYPE_HELI:
	case MISSION_TYPE_HELI_CARGO:
		costpermsp = 30;
		break;
	case MISSION_TYPE_MIL_HELI:
	case MISSION_TYPE_MIL:
	case MISSION_TYPE_AWACS:
	case MISSION_TYPE_STUNT:
	case MISSION_TYPE_CROPD:
	default:
		/* military is govt, and no tax for special missions */
		return 0;
	}

	/*missionpoint cost for every missionpoint this missiontype has*/
	msp = ap->missionpoints;
	while (msp != NULL) {
		if (msp->type & missiontype) {
			tax += costpermsp;
		}
		msp = msp->next;
	}

	/*for helis: 40 per helipad*/
	/*for others: 50 per runway + 15 if VOR + 15 if ILS*/
	rnw = ap->runways;
	while (rnw != ap->runwaysend) {
		if (missiontype & HELI_MISSIONTYPES) {
			if (rnw->type == RUNWAY_TYPE_HELIPAD) {
				tax += 40;
			}
		} else {
			if (chargernws && rnw->type == RUNWAY_TYPE_RUNWAY) {
				tax += 50;
				if (rnw->nav & NAV_VOR) {
					tax += 15;
				}
				if (rnw->nav & NAV_ILS) {
					tax += 15;
				}
			}
		}
		rnw++;
	}
	return tax;
}

#ifdef DEV
/**
@see dev_missions_toggle_closest_point
@see dev_missions_update_closest_point
*/
static int dev_show_closest_point = 0;

void dev_missions_toggle_closest_point()
{
	int i;
	char buf[144];
	if (!(dev_show_closest_point ^= 1)) {
		for (i = 0; i < playercount; i++) {
			NC_DisablePlayerRaceCheckpoint(players[i]);
		}
	}
	sprintf(buf, "showing mission points: %d", dev_show_closest_point);
	amx_SetUString(buf144, buf, sizeof(buf));
	NC_SendClientMessageToAll(-1, buf144a);
}

void dev_missions_update_closest_point()
{
	static struct MISSIONPOINT *dev_closest_point[MAX_PLAYERS];

	const float size = 11.0f;
	int i, playerid;
	struct vec3 ppos;
	float dx, dy, shortestdistance, dist;
	struct AIRPORT *ap, *apend, *closestap;
	struct MISSIONPOINT *mp, *closestmp;

	if (!dev_show_closest_point) {
		return;
	}

	apend = airports + numairports;

	for (i = 0; i < playercount; i++) {
		playerid = players[i];
		common_GetPlayerPos(playerid, &ppos);
		shortestdistance = 0x7F800000;
		closestap = NULL;
		ap = airports;
		while (ap != apend) {
			dx = ap->pos.x - ppos.x;
			dy = ap->pos.y - ppos.y;
			dist = dx * dx + dy * dy;
			if (dist < shortestdistance) {
				shortestdistance = dist;
				closestap = ap;
			}
			ap++;
		}
		if (closestap == NULL) {
			continue;
		}
		shortestdistance = 0x7F800000;
		closestmp = NULL;
		mp = closestap->missionpoints;
		while (mp) {
			dx = mp->x - ppos.x;
			dy = mp->y - ppos.y;
			dist = dx * dx + dy * dy;
			if (dist < shortestdistance) {
				shortestdistance = dist;
				closestmp = mp;
			}
			mp = mp->next;
		}
		if (closestmp && dev_closest_point[playerid] != closestmp) {
			dev_closest_point[playerid] = closestmp;
			NC_PARS(9);
			nc_params[1] = playerid;
			nc_params[2] = 2;
			nc_paramf[3] = nc_paramf[6] = closestmp->x;
			nc_paramf[4] = nc_paramf[7] = closestmp->y;
			nc_paramf[5] = nc_paramf[8] = closestmp->z;
			nc_paramf[9] = size;
			NC(n_SetPlayerRaceCheckpoint);
		}
	}
}
#endif /*DEV*/

/**
Find a random endpoint for given missiontype, skipping airport of startpoint.

@param startpoint may be NULL
@return NULL on no applicable points found
*/
static
struct MISSIONPOINT *missions_get_random_endpoint(
	int missiontype, struct MISSIONPOINT *startpoint)
{
	struct MISSIONPOINT *msp, **possible_missionpts;
	struct AIRPORT *airport, **possible_airports;
	int num_possible_airports, num_possible_missionpts;
	int i, minconcurrent = 1000000;

	if (startpoint == NULL) {
		return NULL;
	}

	i = numairports;
	airport = airports;
	possible_airports = malloc(sizeof(struct AIRPORT*) * numairports);
	num_possible_airports = 0;
	while (i--) {
		if ((airport->missiontypes & missiontype) &&
			airport != startpoint->ap)
		{
			possible_airports[num_possible_airports++] = airport;
		}
		airport++;
	}

	switch (num_possible_airports) {
	case 0: airport = NULL; break;
	case 1: airport = possible_airports[0]; break;
	default:
		airport = possible_airports[NC_random(num_possible_airports)];
		break;
	}

	free(possible_airports);
	if (airport == NULL) {
		return NULL;
	}

	/*got a random airport, now mission points*/
	msp = airport->missionpoints;
	possible_missionpts = malloc(sizeof(int*) * airport->num_missionpts);
	num_possible_missionpts = 0;
	while (msp != NULL) {
		if (msp->type & missiontype) {
			if (msp->currentlyactivemissions < minconcurrent) {
				minconcurrent = msp->currentlyactivemissions;
				num_possible_missionpts = 0;
			}
			possible_missionpts[num_possible_missionpts++] = msp;
		}
		msp = msp->next;
	}

	switch (num_possible_missionpts) {
	case 0:
		/*should not happen, since only airports that has the wanted
		missiontype have been selected*/
		msp = NULL;
		break;
	case 1:
		msp = possible_missionpts[0];
		break;
	default:
		msp = possible_missionpts[NC_random(num_possible_missionpts)];
		break;
	}
	free(possible_missionpts);
	return msp;
}

/**
Find a random startpoint for given missiontype.

Startpoint is chosen based on least amount of currently active missions, then
distance.

@return NULL on no applicable points found
*/
static
struct MISSIONPOINT *missions_get_startpoint(int missiontype, struct vec3 *ppos)
{
	struct AIRPORT *airport;
	struct MISSIONPOINT *msp, **free_missionpts;
	float dx, dy, dz, dist, shortest_distance;
	int i, num_free_missionpts, least_active;

	shortest_distance = FLOAT_PINF;
	airport = NULL;
	i = numairports;
	while (i--) {
		if (airports[i].missiontypes & missiontype) {
			dx = airports[i].pos.x - ppos->x;
			dy = airports[i].pos.y - ppos->y;
			dz = airports[i].pos.z - ppos->z;
			dist = dx * dx + dy * dy + dz * dz;
			if (dist < shortest_distance) {
				shortest_distance = dist;
				airport = airports + i;
			}
		}
	}

	if (airport == NULL) {
		return NULL;
	}

	/*first select mission points with least amount of active missions*/
	free_missionpts = malloc(sizeof(int*) * airport->num_missionpts);
	num_free_missionpts = 0;
	least_active = 1000000;
	msp = airport->missionpoints;
	while (msp != NULL) {
		if (msp->type & missiontype) {
			if (msp->currentlyactivemissions < least_active) {
				least_active = msp->currentlyactivemissions;
				num_free_missionpts = 0;
			}
			free_missionpts[num_free_missionpts++] = msp;
		}
		msp = msp->next;
	}

	/*then choose the missionpoint that is the closest to the player*/
	shortest_distance = FLOAT_PINF;
	while (num_free_missionpts--) {
		dx = free_missionpts[num_free_missionpts]->x - ppos->x;
		dy = free_missionpts[num_free_missionpts]->y - ppos->y;
		dz = free_missionpts[num_free_missionpts]->z - ppos->z;
		dist = dx * dx + dy * dy + dz * dz;
		if (dist < shortest_distance) {
			shortest_distance = dist;
			msp = free_missionpts[num_free_missionpts];
		}
	}

	free(free_missionpts);
	return msp;
}

void missions_cb_create(void* data)
{
	static const char *AUTOWORKNOTICE = "Constant work is ON, "
		"a new mission will be started when you complete this one "
		"(/autow to disable).";

	struct MISSION *mission;
	unsigned int cc = (int) data;
	int playerid;

	playerid = PLAYER_CC_GETID(cc);
	if (!PLAYER_CC_CHECK(cc, playerid) ||
		(mission = activemission[playerid]) == NULL)
	{
		return;
	}

	common_hide_gametext_for_player(playerid);
	tracker_afk_packet_sent[playerid] = 0;

	mission->id = NC_cache_insert_id();
	mission->stage = MISSION_STAGE_PRELOAD;

	NC_PARS(9);
	nc_params[1] = playerid;
	nc_params[2] = 2;
	nc_paramf[3] = mission->startpoint->x;
	nc_paramf[4] = mission->startpoint->y;
	nc_paramf[5] = mission->startpoint->z;
	nc_paramf[6] = nc_paramf[7] = nc_paramf[8] = 0.0f;
	nc_paramf[9] = MISSION_CHECKPOINT_SIZE;
	NC(n_SetPlayerRaceCheckpoint);

	sprintf(cbuf4096,
		"Flight from %s (%s) %s to %s (%s) %s",
		mission->startpoint->ap->name,
		mission->startpoint->ap->beacon,
		mission->startpoint->name,
		mission->endpoint->ap->name,
		mission->endpoint->ap->beacon,
		mission->endpoint->name);
	amx_SetUString(buf144, cbuf4096, 144);
	NC_SendClientMessage(playerid, COL_MISSION, buf144a);

	if (prefs[playerid] & PREF_CONSTANT_WORK) {
		amx_SetUString(buf144, AUTOWORKNOTICE, 144);
		NC_SendClientMessage(playerid, COL_SAMP_GREY, buf144a);
	}

	if (prefs[playerid] & PREF_WORK_AUTONAV) {
		nav_navigate_to_airport(
			mission->veh->spawnedvehicleid,
			mission->veh->model,
			mission->startpoint->ap);
	}

	/* flight tracker packet 1 */
	buf32[0] = 0x01594C46;
	buf32[1] = mission->id;
	*((float*) (cbuf32 + 8)) = model_fuel_capacity(mission->veh->model);
	*((short*) (cbuf32 + 12)) = (short) mission->veh->model;
	cbuf32[14] = pdata[playerid]->namelen;
	memset(cbuf32 + 15, 0, 24); /* don't leak random data */
	strcpy(cbuf32 + 15, pdata[playerid]->name);
	/*buf32 is len 32 * 4 so 40 is fine*/
	NC_ssocket_send(tracker, buf32a, 40);
}

/**
Cleanup a mission and free memory. Does not query. Does send a tracker msg.

Call when ending a mission.

@param mission mission of the player. Must match with given playerid.
*/
static
void missions_cleanup(struct MISSION *mission, int playerid)
{
	/* flight tracker packet 3 */
	buf32[0] = 0x03594C46;
	buf32[1] = mission->id;
	NC_ssocket_send(tracker, buf32a, 8);

	mission->startpoint->currentlyactivemissions--;
	mission->endpoint->currentlyactivemissions--;
	if (mission->missiontype & PASSENGER_MISSIONTYPES) {
		NC_PlayerTextDrawHide(playerid, ptxt_satisfaction[playerid]);
	}

	free(mission);
	activemission[playerid] = NULL;
}

/**
@param reason one of MISSION_STATE_ constants
*/
static
void missions_end_unfinished(struct MISSION *mission, int playerid, int reason)
{
	sprintf(cbuf144,
	        "UPDATE flg "
		"SET state=%d,tlastupdate=UNIX_TIMESTAMP(),adistance=%f "
		"WHERE id=%d",
	        reason,
		mission->actualdistanceM,
	        mission->id);
	amx_SetUString(buf4096, cbuf144, 144);
	NC_mysql_tquery_nocb(buf4096a);

	missions_cleanup(mission, playerid);
}

void missions_on_player_connect(int playerid)
{
	NC_PARS(4);
	nc_params[1] = playerid;
	nc_paramf[2] = 88.0f;
	nc_paramf[3] = 425.0f;
	nc_params[4] = underscorestringa;
	nc_params[2] = NC(n_CreatePlayerTextDraw);
	ptxt_satisfaction[playerid] = nc_params[2];
	nc_paramf[3] = 0.3f;
	nc_paramf[4] = 1.0f;
	NC(n_PlayerTextDrawLetterSize);

	NC_PARS(3);
	nc_params[3] = 255;
	NC(n_PlayerTextDrawBackgroundColor);
	nc_params[3] = 2;
	NC(n_PlayerTextDrawAlignment);
	nc_params[3] = 1;
	NC(n_PlayerTextDrawFont);
	NC(n_PlayerTextDrawSetProportional);
	NC(n_PlayerTextDrawSetOutline);
	nc_params[3] = -1;
	NC(n_PlayerTextDrawColor);
}

void missions_on_player_death(int playerid)
{
	struct MISSION *mission;
	int stopreason, vehicleid;
	float hp;

	if ((mission = activemission[playerid]) != NULL) {
		NC_DisablePlayerRaceCheckpoint(playerid);
		stopreason = MISSION_STATE_DIED;
		vehicleid = NC_GetPlayerVehicleID(playerid);
		if (vehicleid) {
			hp = anticheat_NC_GetVehicleHealth(vehicleid);
			if (hp <= 200.0f) {
				stopreason = MISSION_STATE_CRASHED;
			}
		}
		missions_end_unfinished(mission, playerid, stopreason);
	}
}

void missions_on_player_disconnect(int playerid)
{
	struct MISSION *miss;

	if ((miss = activemission[playerid]) != NULL) {
		missions_end_unfinished(
			miss, playerid, MISSION_STATE_ABANDONED);
	}
}

void missions_on_weather_changed(int weather)
{
	int bonusvalue, i;

	switch (weather) {
	case WEATHER_SF_RAINY:
	case WEATHER_CS_RAINY:
		bonusvalue = MISSION_WEATHERBONUS_RAINY;
		break;
	case WEATHER_SF_FOGGY:
	case WEATHER_UNDERWATER:
		bonusvalue = MISSION_WEATHERBONUS_FOGGY;
		break;
	case WEATHER_DE_SANDSTORMS:
		bonusvalue = MISSION_WEATHERBONUS_SANDSTORM;
		break;
	default:
		return;
	}
	bonusvalue += NC_random(MISSION_WEATHERBONUS_DEVIATION);
	i = playercount;
	while (i--) {
		if (activemission[players[i]] != NULL) {
			activemission[players[i]]->weatherbonus += bonusvalue;
		}
	}
}

void missions_send_tracker_data(
	int playerid, int vehicleid, float hp,
	struct vec3 *vpos, struct vec3 *vvel, int afk, int engine)
{
	struct MISSION *mission;
	unsigned char flags;
	short spd, alt, hpv;

	if ((mission = activemission[playerid]) == NULL ||
		mission->veh->spawnedvehicleid != vehicleid ||
		(afk && tracker_afk_packet_sent[playerid]))
	{
		return;
	}

	tracker_afk_packet_sent[playerid] = flags = afk == 1;
	if (engine) {
		flags |= 2;
	}

	hpv = (short) hp;
	alt = (short) vpos->z;
	spd = (short) (VEL_TO_KTS_VAL * sqrt(
		vvel->x * vvel->x + vvel->y * vvel->y + vvel->z * vvel->z));

	/* flight tracker packet 2 */
	buf32[0] = 0x02594C46;
	buf32[1] = mission->id;
	cbuf32[8] = flags;
	cbuf32[9] = (char) mission->passenger_satisfaction;
	memcpy(cbuf32 + 10, &spd, 2);
	memcpy(cbuf32 + 12, &alt, 2);
	memcpy(cbuf32 + 14, &hpv, 2);
	memcpy(cbuf32 + 16, &mission->veh->fuel, 4);
	memcpy(cbuf32 + 20, &vpos->x, 4);
	memcpy(cbuf32 + 24, &vpos->y, 4);
	NC_ssocket_send(tracker, buf32a, 28);
}

int missions_cmd_automission(CMDPARAMS)
{
	static const char
		*ENABLED = "Constant work enabled.",
		*DISABLED = "Constant work disabled.";

	if ((prefs[playerid] ^= PREF_CONSTANT_WORK) & PREF_CONSTANT_WORK) {
		amx_SetUString(buf144, ENABLED, 144);
	} else {
		amx_SetUString(buf144, DISABLED, 144);
	}
	NC_SendClientMessage(playerid, COL_SAMP_GREY, buf144a);
	return 1;
}

int missions_cmd_cancelmission(CMDPARAMS)
{
	static const char *NOMISSION = WARN"You're not on an active mission.";

	struct MISSION *mission;

	if ((mission = activemission[playerid]) != NULL) {
		NC_DisablePlayerRaceCheckpoint(playerid);
		money_take(playerid, 5000);
		missions_end_unfinished(mission,
			playerid, MISSION_STATE_DECLINED);
	} else {
		amx_SetUString(buf144, NOMISSION, 144);
		NC_SendClientMessage(playerid, COL_WARN, buf144a);
	}
	return 1;
}

static const char
	*RETRIEV = "~b~Retrieving flight data...",
	*NOTYPES = WARN"This vehicle can't complete any type of missions!",
	*NOMISS = WARN"There are no missions available for this type "
		"of vehicle!",
	*ALREADYWORKING = WARN"You're already working! Use /s to stop "
		"your current work first ($"Q(MISSION_CANCEL_FINE_)" fine).",
	*MUSTBEDRIVER = WARN"You must be the driver of a vehicle "
		"before starting work!";

int missions_cmd_mission(CMDPARAMS)
{
	struct MISSION *mission;
	struct MISSIONPOINT *startpoint, *endpoint;
	struct dbvehicle *veh;
	struct vec3 ppos;
	char *errmsg, tmpuseridornullbuf[10];
	int vehicleid, missiontype;
	float vhp, dx, dy;

	if (activemission[playerid] != NULL) {
		amx_SetUString(buf144, ALREADYWORKING, 144);
		NC_SendClientMessage(playerid, COL_WARN, buf144a);
		return 1;
	}

	NC_PARS(1);
	nc_params[1] = playerid;
	if (NC(n_GetPlayerVehicleSeat) != 0) {
		errmsg = (char*) MUSTBEDRIVER;
		goto err;
	}
	vehicleid = NC(n_GetPlayerVehicleID);

	if ((veh = gamevehicles[vehicleid].dbvehicle) == NULL) {
		goto unknownvehicle;
	}

	switch (veh->model) {
	case MODEL_DODO: missiontype = MISSION_TYPE_PASSENGER_S; break;
	case MODEL_SHAMAL:
	case MODEL_BEAGLE: missiontype = MISSION_TYPE_PASSENGER_M; break;
	case MODEL_AT400:
	case MODEL_ANDROM: missiontype = MISSION_TYPE_PASSENGER_L; break;
	case MODEL_NEVADA: missiontype = MISSION_TYPE_CARGO_M; break;
	case MODEL_MAVERICK:
	case MODEL_VCNMAV:
	case MODEL_RAINDANC:
	case MODEL_LEVIATHN:
	case MODEL_POLMAV:
	case MODEL_SPARROW: missiontype = MISSION_TYPE_HELI; break;
	case MODEL_HUNTER:
	case MODEL_CARGOBOB: missiontype = MISSION_TYPE_MIL_HELI; break;
	case MODEL_HYDRA:
	case MODEL_RUSTLER: missiontype = MISSION_TYPE_MIL; break;
	case MODEL_SKIMMER: missiontype = MISSION_TYPE_PASSENGER_WATER; break;
	default:
unknownvehicle:
		errmsg = (char*) NOTYPES;
		goto err;
	}

	common_GetPlayerPos(playerid, &ppos);

	startpoint = missions_get_startpoint(missiontype, &ppos);
	endpoint = missions_get_random_endpoint(missiontype, startpoint);
	/*endpoint will also be NULL when startpoint is NULL*/
	if (endpoint == NULL) {
		errmsg = (char*) NOMISS;
		goto err;
	}

	vhp = anticheat_NC_GetVehicleHealth(vehicleid);

	dx = startpoint->x - endpoint->x;
	dy = startpoint->y - endpoint->y;

	activemission[playerid] = mission = malloc(sizeof(struct MISSION));
	mission->id = -1;
	mission->stage = MISSION_STAGE_CREATE;
	mission->missiontype = missiontype;
	mission->startpoint = startpoint;
	mission->endpoint = endpoint;
	mission->distance = sqrt(dx * dx + dy * dy);
	mission->actualdistanceM = 0.0f;
	mission->passenger_satisfaction = 100;
	mission->veh = veh;
	mission->vehicleid = vehicleid;
	mission->vehicle_reincarnation = gamevehicles[vehicleid].reincarnation;
	mission->starttime = time(NULL);
	mission->lastvehiclehp = (short) vhp;
	mission->damagetaken = 0;
	mission->lastfuel = veh->fuel;
	mission->fuelburned = 0.0f;
	mission->weatherbonus = 0;

	startpoint->currentlyactivemissions++;
	endpoint->currentlyactivemissions++;

	sprintf(cbuf144, "UPDATE msp SET o=o+1 WHERE i=%d", startpoint->id);
	amx_SetUString(buf4096, cbuf144, 144);
	NC_mysql_tquery_nocb(buf4096a);
	sprintf(cbuf144 + 100, "UPDATE msp SET p=p+1 WHERE i=%d", endpoint->id);
	amx_SetUString(buf4096, cbuf144, 144);
	NC_mysql_tquery_nocb(buf4096a);

	useridornull(playerid, tmpuseridornullbuf);
	sprintf(cbuf4096 + 2000,
	        "INSERT INTO flg(player,vehicle,missiontype,fapt,tapt,fmsp,"
		"tmsp,distance,tstart,tlastupdate) "
		"VALUES(%s,%d,%d,%d,%d,%d,%d,%.4f,"
		"UNIX_TIMESTAMP(),UNIX_TIMESTAMP())",
	        tmpuseridornullbuf,
		veh->id,
		missiontype,
		startpoint->ap->id,
		endpoint->ap->id,
		startpoint->id,
		endpoint->id,
		mission->distance);
	common_mysql_tquery(cbuf4096 + 2000,
		missions_cb_create, (void*) MK_PLAYER_CC(playerid));

	amx_SetUString(buf144, RETRIEV, 144);
	NC_GameTextForPlayer(playerid, buf144a, 0x800000, 3);
	return 1;
err:
	amx_SetUString(buf144, errmsg, 144);
	NC_SendClientMessage(playerid, COL_WARN, buf144a);
	return 1;
}

/* native Missions_EndUnfinished(playerid, reason) */
cell AMX_NATIVE_CALL Missions_EndUnfinished(AMX *amx, cell *params)
{
	return 1;
}

/* native Missions_EnterCheckpoint(playerid, vehicleid, vv, x, y, z, errmsg[]) */
cell AMX_NATIVE_CALL Missions_EnterCheckpoint(AMX *amx, cell *params)
{
	const int playerid = params[1], vehicleid = params[2], vv = params[3];
	float x, y, z;
	struct MISSION *mission;
	struct dbvehicle *veh;
	cell *addr;
	char msg[144];

	if ((mission = activemission[playerid]) == NULL) {
		return 0;
	}

	if (vehicleid != mission->vehicleid ||
		vv != mission->vehicle_reincarnation ||
		(veh = gamevehicles[vehicleid].dbvehicle) == NULL ||
		veh->id != mission->veh->id)
	{
		strcpy(msg, WARN"You must be in the starting vehicle to continue!");
		goto exit_set_errmsg;
	}

	x = amx_ctof(params[4]);
	y = amx_ctof(params[5]);
	z = amx_ctof(params[6]);

#if VEL_VER != 2
#error "recalc this"
#endif
	if (x * x + y * y + z * z > /*(35/VEL_TO_KTS_VAL)^2*/0.1307962) {
		strcpy(msg, WARN"You need to slow down to load/unload! Re-enter the checkpoint.");
		goto exit_set_errmsg;
	}

	switch (mission->stage) {
	case MISSION_STAGE_PRELOAD:
		mission->stage = MISSION_STAGE_LOAD;
		return MISSION_ENTERCHECKPOINTRES_LOAD;
	case MISSION_STAGE_FLIGHT:
		mission->stage = MISSION_STAGE_UNLOAD;
		NC_PlayerTextDrawHide(playerid, ptxt_satisfaction[playerid]);
		return MISSION_ENTERCHECKPOINTRES_UNLOAD;
	default:
		logprintf("ERR: player uid %d entered mission checkpoint in invalid stage: %d",
		          pdata[playerid] == NULL ? -1 : pdata[playerid]->userid,
		          mission->stage);
		return 0;
	}

exit_set_errmsg:
	amx_GetAddr(amx, params[7], &addr);
	amx_SetUString(addr, msg, sizeof(msg));
	return MISSION_ENTERCHECKPOINTRES_ERR;
}

int missions_get_stage(int playerid)
{
	struct MISSION *mission = activemission[playerid];
	if (mission != NULL) {
		return mission->stage;
	}
	return MISSION_STAGE_NOMISSION;
}

/* native Missions_GetState(playerid) */
cell AMX_NATIVE_CALL Missions_GetState(AMX *amx, cell *params)
{
	return missions_get_stage(params[1]);
}

/* native Missions_OnVehicleRefueled(playerid, vehicleid, Float:refuelamount) */
cell AMX_NATIVE_CALL Missions_OnVehicleRefueled(AMX *amx, cell *params)
{
	struct MISSION *miss;
	const int playerid = params[1], vehicleid = params[2];
	const float refuelamount = amx_ctof(params[3]);

	if ((miss = activemission[playerid]) != NULL &&
		miss->veh->spawnedvehicleid == vehicleid)
	{
		miss->fuelburned += refuelamount;
		miss->lastfuel = miss->veh->fuel;
	}
	return 1;
}

/* native Missions_OnVehicleRepaired(playerid, vehicleid, Float:oldhp, Float:newhp) */
cell AMX_NATIVE_CALL Missions_OnVehicleRepaired(AMX *amx, cell *params)
{
	struct MISSION *miss;
	const int playerid = params[1], vehicleid = params[2];
	const float newhp = amx_ctof(params[4]), hpdiff = newhp - amx_ctof(params[3]);

	if ((miss = activemission[playerid]) != NULL &&
		miss->veh->spawnedvehicleid == vehicleid)
	{
		miss->damagetaken += (short) hpdiff;
		miss->lastvehiclehp = (short) newhp;
	}
	return 1;
}

/* native Missions_PostLoad(playerid, &Float:x, &Float:y, &Float:z buf[]) */
cell AMX_NATIVE_CALL Missions_PostLoad(AMX *amx, cell *params)
{
	const int playerid = params[1];
	struct MISSION *mission;
	cell *addr;
	char buf[144];

	if ((mission = activemission[playerid]) == NULL ||
		mission->stage != MISSION_STAGE_LOAD)
	{
		return 0;
	}

	if (prefs[playerid] & PREF_WORK_AUTONAV) {
		nav_navigate_to_airport(
			mission->veh->spawnedvehicleid,
			mission->veh->model,
			mission->endpoint->ap);
	}

	if (mission->missiontype & PASSENGER_MISSIONTYPES) {
		sprintf(cbuf32, SATISFACTION_TEXT_FORMAT, 100);
		amx_SetUString(buf32_1, cbuf32, 32);
		NC_PARS(3);
		nc_params[1] = playerid;
		nc_params[2] = ptxt_satisfaction[playerid];
		nc_params[3] = buf32_1a;
		NC(n_PlayerTextDrawSetString);
		NC_PARS(2);
		NC(n_PlayerTextDrawShow);
	}

	mission->stage = MISSION_STAGE_FLIGHT;
	sprintf(buf,
	        "UPDATE flg "
		"SET tload=UNIX_TIMESTAMP(),tlastupdate=UNIX_TIMESTAMP() "
		"WHERE id=%d",
	        mission->id);
	amx_GetAddr(amx, params[2], &addr);
	*addr = amx_ftoc(mission->endpoint->x);
	amx_GetAddr(amx, params[3], &addr);
	*addr = amx_ftoc(mission->endpoint->y);
	amx_GetAddr(amx, params[4], &addr);
	*addr = amx_ftoc(mission->endpoint->z);
	amx_GetAddr(amx, params[5], &addr);
	amx_SetUString(addr, buf, sizeof(buf));
	return 1;
}

/* native Missions_PostUnload(playerid, Float:vehiclehp, &pay, buf[]) */
cell AMX_NATIVE_CALL Missions_PostUnload(AMX *amx, cell *params)
{
	struct MISSION *mission;
	const int playerid = params[1];
	const float vehiclehp = amx_ctof(params[2]);
	float paymp;
	cell *addr;
	char buf[4096];
	int ptax, psatisfaction = 0, pdistance, pbonus = 0, ptotal, pdamage, pcheat = 0, tmp;
	int totaltime, mintime, duration_h, duration_m;
	int p;

	if ((mission = activemission[playerid]) == NULL || pdata[playerid] == NULL) {
		return 0;
	}

	amx_GetAddr(amx, params[4], &addr);

	tmp = (int) (mission->lastvehiclehp - vehiclehp);
	if (tmp < 0) {
		tmp = 0;
		pcheat -= 250000;
		sprintf(buf, "flg(#%d) vhh: was: %hd now: %.0f", mission->id, mission->lastvehiclehp, vehiclehp);
		amx_SetUString(addr + 2100, buf, sizeof(buf));
	} else {
		*(addr + 2100) = 0; /* ac log hp cheat */
	}
	mission->damagetaken += tmp;
	mission->lastvehiclehp = (short) vehiclehp;
	mission->fuelburned += mission->lastfuel - mission->veh->fuel;

	totaltime = (int) difftime(time(NULL), mission->starttime);
	duration_m = totaltime % 60;
	duration_h = (totaltime - duration_m) / 60;

	/* don't use adistance because it also includes z changes */
	mintime = (int) (mission->distance / missions_get_vehicle_maximum_speed(mission->veh->model));
	if (totaltime < mintime) {
		pcheat -= 250000;
		sprintf(buf, "flg(#%d) too fast: min: %d actual: %d", mission->id, mintime, totaltime);
		amx_SetUString(addr + 2000, buf, sizeof(buf));
	} else {
		*(addr + 2000) = 0; /* ac log speed cheat */
	}

	paymp = missions_get_vehicle_paymp(mission->veh->model);
	ptax = -calculate_airport_tax(mission->endpoint->ap, mission->missiontype);
	pdistance = 500 + (int) (mission->distance * 1.135f);
	pdistance = (int) (pdistance * paymp);
	if (mission->missiontype & PASSENGER_MISSIONTYPES) {
		if (mission->passenger_satisfaction == 100) {
			psatisfaction = 500;
		} else {
			psatisfaction = (mission->passenger_satisfaction - 100) * 30;
		}
	}
	pdamage = -3 * mission->damagetaken;
	ptotal = mission->weatherbonus + psatisfaction + pdistance + pbonus + ptax + pdamage + pcheat;

	sprintf(buf,
		"%s completed flight #%d from %s (%s) to %s (%s) in %dh%02dm",
		pdata[playerid]->name,
	        mission->id,
		mission->startpoint->ap->name,
	        mission->startpoint->ap->beacon,
	        mission->endpoint->ap->name,
	        mission->endpoint->ap->beacon,
	        duration_h,
	        duration_m);
	amx_SetUString(addr, buf, sizeof(buf));
	sprintf(buf,
	        "UPDATE flg SET tunload=UNIX_TIMESTAMP(),tlastupdate=UNIX_TIMESTAMP(),"
	        "state=%d,fuel=%f,ptax=%d,pweatherbonus=%d,psatisfaction=%d,"
	        "pdistance=%d,pdamage=%d,pcheat=%d,pbonus=%d,ptotal=%d,satisfaction=%d,adistance=%f,"
	        "paymp=%f,damage=%d WHERE id=%d",
	        MISSION_STATE_FINISHED,
	        mission->fuelburned,
	        ptax,
	        mission->weatherbonus,
	        psatisfaction,
	        pdistance,
	        pdamage,
		pcheat,
	        pbonus,
	        ptotal,
	        mission->passenger_satisfaction,
	        mission->actualdistanceM,
	        paymp,
	        mission->damagetaken,
	        mission->id);
	amx_SetUString(addr + 200, buf, sizeof(buf));
	p = 0;
	p += sprintf(buf,
	             "{ffffff}Flight:\t\t\t"ECOL_MISSION"#%d\n"
	             "{ffffff}Origin:\t\t\t"ECOL_MISSION"%s\n"
	             "{ffffff}Destination:\t\t"ECOL_MISSION"%s\n"
	             "{ffffff}Distance (pt to pt):\t"ECOL_MISSION"%.0fm\n"
	             "{ffffff}Distance (actual):\t"ECOL_MISSION"%.0fm\n"
	             "{ffffff}Duration:\t\t"ECOL_MISSION"%dh%02dm\n"
	             "{ffffff}Fuel Burned:\t\t"ECOL_MISSION"%.1fL\n"
	             "{ffffff}Vehicle pay category:\t"ECOL_MISSION"%.1fx\n",
	             mission->id,
	             mission->startpoint->ap->name,
	             mission->endpoint->ap->name,
	             mission->distance,
	             mission->actualdistanceM,
	             duration_h,
	             duration_m,
		     mission->fuelburned,
	             paymp);
	if (mission->missiontype & PASSENGER_MISSIONTYPES) {
		p += sprintf(buf + p,
		             "{ffffff}Passenger Satisfaction:\t"ECOL_MISSION"%d%%\n",
		             mission->passenger_satisfaction);
	}
	buf[p++] = '\n';
	buf[p++] = '\n';
	if (ptax) {
		p += missions_append_pay(buf + p, "{ffffff}Airport Tax:\t\t", ptax);
	}
	if (mission->weatherbonus) {
		p += missions_append_pay(buf + p, "{ffffff}Weather bonus:\t\t", mission->weatherbonus);
	}
	p += missions_append_pay(buf + p, "{ffffff}Distance Pay:\t\t", pdistance);
	if (mission->missiontype & PASSENGER_MISSIONTYPES) {
		if (psatisfaction > 0) {
			p += missions_append_pay(buf + p, "{ffffff}Satisfaction Bonus:\t", psatisfaction);
		} else {
			p += missions_append_pay(buf + p, "{ffffff}Satisfaction Penalty:\t", psatisfaction);
		}
	}
	if (pdamage) {
		p += missions_append_pay(buf + p, "{ffffff}Damage Penalty:\t", pdamage);
	}
	if (pcheat) {
		p += missions_append_pay(buf + p, "{ffffff}Cheat Penalty:\t\t", pcheat);
	}
	if (pbonus) {
		p += missions_append_pay(buf + p, "{ffffff}Bonus:\t\t\t", pbonus);
	}
	p += missions_append_pay(buf + p, "\n\n\t{ffffff}Total Pay: ", ptotal);
	buf[--p] = 0;
	amx_SetUString(addr + 1000, buf, sizeof(buf));

	amx_GetAddr(amx, params[3], &addr);
	*addr = ptotal;

	missions_cleanup(mission, playerid);
	return 1;
}

void missions_update_satisfaction(int pid, int vid, struct quat *vrot)
{
	struct MISSION *miss;
	int last_satisfaction;
	float qw, qx, qy, qz;
	float tmpvalue = 0.0f;;

	if ((miss = activemission[pid]) != NULL &&
		miss->stage == MISSION_STAGE_FLIGHT &&
		miss->veh->spawnedvehicleid == vid)
	{
		last_satisfaction = miss->passenger_satisfaction;
		qx = vrot->qx;
		qy = vrot->qy;
		qz = vrot->qz;
		qw = vrot->qw;
		/* pitch */
		tmpvalue = fabsf(100.0f * 2.0f * (qy * qz - qw * qx)) - 46.0f;
		if (tmpvalue > 0.0) {
			miss->passenger_satisfaction -= (int) (tmpvalue / 2.0f);
			if (miss->passenger_satisfaction < 0) {
				miss->passenger_satisfaction = 0;
			}
		}
		/* roll */
		tmpvalue = fabsf(100.0f * 2.0f * (qx * qz + qw * qy)) - 61.0f;
		if (tmpvalue > 0.0) {
			miss->passenger_satisfaction -= (int) (tmpvalue / 2.0f);
			if (miss->passenger_satisfaction < 0) {
				miss->passenger_satisfaction = 0;
			}
		}
		if (last_satisfaction != miss->passenger_satisfaction) {
			sprintf(cbuf32_1,
				SATISFACTION_TEXT_FORMAT,
				miss->passenger_satisfaction);
			amx_SetUString(buf32, cbuf32_1, 32);
			NC_PlayerTextDrawSetString(
				pid, ptxt_satisfaction[pid], buf32a);
		}
	}
}
