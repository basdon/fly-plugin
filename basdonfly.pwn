
// vim: set filetype=c ts=8 noexpandtab:

//@summary Validate the script and plugin {@code MAX_PLAYERS} has the same value
//@param maxplayers should be {@code MAX_PLAYERS}
//@returns {@code 0} if the values don't match
native ValidateMaxPlayers(maxplayers)

//@summary Url encodes given string.
//@param data string to encode
//@param len amount of characters in {@param data}
//@param output buffer to store result in (should be {@code len * 3} of size)
//@returns Amount of charactes written (excluding terminating zero byte).
native Urlencode(const data[], len, output[])

//@summary Reset panel caches for player
//@param playerid the player to reset panel caches for
native Panel_ResetCaches(playerid)

//@summary Formats text for altitude part of panel
//@param playerid the player to update the altitude for
//@param altitude the altitude as int
//@param buf4 buffer for the altitude text, must be of length {@code 4}
//@param buf13 buffer for the altitude small meter text, must be of length {@code 13}
//@param buf44 buffer for the altitude large meter text, must be of length {@code 44}
//@returns {@code 1} if an update is needed
//@remarks {@code buf13[0]} will be set to {@code 0} if the small meter doesn't need to be updated
//@remarks {@code buf44[0]} will be set to {@code 0} if the large meter doesn't need to be updated
native Panel_FormatAltitude(playerid, altitude, buf4[], buf13[], buf44[])

//@summary Formats text for speed part of panel
//@param playerid the player to update the speed for
//@param speed the speed as int
//@param buf4 buffer for the speed text, must be of length {@code 4}
//@param buf13 buffer for the speed small meter text, must be of length {@code 13}
//@param buf44 buffer for the speed large meter text, must be of length {@code 44}
//@returns {@code 1} if an update is needed
//@remarks {@code buf13[0]} will be set to {@code 0} if the small meter doesn't need to be updated
//@remarks {@code buf44[0]} will be set to {@code 0} if the large meter doesn't need to be updated
//@remarks If the small meter doesn't need to be updated, the large meter won't need an update either
native Panel_FormatSpeed(playerid, speed, buf4[], buf13[], buf44[])

//@summary Formats text for heading part of panel
//@param playerid the player to update the heading for
//@param heading the heading as int
//@param buf4 buffer for the speed text, must be of length {@code 4}
//@param buf44 buffer for the speed large meter text, must be of length {@code 44}
//@returns {@code 1} if an update is needed
native Panel_FormatHeading(playerid, heading, buf4[], buf44[])

//@summary Checks if a vehicle with specified model is an air vehicle
//@param model the model of the vehicle to check, {@b MUST} be either a valid model or {@code 0}
//@returns {@code 0} if the model given is not an air vehicle
native IsAirVehicle(model)

//@summary Store password hash to confirm it later using {@link ValidatePasswordConfirmData}
//@param playerid the player for which to store the hash
//@param pwhash the hash to store
//@remarks This will cause a memory leak if {@link ResetPasswordConfirmData} or {@link ValidatePasswordConfirmData} is not called before this is called again for the same {@param playerid}.
//@seealso ResetPasswordConfirmData
//@seealso ValidatePasswordConfirmData
native SetPasswordConfirmData(playerid, pwhash[])

//@summary Check if {@param pwhash} equals the previously saved hash
//@param playerid the player for which the data was saved
//@param pwhash the hash to check
//@returns {@code 0} if the hash doesn't match the hash previously saved with {@link SetPasswordConfirmData}
//@remarks The saved data gets reset after the check, so no need to call {@link ResetPasswordConfirmData}
//@seealso SetPasswordConfirmData
//@seealso ResetPasswordConfirmData
native ValidatePasswordConfirmData(playerid, pwhash[])

//@summary Free memory that was used to confirm a player's password
//@param playerid the player for which the data was saved
//@remarks Has no effect if the data was already reset
//@seealso SetPasswordConfirmData
//@seealso ValidatePasswordConfirmData
native ResetPasswordConfirmData(playerid)

//@summary Gets hash of command in entered command text (excluding params)
//@param cmdtext command text to get hash of
//@remarks End delimiter for calculation is either a zero terminator, or anything below {@code ' '}
native CommandHash(cmdtext[])

//@summary Checks if the entered cmd is {@param cmd}
//@param cmdtext cmdtext entered by player
//@param cmd the command that should match (all lowercase, leading slash)
//@remarks don't exceed cmd length of 49
//@remarks commands can't have spaces in them for obvious reasons
native IsCommand(cmdtext[], const cmd[])

//@summary Queue a dialog to show later
//@param playerid see {@link ShowPlayerDialog}
//@param dialogid see {@link ShowPlayerDialog}
//@param style see {@link ShowPlayerDialog}
//@param caption see {@link ShowPlayerDialog}
//@param info see {@link ShowPlayerDialog}
//@param button1 see {@link ShowPlayerDialog}
//@param button2 see {@link ShowPlayerDialog}
//@param transactionid the transactionid for the dialog
//@returns info see {@link ShowPlayerDialog}
//@remarks info see {@link ShowPlayerDialog}
//@seealso HasDialogsInQueue
//@seealso DropDialogQueue
//@seealso PopDialogQueue
native QueueDialog(playerid, dialogid, style, caption[], info[], button1[], button2[], transactionid)

//@summary Drops the dialog queue for a player
//@param playerid the playerid to drop the queue for
//@seealso HasDialogsInQueue
//@seealso QueueDialog
//@seealso PopDialogQueue
native DropDialogQueue(playerid)

//@summary Check if there are dialogs queued for a player
//@param playerid the playerid to check the queue for
//@returns {@code 0} if the queue is empty
//@seealso DropDialogQueue
//@seealso QueueDialog
//@seealso PopDialogQueue
native HasDialogsInQueue(playerid)

//@summary Pop the first queued dialog for a player
//@param playerid the playerid to pop for
//@param dialogid variable to store dialogid in, passed by ref
//@param style variable to store style in, passed by ref
//@param caption variable to store caption in, MUST be of at least {@code 64} len
//@param info variable to store info in, MUST be of at least {@code 4096} len
//@param button1 variable to store button1 in, MUST be of at least {@code 32} len
//@param button2 variable to store button2 in, MUST be of at least {@code 32} len
//@param transactionid variable to store transactionid in, passed by ref
//@returns {@code 0} if there was no dialog to pop
//@seealso DropDialogQueue
//@seealso QueueDialog
//@seealso HasDialogsInQueue
native PopDialogQueue(playerid, &dialogid, &style, caption[], info[], button1[], button2[], &transactionid)

//@summary Stores a player's data used for login stuff
//@param playerid the playerid
//@param ip player's ip
//@param name player's name
//@param namelen length of {@param name}, excluding zero term
//@seealso UpdateLoginData
//@seealso ClearLoginData
native SetLoginData(playerid, ip[], name[], namelen)

//@summary Updates a player's name in data saved for login stuff
//@param playerid the playerid
//@param name player's name
//@param namelen length of {@param name}, excluding zero term
//@seealso SetLoginData
//@seealso ClearLoginData
//@returns {@code 0} if there was no login data saved for {@param playerid}
native UpdateLoginData(playerid, name[], namelen)

//@summary Clears login data for a player
//@param playerid player to clear login data for
//@seealso SetLoginData
native ClearLoginData(playerid)

//@summary Format data to send to register api
//@param playerid the player for who the call is
//@param inputtext password as entered by player
//@param buf the buffer to store the resulting post data in (use {@code buf4096})
//@returns {@code 0} on failure (no login data set for {@param playerid})
native FormatLoginApiRegister(playerid, inputtext[], buf[])

//@summary Format data to send to login api
//@param playerid the player for who the call is
//@param userid userid of the player
//@param inputtext password as entered by player
//@param buf the buffer to store the resulting post data in (use {@code buf4096})
//@returns {@code 0} on failure (no login data set for {@param playerid})
native FormatLoginApiLogin(playerid, userid, inputtext[], buf[])

//@summary Format data to send to guest register api
//@param playerid the player for who the call is
//@param userid userid of the player
//@param inputtext password as entered by player
//@param buf the buffer to store the resulting post data in (use {@code buf4096})
//@returns {@code 0} on failure (no login data set for {@param playerid})
native FormatLoginApiGuestRegister(playerid, userid, inputtext[], buf[])

//@summary Format data to send to check/change passowrd api (they expect the same data)
//@param userid userid of the player
//@param inputtext password as entered by player
//@param buf the buffer to store the resulting post data in (use {@code buf4096})
//@returns {@code 0} on failure (no login data set for {@param playerid})
native FormatLoginApiCheckChangePass(userid, inputtext[], buf[])

//@summary Format data to send to user exists/guest api (they expect the same data)
//@param playerid playerid of the player
//@param buf the buffer to store the resulting post data in (use {@code buf4096})
//@returns {@code 0} on failure (no login data set for {@param playerid})
native FormatLoginApiUserExistsGuest(playerid, buf[])

//@summary Inits the airport table
//@param amount amount of airports that will be created using {@link APT_Add}
native APT_Init(amount)

//@summary Add an airport to the airports table
//@param index the index in the table to add this airport
//@param code airport's code, must not be longer than {@code 4} + zero term
//@param enabled {@code 0} if the airport should not be usable
//@param name airport's name, must not be longer than {@code MAX_AIRPORT_NAME} + zero term
//@param beacon airport's beacon, must not be longer than {@code 4} + zero term
//@param x beacon's x position
//@param y beacon's y position
//@param z beacon's z position
native APT_Add(index, code[], enabled, name[], beacon[], Float:x, Float:y, Float:z)

//@summary Clear the airport table and free the used memory
native APT_Destroy()

