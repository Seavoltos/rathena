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