
/* vim: set filetype=c ts=8 noexpandtab: */

struct dbvehicle {
	int id;
	int model;
	int owneruserid;
	float x, y, z, r;
	int col1, col2;
	float odoKM;
	float fuel;
	char *ownerstring;
	unsigned char ownerstringowneroffset;
	unsigned char needsodoupdate;
	/**
	Vehicleid of the spawned vehicle that reflects this db vehicle.
	If this db vehicle is not spawned, this value should be 0.
	*/
	short spawnedvehicleid;
};

struct vehicle {
	struct dbvehicle *dbvehicle;
	/**
	Number holding the amount of times this vehicle has been respawned.
	Can be used to check if a vehicleid is still assigned to the same
	vehicle as earlier.
	*/
	short reincarnation;
	/**
	Flag denoting if this vehicle needs to be recreated. This should be
	checked whenever the vehicle is respawned (see OnVehicleSpawn).
	Recreation is needed when some vehicle properties change, like spawn
	position or colors.
	*/
	char need_recreation;
};

extern struct vehicle gamevehicles[MAX_VEHICLES];

/**
Should be called in OnGameModeInit.
*/
void veh_create_global_textdraws(AMX*);
/**
Should be called in OnPlayerConnect.
*/
void veh_create_player_textdraws(AMX*, int playerid);
/**
Check if given player can modify a vehicle (park, spray, ..).
Player can modify if they own the vehicle or have admin rights.
*/
int veh_can_player_modify_veh(int playerid, struct dbvehicle *veh);
/**
Commits odo of next vehicle in update queue to db.

To be called in 5 second timer, or to be called in a loop until it returns 0 in
OnGameModeExit.

@return 0 if there was nothing to commit
*/
int veh_commit_next_vehicle_odo_to_db(AMX*);
void veh_init();
/**
Check if a player is allowed to be in given vehicle.

Player are always allowed to be in unknown vehicles, which means veh will never
be NULL when this functions returns 0.

@param veh may be NULL.
*/
int veh_is_player_allowed_in_vehicle(int playerid, struct dbvehicle *veh);
float model_fuel_capacity(int modelid);
/**
Creates a vehicle (see CreateVehicle doc).

@remarks Resets values of gamevehicles struct reflecting the created vehicle.
*/
int veh_NC_CreateVehicle(AMX*, int model, float x, float y, float z,
		         float r, int col1, int col2, int respawn_delay,
		         int addsiren);
int veh_NC_DestroyVehicle(AMX*, int vehicleid);
void veh_on_player_connect(AMX*, int playerid);
/**
To be called from OnPlayerEnterVehicle
*/
void veh_on_player_enter_vehicle(AMX*, int pid, int vid, int ispassenger);
/**
Stuff to do when a player is now driver of a vehicle.

To be called from OnPlayerStateChange with new state being driver, or when
calling PutPlayerInVehicle when the player was already a driver (because
this won't trigger a OnPlayerStateChange.
*/
void veh_on_player_now_driving(AMX*, int playerid, int vehicleid);
void veh_on_player_state_change(AMX*, int, int, int);
int veh_OnVehicleSpawn(AMX*, int vehicleid);
/**
Update vehicle related things like ODO, fuel, ...

To be called every second.
*/
void veh_timed_1s_update(AMX*);
void veh_timed_panel_update(AMX*);
/**
Updates vehicle and player odo.

Given player must be driver of given vehicle.

@param pos the position of the given vehicle
*/
void veh_update_odo(AMX*, int playerid, int vehicleid, struct vec3 pos);
/**
Updates the service point mapicons (and 3D text) for given playerid.

@param x x-position of the player
@param y y-position of the player
*/
void veh_update_service_point_mapicons(AMX*, int playerid, float x, float y);