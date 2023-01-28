// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config structure
 * Place the definition of custom battle configuration structure members in this file.
 *
 * Make sure that the name you want is not already taken by looking into battle.hpp
 * Do not forget to add initilization logic to battle_config_init.inc
 *
 * Format:
 *	 <datatype> name;
 **/

	int bg_idle_announce;
	int bg_idle_autokick;
	int bg_logincount_check;
	int bg_reserved_char_id;
	int woe_reserved_char_id;
	int instance_reserved_char_id;
	int bg_can_trade;
	int bg_party_skills;
	int instance_penalty_show;
	int instance_buff_show;
	int extended_vending;
	int show_broadcast_info;
	int show_item_vending;
	int ex_vending_info;
	int ex_vending_report;
	int ex_buying_bound;
	int item_zeny;
	int item_cash;
	int feature_attendance_close;
	// MvP, Mini-Boss and Normal Card Ad and Log System [Bad]
	int announcement_and_log_system;
	int normal_card_announce_system;
	int normal_card_announce;
	int dropped_normal_card_log;
	int set_normal_announce_color;
	int mvp_card_announce_system;
	int mvp_card_announce;
	int dropped_mvp_card_log;
	int set_mvp_announce_color;
	int mini_boss_card_announce_system;
	int mini_boss_card_announce;
	int dropped_mini_boss_card_log;
	int set_mini_boss_announce_color;
	int bg_extended_check_equip;