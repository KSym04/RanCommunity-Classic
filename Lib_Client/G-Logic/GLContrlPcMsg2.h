#pragma once

#include "./GLContrlBaseMsg.h"
#include "./GLContrlPcMsgEnum.h"
#include "./GLPartyFinderDef.h"


#include "./GLPKMark.h"

#include <boost/static_assert.hpp>

namespace GLMSG
{
	#pragma pack(1)

	/*pk info, Juver, 2017/11/17 */
	struct SNETPC_UPDATE_PK_KILL
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwVal;
		char				szName[CHAR_SZNAME];
		
		SNETPC_UPDATE_PK_KILL ()
			: dwVal(0)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_PK_KILL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*pk info, Juver, 2017/11/17 */
	struct SNETPC_UPDATE_PK_DEATH
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwVal;
		char				szName[CHAR_SZNAME];

		SNETPC_UPDATE_PK_DEATH ()
			: dwVal(0)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_PK_DEATH;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	/*Top Resu, Mhundz */
	struct SNETPC_UPDATE_RESU_SCORE2
	{
		NET_MSG_GENERIC		nmg;
		DWORD				wVal;

		SNETPC_UPDATE_RESU_SCORE2 ()
			: wVal(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_RESU_SCORE2;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	/*Top MMR, Mhundz */
	struct SNETPC_UPDATE_MMR_SCORE
	{
		NET_MSG_GENERIC		nmg;
		DWORD				wVal;

		SNETPC_UPDATE_MMR_SCORE ()
			: wVal(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_MMR_SCORE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*change scale card, Juver, 2018/01/04 */
	struct SNETPC_INVEN_SCALE_CHANGE
	{
		NET_MSG_GENERIC		nmg;
		float				fScale;

		SNETPC_INVEN_SCALE_CHANGE () 
			: fScale(1.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_SCALE_CHANGE;
		}
	};

	/*change scale card, Juver, 2018/01/04 */
	struct SNETPC_INVEN_SCALE_CHANGE_FB
	{
		NET_MSG_GENERIC		nmg;

		EMFB_CHANGE_SCALE	emFB;
		float				fScale;

		SNETPC_INVEN_SCALE_CHANGE_FB ()
			: emFB(EMFB_CHANGE_SCALE_FAIL)
			, fScale(1.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_SCALE_CHANGE_FB;
		}
	};

	/*change scale card, Juver, 2018/01/04 */
	struct SNETPC_INVENSCALE_CHANGE_BRD : public SNETPC_BROAD
	{
		float		fScale;

		SNETPC_INVENSCALE_CHANGE_BRD ()
			: fScale(1.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_SCALE_CHANGE_BRD;
		}
	};

	/*item color, Juver, 2018/01/10 */
	struct SNETPC_INVEN_ITEMCOLOR_CHANGE
	{
		NET_MSG_GENERIC			nmg;

		EMSLOT	emSlot;
		WORD	wColor1;
		WORD	wColor2;

		SNETPC_INVEN_ITEMCOLOR_CHANGE () 
			: emSlot(SLOT_TSIZE)
			, wColor1(0)
			, wColor2(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE;
		}
	};

	/*item color, Juver, 2018/01/10 */
	struct SNETPC_INVEN_ITEMCOLOR_CHANGE_FB
	{
		NET_MSG_GENERIC			nmg;
		EMFB_ITEM_COLOR_CHANGE	emFB;
		EMSLOT	emSlot;
		WORD	wColor1;
		WORD	wColor2;

		SNETPC_INVEN_ITEMCOLOR_CHANGE_FB ()
			: emFB(EMFB_ITEM_COLOR_CHANGE_FAILED)
			, emSlot(SLOT_TSIZE)
			, wColor1(0)
			, wColor2(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_FB;
		}
	};

	/*item color, Juver, 2018/01/10 */
	struct SNETPC_INVEN_ITEMCOLOR_CHANGE_BRD : public SNETPC_BROAD
	{
		EMSLOT	emSlot;
		WORD	wColor1;
		WORD	wColor2;

		SNETPC_INVEN_ITEMCOLOR_CHANGE_BRD ()
			: emSlot(SLOT_TSIZE)
			, wColor1(0)
			, wColor2(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_BRD;
		}
	};

	/*item wrapper, Juver, 2018/01/12 */
	struct SNETPC_INVEN_WRAP
	{
		NET_MSG_GENERIC	nmg;
		WORD			wPosX;
		WORD			wPosY;

		SNETPC_INVEN_WRAP () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_WRAP;
		}
	};

	/*item wrapper, Juver, 2018/01/12 */
	struct SNETPC_INVEN_WRAP_FB
	{
		NET_MSG_GENERIC		nmg;
		EMFB_ITEM_WRAP		emFB;
		
		SNETPC_INVEN_WRAP_FB ()
			: emFB(EMFB_ITEM_WRAP_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_WRAP_FB;
		}
	};

	/*item wrapper, Juver, 2018/01/12 */
	struct SNETPC_INVEN_UNWRAP
	{
		NET_MSG_GENERIC	nmg;
		WORD			wPosX;
		WORD			wPosY;

		SNETPC_INVEN_UNWRAP () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_UNWRAP;
		}
	};

	/*item wrapper, Juver, 2018/01/12 */
	struct SNETPC_INVEN_UNWRAP_FB
	{
		NET_MSG_GENERIC		nmg;
		EMFB_ITEM_UNWRAP	emFB;

		SNETPC_INVEN_UNWRAP_FB ()
			: emFB(EMFB_ITEM_UNWRAP_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_UNWRAP_FB;
		}
	};

	/*change school card, Juver, 2018/01/12 */
	struct SNETPC_INVEN_CHANGE_SCHOOL
	{
		NET_MSG_GENERIC	nmg;
		WORD			wSchool;

		SNETPC_INVEN_CHANGE_SCHOOL () 
			: wSchool(0)
		{
			nmg.dwSize = (DWORD) sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_CHANGE_SCHOOL;
		}
	};

	/*change school card, Juver, 2018/01/12 */
	struct SNETPC_INVEN_CHANGE_SCHOOL_FB
	{
		NET_MSG_GENERIC		nmg;
		EMFB_CHANGE_SCHOOL	emFB;

		SNETPC_INVEN_CHANGE_SCHOOL_FB ()
			: emFB(EMFB_CHANGE_SCHOOL_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_CHANGE_SCHOOL_FB;
		}
	};

	/*item transfer card, Juver, 2018/01/18 */
	struct SNETPC_INVEN_TRANSFER_STATS
	{
		NET_MSG_GENERIC		nmg;
		SINVEN_POS			sInvenPosOLD;
		SINVEN_POS			sInvenPosNEW;

		SNETPC_INVEN_TRANSFER_STATS ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_TRANSFER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*item transfer card, Juver, 2018/01/18 */
	struct SNETPC_INVEN_TRANSFER_STATS_FB
	{
		NET_MSG_GENERIC		nmg;
		EMFB_ITEM_TRANSFER	emFB;

		SNETPC_INVEN_TRANSFER_STATS_FB ()
			: emFB(EMFB_ITEM_TRANSFER_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_TRANSFER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_INVEN_OPEN
	{
		NET_MSG_GENERIC		nmg;
		char		szPass[MAX_PATH];

		SNETPC_GM_COMMAND_INVEN_OPEN ()
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_INVEN_OPEN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_INVEN_OPEN_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_INVEN_OPEN_FB	emFB;
		WORD	wInvenLine;

		SNETPC_GM_COMMAND_INVEN_OPEN_FB ()
			: emFB(EMREQ_GM_INVEN_OPEN_FB_FAIL)
			, wInvenLine(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_INVEN_OPEN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_SKILL
	{
		NET_MSG_GENERIC		nmg;
		BOOL	bALL;
		SNATIVEID sidSkill;
		char	szPass[MAX_PATH];

		SNETPC_GM_COMMAND_GET_SKILL ()
			: bALL(FALSE)
			, sidSkill(false)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_SKILL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_SKILL_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_SKILL_FB		emFB;

		SNETPC_GM_COMMAND_GET_SKILL_FB ()
			: emFB(EMREQ_GM_SKILL_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_SKILL_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_SKILL2_FB
	{
		NET_MSG_GENERIC			nmg;
		SNATIVEID		sidSkill;
		WORD			wLevel;

		SNETPC_GM_COMMAND_GET_SKILL2_FB ()
			: sidSkill(false)
			, wLevel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_SKILL2_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_LEVEL
	{
		NET_MSG_GENERIC		nmg;
		WORD	wLevel;
		char	szPass[MAX_PATH];

		SNETPC_GM_COMMAND_GET_LEVEL ()
			: wLevel(0)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_LEVEL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_LEVEL_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_LEVEL_FB		emFB;
		WORD	wLevel;

		SNETPC_GM_COMMAND_GET_LEVEL_FB ()
			: emFB(EMREQ_GM_LEVEL_FB_FAIL)
			, wLevel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_LEVEL_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_STATS
	{
		NET_MSG_GENERIC		nmg;
		EMSTATS emStat;
		WORD	wNum;
		char	szPass[MAX_PATH];

		SNETPC_GM_COMMAND_GET_STATS ()
			: emStat(EMSIZE)
			, wNum(0)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_STATS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_STATS_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_STATS_FB		emFB;
		SCHARSTATS				sStats;	

		SNETPC_GM_COMMAND_GET_STATS_FB ()
			: emFB(EMREQ_GM_STATS_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_STATS_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_STATSP
	{
		NET_MSG_GENERIC		nmg;
		WORD	wNum;
		char	szPass[MAX_PATH];

		SNETPC_GM_COMMAND_GET_STATSP ()
			: wNum(0)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_STATSP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_STATSP_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_STATSP_FB		emFB;
		WORD	wStatsP;

		SNETPC_GM_COMMAND_GET_STATSP_FB ()
			: wStatsP(0)
			, emFB(EMREQ_GM_STATSP_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_STATSP_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_SKILLP
	{
		NET_MSG_GENERIC		nmg;
		WORD	wNum;
		char	szPass[MAX_PATH];

		SNETPC_GM_COMMAND_GET_SKILLP ()
			: wNum(0)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_SKILLP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* additional gm commands, Juver, 2018/03/01 */
	struct SNETPC_GM_COMMAND_GET_SKILLP_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_SKILLP_FB		emFB;
		DWORD	dwSkillP;

		SNETPC_GM_COMMAND_GET_SKILLP_FB ()
			: dwSkillP(0)
			, emFB(EMREQ_GM_SKILLP_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_SKILLP_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*modern character window, Juver, 2018/03/17 */
	struct SNETPC_REQ_STATSUP_MODERN
	{
		NET_MSG_GENERIC		nmg;
		SCHARSTATS			sStats;
		BOOL bCommand;

		SNETPC_REQ_STATSUP_MODERN ()
			: bCommand(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_STATSUP_MODERN;
		}
	};

	/*modern character window, Juver, 2018/03/17 */
	struct SNETPC_REQ_STATSUP_MODERN_FB
	{
		NET_MSG_GENERIC		nmg;
		SCHARSTATS			sStats;
		WORD				wStatsPoint;
		BOOL bCommand;

		SNETPC_REQ_STATSUP_MODERN_FB ()
			: wStatsPoint(0)
			, bCommand(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_STATSUP_MODERN_FB;
		}
	};


#ifdef USE_HACK_SHIELD
	/*hackshield implementation, Juver, 2018/06/19 */
	struct SNETPC_HS_CALLBACK
	{
		NET_MSG_GENERIC		nmg;
		int			type;
		char		info[MAX_PATH];
		char		file[MAX_PATH];

		SNETPC_HS_CALLBACK () : 
			type(0)
		{
			memset (info, 0, sizeof(char) * MAX_PATH);
			memset (file, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_HS_CALLBACK;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*hackshield implementation, Juver, 2018/06/21 */
	struct SNETPC_HS_CLIENT_CLOSE
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_HS_CLIENT_CLOSE ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_HS_CLIENT_CLOSE;
		}
	};

#endif

	/*item random option rebuild, Juver, 2018/07/04 */
	struct SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD
	{
		NET_MSG_GENERIC		nmg;
		SINVEN_POS			sInvenPosTarget;
		SINVEN_POS			sInvenPosStamp;
		BOOL				bOptionLock0;
		BOOL				bOptionLock1;
		BOOL				bOptionLock2;
		BOOL				bOptionLock3;

		SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD ()
			: bOptionLock0(FALSE)
			, bOptionLock1(FALSE)
			, bOptionLock2(FALSE)
			, bOptionLock3(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_RANDOM_OPTION_REBUILD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*item random option rebuild, Juver, 2018/07/04 */
	struct SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD_FB
	{
		NET_MSG_GENERIC		nmg;
		EMFB_ITEM_RANDOM_OPTION_REBUILD	emFB;

		SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD_FB ()
			: emFB(EMFB_ITEM_RANDOM_OPTION_REBUILD_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_RANDOM_OPTION_REBUILD_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*inventory sort, Juver, 2018/07/07 */
	struct SNETPC_INVEN_SORT
	{
		NET_MSG_GENERIC		nmg;
		DWORD	item_num;

		SNETPC_INVEN_SORT ()
			: item_num(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_SORT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*inventory sort, Juver, 2018/07/07 */
	struct SNETPC_INVEN_SORT_FB
	{
		NET_MSG_GENERIC		nmg;
		EMFB_ITEM_INVEN_SORT	emFB;

		SNETPC_INVEN_SORT_FB ()
			: emFB(EMFB_ITEM_INVEN_SORT_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_SORT_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*inventory sort, Juver, 2018/07/07 */
	struct SNETPC_INVEN_SORT_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		BOOL				bClear;
		DWORD				wNum;
		SINVENITEM			sInvenItem[EMGLMSG_INVEN_SORT_MAX];

		SNETPC_INVEN_SORT_UPDATE () :
			bClear(FALSE),
			wNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_ITEM_SORT_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		BOOL ADD ( const SINVENITEM &sItem )
		{
			if ( wNum == EMGLMSG_INVEN_SORT_MAX )	return FALSE;

			sInvenItem[wNum] = sItem;
			wNum++;

			nmg.dwSize = sizeof(*this) - sizeof(SINVENITEM) * ( EMGLMSG_INVEN_SORT_MAX - wNum );
			return TRUE;
		}
	};

	/*buffs manual remove, Juver, 2018/08/14 */
	struct SNETPC_REQ_BUFF_REMOVE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID	skill_id;

		SNETPC_REQ_BUFF_REMOVE ()
			: skill_id(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BUFF_REMOVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*skill teleport, Juver, 2018/09/06 */
	struct SNET_SKILL_TELEPORT_BRD : public SNETCROW_BROAD
	{
		D3DXVECTOR3	vPos;

		SNET_SKILL_TELEPORT_BRD () :
			vPos(0,0,0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILL_TELEPORT_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*manual lunchbox remove, Juver, 2018/09/19 */
	struct SNETPC_REQ_LUNCHBOX_MANUAL_REMOVE
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID	skill_id;

		SNETPC_REQ_LUNCHBOX_MANUAL_REMOVE ()
			: skill_id(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_LUNCHBOX_MANUAL_REMOVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*manual lunchbox remove, Juver, 2018/09/19 */
	struct SNETPC_LUNCHBOX_REMOVE_BRD : public SNETCROW_BROAD
	{
		bool bRESET[FITEMFACT_SIZE];

		SNETPC_LUNCHBOX_REMOVE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_LUNCHBOX_MANUAL_REMOVE_BRD;

			memset ( bRESET, 0, sizeof(bool)*FITEMFACT_SIZE );
		}
	};

	///*hide costume, EJCode, 2018/11/18 */
	//struct SNETPC_HIDE_COSTUME
	//{
	//	NET_MSG_GENERIC		nmg;

	//	bool	hide_costume;

	//	SNETPC_HIDE_COSTUME ()
	//		: hide_costume(false)
	//	{
	//		nmg.dwSize = sizeof(*this);
	//		nmg.nType = NET_MSG_GCTRL_HIDE_COSTUME;
	//	}	
	//};

	///*hide costume, EJCode, 2018/11/18 */
	//struct SNETPC_HIDE_COSTUME_BRD : public SNETPC_BROAD
	//{
	//	bool	hide_costume;			

	//	SNETPC_HIDE_COSTUME_BRD ()
	//		: hide_costume(false)		
	//	{
	//		nmg.dwSize = sizeof(*this);
	//		nmg.nType = NET_MSG_GCTRL_HIDE_COSTUME_BRD;
	//	}	
	//};

	///*hide costume, EJCode, 2018/11/18 */
	//struct SNETPC_HIDE_COSTUME_FB
	//{
	//	NET_MSG_GENERIC		nmg;

	//	EMFB_HIDE_COSTUME	fb;
	//	bool	hide_costume;	

	//	SNETPC_HIDE_COSTUME_FB ()
	//		: fb(EMFB_HIDE_COSTUME_DELAY_TIME)
	//		, hide_costume(false)
	//	{
	//		nmg.dwSize = sizeof(*this);
	//		nmg.nType = NET_MSG_GCTRL_HIDE_COSTUME_FB;
	//		GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
	//	}
	//};

	/*client tick count check, EJCode, 2018/11/26 */
	struct SNETPC_CLIENT_TICK_COUNT_CHECK_REQUEST
	{
		NET_MSG_GENERIC		nmg;
		DWORD	key1;
		DWORD	key2;

		SNETPC_CLIENT_TICK_COUNT_CHECK_REQUEST ()
			: key1(0)
			, key2(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CLIENT_TICK_COUNT_CHECK_REQUEST;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*client tick count check, EJCode, 2018/11/26 */
	struct SNETPC_CLIENT_TICK_COUNT_CHECK_RESULT
	{
		NET_MSG_GENERIC		nmg;

		DWORD	result;
		DWORD	key1;
		DWORD	key2;

		SNETPC_CLIENT_TICK_COUNT_CHECK_RESULT ()
			: result(0)
			, key1(0)
			, key2(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CLIENT_TICK_COUNT_CHECK_RESULT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}	
	};

	/*gm command send item, Juver, 2019/03/23 */
	struct SNETPC_GM_COMMAND_C2A_SEND_ITEM
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwCharID;
		SNATIVEID			sidItem;
		char				szPass[MAX_PATH];

		SNETPC_GM_COMMAND_C2A_SEND_ITEM ()
			: dwCharID(GAEAID_NULL)
			, sidItem(false)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_C2A_SEND_ITEM;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*gm command send item, Juver, 2019/03/23 */
	struct SNETPC_GM_COMMAND_A2C_SEND_ITEM_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_SEND_ITEM_FB	emFB;

		SNETPC_GM_COMMAND_A2C_SEND_ITEM_FB ()
			: emFB(EMREQ_GM_SEND_ITEM_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_A2C_SEND_ITEM_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* gm command send item, Juver, 2021/02/14 */
	struct SNETPC_GM_COMMAND_A2F_SEND_ITEM
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwCharID;
		DWORD				dwCharIDSender;
		SNATIVEID			sidItem;
		char				szPass[MAX_PATH];

		SNETPC_GM_COMMAND_A2F_SEND_ITEM ()
			: dwCharID(GAEAID_NULL)
			, sidItem(false)
			, dwCharIDSender(GAEAID_NULL)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_A2F_SEND_ITEM;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* gm command send item, Juver, 2021/02/14 */
	struct SNETPC_GM_COMMAND_F2A_SEND_ITEM_FB
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwCharSender;
		DWORD					dwCharReceiver;
		EMREQ_GM_SEND_ITEM_FB	emFB;
		SNATIVEID				sidItem;

		SNETPC_GM_COMMAND_F2A_SEND_ITEM_FB ()
			: dwCharSender(GAEAID_NULL)
			, dwCharReceiver(GAEAID_NULL)
			, emFB(EMREQ_GM_SEND_ITEM_FB_FAIL)
			, sidItem(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_F2A_SEND_ITEM_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/*gm command send item, Juver, 2019/03/23 */
	struct SNETPC_GM_COMMAND_A2C_SEND_ITEM_NOTICE
	{
		NET_MSG_GENERIC			nmg;
		SNATIVEID	sidItem;
		char		szName[CHAR_SZNAME];

		SNETPC_GM_COMMAND_A2C_SEND_ITEM_NOTICE ()
			: sidItem(false)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_A2C_SEND_ITEM_NOTICE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_CAPTURE_PUTON_DROP
	{
		NET_MSG_GENERIC		nmg;

		char	szSerial[LOG_PUTON_SERIAL_SIZE+1];

		SNETPC_CAPTURE_PUTON_DROP () 
		{
			memset (szSerial, 0, sizeof(char) * (LOG_PUTON_SERIAL_SIZE+1));

			nmg.dwSize = sizeof(SNETPC_CAPTURE_PUTON_DROP);
			nmg.nType = NET_MSG_GCTRL_CAPTURE_PUTON_DROP;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_COMMAND_GET_CROW_TIME
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID	sMobID;
		SNATIVEID	sMapID;

		char		szPass[MAX_PATH];

		SNETPC_GM_COMMAND_GET_CROW_TIME ()
			: sMobID(false)
			, sMapID(false)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_CROW_TIME;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_COMMAND_GET_CROW_TIME_FB
	{
		NET_MSG_GENERIC				nmg;
		EMREQ_GM_GET_CROW_TIME_FB	emFB;
		BOOL		bAlive;

		BOOL		bUseRegenTime;
		bool		bDayOfWeek[7];
		int			nRegenHour;
		int			nRegenMinute;

		float		fTimer;

		SNATIVEID	sMobID;
		SNATIVEID	sMapID;

		SNETPC_GM_COMMAND_GET_CROW_TIME_FB ()
			: emFB(EMREQ_GM_GET_CROW_TIME_FB_FAIL)
			, bAlive(FALSE)
			, bUseRegenTime(FALSE)
			, nRegenHour(0)
			, nRegenMinute(0)
			, fTimer(0.0f)
			, sMobID(false)
			, sMapID(false)
		{
			for( int i=0; i<7; ++i )
				bDayOfWeek[i] = false;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_GET_CROW_TIME_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_TYRANNY
	{
		NET_MSG_GENERIC			nmg;

		char		szPass[MAX_PATH];
		float		fRate;

		SNETPC_GM_EVENT_CONTRI_TYRANNY ()
			: fRate(0.0f)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_TYRANNY_FB
	{
		NET_MSG_GENERIC				nmg;
		EMREQ_GM_EVENT_CONTRI_TYRANNY_FB	emFB;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_TYRANNY_FB ()
			: emFB(EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_FAIL)
			, fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_TYRANNY_BROADCAST
	{
		NET_MSG_GENERIC		nmg;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_TYRANNY_BROADCAST ()
			: fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_TYRANNY_FLD
	{
		NET_MSG_GENERIC		nmg;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_TYRANNY_FLD ()
			: fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR
	{
		NET_MSG_GENERIC			nmg;

		char		szPass[MAX_PATH];
		float		fRate;

		SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR ()
			: fRate(0.0f)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB
	{
		NET_MSG_GENERIC				nmg;
		EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB	emFB;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB ()
			: emFB(EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_FAIL)
			, fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_BROADCAST
	{
		NET_MSG_GENERIC		nmg;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_BROADCAST ()
			: fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FLD
	{
		NET_MSG_GENERIC		nmg;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FLD ()
			: fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG
	{
		NET_MSG_GENERIC			nmg;

		char		szPass[MAX_PATH];
		float		fRate;

		SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG ()
			: fRate(0.0f)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB
	{
		NET_MSG_GENERIC				nmg;
		EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB	emFB;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB ()
			: emFB(EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_FAIL)
			, fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_BROADCAST
	{
		NET_MSG_GENERIC		nmg;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_BROADCAST ()
			: fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FLD
	{
		NET_MSG_GENERIC		nmg;
		float	fRate;

		SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FLD ()
			: fRate(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_GM_KICKOUT
	{
		NET_MSG_GENERIC			nmg;

		DWORD		dwUserNum;
		float		fTime;
		char		szPass[MAX_PATH];
		
		SNETPC_GM_KICKOUT ()
			: dwUserNum(GAEAID_NULL)
			, fTime(0.0f)
		{
			memset (szPass, 0, sizeof(char) * MAX_PATH);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_KICKOUT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_KICKOUT)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_KICKOUT_FB
	{
		NET_MSG_GENERIC				nmg;
		EMREQ_GM_KICKOUT_FB			emFB;
		
		float fTime;

		DWORD dwUserNum;

		SNETPC_GM_KICKOUT_FB ()
			: emFB(EMREQ_GM_KICKOUT_FB_FAIL)
			, fTime(0.0f)
			, dwUserNum(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_COMMAND_KICKOUT_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_KICKOUT_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/12 */
	struct SNETPC_PERSONAL_LOCK_REQUEST
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;

		SNETPC_PERSONAL_LOCK_REQUEST () 
			: emLock(EMPERSONAL_LOCK_SIZE)
		{
			nmg.dwSize = sizeof(SNETPC_PERSONAL_LOCK_REQUEST);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_REQUEST;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_REQUEST)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/12 */
	struct SNETPC_PERSONAL_LOCK_REQUEST_FB
	{
		NET_MSG_GENERIC				nmg;
		EMPERSONAL_LOCK				emLock;
		EMREQ_PERSONAL_LOCK_REQUEST	emFB;
		
		SNETPC_PERSONAL_LOCK_REQUEST_FB ()
			: emLock(EMPERSONAL_LOCK_SIZE)
			, emFB(EMREQ_PERSONAL_LOCK_REQUEST_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_REQUEST_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_REQUEST_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/13 */
	struct SNETPC_PERSONAL_LOCK_CREATE_PIN
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		char				szPin1[PERSONAL_LOCK_PIN_SIZE+1];
		char				szPin2[PERSONAL_LOCK_PIN_SIZE+1];

		SNETPC_PERSONAL_LOCK_CREATE_PIN ()
			: emLock(EMPERSONAL_LOCK_SIZE)
		{
			memset (szPin1, 0, sizeof(szPin1));
			memset (szPin2, 0, sizeof(szPin1));

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_CREATE_PIN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_CREATE_PIN)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/13 */
	struct SNETPC_PERSONAL_LOCK_CREATE_PIN_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		EMFB_PERSONAL_LOCK_CREATE_PIN emFB;

		SNETPC_PERSONAL_LOCK_CREATE_PIN_FB ()
			: emLock(EMPERSONAL_LOCK_SIZE)
			, emFB(EMFB_PERSONAL_LOCK_CREATE_PIN_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_CREATE_PIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_CREATE_PIN_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/13 */
	struct SNETPC_PERSONAL_LOCK_INPUT_PIN
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		char				szPin[PERSONAL_LOCK_PIN_SIZE+1];

		SNETPC_PERSONAL_LOCK_INPUT_PIN ()
			: emLock(EMPERSONAL_LOCK_SIZE)
		{
			memset (szPin, 0, sizeof(szPin));

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_INPUT_PIN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_INPUT_PIN)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/13 */
	struct SNETPC_PERSONAL_LOCK_INPUT_PIN_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		EMFB_PERSONAL_LOCK_INPUT_PIN emFB;
		bool	bLock;

		SNETPC_PERSONAL_LOCK_INPUT_PIN_FB ()
			: emLock(EMPERSONAL_LOCK_SIZE)
			, emFB(EMFB_PERSONAL_LOCK_INPUT_PIN_FAILED)
			, bLock(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_INPUT_PIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_INPUT_PIN_FB)<=NET_DATA_BUFSIZE);
		}
	};


	/* personal lock system, Juver, 2019/12/14 */
	struct SNETPC_PERSONAL_LOCK_PIN_RESET
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;

		SNETPC_PERSONAL_LOCK_PIN_RESET ()
			: emLock(EMPERSONAL_LOCK_SIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_RESET_PIN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2019/12/14 */
	struct SNETPC_PERSONAL_LOCK_PIN_RESET_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		EMFB_PERSONAL_LOCK_PIN_RESET emFB;
		bool	 bLock[EMPERSONAL_LOCK_SIZE];

		SNETPC_PERSONAL_LOCK_PIN_RESET_FB ()
			: emLock(EMPERSONAL_LOCK_SIZE)
			, emFB(EMFB_PERSONAL_LOCK_PIN_RESET_FAILED)
		{
			for ( int i=0; i<EMPERSONAL_LOCK_SIZE; ++i )
			{
				bLock[i] = false;
			}

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_RESET_PIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_PIN_RESET_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2020/01/27 */
	struct SNETPC_PERSONAL_LOCK_PIN_CHANGE
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		char				szPinOld[PERSONAL_LOCK_PIN_SIZE+1];
		char				szPinNew[PERSONAL_LOCK_PIN_SIZE+1];
		char				szPinNew2[PERSONAL_LOCK_PIN_SIZE+1];

		SNETPC_PERSONAL_LOCK_PIN_CHANGE ()
			: emLock(EMPERSONAL_LOCK_SIZE)
		{
			memset (szPinOld, 0, sizeof(szPinOld));
			memset (szPinNew, 0, sizeof(szPinNew));
			memset (szPinNew2, 0, sizeof(szPinNew2));

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_CHANGE_PIN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2020/01/29 */
	struct SNETPC_PERSONAL_LOCK_PIN_CHANGE_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		EMFB_PERSONAL_LOCK_PIN_CHANGE emFB;

		SNETPC_PERSONAL_LOCK_PIN_CHANGE_FB ()
			: emLock(EMPERSONAL_LOCK_SIZE)
			, emFB(EMFB_PERSONAL_LOCK_PIN_CHANGE_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_CHANGE_PIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_PIN_CHANGE_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2020/01/31 */
	struct SNETPC_PERSONAL_LOCK_PIN_RECOVER
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;

		SNETPC_PERSONAL_LOCK_PIN_RECOVER ()
			: emLock(EMPERSONAL_LOCK_SIZE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_RECOVER_PIN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	
	/* PK Streak Sigaw - Jxyy */
	struct SNETPC_PK_STREAK
	{
		NET_MSG_GENERIC		nmg;
		char				szName[CHAR_SZNAME];
		DWORD				dwPkCounter;
		
		SNETPC_PK_STREAK () :
			dwPkCounter (0)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PK_STREAK;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PK_STREAK_KILL
	{
		NET_MSG_GENERIC		nmg;
		char				szName[CHAR_SZNAME];
		char				szNameKiller[CHAR_SZNAME];
		DWORD				dwPkCounter;
		
		SNETPC_PK_STREAK_KILL () :
			dwPkCounter (0)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);
			memset (szNameKiller, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PK_STREAK_KILL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	/* PK Streak Sigaw - Jxyy End*/	

	struct SNETPC_RESU_STREAK
	{
		NET_MSG_GENERIC		nmg;
		char				szName[CHAR_SZNAME];
		DWORD				dwResuCounter;

		SNETPC_RESU_STREAK() :
			dwResuCounter(0)
		{
			memset(szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_RESU_STREAK;
			GASSERT(nmg.dwSize <= NET_DATA_BUFSIZE);
		}
	};

	/* personal lock system, Juver, 2020/01/31 */
	struct SNETPC_PERSONAL_LOCK_PIN_RECOVER_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPERSONAL_LOCK		emLock;
		EMFB_PERSONAL_LOCK_PIN_RECOVER emFB;
		
		SPERSONAL_LOCK_PIN	sPin[EMPERSONAL_LOCK_SIZE];

		SNETPC_PERSONAL_LOCK_PIN_RECOVER_FB ()
			: emLock(EMPERSONAL_LOCK_SIZE)
			, emFB(EMFB_PERSONAL_LOCK_PIN_RECOVER_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PERSONAL_LOCK_RECOVER_PIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PERSONAL_LOCK_PIN_RECOVER_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* party finder, Juver, 2020/01/03 */
	struct SNETPC_PARTY_FINDER_SEARCH
	{
		NET_MSG_GENERIC			nmg;
		DWORD dwIndex;

		SNETPC_PARTY_FINDER_SEARCH ()
			: dwIndex(SPARTY_FINDER_SEARCH_INDEX_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_SEARCH;

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_SEARCH)<=NET_DATA_BUFSIZE);
		}
	};

	/* party finder, Juver, 2020/01/03 */
	struct SNETPC_PARTY_FINDER_SEARCH_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPARTY_FINDER_SEARCH_FB	emFB;

		SNETPC_PARTY_FINDER_SEARCH_FB ()
			: emFB(EMPARTY_FINDER_SEARCH_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_SEARCH_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_SEARCH_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* party finder, Juver, 2020/01/16 */
	struct SNETPC_PARTY_FINDER_RESULT_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		WORD					wResultNum;
		SPARTY_FINDER_RESULT	sResultData[SPARTY_FINDER_RESULT_NUM];

		SNETPC_PARTY_FINDER_RESULT_UPDATE () 
			: wResultNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_RESULT_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_RESULT_UPDATE)<=NET_DATA_BUFSIZE);
		}

		BOOL ADD ( const SPARTY_FINDER_RESULT& sData )
		{
			if ( wResultNum == SPARTY_FINDER_RESULT_NUM )	return FALSE;

			sResultData[wResultNum] = sData;
			wResultNum++;

			nmg.dwSize = sizeof(*this) - sizeof(SPARTY_FINDER_RESULT) * ( SPARTY_FINDER_RESULT_NUM - wResultNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void Reset()
		{
			wResultNum = 0;

			for( int i=0; i<SPARTY_FINDER_RESULT_NUM; ++i )
			{
				sResultData[i] = SPARTY_FINDER_RESULT();
			}
		}
	};

	/* party finder, Juver, 2020/01/16 */
	struct SNETPC_PARTY_FINDER_RESULT_DONE
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_PARTY_FINDER_RESULT_DONE ()
		{
			
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_RESULT_DONE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* party finder, Juver, 2020/01/16 */
	struct SNETPC_PARTY_FINDER_JOIN
	{
		NET_MSG_GENERIC			nmg;
		DWORD dwPartyID;

		WORD wLevel;

		SNETPC_PARTY_FINDER_JOIN ()
			: dwPartyID(PARTY_NULL)
			, wLevel(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_JOIN;

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_JOIN)<=NET_DATA_BUFSIZE);
		}
	};

	/* party finder, Juver, 2020/01/16 */
	struct SNETPC_PARTY_FINDER_JOIN_FB
	{
		NET_MSG_GENERIC		nmg;
		EMPARTY_FINDER_JOIN_FB	emFB;

		SNETPC_PARTY_FINDER_JOIN_FB ()
			: emFB(EMPARTY_FINDER_JOIN_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_JOIN_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_JOIN_FB)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_PARTY_FINDER_QUESTION
	{
		NET_MSG_GENERIC			nmg;
		SPARTY_FINDER_QUESTION	sQuestion;

		SNETPC_PARTY_FINDER_QUESTION ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_QUESTION;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_QUESTION)<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_PARTY_FINDER_REPLY
	{
		NET_MSG_GENERIC			nmg;
		DWORD dwPartyID;
		DWORD dwCharID;
		BOOL bAccept;

		SNETPC_PARTY_FINDER_REPLY ()
			: dwPartyID(PARTY_NULL)
			, dwCharID(GAEAID_NULL)
			, bAccept(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_PARTY_FINDER_REPLY;

			BOOST_STATIC_ASSERT(sizeof(SNETPC_PARTY_FINDER_REPLY)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_PCID
	{
		NET_MSG_GENERIC nmg;

		DWORD dwType;
		DWORD dwCharID;
		char szPass[MAX_PATH];

		SNETPC_GM_PCID ()
			: dwType(0)
			, dwCharID(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_PCID;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			memset (szPass, 0, sizeof(char) * MAX_PATH);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_PCID)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_PCID_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_PCID_FB		emFB;

		TCHAR szPCID_HWID[PCID_LENGTH];
		TCHAR szPCID_MAC[PCID_LENGTH];

		SNETPC_GM_PCID_FB ()
			: emFB(EMREQ_GM_PCID_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_PCID_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			//PCID
			SecureZeroMemory( szPCID_HWID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szPCID_MAC, sizeof(TCHAR) * (PCID_LENGTH) );

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_PCID_FB)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_PCID_FB2
	{
		NET_MSG_GENERIC			nmg;

		TCHAR szBiosUUID[PCID_LENGTH];
		TCHAR szBoardModel[PCID_LENGTH];
		TCHAR szBoardSerial[PCID_LENGTH];
		TCHAR szCPUVersion[PCID_LENGTH];
		TCHAR szCPUID[PCID_LENGTH];
		TCHAR szMAC[PCID_LENGTH];

		TCHAR szPCID_HWID[PCID_LENGTH];
		TCHAR szPCID_MAC[PCID_LENGTH];

		SNETPC_GM_PCID_FB2 ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_PCID_FB2;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			//PCID
			SecureZeroMemory( szBiosUUID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szBoardModel, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szBoardSerial, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szCPUVersion, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szCPUID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szMAC, sizeof(TCHAR) * (PCID_LENGTH) );

			SecureZeroMemory( szPCID_HWID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szPCID_MAC, sizeof(TCHAR) * (PCID_LENGTH) );

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_PCID_FB2)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_PCID_TO_CHAR
	{
		NET_MSG_GENERIC nmg;
		DWORD dwType;
		DWORD dwCharIDSource;
		DWORD dwCharIDTarget;

		SNETPC_GM_PCID_TO_CHAR ()
			: dwType(0)
			, dwCharIDSource(GAEAID_NULL)
			, dwCharIDTarget(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_PCID_TO_CHAR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_PCID_TO_CHAR)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_PCID_REPLY
	{
		NET_MSG_GENERIC nmg;
		DWORD dwType;
		DWORD dwCharIDSource;
		DWORD dwCharIDTarget;

		//PCID
		TCHAR szPCID_HWID[PCID_LENGTH];
		TCHAR szPCID_MAC[PCID_LENGTH];

		SNETPC_GM_PCID_REPLY ()
			: dwType(0)
			, dwCharIDSource(GAEAID_NULL)
			, dwCharIDTarget(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_PCID_REPLY;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			//PCID
			SecureZeroMemory( szPCID_HWID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szPCID_MAC, sizeof(TCHAR) * (PCID_LENGTH) );

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_PCID_REPLY)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_PCID_REPLY2
	{
		NET_MSG_GENERIC nmg;
		DWORD dwType;
		DWORD dwCharIDSource;
		DWORD dwCharIDTarget;

		//PCID
		TCHAR szBiosUUID[PCID_LENGTH];
		TCHAR szBoardModel[PCID_LENGTH];
		TCHAR szBoardSerial[PCID_LENGTH];
		TCHAR szCPUVersion[PCID_LENGTH];
		TCHAR szCPUID[PCID_LENGTH];
		TCHAR szMAC[PCID_LENGTH];

		TCHAR szPCID_HWID[PCID_LENGTH];
		TCHAR szPCID_MAC[PCID_LENGTH];

		SNETPC_GM_PCID_REPLY2 ()
			: dwType(0)
			, dwCharIDSource(GAEAID_NULL)
			, dwCharIDTarget(GAEAID_NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_PCID_REPLY2;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			//PCID
			SecureZeroMemory( szBiosUUID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szBoardModel, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szBoardSerial, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szCPUVersion, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szCPUID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szMAC, sizeof(TCHAR) * (PCID_LENGTH) );

			SecureZeroMemory( szPCID_HWID, sizeof(TCHAR) * (PCID_LENGTH) );
			SecureZeroMemory( szPCID_MAC, sizeof(TCHAR) * (PCID_LENGTH) );

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_PCID_REPLY2)<=NET_DATA_BUFSIZE);
		}
	};

	/* user flag restricted, Juver, 2020/04/21 */
	struct SNETPC_GM_USER_RESTRICT
	{
		NET_MSG_GENERIC nmg;

		DWORD dwCharID;
		bool bRestrict;
		char szPass[MAX_PATH];

		SNETPC_GM_USER_RESTRICT ()
			: dwCharID(GAEAID_NULL)
			, bRestrict(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_USER_RESTRICT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			memset (szPass, 0, sizeof(char) * MAX_PATH);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_USER_RESTRICT)<=NET_DATA_BUFSIZE);
		}
	};

	/* user flag restricted, Juver, 2020/04/21 */
	struct SNETPC_GM_USER_RESTRICT_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_USER_RESTRICT_FB		emFB;
		bool	bUserRestricted;

		SNETPC_GM_USER_RESTRICT_FB ()
			: emFB(EMREQ_GM_USER_RESTRICT_FB_FAIL)
			, bUserRestricted(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_USER_RESTRICT_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_USER_RESTRICT_FB)<=NET_DATA_BUFSIZE);
		}
	};

	/* user flag restricted, Juver, 2020/04/21 */
	struct SNET_GM_USER_RESTRICT_PROC_FLD
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;
		bool	bUserRestricted;

		SNET_GM_USER_RESTRICT_PROC_FLD ()
			: dwCharID(0)
			, bUserRestricted(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_USER_RESTRICT_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNET_GM_USER_RESTRICT_PROC_FLD)<=NET_DATA_BUFSIZE);
		}
	};

	/* user flag restricted, Juver, 2020/04/21 */
	struct SNET_GM_USER_RESTRICT_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwCharID;
		bool	bUserRestricted;

		SNET_GM_USER_RESTRICT_UPDATE ()
			: dwCharID(0)
			, bUserRestricted(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_USER_RESTRICT_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNET_GM_USER_RESTRICT_UPDATE)<=NET_DATA_BUFSIZE);
		}
	};


	/* gm command inven clear, Juver, 2020/05/09 */
	struct SNETPC_GM_INVEN_CLEAR
	{
		NET_MSG_GENERIC nmg;

		char szPass[MAX_PATH];

		SNETPC_GM_INVEN_CLEAR ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_INVEN_CLEAR;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_INVEN_CLEAR)<=NET_DATA_BUFSIZE);
		}
	};

	/* gm command inven clear, Juver, 2020/05/09 */
	struct SNETPC_GM_INVEN_CLEAR_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_INVEN_CLEAR_FB	emFB;

		SNETPC_GM_INVEN_CLEAR_FB ()
			: emFB(EMREQ_GM_INVEN_CLEAR_FB_FAILED)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_INVEN_CLEAR_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* force auto revive, Juver, 2020/12/24 */
	struct SNETPC_F2C_FORCE_AUTO_REVIVE
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwGaeaID;

		SNETPC_F2C_FORCE_AUTO_REVIVE ()
			: dwGaeaID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_F2A_FORCE_AUTO_REVIVE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	/* award title, Juver, 2021/02/12 */
	struct SNETPC_AWARD_TITLE_REQUEST_DATA
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_AWARD_TITLE_REQUEST_DATA ()
		{
			nmg.dwSize = sizeof(SNETPC_AWARD_TITLE_REQUEST_DATA);
			nmg.nType = NET_MSG_GCTRL_C2A_AWARD_TITLE_REQUEST_DATA;
		}
	};

	/* award title, Juver, 2021/02/12 */
	struct SNETPC_AWARD_TITLE_UPDATE_DATA
	{
		NET_MSG_GENERIC		nmg;
		SAWARD_TITLE_DATA	sData;

		SNETPC_AWARD_TITLE_UPDATE_DATA ()
		{
			nmg.dwSize = sizeof(SNETPC_AWARD_TITLE_UPDATE_DATA);
			nmg.nType = NET_MSG_GCTRL_A2C_AWARD_TITLE_UPDATE_DATA;
		}
	};

	/* award title, Juver, 2021/02/12 */
	struct SNETPC_AWARD_TITLE_UPDATE_DONE
	{
		NET_MSG_GENERIC			nmg;

		SNETPC_AWARD_TITLE_UPDATE_DONE ()
		{
			nmg.dwSize = sizeof(SNETPC_AWARD_TITLE_UPDATE_DONE);
			nmg.nType = NET_MSG_GCTRL_A2C_AWARD_TITLE_UPDATE_DONE;
		}
	};

	/* game notice, Juver, 2021/06/12 */
	struct SNETPC_GM_GAME_NOTICE_RELOAD
	{
		NET_MSG_GENERIC nmg;

		char szPass[MAX_PATH];

		SNETPC_GM_GAME_NOTICE_RELOAD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_GAME_NOTICE_RELOAD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			memset (szPass, 0, sizeof(char) * MAX_PATH);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_GAME_NOTICE_RELOAD)<=NET_DATA_BUFSIZE);
		}
	};

	/* game notice, Juver, 2021/06/12 */
	struct SNETPC_GM_GAME_NOTICE_RELOAD_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_GAME_NOTICE_RELOAD_FB	emFB;

		SNETPC_GM_GAME_NOTICE_RELOAD_FB ()
			: emFB(EMREQ_GM_GAME_NOTICE_RELOAD_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_GAME_NOTICE_RELOAD_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* variable check, Juver, 2021/07/02 */
	struct SNET_VARIABLE_CHECK_CLIENT
	{
		NET_MSG_GENERIC		nmg;

		SNET_VARIABLE_CHECK_CLIENT () 
		{
			nmg.dwSize = sizeof(SNET_VARIABLE_CHECK_CLIENT);
			nmg.nType = NET_MSG_GCTRL_VARIABLE_CHECK_CLIENT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* variable check, Juver, 2021/07/02 */
	struct SNET_VARIABLE_CHECK_RESULT
	{
		NET_MSG_GENERIC nmg;

		EMVARIABLE_CHECK emCheck;
		float fVar1;
		float fVar2;

		SNET_VARIABLE_CHECK_RESULT ()
			: emCheck(EMVARIABLE_CHECK_NONE)
			, fVar1(0.0f)
			, fVar2(0.0f)
		{
			nmg.dwSize = sizeof(SNET_VARIABLE_CHECK_RESULT);
			nmg.nType = NET_MSG_GCTRL_VARIABLE_CHECK_RESULT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* chaos machine, Juver, 2021/07/09 */
	struct SNETPC_INVEN_CHAOS_MACHINE
	{
		NET_MSG_GENERIC		nmg;
		SINVEN_POS			sInvenItem;
		DWORD				dwNPCID;

		SNETPC_INVEN_CHAOS_MACHINE ()
			: dwNPCID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_CHAOS_MACHINE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_INVEN_CHAOS_MACHINE_FB
	{
		NET_MSG_GENERIC				nmg;
		EMFB_CHAOS_MACHINE			emFB;

		SNETPC_INVEN_CHAOS_MACHINE_FB ()
			: emFB(EMFB_CHAOS_MACHINE_ERROR)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_INVEN_CHAOS_MACHINE_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* max level notice, Juver, 2021/07/26 */
	struct SNETPC_MAX_LEVEL_NOTICE_AGENT
	{
		NET_MSG_GENERIC		nmg;
		DWORD	dwUserID;
		DWORD	dwCharID;
		char	szName[CHAR_SZNAME];
		WORD	wLevel;

		SNETPC_MAX_LEVEL_NOTICE_AGENT ()
			: dwUserID(0)
			, dwCharID(0)
			, wLevel(0)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_MAX_LEVEL_NOTICE_AGENT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* max level notice, Juver, 2021/07/26 */
	struct SNETPC_MAX_LEVEL_NOTICE_CLIENT
	{
		NET_MSG_GENERIC		nmg;
		char	szName[CHAR_SZNAME];
		WORD	wLevel;

		SNETPC_MAX_LEVEL_NOTICE_CLIENT ()
			: wLevel(0)
		{
			memset (szName, 0, sizeof(char) * CHAR_SZNAME);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_MAX_LEVEL_NOTICE_CLIENT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	/* tyranny AFK disconnect, Juver, 2021/08/07 */
	struct SNETPC_TYRANNY_AFK_DISCONNECT
	{
		NET_MSG_GENERIC		nmg;
		float	fTime;
		
		SNETPC_TYRANNY_AFK_DISCONNECT ()
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_TYRANNY_AFK_DISCONNECT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_CAPTURE_THE_FLAG_AFK_DISCONNECT
	{
		NET_MSG_GENERIC		nmg;
		float	fTime;

		SNETPC_CAPTURE_THE_FLAG_AFK_DISCONNECT ()
			: fTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CAPTURE_THE_FLAG_AFK_DISCONNECT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_F2C_PKMARK_BRD
	{
		NET_MSG_GENERIC		nmg;

		SPKMARK_INFO	sData;

		SNET_F2C_PKMARK_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GTRL_PKMARK_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_GM_GIS_RELOAD
	{
		NET_MSG_GENERIC nmg;

		char szPass[MAX_PATH];

		SNETPC_GM_GIS_RELOAD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_GIS_RELOAD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			memset (szPass, 0, sizeof(char) * MAX_PATH);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_GM_GIS_RELOAD)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_GIS_RELOAD_FB
	{
		NET_MSG_GENERIC			nmg;
		EMREQ_GM_GIS_RELOAD_FB	emFB;

		SNETPC_GM_GIS_RELOAD_FB ()
			: emFB(EMREQ_GM_GIS_RELOAD_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_GIS_RELOAD_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_GIS_RELOAD_CLOSE
	{
		NET_MSG_GENERIC			nmg;

		SNETPC_GM_GIS_RELOAD_CLOSE ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_GIS_RELOAD_CLOSE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_GM_GIS_RELOAD_DONE
	{
		NET_MSG_GENERIC			nmg;

		SNETPC_GM_GIS_RELOAD_DONE ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_GM_GIS_RELOAD_DONE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_ENCHANT_NOTIFY_AGT
	{
		NET_MSG_GENERIC		nmg;
		char szName[CHAR_SZNAME];
		SITEMCUSTOM sItemCustom;

		SNETPC_ENCHANT_NOTIFY_AGT () 
		{
			memset(szName, 0, sizeof(char) * (CHAR_SZNAME));

			nmg.dwSize = sizeof(SNETPC_ENCHANT_NOTIFY_AGT);
			nmg.nType = NET_MSG_ENCHANT_NOTIFY_AGT;
			BOOST_STATIC_ASSERT(sizeof(SNETPC_ENCHANT_NOTIFY_AGT)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_ENCHANT_NOTIFY_CLIENT
	{
		NET_MSG_GENERIC		nmg;
		char szName[CHAR_SZNAME];
		SITEMCUSTOM sItemCustom;

		SNETPC_ENCHANT_NOTIFY_CLIENT ()
		{
			memset(szName, 0, sizeof(char) * (CHAR_SZNAME));

			nmg.dwSize = sizeof(SNETPC_ENCHANT_NOTIFY_CLIENT);
			nmg.nType = NET_MSG_ENCHANT_NOTIFY_CLIENT;
			BOOST_STATIC_ASSERT(sizeof(SNETPC_ENCHANT_NOTIFY_CLIENT)<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_COMBO_WHORE_NOTIFY_AGT
	{
		NET_MSG_GENERIC		nmg;
		char szName[CHAR_SZNAME];

		SNETPC_COMBO_WHORE_NOTIFY_AGT () 
		{
			memset(szName, 0, sizeof(char) * (CHAR_SZNAME));

			nmg.dwSize = sizeof(SNETPC_COMBO_WHORE_NOTIFY_AGT);
			nmg.nType = NET_MSG_COMBO_WHORE_NOTIFY_AGT;
			BOOST_STATIC_ASSERT(sizeof(SNETPC_COMBO_WHORE_NOTIFY_AGT)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_COMBO_WHORE_NOTIFY_CLIENT
	{
		NET_MSG_GENERIC		nmg;
		char szName[CHAR_SZNAME];

		SNETPC_COMBO_WHORE_NOTIFY_CLIENT ()
		{
			memset(szName, 0, sizeof(char) * (CHAR_SZNAME));

			nmg.dwSize = sizeof(SNETPC_COMBO_WHORE_NOTIFY_CLIENT);
			nmg.nType = NET_MSG_COMBO_WHORE_NOTIFY_CLIENT;
			BOOST_STATIC_ASSERT(sizeof(SNETPC_COMBO_WHORE_NOTIFY_CLIENT)<=NET_DATA_BUFSIZE);
		}
	};


	struct SNETPC_CARD_REBORN_A
	{
		NET_MSG_GENERIC			nmg;
		WORD	wPosX;
		WORD	wPosY;

		SNETPC_CARD_REBORN_A () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_CARD_REBORN_A);
			nmg.nType = NET_MSG_GCTRL_CARD_REBORN_A;
		}
	};

	struct SNETPC_CARD_REBORN_A_FB
	{
		NET_MSG_GENERIC			nmg;

		EMINVEN_CARD_REBORN_FB	emFB;

		SNETPC_CARD_REBORN_A_FB () 
			: emFB(EMINVEN_CARD_REBORN_FB_FAIL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CARD_REBORN_A_FB;
		}
	};


	struct SNETPC_CARD_REBORN_B
	{
		NET_MSG_GENERIC			nmg;
		WORD	wPosX;
		WORD	wPosY;

		SNETPC_CARD_REBORN_B () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_CARD_REBORN_B);
			nmg.nType = NET_MSG_GCTRL_CARD_REBORN_B;
		}
	};

	struct SNETPC_CARD_REBORN_B_FB
	{
		NET_MSG_GENERIC			nmg;

		BOOL		bOK;
		WORD		wReborn;

		SNETPC_CARD_REBORN_B_FB () 
			: bOK(FALSE)
			, wReborn(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CARD_REBORN_B_FB;
		}
	};


	struct SNET_REQ_EXCHANGE_ITEM
	{
		NET_MSG_GENERIC		nmg;
		WORD				wPosX;
		WORD				wPosY;
		WORD				wCardPosX;
		WORD				wCardPosY;
		SNATIVEID			sID;

		SNET_REQ_EXCHANGE_ITEM()
			: wPosX(EM_INVENSIZE_X)
			, wPosY(EM_INVENSIZE_Y)
			, wCardPosX(EM_INVENSIZE_X)
			, wCardPosY(EM_INVENSIZE_Y)
		{
			sID = NATIVEID_NULL();
			nmg.dwSize = sizeof( SNET_REQ_EXCHANGE_ITEM );
			nmg.nType = NET_MSG_GCTRL_REQ_EXCHANGE_ITEM;
			GASSERT( nmg.dwSize <= NET_DATA_BUFSIZE );
		}
	};

	struct SNET_REQ_EXCHANGE_ITEM_FB
	{
		NET_MSG_GENERIC		nmg;
		EMEXCHANGE_ITEM_FB	emFB;
		SNET_REQ_EXCHANGE_ITEM_FB () 
			: emFB(EMEXCHANGE_ITEM_ERROR)
		{
			nmg.dwSize = sizeof(SNET_REQ_EXCHANGE_ITEM_FB);
			nmg.nType = NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_UPDATE_CHANGE_ITEM_POINTS
	{
		NET_MSG_GENERIC nmg;
		DWORD wChangeItemPoints;

		SNETPC_UPDATE_CHANGE_ITEM_POINTS()
			: wChangeItemPoints(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_CHANGE_ITEM_POINTS;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_REQ_EXCHANGE_ITEM_CARD
	{
		NET_MSG_GENERIC			nmg;
		WORD	wPosX;
		WORD	wPosY;

		SNETPC_REQ_EXCHANGE_ITEM_CARD () 
			: wPosX(0)
			, wPosY(0)
		{
			nmg.dwSize = (DWORD) sizeof(SNETPC_REQ_EXCHANGE_ITEM_CARD);
			nmg.nType = NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_CARD;
		}
	};

	struct SNETPC_REQ_EXCHANGE_ITEM_CARD_FB
	{
		NET_MSG_GENERIC			nmg;

		EMEXCHANGE_ITEM_POINT emFb;
		WORD wChangeItemPoint;

		SNETPC_REQ_EXCHANGE_ITEM_CARD_FB () 
			: emFb(EMEXCHANGE_ITEM_POINT_ERROR)
			, wChangeItemPoint(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_CARD_FB;
		}
	};


	// Revert to default structure packing
	#pragma pack()
};
