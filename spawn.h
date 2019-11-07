
/* vim: set filetype=c ts=8 noexpandtab: */

/**
Call on spawn to show a dialog of spawn locations to teleport to.

spawn_prespawn should've been called before, which should've set the player's
spawninfo before spawning, so player should already have spawned at a location
of their choice (as set in preferences) by this time.
*/
void spawn_on_player_spawn(AMX*, int playerid);
/**
Set player spawn info before a player spawns.

To be called from OnPlayerRequestClass and OnPlayerDeath.

Otherwise players get teleported right after spawn, causing updates like map
streaming and zones twice right after each other.
*/
void spawn_prespawn(AMX*, int playerid);