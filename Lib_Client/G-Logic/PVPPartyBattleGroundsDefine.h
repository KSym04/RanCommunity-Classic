/*!
 * \file PVPPartyBattleGroundsDefine.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSDEFINE_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"

#define PVP_PBG_CHAR_ID_NULL		0xFFFFFFFF	//similar to GAEAID_NULL
#define PVP_PBG_CHAR_NAME_SIZE		33			//similar to CHAR_SZNAME
#define PVP_PBG_PARTY_NULL			0xFFFFFFFF

#define PVP_PBG_SCHEDULE_NONE		0xFFFFFFFF
#define PVP_PBG_PLAYER_HASH_SIZE	128	
#define PVP_PBG_PACKET_PLAYER_DATA_NUM	32		//agent to field player data

#define PVP_PBG_REVIVE_TIME			15.0f		//revive time
#define PVP_PBG_PACKET_RANK_NUM		30			//field to client ranking data
#define PVP_PBG_MINI_RANKING_NUM	10			//top rankings size

namespace PVPPBG
{

	enum EVENT_STATE
	{
		EVENT_STATE_ENDED		= 0,
		EVENT_STATE_REGISTER	= 1,
		EVENT_STATE_BATTLE		= 2,
		EVENT_STATE_REWARD		= 3,
		EVENT_STATE_SIZE		= 4,
	};

	enum 
	{
		SCHOOL_SG	= 0,
		SCHOOL_MP	= 1,
		SCHOOL_PHX	= 2,
		SCHOOL_SIZE	= 3,
	};

	extern std::string STRING_EVENT_STATE[PVPPBG::EVENT_STATE_SIZE];
};


#endif // PVPPARTYBATTLEGROUNDSDEFINE_H_INCLUDED__
