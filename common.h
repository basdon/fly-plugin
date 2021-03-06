
/* vim: set filetype=c ts=8 noexpandtab: */

/*prototype for commonly used struct*/
struct dbvehicle;

struct PLAYERKEYS {
	int keys;
	int updown;
	int leftright;
};
EXPECT_SIZE(struct PLAYERKEYS, 3 * sizeof(cell));

struct VEHICLEPARAMS {
	int engine;
	int lights;
	int alarm;
	int doors;
	int bonnet;
	int boot;
	int objective;
};
EXPECT_SIZE(struct VEHICLEPARAMS, 7 * sizeof(cell));

struct VEHICLEDAMAGE {
	unsigned int panels;
	unsigned int doors;
	unsigned int lights;
	unsigned int tires;
};
EXPECT_SIZE(struct VEHICLEDAMAGE, 4 * sizeof(cell));

/**
Convert char string to cell string.

@param max must be gt 0
*/
void atoc(cell *dst, char *src, int max);
/**
Convert cell string to char string.

@param max must be gt 0
*/
void ctoa(char *dst, cell *src, int max);
/**
Convert char string to cell string, in place.

@param len length of the string, excluding zero term
*/
void atoci(cell *dstsrc, int len);
/**
Convert a cell string to a char string, in place.

Relies on good faith that the cell string is zero terminated.
*/
void ctoai(char *dstsrc);

#define CLAMP(X,L,U) ((X < L) ? L : ((X > U) ? U : X))
#define Q_USE_EQ_INSTEAD(X) #X
#define EQ(X) Q_USE_EQ_INSTEAD(X)

#define MK_PLAYER_CC(PLAYERID) \
	((_cc[PLAYERID] & 0x003FFFFF) | (PLAYERID << 22))
#define V_MK_PLAYER_CC(PLAYERID) ((void*) MK_PLAYER_CC(PLAYERID))
#define PLAYER_CC_GETID(VALUE) (((unsigned int) VALUE >> 22) & 0x3FF)
#define PLAYER_CC_CHECK(VALUE,PLAYERID) \
	(_cc[PLAYERID] == ((unsigned int) VALUE & 0x003FFFFF))
#define PLAYER_CC_GET_CHECK_RETURN(VALUE,PLAYERID) \
	PLAYERID=PLAYER_CC_GETID(VALUE);\
	if(!PLAYER_CC_CHECK(VALUE,PLAYERID))return;


#define ECOL_INFO "{3498db}"
#define ECOL_WARN "{e84c3d}"
#define ECOL_SUCC "{2cc36b}"
#define ECOL_DIALOG_CAPTION "{d2d2d2}"
#define ECOL_DIALOG_TEXT "{a9c4e4}"
#define ECOL_DIALOG_BUTTON "{c8c8c8}"
#define ECOL_MISSION "{ff9900}"

#define WARN "! "
#define INFO "* "
#define SUCC "" /*the + might confuse people*/

#define COL_SAMP_GREEN 0x88aa62ff /*as in username error message*/
#define COL_SAMP_GREY 0xa9c4e4ff /*as in 'Connected to..'*/
#define COL_SAMP_BLUE 0x2587ceff /*as in '* Admin:'*/
#define COL_INFO 0x3498dbff
#define COL_WARN 0xe84c3dff
#define COL_SUCC 0x2cc36bff
#define COL_INFO_GENERIC 0xffffffff
#define COL_INFO_LIGHT 0x33ccffff
#define COL_INFO_BROWN 0xcc9966ff
#define COL_PRIVMSG 0xffff00ff
#define COL_PRIVMSG_HINT 0xafafafff
#define COL_MISSION 0xff9900ff
#define COL_JOIN 0x1b8ae4ff
#define COL_QUIT 0x1674Bcff
#define COL_METAR COL_INFO_LIGHT
#define COL_IRC_MODE 0x00ffffff
#define COL_IRC_ACTION 0xff00ffff

#define GROUP_BANNED (1)
#define GROUP_GUEST (2)
#define GROUP_MEMBER (4)
#define GROUP_ADMIN (268435456)
#define GROUP_OWNER (1073741824)

#define GROUPS_ALL \
	(GROUP_BANNED|GROUP_GUEST|GROUP_MEMBER|\
	GROUP_ADMIN|GROUP_OWNER)
#define GROUPS_ADMIN (GROUP_ADMIN|GROUP_OWNER)
#define GROUPS_NOTBANNED (GROUPS_ALL&~GROUPS_BANNED)
#define GROUPS_REGISTERED (GROUPS_ALL&~GROUP_GUEST)

#define GROUPS_ISADMIN(X) ((X) >= GROUP_ADMIN)

/*in cells*/
#define STACK_HEAP_SIZE 1024

struct FAKEAMX {
	char _cod[4472];
	union {
		char _dat;
		cell emptystring;
	};
	cell underscorestring[2];
	union {
		cell ascell[32];
		char aschr[32 * sizeof(cell)];
		float asflt[32];
	} a32;
	union {
		cell ascell[32];
		char aschr[32 * sizeof(cell)];
		float asflt[32];
	} a32_1;
	union {
		cell ascell[64];
		char aschr[64 * sizeof(cell)];
		float asflt[64];
	} a64;
	union {
		cell ascell[144];
		char aschr[144 * sizeof(cell)];
		float asflt[144];
	} a144;
	union {
		cell ascell[4096];
		char aschr[4096 * sizeof(cell)];
		float asflt[4096];
		struct {
			char _pad[4096 * 3];
			char last4096[4096];
		} splitted;
	} a4096;
	cell _stackheap[STACK_HEAP_SIZE];
};

#define basea ((int) &fakeamx._dat)
#define emptystringa ((int) &fakeamx.emptystring - basea)
#define underscorestringa ((int) &fakeamx.underscorestring - basea)
#define buf32a ((int) &fakeamx.a32 - basea)
#define buf32_1a ((int) &fakeamx.a32_1 - basea)
#define buf64a ((int) &fakeamx.a64 - basea)
#define buf144a ((int) &fakeamx.a144 - basea)
#define buf4096a ((int) &fakeamx.a4096 - basea)
#define buf32 (fakeamx.a32.ascell)
#define buf32_1 (fakeamx.a32_1.ascell)
#define buf64 (fakeamx.a64.ascell)
#define buf144 (fakeamx.a144.ascell)
#define buf4096 (fakeamx.a4096.ascell)
#define cbuf32 fakeamx.a32.aschr
#define cbuf32_1 fakeamx.a32_1.aschr
#define cbuf64 fakeamx.a64.aschr
#define cbuf144 fakeamx.a144.aschr
#define cbuf4096 fakeamx.a4096.aschr
#define cbuf4096_ fakeamx.a4096.splitted.last4096
#define fbuf32 fakeamx.a32.asflt
#define fbuf32_1 fakeamx.a32_1.asflt
#define fbuf64 fakeamx.a64.asflt
#define fbuf144 fakeamx.a144.asflt
#define fbuf4096 fakeamx.a4096.asflt


/**
element at index playerid is either 0 or 1
*/
extern char playeronlineflag[MAX_PLAYERS];
/**
contains 'playercount' elements, ids of connected players (not sorted)
*/
extern short players[MAX_PLAYERS];
extern int playercount;
/**
Should be decremented in OnPlayerUpdate and player kicked when it reaches 0.
*/
extern char kick_update_delay[MAX_PLAYERS];
/**
Holds spawned status of players.
*/
extern int spawned[MAX_PLAYERS];
/**
Holds class the players are playing as.
*/
extern int playerclass[MAX_PLAYERS];
/**
Current in-game hour.
*/
extern int time_h;
/**
Current in-game minute.
*/
extern int time_m;

#define IsPlayerConnected(PLAYERID) playeronlineflag[PLAYERID]

/**
Teleport the player to a coordinate, and set facing angle and reset camera.
*/
void common_tp_player(int playerid, struct vec4 pos);
/**
Try to find the player that is in given seat of given vehicle.

@return player id or INVALID_PLAYER_ID
*/
int common_find_player_in_vehicle_seat(int vehicleid, int seat);
#define common_find_vehicle_driver(VEHICLEID) \
	common_find_player_in_vehicle_seat(VEHICLEID,0)
/**
@return squared xyz distance between given points
*/
float common_dist_sq(struct vec3 a, struct vec3 b);
/**
@return squared xy distance between given points
*/
float common_xy_dist_sq(struct vec3 a, struct vec3 b);
/**
Sets the state of the engine for given vehicle id.
*/
void common_set_vehicle_engine(int vehicleid, int enginestatus);
/**
Alternative for GetPlayerKeys to get it directly into a PLAYERKEYS struct.

Will use buf32.
*/
void common_GetPlayerKeys(int playerid, struct PLAYERKEYS *keys);
/**
Alternative for GetPlayerPos to get it directly into a vec3 struct.

Will use buf32, buf64, buf144.
*/
int common_GetPlayerPos(int playerid, struct vec3 *pos);
/**
Get a player's position and rotation directly into a vec4 struct.

Will use buf32, buf64, buf144.
*/
int common_GetPlayerPosRot(int playerid, struct vec4 *pos);
/**
Convenience method for GetVehicleDamageStatus.

Uses buf32.
*/
int common_GetVehicleDamageStatus(int vehicleid, struct VEHICLEDAMAGE *d);
/**
Gets vehicle params of given vehicle into given VEHICLEPARAMS struct.

Uses buf32.
*/
int common_GetVehicleParamsEx(int vehicleid, struct VEHICLEPARAMS *p);
/**
Callback-like for DestroyVehicle and OnVehicleSpawn (without recreation) events.

In the case of OnVehicleSpawn WITH recreation, DestroyVehicle is used, which
also calls this function.

In the case of DestroyVehicle, this function is
called _before_ dbvehicle <-> gamevehicle links are cleared.
*/
void common_on_vehicle_respawn_or_destroy(int vehicleid, struct dbvehicle *veh);
/**
Sets vehicle params of given vehicle into given VEHICLEPARAMS struct.

Uses buf32.
*/
int common_SetVehicleParamsEx(int vehicleid, struct VEHICLEPARAMS *p);
/**
Convenience method to set a vehicle's pos to given vec3 struct.

Uses buf32 buf64 buf144.
*/
int common_SetVehiclePos(int vehicleid, struct vec3 *pos);
/**
Convenience method for UpdateVehicleDamageStatus.

Uses buf32.
*/
int common_UpdateVehicleDamageStatus(int vehicleid, struct VEHICLEDAMAGE *d);
/**
Check a string agains its hash.

Uses the last 8 components of buf4096 for callback stuff.

@param callback function to call when the checking is done
@param data usually a pointer to allocated memory that should be freed in the cb
*/
void common_bcrypt_check(cell pw, cell hash, cb_t callback, void *data);
/**
Hash a string.

Uses the last 8 components of buf4096 for callback stuff.

@param callback function to call when the hasing is done
@param data usually a pointer to allocated memory that should be freed in the cb
*/
void common_bcrypt_hash(cell pw, cb_t callback, void *data);
/**
Uses the mysql plugin escape string routine.

Uses buf4096
*/
void common_mysql_escape_string(char *data, char *dest, int maxlen);
/**
Calls mysql_tquery with a callback and data to pass.

Uses buf4096 for query. Also uses the last 8 components of buf4096.

@param callback function to call when the query has been executed
@param data usually a pointer to allocated memory that should be freed in the cb
*/
void common_mysql_tquery(char *query, cb_t callback, void *data);
float common_vectorsize_sq(struct vec3 vec);
