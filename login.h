
/* vim: set filetype=c ts=8 noexpandtab: */

#define LOGGED_NO 0
#define LOGGED_IN 1
#define LOGGED_GUEST 2

#define ISPLAYING(PLAYERID) (loggedstatus[PLAYERID])

#define PW_HASH_LENGTH (65) /* including zero term */
#define MONEY_DEFAULT_AMOUNT 15000

/**
User id of player, -1 if no valid session (should be extremely rare)

Starts from 1
*/
extern int userid[MAX_PLAYERS];
/**
Player's session id, -1 if no valid session (should be extremely rare)

Starts from 1
*/
extern int sessionid[MAX_PLAYERS];
/**
Logged-in status of each player (one of the LOGGED_ definitions).
*/
extern int loggedstatus[MAX_PLAYERS];
/**
Some data saved to use in the progress of login in, registering, or changing pw.
*/
extern void *pwdata[MAX_PLAYERS];

/**
Give the user a name prefixed with an '@' symbol, indicating they're a guest.

@return 0 on failure and player will be kicked
*/
int login_give_guest_name(int playerid);
/**
@return 0 when player has unacceptable name
*/
int login_on_player_connect(int playerid);
void login_on_player_disconnect(int playerid, int reason);
/**
Function to call on response from showing login dialog.
*/
void login_dlg_login_or_namechange(int playerid, int response, char *inputtext);
/**
Function to call on response from namechange dialog.
*/
void login_dlg_namechange(int playerid, int response, char *inputtext);
/**
Function to call on response from showing register confirm pass dialog.
*/
void login_dlg_register_confirmpass(int pid, int resp, cell ia, char *i);
/**
Function to call on response from showing register first pass dialog.
*/
void login_dlg_register_firstpass(int playerid, int response, cell inputaddr);