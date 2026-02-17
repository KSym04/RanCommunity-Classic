/*!
 * \file PVPClubDeathMatchControlMsg.h
 *
 * \author Juver
 * \date 2019/09/17
 *
 * 
 */

#ifndef PVPCLUBDEATHMATCHCONTROLMSG_H_INCLUDED__
#define PVPCLUBDEATHMATCHCONTROLMSG_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLContrlBaseMsg.h"
#include "./PVPClubDeathMatchData.h"
#include "./PVPClubDeathMatchControlMsgEnum.h"

namespace GLMSG
{
#pragma pack(1)

	struct SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER	
	{
		NET_MSG_GENERIC			nmg;
		float		fTime;

		SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE
	{
		NET_MSG_GENERIC			nmg;
		float		fTime;
		DWORD		dwClubNum;

		SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE () 
			: fTime(0.0f)
			, dwClubNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD
	{
		NET_MSG_GENERIC			nmg;
		float		fTime;

		SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED
	{
		NET_MSG_GENERIC			nmg;

		SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME
	{
		NET_MSG_GENERIC			nmg;
		WORD					wTime;

		SNET_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME () 
			: wTime(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED
	{
		NET_MSG_GENERIC			nmg;
		PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT	sScheduleNext;

		SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC
	{
		NET_MSG_GENERIC			nmg;
		DWORD		dwCharID;
		BOOL		bLobbyMap;
		BOOL		bBattleMap;

		SNET_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC () 
			: dwCharID(GAEAID_NULL)
			, bLobbyMap(FALSE)
			, bBattleMap(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC
	{
		NET_MSG_GENERIC				nmg;
		PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT	sScheduleNext;
		PVP_CLUB_DEATH_MATCH_EVENT_STATE		emState;
		float fRemain;

		WORD wLevelReq;
		WORD wClubLevelLimit;
		WORD wClubPlayerMin;
		WORD wClubPlayerMax;
		bool bClubPlayerReqCDMRights;

		WORD wBattleTime;

		bool bRegistered;
		WORD wClubParticipants;

		PVP_CLUB_DEATH_MATCH_REWARD_DATA sReward[PVP_CLUB_DEATH_MATCH_RANK_TOP];

		SNET_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC () 
			: emState(PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED)
			, fRemain(0.0f)

			, wLevelReq(1)
			, wClubLevelLimit(1)
			, wClubPlayerMin(1)
			, wClubPlayerMax(1)
			, bClubPlayerReqCDMRights(true)

			, wBattleTime(1)

			, bRegistered(false)
			, wClubParticipants(0)
		{
			for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
			{
				sReward[i] = PVP_CLUB_DEATH_MATCH_REWARD_DATA();
			}

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNET_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM
	{
		NET_MSG_GENERIC		nmg;
		WORD				wClubParticipants;

		SNETPC_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM()
			: wClubParticipants(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;
		WORD	wLevel;
		BOOL	bRegister;

		/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
		TCHAR	szHASH[PVP_CLUB_DEATH_MATCH_PLAYER_HASH_SIZE];

		SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ () 
			: dwCharID(PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL)
			, wLevel(1)
			, bRegister(FALSE)
		{
			/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
			SecureZeroMemory( szHASH, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_PLAYER_HASH_SIZE) );

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB
	{
		NET_MSG_GENERIC		nmg;
		PVP_CLUB_DEATH_MATCH_REGISTER_FB emFB;
		WORD				wClubParticipants;

		SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB()
			: emFB(PVP_CLUB_DEATH_MATCH_REGISTER_FB_FAILED)
			, wClubParticipants(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID	nidMAP;
		DWORD		dwCharID;
		D3DXVECTOR3	vPos;
		BOOL		bCurMapCheck;
		BOOL		bSourceMapCheck;
		SNATIVEID	nidSourceLobbyMAP;
		SNATIVEID	nidSourceBattleMAP;

		SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE ()
			: nidMAP(false)
			, dwCharID(GAEAID_NULL)
			, vPos(0.0f,0.0f,0.0f)
			, bCurMapCheck(FALSE)
			, bSourceMapCheck(FALSE)
			, nidSourceLobbyMAP(false)
			, nidSourceBattleMAP(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ ()
			: dwCharID(PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB
	{
		NET_MSG_GENERIC		nmg;
		PVP_CLUB_DEATH_MATCH_REJOIN_FB	emFB;

		SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB()
			: emFB(PVP_CLUB_DEATH_MATCH_REJOIN_FB_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ ()
			: dwCharID(PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA
	{
		NET_MSG_GENERIC		nmg;

		DWORD	dwClubID;
		TCHAR	szClubName[PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE];
		TCHAR	szMasterName[PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE];
		DWORD	dwClubMarkVer;

		WORD	wRankNum;
		PVP_CLUB_DEATH_MATCH_REGISTER_DATA	sPlayerData[PVP_CLUB_DEATH_MATCH_PACKET_PLAYER_DATA_NUM];

		SNETPC_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA () 
			: dwClubID(PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL)
			, dwClubMarkVer(0)
			, wRankNum(0)
		{
			memset( szClubName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE) );
			memset( szMasterName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE) );

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA;

			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA)<=NET_DATA_BUFSIZE);
		}

		BOOL ADDRANK ( const PVP_CLUB_DEATH_MATCH_REGISTER_DATA& sRANK )
		{
			if ( wRankNum == PVP_CLUB_DEATH_MATCH_PACKET_PLAYER_DATA_NUM )	return FALSE;
			sPlayerData[wRankNum] = sRANK;
			wRankNum++;

			nmg.dwSize = sizeof(*this) - sizeof(PVP_CLUB_DEATH_MATCH_REGISTER_DATA) * ( PVP_CLUB_DEATH_MATCH_PACKET_PLAYER_DATA_NUM - wRankNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void RESETRANK()
		{
			wRankNum = 0;
			for( int i=0; i<PVP_CLUB_DEATH_MATCH_PACKET_PLAYER_DATA_NUM; ++i )
			{
				sPlayerData[i] = PVP_CLUB_DEATH_MATCH_REGISTER_DATA();
			}
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ ()
			: dwCharID(PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB
	{
		NET_MSG_GENERIC		nmg;

		SNATIVEID			sMapID;
		D3DXVECTOR3			vPos;

		WORD				wNowHP;
		WORD				wNowMP;
		WORD				wNowSP;

		SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB () :
			vPos(0,0,0),
			wNowHP(0),
			wNowMP(0),
			wNowSP(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2F_REVIVE_REQ_FIELD_OUT
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PVP_CLUB_DEATH_MATCH_A2F_REVIVE_REQ_FIELD_OUT ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_FIELD_OUT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_F2A_REVIVE_REQ_FIELD_OUT_FB
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PVP_CLUB_DEATH_MATCH_F2A_REVIVE_REQ_FIELD_OUT_FB ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_FIELD_OUT_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE
	{
		NET_MSG_GENERIC		nmg;

		BOOL	bFinal;
		WORD	wDataNum;
		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK	sClubData[PVP_CLUB_DEATH_MATCH_PACKET_CLUB_DATA_NUM];

		SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE () 
			: bFinal(FALSE)
			, wDataNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE;

			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE)<=NET_DATA_BUFSIZE);
		}

		BOOL ADD ( const PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData )
		{
			if ( wDataNum == PVP_CLUB_DEATH_MATCH_PACKET_CLUB_DATA_NUM )	return FALSE;
			sClubData[wDataNum] = sData;
			wDataNum++;

			nmg.dwSize = sizeof(*this) - sizeof(PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK) * ( PVP_CLUB_DEATH_MATCH_PACKET_CLUB_DATA_NUM - wDataNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void RESET()
		{
			wDataNum = 0;
			for( int i=0; i<PVP_CLUB_DEATH_MATCH_PACKET_CLUB_DATA_NUM; ++i )
			{
				sClubData[i] = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();
			}
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END
	{
		NET_MSG_GENERIC		nmg;
		DWORD		dwRankNum;
		BOOL		bFinal;

		SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END ()
			: dwRankNum(0)
			, bFinal(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		
		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK sFirstRank;
		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK sMyRank;

		BOOL	bDoublePoint;

		SNETPC_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE ()
			: bDoublePoint(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ ()
			: dwCharID(CAPTURE_THE_FLAG_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO
	{
		NET_MSG_GENERIC		nmg;
		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK	sData[PVP_CLUB_DEATH_MATCH_RANK_TOP];

		SNETPC_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		BOOL		bDoublePoint;

		SNETPC_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE ()
			: bDoublePoint(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	


#pragma pack()
};


#endif // PVPCLUBDEATHMATCHCONTROLMSG_H_INCLUDED__
