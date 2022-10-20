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

BUILDIN_FUNC(checkspace)
{
	int nameid = 0, amount;
	struct map_session_data *sd;

	if( !script_rid2sd(sd) )
		script_pushint(st,0);
	else if( sd->storage.amount > MAX_STORAGE )
		script_pushint(st,0); // Storage at max
	else
	{
		struct script_data *data = script_getdata(st,2);
		std::shared_ptr<item_data> id;
		struct item it;
		int i;

		get_val(st,data);
		if( data_isstring(data) )
		{
			const char *name = conv_str(st,data);
			std::shared_ptr<item_data> id = item_db.searchname(name);
			if( id )
				nameid = id->nameid;
		}
		else
			nameid = conv_num(st,data);

		memset(&it,0,sizeof(it));
		amount = script_getnum(st,3);
		it.nameid = nameid;
		it.identify = script_getnum(st,4);
		it.refine = script_getnum(st,5);
		it.attribute = script_getnum(st,6);
		it.card[0] = (short)script_getnum(st,7);
		it.card[1] = (short)script_getnum(st,8);
		it.card[2] = (short)script_getnum(st,9);
		it.card[3] = (short)script_getnum(st,10);

		if( nameid < 500 || amount <= 0 || (id = item_db.find(nameid)) == nullptr || !itemdb_canstore(&it, pc_get_group_level(sd)) || !itemdb_isstackable2(id.get()) )
		{
			script_pushint(st,0);
			return 0;
		}

		if( itemdb_isstackable2(id.get()) )
		{
			ARR_FIND(0,MAX_STORAGE,i,compare_item(&sd->storage.u.items_storage[i],&it,0));
			if( i < MAX_STORAGE )
			{ // Item on Storage
				script_pushint(st,amount + sd->storage.u.items_storage[i].amount > MAX_AMOUNT ? 0 : 1);
				return 0;
			}
		}

		ARR_FIND(0,MAX_STORAGE,i,sd->storage.u.items_storage[i].nameid == 0);
		if( i >= MAX_STORAGE )
		{
			script_pushint(st,0);
			return 0;
		}
	
		script_pushint(st,1); // Can be Stored
	}

	return 0;
}

BUILDIN_FUNC(storeitem)
{
	int nameid = 0, amount;
	struct item it;
	struct map_session_data *sd;
	struct script_data *data;
	int result = 1;

	data = script_getdata(st,2);
	get_val(st,data);
	if( data_isstring(data) )
	{ // "<item name>"
		const char *name = conv_str(st,data);
		std::shared_ptr<item_data> item_data = item_db.searchname(name);
		if( item_data == NULL )
		{
			ShowError("buildin_storeitem: Nonexistant item %s requested.\n", name);
			result = 0; //No item created.
		}
		else
			nameid = item_data->nameid;
	}
	else if( data_isint(data) )
	{ // <item id>
		nameid = conv_num(st,data);
		if( nameid <= 0 || !item_db.exists(nameid) )
		{
			ShowError("buildin_storeitem: Nonexistant item %d requested.\n", nameid);
			result = 0;
		}
	}
	else
	{
		ShowError("buildin_storeitem: invalid data type for argument #1 (%d).", data->type);
		result = 0;
	}

	if( !itemdb_isstackable(nameid) )
		result = 0;
	if( (amount = script_getnum(st,3)) <= 0 )
		result = 0;

	if( script_hasdata(st,4) )
		sd = map_id2sd(script_getnum(st,4)); // <Account ID>
	else
		script_rid2sd(sd); // Attached player

	if( sd == NULL ) // no target
		result = 0;

	if( result )
	{
		memset(&it,0,sizeof(it));
		it.nameid = nameid;
		it.identify = 1;
		if( storage_storageopen(sd) == 1 ) {
			result = 0;
		} else {
			result = storage_additem2(sd,&it,amount);
			storage_storageclose(sd);
			if( result )
				log_pick_pc(sd, LOG_TYPE_SCRIPT, amount, &it);
		}
	}

	script_pushint(st,result);
	return 0;
}

BUILDIN_FUNC(storeitem2)
{
	int nameid = 0, amount = 0;
	int iden = 0,ref = 0,attr = 0,c1 = 0,c2 = 0,c3 = 0,c4 = 0;
	struct map_session_data *sd;
	struct script_data *data;
	struct item it;
	int result = 1;

	data = script_getdata(st,2);
	get_val(st,data);
	if( data_isstring(data) )
	{
		const char *name = conv_str(st,data);
		std::shared_ptr<item_data> item_data = item_db.searchname(name);
		if( item_data == NULL )
		{
			ShowError("buildin_storeitem2: Nonexistant item %s requested.\n", name);
			result = 0;
		}
		else
			nameid = item_data->nameid;
	}
	else if( data_isint(data) )
	{
		nameid = conv_num(st,data);
		if( nameid <= 0 || !item_db.exists(nameid) )
		{
			ShowError("buildin_storeitem2: Nonexistant item %d requested.\n", nameid);
			result = 0;
		}
	}
	else
	{
		ShowError("buildin_storeitem2: invalid data type for argument #1 (%d).", data->type);
		result = 0;
	}

	if( !itemdb_isstackable(nameid) )
		result = 0;
	else if( (amount = script_getnum(st,3)) <= 0 )
		result = 0;
	else
	{
		iden = script_getnum(st,4);
		ref = script_getnum(st,5);
		attr = script_getnum(st,6);
		c1 = (short)script_getnum(st,7);
		c2 = (short)script_getnum(st,8);
		c3 = (short)script_getnum(st,9);
		c4 = (short)script_getnum(st,10);
	}

	if( script_hasdata(st,11) )
		sd = map_id2sd(script_getnum(st,11));
	else
		script_rid2sd(sd);

	if( sd == NULL )
		result = 0;

	if( result )
	{
		memset(&it,0,sizeof(it));
		it.nameid = nameid;
		it.identify = 1; // always Identify
		it.refine = ref;
		it.attribute = attr;
		it.card[0] = (short)c1;
		it.card[1] = (short)c2;
		it.card[2] = (short)c3;
		it.card[3] = (short)c4;
		if( storage_storageopen(sd) == 1 ) {
			result = 0;
		} else {
			result = storage_additem2(sd,&it,amount);
			storage_storageclose(sd);
			if( result )
				log_pick_pc(sd, LOG_TYPE_SCRIPT, amount, &it);
		}
	}
	script_pushint(st,result);
	return 0;
}

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