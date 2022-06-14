// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}

static int buildin_reloadmob_instance_sub(struct block_list *bl,va_list ap)	// Added by RoVeRT
{
	struct mob_data *md = ((struct mob_data *)bl);
	if (md)
		status_calc_mob_instance(md);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(instance_setdifficulty)
{
	int instance_id = script_getnum(st,2);
	e_instance_difficulty id_mode = static_cast<e_instance_difficulty>(script_getnum(st,3));

	if( instance_id <= 0 ) {
		ShowError("instance_setdifficulty: Trying to set difficulty to an invalid instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	if (!(id_mode >= ID_EASY && id_mode < ID_MAX)) {
		ShowError("instance_setdifficulty: Trying to set difficulty to an unexistent difficulty mode to instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	std::shared_ptr<s_instance_data> idata = util::umap_find(instances, instance_id);
	if (!idata) {
		ShowError("instance_setdifficulty: Trying to set difficulty to an unexistent instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}
	if (idata->difficulty == id_mode) { //Same mode
		script_pushint(st,0);
		return SCRIPT_CMD_SUCCESS;
	}
	idata->difficulty = id_mode;

	// Reload Mob Stats
	for (const auto &it : idata->map) {
		map_foreachinmap(buildin_reloadmob_instance_sub, it.m, BL_MOB, 0);
	}

	script_pushint(st,1);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(instance_setpenalty)
{
	int instance_id = script_getnum(st,2);
	enum sc_type type;
	int16 m;

	if( instance_id <= 0 ) {
		ShowError("instance_setpenalty: Trying to set penalty to an invalid instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	std::shared_ptr<s_instance_data> idata = util::umap_find(instances, instance_id);
	if (!idata) {
		ShowError("instance_setpenalty: Trying to set penalty to an unexistent instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	type = (sc_type)script_getnum(st,3);
	int value = script_getnum(st,4);

	if (!(type > SC_NONE && type < SC_MAX)) {
		ShowError("instance_setdifficulty: Unknown SC_STATUS for instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}
	idata->sc_penalties[type] = value;

	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(instance_setbuff)
{
	int instance_id = script_getnum(st,2);
	enum sc_type type;
	int16 m;

	if( instance_id <= 0 ) {
		ShowError("instance_setbuff: Trying to set buff to an invalid instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	std::shared_ptr<s_instance_data> idata = util::umap_find(instances, instance_id);
	if (!idata) {
		ShowError("instance_setbuff: Trying to set penalty to an unexistent instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	type = (sc_type)script_getnum(st,3);
	int value = script_getnum(st,4);

	if (!(type > SC_NONE && type < SC_MAX)) {
		ShowError("instance_setbuff: Unknown SC_STATUS for instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}
	idata->sc_buff[type] = value;

	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(instance_getdifficulty)
{
	int instance_id = script_getnum(st,2);

	if( instance_id <= 0 ) {
		ShowError("instance_getdifficulty: Trying to get difficulty to an invalid instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	std::shared_ptr<s_instance_data> idata = util::umap_find(instances, instance_id);
	if (!idata) {
		ShowError("instance_getdifficulty: Trying to get difficulty to an unexistent instance %d.\n", instance_id);
		return SCRIPT_CMD_FAILURE;
	}

	script_pushint(st, (int)idata->difficulty);
	return SCRIPT_CMD_SUCCESS;
}