#pragma once

#include "./GLContrlBaseMsg.h"

#include "./GLContrlServerMsg.h"
#include "./GLContrlConflictMsg.h"
#include "./GLContrlCrowMsg.h"
#include "./GLContrlSummonMsg.h"
#include "./GLContrlPartyMsg.h"
#include "./GLContrlTradeMsg.h"
#include "./GLContrlCharJoinMsg.h"
#include "./GLContrlPcMsg.h"
#include "./GLContrlPcMsg2.h"
#include "./GLContrlInvenMsg.h"
#include "./GLContrlSkillMsg.h"
#include "./GLContrlNpcMsg.h"
#include "./GLContrlQuestMsg.h"
#include "./GLContrlFriendMsg.h"
#include "./GLContrlPrivateMarket.h"
#include "./GLContrlClubMsg.h"
#include "./GLContrlPetMsg.h"
#include "./GLContrlInDunMsg.h"

/*item link, Juver, 2017/07/31 */
#include "./GLItemLink.h"  

/*pvp tyranny, Juver, 2017/08/25 */
#include "./GLContrlTyrannyMsg.h"
#include "./GLContrlTyrannyMsgEnum.h"

/*woe Arc Development 08-06-2024*/
#include "./GLContrlWoeMsg.h"
#include "./GLContrlWoeMsgEnum.h"
/*activity system, Juver, 2017/10/31 */
#include "./GLContrlActivityMsg.h"
/* Codex, Archie 02/16/24 */
#include "GLContrlCodexMsg.h"

/*school wars, Juver, 2018/01/19 */
#include "./GLContrlSchoolWarsMsg.h"
#include "./GLContrlSchoolWarsMsgEnum.h"

/*pvp capture the flag, Juver, 2018/01/24 */
#include "./GLContrlCaptureTheFlagMsg.h"
#include "./GLContrlCaptureTheFlagMsgEnum.h"

#include "./PVPClubDeathMatchControlMsg.h"

#include <boost/static_assert.hpp>
/*12-9-14, Battle Pass - CNDev*/
#include "./GLContrlBattlePassMsg.h"
/*12-16-24, daily login - CNDev*/
#include "./GLContrlDailyLoginMsg.h"

enum EMCHAT_LOUDSPEAKER
{
	EMCHAT_LOUDSPEAKER_FAIL				= 0,
	EMCHAT_LOUDSPEAKER_OK				= 1,
	EMCHAT_LOUDSPEAKER_NOITEM			= 2,
	EMCHAT_LOUDSPEAKER_BLOCK			= 3,

	/*megaphone set, Juver, 2018/01/02 */
	EMCHAT_LOUDSPEAKER_DISABLED			= 4, 

	/* megaphone user verfied, Juver, 2020/02/27 */
	EMCHAT_LOUDSPEAKER_REQUIRE_VERIFIED	= 5,	
};

namespace GLMSG
{
	#pragma pack(1)

	struct SNETPC_CHAT_BLOCK
	{
		NET_MSG_GENERIC		nmg;
		CHAR				szName[CHR_ID_LENGTH+1];	// 특정 캐릭터명 or 사용자명
		__time64_t			tChatBlock;					// 채팅 차단 시간
		
		SNETPC_CHAT_BLOCK() 
			: tChatBlock(0)
		{
			nmg.dwSize = sizeof(SNETPC_CHAT_BLOCK);
			nmg.nType = NET_MSG_CHAT_BLOCK;
			memset(szName, 0, sizeof(CHAR) * (CHR_ID_LENGTH+1));
			GASSERT( nmg.dwSize <= NET_DATA_BUFSIZE );			
		}
	};

	struct SNETPC_CHAT_PRIVATE_FAIL
	{
		NET_MSG_GENERIC		nmg;
		CHAR				szName[CHR_ID_LENGTH+1];	// 특정 캐릭터명 or 사용자명

		SNETPC_CHAT_PRIVATE_FAIL ()
		{
			nmg.dwSize = sizeof(SNETPC_CHAT_PRIVATE_FAIL);
			nmg.nType = NET_MSG_CHAT_PRIVATE_FAIL;
			memset(szName, 0, sizeof(CHAR) * (CHR_ID_LENGTH+1));
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_CHAT_GENERAL_FAIL
	{
		NET_MSG_GENERIC		nmg;
	
		SNETPC_CHAT_GENERAL_FAIL ()
		{
			nmg.dwSize = sizeof(SNETPC_CHAT_GENERAL_FAIL);
			nmg.nType = NET_MSG_CHAT_GENERAL_FAIL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_CHAT_LOUDSPEAKER
	{
		NET_MSG_GENERIC		nmg;
		WORD				wPosX;
		WORD				wPosY;

		char				szMSG[CHAT_MSG_SIZE+1];
		SITEMLINK			sItemLink; /*item link, Juver, 2017/07/31 */

		SNETPC_CHAT_LOUDSPEAKER () 
			: wPosX(0)
			, wPosY(0)
		{
			sItemLink = SITEMLINK(); /*item link, Juver, 2017/07/31 */
			nmg.dwSize = sizeof(SNETPC_CHAT_LOUDSPEAKER);
			nmg.nType = NET_MSG_CHAT_LOUDSPEAKER;
			memset(szMSG, 0, sizeof(char) * (CHAT_MSG_SIZE+1));
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_CHAT_LOUDSPEAKER)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_CHAT_LOUDSPEAKER_AGT
	{
		NET_MSG_GENERIC		nmg;
		bool				bchannel_all;
		char				szMSG[CHAT_MSG_SIZE+1];
		SITEMLINK			sItemLink; /*item link, Juver, 2017/07/31 */

		SNETPC_CHAT_LOUDSPEAKER_AGT () 
			: bchannel_all(false)
		{
			sItemLink = SITEMLINK(); /*item link, Juver, 2017/07/31 */
			nmg.dwSize = sizeof(SNETPC_CHAT_LOUDSPEAKER_AGT);
			nmg.nType = NET_MSG_CHAT_LOUDSPEAKER_AGT;
			memset(szMSG, 0, sizeof(char) * (CHAT_MSG_SIZE+1));
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_CHAT_LOUDSPEAKER_AGT)<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_CHAT_LOUDSPEAKER_FB
	{
		NET_MSG_GENERIC		nmg;
		EMCHAT_LOUDSPEAKER	emFB;

		SNETPC_CHAT_LOUDSPEAKER_FB () 
			: emFB(EMCHAT_LOUDSPEAKER_FAIL)
		{
			nmg.dwSize = sizeof(SNETPC_CHAT_LOUDSPEAKER_FB);
			nmg.nType = NET_MSG_CHAT_LOUDSPEAKER_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETDROP_ITEM
	{
		NET_MSG_GENERIC		nmg;

		SDROP_CLIENT_ITEM	Data;

		SNETDROP_ITEM ()
		{
			nmg.dwSize = sizeof(SNETDROP_ITEM);
			nmg.nType = NET_MSG_GCTRL_DROP_ITEM;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETDROP_ITEM)<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETDROP_MONEY
	{
		NET_MSG_GENERIC		nmg;

		float				fAge;
		DWORD				dwGlobID;
		D3DXVECTOR3			vPos;
		LONGLONG			lnAmount;


		SNETDROP_MONEY () 
			: fAge(0)
			, dwGlobID(0)
			, vPos(0,0,0)
			, lnAmount(0)
		{
			nmg.dwSize = sizeof(SNETDROP_MONEY);
			nmg.nType = NET_MSG_GCTRL_DROP_MONEY;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETDROP_MONEY)<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETDROP_CROW
	{
		NET_MSG_GENERIC		nmg;

		SDROP_CROW			Data;

		SNETDROP_CROW ()
		{
			nmg.dwSize = sizeof(SNETDROP_CROW);
			nmg.nType = NET_MSG_GCTRL_DROP_CROW;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETDROP_CROW)<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETDROP_PC
	{
		NET_MSG_GENERIC		nmg;

		SDROP_CHAR			Data;

		SNETDROP_PC ()
		{
			nmg.dwSize = sizeof(SNETDROP_PC);
			nmg.nType = NET_MSG_GCTRL_DROP_PC;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETDROP_PC)<=NET_DATA_BUFSIZE);
		}
	};

//---------------------------------------------------------------------------NET
	struct SNETDROP_MATERIAL
	{
		NET_MSG_GENERIC		nmg;

		SDROP_MATERIAL		Data;

		SNETDROP_MATERIAL ()
		{
			nmg.dwSize = sizeof(SNETDROP_MATERIAL);
			nmg.nType = NET_MSG_GCTRL_DROP_MATERIAL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETDROP_MATERIAL)<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETDROP_OUT
	{
		enum { MAX_NUM = 200, };

		NET_MSG_GENERIC		nmg;
		BYTE				cNUM;
		BYTE				cCUR;
		STARID				sOUTID[MAX_NUM];

		SNETDROP_OUT () 
			: cNUM(0)
			, cCUR(0)
		{
			nmg.dwSize = sizeof(nmg) + sizeof(cNUM) + sizeof(cCUR);
			nmg.nType = NET_MSG_GCTRL_DROP_OUT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETDROP_OUT)<=NET_DATA_BUFSIZE);
		}

		void RESET_CROW ()
		{
			cNUM = 0;
			cCUR = 0;
		}

		BYTE GETAMOUNT ()
		{
			return cNUM;
		}

		bool ADD_CROW ( const STARID &cOUT )
		{
			if ( cNUM >= MAX_NUM )
			{
				return false;
			}
			else
			{
				sOUTID[cNUM++] = cOUT;
				nmg.dwSize = sizeof(nmg) + sizeof(cNUM) + sizeof(cCUR) + sizeof(STARID)*cNUM;
				return true;
			}
		}

		void RESET_CUR ()
		{
			cCUR = 0;
		}

		bool POP_CROW ( STARID &cOUT )
		{
			if ( cNUM == cCUR )
			{
				return false;
			}
			else
			{
				cOUT = sOUTID[cCUR++];
				return true;
			}
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNET_DROP_OUT_FORCED
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwCharID;
		EMDROP_OUT_FORCED	emForced;

		SNET_DROP_OUT_FORCED () 
			: dwCharID(0)
			, emForced(EMDROPOUT_REQLOGIN)
		{
			nmg.dwSize = sizeof(SNET_DROP_OUT_FORCED);
			nmg.nType = NET_MSG_GCTRL_DROP_OUT_FORCED;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_REQ_GENITEM_AGT
	{
		NET_MSG_GENERIC		nmg;

		int					nCHANNEL;
		SNATIVEID			sNID;

		SNATIVEID			sMAPID;
		EMITEMGEN			emGENTYPE;
		EMGROUP				emHoldGroup;
		DWORD				dwHoldGID;
		D3DXVECTOR3			vPos;

		SNET_REQ_GENITEM_AGT () 
			: nCHANNEL(0)
			, sNID(false)
			
			, sMAPID(false)
			, emGENTYPE(EMGEN_DEFAULT)
			, emHoldGroup(EMGROUP_ONE)
			, dwHoldGID(GAEAID_NULL)
			, vPos(0,0,0)
		{
			nmg.dwSize = sizeof(SNET_REQ_GENITEM_AGT);
			nmg.nType = NET_MSG_GCTRL_REQ_GENITEM_AGT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_REQ_GENITEM_FLD_FB
	{
		NET_MSG_GENERIC		nmg;

		SNATIVEID			sNID;

		SNATIVEID			sMAPID;
		EMITEMGEN			emGENTYPE;
		LONGLONG			lnGENCOUNT;
		EMGROUP				emHoldGroup;
		DWORD				dwHoldGID;
		D3DXVECTOR3			vPos;

		SNET_REQ_GENITEM_FLD_FB () 
			: sNID(false)
			
			, sMAPID(false)
			, emGENTYPE(EMGEN_DEFAULT)
			, lnGENCOUNT(0)
			, emHoldGroup(EMGROUP_ONE)
			, dwHoldGID(GAEAID_NULL)
			, vPos(0,0,0)
		{
			nmg.dwSize = sizeof(SNET_REQ_GENITEM_FLD_FB);
			nmg.nType = NET_MSG_GCTRL_REQ_GENITEM_FLD_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_REQ_SERVERTEST
	{
		NET_MSG_GENERIC		nmg;

		enum { EMMSG_SIZE = sizeof(NET_MSG_GENERIC) };
		char				m_cBUFFER[NET_DATA_BUFSIZE-EMMSG_SIZE];

		SNET_REQ_SERVERTEST () 
		{
			nmg.dwSize = sizeof(SNET_REQ_SERVERTEST);
			nmg.nType = NET_MSG_GCTRL_SERVERTEST;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_REQ_SERVERTEST_FB
	{
		NET_MSG_GENERIC		nmg;

		SNET_REQ_SERVERTEST_FB () 
		{
			nmg.dwSize = sizeof(SNET_REQ_SERVERTEST_FB);
			nmg.nType = NET_MSG_GCTRL_SERVERTEST_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_MSG_SERVERTIME_BRD
	{
		NET_MSG_GENERIC		nmg;

		__time64_t			t64Time;
		int					nTimeBias;

		SNET_MSG_SERVERTIME_BRD () 
			: t64Time ( 0 )
			, nTimeBias ( 0 )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_SERVERTIME_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	/* Boss Spawn Viewer, Review000 */
	struct SNETPC_BOSS_DETAILS_REQ
	{
		NET_MSG_GENERIC		nmg;

		DWORD dwGlobID;
		SNATIVEID sMapID;
		SNATIVEID sMobID;

		SNETPC_BOSS_DETAILS_REQ () 
			: dwGlobID( -1 )
			, sMapID( false )
			, sMobID(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BOSS_DETAIL_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	/* Boss Spawn Viewer, Review000 */
	struct SNETPC_BOSS_DETAILS_FB
	{
		NET_MSG_GENERIC		nmg;

		BOOL  bIsAlive;
		float fTimer;
		SNATIVEID sMobID;

		SNETPC_BOSS_DETAILS_FB () 
			: bIsAlive( false )
			, fTimer( 0.0f )
			, sMobID( false )
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_BOSS_DETAIL_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


#if defined( BUILD_CH ) || defined( BUILD_CHY )
	/* wtf, Juver, 2021/06/24 */
	struct SNET_WTF_FLD
	{
		NET_MSG_GENERIC		nmg;
		DWORD dwKey;

		SNET_WTF_FLD () 
			: dwKey(0)
		{
			nmg.dwSize = sizeof(SNET_WTF_FLD);
			nmg.nType = NET_MSG_GCTRL_WTF_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
#endif

	// Revert to default structure packing
	#pragma pack()
};

