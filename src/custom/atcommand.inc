// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}

ACMD_FUNC(hold)
{
	nullpo_retr(-1, sd);
	
	if (!sd->state.block_action)
	{
		sd->state.block_action  = true;
		clif_displaymessage(fd, "Character movement turned off");
	}
	else
	{
		sd->state.block_action  = false;
		clif_displaymessage(fd, "Character movement turned on");
	}
	return 0;
}

ACMD_FUNC(heart)
{
	nullpo_retr(-1, sd);

	if(!message || !*message || atoi(message)<0 || atoi(message)>2)
	{
		clif_displaymessage(fd, "usage: @heart 1 or 2");
		return -1;
	}
	
	if(atoi(message)==1)
	{
		clif_specialeffect(&sd->bl, 364, ALL_CLIENT);
	} 
	else if(atoi(message)==2)
	{
		clif_specialeffect(&sd->bl, 509, ALL_CLIENT);
	}
	
	return 0;
}

ACMD_FUNC(dance)
{
	nullpo_retr(-1, sd);

	if(!message || !*message || atoi(message)<0 || atoi(message)>9)
	{
		clif_displaymessage(fd, "usage: @dance 1-9");
		return -1;
	}
	
	switch(atoi(message))
	{
		case 1:
			clif_specialeffect(&sd->bl, 413, ALL_CLIENT);
			break;
		case 2:
			clif_specialeffect(&sd->bl, 414, ALL_CLIENT);
			break;
		case 3:
			clif_specialeffect(&sd->bl, 415, ALL_CLIENT);
			break;
		case 4:
			clif_specialeffect(&sd->bl, 426, ALL_CLIENT);
			break;
		case 5:
			clif_specialeffect(&sd->bl, 458, ALL_CLIENT);
			break;
		case 6:
			clif_specialeffect(&sd->bl, 466, ALL_CLIENT);
			break;
		case 7:
			clif_specialeffect(&sd->bl, 501, ALL_CLIENT);
			break;
		case 8:
			clif_specialeffect(&sd->bl, 540, ALL_CLIENT);
			break;
		case 9:
			clif_specialeffect(&sd->bl, 550, ALL_CLIENT);
			break;
	}
	
	return 0;
}

/*==========================================
 * Packet Filter
 *------------------------------------------*/
ACMD_FUNC(packetfilter)
{
	nullpo_retr(-1,sd);
	if( !message || !*message )
	{
		clif_displaymessage(fd,"<<----- Packet Filtering Usage ----->>");
		clif_displaymessage(fd,".   @packetfilter <options>");
		clif_displaymessage(fd,".   C : To filter global chat messages.");
		clif_displaymessage(fd,".   I : To filter item usage.");
		clif_displaymessage(fd,".   - Samples");
		clif_displaymessage(fd,".   @packetfilter CI : To filter the 2 options.");
		clif_displaymessage(fd,".   @packetfilter off : To turn packet filter off.");
	}
	else if( !strcmpi(message,"off") )
	{
		sd->state.packet_filter = 0;
		clif_displaymessage(fd,"<< Packet Filtering Off >>");
	}
	else
	{
		if( strstr(message,"C") )
			sd->state.packet_filter |= 1;
		if( strstr(message,"I") )
			sd->state.packet_filter |= 2;

		sprintf(atcmd_output,"<< Packet Filtering | Chat %s | Items %s >>", (sd->state.packet_filter&1) ? "ON" : "OFF", (sd->state.packet_filter&2) ? "ON" : "OFF");
		clif_displaymessage(fd,atcmd_output);
	}

	return 0;
}

ACMD_FUNC(reportafk)
{
	map_session_data *pl_sd;
	nullpo_retr(-1,sd);
	if( !sd->bg_id )
		clif_displaymessage(fd, "This command is reserved for Battleground Only.");
	else if( !message || !*message )
		clif_displaymessage(fd, "Please, enter the character name (usage: @reportafk <name>).");
	else if( (pl_sd = map_nick2sd((char *)message,false)) == NULL )
		clif_displaymessage(fd, msg_txt(sd,3)); // Character not found.
	else if( sd->bg_id != pl_sd->bg_id )
		clif_displaymessage(fd, "Destination Player is not in your Team.");
	else if( sd == pl_sd )
		clif_displaymessage(fd, "You cannot kick yourself.");
	else if( pl_sd->state.bg_afk == 0 )
		clif_displaymessage(fd, "The player is not AFK on this Battleground.");
	else
	{ // Everytest OK!
		std::shared_ptr<s_battleground_data> bg = util::umap_find(bg_team_db, sd->bg_id);
		if( !bg )
			return -1;

		clif_displaymessage(pl_sd->fd, "You have been kicked from Battleground because of your AFK status.");
		sprintf(atcmd_output, "- AFK [%s] Kicked -", pl_sd->status.name);
		
		const char *fontColor = "0x6FA8DC";
		int         fontType = 0x190; 	// default fontType (FW_NORMAL)
		int         fontSize = 12;    	// default fontSize
		int         fontAlign = 0;     	// default fontAlign
		int         fontY = 0;     		// default fontY

		clif_broadcast2(NULL, atcmd_output , (int)strlen(atcmd_output) + 1, strtol(fontColor, (char **)NULL, 0), fontType, fontSize, fontAlign, fontY, BG_LISTEN);
		bg_team_leave(pl_sd, true, true, 2);
		return 0;
	}
	return -1;
}

ACMD_FUNC(listenbg)
{
	if (sd->state.bg_listen)
	{
		sd->state.bg_listen = 0;
		clif_displaymessage(fd, "You will receive Battleground announcements");
	}
	else
	{
		sd->state.bg_listen = 1;
		clif_displaymessage(fd, "You will not receive Battleground announcements.");
	}

	return 0;
}

ACMD_FUNC(whosell){
	char item_name[100];
	int item_id = 0, j, count = 0, sat_num = 0;
	int s_type = 1; // search bitmask: 0-name,1-id, 2-card, 4-refine
	int refine = 0,card_id = 0;
	bool flag = 1; // place dot on the minimap?
	struct map_session_data* pl_sd;
	struct s_mapiterator* iter;
	unsigned int MinPrice = battle_config.vending_max_value, MaxPrice = 0;
	static char atcmd_output[CHAT_SIZE_MAX];
	struct item item_tmp = {};
	
	std::shared_ptr<item_data> item_data = item_db.searchname( item_name );

	if (!*message) {
		clif_displaymessage(fd, "Use: @whosell <+refine> <name or id>[card id]");
		return -1;
	}
	if (sscanf(message, "+%d %d[%d]", &refine, &item_id, &card_id) == 3){
		s_type = 1+2+4;
	}
	else if (sscanf(message, "+%d %d", &refine, &item_id) == 2){
		s_type = 1+4;
	}
	else if (sscanf(message, "+%d [%d]", &refine, &card_id) == 2){
		s_type = 2+4;
	}
	else if (sscanf(message, "%d[%d]", &item_id, &card_id) == 2){
		s_type = 1+2;
	}
	else if (sscanf(message, "[%d]", &card_id) == 1){
		s_type = 2;
	}
	else if (sscanf(message, "+%d", &refine) == 1){
		s_type = 4;
	}
	else if (sscanf(message, "%d", &item_id) == 1 && item_id == atoi(message)){
		s_type = 1;
	}
	else if (sscanf(message, "%99[^\n]", item_name) == 1){
		s_type = 1;
	if ((item_data = item_db.searchname(item_name)) == NULL){
		clif_displaymessage(fd, "Not found item with this name");
		return -1;
	}
		item_id = item_data->nameid;
	} else {
		clif_displaymessage(fd, "Use: @whosell <item_id> or @whosell <name>");
		return -1;
	}

	//check card
	if(s_type & 2 && ((item_data = item_db.find(card_id)) == NULL || item_data->type != IT_CARD)){
		clif_displaymessage(fd, "Not found a card with than ID");
		return -1;
	}
	//check item
	if(s_type & 1 && (item_data = item_db.find(item_id)) == NULL){
		clif_displaymessage(fd, "Not found an item with than ID");
		return -1;
	}
	//check refine
	if(s_type & 4){
		if (refine<0 || refine>10){
			clif_displaymessage(fd, "Refine out of bounds: 0 - 10");
			return -1;
		}
		/*if(item_data->type != IT_WEAPON && item_data->type != IT_ARMOR){
			clif_displaymessage(fd, "Use refine only with weapon or armor");
			return -1;
		}*/
	}
	iter = mapit_getallusers();
	for (pl_sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); pl_sd = (TBL_PC*)mapit_next(iter)) {
		if (pl_sd->vender_id ) {	 // check if player is vending
			for (j = 0; j < pl_sd->vend_num; j++) {
				if ((item_data = item_db.find(pl_sd->cart.u.items_cart[pl_sd->vending[j].index].nameid)) == NULL)
					continue;
				if(s_type & 1 && pl_sd->cart.u.items_cart[pl_sd->vending[j].index].nameid != item_id)
					continue;
				if(s_type & 2 && ((item_data->type != IT_ARMOR && item_data->type != IT_WEAPON) ||
						(pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[0] != card_id &&
						pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[1] != card_id &&
						pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[2] != card_id &&
						pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[3] != card_id)))
					continue;
				if(s_type & 4 && ((item_data->type != IT_ARMOR && item_data->type != IT_WEAPON) || pl_sd->cart.u.items_cart[pl_sd->vending[j].index].refine != refine))
					continue;
				item_tmp.nameid = item_data->nameid;
				if(item_data->type == IT_ARMOR) {
					item_tmp.refine = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].refine;
					item_tmp.card[0] = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[0];
				} else if(item_data->type == IT_WEAPON) {
					item_tmp.refine = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].refine;
					item_tmp.card[0] = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[0];
					item_tmp.card[1] = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[1];
					item_tmp.card[2] = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[2];
					item_tmp.card[3] = pl_sd->cart.u.items_cart[pl_sd->vending[j].index].card[3];
				}
				snprintf(atcmd_output, CHAT_SIZE_MAX, "%s - Price: %uz - Amount: %d - %s %d,%d - %s"
				,item_db.create_item_link( item_tmp ).c_str()
				,pl_sd->vending[j].value
				,pl_sd->vending[j].amount
				,mapindex_id2name(pl_sd->mapindex)
				,pl_sd->bl.x,pl_sd->bl.y
				,pl_sd->message); 
				if(pl_sd->vending[j].value < MinPrice) MinPrice = pl_sd->vending[j].value;
				if(pl_sd->vending[j].value > MaxPrice) MaxPrice = pl_sd->vending[j].value;
				clif_displaymessage(fd, atcmd_output);
				count++;
				flag = 1;
			}
			if (flag && pl_sd->mapindex == sd->mapindex) {
				clif_viewpoint(sd, 1, 1, pl_sd->bl.x, pl_sd->bl.y, ++sat_num, 0xFFFFFF);
				flag = 0;
			}
		}
	}
	mapit_free(iter);
	if(count > 0) {
		snprintf(atcmd_output, CHAT_SIZE_MAX, "Found %d entries.", count);
		clif_displaymessage(fd, atcmd_output);
	} else
		clif_displaymessage(fd, "Nobody is selling it now.");

	return 0;
}

ACMD_FUNC(bgrank)
{
	int i;

	clif_messagecolor(&sd->bl, color_table[COLOR_LIGHT_GREEN], "============ BATTLEGROUND RANK ============", false, SELF);
	for( i = 0; i < MAX_FAME_LIST; i++ )
	{
		if (!bg_fame_list[i].id)
			sprintf(atcmd_output,"[ %d ] None        :    0 POINT",i+1);
		else
			sprintf(atcmd_output,"[ %d ] %s        :    %d POINT",i+1,bg_fame_list[i].name,bg_fame_list[i].fame);
		clif_messagecolor(&sd->bl, color_table[COLOR_LIGHT_GREEN], atcmd_output, false, SELF);
	}
	sprintf(atcmd_output,"MY POINT    :    %d POINT",sd->status.bgstats.points);
	clif_messagecolor(&sd->bl, color_table[COLOR_LIGHT_GREEN], "===========================================", false, SELF);
	clif_messagecolor(&sd->bl, color_table[COLOR_LIGHT_GREEN], atcmd_output, false, SELF);
	clif_messagecolor(&sd->bl, color_table[COLOR_LIGHT_GREEN], "===========================================", false, SELF);
	return 0;
}

ACMD_FUNC(woerank)
{
	int i;

	clif_messagecolor(&sd->bl, color_table[COLOR_CYAN], "============ WAR OF EMPERIUM RANK ============", false, SELF);
	for( i = 0; i < MAX_FAME_LIST; i++ )
	{
		if (!woe_fame_list[i].id)
			sprintf(atcmd_output,"[ %d ] None        :    0 POINT",i+1);
		else
			sprintf(atcmd_output,"[ %d ] %s        :    %d POINT",i+1,woe_fame_list[i].name,woe_fame_list[i].fame);
		clif_messagecolor(&sd->bl, color_table[COLOR_CYAN], atcmd_output, false, SELF);
	}
	sprintf(atcmd_output,"MY POINT    :    %d POINT",sd->status.wstats.points);
	clif_messagecolor(&sd->bl, color_table[COLOR_CYAN], "===========================================", false, SELF);
	clif_messagecolor(&sd->bl, color_table[COLOR_CYAN], atcmd_output, false, SELF);
	clif_messagecolor(&sd->bl, color_table[COLOR_CYAN], "===========================================", false, SELF);
	return 0;
}

ACMD_FUNC(bgstats)
{
	pc_battle_stats(sd,sd,1);
	return 0;
}

ACMD_FUNC(woestats)
{
	pc_battle_stats(sd,sd,2);
	return 0;
}

ACMD_FUNC(gcash)
{
	map_session_data* tsd = nullptr;
	int amount = 0, ret = 0;
	char output[CHAT_SIZE_MAX];
	nullpo_retr(-1,sd);

	memset(atcmd_output, '\0', sizeof(atcmd_output));

	if( sd->state.cashshop_open ){
		clif_displaymessage(fd, msg_txt(sd, 1376)); // Please close the cashshop before using this command.
		return -1;
	}

	if (!message || !*message || sscanf(message, "%11d %23[^\n]", &amount, atcmd_player_name) < 2) {
		clif_displaymessage(fd, "Invalid value (@gcash <amount> <character name>");
		return -1;
	}
	
	if ((tsd = map_nick2sd(atcmd_player_name,false)) == NULL) {
		clif_displaymessage(fd, msg_txt(sd,3)); // Character not found.
		return -1;
	}

	if (sd->cashPoints < amount || amount < 0) {
        clif_displaymessage(fd, "Not enough Cash Points.");
        return -1;
    }
    
    // Se realiza la transferencia de Cash Points.
	if( (ret=pc_paycash(sd, amount, 0, LOG_TYPE_COMMAND)) >= 0)
	{
		sprintf(output, msg_txt(sd,410), ret, sd->cashPoints); // Removed %d cash points. Total %d points.
		clif_messagecolor(&sd->bl, color_table[COLOR_LIGHT_GREEN], output, false, SELF);
	}
		
	if( (ret=pc_getcash(tsd, amount, 0, LOG_TYPE_COMMAND)) >= 0)
	{
		sprintf(output, msg_txt(sd,505), ret, tsd->cashPoints); // Gained %d cash points. Total %d points.
		clif_messagecolor(&tsd->bl, color_table[COLOR_LIGHT_GREEN], output, false, SELF);
	}

    return 0;
}

ACMD_FUNC(myinfo)
{
	char output[CHAT_SIZE_MAX];
	uint32 ip;
	int i;
	time_t now=time(NULL);

	struct {
		const char* format;
		int value;
	} output_table[] = {
		{ "Cash Points - %d", 0 },
		{ "Free/Kafra Points - %d", 0 },
		{ "Vote Points - %d", 0 },
		{ "MvP - %d", 0 },
		{ NULL, 0 }
	};

	memset(output, '\0', sizeof(output));

	//direct array initialization with variables is not standard C compliant.
	output_table[0].value = sd->cashPoints;
	output_table[1].value = sd->kafraPoints;
	output_table[2].value = static_cast<int>(pc_readaccountreg(sd, add_str("#VOTEPOINTS")));
	output_table[3].value = static_cast<int>(pc_readaccountreg(sd, add_str("#MVPRank")));

	sprintf(output, "'%s' Info:", sd->status.name); // '%s' stats:
	clif_displaymessage(fd,output);

	for( i = 0; output_table[i].format != NULL; i++ )
	{
		sprintf(output,output_table[i].format,output_table[i].value);
		clif_displaymessage(fd,output);
	}

	if ( pc_isvip(sd) )
	{
		int year,month,day,hour,minute,second;
		char timestr[21];
		
		split_time((int)(sd->vip.time-now),&year,&month,&day,&hour,&minute,&second);
		sprintf(atcmd_output,msg_txt(sd,705),year,month,day,hour,minute); // Your VIP status is valid for %d years, %d months, %d days, %d hours and %d minutes.
		clif_displaymessage(sd->fd,atcmd_output);
		timestamp2string(timestr,20,sd->vip.time,"%Y-%m-%d %H:%M");
		sprintf(atcmd_output,msg_txt(sd,707),timestr); // You are VIP until : %s
		clif_displaymessage(sd->fd,atcmd_output);
	}

	ip = session[sd->fd]->client_addr;
	sprintf(output,"IP Address - %d.%d.%d.%d",CONVIP(ip));
	clif_displaymessage(fd,output);

	return 0;
}