/**
Initialize mapping system. Loads maps from db and reads their files.
*/
void maps_init();
void maps_on_player_connect(int playerid);
void maps_on_player_disconnect(int playerid);
/*
Go over players to see if maps need to be streamed for them.

This function should be called from timer100
It checks 1+playercount/10 amount of players per call, so:

 playercount | checked players | ms between ticks
     1-9              1              100-900
    10-19             2              500-950
    20-29             3              700-966
   100-109           11	            1000-1000
 (not sure if this is correct; I'm too tired)
*/
void maps_process_tick();
/**
Stream maps for given player.
*/
void maps_stream_for_player(int playerid, struct vec3 pos);
/**
@return amount of objects that would be created when the player would be teleported to given pos
*/
int maps_calculate_objects_to_create_for_player_at_position(int playerid, struct vec3 pos);
