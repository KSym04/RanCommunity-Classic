#pragma once

#include "GLContrlBaseMsg.h"
#include "GLContrlWoeMsgEnum.h"
#include "GLPVPWoeData.h"

enum ENUM_WOE_A2C_TOWER_WINNER
{
	ENUM_WOE_A2C_TOWER_WINNER_NONE		= 0,
	ENUM_WOE_A2C_TOWER_WINNER_FIRST		= 1,
	ENUM_WOE_A2C_TOWER_WINNER_SECOND	= 2
};
namespace GLMSG
{
	#pragma pack(1)

	struct SNET_WOE_A2FC_STATE_REGISTER	
	{
		NET_MSG_GENERIC			nmg;
		float		fTime;

		SNET_WOE_A2FC_STATE_REGISTER () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2FC_STATE_REGISTER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_A2FC_STATE_BATTLE
	{
		NET_MSG_GENERIC			nmg;
		float		fTime;
		WORD		wPlayerNum;

		SNET_WOE_A2FC_STATE_BATTLE () 
			: fTime(0.0f)
			, wPlayerNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2FC_STATE_BATTLE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_A2FC_STATE_REWARD
	{
		NET_MSG_GENERIC		nmg;
		float				fTime;
		
		SNET_WOE_A2FC_STATE_REWARD () 
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2FC_STATE_REWARD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_A2FC_STATE_ENDED
	{
		NET_MSG_GENERIC			nmg;

		SNET_WOE_A2FC_STATE_ENDED () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2FC_STATE_ENDED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_A2C_TOBATTLE_TIME
	{
		NET_MSG_GENERIC			nmg;
		WORD					wTime;

		SNET_WOE_A2C_TOBATTLE_TIME () 
			: wTime(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2C_TOBATTLE_TIME;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_A2FC_NEXTSCHED
	{
		NET_MSG_GENERIC			nmg;
		WOE_SCHED_NEXT		sScheduleNext;

		SNET_WOE_A2FC_NEXTSCHED () 
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2FC_NEXTSCHED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	
	struct SNET_WOE_F2A_BATTLEINFO_PC
	{
		NET_MSG_GENERIC			nmg;
		DWORD		dwCharID;
		BOOL		bWoeMap;

		SNET_WOE_F2A_BATTLEINFO_PC () 
			: dwCharID(GAEAID_NULL)
			, bWoeMap(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_F2A_BATTLEINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_A2C_BATTLEINFO_PC
	{
		NET_MSG_GENERIC			nmg;
		WOE_SCHED_NEXT			sScheduleNext;
		WOE_STATE				emState;
		float					fRemain;
		WORD					wLevelReq;
		WORD					wPlayerLimit;
		bool					bRegistered;

		SNET_WOE_A2C_BATTLEINFO_PC () 
			: emState(WOE_STATE_ENDED)
			, fRemain(0.0f)
			, wLevelReq(0)
			, wPlayerLimit(0)
			, bRegistered(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2C_BATTLEINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_C2A_REGISTER_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;
		WORD	wLevel;
		BOOL	bRegister;
		
		SNETPC_WOE_C2A_REGISTER_REQ () 
			: dwCharID(WOE_PLAYER_NULL)
			, wLevel(1)
			, bRegister(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_C2A_REGISTER_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2C_REGISTER_FB
	{
		NET_MSG_GENERIC		nmg;
		WOE_REGISTER_FB		emFB;

		SNETPC_WOE_A2C_REGISTER_FB()
			: emFB(WOE_REGISTER_FB_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2C_REGISTER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2F_MAP_MOVE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			nidMAP;
		DWORD				dwCharID;
		D3DXVECTOR3			vPos;
		BOOL				bCurMapCheck;

		SNETPC_WOE_A2F_MAP_MOVE () :
			nidMAP(false),
			dwCharID(GAEAID_NULL),
			vPos(0.0f,0.0f,0.0f),
			bCurMapCheck(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2F_MAP_MOVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_C2A_REJOIN_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwCharID;

		SNETPC_WOE_C2A_REJOIN_REQ ()
			: dwCharID(WOE_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_C2A_REJOIN_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2C_REJOIN_FB
	{
		NET_MSG_GENERIC		nmg;
		WOE_REJOIN_FB	emFB;

		SNETPC_WOE_A2C_REJOIN_FB()
			: emFB(WOE_REJOIN_FB_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2C_REJOIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_C2A_MAPEXIT_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_WOE_C2A_MAPEXIT_REQ ()
			: dwCharID(WOE_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_C2A_MAPEXIT_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2F_PLAYER_DATA
	{
		NET_MSG_GENERIC		nmg;
		WORD				wRankNum;
		WOE_REGISTER_DATA	sPlayerData[WOE_PACKET_PLAYER_DATA_NUM];

		SNETPC_WOE_A2F_PLAYER_DATA () 
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_A2F_PLAYER_DATA;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		BOOL ADDRANK ( const WOE_REGISTER_DATA& sRANK )
		{
			if ( wRankNum == WOE_PACKET_PLAYER_DATA_NUM )	return FALSE;
			sPlayerData[wRankNum] = sRANK;
			wRankNum++;

			nmg.dwSize = sizeof(*this) - sizeof(WOE_REGISTER_DATA) * ( WOE_PACKET_PLAYER_DATA_NUM - wRankNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void Reset()
		{
			wRankNum = 0;
			for( int i=0; i<WOE_PACKET_PLAYER_DATA_NUM; ++i ){
				sPlayerData[i] = WOE_REGISTER_DATA();
			}
		}
	};


	struct SNETPC_WOE_C2F_REVIVE_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_WOE_C2F_REVIVE_REQ ()
			: dwCharID(WOE_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_C2F_REVIVE_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_F2A_TOWER_CAPTURE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			sidMob;
		WORD				wClubID;

		SNETPC_WOE_F2A_TOWER_CAPTURE ()
			: sidMob(NATIVEID_NULL())
			, wClubID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_F2A_TOWER_CAPTURE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2F_TOWER_CAPTURE : public NET_MSG_GENERIC
	{
		DWORD	dwClubID;
		DWORD	dwMarkVer;
		char	szName[CLUB_NAME + 1];
		int		nCapturedFB;

		SNETPC_WOE_A2F_TOWER_CAPTURE()
			: dwClubID(CLUB_NULL)
			, dwMarkVer(0)
			, nCapturedFB(0)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_GCTRL_WOE_A2F_TOWER_CAPTURE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_F2C_TOWER_CAPTURE : public NET_MSG_GENERIC
	{
		DWORD	dwClubID;
		DWORD	dwMarkVer;
		char	szName[CLUB_NAME + 1];
		int		nCapturedFB;

		SNETPC_WOE_F2C_TOWER_CAPTURE ()
			: dwClubID(CLUB_NULL)
			, dwMarkVer(0)
			, nCapturedFB(0)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_GCTRL_WOE_F2C_TOWER_CAPTURE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	
	struct SNETPC_WOE_C2A_TOWER_OWNER_INFO_REQ
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;

		SNETPC_WOE_C2A_TOWER_OWNER_INFO_REQ ()
			: dwCharID(WOE_PLAYER_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_WOE_C2A_TOWER_INFO_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2C_TOWER_OWNER_INFO : public NET_MSG_GENERIC
	{
		WORD	wTowerGuild;
		DWORD	dwCurHolder;
		DWORD	dwCurMarkVer;
		char	szName[CLUB_NAME + 1];
		char	szLeader[CLUB_NAME + 1];

		SNETPC_WOE_A2C_TOWER_OWNER_INFO ()
			: dwCurHolder(0)
			, dwCurMarkVer(0)
			, wTowerGuild(CLUB_NULL)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			memset(szLeader, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_GCTRL_WOE_A2C_TOWER_INFO;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2F_TOWER_WINNER : public NET_MSG_GENERIC
	{
		DWORD	dwClubID;
		DWORD	dwMarkVer;
		char	szNamePrev[CLUB_NAME + 1];
		char	szNameCur[CLUB_NAME + 1];
		bool	bWinner;
		int		nCaptureFB;

		SNETPC_WOE_A2F_TOWER_WINNER ()
			: dwClubID(CLUB_NULL)
			, dwMarkVer(0)
			, bWinner(false)
			, nCaptureFB(0)
		{
			memset(szNamePrev, 0, sizeof(char) * CLUB_NAME + 1);
			memset(szNameCur, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_GCTRL_WOE_A2F_TOWER_WINNER;
			GASSERT(dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_MSG_WOE_A2C_TOWER_WINNER : public NET_MSG_GENERIC
	{
		ENUM_WOE_A2C_TOWER_WINNER	emFB;
		char						szNamePrev[CLUB_NAME + 1];
		char						szNameCur[CLUB_NAME + 1];

		SNET_MSG_WOE_A2C_TOWER_WINNER()
			: emFB(ENUM_WOE_A2C_TOWER_WINNER_NONE)
		{
			memset(szNamePrev, 0, sizeof(char) * CLUB_NAME + 1);
			memset(szNameCur, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_A2C_TOWER_WINNER;
		}
	};

	struct SNETPC_WOE_F2C_TOWER_WINNER : public NET_MSG_GENERIC
	{
		DWORD	dwClubID;
		DWORD	dwMarkVer;
		char	szNamePrev[CLUB_NAME + 1];
		char	szNameCur[CLUB_NAME + 1];
		int		nCapturedFB;

		SNETPC_WOE_F2C_TOWER_WINNER ()
			: dwClubID(CLUB_NULL)
			, dwMarkVer(0)
			, nCapturedFB(0)
		{
			memset(szNamePrev, 0, sizeof(char) * CLUB_NAME + 1);
			memset(szNameCur, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_GCTRL_WOE_F2C_TOWER_WINNER;
			GASSERT(dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_WOE_A2C_RANKINFO_PC : public NET_MSG_GENERIC
	{
		DWORD	dwClubID;
		DWORD	dwEmperiumMarkVer;
		char	szName[CLUB_NAME + 1];
		char	szLeader[CLUB_NAME + 1];
		
		SNETPC_WOE_A2C_RANKINFO_PC () 
			: dwClubID(CLUB_NULL)
			, dwEmperiumMarkVer(0)
		{
			memset(szName, 0, CLUB_NAME + 1);
			memset(szLeader, 0, CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_GCTRL_WOE_A2C_RANKINFO_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_MSG_WOE_RANKING_FLD : public NET_MSG_GENERIC
	{
		WORD	wClubID;
		WORD	wClubKiller;
		WORD	wClubKilled;

		SNET_MSG_WOE_RANKING_FLD()
			: wClubID(0)
			, wClubKiller(0)
			, wClubKilled(0)
		{
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_RANKING_FLD;
			GASSERT(dwSize <= NET_DATA_BUFSIZE);
		}
	};

	struct SNET_MSG_WOE_RANKING_AGT : public NET_MSG_GENERIC
	{
		WORD	wClubID;
		WORD	wClubKiller;
		WORD	wClubKilled;
		int		nIndex;

		SNET_MSG_WOE_RANKING_AGT()
			: nIndex(-1)
			, wClubID(0)
			, wClubKiller(0)
			, wClubKilled(0)
		{
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_RANKING_AGT;
			GASSERT(dwSize <= NET_DATA_BUFSIZE);
		}
	};

	struct SNET_MSG_WOE_RANKING_CLT : public NET_MSG_GENERIC
	{
		WORD				wClubID;
		WORD				wClubKiller;
		WORD				wClubKilled;
		char				szName[CLUB_NAME + 1];
		int					nIndex;

		SNET_MSG_WOE_RANKING_CLT()
			: nIndex(-1)
			, wClubID(0)
			, wClubKiller(0)
			, wClubKilled(0)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_RANKING_CLT;
			GASSERT(dwSize <= NET_DATA_BUFSIZE);
		}
	};
	struct SNET_WOE_GUILD_POINT_UPDATE
	{
		NET_MSG_GENERIC			nmg;

		bool					bKillPoint;

		SNET_WOE_GUILD_POINT_UPDATE()
			: bKillPoint(false)
		{
			nmg.dwSize = sizeof(SNET_WOE_GUILD_POINT_UPDATE);
			nmg.nType = NET_MSG_GCTRL_WOE_GUILD_POINT_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}
	};
	
	struct SNET_WOE_GUILD_MYRANK_UPDATE
	{
		NET_MSG_GENERIC			nmg;

		SWOE_GUILD_RANK_EX		sMyWoeGuildRank;

		SNET_WOE_GUILD_MYRANK_UPDATE()
		{
			nmg.dwSize = sizeof(SNET_WOE_GUILD_MYRANK_UPDATE);
			nmg.nType = NET_MSG_GCTRL_WOE_GUILD_MYRANK_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}
	};
	struct SNET_WOE_GUILD_RANKING_UPDATE
	{
		NET_MSG_GENERIC		nmg;

		WORD				wRankNum;
		SWOE_GUILD_RANK		sWoeGuildRank[RANKING_NUM];

		SNET_WOE_GUILD_RANKING_UPDATE()
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_WOE_GUILD_RANKING_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}

		bool ADDCLUB(const SWOE_GUILD_RANK& sRank)
		{
			if (RANKING_NUM == wRankNum)		return false;

			sWoeGuildRank[wRankNum] = sRank;

			++wRankNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(SWOE_GUILD_RANK) * wRankNum;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
			return true;
		}

		void RESET()
		{
			wRankNum = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
		}
	};
	struct SNET_WOE_GUILD_RANKING_REQ
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwMapID;

		SNET_WOE_GUILD_RANKING_REQ()
			: dwMapID(UINT_MAX)
		{
			nmg.dwSize = sizeof(SNET_WOE_GUILD_RANKING_REQ);
			nmg.nType = NET_MSG_GCTRL_WOE_GUILD_RANKING_REQ;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}

	};
	struct SNET_WOE_PLAYER_MYRANK_UPDATE
	{
		NET_MSG_GENERIC			nmg;

		SWOE_PLAYER_RANK_EX		sMySelfPlayerRank;

		SNET_WOE_PLAYER_MYRANK_UPDATE()
		{
			nmg.dwSize = sizeof(SNET_WOE_PLAYER_MYRANK_UPDATE);
			nmg.nType = NET_MSG_GCTRL_WOE_PLAYER_MYRANK_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_PLAYER_RANKING_UPDATE
	{
		NET_MSG_GENERIC		nmg;

		WORD				wRankNum;
		SWOE_PLAYER_RANK	sWoePlayerRank[RANKING_NUM];

		SNET_WOE_PLAYER_RANKING_UPDATE()
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_WOE_PLAYER_RANKING_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}

		bool ADDPLAYER(const SWOE_PLAYER_RANK& sPlayerRank)
		{
			if (RANKING_NUM == wRankNum)		return false;

			sWoePlayerRank[wRankNum] = sPlayerRank;

			++wRankNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(SWOE_PLAYER_RANK) * wRankNum;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
			return true;
		}

		void RESET()
		{
			wRankNum = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
		}
	};

	struct SNET_WOE_PLAYER_RANKING_REQ
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwMapID;

		SNET_WOE_PLAYER_RANKING_REQ()
			: dwMapID(UINT_MAX)
		{
			nmg.dwSize = sizeof(SNET_WOE_PLAYER_RANKING_REQ);
			nmg.nType = NET_MSG_GCTRL_WOE_PLAYER_RANKING_REQ;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}

	};
	
	struct SNET_WOE_RESU_MYRANK_UPDATE
	{
		NET_MSG_GENERIC			nmg;

		SWOE_RESU_RANK_EX		sMySelfResuRank;

		SNET_WOE_RESU_MYRANK_UPDATE()
		{
			nmg.dwSize = sizeof(SNET_WOE_RESU_MYRANK_UPDATE);
			nmg.nType = NET_MSG_GCTRL_WOE_RESU_MYRANK_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}
	};

	struct SNET_WOE_RESU_RANKING_UPDATE
	{
		NET_MSG_GENERIC		nmg;

		WORD				wRankNum;
		SWOE_RANK_RESU		sWoeResu[RANKING_NUM];

		SNET_WOE_RESU_RANKING_UPDATE()
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_WOE_RESU_RANKING_UPDATE;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}

		bool ADDPLAYER(const SWOE_RANK_RESU& sPlayerRank)
		{
			if (RANKING_NUM == wRankNum)		return false;

			sWoeResu[wRankNum] = sPlayerRank;

			++wRankNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(SWOE_RANK_RESU) * wRankNum;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
			return true;
		}

		void RESET()
		{
			wRankNum = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
		}
	};

	struct SNET_WOE_RESU_RANKING_REQ
	{
		NET_MSG_GENERIC		nmg;

		DWORD				dwMapID;

		SNET_WOE_RESU_RANKING_REQ()
			: dwMapID(UINT_MAX)
		{
			nmg.dwSize = sizeof(SNET_WOE_RESU_RANKING_REQ);
			nmg.nType = NET_MSG_GCTRL_WOE_RESU_RANKING_REQ;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}

	};

	struct SNET_MSG_WOE_A2F_DECLARE_WINNER : public NET_MSG_GENERIC
	{
		char	szName[CLUB_NAME + 1];

		SNET_MSG_WOE_A2F_DECLARE_WINNER()
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_A2F_DECLARE_WINNER;
		}
	};

	struct SNET_MSG_WOE_F2C_DECLARE_WINNER : public NET_MSG_GENERIC
	{
		char	szName[CLUB_NAME + 1];

		SNET_MSG_WOE_F2C_DECLARE_WINNER()
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_F2C_DECLARE_WINNER;
		}
	};

	struct SNET_MSG_WOE_A2C_DECLARE_WINNER : public NET_MSG_GENERIC
	{
		char	szName[CLUB_NAME + 1];

		SNET_MSG_WOE_A2C_DECLARE_WINNER()
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_A2C_DECLARE_WINNER;
		}
	};
	#pragma pack()
};

