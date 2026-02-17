/*!
 * \file PVPPartyBattleGroundsMsg.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSMSG_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSMSG_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLContrlBaseMsg.h"
#include "./PVPPartyBattleGroundsData.h"

namespace GLMSG
{
#pragma pack(1)
	struct SNET_PVP_PBG_A2FC_STATE_REGISTER	
	{
		NET_MSG_GENERIC	nmg;
		float fTime;

		SNET_PVP_PBG_A2FC_STATE_REGISTER () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REGISTER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_PBG_A2FC_STATE_BATTLE
	{
		NET_MSG_GENERIC nmg;
		float fTime;

		SNET_PVP_PBG_A2FC_STATE_BATTLE () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_BATTLE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_PBG_A2FC_STATE_REWARD
	{
		NET_MSG_GENERIC nmg;
		float fTime;

		SNET_PVP_PBG_A2FC_STATE_REWARD () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REWARD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_PBG_A2FC_STATE_ENDED
	{
		NET_MSG_GENERIC	nmg;

		SNET_PVP_PBG_A2FC_STATE_ENDED () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_ENDED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_PBG_A2C_TOBATTLE_TIME
	{
		NET_MSG_GENERIC	nmg;
		WORD wTime;

		SNET_PVP_PBG_A2C_TOBATTLE_TIME () 
			: wTime(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_TOBATTLE_TIME;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_PBG_A2FC_NEXTSCHED
	{
		NET_MSG_GENERIC	nmg;
		PVPPBG::EVENT_SCHEDEDULE_NEXT	sScheduleNext;

		SNET_PVP_PBG_A2FC_NEXTSCHED () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2FC_NEXTSCHED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};



	struct SNET_PVP_PBG_F2A_BATTLEINFO_PC
	{
		NET_MSG_GENERIC	nmg;
		DWORD dwCharID;

		SNET_PVP_PBG_F2A_BATTLEINFO_PC () 
			: dwCharID(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2A_BATTLEINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_PBG_A2C_BATTLEINFO_PC
	{
		NET_MSG_GENERIC	nmg;
		PVPPBG::EVENT_SCHEDEDULE_NEXT sScheduleNext;
		PVPPBG::EVENT_STATE	emState;

		float fRemain;
		WORD wLevelReq;
		DWORD dwContriReq;
		WORD wPlayerLimit;
		
		WORD wBattleTime;
		BOOL bRegistered;
		BOOL bQueued;
		WORD wQueueNum;

		SNET_PVP_PBG_A2C_BATTLEINFO_PC () 
			: emState(PVPPBG::EVENT_STATE_ENDED)
			, fRemain(0.0f)
			, wLevelReq(0)
			, dwContriReq(0)
			, wPlayerLimit(0)
	
			, wBattleTime(0)
			, bRegistered(false)
			, bQueued(false)
			, wQueueNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_BATTLEINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_C2A_REGISTER_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;
		WORD	wLevel;
		DWORD	dwContri;
		BOOL	bRegister;

		TCHAR	szHASH[PVP_PBG_PLAYER_HASH_SIZE];

		SNETPC_PVP_PBG_C2A_REGISTER_REQ () 
			: dwCharID(PVP_PBG_CHAR_ID_NULL)
			, wLevel(1)
			, dwContri(0)
			, bRegister(FALSE)
		{
			SecureZeroMemory( szHASH, sizeof(TCHAR) * (PVP_PBG_PLAYER_HASH_SIZE) );

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_C2A_REGISTER_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	enum PVP_PBG_REGISTER_FB
	{
		PVP_PBG_REGISTER_FB_FAILED			= 0,
		PVP_PBG_REGISTER_FB_NOTPOSSIBLE		= 1,
		PVP_PBG_REGISTER_FB_INVALID_CHANNEL	= 2,
		PVP_PBG_REGISTER_FB_INVALID_CHARID	= 3,
		PVP_PBG_REGISTER_FB_LEVEL_LOW		= 4,
		PVP_PBG_REGISTER_FB_CONTRI_LOW		= 5,
		PVP_PBG_REGISTER_FB_ALREADY_REG		= 6,
		PVP_PBG_REGISTER_FB_ALREADY_QUEUE	= 7,
		PVP_PBG_REGISTER_FB_QUEUED			= 8,
		PVP_PBG_REGISTER_FB_REGISTERED		= 9,
		PVP_PBG_REGISTER_FB_CANCEL_REG		= 10,
		PVP_PBG_REGISTER_FB_CANCEL_FAIL		= 11,
		

		PVP_PBG_REGISTER_FB_DEVICE_REGISTERED	= 12,
		PVP_PBG_REGISTER_FB_DEVICE_INFO_ERROR	= 13,
	};

	struct SNETPC_PVP_PBG_A2C_REGISTER_FB
	{
		NET_MSG_GENERIC		nmg;
		PVP_PBG_REGISTER_FB emFB;
		WORD				wQueueNum;

		SNETPC_PVP_PBG_A2C_REGISTER_FB()
			: emFB(PVP_PBG_REGISTER_FB_FAILED)
			, wQueueNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_REGISTER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_C2A_REJOIN_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_PBG_C2A_REJOIN_REQ ()
			: dwCharID(SCHOOLWARS_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_C2A_REJOIN_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	enum PVP_PBG_REJOIN_FB
	{
		PVP_PBG_REJOIN_FB_FAILED			= 0,
		PVP_PBG_REJOIN_FB_NOTPOSSIBLE		= 1,
		PVP_PBG_REJOIN_FB_NOT_REGISTERED	= 2,
		PVP_PBG_REJOIN_FB_INVALID_CHARID	= 3,
		PVP_PBG_REJOIN_FB_INBATTLE			= 4,
	};

	struct SNETPC_PVP_PBG_A2C_REJOIN_FB
	{
		NET_MSG_GENERIC		nmg;
		PVP_PBG_REJOIN_FB	emFB;

		SNETPC_PVP_PBG_A2C_REJOIN_FB()
			: emFB(PVP_PBG_REJOIN_FB_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_REJOIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_A2C_QUEUE_MOVED
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PVP_PBG_A2C_QUEUE_MOVED ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_QUEUE_MOVED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_A2C_QUEUE_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		WORD				wQueueNum;

		SNETPC_PVP_PBG_A2C_QUEUE_UPDATE ()
			: wQueueNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_QUEUE_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_A2F_PLAYER_DATA
	{
		NET_MSG_GENERIC		nmg;
		WORD				wRankNum;
		PVPPBG::REGISTER_DATA	sPlayerData[PVP_PBG_PACKET_PLAYER_DATA_NUM];

		SNETPC_PVP_PBG_A2F_PLAYER_DATA () 
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2F_PLAYER_DATA;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		BOOL ADDRANK ( const PVPPBG::REGISTER_DATA& sRANK )
		{
			if ( wRankNum == PVP_PBG_PACKET_PLAYER_DATA_NUM )	
				return FALSE;

			sPlayerData[wRankNum] = sRANK;
			wRankNum++;

			nmg.dwSize = sizeof(*this) - sizeof(PVPPBG::REGISTER_DATA) * ( PVP_PBG_PACKET_PLAYER_DATA_NUM - wRankNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void Reset()
		{
			wRankNum = 0;
			for( int i=0; i<PVP_PBG_PACKET_PLAYER_DATA_NUM; ++i )
			{
				sPlayerData[i] = PVPPBG::REGISTER_DATA();
			}
		}
	};

	struct SNETPC_PVP_PBG_A2F_MAP_MOVE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID	nidMAP;
		DWORD		dwCharID;
		D3DXVECTOR3	vPos;
		BOOL		bCurMapCheck;
		BOOL		bSourceMapCheck;
		SNATIVEID	nidSourceLobbyMAP;
		SNATIVEID	nidSourceBattleMAP;

		SNETPC_PVP_PBG_A2F_MAP_MOVE ()
			: nidMAP(false)
			, dwCharID(GAEAID_NULL)
			, vPos(0.0f,0.0f,0.0f)
			, bCurMapCheck(FALSE)
			, bSourceMapCheck(FALSE)
			, nidSourceLobbyMAP(false)
			, nidSourceBattleMAP(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2F_MAP_MOVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_PVP_PBG_F2C_PLAYER_SCORE_UPDATE
	{
		NET_MSG_GENERIC		nmg;

		PVPPBG::PLAYER_DATA PlayerData;

		SNETPC_PVP_PBG_F2C_PLAYER_SCORE_UPDATE ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2C_PLAYER_SCORE_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_C2F_SCORE_INFO_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_PBG_C2F_SCORE_INFO_REQ ()
			: dwCharID(PVP_PBG_CHAR_ID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_C2F_PLAYER_SCORE_INFO_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_C2AF_REVIVE_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_PBG_C2AF_REVIVE_REQ ()
			: dwCharID(PVP_PBG_CHAR_ID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_C2AF_REVIVE_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_A2F_REVIVE_REQ_FIELD_OUT
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PVP_PBG_A2F_REVIVE_REQ_FIELD_OUT ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2F_FIELD_OUT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_F2A_REVIVE_REQ_FIELD_OUT_FB
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PVP_PBG_F2A_REVIVE_REQ_FIELD_OUT_FB ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2A_FIELD_OUT_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB
	{
		NET_MSG_GENERIC		nmg;

		SNATIVEID			sMapID;
		D3DXVECTOR3			vPos;

		WORD				wNowHP;
		WORD				wNowMP;
		WORD				wNowSP;

		SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB () :
			vPos(0,0,0),
			wNowHP(0),
			wNowMP(0),
			wNowSP(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2AC_REVIVE_REQ_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_F2C_RANKING_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		WORD	wRankNum;
		PVPPBG::PLAYER_DATA	sPlayerData[PVP_PBG_PACKET_RANK_NUM];

		SNETPC_PVP_PBG_F2C_RANKING_UPDATE () 
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2C_RANKING_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		BOOL ADDRANK ( const PVPPBG::PLAYER_DATA& sRANK )
		{
			if ( wRankNum == PVP_PBG_PACKET_RANK_NUM )	
				return FALSE;

			sPlayerData[wRankNum] = sRANK;
			wRankNum++;

			nmg.dwSize = sizeof(*this) - sizeof(PVPPBG::PLAYER_DATA) * ( PVP_PBG_PACKET_RANK_NUM - wRankNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void Reset()
		{
			wRankNum = 0;
			for( int i=0; i<PVP_PBG_PACKET_RANK_NUM; ++i )
			{
				sPlayerData[i] = PVPPBG::PLAYER_DATA();
			}
		}
	};

	struct SNETPC_PVP_PBG_F2C_RANKING_END
	{
		NET_MSG_GENERIC		nmg;
		WORD	wPlayerNum;

		SNETPC_PVP_PBG_F2C_RANKING_END ()
			: wPlayerNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2C_RANKING_END;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_F2A_RANKINFO_PC
	{
		NET_MSG_GENERIC		nmg;
		WORD				wRankNum;
		PVPPBG::PLAYER_DATA	sPlayerData[PVP_PBG_MINI_RANKING_NUM];

		SNETPC_PVP_PBG_F2A_RANKINFO_PC () 
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_F2A_RANKINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		bool ADDRANK ( const PVPPBG::PLAYER_DATA& sRANK )
		{
			if ( PVP_PBG_MINI_RANKING_NUM==wRankNum )		
				return false;

			sPlayerData[wRankNum] = sRANK;

			++wRankNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(PVPPBG::PLAYER_DATA)*wRankNum;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			return true;
		}
	};

	struct SNETPC_PVP_PBG_A2C_RANKINFO_PC
	{
		NET_MSG_GENERIC		nmg;

		PVPPBG::PLAYER_DATA	sPlayerData[PVP_PBG_MINI_RANKING_NUM];

		SNETPC_PVP_PBG_A2C_RANKINFO_PC () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_A2C_RANKINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_PBG_C2A_MAPEXIT_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_PBG_C2A_MAPEXIT_REQ ()
			: dwCharID(SCHOOLWARS_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_PBG_C2A_MAPEXIT_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


#pragma pack()
}

#endif // PVPPARTYBATTLEGROUNDSMSG_H_INCLUDED__
