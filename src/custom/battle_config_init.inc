// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config initializations
 * Place the definition of custom battle configuration initializations in this file.
 *
 * Do not forget to add a comma at the end of the line, if you want to add another config
 *
 * Format:
 *	 { "name", &battle_config.<variable name>, <default value>, <minimum value>, <maximum value> },
 **/

	{ "instance_penalty_show",				&battle_config.instance_penalty_show,			1,		0,		1,				},
	{ "instance_buff_show",            	 	&battle_config.instance_buff_show,         	 	1,		0,		1,  			},
	{ "extended_vending",					&battle_config.extended_vending,				1,		0,		1,				},
	{ "show_broadcast_info",				&battle_config.show_broadcast_info,				1,		0,		1,				},
	{ "show_item_vending",					&battle_config.show_item_vending,				1,		0,		1,				},
	{ "ex_vending_info",					&battle_config.ex_vending_info,					1,		0,		1,				},
	{ "ex_vending_report",					&battle_config.ex_vending_report,				1,		0,		1,				},
	{ "ex_buying_bound",					&battle_config.ex_buying_bound,					1,		0,		1,				},
	{ "item_zeny",							&battle_config.item_zeny,						0,		0,		INT32_MAX,		},
	{ "item_cash",							&battle_config.item_cash,						0,		0,		INT32_MAX,		},
	{ "feature.attendance_close",			&battle_config.feature_attendance_close,		1,		0,		1,				},
	{ "bg_idle_announce",               	&battle_config.bg_idle_announce,            	0,      0,      INT_MAX,        },
	{ "bg_idle_autokick",               	&battle_config.bg_idle_autokick,            	0,      0,      INT_MAX,        },
	{ "bg_logincount_check",            	&battle_config.bg_logincount_check,         	1,      0,      1,              },
	{ "bg_can_trade",				        &battle_config.bg_can_trade,                    1,      0,      1,              },
	{ "bg_reserved_char_id",                &battle_config.bg_reserved_char_id,             999996, 0,      INT_MAX,        },
	{ "woe_reserved_char_id",				&battle_config.woe_reserved_char_id,            999997, 0,      INT_MAX,        },
	{ "bg_party_skills",				    &battle_config.bg_party_skills,                 1,      0,      1,              },
	{ "instance_reserved_char_id",          &battle_config.instance_reserved_char_id,       999998, 0,      INT_MAX,        },
	// MvP, Mini-Boss and Normal Card Ad and Log System [Bad]
	{ "announcement_and_log_system",        &battle_config.announcement_and_log_system,     1,      0,      1,              },
	{ "normal_card_announce_system",        &battle_config.normal_card_announce_system,     1,      0,      1,              },
	{ "normal_card_announce",               &battle_config.normal_card_announce,            1,      0,      1,              },
	{ "dropped_normal_card_log",            &battle_config.dropped_normal_card_log,         1,      0,      1,              },
	{ "set_normal_announce_color",          &battle_config.set_normal_announce_color,       0xFFBB00,      0,INT_MAX,       },
	{ "mvp_card_announce_system",           &battle_config.mvp_card_announce_system,        1,      0,      1,              },
	{ "mvp_card_announce",                  &battle_config.mvp_card_announce,               1,      0,      1,              },
	{ "dropped_mvp_card_log",               &battle_config.dropped_mvp_card_log,            1,      0,      1,              },
	{ "set_mvp_announce_color",             &battle_config.set_mvp_announce_color,          0xFFBB00,      0,INT_MAX,       },
	{ "mini_boss_card_announce_system",     &battle_config.mini_boss_card_announce_system,  1,      0,      1,              },
	{ "mini_boss_card_announce",            &battle_config.mini_boss_card_announce,         1,      0,      1,              },
	{ "dropped_mini_boss_card_log",         &battle_config.dropped_mini_boss_card_log,      1,      0,      1,              },
	{ "set_mini_boss_announce_color",       &battle_config.set_mini_boss_announce_color,    0xFFBB00,      0,INT_MAX,       },