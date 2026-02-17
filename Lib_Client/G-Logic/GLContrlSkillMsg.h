#pragma once

#include "./GLContrlBaseMsg.h"
#include "../../Lib_Helper/cMtSafeVar.h"

#include <boost/static_assert.hpp>

namespace GLMSG
{
	#pragma pack(1)

	struct SNETPC_REQ_SKILLQUICK_SET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;
		SNATIVEID			skill_id;

		SNETPC_REQ_SKILLQUICK_SET () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_SKILLQUICK_SET);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_SET;
		}
	};

	struct SNETPC_REQ_SKILLQUICK_RESET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;

		SNETPC_REQ_SKILLQUICK_RESET () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_SKILLQUICK_RESET);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_RESET;
		}
	};

	struct SNETPC_REQ_SKILLQUICK_FB
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;
		SNATIVEID			skill_id;

		SNETPC_REQ_SKILLQUICK_FB () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_SKILLQUICK_FB);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_FB;
		}
	};

	struct SNETPC_REQ_SKILLQUICK_ACTIVE
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;

		SNETPC_REQ_SKILLQUICK_ACTIVE () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_ACTIVE;
		}
	};

	struct SNETPC_REQ_ACTIONQUICK_SET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;
		WORD				wACT;

		SNETPC_REQ_ACTIONQUICK_SET () :
			wSLOT(0),
			wACT(EMACT_SLOT_DRUG)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_ACTIONQ_SET;
		}
	};

	struct SNETPC_REQ_ACTIONQUICK_RESET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;

		SNETPC_REQ_ACTIONQUICK_RESET () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_ACTIONQ_RESET;
		}
	};

	struct SNETPC_REQ_ACTIONQUICK_FB
	{
		NET_MSG_GENERIC		nmg;

		WORD				wSLOT;
		SACTION_SLOT		sACT;

		SNETPC_REQ_ACTIONQUICK_FB () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ACTIONQUICK_FB);
			nmg.nType = NET_MSG_GCTRL_REQ_ACTIONQ_FB;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_SKILL_CANCEL
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_SKILL_CANCEL ()
		{
			nmg.dwSize = sizeof(SNETPC_SKILL_CANCEL);
			nmg.nType = NET_MSG_GCTRL_SKILL_CANCEL;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_SKILL_CANCEL_BRD : public SNETPC_BROAD
	{
		SNETPC_SKILL_CANCEL_BRD ()
		{
			nmg.dwSize = sizeof(SNETPC_SKILL_CANCEL_BRD);
			nmg.nType = NET_MSG_GCTRL_SKILL_CANCEL_BRD;
		}
	};

	struct SNETPC_REQ_SKILLUP
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			skill_id;

		SNETPC_REQ_SKILLUP () :
			skill_id(NATIVEID_NULL())
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLUP;
		}
	};

	struct SNETPC_REQ_SKILLUP_FB
	{
		NET_MSG_GENERIC		nmg;

		SCHARSKILL			sSkill;
		EMSKILL_LEARNCHECK	emCHECK;

		SNETPC_REQ_SKILLUP_FB ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLUP_FB;
		}
	};

	struct SNETPC_SKILL_EXPIRE_FB
	{
		NET_MSG_GENERIC		nmg;

		SNATIVEID			sSkillID;

		SNETPC_SKILL_EXPIRE_FB ()
			: sSkillID( NATIVEID_NULL())
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILL_EXPIRE_FB;
		}
	};

	struct SNETPC_REQ_SKILL
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			skill_id;
		
		DWORD				dwFlags;
		bool				bDefenseSkill;			// 발동 스킬

		/* anti shit */
		float				fAttVelo;
		float				fAttVeloItem;

		/* client variable check, Juver, 2020/03/29 */
		char				szHash[SKILL_HASH_SIZE];

		/*multi skill Juver, 2018/09/04 */
		D3DXVECTOR3			vTARGET_POS_MAIN;
		D3DXVECTOR3			vTARGET_POS_SUB;
		WORD				wTARGET_NUM_MAIN;
		WORD				wTARGET_NUM_SUB;
		STARID				sTARGET_IDS_MAIN[EMTARGET_NET];
		STARID				sTARGET_IDS_SUB[EMTARGET_NET];

		SNETPC_REQ_SKILL ()
			: skill_id(NATIVEID_NULL())

			/*multi skill Juver, 2018/09/04 */
			, vTARGET_POS_MAIN(0,0,0)
			, vTARGET_POS_SUB(0,0,0)
			, wTARGET_NUM_MAIN(0)
			, wTARGET_NUM_SUB(0)

			, dwFlags(NULL)
			, bDefenseSkill(false)
			/* anti shit */
			, fAttVelo(0.0f)
			, fAttVeloItem(0.0f)
		{
			memset(szHash, 0, sizeof(szHash));

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL;

			BOOST_STATIC_ASSERT(sizeof(SNETPC_REQ_SKILL)<=NET_DATA_BUFSIZE);
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_MAIN ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM_MAIN==EMTARGET_NET )	return FALSE;

			sTARGET_IDS_MAIN[wTARGET_NUM_MAIN].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS_MAIN[wTARGET_NUM_MAIN].wID = static_cast<WORD>(dwID);
			
			wTARGET_NUM_MAIN++;

			return TRUE;
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_SUB ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM_SUB==EMTARGET_NET )	return FALSE;

			sTARGET_IDS_SUB[wTARGET_NUM_SUB].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS_SUB[wTARGET_NUM_SUB].wID = static_cast<WORD>(dwID);

			wTARGET_NUM_SUB++;

			return TRUE;
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_MAIN ( const STARID &starget )
		{
			return ADDTARGET_MAIN(starget.GETCROW(),starget.GETID());
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_SUB ( const STARID &starget )
		{
			return ADDTARGET_SUB(starget.GETCROW(),starget.GETID());
		}
	};

	struct SNETPC_REQ_SKILL_FB
	{
		NET_MSG_GENERIC		nmg;
		EMSKILLCHECK		emSKILL_FB;
		char				szName[CHAR_SZNAME];

		SNETPC_REQ_SKILL_FB () :
			emSKILL_FB(EMSKILL_OK)
		{
			ZeroMemory( szName, CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_FB;
		}
	};

	struct SNET_MSG_REQ_SKILL_REVIVEL_FAILED
	{
		NET_MSG_GENERIC		nmg;
		char				szName[CHAR_SZNAME];

		SNET_MSG_REQ_SKILL_REVIVEL_FAILED () 
		{
			ZeroMemory( szName, CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_REQ_SKILL_REVIVEL_FAILED;
		}
	};

	struct SNETPC_REQ_SKILL_BRD
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emCrow;
		DWORD				dwID;

		SNATIVEID			skill_id;
		WORD				wLEVEL;

		/*multi skill Juver, 2018/09/05 */
		D3DXVECTOR3			vTARGET_POS_MAIN;
		D3DXVECTOR3			vTARGET_POS_SUB;
		WORD				wTARGET_NUM_MAIN;
		WORD				wTARGET_NUM_SUB;
		STARID				sTARGET_IDS_MAIN[EMTARGET_NET];
		STARID				sTARGET_IDS_SUB[EMTARGET_NET];

		SNETPC_REQ_SKILL_BRD () :
			emCrow(CROW_PC),
			dwID(0),
			skill_id(NATIVEID_NULL()),
			wLEVEL(0),

			/*multi skill Juver, 2018/09/05 */
			vTARGET_POS_MAIN(0,0,0),
			vTARGET_POS_SUB(0,0,0),
			wTARGET_NUM_MAIN(0),
			wTARGET_NUM_SUB(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_BRD;
		}

		BOOL ADDTARGET_MAIN ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM_MAIN==EMTARGET_NET )	return FALSE;

			sTARGET_IDS_MAIN[wTARGET_NUM_MAIN].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS_MAIN[wTARGET_NUM_MAIN].wID = static_cast<WORD>(dwID);
			
			wTARGET_NUM_MAIN++;

			return TRUE;
		}

		BOOL ADDTARGET_SUB ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM_SUB==EMTARGET_NET )	return FALSE;

			sTARGET_IDS_SUB[wTARGET_NUM_SUB].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS_SUB[wTARGET_NUM_SUB].wID = static_cast<WORD>(dwID);

			wTARGET_NUM_SUB++;

			return TRUE;
		}

		BOOL ADDTARGET_MAIN ( const STARID &starget )
		{
			return ADDTARGET_MAIN(starget.GETCROW(),starget.GETID());
		}

		BOOL ADDTARGET_SUB ( const STARID &starget )
		{
			return ADDTARGET_SUB(starget.GETCROW(),starget.GETID());
		}
	};

	struct SNETPC_SKILLFACT_BRD : public SNETCROW_BROAD
	{
		STARID				sACTOR;

		DWORD				dwDamageFlag;
		int					nVAR_HP;
		short				nVAR_MP;
		short				nVAR_SP;

		SNETPC_SKILLFACT_BRD () :
			dwDamageFlag( DAMAGE_TYPE_NONE ),
			nVAR_HP(0),
			nVAR_MP(0),
			nVAR_SP(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLFACT_BRD;
		}
	};

	struct SNETPC_SKILLHOLD_BRD : public SNETCROW_BROAD
	{
		SNATIVEID			skill_id;
		WORD				wLEVEL;
		WORD				wSELSLOT;

		WORD				wCasterCrow;
		DWORD				dwCasterID;

		SNETPC_SKILLHOLD_BRD () :
			skill_id(NATIVEID_NULL()),
			wLEVEL(0),
			wSELSLOT(0),
			wCasterCrow(0),
			dwCasterID(NATIVEID_NULL().dwID)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLHOLD_BRD;
		}
	};

	struct SNETPC_SKILLHOLD_RS_BRD : public SNETCROW_BROAD
	{
		bool bRESET[SKILLFACT_SIZE];

		SNETPC_SKILLHOLD_RS_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLHOLD_RS_BRD;

			memset ( bRESET, 0, sizeof(bool)*SKILLFACT_SIZE );
		}
	};

	struct SNETPC_REQ_SKILLHOLDEX_BRD : public SNETCROW_BROAD
	{
		WORD				wSLOT;
		SSKILLFACT			sSKILLEF;

		SNETPC_REQ_SKILLHOLDEX_BRD () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLHOLDEX_BRD;
		}
	};

	struct SNETPC_STATEBLOW_BRD : public SNETCROW_BROAD
	{
		EMSTATE_BLOW		emBLOW;
		float				fAGE;
		float				fSTATE_VAR1;
		float				fSTATE_VAR2;

		SNETPC_STATEBLOW_BRD() :
			emBLOW(EMBLOW_NONE),
			fAGE(0),
			fSTATE_VAR1(0),
			fSTATE_VAR2(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_STATEBLOW_BRD;
		}
	};

	struct SNETPC_CURESTATEBLOW_BRD : public SNETCROW_BROAD
	{
		DWORD				dwCUREFLAG;

		SNETPC_CURESTATEBLOW_BRD () :
			dwCUREFLAG(NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CURESTATEBLOW_BRD;
		}
	};

	struct SNETPC_SKILLCONSUME_FB
	{
		NET_MSG_GENERIC		nmg;

		WORD				wTurnNum;	// 소모형 아이템의 사용가능 횟수.
		WORD				wNowHP;
		WORD				wNowMP;
		WORD				wNowSP;
		WORD				wNowCP; /*combatpoint logic, Juver, 2017/05/28 */

		SNETPC_SKILLCONSUME_FB () :
			wTurnNum(0),
			wNowHP(0),
			wNowMP(0),
			wNowSP(0),
			wNowCP(0) /*combatpoint logic, Juver, 2017/05/28 */
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLCONSUME_FB;
		}
	};

	/*extreme auto tab, Juver, 2018/09/10 */
	struct SNETPC_SKILL_TAB_CHANGE
	{
		NET_MSG_GENERIC		nmg;
		BOOL	sub_arms;

		SNETPC_SKILL_TAB_CHANGE ()
			: sub_arms(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}	
	};

	/*extreme auto tab, Juver, 2018/09/10 */
	struct SNETPC_SKILL_TAB_CHANGE_FB
	{
		NET_MSG_GENERIC		nmg;
		
		SNETPC_SKILL_TAB_CHANGE_FB ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE_FB;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}	
	};

	/*extreme auto tab, Juver, 2018/09/10 */
	struct SNETPC_REQ_SKILL_TAB_CHANGE_BRD
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emCrow;
		DWORD				dwID;

		SNETPC_REQ_SKILL_TAB_CHANGE_BRD () :
			emCrow(CROW_PC),
			dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE_BRD;
		}
	};

	/* skill delay addition, Juver, 2020/12/09 */
	struct SNETPC_F2C_SKILL_DELAY_ADJUSTMENT
	{
		NET_MSG_GENERIC		nmg;
		
		SNATIVEID sSkillID;
		float fAdjust;

		SNETPC_F2C_SKILL_DELAY_ADJUSTMENT ()
			: sSkillID(false)
			, fAdjust(0.0f)
		{
			nmg.dwSize = sizeof(SNETPC_F2C_SKILL_DELAY_ADJUSTMENT);
			nmg.nType = NET_MSG_GCTRL_F2C_SKILL_DELAY_ADJUSTMENT;
		}
	};

	
	/* skill delay addition, Juver, 2020/12/09 */
	struct SNETPC_F2C_SKILL_DELAY_ADJUSTMENT_MULTI
	{
		NET_MSG_GENERIC		nmg;

		WORD	wDataNum;
		SKILL_DELAY_ARRAY_DATA	sData[SKILL_DELAY_MULTI_UPDATE_MAX_ARRAY];

		SNETPC_F2C_SKILL_DELAY_ADJUSTMENT_MULTI () 
			: wDataNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_F2C_SKILL_DELAY_ADJUSTMENT_MULTI;

			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			BOOST_STATIC_ASSERT(sizeof(SNETPC_F2C_SKILL_DELAY_ADJUSTMENT_MULTI)<=NET_DATA_BUFSIZE);
		}

		BOOL ADD ( const SNATIVEID& sSkillID, const float& fDelay )
		{
			if ( wDataNum == SKILL_DELAY_MULTI_UPDATE_MAX_ARRAY )	return FALSE;
			sData[wDataNum].sSkillID = sSkillID;
			sData[wDataNum].fAdjust = fDelay;
			wDataNum++;

			nmg.dwSize = sizeof(*this) - sizeof(SKILL_DELAY_ARRAY_DATA) * ( SKILL_DELAY_MULTI_UPDATE_MAX_ARRAY - wDataNum );
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
			return TRUE;
		}

		void RESET()
		{
			wDataNum = 0;
			for( int i=0; i<SKILL_DELAY_MULTI_UPDATE_MAX_ARRAY; ++i )
			{
				sData[i] = SKILL_DELAY_ARRAY_DATA();
			}
		}
	};

	/* skill buff duration change, Juver, 2020/12/17 */
	struct SNETPC_F2C_SKILL_BUFF_DURATION_CHANGE_TIME_MULTIPLE_BRD : public SNETCROW_BROAD
	{
		bool bApply[SKILLFACT_SIZE];
		float fTime[SKILLFACT_SIZE];

		SNETPC_F2C_SKILL_BUFF_DURATION_CHANGE_TIME_MULTIPLE_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_F2C_SKILL_BUFF_DURATION_CHANGE_TIME_MULTIPLE_BRD;

			for ( int i=0; i<SKILLFACT_SIZE; ++i )
			{
				bApply[i] = false;
				fTime[i] = 0.0f;
			}
		}
	};

	struct SNETPC_REQ_SKILL_TARGET_FB
	{
		NET_MSG_GENERIC		nmg;

		SNATIVEID			skill_id;

		/*multi skill Juver, 2018/09/05 */
		D3DXVECTOR3			vTARGET_POS_MAIN;
		D3DXVECTOR3			vTARGET_POS_SUB;
		WORD				wTARGET_NUM_MAIN;
		WORD				wTARGET_NUM_SUB;
		STARID				sTARGET_IDS_MAIN[EMTARGET_NET];
		STARID				sTARGET_IDS_SUB[EMTARGET_NET];

		SNETPC_REQ_SKILL_TARGET_FB ()
			: skill_id(NATIVEID_NULL() )
			/*multi skill Juver, 2018/09/04 */
			, vTARGET_POS_MAIN(0,0,0)
			, vTARGET_POS_SUB(0,0,0)
			, wTARGET_NUM_MAIN(0)
			, wTARGET_NUM_SUB(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_TARGET_FB;

			BOOST_STATIC_ASSERT(sizeof(SNETPC_REQ_SKILL_TARGET_FB)<=NET_DATA_BUFSIZE);
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_MAIN ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM_MAIN==EMTARGET_NET )	return FALSE;

			sTARGET_IDS_MAIN[wTARGET_NUM_MAIN].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS_MAIN[wTARGET_NUM_MAIN].wID = static_cast<WORD>(dwID);

			wTARGET_NUM_MAIN++;

			return TRUE;
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_SUB ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM_SUB==EMTARGET_NET )	return FALSE;

			sTARGET_IDS_SUB[wTARGET_NUM_SUB].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS_SUB[wTARGET_NUM_SUB].wID = static_cast<WORD>(dwID);

			wTARGET_NUM_SUB++;

			return TRUE;
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_MAIN ( const STARID &starget )
		{
			return ADDTARGET_MAIN(starget.GETCROW(),starget.GETID());
		}

		/*multi skill Juver, 2018/09/04 */
		BOOL ADDTARGET_SUB ( const STARID &starget )
		{
			return ADDTARGET_SUB(starget.GETCROW(),starget.GETID());
		}
	};

	struct SNETPC_REQ_SKILL_GO_TO_IDLE_BRD
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emCrow;
		DWORD				dwID;

		SNETPC_REQ_SKILL_GO_TO_IDLE_BRD ()
			: emCrow(CROW_PC)
			, dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_GO_TO_IDLE_BRD;
		}
	};
	
	/*woe Arc Development 08-06-2024*/
	struct SNET_MSG_WOE_CAPTURE : public NET_MSG_GENERIC
	{
		WORD	wClubID;
		bool	bCaptured;
		char	szName[CLUB_NAME + 1];

		SNET_MSG_WOE_CAPTURE()
			: wClubID(CLUB_NULL)
			, bCaptured(false)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_CAPTURE;
		}
	};
	struct SNET_MSG_WOE_CAPTURE_FLD : public NET_MSG_GENERIC
	{
		WORD	wClubID;
		bool	bCaptured;
		char	szName[CLUB_NAME + 1];

		SNET_MSG_WOE_CAPTURE_FLD()
			: wClubID(CLUB_NULL)
			, bCaptured(false)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_CAPTURE_FLD;
		}
	};
	struct SNET_MSG_WOE_CAPTURE_AGT : public NET_MSG_GENERIC
	{
		WORD	wGuildHolder;
		WORD	wClubID;
		bool	bCaptured;

		SNET_MSG_WOE_CAPTURE_AGT()
			: wGuildHolder(CLUB_NULL)
			, wClubID(CLUB_NULL)
			, bCaptured(false)
		{
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_CAPTURE_AGT;
		}
	};
	struct SNET_MSG_WOE_END_BATTLE : public NET_MSG_GENERIC
	{
		DWORD	dwClubID;
		BOOL	m_bWarEnd;
		
		SNET_MSG_WOE_END_BATTLE()
			: dwClubID(CLUB_NULL)
			, m_bWarEnd(false)
		{
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_END_BATTLE;
		}
	};
	struct SNET_MSG_WOE_FIRST_CAPTURE : public NET_MSG_GENERIC
	{
		bool	bCaptured;
		WORD	wClubID;
		DWORD	dwMarkVer;
		char	szName[CLUB_NAME + 1];

		SNET_MSG_WOE_FIRST_CAPTURE()
			: bCaptured(false)
			, wClubID(CLUB_NULL)
			, dwMarkVer(0)
		{
			memset(szName, 0, sizeof(char) * CLUB_NAME + 1);
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_FIRST_CAPTURE;
		}
	};

	struct SNET_MSG_WOE_CHECK_WINNER : public NET_MSG_GENERIC
	{
		DWORD dwClubID;

		SNET_MSG_WOE_CHECK_WINNER()
			: dwClubID(0)
		{
			dwSize = sizeof(*this);
			nType = NET_MSG_WOE_CHECK_WINNER;
		}
	};

	// Revert to default structure packing
	#pragma pack()
};
