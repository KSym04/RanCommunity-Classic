#include "stdafx.h"
#include "shlobj.h"
#include "../../Lib_Engine/Common/SUBPATH.h"
#include "../../Lib_Engine/GUInterface/Cursor.h"
#include "../../Lib_Engine/DxCommon/DxInputDevice.h"
#include "../../Lib_Engine/DxCommon/EditMeshs.h"
#include "../../Lib_Engine/DxCommon/DxMethods.h"
#include "../../Lib_Engine/DxCommon/DxViewPort.h"
#include "../../Lib_Engine/DxEffect/DxEffectMan.h"
#include "../../Lib_Engine/DxCommon/DxEnvironment.h"
#include "../../Lib_Engine/DxCommon/DxShadowMap.h"
#include "../../Lib_Engine/DxCommon/DxSoftShadowMap.h"
#include "../../Lib_Engine/DxCommon/EditMeshs.h"
#include "./GLogicData.h"
#include "./GLItemMan.h"
#include "../../Lib_Engine/DxEffect/Char/DxEffcharData.h"
#include "../../Lib_Engine/DxEffect/EffProj/DxEffProj.h"

#include "tlhelp32.h"
#include "winbase.h"

#include "Psapi.h"
#pragma comment( lib, "Psapi.lib" )

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../DxGlobalStage.h"
#include "./GLGaeaClient.h"
#include "./stl_Func.h"
#include "../../Lib_Engine/DxEffect/Single/DxEffGroupPlayer.h"
#include "./GLStrikeM.h"
#include "./GLCrowData.h"
#include "./GLPartyClient.h"
#include "./GLQUEST.h"
#include "./GLQUESTMAN.h"
#include "./GLFriendClient.h"
#include "./GLFactEffect.h"


#include "../../Lib_Engine/DxCommon/DxRenderStates.h"
#include "./GLCharacter.h"
#include "../../Lib_Engine/Common/DXInputString.h"
#include "../../Lib_Engine/DxSound/BgmSound.h"
#include "./GLTaxiStation.h"
#include "./GLItemMixMan.h"


#include "../../Lib_Engine/DxEffect/Single/DxEffExtra.h"

#include "../NpcTalk/NpcDialogue.h"
#include "../NpcTalk/NpcTalkControl.h"
#include "../NpcTalk/NpcTalk.h"

/*ABL system, Juver, 2017/06/02 */
#include "./GLStrikeSelector.h"

/*game stats, Juver, 2017/06/21 */
#include "../../Lib_ClientUI/Interface/GameStats.h"

/*pvp tyranny, Juver, 2017/08/24 */
#include "./GLPVPTyrannyClient.h" 

/*bike color , Juver, 2017/11/15 */
#include "./GLBikeColorDef.h"

/*school wars, Juver, 2018/01/19 */
#include "./GLPVPSchoolWarsClient.h" 

/*pvp capture the flag, Juver, 2018/01/31 */
#include "./GLPVPCaptureTheFlagClient.h"
#include "./GLPVPCaptureTheFlagDefine.h"

/* pvp club death match, Juver, 2020/11/26 */
#include "./PVPClubDeathMatchClient.h"

/*skill transform, Juver, 2018/09/09 */
#include "./GLTransformSkinSet.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSROption.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "./RTimerDefine.h"

#include "../dxincommand.h"

/* post system, Juver, 2021/02/05 */
#include "./GLPostData.h"

#include "./GLAwardTitleClient.h"

#include "./GLSkillPurchase.h"
#include "./UserTypeDefine.h"
#include "./GLAutoPotion.h"
#include "./GLAutoPilotC.h"

#include "./PVPPartyBattleGroundsClient.h"

/*woe Arc Development 08-06-2024*/
#include "GLPVPWoeClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static BOOL ISFILE_CPS_EXIST ( CString strFile )
{
	CString strFullName;
	strFullName = DxSkinPieceContainer::GetInstance().GetPath() + strFile;

	BOOL bEXIST = FALSE;
 	FILE *file = fopen ( strFullName.GetString(), "r" );
	if ( file )
	{
		bEXIST = TRUE;
		fclose(file);
	}

	return bEXIST;
}

namespace COMMENT
{
	std::string szEMREACTION[REACT_SIZE+1] =
	{
		"REACT_MOB",
		"REACT_NPC",
		"REACT_P2P",
		"REACT_PVP",
		"REACT_ITEM",
		"REACT_SKILL",
		"REACT_GATHERING",
		"REACT_TOGGLE_VEHICLE",
		"REACT_SWAP_ARMS",
		"REACT_NULL",
	};
};

namespace
{
	class CFINDER
	{
	public:
		enum { BUFFER = 36, };
	protected:
		std::vector<STARGETID*>		m_vecTAR;

	public:
		CFINDER()
		{
			m_vecTAR.reserve(BUFFER);
		}

	public:
		void CLEAR ()
		{
			if ( !m_vecTAR.empty() )
			{
				m_vecTAR.erase ( m_vecTAR.begin(), m_vecTAR.end() );
			}
		}

	public:
		STARGETID& FindClosedCrow ( const DETECTMAP_RANGE &mapRange, const D3DXVECTOR3 vFromPt )
		{
			GASSERT(mapRange.first!=mapRange.second);

			CLEAR();
			std_afunc::CCompareTargetDist sCOMP(vFromPt);

			for ( DETECTMAP_ITER pos=mapRange.first; pos!=mapRange.second; ++pos )
			{
				m_vecTAR.push_back ( &(*pos) );
			}

			std::vector<STARGETID*>::iterator found = std::min_element ( m_vecTAR.begin(), m_vecTAR.end(), sCOMP );

			return *(*found);
		}

		static CFINDER& GetInstance()
		{
			static CFINDER Instance;
			return Instance;
		}
	};

	STARGETID* FindCrow ( const DETECTMAP_RANGE &mapRange, const STARGETID &_starid )
	{
		static STARGETID sTARID;

		GASSERT(mapRange.first!=mapRange.second);

		DETECTMAP_ITER iter = mapRange.first;
		for ( ; iter!=mapRange.second; iter++ )
		{
			sTARID = (*iter);

			/*skill summon, Juver, 2017/10/11 */
			if ( sTARID.emCrow == CROW_SUMMON )
			{
				PGLANYSUMMON psummon_any = GLGaeaClient::GetInstance().GetSummon ( sTARID.dwID );
				if ( psummon_any )
				{
					PGLCHARCLIENT pchar_client = GLGaeaClient::GetInstance().GetChar ( psummon_any->m_dwOwner );
					if ( pchar_client )
					{
						if ( pchar_client->GetCrow()==_starid.emCrow && pchar_client->GetCtrlID()==_starid.dwID )	
							return &sTARID;
					}
				}
			}
			else
			{
				if ( sTARID.emCrow==_starid.emCrow && sTARID.dwID==_starid.dwID )	return &sTARID;
			}
			
		}

		return NULL;
	}

	STARGETID* FindCrow ( const DETECTMAP_RANGE &mapRange, SCONFTING_CLT::CONFT_MEM &setconftMember )
	{
		static STARGETID sTARID;

		GASSERT(mapRange.first!=mapRange.second);

		DETECTMAP_ITER iter = mapRange.first;
		for ( ; iter!=mapRange.second; iter++ )
		{
			sTARID = (*iter);

			/*skill summon, Juver, 2017/10/11 */
			if ( sTARID.emCrow == CROW_SUMMON )
			{
				PGLANYSUMMON psummon_any = GLGaeaClient::GetInstance().GetSummon ( sTARID.dwID );
				if ( psummon_any )
				{
					PGLCHARCLIENT pchar_client = GLGaeaClient::GetInstance().GetChar ( psummon_any->m_dwOwner );
					if ( pchar_client )
					{
						SCONFTING_CLT::CONFT_MEM_ITER iter = setconftMember.find ( pchar_client->GetCtrlID() );
						if ( iter!=setconftMember.end() )	return &sTARID;
					}
				}
			}
			else if ( sTARID.emCrow==CROW_PC )
			{
				SCONFTING_CLT::CONFT_MEM_ITER iter = setconftMember.find ( sTARID.dwID );
				if ( iter!=setconftMember.end() )	return &sTARID;
			}
		}

		return NULL;
	}

	STARGETID* FindCrowCID ( const DETECTMAP_RANGE &mapRange, SCONFTING_CLT::CONFT_MEM &setconftMember )
	{
		static STARGETID sTARID;

		GASSERT(mapRange.first!=mapRange.second);

		DETECTMAP_ITER iter = mapRange.first;
		for ( ; iter!=mapRange.second; iter++ )
		{
			sTARID = (*iter);

			/*skill summon, Juver, 2017/10/11 */
			if ( sTARID.emCrow == CROW_SUMMON )
			{
				PGLANYSUMMON psummon_any = GLGaeaClient::GetInstance().GetSummon ( sTARID.dwID );
				if ( psummon_any )
				{
					PGLCHARCLIENT pchar_client = GLGaeaClient::GetInstance().GetChar ( psummon_any->m_dwOwner );
					if ( pchar_client )
					{
						SCONFTING_CLT::CONFT_MEM_ITER iter = setconftMember.find ( pchar_client->GetCharData().dwCharID );
						if ( iter!=setconftMember.end() )	return &sTARID;
					}
				}
			}
			else if ( sTARID.emCrow==CROW_PC )
			{
				PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTARID.dwID );

				if ( pCHAR )
				{
					SCONFTING_CLT::CONFT_MEM_ITER iter = setconftMember.find ( pCHAR->GetCharData().dwCharID );
					if ( iter!=setconftMember.end() )	return &sTARID;
				}
			}
		}

		return NULL;
	}
};

//delay movement client to field update
//const float GLCharacter::m_fELAPS_MOVE = 0.2f;
const float GLCharacter::m_fELAPS_MOVE = 0.1f;

GLCharacter::GLCharacter () :
	m_pd3dDevice(NULL),
	m_pSkinChar(NULL),

	m_vDir(D3DXVECTOR3(0,0,-1)),
	m_vDirOrig(D3DXVECTOR3(0,0,-1)),
	m_vPos(D3DXVECTOR3(0,0,0)),

	m_vServerPos(0,0,0),

// 	m_bPASSIVITY_ACTION(FALSE),
// 	m_fPASSIVITY_ACTION_ELAPS(0.0f),
// 	m_fPASSIVITY_ACTION_TOTAL(0.0f),
// 
// 	m_bPASSIVITY_ACTION_CHECK(FALSE),
// 	m_fPASSIVITY_ACTION_CHECK(0.0f),
// 
// 	m_fKeyDownCheckTime(0.0f),
// 	m_bOneHourNotInputKey(FALSE),

	m_Action(GLAT_IDLE),
	m_dwActState(NULL),

	/*multi skill Juver, 2018/09/04 */
	m_wTARGET_NUM_MAIN(0),
	m_wTARGET_NUM_SUB(0),
	m_vTARGET_POS_MAIN(0,0,0),
	m_vTARGET_POS_SUB(0,0,0),

	m_fLastMsgMoveSend(0.0f),

	m_fIdleTime(0.0f),
	m_fattTIMER(0.0f),
	m_nattSTEP(0),
	m_fAccumTime(0.0f),
	m_dwWAIT(0),

	m_sRunSkill(NATIVEID_NULL()),
	m_sActiveSkill(NATIVEID_NULL()),
	m_dwANISUBCOUNT(0),
	m_dwANISUBSELECT(0),
	m_dwANISUBGESTURE(0),

	m_emOldQuestionType(QUESTION_NONE),

	m_dwGaeaID(0),

	m_wInvenPosX1(0),
	m_wInvenPosY1(0),
	m_wInvenPosX2(0),
	m_wInvenPosY2(0),

	/*item wrapper, Juver, 2018/01/12 */
	m_wInvenPosX3(0),
	m_wInvenPosY3(0),

	m_bCLUB_CERTIFY(FALSE),
	m_fCLUB_CERTIFY_TIMER(0),

	m_wPMPosX(USHRT_MAX),
	m_wPMPosY(USHRT_MAX),

	m_vMaxOrg(6,20,6),
	m_vMinOrg(-6,0,-6),

	m_vMax(6,20,6),
	m_vMin(-6,0,-6),
	m_fHeight(20.f),
	m_dwNPCID(0),

	//m_dwSummonGUID(GAEAID_NULL),

	m_bEnableHairSytle(FALSE),
	m_bEnableHairColor(FALSE),
	m_bEnableFaceStyle(FALSE),

	m_bAttackable(TRUE),
	m_llPetCardGenNum(0),
	m_sPetCardNativeID(NATIVEID_NULL()),
	m_cPetCardGenType(-1),
	m_bRecievedPetCardInfo(FALSE),
	m_bIsPetActive(FALSE),
	m_bMiniGameOpen(FALSE),
//	m_bWeaponSwitching(FALSE),	
	m_hCheckStrDLL( NULL ),
	m_pCheckString( NULL ),
	m_bReqVehicle ( FALSE ),
	m_bIsVehicleActive ( FALSE ),
	m_bRecivedVehicleItemInfo ( FALSE ),
	m_bGarbageOpen ( false ),
	m_bItemShopOpen ( false ),
	m_bReqAttendList( false ),
	m_dwComboAttend(0),
	m_tAttendLogin(0),
	m_dwAttendTime(0),

	m_bRecordChat(FALSE),
	m_strRecordChat(""),

	m_dwActionAnim(AN_SUB_00_SIZE), /*dash skill logic, Juver, 2017/06/17 */

	/*rightclick wear/unwear, Juver, 2017/06/23 */
	m_bRingSlotFirst(false), 
	m_fWearDelay(1.0f),

	/*pet fix add call delay, EJCode, 2018/11/28 */
	m_fPetUseDelay(0.0f),

	/*npc shop, Juver, 2017/07/27 */
	m_sNPCShopCrow(NATIVEID_NULL()),
	m_sNPCShopItem(NATIVEID_NULL()),
	m_wNPCShopType(0),
	m_fNPCShopDelay(NPC_SHOP_DELAY),

	/*item exchange, Juver, 2017/10/13 */
	m_fNPCItemExchangeDelay(NPC_ITEM_EXCHANGE_DELAY),

	/*product item, Juver, 2017/10/18 */
	m_dwProductID(UINT_MAX),
	m_bItemCompoundTask(FALSE),
	m_dwItemCompoundTaskID(UINT_MAX),
	m_fItemCompoundTaskTime(0.0f),
	m_fItemCompoundTaskTimer(0.0f),

	/*activity system, Juver, 2017/11/05 */
	m_fCharTitleDelay(0.0f),

	/*charinfoview , Juver, 2017/11/11 */
	m_fReqCharInfoTimer(REQ_CHARINFO_DELAY_TIME_DEFAULT),

	/*bike color , Juver, 2017/11/16 */
	m_bEnableBikeColor(FALSE),

	/*change scale card, Juver, 2018/01/03 */
	m_fActiveCharScale(1.0f),
	m_bChangeCharScale(FALSE),

	m_wItemColor1(ITEMCOLOR_WHITE),
	m_wItemColor2(ITEMCOLOR_WHITE),
	m_bEnableItemColor(FALSE),
	m_emSlotItemColor(SLOT_TSIZE),

	/*pvp capture the flag, Juver, 2018/02/07 */
	m_fCaptureTheFlagHoldFlagTimer(0.0f),
	m_bCaptureTheFlagLocatorOn(FALSE),

	/* car, cart color, Juver, 2018/02/14 */
	m_bEnableCarColor(FALSE),
	m_wCarColorA(ITEMCOLOR_WHITE),
	m_wCarColorB(ITEMCOLOR_WHITE),

	/*ep8 puton slots, Juver, 2018/03/29 */
	m_bAccessorySlotFirst(false),

	/*modern inventory window, Juver, 2018/04/09 */
	m_wInventoryUsePosX(USHRT_MAX),
	m_wInventoryUsePosY(USHRT_MAX),
	m_bGarbageUseCard(TRUE),

	/*item random option rebuild, Juver, 2018/07/04 */
	m_bItemRandomOptionRebuildOptionLock0(FALSE),
	m_bItemRandomOptionRebuildOptionLock1(FALSE),
	m_bItemRandomOptionRebuildOptionLock2(FALSE),
	m_bItemRandomOptionRebuildOptionLock3(FALSE),

	/*inventory sort, Juver, 2018/07/07 */
	m_fInventorySortTimer(0.0f),

	/*buffs manual remove, Juver, 2018/08/14 */
	m_buff_remove_id(false),

	/*item drop question, Juver, 2018/08/22 */
	m_vdrop_pos_temp(D3DXVECTOR3(0,0,0)),

	/*item bank question, Juver, 2018/08/22 */
	m_item_bank_pos_x(0),
	m_item_bank_pos_y(0),

	/*specific item box, Juver, 2018/09/03 */
	m_item_box_pos_x(0),
	m_item_box_pos_y(0),

	/*extreme auto tab, Juver, 2018/09/10 */
	m_skill_change_weapon_tab_pending(FALSE),
	m_skill_change_weapon_tab_pending_time(0.0f),

	/*manual lunchbox remove, Juver, 2018/09/19 */
	m_lunchbox_remove_id(false),

	/*item random option rebuild, Juver, 2018/07/03 */
	m_item_random_option_rebuild_timer(0.0f),
	m_item_random_option_rebuild_active_task(FALSE),

	///*hide costume, EJCode, 2018/11/18 */
	//m_hide_costume(false),
	//m_hide_costume_timer(0.0f),

	m_fPetLastUseTime(PET_LAST_USE_TIME_DEFAULT),
	m_fPetBlockTime(PET_BLOCK_TIME_DEFAULT),

	m_fCurrencyShopDelayTimer(CURRENCY_SHOP_TIME_DEFAULT),

	/* party finder, Juver, 2020/01/03 */
	m_fPartyFinderSearchDelayTimer(PARTY_FINDER_SEARCH_DELAY_TIME_DEFAULT),
	m_fPartyFinderJoinDelayTimer(PARTY_FINDER_JOIN_DELAY_TIME_DEFAULT),

	/* skill turn/find invisible, Juver, 2020/04/26 */
	m_bSkillTurnInvisibleLast(false),
	m_wSkillTurnInvisibleAnimationLast(0),
	m_sSkillTurnInvisibleIDLast(false),

	/*game stats, Juver, 2017/06/21 */
	m_bRecievedPingAgent(true),
	m_bRecievedPingField(true),

	/* tyranny AFK disconnect, Juver, 2021/08/07 */
	m_fTyrannyAFKTimer(0.0f),
	m_fCaptureTheFlagAFKTimer(0.0f),

	m_sNPCContributionShopCrow(false),
	m_sNPCContributionShopItem(false),
	m_fNPCContributionShopDelay(NPC_CONTRIBUTION_SHOP_DELAY),

	m_fMaxRVPreviewTimer(MAX_RV_PREVIEW_COOLTIME),

	m_pAutoPotion(NULL),

	m_bExchangeItemOpen(false),

	m_fCaptchaWaitTimer(0.0f),
	m_bCaptchaWait(false),
	/*dmk14 pk safetime*/
	//m_fMoveDelay( 0.0f),
	
	/*12-9-14, Battle Pass - CNDev*/
	m_fBattlePassDelay(BATTLEPASS_DELAY),

	/*12-16-24, daily login - CNDev*/
	m_fDailyLoginRemain(0.0f),
	m_bDailyLoginRemain(false),
	m_fDailyLoginDelay(DAILYLOGIN_DELAY)
{	
	D3DXMatrixIdentity ( &m_matTrans );
	for ( int i=0; i<EMSTORAGE_CHANNEL; ++i )		m_dwNumStorageItem[i] = UINT_MAX;

	for( int i = 0; i < 8; i++ )
	{
		m_bOldVisibleTracingUI[i] = FALSE;
		m_bNewVisibleTracingUI[i] = FALSE;
	}

	m_fPrintProcessInfoMsgGap = -10.0f;

	m_vecTAR_TEMP.reserve(220);

	SetSTATE(EM_ACT_RUN);

	m_mapPETCardInfo.clear();
	m_mapPETCardInfoTemp.clear();

	m_mapPETReviveInfo.clear();

	m_mapVEHICLEItemInfo.clear();
	m_mapVEHICLEItemInfoTemp.clear();

	m_vecAttend.clear();

	m_vecMarketClick.clear();

#ifdef TH_PARAM
	HMODULE m_hCheckStrDLL = LoadLibrary("ThaiCheck.dll");

	if ( m_hCheckStrDLL )
	{
		m_pCheckString = ( BOOL (_stdcall*)(CString)) GetProcAddress(m_hCheckStrDLL, "IsCompleteThaiChar");
	}
#endif 

	/*skill summon, Juver, 2017/10/08 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		m_dwSummonGUID[i] = GAEAID_NULL;

	/*bike color , Juver, 2017/11/13 */
	for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
		m_wBikeColor[i] = 0;

	/* chaos machine, Juver, 2021/07/09 */
	m_dwChaosMachineNPCID = 0;

	m_sMaxRvPreview.Reset();

	m_pAutoPotion = new GLAutoPotion;
	m_pAutoPotion->Reset();

	m_pAutoPilot = new GLAutoPilotC(this);
	m_pAutoPilot->Reset();
}

GLCharacter::~GLCharacter ()
{

	SAFE_DELETE(m_pSkinChar);

#ifdef TH_PARAM
	if( m_hCheckStrDLL ) FreeLibrary( m_hCheckStrDLL );
#endif

	delete m_pAutoPotion;
	m_pAutoPotion = NULL;

	delete m_pAutoPilot;
	m_pAutoPilot = NULL;
}

float GLCharacter::GetBuyRate ()
{
	float fBUY_RATE = GET_PK_SHOP2BUY();

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLand )
	{
		fBUY_RATE += pLand->m_fCommission;
	}

	return fBUY_RATE;
}

float GLCharacter::GetSaleRate ()
{
	float fSALE_RATE = GET_PK_SHOP2SALE ();
	
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLand )
	{
		fSALE_RATE -= pLand->m_fCommission;
	}

	return fSALE_RATE;
}

void GLCharacter::ResetData ()
{
	int i = 0;

	GLCHARLOGIC_CLIENT::RESET_DATA();

	m_vDir = D3DXVECTOR3(0,0,-1);
	m_vDirOrig = D3DXVECTOR3(0,0,-1);
	m_vPos = D3DXVECTOR3(0,0,0);
	D3DXMatrixIdentity ( &m_matTrans );

	m_Action = GLAT_IDLE;
	m_dwActState = NULL;
	SetSTATE(EM_ACT_RUN);

	m_sTargetID = STARGETID(CROW_MOB,EMTARGET_NULL,D3DXVECTOR3( 0, 0, 0 ));

	/*multi skill Juver, 2018/09/04 */
	m_wTARGET_NUM_MAIN = 0;
	m_wTARGET_NUM_SUB = 0;
	m_vTARGET_POS_MAIN = D3DXVECTOR3(0,0,0);
	m_vTARGET_POS_SUB = D3DXVECTOR3(0,0,0);
	for ( i=0; i<EMTARGET_NET; ++i )
	{
		m_sTARGET_IDS_MAIN[i] = STARID(CROW_MOB,EMTARGET_NULL);
		m_sTARGET_IDS_SUB[i] = STARID(CROW_MOB,EMTARGET_NULL);
	}

	m_sRunSkill = SNATIVEID(false);
	m_sActiveSkill = SNATIVEID(false);
	m_dwANISUBCOUNT = 0;
	m_dwANISUBSELECT = 0;
	m_dwANISUBGESTURE = 0;

	m_fLastMsgMoveSend = 0.0f;
	m_sLastMsgMove = GLMSG::SNETPC_GOTO();
	m_sLastMsgMoveSend = GLMSG::SNETPC_GOTO();

	m_sREACTION.RESET();

	m_fIdleTime = 0.0f;
	m_fattTIMER = 0.0f;
	m_nattSTEP = 0;
	m_fAccumTime = 0.0f;
	m_dwWAIT = 0;

	m_bRecordChat = FALSE;

	m_strRecordChat = "";

	m_dwGaeaID = 0;

	for ( i=0; i<EMSTORAGE_CHANNEL; ++i )		m_dwNumStorageItem[i] = UINT_MAX;
	m_sCONFTING.RESET();

	m_sPMarket.DoMarketClose();

	m_bCLUB_CERTIFY = FALSE;
	m_fCLUB_CERTIFY_TIMER = 0;

	m_sCLUB.RESET();

	m_vMaxOrg = D3DXVECTOR3(6,20,6);
	m_vMinOrg = D3DXVECTOR3(-6,0,-6);

	m_vMax = D3DXVECTOR3(6,20,6);
	m_vMin = D3DXVECTOR3(-6,0,-6);

	m_dwNPCID = 0;

	m_bAttackable = TRUE;

	m_mapPETCardInfo.clear();
	m_mapPETCardInfoTemp.clear();
	m_bRecievedPetCardInfo = FALSE;
	m_bIsPetActive		   = FALSE;

	m_mapPETReviveInfo.clear();

	m_mapVEHICLEItemInfo.clear();
	m_mapVEHICLEItemInfoTemp.clear();

	m_bReqVehicle			  = FALSE;
	m_bIsVehicleActive		  = FALSE;
	m_bRecivedVehicleItemInfo = FALSE;

	for( i = 0; i < 8; i++ )
	{
		m_bOldVisibleTracingUI[i] = FALSE;
		m_bNewVisibleTracingUI[i] = FALSE;
	}

	m_fPrintProcessInfoMsgGap = -10.0f;

	m_sVehicle.RESET();

	m_bGarbageOpen = false;
	m_bItemShopOpen = false;
	m_bReqAttendList = false;
	m_vecAttend.clear();
	m_dwComboAttend = 0;
	m_tAttendLogin = 0;
	m_dwAttendTime = 0;

	m_dwTransfromSkill = SNATIVEID::ID_NULL;

	m_vecMarketClick.clear();

	/*charinfoview , Juver, 2017/11/11 */
	m_fReqCharInfoTimer = RPARAM::fViewCharInfoDelay;

	/*bike color , Juver, 2017/11/13 */
	for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
		m_wBikeColor[i] = 0;

	m_bEnableBikeColor = FALSE;

	/*change scale card, Juver, 2018/01/03 */
	m_fActiveCharScale = 1.0f;
	m_bChangeCharScale = FALSE;

	/*item color, Juver, 2018/01/10 */
	m_wItemColor1 = ITEMCOLOR_WHITE;
	m_wItemColor2 = ITEMCOLOR_WHITE;
	m_bEnableItemColor = FALSE;
	m_emSlotItemColor = SLOT_TSIZE;

	/*pvp capture the flag, Juver, 2018/02/07 */
	m_fCaptureTheFlagHoldFlagTimer = 0.0f;
	m_bCaptureTheFlagLocatorOn = FALSE;

	/* car, cart color, Juver, 2018/02/14 */
	m_bEnableCarColor = FALSE;
	m_wCarColorA = ITEMCOLOR_WHITE;
	m_wCarColorB = ITEMCOLOR_WHITE;

	/*inventory sort, Juver, 2018/07/07 */
	m_fInventorySortTimer = 0.0f;

	/*buffs manual remove, Juver, 2018/08/14 */
	m_buff_remove_id = NATIVEID_NULL().dwID;

	/*item drop question, Juver, 2018/08/22 */
	m_vdrop_pos_temp = D3DXVECTOR3(0.0f,0.0f,0.0f);

	/*item bank question, Juver, 2018/08/22 */
	m_item_bank_pos_x = 0;
	m_item_bank_pos_y = 0;

	/*specific item box, Juver, 2018/09/03 */
	m_item_box_pos_x = 0;
	m_item_box_pos_y = 0;

	/*extreme auto tab, Juver, 2018/09/10 */
	m_skill_change_weapon_tab_pending = FALSE;
	m_skill_change_weapon_tab_pending_time = 0.0f;
	m_skill_change_weapon_tab_pending_skill = GLMSG::SNETPC_REQ_SKILL();
	m_skill_change_weapon_tab_pending_skill.skill_id = NATIVEID_NULL();

	/*manual lunchbox remove, Juver, 2018/09/19 */
	m_lunchbox_remove_id = NATIVEID_NULL().dwID;

	///*hide costume, EJCode, 2018/11/18 */
	//m_hide_costume = false;
	//m_hide_costume_timer = 0.0f;

	/*pet fix add call delay, EJCode, 2018/11/28 */
	m_fPetUseDelay = 0.0f;


	m_fPetLastUseTime = PET_LAST_USE_TIME_DEFAULT;
	m_fPetBlockTime = PET_BLOCK_TIME_DEFAULT;

	m_fCurrencyShopDelayTimer = RPARAM::fCurrencyShopPurchaseDelayTime;

	/* party finder, Juver, 2020/01/03 */
	m_fPartyFinderSearchDelayTimer = RPARAM::fPartyFinderSearchDelay;
	m_fPartyFinderJoinDelayTimer = RPARAM::fPartyFinderJoinDelay;

	/* skill turn/find invisible, Juver, 2020/04/26 */
	m_bSkillTurnInvisibleLast = false;
	m_wSkillTurnInvisibleAnimationLast = 0;
	m_sSkillTurnInvisibleIDLast = NATIVEID_NULL();

	/* chaos machine, Juver, 2021/07/09 */
	m_sChaosMachineItemPos.RESET();
	m_dwChaosMachineNPCID = 0;

	/* tyranny AFK disconnect, Juver, 2021/08/07 */
	m_fTyrannyAFKTimer = 0.0f;

	m_fCaptureTheFlagAFKTimer = 0.0f;


	m_sNPCContributionShopCrow.ResetNativeID();
	m_sNPCContributionShopItem.ResetNativeID();
	m_fNPCContributionShopDelay = NPC_CONTRIBUTION_SHOP_DELAY;

	m_fMaxRVPreviewTimer = MAX_RV_PREVIEW_COOLTIME;

	m_sMaxRvPreview.Reset();

	if ( m_pAutoPotion )
		m_pAutoPotion->Reset();

	if ( m_pAutoPilot )
		m_pAutoPilot->Reset();

	m_bExchangeItemOpen = false;

	m_fCaptchaWaitTimer = 0.0f;
	m_bCaptchaWait = false;

	/*dmk14 pk safetime*/
//	m_fMoveDelay = 0.0f;
	
	/*12-9-14, Battle Pass - CNDev*/
	m_fBattlePassDelay = BATTLEPASS_DELAY;
	
	/*12-16-24, daily login - CNDev*/
	m_fDailyLoginRemain = 0.0f;
	m_bDailyLoginRemain = false;
	m_fDailyLoginDelay = DAILYLOGIN_DELAY;
}

HRESULT GLCharacter::SkinLoad ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[emIndex], pd3dDevice, TRUE );
	if ( !pSkinChar )	return E_FAIL;


	SAFE_DELETE(m_pSkinChar);
	m_pSkinChar = new DxSkinChar;

	m_pSkinChar->SetCharData ( pSkinChar, pd3dDevice, TRUE );

	
	UpdateSuit( TRUE );





	// Note : 1.AABB Box를 가져온다. 2.높이를 계산해 놓는다.
	m_pSkinChar->GetAABBBox( m_vMaxOrg, m_vMinOrg );
	m_fHeight = m_vMaxOrg.y - m_vMinOrg.y;

	//	Note : 에니메이션 초기화.
	//
	if ( m_pSkinChar )
	{
		m_pSkinChar->SELECTANI ( AN_GUARD_N, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );

		if ( !m_pSkinChar->GETCURANIMNODE() )
		{
			CDebugSet::ToLogFile ( "ERORR : current animation node null point error [ %s ] [ M %d S %d ]", m_pSkinChar->m_szFileName, AN_GUARD_N, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
			m_pSkinChar->DEFAULTANI ();
		}
	}

	//	Note : 스킬 버프, 상태이상 효과 생성.
	FACTEFF::ReNewEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS, m_matTrans, m_vDir );

	if( GLCONST_CHAR::cCONSTCLASS[emIndex].strCLASS_EFFECT.size() != 0 )
	{
		DxEffcharDataMan::GetInstance().PutPassiveEffect ( m_pSkinChar, GLCONST_CHAR::cCONSTCLASS[emIndex].strCLASS_EFFECT.c_str(), &m_vDir );
	}

	return S_OK;
}

HRESULT GLCharacter::SkinLoadForTool( LPDIRECT3DDEVICEQ pd3dDevice, const char* szFile )
{
	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( szFile, pd3dDevice, TRUE );
	if ( !pSkinChar )	return E_FAIL;


	SAFE_DELETE(m_pSkinChar);
	m_pSkinChar = new DxSkinChar;

	m_pSkinChar->SetCharData ( pSkinChar, pd3dDevice, TRUE );


//	UpdateSuit( TRUE );





	// Note : 1.AABB Box를 가져온다. 2.높이를 계산해 놓는다.
	m_pSkinChar->GetAABBBox( m_vMaxOrg, m_vMinOrg );
	m_fHeight = m_vMaxOrg.y - m_vMinOrg.y;

	//	Note : 에니메이션 초기화.
	//
	if ( m_pSkinChar )
	{
		m_pSkinChar->SELECTANI ( AN_GUARD_N, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );

		if ( !m_pSkinChar->GETCURANIMNODE() )
		{
			CDebugSet::ToLogFile ( "ERORR : current animation node null point error [ %s ] [ M %d S %d ]", m_pSkinChar->m_szFileName, AN_GUARD_N, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
			m_pSkinChar->DEFAULTANI ();
		}
	}

	//	Note : 스킬 버프, 상태이상 효과 생성.
	FACTEFF::ReNewEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS, m_matTrans, m_vDir );

	if( GLCONST_CHAR::cCONSTCLASS[emIndex].strCLASS_EFFECT.size() != 0 )
	{
		DxEffcharDataMan::GetInstance().PutPassiveEffect ( m_pSkinChar, GLCONST_CHAR::cCONSTCLASS[emIndex].strCLASS_EFFECT.c_str(), &m_vDir );
	}

	return S_OK;
}


HRESULT GLCharacter::Create ( NavigationMesh* pNavi, SCHARDATA2 *pCharData2, D3DXVECTOR3* pvPos, DWORD dwGaeaID, LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT(pd3dDevice);
	HRESULT hr;

	m_pd3dDevice = pd3dDevice;
	m_dwGaeaID = dwGaeaID;

	m_dwTransfromSkill = SNATIVEID::ID_NULL;

	//	Note : 케릭터의 외형을 설정.
	if ( pCharData2 )										SCHARDATA2::Assign ( *pCharData2 );
	else if ( GLCONST_CHAR::VAID_CHAR_DATA2(0,GLCI_SWORDSMAN_M) )	SCHARDATA2::Assign ( GLCONST_CHAR::GET_CHAR_DATA2(0,GLCI_SWORDSMAN_M) );

    m_cInventory.SetAddLine ( GetOnINVENLINE(), true );
	CInnerInterface::GetInstance().SetInventorySlotViewSize ( EM_INVEN_DEF_SIZE_Y + GetOnINVENLINE() );

	// 베트남 인벤토리는 세로가 10칸이므로 6칸을 더 추가한다.
#if defined(VN_PARAM) //vietnamtest%%%
	m_cVietnamInventory.SetAddLine ( 6, true );
#endif	

	GLCHARLOGIC_CLIENT::INIT_DATA ( FALSE, TRUE );
	m_sCONFTING.RESET();

	//	초기 런 스킬 지정.
	if ( m_wSKILLQUICK_ACT < EMSKILLQUICK_SIZE )
	{
		m_sRunSkill = m_sSKILLQUICK[m_wSKILLQUICK_ACT];
	}

	hr = SkinLoad ( pd3dDevice );
	if ( FAILED(hr) )	return hr;


	//	Note : 이동 제어 초기화.
	if ( pvPos )	m_vPos = *pvPos;
	else			m_vPos = m_vStartPos;
	
	//	Note : 형상 위치 초기화.
	D3DXMatrixTranslation ( &m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z );

	m_actorMove.SetMaxSpeed ( GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fWALKVELO );
	if ( pNavi )
	{
		m_actorMove.Create ( pNavi, m_vPos, -1 );
	}

	DoActWait ();

	GLQuestPlay::MAPQUEST& mapQuest = m_cQuestPlay.GetQuestProc ();
	{
		bool bKNOCK(false);

		GLQuestPlay::MAPQUEST_ITER iter = mapQuest.begin();
		GLQuestPlay::MAPQUEST_ITER iter_end = mapQuest.end();
		for ( ; iter!=iter_end; ++iter )
		{
			GLQUESTPROG *pPROG = (*iter).second;
			if ( !pPROG->IsReqREADING () )	continue;

			bKNOCK = true;

			GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pPROG->m_sNID.dwID );
			if ( pQUEST )
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("QUEST_KNOCK"), pQUEST->GetTITLE() );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("QUEST_NOINFO"), pPROG->m_sNID.dwID );
		}

		if ( bKNOCK )
		{
			CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("QUEST_KNOCK_END") );
		}
	}

	GLFriendClient::GetInstance().Clear();

	/* award title, Juver, 2021/02/12 */
	GLAwardTitleClient::GetInstance().Clear();


	GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ NetMsg;
	NetMsg.dwGaeaID	= m_dwGaeaID;
	NETSEND ( &NetMsg );

#if defined(VN_PARAM) //vietnamtest%%%
	// 이동 할 때마다 베트남 탐닉 시스템 시간을 요청한다.
	GLMSG::SNETPC_VIETNAM_TIME_REQ NetVietnamMsg;
	NetMsg.dwGaeaID	= m_dwGaeaID;	
	NETSENDTOFIELD ( &NetVietnamMsg );
#endif


	PLANDMANCLIENT pLandMClient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLandMClient )								return E_FAIL;

	SNATIVEID mapID = pLandMClient->GetMapID();
	if( pLandMClient->IsInstantMap() ) mapID.wSubID = 0;

	SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( mapID );
	if ( pMapNode )
	{
		GLLevelFile cLevelFile;
		if( cLevelFile.LoadFile ( pMapNode->strFile.c_str(), TRUE, NULL ) )
		{
			SLEVEL_ETC_FUNC* pEtcFunc = cLevelFile.GetLevelEtcFunc();
			if( !pEtcFunc )	return E_FAIL;
			if( pEtcFunc->m_bUseFunction[EMETCFUNC_CONTROLCAM] )
			{
				D3DXVECTOR3 vFromPt, vLookAtPt, vUpPt;
				DxViewPort::GetInstance().GetViewTrans( vFromPt, vLookAtPt, vUpPt );
				vFromPt = pEtcFunc->m_sControlCam.vCamPos + vLookAtPt;

				DxViewPort::GetInstance().SetViewTrans( vFromPt, vLookAtPt, vUpPt );
				DxViewPort::GetInstance().SetCameraControl( TRUE, pEtcFunc->m_sControlCam.vCamPos, pEtcFunc->m_sControlCam.fUp, 
																	pEtcFunc->m_sControlCam.fDown, pEtcFunc->m_sControlCam.fLeft, 
																	pEtcFunc->m_sControlCam.fRight, pEtcFunc->m_sControlCam.fFar, 
																	pEtcFunc->m_sControlCam.fNear );
			}else{
				DxViewPort::GetInstance().SetCameraControl( FALSE );
			}
		}

	}

	if ( m_pAutoPotion )
		m_pAutoPotion->Reset();

	if ( m_pAutoPilot )
		m_pAutoPilot->Reset();

	// Admin Auto Invi - Jxyy
	if ( RPARAM::bAutoInviGM )
	{
		if ( m_dwUserLvl > NSUSER_TYPE::USER_TYPE_MASTER )
		{
			ReqVisibleOff();
		}
	}

	CODEX_STATS(1,1); /* Codex, Archie 02/16/24 */
	ReqRankMark( ROPTION::bShowRankMark );
	ReqRankName( ROPTION::bShowRankName );
	
	return S_OK;
}

BOOL GLCharacter::IsVALID_STORAGE ( DWORD dwChannel )
{
	if ( dwChannel >= EMSTORAGE_CHANNEL )	return FALSE;
	return m_bStorage[dwChannel];
}

const SITEMCUSTOM& GLCharacter::GET_PRETRADE_ITEM ()
{
	static SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();

	SINVEN_POS sPreTradeItem = GLTradeClient::GetInstance().GetPreItem();

	if ( !sPreTradeItem.VALID() )		return sItemCustom;

	SINVENITEM* pResistItem = m_cInventory.GetItem ( sPreTradeItem.wPosX, sPreTradeItem.wPosY );
	if ( !pResistItem )					return sItemCustom;

	sItemCustom = pResistItem->sItemCustom;

	return sItemCustom;
}

void GLCharacter::ReSelectAnimation ()
{
	// 제스처중이면 애니메이션 리셋 안함.
	if ( !IsACTION ( GLAT_TALK ) && !IsACTION( GLAT_GATHERING ) )
	{
		//	Note : 에니메이션 초기화.
		//

		/*vehicle system, Juver, 2017/08/07 */
		if ( m_bVehicle )
		{
			int emType = m_sVehicle.m_emTYPE;
			if ( emType == VEHICLE_TYPE_BOARD )
			{
				m_emANISUBTYPE = (EMANI_SUBTYPE) ( AN_SUB_HOVERBOARD );
				if ( m_pSkinChar->GETCURSTYPE() !=GLCHARLOGIC_CLIENT::m_emANISUBTYPE )
					m_pSkinChar->SELECTANI ( m_pSkinChar->GETCURMTYPE(), GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
			}else{
				m_emANISUBTYPE = CHECK_VEHICLE_SUB( m_pSkinChar->GETCURMTYPE() );
				if ( m_pSkinChar->GETCURSTYPE() !=GLCHARLOGIC_CLIENT::m_emANISUBTYPE )
					m_pSkinChar->SELECTANI ( CHECK_VEHICLE_MAIN( m_sVehicle.m_sVehicleID ), GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
			}
		}else{
			if ( m_pSkinChar->GETCURSTYPE() !=GLCHARLOGIC_CLIENT::m_emANISUBTYPE )
				m_pSkinChar->SELECTANI ( m_pSkinChar->GETCURMTYPE(), GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
		}
	}
}

HRESULT GLCharacter::DoActWait ()
{
	m_dwWAIT = 0;
	SetSTATE(EM_ACT_WAITING);

	return S_OK;
}

HRESULT GLCharacter::ReBirth ( WORD wHP, WORD wMP, WORD wSP, D3DXVECTOR3 vPos, bool bWait )
{
	FACTEFF::DeleteEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS );

	//	부활시 채력치 회복값.
	INIT_RECOVER ();

	m_sHP.wNow = wHP;
	m_sMP.wNow = wMP;
	m_sSP.wNow = wSP;

	//	이동된 멥에 적응.
	MoveActiveMap ( GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh(), vPos, bWait );

	TurnAction ( GLAT_IDLE );

	D3DXMATRIX matEffect;
	D3DXMatrixTranslation ( &matEffect, m_vPos.x, m_vPos.y, m_vPos.z );

	STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
	DxEffGroupPlayer::GetInstance().NewEffGroup
	(
		GLCONST_CHAR::strREBIRTH_EFFECT.c_str(),
		matEffect,
		&sTargetID
	);

	ReSetSTATE(EM_ACT_DIE);

	return S_OK;
}

HRESULT GLCharacter::MoveActiveMap ( NavigationMesh* pNavi, const D3DXVECTOR3 &vPos, bool bWait )
{
	if ( bWait ) DoActWait ();

	//	Note : 이동 제어 초기화.
	m_vPos = vPos;

	m_actorMove.Stop ();
	if ( pNavi )
	{
		m_actorMove.Create ( pNavi, m_vPos, -1 );
		SetPosition ( m_vPos );
	}

	ReSetSTATE(EM_REQ_GATEOUT);

	// 이동 할 때마다 이벤트 시간을 요청한다.
	GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ NetMsg;
	NetMsg.dwGaeaID	= m_dwGaeaID;
	NETSEND ( &NetMsg );

#if defined(VN_PARAM) //vietnamtest%%%
	// 이동 할 때마다 베트남 탐닉 시스템 시간을 요청한다.
	GLMSG::SNETPC_VIETNAM_TIME_REQ NetVietnamMsg;
	NetMsg.dwGaeaID	= m_dwGaeaID;	
	NETSENDTOFIELD ( &NetVietnamMsg );
#endif

	PLANDMANCLIENT pLandMClient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLandMClient )								return E_FAIL;

	SNATIVEID mapID = pLandMClient->GetMapID();
	if( pLandMClient->IsInstantMap() ) mapID.wSubID = 0;

	SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( mapID );
	if ( pMapNode )
	{
		GLLevelFile cLevelFile;
		if( cLevelFile.LoadFile ( pMapNode->strFile.c_str(), TRUE, NULL ) )

		{
			SLEVEL_ETC_FUNC* pEtcFunc = cLevelFile.GetLevelEtcFunc();
			if( !pEtcFunc )	return E_FAIL;
			if( pEtcFunc->m_bUseFunction[EMETCFUNC_CONTROLCAM] )
			{
				D3DXVECTOR3 vFromPt, vLookAtPt, vUpPt;
				DxViewPort::GetInstance().GetViewTrans( vFromPt, vLookAtPt, vUpPt );
				vFromPt = pEtcFunc->m_sControlCam.vCamPos + vLookAtPt;

				DxViewPort::GetInstance().SetViewTrans( vFromPt, vLookAtPt, vUpPt );
				DxViewPort::GetInstance().SetCameraControl( TRUE, pEtcFunc->m_sControlCam.vCamPos, pEtcFunc->m_sControlCam.fUp, 
																	pEtcFunc->m_sControlCam.fDown, pEtcFunc->m_sControlCam.fLeft, 
																	pEtcFunc->m_sControlCam.fRight, pEtcFunc->m_sControlCam.fFar, 
																	pEtcFunc->m_sControlCam.fNear );
			}else{
				DxViewPort::GetInstance().SetCameraControl( FALSE );
			}
		}

	}


	return S_OK;
}

// *****************************************************
// Desc: 장착한 아이템 업데이트
// *****************************************************
HRESULT GLCharacter::UpdateSuit( BOOL bChangeHair, BOOL bChangeFace )
{
	GASSERT(m_pd3dDevice);

	/*skill transform, Juver, 2018/09/09 */
	if ( m_skill_transform_data.is_skin_change_valid() )	return S_OK;

	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().FindData ( GLCONST_CHAR::szCharSkin[emIndex] );
	if ( !pSkinChar )	return E_FAIL;

	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	if ( bChangeFace )
	{
		// 얼굴
		if ( sCONST.dwHEADNUM > m_wFace )
		{
			std::string strHEAD_CPS = sCONST.strHEAD_CPS[m_wFace];
		
			PDXCHARPART pCharPart = NULL;
			pCharPart = m_pSkinChar->GetPiece(PIECE_HEAD);		//	현재 장착 스킨.

			if ( pCharPart && strcmp(strHEAD_CPS.c_str(),pCharPart->m_szFileName) )
			{
				m_pSkinChar->SetPiece ( strHEAD_CPS.c_str(), m_pd3dDevice, 0x0, 0, TRUE );
			}
		}
	}
	// 헤어스타일
	if( bChangeHair )
	{
		if ( sCONST.dwHAIRNUM > m_wHair )
		{
			std::string strHAIR_CPS = sCONST.strHAIR_CPS[m_wHair];
		
			PDXCHARPART pCharPart = NULL;
			pCharPart = m_pSkinChar->GetPiece(PIECE_HAIR);		//	현재 장착 스킨.

			if ( pCharPart && strcmp(strHAIR_CPS.c_str(),pCharPart->m_szFileName) )
			{
				m_pSkinChar->SetPiece ( strHAIR_CPS.c_str(), m_pd3dDevice, 0x0, 0, TRUE );
			}
		}
	}

	m_pSkinChar->SetHairColor( m_wHairColor );

	// 장착한 아이템 업데이트
	for ( int i=0; i<SLOT_NSIZE_S_2; i++ )
	{	
		// 현재 장착중인 무기가 아니면 넘어간다.
		if( !IsCurUseArm( EMSLOT(i) ) ) continue;

		SITEMCUSTOM ItemCustom = m_PutOnItems[i];

		/*ABL system, Juver, 2017/06/02 */
		if ( i == SLOT_LHAND  )	ItemCustom = m_PutOnItems[SLOT_RHAND];
		if ( i == SLOT_LHAND_S  )	ItemCustom = m_PutOnItems[SLOT_RHAND_S];

		// 복장 아이템
		SNATIVEID nidITEM = ItemCustom.nidDISGUISE;
		if ( nidITEM==SNATIVEID(false) )	nidITEM = ItemCustom.sNativeID;

		if ( ItemCustom.nidDISGUISE != SNATIVEID(false) )
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( ItemCustom.nidDISGUISE );
			if ( pItem )
			{
				if( !ISFILE_CPS_EXIST( pItem->GetWearingFileL(emIndex) ) && !ISFILE_CPS_EXIST( pItem->GetWearingFileR(emIndex) ) )
					nidITEM = ItemCustom.sNativeID;
			}
		}

		/*hide costume, EJCode, 2018/11/18 */
		//if ( m_hide_costume && i != SLOT_VEHICLE )
		//	nidITEM = ItemCustom.sNativeID;

		///*character render setting, Juver, 2019/03/23 */
		//if ( RPROFILE::bApplySelf )
		//{
		//	switch( i )
		//	{
		//	case SLOT_HEADGEAR:
		//		{
		//			if (RPROFILE::emRenderHead == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_UPPER:
		//		{
		//			if (RPROFILE::emRenderUpper == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_LOWER:
		//		{
		//			if (RPROFILE::emRenderLower == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_HAND:
		//		{
		//			if (RPROFILE::emRenderGloves == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_FOOT:
		//		{
		//			if (RPROFILE::emRenderShoes == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_RHAND:
		//	case SLOT_LHAND:
		//	case SLOT_RHAND_S:
		//	case SLOT_LHAND_S:
		//		{
		//			if (RPROFILE::emRenderWeapon == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_NECK:
		//		{
		//			if (RPROFILE::emRenderNeckLace == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_WRIST:
		//		{
		//			if (RPROFILE::emRenderBracelet == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_RFINGER:
		//	case SLOT_LFINGER:
		//		{
		//			if (RPROFILE::emRenderRing == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_BELT:
		//		{
		//			if (RPROFILE::emRenderBelt == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_EARRING:
		//		{
		//			if (RPROFILE::emRenderEarring == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;

		//	case SLOT_ACCESSORY_R:
		//	case SLOT_ACCESSORY_L:
		//		{
		//			if (RPROFILE::emRenderAccessory == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;
		//	
		//	case SLOT_DECORATION:
		//		{
		//			if (RPROFILE::emRenderDecoration == ERENDER_SETTING_TYPE_HIDE_DISGUISE )
		//				nidITEM = ItemCustom.sNativeID;
		//		}break;
		//	};
		//}
		
		if ( !m_bVehicle && i == SLOT_VEHICLE  ) nidITEM = SNATIVEID(false);
		else if (  m_bVehicle && i == SLOT_VEHICLE )
		{
			nidITEM = m_sVehicle.GetSkinID();
			if ( nidITEM == SNATIVEID(false) )
			{
				m_sVehicle.m_sVehicleID = ItemCustom.sNativeID;
				nidITEM = ItemCustom.sNativeID;
			}
		}

		///*character render setting, Juver, 2019/03/23 */
		//bool bReset = false;
		//if (RPROFILE::bApplySelf )
		//{
		//	switch( i )
		//	{
		//	case SLOT_NECK:
		//		{
		//			if (RPROFILE::emRenderNeckLace == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;

		//	case SLOT_WRIST:
		//		{
		//			if (RPROFILE::emRenderBracelet == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;

		//	case SLOT_RFINGER:
		//	case SLOT_LFINGER:
		//		{
		//			if (RPROFILE::emRenderRing == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;

		//	case SLOT_BELT:
		//		{
		//			if (RPROFILE::emRenderBelt == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;

		//	case SLOT_EARRING:
		//		{
		//			if (RPROFILE::emRenderEarring == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;

		//	case SLOT_ACCESSORY_R:
		//	case SLOT_ACCESSORY_L:
		//		{
		//			if (RPROFILE::emRenderAccessory == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;

		//	case SLOT_DECORATION:
		//		{
		//			if (RPROFILE::emRenderDecoration == ERENDER_SETTING_TYPE_SIMPLIFIED )
		//				bReset = true;
		//		}break;
		//	};
		//}

		// 유효하지 않으면
		if ( nidITEM == SNATIVEID(false) /*|| bReset*/ )
		{
			//	Note : 기본 스킨과 지금 장착된 스킨이 틀릴 경우.
			//	SLOT->PIECE.
			PDXSKINPIECE pSkinPiece = NULL;	//	기본 스킨.
			PDXCHARPART pCharPart = NULL;	//	현재 장착 스킨.

			/*ABL system, Juver, 2017/06/02 */
			DxAttBoneLink* pBoneLink = NULL;
			DxAttBone* pAttBone = NULL;

			EMPIECECHAR emPiece = SLOT_2_PIECE(EMSLOT(i));
			if ( emPiece!=PIECE_SIZE )
			{
				pSkinPiece = pSkinChar->GetPiece(emPiece);	//	기본 스킨.
				pCharPart = m_pSkinChar->GetPiece(emPiece);	//	현재 장착 스킨.

				/*ABL system, Juver, 2017/06/02 */
				pBoneLink = pSkinChar->GetAttBone(emPiece);
				pAttBone = m_pSkinChar->GetAttBone(emPiece);
			}

			/*ABL system, Juver, 2017/06/02 */
			if ( pBoneLink )
			{
				if ( pAttBone && strcmp( pBoneLink->GetFileName(), pAttBone->GetFileName() ) )
				{
					m_pSkinChar->SetAttBone( pBoneLink, m_pd3dDevice, 0X0, 0, TRUE );
				}
				else if ( !pAttBone )
				{
					m_pSkinChar->SetAttBone( pBoneLink, m_pd3dDevice, 0X0, 0, TRUE );
				}
			}
			else if ( pSkinPiece )
			{
				if ( pCharPart && strcmp(pSkinPiece->m_szFileName,pCharPart->m_szFileName) )
					m_pSkinChar->SetPiece ( pSkinPiece->m_szFileName, m_pd3dDevice, 0X0, 0, TRUE );
			}
			else
			{
				if ( pCharPart )	m_pSkinChar->ResetPiece(emPiece);
			}

			/*upgrade effect, Juver, 2017/09/01 */
			EMPIECECHAR piece_reset = SLOT_2_UPGRADEPIECE( EMSLOT(i) );
			if ( piece_reset != PIECE_SIZE )	
				m_pSkinChar->ResetPiece( piece_reset );
		}
		// 유효하면
		else
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( nidITEM );
			if ( pItem ) 
			{
				/*Item Self Effect, Juver, 2017/06/09 */
				std::string strSelfEffect = pItem->GetSelfBodyEffect();

				/*ABL system, Juver, 2017/06/02 */
				if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )
				{
					std::string strFileName = pItem->GetWearingFileR( emIndex );
					if ( i == SLOT_LHAND || i == SLOT_LHAND_S )	strFileName = pItem->GetWearingFileL( emIndex );

					///*character render setting, Juver, 2019/03/23 */
					//bool bSetSimple = false;
					//if ( RPROFILE::bApplySelf )
					//{
					//	switch( i )
					//	{
					//	case SLOT_HEADGEAR:
					//		{
					//			if (RPROFILE::emRenderHead == ERENDER_SETTING_TYPE_SIMPLIFIED )
					//				bSetSimple = true;
					//		}break;

					//	case SLOT_UPPER:
					//		{
					//			if (RPROFILE::emRenderUpper == ERENDER_SETTING_TYPE_SIMPLIFIED )
					//				bSetSimple = true;
					//		}break;

					//	case SLOT_LOWER:
					//		{
					//			if (RPROFILE::emRenderLower == ERENDER_SETTING_TYPE_SIMPLIFIED )
					//				bSetSimple = true;
					//		}break;

					//	case SLOT_HAND:
					//		{
					//			if (RPROFILE::emRenderGloves == ERENDER_SETTING_TYPE_SIMPLIFIED )
					//				bSetSimple = true;
					//		}break;

					//	case SLOT_FOOT:
					//		{
					//			if (RPROFILE::emRenderShoes == ERENDER_SETTING_TYPE_SIMPLIFIED )
					//				bSetSimple = true;
					//		}break;
					//	};
					//}

					//if ( bSetSimple )
					//{
					//	EMCHARGENDER char_gender = CharIndexToGender( emIndex );
					//	if ( char_gender == GLGENDER_W )
					//		strFileName = GLCONST_CHAR::strSimpleWearF[i];
					//	else if ( char_gender == GLGENDER_M )
					//		strFileName = GLCONST_CHAR::strSimpleWearM[i];
					//}

					DxAttBoneLink* pBoneLink = DxAttBoneLinkContainer::GetInstance().LoadData( strFileName.c_str(), m_pd3dDevice, TRUE );
					if ( pBoneLink )	m_pSkinChar->SetAttBone( pBoneLink, m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE, strSelfEffect.c_str() );
					else	m_pSkinChar->SetPiece ( strFileName.c_str(), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE, strSelfEffect.c_str() );
				
					/*item color, Juver, 2018/01/10 */
					EMPIECECHAR piece_color = SLOT_2_PIECE(EMSLOT(i));
					if ( piece_color != PIECE_SIZE )
					{
						m_pSkinChar->SetColor1( piece_color, ItemCustom.wColor1 );
						m_pSkinChar->SetColor2( piece_color, ItemCustom.wColor2 );
					}
					
					/*upgrade effect, Juver, 2017/09/01 */
					/*character render setting, Juver, 2019/03/23 */
					BOOL bResetUpgradeEffect = TRUE;
					if( /*!bSetSimple &&*/ ItemCustom.GETGRADE_EFFECT() >= 6 && ( i == SLOT_UPPER || i == SLOT_LOWER || i == SLOT_HAND || i == SLOT_FOOT ) )
					{
						EMCHARGENDER emGender = CharClassToGender( m_emClass );
						std::string strPartName[4] = { "body", "leg", "hand", "foot" };
						std::string strGender[2] = { "w", "m" };

						CString strUpgradeFileName;
						strUpgradeFileName.Format("%s_bs_%s_enchent+%d.cps", strGender[emGender].c_str(), strPartName[i-1].c_str(), ItemCustom.GETGRADE_EFFECT() );
						HRESULT hrSetUpgrade = m_pSkinChar->SetPiece ( strUpgradeFileName.GetString(), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
						if ( hrSetUpgrade == S_OK )	bResetUpgradeEffect = FALSE;
					}

					if ( bResetUpgradeEffect )
					{
						EMPIECECHAR piece_reset = SLOT_2_UPGRADEPIECE( EMSLOT(i) );
						if ( piece_reset != PIECE_SIZE )	
							m_pSkinChar->ResetPiece( piece_reset );
					}
				}
				else
				{
					/*vehicle system, Juver, 2017/08/06 */
					DxVehicleData*	pVehicleData = DxVehicleDataContainer::GetInstance().LoadData( pItem->GetWearingFileR(emIndex), m_pd3dDevice, TRUE  );
					if ( pVehicleData )
					{
						m_pSkinChar->SetVehicle ( pVehicleData, m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE, strSelfEffect.c_str() );

						/*character render setting, Juver, 2019/03/23 */
						if ( /*!RPROFILE::bVehicleSimple &&*/ m_pSkinChar->m_pVehicle )
						{
							/* vehicle no accessory, Juver, 2018/02/14 */
							if ( !pItem->sVehicle.bNoAcc )
							{	
								for( int j=0; j<VEHICLE_ACCETYPE_SIZE; j++ )
								{
									SITEM* pVehicleItem = GLItemMan::GetInstance().GetItem ( m_sVehicle.m_PutOnItems[j].sNativeID );
									if ( pVehicleItem )
										m_pSkinChar->m_pVehicle->SetPiece( pVehicleItem->GetVehicleWearingFile( pItem->sVehicle.emVehicleSubType ), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
								}
							}
							
							/*bike color , Juver, 2017/11/15 */
							if ( pItem->sVehicle.emVehicleType == VEHICLE_TYPE_BIKE )
							{
								for ( int ncolor=0; ncolor<BIKE_COLOR_SLOT_PART_SIZE; ++ncolor )
								{
									EMPIECECHAR piece_color = BIKE_COLOR_SLOT_TO_PIECE( (BIKE_COLOR_SLOT_PART) ncolor );
									BOOL bColor1 = IS_BIKECOLOR_1( (BIKE_COLOR_SLOT_PART) ncolor );
									if ( bColor1 )
										m_pSkinChar->m_pVehicle->SetColor1( piece_color, m_sVehicle.m_wColor[ncolor] );
									else
										m_pSkinChar->m_pVehicle->SetColor2( piece_color, m_sVehicle.m_wColor[ncolor] );
								}
							}

							/* car, cart color, Juver, 2018/02/14 */
							if ( pItem->sVehicle.emVehicleType == VEHICLE_TYPE_CAR || pItem->sVehicle.emVehicleType == VEHICLE_TYPE_CART )
							{
								m_pSkinChar->m_pVehicle->SetColor1( PIECE_SIZE, m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1] );
								m_pSkinChar->m_pVehicle->SetColor2( PIECE_SIZE, m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2] );
							}
						}	
					}
					else
						m_pSkinChar->SetPiece ( pItem->GetWearingFileR(emIndex), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE, strSelfEffect.c_str() );
				}
			}
		}
	}

	//	Note : 스킬 버프, 상태이상 효과 생성.
	FACTEFF::ReNewEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS, m_matTrans, m_vDir );

	if( GLCONST_CHAR::cCONSTCLASS[emIndex].strCLASS_EFFECT.size() != 0 )
	{
		DxEffcharDataMan::GetInstance().PutPassiveEffect ( m_pSkinChar, GLCONST_CHAR::cCONSTCLASS[emIndex].strCLASS_EFFECT.c_str(), &m_vDir );
	}

	if( m_dwTransfromSkill != SNATIVEID::ID_NULL )
	{
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[m_dwTransfromSkill].sNATIVEID.wMainID, m_sSKILLFACT[m_dwTransfromSkill].sNATIVEID.wSubID );
		if( pSkill ) 
		{
			for( BYTE i = 0; i < SKILL::EMTRANSFORM_NSIZE; i++ )
			{
				if( m_wSex == 1 )
				{				
					m_pSkinChar->SetPiece ( pSkill->m_sSPECIAL_SKILL.strTransform_Man[i].c_str(), m_pd3dDevice, 0X0, 0, TRUE );
				}else{
					m_pSkinChar->SetPiece ( pSkill->m_sSPECIAL_SKILL.strTransform_Woman[i].c_str(), m_pd3dDevice, 0X0, 0, TRUE );
				}
			}
		}
	}

	return S_OK;
}

WORD GLCharacter::GetBodyRadius ()
{
	return CHARACTER_BODY_RADIUS;
}

HRESULT GLCharacter::SetPosition ( D3DXVECTOR3 vPos )
{
	m_vPos = vPos;
	m_actorMove.SetPosition ( m_vPos, -1 );
	if ( m_actorMove.PathIsActive() )		m_actorMove.Stop();

	return S_OK;
}

EMCONFT_TYPE GLCharacter::GetConftType ( DWORD dwID )
{
	PGLCHARCLIENT pChar = GLGaeaClient::GetInstance().GetChar ( dwID );
	if ( !pChar )		return EMCONFT_NONE;

	if ( IsClubMaster() && pChar->IsClubMaster() )			return EMCONFT_GUILD;
	else if ( IsPartyMaster() && pChar->IsPartyMaster() )	return EMCONFT_PARTY;
	else													return EMCONFT_ONE;

	return EMCONFT_NONE;
}

// void GLCharacter::DoPASSIVITY ( BOOL bPASSIVITY )
// {
// 	if ( m_bPASSIVITY_ACTION_CHECK )	return;
// 
// 	m_bPASSIVITY_ACTION = bPASSIVITY;
// 	m_fPASSIVITY_ACTION_ELAPS = 0.0f;
// 
// 	if ( !bPASSIVITY )
// 	{
// 		ResetPASSIVITY ();
// 	}
// }

enum
{
	EMPASSIVITY_ACTION_TOTAL		= 3600,
	EMPASSIVITY_ACTION_ELAPS		= 60,
	EMPASSIVITY_ACTION_CHECK		= 30,
	EMPASSIVITY_ACTION_CHECK_RESET	= 360,
};

// bool GLCharacter::CheckPASSIVITY ( float fElaps )
// {
// 	if ( m_bPASSIVITY_ACTION_CHECK )
// 	{
// 		m_fPASSIVITY_ACTION_CHECK += fElaps;
// 		return true;
// 	}
// 
// 	m_fPASSIVITY_ACTION_ELAPS += fElaps;
// 	m_fPASSIVITY_ACTION_TOTAL += fElaps;
// 
// 	if ( m_fPASSIVITY_ACTION_ELAPS > EMPASSIVITY_ACTION_CHECK_RESET )
// 	{
// 		ResetPASSIVITY ();
// 	}
// 
// //	CDebugSet::ToView ( 4, 1, "action total : %5.1f", m_fPASSIVITY_ACTION_TOTAL );
// //	CDebugSet::ToView ( 4, 2, "action elaps : %5.1f", m_fPASSIVITY_ACTION_ELAPS );
// 
// 	if ( m_fPASSIVITY_ACTION_TOTAL > EMPASSIVITY_ACTION_TOTAL &&
// 		m_fPASSIVITY_ACTION_ELAPS < EMPASSIVITY_ACTION_ELAPS )
// 	{
// 		m_bPASSIVITY_ACTION_CHECK = TRUE;
// 		CDebugSet::ToListView ( "[___CheckPASSIVITY___]" );
// 		return true;
// 	}
// 
// 	return false;
// }

// bool GLCharacter::IsBlockPASSIVITY ()
// {
// 	return ( m_bPASSIVITY_ACTION_CHECK && m_fPASSIVITY_ACTION_CHECK>EMPASSIVITY_ACTION_CHECK );
// }

// void GLCharacter::ResetPASSIVITY ()
// {
// 	m_bPASSIVITY_ACTION = FALSE;
// 	m_fPASSIVITY_ACTION_ELAPS = 0.0f;
// 	m_fPASSIVITY_ACTION_TOTAL = 0.0f;
// 	m_bPASSIVITY_ACTION_CHECK = FALSE;
// 	m_fPASSIVITY_ACTION_CHECK = 0.0f;
// }

BOOL GLCharacter::ActionMoveTo ( float fTime, D3DXVECTOR3 vFromPt, D3DXVECTOR3 vTargetPt, BOOL bContinue, BOOL bREACT )
{
	// 상점이 개설된 상태에서 이동할 수 없다.
	if ( m_sPMarket.IsOpen() ) return FALSE;

	if ( StateStunGet() )	   return FALSE;

	BOOL bColl=FALSE;
	DWORD dwCollID;
	D3DXVECTOR3 vCollPos;
	if ( m_actorMove.GetParentMesh() )
	{
		m_actorMove.GetParentMesh()->IsCollision ( vFromPt, vTargetPt, vCollPos, dwCollID, bColl );
	}
	if ( !bColl )	return FALSE;
	
	//	이동하고자 하는 영역이 몹이 있는 영역과 겹칠 경우 이동 불가.
	bColl = GLGaeaClient::GetInstance().IsCollisionMobToPoint ( vCollPos, CHARACTER_BODY_RADIUS );
	if ( bColl )	return FALSE;

	BOOL bGoto = m_actorMove.GotoLocation
	(
		vCollPos+D3DXVECTOR3(0,+10,0),
		vCollPos+D3DXVECTOR3(0,-10,0)
	);

	if ( bGoto )
	{
		TurnAction ( GLAT_MOVE );
		m_sTargetID.vPos = vCollPos;

		//	Note : 이동 메시지 정보.
		GLMSG::SNETPC_GOTO NetMsg;
		NetMsg.dwActState = m_dwActState;
		NetMsg.vCurPos = m_vPos;
		NetMsg.vTarPos = m_sTargetID.vPos;

		m_sLastMsgMove = NetMsg;

		if ( ROPTION::bClickEffect )
		{
			STARGETID sTargetID( CROW_PC, m_dwGaeaID, vCollPos );
			D3DXMATRIX matTrans;
			D3DXMatrixTranslation ( &matTrans, vCollPos.x, vCollPos.y, vCollPos.z );
			DxEffExtra::GetInstance().NewClickEff ( GLCONST_CHAR::strCLICK_EFFECT.c_str(), matTrans, &sTargetID );
		}

		//	Note : 메세지 발생. ( 지속적으로 발생하는 이동 콘트롤중 일부만 메시지 발생. )
		//
		if ( bContinue && (m_fLastMsgMoveSend+m_fELAPS_MOVE) > fTime )		return TRUE;

		//DoPASSIVITY ( bREACT );

		if ( m_actorMove.PathIsActive() )
		{
			m_fLastMsgMoveSend = fTime;
			m_sLastMsgMoveSend = NetMsg;
			NETSENDTOFIELD ( &NetMsg );
		}
	}

	return TRUE;
}

BOOL GLCharacter::LargeMapMoveTo( D3DXVECTOR3* pPos )
{	/*right click teleport*/
	if ( m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 )
	{
		int nPosX(0), nPosY(0);
		PLANDMANCLIENT pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
		if ( pLandClient )
		{
			const GLMapAxisInfo &sMapAxisInfo = pLandClient->GetMapAxisInfo();
			sMapAxisInfo.Convert2MapPos ( pPos->x, pPos->z, nPosX, nPosY );

			if (DxGlobalStage::GetInstance().IsEmulator())
			{
				GLMSG::SNET_GM_MOVE2MAPPOS_FLD NetMsgFld;
				NetMsgFld.nidMAP = pLandClient->GetMapID();
				NetMsgFld.dwPOSX = nPosX;
				NetMsgFld.dwPOSY = nPosY;
				NetMsgFld.dwGaeaID = 0;
				NETSENDTOFIELD(&NetMsgFld);
			}
			else
			{
				GLMSG::SNET_GM_MOVE2MAPPOS NetMsg;
				NetMsg.nidMAP = pLandClient->GetMapID();
				NetMsg.dwPOSX = nPosX;
				NetMsg.dwPOSY = nPosY;
				NETSEND(&NetMsg);
			}
		}
		return FALSE;
	}

	D3DXVECTOR3 vFrom = *pPos;
	vFrom.y += 1000.0f;

	D3DXVECTOR3 vTarget = *pPos;
	vTarget.y -= 1000.0f;

	return ActionMoveTo( 0.0f, vFrom, vTarget, FALSE, TRUE );
}

BOOL GLCharacter::LargeMapTeleportTo( WORD wX, WORD wY )
{
	if ( m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_MASTER )
	{
		if ( DxGlobalStage::GetInstance().IsEmulator() )
		{
			GLMSG::SNET_GM_MOVE2MAPPOS_FLD NetMsgFld;
			NetMsgFld.nidMAP.wMainID = GLGaeaClient::GetInstance().GetActiveMapID().wMainID;
			NetMsgFld.nidMAP.wSubID = GLGaeaClient::GetInstance().GetActiveMapID().wSubID;
			NetMsgFld.dwPOSX = wX;
			NetMsgFld.dwPOSY = wY;
			NetMsgFld.dwGaeaID = 0;

			NETSEND( &NetMsgFld );
		}
		else
		{
			GLMSG::SNET_GM_MOVE2MAPPOS NetMsg;
			NetMsg.nidMAP.wMainID = GLGaeaClient::GetInstance().GetActiveMapID().wMainID;
			NetMsg.nidMAP.wSubID = GLGaeaClient::GetInstance().GetActiveMapID().wSubID;
			NetMsg.dwPOSX = wX;
			NetMsg.dwPOSY = wY;

			NETSEND( &NetMsg );
		}

		return TRUE;
	}

	return FALSE;
}

void GLCharacter::TurnAction ( EMACTIONTYPE toAction )
{
	//	Note : 앳 액션 리셋.
	//
	switch ( m_Action )
	{
	case GLAT_ATTACK:
		if ( toAction<=GLAT_TALK )
		{
			//	Note : 공격 종료 메세지 발생.
			GLMSG::SNETPC_ATTACK_CANCEL NetMsg;
			NETSENDTOFIELD ( &NetMsg );
		}
		break;

	case GLAT_SKILL:
		{
			if ( toAction == GLAT_MOVE || toAction == GLAT_ATTACK || toAction == GLAT_SHOCK )
			{
				GLMSG::SNETPC_SKILL_CANCEL NetMsg;
				NETSENDTOFIELD ( &NetMsg );
			}

			m_idACTIVESKILL = NATIVEID_NULL();
			SetDefenseSkill( false );

			/*multi skill Juver, 2018/09/04 */
			m_wTARGET_NUM_MAIN = 0;
			m_wTARGET_NUM_SUB = 0;

			/*dmk14 hidden weapon*/
			ResetHiddenWeaponPiece();
		}
		break;

	case GLAT_FALLING:
		if ( toAction==GLAT_FALLING )	return;
		break;
		
	case GLAT_DIE:
		if ( toAction==GLAT_FALLING || toAction==GLAT_DIE )	return;
		break;

	case GLAT_CONFT_END:
		ReSetSTATE(EM_ACT_CONFT_WIN);
		break;
	case GLAT_GATHERING:
		ReqCancelGathering();
		break;
	};

	//	Note : 액션 초기화.
	//

	m_Action = toAction;



	//	Note : 액션 바뀜.
	//
	switch ( m_Action )
	{
	case GLAT_IDLE:
		{
			m_fIdleTime = 0.0f;

			/* tyranny AFK disconnect, Juver, 2021/08/07 */
			m_fTyrannyAFKTimer = 0.0f;
			m_fCaptureTheFlagAFKTimer = 0.0f;

			/*dash skill logic, Juver, 2017/06/17 */
			m_dwActionAnim = AN_SUB_00_SIZE;
		}
		break;

	case GLAT_ATTACK:
		{
			StartAttackProc ();
		}
		break;

	case GLAT_SKILL:
		{
			m_pSkinChar->m_bResetSkillAni = TRUE;
			/*dmk14 hidden weapon*/
			SetHiddenWeaponPiece();
			StartSkillProc ();
		}
		break;

	case GLAT_TALK:
		{
			/* tyranny AFK disconnect, Juver, 2021/08/07 */
			m_fTyrannyAFKTimer = 0.0f;
			m_fCaptureTheFlagAFKTimer = 0.0f;
		}
		break;

	case GLAT_FALLING:
		{
			m_sHP.wNow = 0;
			/*dmk14 clear skill delay when died*/
			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap ();
			if ( pLAND && pLAND->isResetSkillOnDie() )		m_SKILLDELAY.clear();

			FACTEFF::DeleteEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS );

			/*skill transform, Juver, 2018/09/08 */
			m_skill_transform_data.Reset();

			/* skill position shift, Juver, 2020/12/15 */
			m_bSkillPositionShift = false;
		}break;

	case GLAT_DIE:
		{
			//	채력을 0로 초기화.
			//	( 클라이언트에서 회복량 보간으로 서버에서 채력이 0 일때 0가 아닐수 있음 )
			m_sHP.wNow = 0;
			/*dmk14 clear skill delay when died*/
			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap ();
			if ( pLAND && pLAND->isResetSkillOnDie() )	m_SKILLDELAY.clear();

			FACTEFF::DeleteEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS );

			/*skill transform, Juver, 2018/09/08 */
			m_skill_transform_data.Reset();

			/* skill position shift, Juver, 2020/12/15 */
			m_bSkillPositionShift = false;
		}break;

	case GLAT_CONFT_END:
		break;

	case GLAT_GATHERING:		
		break;

	default:
		GASSERT("GLCharacter::TurnAction() 준비되지 않은 ACTION 이 들어왔습니다.");
		break;
	};

	if ( m_actorMove.PathIsActive() )
	{
		/* push skill logic, Juver, 2020/12/14 */
		/* pull skill logic, Juver, 2020/12/14 */
		/*dash skill logic, Juver, 2017/06/17 */
		/* skill position shift, Juver, 2020/12/15 */
		if ( !IsACTION(GLAT_MOVE) && !IsACTION(GLAT_PUSHPULL) && !IsACTION(GLAT_SKILL_PUSH_PULL) && !IsACTION(GLAT_SKILL_DASH) && !m_bSkillPositionShift ) m_actorMove.Stop();
	}
}

BOOL GLCharacter::IsCtrlBlockBody ()
{
	if ( IsACTION(GLAT_SHOCK) )				return TRUE;
	if ( IsACTION(GLAT_PUSHPULL) )			return TRUE;
	if ( IsACTION(GLAT_SKILL_PUSH_PULL) )	return TRUE;	/* push skill logic, Juver, 2020/12/14 */	/* pull skill logic, Juver, 2020/12/14 */
	if ( IsACTION(GLAT_SKILL_DASH) )		return TRUE;	/*dash skill logic, Juver, 2017/06/17 */
	if ( IsACTION(GLAT_CONFT_END) )			return TRUE;
	if ( IsACTION(GLAT_GATHERING) && ( m_dwANISUBGESTURE == 0 ||  m_dwANISUBGESTURE == 1 ) ) 
		return TRUE;

	/* skill position shift, Juver, 2020/12/15 */
	if ( m_bSkillPositionShift )			return TRUE;

	return FALSE;
}

BOOL GLCharacter::IsValidBody ()
{
	if ( IsSTATE(EM_ACT_WAITING) )		return FALSE;
	if ( IsSTATE(EM_REQ_GATEOUT) )		return FALSE;
	if ( IsSTATE(EM_ACT_DIE) )			return FALSE;

	return ( m_Action < GLAT_FALLING );
}

BOOL GLCharacter::IsDie ()
{
	if ( !IsSTATE(EM_ACT_DIE) )
	{
		if ( IsACTION(GLAT_DIE) )		return TRUE;
	}

	return FALSE;
}

BOOL GLCharacter::IsPartyMaster ()
{
	GLPARTY_CLIENT* pParty = GLPartyClient::GetInstance().GetMaster();
	if ( pParty==NULL )		return FALSE;
	
	return ( pParty->m_dwGaeaID==m_dwGaeaID );
}

BOOL GLCharacter::IsPartyMem ()
{
	return GLPartyClient::GetInstance().GetMaster()!=NULL;
}

BOOL GLCharacter::IsClubMaster ()
{
	return m_sCLUB.m_dwMasterID==m_dwCharID;
}

BOOL GLCharacter::IsCDCertify ()
{
	return m_sCLUB.IsMemberFlgCDCertify(m_dwCharID);
}

void GLCharacter::P2PReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( StateStunGet() )				return;

	if ( dwML & (DXKEY_UP|DXKEY_DUP) )
	{
		//	Note : 리엑션 취소.
		SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
		m_sREACTION.RESET();

		PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap();
		
		PGLCHARCLIENT pCharClient = pLandMan->GetChar ( sTargetID.dwID );
		if ( !pCharClient )		return;


		const D3DXVECTOR3 &vTarPos = pCharClient->GetPosition();

		float fMoveDist = FLT_MAX;
		float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
		if ( IsACTION(GLAT_MOVE) )	fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) );

		WORD wP2PRange = pCharClient->GetBodyRadius() + CHARACTER_BODY_RADIUS + 25;
		WORD wP2PAbleDis = wP2PRange + 2;

		if ( fDist>wP2PAbleDis )
		{
			if ( fMoveDist > wP2PAbleDis )
			{
				//	Note : 이동지점 지정.
				//
				D3DXVECTOR3 vDir = vTarPos - m_vPos;
				D3DXVec3Normalize ( &vDir, &vDir );

				bMove = TRUE;
				vMoveTo = m_vPos + vDir*wP2PRange;
				vMoveTo.y = vTarPos.y;

				//	Note : 리엑션 등록.
				//
				m_sREACTION.emREACTION = REACT_P2P;
				m_sREACTION.sTARID = sTargetID;
				m_sREACTION.vMARK_POS = vMoveTo;
			}
			else
			{
				m_sREACTION = sREC_TEMP;
			}

			return;
		}

		//	Note : PtoP 메뉴.
		//
		//bool bVALID_CLUB = m_sCLUB.m_dwID!=CLUB_NULL;
		//bool bMASTER = m_sCLUB.m_dwMasterID==m_dwCharID;

		if( !GLCONST_CHAR::bBATTLEROYAL )
		{
			CInnerInterface::GetInstance().SetPtoPWindowOpen( pCharClient, sTargetID.dwID, m_sCLUB );
		}
	}
}

void GLCharacter::PvPReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( StateStunGet() )	return;	

	// 공격애니메이션이 종료유무
	if ( !m_bAttackable ) return;

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLand && pLand->IsPeaceZone() )	return;

	//	Note : 리엑션 취소.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

	float fMoveDist = FLT_MAX;
	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
	if ( IsACTION(GLAT_MOVE) )	fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) );

	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pTarget )		return;

	bool bToReturn = false;
	

	//	Note : 장거리 무기일 경우만 공격가능거리에 사정거리 옵션 값을 더해줌.
	//
	WORD wAttackRange = pTarget->GetBodyRadius() + CHARACTER_BODY_RADIUS + GETATTACKRANGE() + 2;
	if ( ISLONGRANGE_ARMS() )	 wAttackRange += (WORD) GETSUM_TARRANGE();

	WORD wAttackAbleDis = wAttackRange + 2;

	if ( fDist>wAttackAbleDis )
	{
		if ( fMoveDist > wAttackAbleDis )
		{
			//	Note : 이동지점 지정.
			//
			D3DXVECTOR3 vPos = vTarPos - m_vPos;
			D3DXVec3Normalize ( &vPos, &vPos );

			bMove = TRUE;
			vMoveTo = vTarPos - vPos*wAttackRange;
			vMoveTo.y = vTarPos.y;

			//	Note : 리엑션 등록.
			//
			m_sREACTION.emREACTION = REACT_PVP;
			m_sREACTION.bCONTINUE = bcontinue;
			m_sREACTION.sTARID = sTargetID;
			m_sREACTION.vMARK_POS = vMoveTo;
		}
		else
		{
			m_sREACTION = sREC_TEMP;
		}

		bToReturn = true;
	}

	
	if (bToReturn)	return;

	//	지형과 충돌하는지 검사.
	//	
	D3DXVECTOR3 vPos1 = GetPosition(); vPos1.y += 15.0f;
	D3DXVECTOR3 vPos2 = vTarPos; vPos2.y += 15.0f;
	
	BOOL bCollision;
	D3DXVECTOR3 vCollision;
	LPDXFRAME pDxFrame;

	DxLandMan *pLandMan = GLGaeaClient::GetInstance().GetActiveMap()->GetLandMan();
	pLandMan->IsCollision ( vPos1, vPos2, vCollision, bCollision, pDxFrame, FALSE );
	if ( bCollision )	return;

	PANIMCONTNODE pAnicont = m_pSkinChar->GETANI ( AN_ATTACK, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
	if ( !pAnicont )	return;

	WORD wStrikeNum = pAnicont->pAnimCont->m_wStrikeCount;

	//	Note : 공격수행 가능 검사.
	//    
	EMBEGINATTACK_FB emBeginFB = BEGIN_ATTACK(wStrikeNum);
	if ( emBeginFB==EMBEGINA_OK || emBeginFB==EMBEGINA_SP )
	{
		m_sTargetID.emCrow = sTargetID.emCrow;
		m_sTargetID.dwID = sTargetID.dwID;
		TurnAction ( GLAT_ATTACK );
	}

	if ( bcontinue )
	{
		m_sREACTION.emREACTION = REACT_PVP;
		m_sREACTION.bCONTINUE = true;
		m_sREACTION.sTARID = sTargetID;
		m_sREACTION.vMARK_POS = m_vPos;
	}

	return;
}

void GLCharacter::MobReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( StateStunGet() )	return;

	// 공격애니메이션이 종료유무
	if ( !m_bAttackable ) return;

	m_sOLD_TARMOB = sTargetID;

	//	Note : 리엑션 취소.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

	float fMoveDist = FLT_MAX;
	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
	if ( IsACTION(GLAT_MOVE) )	fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) );

	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pTarget )		return;

	bool bToReturn = false;
	

	//	Note : 장거리 무기일 경우만 공격가능거리에 사정거리 옵션 값을 더해줌.
	//
	WORD wAttackRange = pTarget->GetBodyRadius() + CHARACTER_BODY_RADIUS + GETATTACKRANGE() + 2;
	if ( ISLONGRANGE_ARMS() )	 wAttackRange += (WORD) GETSUM_TARRANGE();

	WORD wAttackAbleDis = wAttackRange + 2;

	if ( fDist>wAttackAbleDis )
	{
		if ( fMoveDist > wAttackAbleDis )
		{
			//	Note : 이동지점 지정.
			//
			D3DXVECTOR3 vPos = vTarPos - m_vPos;
			D3DXVec3Normalize ( &vPos, &vPos );

			bMove = TRUE;
			vMoveTo = vTarPos - vPos*wAttackRange;
			vMoveTo.y = vTarPos.y;

			//	Note : 리엑션 등록.
			//
			m_sREACTION.emREACTION = REACT_MOB;
			m_sREACTION.bCONTINUE = bcontinue;
			m_sREACTION.sTARID = sTargetID;
			m_sREACTION.vMARK_POS = vMoveTo;
		}
		else
		{
			m_sREACTION = sREC_TEMP;
		}

		bToReturn = true;
	}

	
	if (bToReturn)	return;

	//	지형과 충돌하는지 검사.
	//	
	D3DXVECTOR3 vPos1 = GetPosition(); vPos1.y += 15.0f;
	D3DXVECTOR3 vPos2 = vTarPos; vPos2.y += 15.0f;
	
	BOOL bCollision;
	D3DXVECTOR3 vCollision;
	LPDXFRAME pDxFrame;

	DxLandMan *pLandMan = GLGaeaClient::GetInstance().GetActiveMap()->GetLandMan();
	pLandMan->IsCollision ( vPos1, vPos2, vCollision, bCollision, pDxFrame, FALSE );
	if ( bCollision )	return;

	PANIMCONTNODE pAnicont = m_pSkinChar->GETANI ( AN_ATTACK, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
	if ( !pAnicont )	return;

	WORD wStrikeNum = pAnicont->pAnimCont->m_wStrikeCount;

	//	Note : 공격수행 가능 검사.
	//
	EMBEGINATTACK_FB emBeginFB = BEGIN_ATTACK(wStrikeNum);
	if ( emBeginFB==EMBEGINA_OK || emBeginFB==EMBEGINA_SP )
	{
		m_sTargetID.emCrow = sTargetID.emCrow;
		m_sTargetID.dwID = sTargetID.dwID;
		TurnAction ( GLAT_ATTACK );
	}

	if ( bcontinue )
	{
		m_sREACTION.bCONTINUE = true;
		m_sREACTION.emREACTION = REACT_MOB;
		m_sREACTION.sTARID = sTargetID;
		m_sREACTION.vMARK_POS = m_vPos;
	}

	return;
}

// *****************************************************
// Desc: NPC에 관련된 예약된 액션을 처리한다.
// *****************************************************
void GLCharacter::NpcReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( StateStunGet() )	return;

	//	Note : 리엑션 취소.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	// 선택된 NPC 초기화
	m_dwNPCID = 0;

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;
	// 필요없는 코드
	D3DXVECTOR3 vDistance = m_vPos - vTarPos;
	float fDistance = D3DXVec3Length ( &vDistance );

	// NPC와의 거리
	float fMoveDist = FLT_MAX;
	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
	if ( IsACTION(GLAT_MOVE) )	fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) );

	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pTarget )	return;

	// 대화가 가능한 범위
	WORD wTakeRange = pTarget->GetBodyRadius() + CHARACTER_BODY_RADIUS + 30;
	WORD wTakeAbleDis = wTakeRange + 2;

	// 지금 위치가 대화가 불가능 하면
	if ( fDist>wTakeAbleDis )
	{
		// 이동한 목적지가 대화가 불가능 하면 가능한 위치로 이동하는 예약된 액션을 등록
		if ( fMoveDist > wTakeAbleDis )
		{
			//	Note : 이동지점 지정.
			//
			D3DXVECTOR3 vPos = vTarPos - m_vPos;
			D3DXVec3Normalize ( &vPos, &vPos );

			bMove = TRUE; 
			vMoveTo = vTarPos - vPos*wTakeRange;
			vMoveTo.y = vTarPos.y;

			//	Note : 리엑션 등록.
			//
			m_sREACTION.emREACTION = REACT_NPC;
			m_sREACTION.sTARID = sTargetID;
			m_sREACTION.vMARK_POS = vMoveTo;
		}
		// 이동한 목적지가 대화가 가능하면 그냥 예전 예약액션을 그래도 사용
		else
		{
			m_sREACTION = sREC_TEMP;
		}

		return;
	}

	// 만약 예약된 NPC가 있을경우 NPC의 위치 확인
	if ( dwML&(DXKEY_UP|DXKEY_DUP) )
	{
		PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );
		if ( !pCrow )		return;
		PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pCrow->m_sNativeID );
		if ( !pCrowData )	return;

		m_dwNPCID   = sTargetID.dwID;

		/*pvp capture the flag, Juver, 2018/02/03 */
		if ( pCrowData->m_sBasic.m_emCTFType != EMNPC_CTF_FLAG_TYPE_NONE )
			GLPVPCaptureTheFlagClient::GetInstance().FlagActionCapture( m_dwCharID, pCrowData->m_sBasic.m_emCTFType, pCrow->m_dwGlobID );
		else
			CInnerInterface::GetInstance().SetDialogueWindowOpen ( sTargetID, this );
	}
}

// *****************************************************
// Desc: 채집NPC에 관련된 예약된 액션을 처리한다.
// *****************************************************
void GLCharacter::GatheringReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( StateStunGet() )	return;

	//	Note : 리엑션 취소.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	// 선택된 NPC 초기화
	m_dwNPCID = 0;

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;
	// 필요없는 코드
	D3DXVECTOR3 vDistance = m_vPos - vTarPos;
	float fDistance = D3DXVec3Length ( &vDistance );

	// NPC와의 거리
	float fMoveDist = FLT_MAX;
	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
	if ( IsACTION(GLAT_MOVE) )	fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) );

	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pTarget )	return;

	// 대화가 가능한 범위
	WORD wCollectionRange = pTarget->GetBodyRadius() + CHARACTER_BODY_RADIUS + 2;
	WORD wCollectionAbleDis = wCollectionRange + 10;

	// 지금 위치가 대화가 불가능 하면
	if ( fDist>wCollectionAbleDis )
	{
		// 이동한 목적지가 대화가 불가능 하면 가능한 위치로 이동하는 예약된 액션을 등록
		if ( fMoveDist > wCollectionAbleDis )
		{
			//	Note : 이동지점 지정.
			//
			D3DXVECTOR3 vPos = vTarPos - m_vPos;
			D3DXVec3Normalize ( &vPos, &vPos );

			bMove = TRUE; 
			vMoveTo = vTarPos - vPos*wCollectionRange;
			vMoveTo.y = vTarPos.y;

			//	Note : 리엑션 등록.
			//
			m_sREACTION.emREACTION = REACT_GATHERING;
			m_sREACTION.sTARID = sTargetID;
			m_sREACTION.vMARK_POS = vMoveTo;
		}
		// 이동한 목적지가 대화가 가능하면 그냥 예전 예약액션을 그래도 사용
		else
		{
			m_sREACTION = sREC_TEMP;
		}

		return;
	}

	// 만약 예약된 NPC가 있을경우 NPC의 위치 확인
	if ( dwML&(DXKEY_UP|DXKEY_DUP) )
	{
		PGLMATERIALCLIENT pMaterial = GLGaeaClient::GetInstance().GetActiveMap()->GetMaterial ( sTargetID.dwID );
		if ( !pMaterial )		return;
		PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pMaterial->m_sNativeID );
		if ( !pCrowData )	return;
		if ( pCrowData->m_sBasic.m_emCrow != CROW_MATERIAL ) return;

		// 채집 시작
		ReqGathering( sTargetID );
	}
}

void GLCharacter::ItemReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( StateStunGet() )	return;

	//	Note : 리엑션 취소.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

	D3DXVECTOR3 vDistance = m_vPos - vTarPos;
	float fDistance = D3DXVec3Length ( &vDistance );

	WORD wTarBodyRadius = 4;
	WORD wTakeRange = wTarBodyRadius + CHARACTER_BODY_RADIUS + 2;
	WORD wTakeAbleDis = wTakeRange + 12;

	if ( fDistance>wTakeAbleDis )
	{
		//	Note : 이동지점 지정.
		//
		D3DXVECTOR3 vPos = vTarPos - m_vPos;
		D3DXVec3Normalize ( &vPos, &vPos );

		bMove = TRUE; 
		vMoveTo = vTarPos - vPos*wTakeRange;

		//	Note : 리엑션 등록.
		//
		m_sREACTION.emREACTION = REACT_ITEM;
		m_sREACTION.sTARID = sTargetID;
		m_sREACTION.vMARK_POS = vMoveTo;

		return;
	}
	
	if ( dwML&(DXKEY_UP|DXKEY_DUP) )
	{
		//	줍기 시도.
		ReqFieldTo ( sTargetID );
	}
}

void GLCharacter::ReservedAction ( float fTime )
{
    if ( !m_sREACTION.ISVALID() )									return;
	if ( m_actorMove.PathIsActive() || !IsACTION(GLAT_IDLE) )		return;
	if ( !GLGaeaClient::GetInstance().GetActiveMap() )				return;
	if ( m_bVehicle )												return;
	
	if ( StateStunGet() )
	{
		m_sREACTION.RESET();
		return;
	}

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();

	//	Note : 목표 위치가 정확한지 검사.
	//
	D3DXVECTOR3 vCURPOS = m_actorMove.Position();
	D3DXVECTOR3 vDIR =  m_sREACTION.vMARK_POS - vCURPOS;
	float fLength = D3DXVec3Length ( &vDIR );
	if ( fLength > 10.0f )
	{
		m_sREACTION.RESET();
		return;
	}

	//	Note : 피행위자가 정확한지 검사.
	//
	BOOL bTARID(TRUE);
	STARGETID sTARID = m_sREACTION.sTARID;
	switch ( m_sREACTION.emREACTION )
	{
	case REACT_MOB:
	case REACT_NPC:
	case REACT_P2P:
	case REACT_PVP:
	case REACT_GATHERING:
		{
			GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTARID );
 			if ( !pTarget || pTarget->IsACTION(GLAT_FALLING) || pTarget->IsACTION(GLAT_DIE) )	bTARID = FALSE;
		}
		break;

	case REACT_ITEM:
		switch ( sTARID.emCrow )
		{
		case CROW_ITEM:
			if ( !pLand->GetItem(sTARID.dwID) )										bTARID = FALSE;
			break;
		case CROW_MONEY:
			if ( !pLand->GetMoney(sTARID.dwID) )									bTARID = FALSE;
			break;
		//case CROW_PET:	break;	// PetData
		default:
			bTARID = FALSE;
			break;
		};
		break;
	};

	if ( !bTARID )
	{
		m_sREACTION.RESET();
		return;
	}

	//	피행위자의 현제 위치.
	m_sREACTION.sTARID.vPos = GLGaeaClient::GetInstance().GetTargetPos ( sTARID );

	BOOL bMove(FALSE);
	D3DXVECTOR3 vMoveTo(0,0,0);
	bool bcontinue = m_sREACTION.bCONTINUE;
	switch ( m_sREACTION.emREACTION )
	{
	case REACT_P2P:			P2PReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);		break;
	case REACT_PVP:			PvPReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);		break;

	case REACT_MOB:			MobReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);		break;
	case REACT_NPC:			NpcReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);		break;
	case REACT_ITEM:		ItemReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);		break;
	case REACT_SKILL:		SkillReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);		break;
	case REACT_GATHERING:	GatheringReaction(m_sREACTION.sTARID,DXKEY_UP,bcontinue,bMove,vMoveTo);	break;
	};

	//	Note : Reaction 에서 이동을 요청한 경우.
	//
	if ( bMove )
	{
		ActionMoveTo ( fTime, vMoveTo+D3DXVECTOR3(0,+5,0), vMoveTo+D3DXVECTOR3(0,-5,0), FALSE, TRUE );
	}
}

STARGETID* GLCharacter::GetCONFT_TAR ( DETECTMAP_RANGE &pair )
{
	if ( !m_sCONFTING.IsFIGHTING() )	return NULL;

	switch ( m_sCONFTING.emTYPE )
	{
	case EMCONFT_ONE:
		{
			//	Note : 대련자가 걸렸을 경우 이를 우선시.
			STARGETID sCONFT_TARID(CROW_PC,m_sCONFTING.dwTAR_ID);
			STARGETID *pTARID = FindCrow ( pair, sCONFT_TARID );
			if ( pTARID )	return pTARID;
		}
		break;

	case EMCONFT_PARTY:
		{
			STARGETID *pTARID = FindCrow ( pair, m_sCONFTING.setConftMember );
			if ( pTARID )	return pTARID;
		}
		break;

	case EMCONFT_GUILD:
		{
			STARGETID *pTARID = FindCrowCID ( pair, m_sCONFTING.setConftMember );
			if ( pTARID )	return pTARID;
		}
		break;
	};

	return NULL;
}

bool GLCharacter::IsPK_TAR ( PGLCHARCLIENT pCHAR, bool bFORCED, bool bCheckSafeTime )
{
	//	Note : PK 의 경우.
	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap ();
	if ( !pLAND )				return false;
	if ( !pLAND->IsPKZone() )	return false;

	/*dmk14 pk safetime*/
	//if ( IsSafeTime() && bCheckSafeTime )			return false;
	//if ( pCHAR->IsSafeTime() && bCheckSafeTime )	return false;

	/* SafeTime Logic - Montage, 3-28-25 */
	if ( m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 ) return false;

	if ( pCHAR->IsSafeZone() )	
	{
		return false;
	}
	if ( pCHAR->IsSafeTime() && !bFORCED )
	{
		return false;
	}
	if ( IsSafeTime () && !bFORCED )
	{
		return false;
	}
	if ( IsSafeZone () )		
	{
		return false;
	}

	SDROP_CHAR &sCHARDATA_TAR = pCHAR->GetCharData();

	//	파티일 경우는 어떤 경우든 적대행위가 되지 않음.
	GLPARTY_CLIENT* pPARTY_CLT = GLPartyClient::GetInstance().FindMember(sCHARDATA_TAR.dwGaeaID);
	if ( pPARTY_CLT )			return false;

	//	적대행위자.
	bool bHOSTILE = IS_PLAYHOSTILE ( sCHARDATA_TAR.dwCharID );
	if ( bHOSTILE )																		return true;

	//	선도클럽전 타겟.
	if ( pLAND->m_bClubBattle || ( pLAND->m_bPVPWoeMap && RPARAM::bWoeGVG == 1 ) )
	{
		if ( m_sCLUB.IsAllianceGuild(sCHARDATA_TAR.dwGuild) ) return false;
		else												  return true;
	}

	if ( pLAND->m_bClubDeathMatch && GLGaeaClient::GetInstance().IsClubDeathMatch() )
	{
		if ( m_sCLUB.m_dwID != sCHARDATA_TAR.dwGuild ) 
			return true;
	}

	/*pvp tyranny, Juver, 2017/08/24 */
	if ( pLAND->m_bPVPTyrannyMap && GLPVPTyrannyClient::GetInstance().IsBattle() )
	{
		if ( RPARAM::bCTFGVG )
		{
			if ( m_sCLUB.m_dwID!=sCHARDATA_TAR.dwGuild )	
				return true;
			else 
				return false;
		}
		else
		{
			if ( m_wSchool!=sCHARDATA_TAR.wSchool )	
				return true;
			else 
				return false;
		}
	}

	/*school wars, Juver, 2018/01/19 */
	if ( pLAND->m_bPVPSchoolWarsMap && GLPVPSchoolWarsClient::GetInstance().IsBattle() )
	{
		if ( m_wSchool!=sCHARDATA_TAR.wSchool )	return true;
		else return false;
	}

	/*pvp capture the flag, Juver, 2018/01/31 */
	if ( pLAND->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagClient::GetInstance().IsBattle() )
	{
		if ( m_wCaptureTheFlagTeam!=sCHARDATA_TAR.m_wCaptureTheFlagTeam )	return true;
		else return false;
	}

	/* pvp club death match, Juver, 2020/11/26 */
	if ( ( pLAND->m_bPVPClubDeathMatchLobbyMap || pLAND->m_bPVPClubDeathMatchBattleMap )
		&& PVPClubDeathMatchClient::GetInstance().IsBattle() )
	{
		if ( m_sCLUB.m_dwID != sCHARDATA_TAR.dwGuild ) 
			return true;
		else 
			return false;
	}

	if ( ( pLAND->m_bPVPPBGLobbyMap || pLAND->m_bPVPPBGBattleMap )
		&& PVPPBG::ManagerClient::GetInstance().IsBattle() )
	{
		if ( GLPartyClient::GetInstance().GetPartyID() != sCHARDATA_TAR.dwParty ) 
			return true;
		else 
			return false;
	}
	
	/*woe Arc Development 08-06-2024*/
	if ( RPARAM::bWoeGVG == 0 && pLAND->m_bPVPWoeMap && GLPVPWoeClient::GetInstance().IsBattle())
	{
		if (m_wSchool != sCHARDATA_TAR.wSchool)	return true;
		else return false;
	}

	//	강제 PK.
	if ( bFORCED )																	
	{
		if ( sCHARDATA_TAR.wLevel <= GLCONST_CHAR::nPK_LIMIT_LEVEL )
			return false;
		else
			return true;
	}

	//	학교간 프리 PK.
	if ( !GLCONST_CHAR::bSCHOOL_FREE_PK_Z_FORCED ) 
	{
		bool bSCHOOL_FREEPK = GLGaeaClient::GetInstance().IsSchoolFreePk ();
		if ( bSCHOOL_FREEPK && m_wSchool!=sCHARDATA_TAR.wSchool )					return true;
	}

	// 성향 이벤트일경우.
	if ( GLGaeaClient::GetInstance().IsBRIGHTEVENT() )								return true;

	/* open pk function, Juver, 2019/12/24 */
	if ( pLAND && pLAND->IsOpenPK()) return true;
	
	// 클럽 배틀중일경우
	if ( ( m_sCLUB.IsBattle(pCHAR->GETCLUBID()) || m_sCLUB.IsBattleAlliance(pCHAR->GETALLIANCEID()) ) 
		&& pLAND->IsClubBattleZone() )		
		return true;

	return false;
}

bool GLCharacter::IsClubBattle_TAR ( GLCharClient* pCHAR )
{
	//	Note : PK 의 경우.
	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap ();
	if ( !pLAND )				return false;

	// 클럽 배틀중일경우
	if ( ( m_sCLUB.IsBattle(pCHAR->GETCLUBID()) || m_sCLUB.IsBattleAlliance( pCHAR->GETALLIANCEID()) )
		&& pLAND->IsClubBattleZone() )
		return true;

	return false;
}

int GLCharacter::GetClubColorIndex( GLCharClient* pCHAR )
{
	int nIndex = 0;
	
	if ( !pCHAR ) return nIndex;

	CLUB_BATTLE_ITER pos = m_sCLUB.m_mapBattle.begin();
	CLUB_BATTLE_ITER pos_end = m_sCLUB.m_mapBattle.end();

	for ( ; pos != pos_end; ++pos )
	{
		if ( pCHAR->GETCLUBID() == (*pos).first ) break;
		if ( pCHAR->GETALLIANCEID() == (*pos).first ) break;
		nIndex++;
	}

	nIndex = nIndex % NS_UITEXTCOLOR::MAX_CLUB_BATTLE_COLOR;

	return nIndex;
}

SACTIONTAR GLCharacter::FindActionTarget ( const D3DXVECTOR3 &vTargetPt, const D3DXVECTOR3 &vFromPt, const EMFIND_TAR emFINDTAR )
{
	SACTIONTAR sACTAR;
	if ( IsSTATE(EM_ACT_CONTINUEMOVE) )		return sACTAR;

	//	Note : 사용자 입력값.
	//
	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();
	
	DWORD dwKeyR = dxInputDev.GetKeyState ( RPROFILE::MenuShortcut[SHORTCUT_RUN] );
	DWORD dwKeyLAlt = dxInputDev.GetKeyState ( DIK_LMENU );
	DWORD dwKeyLCtrl = dxInputDev.GetKeyState ( DIK_LCONTROL );
	DWORD dwKeyLShift = dxInputDev.GetKeyState ( DIK_LSHIFT );
	DWORD dwKeySPACE = dxInputDev.GetKeyState ( DIK_SPACE );

	DWORD dwKeyZ = dxInputDev.GetKeyState (RPROFILE::MenuShortcut[SHORTCUT_PKMODE] );

	bool bONLY_ITEM = (dwKeyLAlt&DXKEY_DOWNED)!=NULL;
	bool bODER_P2P = (dwKeyLShift&DXKEY_DOWNED)!=NULL;

	bool bANY(false), bENEMY(false), bOUR(false), bDIE(false);
	switch ( emFINDTAR )
	{
	case ENFIND_TAR_NULL:
		bANY = true;
		bENEMY = true;
		bOUR = true;
		break;
	case EMFIND_TAR_ENEMY:
		bENEMY = true;
		break;
	case EMFIND_TAR_ANY:
		bANY = true;
		break;
	case EMFIND_TAR_OUR:
		bOUR = true;
		break;

	case EMFIND_TAR_ANY_DIE:
		bANY = true;
		bDIE = true;
		break;
	case EMFIND_TAR_OUR_DIE:
		bOUR = true;
		bDIE = true;
		break;
	};

	STARGETID sTID_FOUND;
	DETECTMAP_RANGE pairRange;

	//	Crow 검색 ( 단, 연속 이동일 때는 검색하지 않는다. )
	DETECTMAP *pDetectMap = NULL;

	if ( bDIE )
	{
		pDetectMap = GLGaeaClient::GetInstance().DetectCrowDie ( vFromPt, vTargetPt );
		if ( pDetectMap )
		{
			//	Note : ALT 키를 누른 상태에서는 검출 제외 ==> 아이템, 돈에 우선권을 줌.
			//
			if ( !bONLY_ITEM )
			{
				if ( bENEMY )
				{
					//	MOB.
					pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
					if ( pairRange.first != pairRange.second )
					{
						sACTAR.emACTAR = EMACTAR_MOB;
						sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
						return sACTAR;
					}
				}
				else
				{
					//	PC ANY.
					pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
					if ( pairRange.first != pairRange.second )
					{
						sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );

						sACTAR.emACTAR = EMACTAR_PC_ANY;
						sACTAR.sTARID = sTID_FOUND;
						return sACTAR;
					}
				}
			}
		}

		return sACTAR;
	}

	DWORD emCrow = ( CROW_EX_PC | CROW_EX_NPC | CROW_EX_MOB | CROW_EX_ITEM | 
					 CROW_EX_MONEY | CROW_EX_SUMMON | CROW_EX_MATERIAL );
	pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, vFromPt, vTargetPt );

	if ( !pDetectMap )
	{
		//	Note : 스킬의 '모두에게' 혹은 '우리편에게' 속성일 경우 선택된 케릭이 없을때,
		//			자신이 선택되는지 점검.
		if ( emFINDTAR==EMFIND_TAR_ANY || emFINDTAR==EMFIND_TAR_ENEMY )
		{
			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, m_vMax, m_vMin );
			if ( bCol )
			{
				//	우리편이면 OUR 아니면 ANY
				EMACTIONTAR		emACTAR = EMACTAR_PC_ANY;
				if ( bOUR )		emACTAR = EMACTAR_PC_OUR;

				sACTAR.emACTAR = emACTAR;
				sACTAR.sTARID = STARGETID(CROW_PC,m_dwGaeaID,m_vPos);
				return sACTAR;
			}
		}

		return sACTAR;
	}

	//	Note : ALT 키를 누른 상태에서는 검출 제외 ==> 아이템, 돈에 우선권을 줌.
	//
	if ( !bONLY_ITEM )
	{
		//	PVP.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			//	Note : P2P 용도.
			//
			sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			if ( bODER_P2P )
			{
				sACTAR.emACTAR = EMACTAR_PC_P2P;
				sACTAR.sTARID = sTID_FOUND;
				return sACTAR;
			}
			else if ( bENEMY )
			{
				STARGETID *pPVP(NULL);

				//	Note : 대련자가 있는지 검사.	( 대련자 우선 )
				//
				pPVP = GetCONFT_TAR(pairRange);
				if ( pPVP )
				{
					sACTAR.emACTAR = EMACTAR_PC_PVP;
					sACTAR.sTARID = *pPVP;
					return sACTAR;
				}

				STARGETID &sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
				PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTID_FOUND.dwID );
				if ( pCHAR )
				{
					// 강제 공격키를 누르지 않아도 되는 경우( 성향 이벤트 중일경우 )
					bool bFORCED_PK = (dwKeyZ&DXKEY_DOWNED)!=NULL;
					bFORCED_PK = bFORCED_PK && !CInnerInterface::GetInstance().IsCHAT_BEGIN();
					bool bBRIGHTEVENT = GLGaeaClient::GetInstance().IsBRIGHTEVENT();

					/* open pk function, Juver, 2019/12/24 */
					bool bOpenPK = false;
					PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap();
					if ( pLandManClient && pLandManClient->IsOpenPK() )
					{
						bOpenPK = true;
					}

					bFORCED_PK = bFORCED_PK || bBRIGHTEVENT || bOpenPK;
					if ( IsPK_TAR(pCHAR,bFORCED_PK,true) )
					{
						sACTAR.emACTAR = EMACTAR_PC_PVP;
						sACTAR.sTARID = sTID_FOUND;
						return sACTAR;
					}
				}
				
			}
		}

		

		if ( bENEMY )
		{
			//	MOB.
			pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER() );
			if ( pairRange.first != pairRange.second )
			{
				STARGETID *pTARID = FindCrow ( pairRange, m_sOLD_TARMOB );
				if ( pTARID )
				{
					sACTAR.emACTAR = EMACTAR_MOB;
					sACTAR.sTARID = *pTARID;
					return sACTAR;
				}

				sACTAR.emACTAR = EMACTAR_MOB;
				sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
				return sACTAR;
			}
		}

		/*skill summon, Juver, 2017/10/08 */
		if ( bENEMY )
		{
			pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
			if ( pairRange.first != pairRange.second )
			{
				STARGETID *pPVP(NULL);
				pPVP = GetCONFT_TAR(pairRange);
				if ( pPVP )
				{
					sACTAR.emACTAR = EMACTAR_SUMMON_ATTACK;
					sACTAR.sTARID = *pPVP;
					return sACTAR;
				}

				sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );

				PGLANYSUMMON pSummon = GLGaeaClient::GetInstance().GetSummon ( sTID_FOUND.dwID );
				if ( pSummon && pSummon->IsTargetReady() )
				{
					PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( pSummon->m_dwOwner );
					if ( pCHAR )
					{
						// 강제 공격키를 누르지 않아도 되는 경우( 성향 이벤트 중일경우 )
						bool bFORCED_PK = (dwKeyZ&DXKEY_DOWNED)!=NULL;
						bFORCED_PK = bFORCED_PK && !CInnerInterface::GetInstance().IsCHAT_BEGIN();
						bool bBRIGHTEVENT = GLGaeaClient::GetInstance().IsBRIGHTEVENT();

						/* open pk function, Juver, 2019/12/24 */
						bool bOpenPK = false;
						PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap();
						if ( pLandManClient && pLandManClient->IsOpenPK() )
						{
							bOpenPK = true;
						}

						bFORCED_PK = bFORCED_PK || bBRIGHTEVENT || bOpenPK;
						if ( IsPK_TAR(pCHAR,bFORCED_PK,true) )
						{
							sACTAR.emACTAR = EMACTAR_SUMMON_ATTACK;
							sACTAR.sTARID = sTID_FOUND;
							return sACTAR;
						}
					}
				}
			}
		}

		/*skill summon, Juver, 2017/10/08 */
		if ( bOUR || bANY )
		{
			pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
			if ( pairRange.first != pairRange.second )
			{
				sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );

				PGLANYSUMMON pSummon = GLGaeaClient::GetInstance().GetSummon ( sTID_FOUND.dwID );
				if ( pSummon && pSummon->IsTargetReady() )
				{
					GLPARTY_CLIENT* pPARTY_CLT = GLPartyClient::GetInstance().FindMember(pSummon->m_dwOwner);

					if ( pPARTY_CLT && bOUR )
					{
						sACTAR.emACTAR = EMACTAR_PC_OUR;
						sACTAR.sTARID = sTID_FOUND;
						return sACTAR;
					}
					else
					{
						sACTAR.emACTAR = EMACTAR_PC_ANY;
						sACTAR.sTARID = sTID_FOUND;
						return sACTAR;
					}
				}	
			}
		}

		//	NPC.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_NPC, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sACTAR.emACTAR = EMACTAR_NPC;
			sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			return sACTAR;
		}

		/* crow zone, Juver, 2018/02/21 */
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_ZONE_NAME, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sACTAR.emACTAR = EMACTAR_ZONE_NAME;
			sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			return sACTAR;
		}

		/* crow zone, Juver, 2018/02/21 */
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_GATE_NAME, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sACTAR.emACTAR = EMACTAR_GATE_NAME;
			sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			return sACTAR;
		}

		//	METERIAL
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MATERIAL, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sACTAR.emACTAR = EMACTAR_MATERIAL;
			sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			return sACTAR;
		}

		//	PC, OUR, ANY.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_PC, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			GLPARTY_CLIENT* pPARTY_CLT = GLPartyClient::GetInstance().FindMember(sTID_FOUND.dwID);

			//	우리편이면 OUR 아니면 ANY
			if ( pPARTY_CLT && bOUR )
			{
				sACTAR.emACTAR = EMACTAR_PC_OUR;
				sACTAR.sTARID = sTID_FOUND;
				return sACTAR;
			}
			else
			{
				sACTAR.emACTAR = EMACTAR_PC_ANY;
				sACTAR.sTARID = sTID_FOUND;
				return sACTAR;
			}
		}

		//	Note : 스킬의 '모두에게' 혹은 '우리편에게' 속성일 경우 선택된 케릭이 없을때,
		//			자신이 선택되는지 점검.
		if ( emFINDTAR==EMFIND_TAR_ANY || emFINDTAR==EMFIND_TAR_ENEMY )
		{
			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, m_vMax, m_vMin );
			if ( bCol )
			{
				//	우리편이면 OUR 아니면 ANY
				EMACTIONTAR		emACTAR = EMACTAR_PC_ANY;
				if ( bOUR )		emACTAR = EMACTAR_PC_OUR;

				sACTAR.emACTAR = emACTAR;
				sACTAR.sTARID = STARGETID(CROW_PC,m_dwGaeaID,m_vPos);
				return sACTAR;
			}
		}
	}

	// SUMMON ATTACK
	bool bFORCED_PK = (dwKeyZ&DXKEY_DOWNED)!=NULL;
	if ( bFORCED_PK )
	{
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_SUMMON, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
			if ( bENEMY )
			{
				PGLANYSUMMON pSummon = GLGaeaClient::GetInstance().GetSummon ( sTID_FOUND.dwID );
				if ( pSummon && pSummon->IsTargetReady() )
				{
					// 강제 공격키를 누르지 않아도 되는 경우( 성향 이벤트 중일경우 )
					bool bFORCED_PK = (dwKeyZ&DXKEY_DOWNED)!=NULL;
					if ( bFORCED_PK )
					{
						GLPARTY_CLIENT* pPARTY_CLT = GLPartyClient::GetInstance().FindMember(pSummon->m_dwOwner);
						if ( !pPARTY_CLT || !bOUR ) 
						{
							sACTAR.emACTAR = EMACTAR_SUMMON_ATTACK;
							sACTAR.sTARID = sTID_FOUND;
						}
						return sACTAR;
					}
				}
			}
		}
	}

	//	ITEM.
	pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_ITEM, STAR_ORDER() );
	if ( pairRange.first != pairRange.second )
	{
		sACTAR.emACTAR = EMACTAR_ITEM;
		sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
		return sACTAR;
	}

	//	MONEY.
	pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MONEY, STAR_ORDER() );
	if ( pairRange.first != pairRange.second )
	{
		sACTAR.emACTAR = EMACTAR_ITEM;
		sACTAR.sTARID = CFINDER::GetInstance().FindClosedCrow ( pairRange, vFromPt );
		return sACTAR;
	}

	return sACTAR;
}

SACTIONTAR GLCharacter::FindNearItem ()
{
	SACTIONTAR sACTAR_ITEM;
	SACTIONTAR sACTAR_MONEY;

	//	Crow 검색 ( 단, 연속 이동일 때는 검색하지 않는다. )
	DETECTMAP *pDetectMap = NULL;
	DETECTMAP_RANGE pairRange;
	STARGETID sTID_FOUND;

	DWORD emCrow = ( CROW_EX_ITEM | CROW_EX_MONEY );
	pDetectMap = GLGaeaClient::GetInstance().DetectCrow ( emCrow, m_vPos, MAX_VIEWRANGE/3.0f );
	if ( pDetectMap )
	{
		//	Item.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_ITEM, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, m_vPos );

			sACTAR_ITEM.emACTAR = EMACTAR_ITEM;
			sACTAR_ITEM.sTARID = sTID_FOUND;
		}

		//	Money.
		pairRange = std::equal_range ( pDetectMap->begin(), pDetectMap->end(), CROW_MONEY, STAR_ORDER() );
		if ( pairRange.first != pairRange.second )
		{
			sTID_FOUND = CFINDER::GetInstance().FindClosedCrow ( pairRange, m_vPos );

			sACTAR_MONEY.emACTAR = EMACTAR_ITEM;
			sACTAR_MONEY.sTARID = sTID_FOUND;
		}
	}

	if ( sACTAR_MONEY.emACTAR!=EMACTAR_NULL && sACTAR_ITEM.emACTAR!=EMACTAR_NULL )
	{
		std_afunc::CCompareTargetDist sDist(m_vPos);

		bool bMore = sDist.operator () ( sACTAR_MONEY.sTARID, sACTAR_ITEM.sTARID );

		if ( bMore )	return sACTAR_MONEY;
		else			return sACTAR_ITEM;
	}
	else if ( sACTAR_MONEY.emACTAR!=EMACTAR_NULL )
	{
		return sACTAR_MONEY;
	}
	else if ( sACTAR_ITEM.emACTAR!=EMACTAR_NULL )
	{
		return sACTAR_ITEM;
	}

	SACTIONTAR sACTAR;
	return sACTAR;
}

HRESULT GLCharacter::PlayerUpdate ( float fTime, float fElapsedTime )
{
	//	Note : 사용자 입력값 적용.
	//
	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();

	DWORD dwKeyR(NULL);
	DWORD dwKeyX(NULL);
	DWORD dwKeyLAlt(NULL);
	DWORD dwKeyLCtrl(NULL);
	DWORD dwKeyLShift(NULL);
	DWORD dwkeySPACE(NULL);

	DWORD dwML(NULL), dwMM(NULL), dwMR(NULL);


	//	Note : 채팅윈도우 활성화시 키보드 가져 오지 않음.
	//
	if ( !DXInputString::GetInstance().IsOn() )
	{
		dwKeyR = dxInputDev.GetKeyState (RPROFILE::MenuShortcut[SHORTCUT_RUN] );
		dwKeyX = dxInputDev.GetKeyState (RPROFILE::MenuShortcut[SHORTCUT_ATTACKMODE] );
		dwkeySPACE = dxInputDev.GetKeyState ( DIK_SPACE );
	}

#if defined( TW_PARAM ) || defined( HK_PARAM ) 
	if ( !m_sPMarket.IsOpen() ) 
	{
		if( m_bOneHourNotInputKey == FALSE )
		{
			if( dxInputDev.IsUpdatInputState() )
			{
				m_fKeyDownCheckTime = 0.0f;
			}else{
				m_fKeyDownCheckTime += fElapsedTime;

				if( m_fKeyDownCheckTime >= 3600 )
				{
					m_bOneHourNotInputKey = TRUE;
					CInnerInterface::GetInstance().SetBlockProgramFound(true);
				}
			}
		}
	}else{
		m_fKeyDownCheckTime = 0.0f;
	}

	CDebugSet::ToView ( 1, "KeyDownCheckTime %f", m_fKeyDownCheckTime );
#endif

	dwKeyLAlt = dxInputDev.GetKeyState ( DIK_LMENU );
	dwKeyLCtrl = dxInputDev.GetKeyState ( DIK_LCONTROL );
	dwKeyLShift = dxInputDev.GetKeyState ( DIK_LSHIFT );

	dwML = dxInputDev.GetMouseState ( DXMOUSE_LEFT );
	dwMM = dxInputDev.GetMouseState ( DXMOUSE_MIDDLE );
	dwMR = dxInputDev.GetMouseState ( DXMOUSE_RIGHT );

#ifdef _RELEASED
	// 메시지 해킹 테스트
	DWORD dwKeyA = dxInputDev.GetKeyState ( DIK_A );
	DWORD dwKeyQ = dxInputDev.GetKeyState ( DIK_Q );

	if ( (dwKeyA&DXKEY_DOWNED) && (dwKeyLShift&DXKEY_DOWNED) )
	{
        SendHackingMSG ();
	}

	if ( (dwKeyQ&DXKEY_DOWNED) && (dwKeyLShift&DXKEY_DOWNED) )
	{
		SendHackingMSG1 ();
	}
#endif

	//	Note : 트레이드 등록 예비 아이템 리셋.
	//
	if ( dwMR & DXKEY_DOWN )
	{
		GLTradeClient::GetInstance().ReSetPreItem();
	}

	//	Note : 것기 뛰기 변경.
	if ( dwKeyR&DXKEY_UP )
	{
		ReqToggleRun ();
	}

	//	Note : 평화 모드 모션 변경.
	if ( dwKeyX&DXKEY_UP )
	{
		ReqTogglePeaceMode ();
	}

	//	Note : 마켓, 창고, 거래창, 모달박스가 활성화시 케릭터 제어 잠김.
	//
	BOOL bCharMoveBlock = CInnerInterface::GetInstance().IsCharMoveBlock ();
	if ( bCharMoveBlock )
	{
		//DxCursor::GetInstance().SetCursorType(EMCS_NORMAL);
		CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);
		DxViewPort::GetInstance().SetHandling ( false );

		return S_FALSE;
	}

	//	Note : 카메라 키보드로 제어 on/off
	//
	{
		DxViewPort::GetInstance().SetHandling ( true );

		bool bKeyHandling(true);
		if ( DXInputString::GetInstance().IsOn() )	bKeyHandling = false;
		DxViewPort::GetInstance().SetKeyboard ( bKeyHandling );
	}

	D3DXVECTOR3 vTargetPt, vFromPt;
	vFromPt = DxViewPort::GetInstance().GetFromPt ();
	BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vTargetPt );
	if ( !bOk )
	{
		//when auto pilot is running, proceed with the code below 
		if ( isAutoPilotActive() == FALSE )
			return S_FALSE;
	}

	//	손에 든 아이탬이 있을때. 바닥을 찍는다면.
	bool bCLICK_LEFT = NULL != (dwML&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP));
	bool bCLICK_RIGHT = NULL != (dwMR&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP));

	bool bCLICK_LEFT_EFFECT = NULL != (dwML&(DXKEY_DOWN));
	if ( bCLICK_LEFT_EFFECT )
	{
		if ( ROPTION::bClickEffect )
			DxEffExtra::GetInstance().RestartClickEffect();
	}

	if ( VALID_HOLD_ITEM() && (bCLICK_LEFT||bCLICK_RIGHT) )
	{
		if ( dwML&DXKEY_UP || dwMR&DXKEY_UP )
		{
			D3DXVECTOR3 vCollisionPos;
			DWORD dwCollisionID;
			BOOL bCollision;
			GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
			(
				vFromPt,
				vTargetPt,
				vCollisionPos,
				dwCollisionID,
				bCollision
			);

			if ( bCollision )
			{
				D3DXVECTOR3 vDir = vCollisionPos - GetPosition();
				vDir.y = 0.0f;

				D3DXVec3Normalize ( &vDir, &vDir );
				D3DXVECTOR3 vDropPos = GetPosition() + vDir*float(CHARACTER_BODY_RADIUS+12);

				GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
				(
					vDropPos + D3DXVECTOR3(0,+5,0),
					vDropPos + D3DXVECTOR3(0,-5,0),
					vCollisionPos,
					dwCollisionID,
					bCollision
				);

				vDropPos = GetPosition();
				if ( bCollision )		vDropPos = vCollisionPos;

				if ( bCLICK_LEFT )		ReqHoldToField ( vDropPos );
				else					ReqHoldToFieldFireCracker ( vDropPos );

				return S_FALSE;
			}
		}
		else	return S_FALSE;
	}

	//	Note : 스킬 행동 우선일 경우 스킬 정보 가져옴.
	//
	PGLSKILL pRunSkill(NULL);
	if ( dwMR&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP) )
	{
		m_sActiveSkill = m_sRunSkill;

		SKILL_MAP_ITER learniter = m_ExpSkills.find ( m_sActiveSkill.dwID );
		if ( learniter!=m_ExpSkills.end() )
		{
			SCHARSKILL &sSkill = (*learniter).second;
			pRunSkill = GLSkillMan::GetInstance().GetData ( m_sActiveSkill.wMainID, m_sActiveSkill.wSubID );
		}
	}

	//	Note : 피행위자 찾기 속성.
	EMFIND_TAR emFINDTAR(ENFIND_TAR_NULL);
	if ( pRunSkill )
	{
		if ( pRunSkill->m_sAPPLY.IsSpec( EMSPECA_REBIRTH ) )
		{
			switch ( pRunSkill->m_sBASIC.emIMPACT_SIDE )
			{
			default:				break;
			case SIDE_ENEMY:		break;
			case SIDE_OUR:			emFINDTAR = EMFIND_TAR_OUR_DIE;		break;
			case SIDE_ANYBODY:		emFINDTAR = EMFIND_TAR_ANY_DIE;		break;
			};
		}
		else
		{
			switch ( pRunSkill->m_sBASIC.emIMPACT_SIDE )
			{
			default:				break;
			case SIDE_ENEMY:		emFINDTAR = EMFIND_TAR_ENEMY;	break;
			case SIDE_OUR:			emFINDTAR = EMFIND_TAR_OUR;		break;
			case SIDE_ANYBODY:		emFINDTAR = EMFIND_TAR_ANY;		break;
			};
		}
	}

	//	Note : 행동과 관련된 피행위자 찾기.
	//
	GLCOPY* pACTTAR(NULL);

	/*specific skill target, Juver, 2018/08/27 */
	if ( pRunSkill && pRunSkill->m_sBASIC.emIMPACT_TAR == TAR_SPECIFIC )
		m_sACTAR = SACTIONTAR();
	else
		m_sACTAR = FindActionTarget ( vTargetPt, vFromPt, emFINDTAR );

	EMCROW emCROW = m_sACTAR.sTARID.emCrow;

	//EMACTAR_PC_ANY;

	if ( ROPTION::bTargetEffect )
	{
		if ( bCLICK_LEFT || bCLICK_RIGHT )
		{
			if ( m_sACTARTarget.sTARID != m_sACTAR.sTARID ) 
			{
				if ( m_sACTAR.emACTAR != EMACTAR_NULL )
				{
					EMCROW emTargetCrowType = m_sACTAR.sTARID.emCrow;
					if ( emTargetCrowType == CROW_PC || 
						emTargetCrowType == CROW_NPC || 
						emTargetCrowType == CROW_MOB || 
						emTargetCrowType == CROW_SUMMON ||
						emTargetCrowType == CROW_MATERIAL )
					{
						m_sACTARTarget = SACTIONTAR();
						m_sACTARTarget = m_sACTAR;
					}
				}
			}
		}
	}

	/*target information display, Juver, 2018/08/22 */
	if ( m_sACTAR.sTARID.emCrow == CROW_PC || m_sACTAR.sTARID.emCrow == CROW_NPC || 
		m_sACTAR.sTARID.emCrow == CROW_MOB || m_sACTAR.sTARID.emCrow == CROW_SUMMON )
	{
		GLCOPY* pCopy = GLGaeaClient::GetInstance().GetCopyActor ( m_sACTAR.sTARID );
		if ( pCopy )
		{
			if ( !pCopy->IsACTION( GLAT_DIE ) && !pCopy->IsACTION( GLAT_FALLING ) )
			{
				D3DXVECTOR3 vCenterTarget = m_sACTAR.sTARID.vPos;
				vCenterTarget.y += 10.f;
				D3DXVECTOR3 vFromTEMP = vFromPt;
				if( !GLGaeaClient::GetInstance().IsMapCollsion( vCenterTarget, vFromTEMP ) )
					CInnerInterface::GetInstance().target_information_set ( m_sACTAR.sTARID );
			}
		}
	}

	bool bBRIGHTEVENT = GLGaeaClient::GetInstance().IsBRIGHTEVENT();

	/* open pk function, Juver, 2019/12/24 */
	bool bOpenPK = false;
	PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLandManClient && pLandManClient->IsOpenPK() )
	{
		bOpenPK = true;
	}

	/* open pk function, Juver, 2019/12/25 */
	if ( (emCROW==CROW_PC&&m_sACTAR.emACTAR==EMACTAR_PC_PVP) ||
		m_sACTAR.emACTAR==EMACTAR_MOB || 
		( bBRIGHTEVENT&&m_sACTAR.emACTAR!=EMACTAR_NPC ) ||
		( bOpenPK&&m_sACTAR.emACTAR!=EMACTAR_NPC ) ||
		(emCROW==CROW_SUMMON&&m_sACTAR.emACTAR==EMACTAR_SUMMON_ATTACK) )
	{
		pACTTAR = GLGaeaClient::GetInstance().GetCopyActor ( m_sACTAR.sTARID );
	}

	if ( pACTTAR )
	{
//#ifdef CH_PARAM
		// 중국 : 오른쪽 버튼으로 타켓 지정시 카메라 제어 잠김.
		// DxViewPort::GetInstance().SetHandling( false );
//#endif

		D3DXVECTOR3 vCenterTarget = m_sACTAR.sTARID.vPos;
		vCenterTarget.y += 10.f;

		D3DXVECTOR3 vFromTEMP = vFromPt;
		if( !GLGaeaClient::GetInstance().IsMapCollsion( vCenterTarget, vFromTEMP ) )
		{
			CInnerInterface::GetInstance().SetTargetInfo ( m_sACTAR.sTARID );
		}
	}
	else
	{
//#ifdef CH_PARAM
		// 중국 : 오른쪽 버튼으로 타켓 지정시 카메라 제어 풀림.
		// DxViewPort::GetInstance().SetHandling( true );
//#endif
	}

	if ( m_sCONFTING.bFIGHT && m_sCONFTING.sOption.bSCHOOL )
	{
		//	Note : 학교간 대련 마크 표시 Update.
		CInnerInterface::GetInstance().SetAcademyUpdateNumber
		(
			GLPartyClient::GetInstance().GetConfrontNum (),
			(int) m_sCONFTING.setConftMember.size()
		);
	}

	////	Note : 마우스 모양 선택.
	////
	//switch ( m_sACTAR.emACTAR )
	//{
	//case EMACTAR_NULL:		DxCursor::GetInstance().SetCursorType(EMCS_NORMAL);		break;
	//case EMACTAR_PC_PVP:	DxCursor::GetInstance().SetCursorType(EMCS_ATTACK);		break;
	//case EMACTAR_MOB:		DxCursor::GetInstance().SetCursorType(EMCS_ATTACK);		break;
	//case EMACTAR_NPC:
	//	{
	//		bool bTALK(false);
	//		PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	//		if ( pLAND )
	//		{
	//			PGLCROWCLIENT pCROW = pLAND->GetCrow(m_sACTAR.sTARID.dwID);
	//			if ( pCROW )
	//			{
	//				if ( strlen(pCROW->m_pCrowData->GetTalkFile()) )		bTALK = true;
	//			}
	//		}

	//		if ( bTALK )	DxCursor::GetInstance().SetCursorType(EMCS_TALK);
	//		else			DxCursor::GetInstance().SetCursorType(EMCS_NORMAL);
	//	}
	//	break;

	//case EMACTAR_PC_OUR:	DxCursor::GetInstance().SetCursorType(EMCS_NORMAL);		break;
	//case EMACTAR_PC_P2P:	DxCursor::GetInstance().SetCursorType(EMCS_NORMAL);		break;
	//case EMACTAR_PC_ANY:	DxCursor::GetInstance().SetCursorType(EMCS_NORMAL);		break;
	//case EMACTAR_ITEM:		DxCursor::GetInstance().SetCursorType(EMCS_SELECT);		break;
	//};
	//	Note : 마우스 모양 선택.
	//
	switch ( m_sACTAR.emACTAR )
	{
	case EMACTAR_NULL:			CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);		break;
	case EMACTAR_SUMMON_ATTACK: CCursor::GetInstance().SetCursorType(CURSOR_ATTACK);		break;
	case EMACTAR_PC_PVP:		CCursor::GetInstance().SetCursorType(CURSOR_ATTACK);		break;
	case EMACTAR_MOB:			CCursor::GetInstance().SetCursorType(CURSOR_ATTACK);		break;
	case EMACTAR_NPC:
		{
			bool bTALK(false);
			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
			if ( pLAND )
			{
				PGLCROWCLIENT pCROW = pLAND->GetCrow(m_sACTAR.sTARID.dwID);
				if ( pCROW )
				{
					if ( _tcslen(pCROW->m_pCrowData->GetTalkFile()) )		bTALK = true;
				}
			}

			if ( bTALK )	CCursor::GetInstance().SetCursorType(CURSOR_TALK);
			else			CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);
		}
		break;

	case EMACTAR_PC_OUR:	CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);		break;
	case EMACTAR_PC_P2P:	CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);		break;
	case EMACTAR_PC_ANY:	CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);		break;
	case EMACTAR_ITEM:		CCursor::GetInstance().SetCursorType(CURSOR_HAND);			break;
	case EMACTAR_MATERIAL:	CCursor::GetInstance().SetCursorType(CURSOR_NORMAL);		break;

		/* crow zone, Juver, 2018/02/21 */
	case EMACTAR_ZONE_NAME:	break;
	case EMACTAR_GATE_NAME:	break;
	};

	//	Note : 연속 강제 행동 모드 인지 체크.
	bool bcontinue = (dwKeyLCtrl&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP)) != NULL;
	if ( !bcontinue ) bcontinue = RPROFILE::bFORCED_ATTACK!=FALSE;

	BOOL bMove(FALSE);
	D3DXVECTOR3	vMoveTo(0,0,0);
	if ( !IsACTION(GLAT_ATTACK) && !IsACTION(GLAT_SKILL) && !m_bVehicle && !m_bReqVehicle )
	{
		STARGETID sTARID = m_sACTAR.sTARID;

		if ( pRunSkill )
		{			
			SetDefenseSkill( false );

			//	Note : 스킬 발동 시도.
			//
			EMIMPACT_TAR emTAR = pRunSkill->m_sBASIC.emIMPACT_TAR;

			/*specific skill target, Juver, 2018/08/27 */
			bool bSKILL_ACT = ( emTAR==TAR_SELF || emTAR==TAR_ZONE || emTAR==TAR_SPECIFIC );

			//	Note : 스킬 속성 검사.11
			bool bSK2ENERMY(false), bSK2OUR(false), bSK2ANYBODY(false);
			switch ( pRunSkill->m_sBASIC.emIMPACT_SIDE )
			{
			default:				break;
			case SIDE_ENEMY:
				bSK2ENERMY = true;
				bcontinue = false;	// 스킬 강제공격 옵션 off
				break;
			
			case SIDE_OUR:
				bSK2OUR = true;
				bcontinue = false;	//	연속 강제 행동 off ( 적에게 수행하는 것만 가능하게. )
				break;
			
			case SIDE_ANYBODY:
				bSK2ANYBODY = true;
				bcontinue = false;	//	연속 강제 행동 off ( 적에게 수행하는 것만 가능하게. )
				break;
			};

			if ( bSKILL_ACT && m_sACTAR.emACTAR==EMACTAR_NULL )
			{
				D3DXVECTOR3 vCollisionPos;
				DWORD dwCollisionID;
				BOOL bCollision;
				GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
				(
					vFromPt, vTargetPt,
					vCollisionPos, dwCollisionID, bCollision
				);

				if ( bCollision )
				{
					D3DXVECTOR3 vDir = vCollisionPos - GetPosition();
					sTARID.vPos = vCollisionPos;
				}
				else
				{
					if ( emTAR==TAR_ZONE )		bSKILL_ACT = false;

					/*specific skill target, Juver, 2018/08/27 */
					if ( emTAR==TAR_SPECIFIC )	bSKILL_ACT = false;
				}
			}

			switch ( m_sACTAR.emACTAR )
			{
			case EMACTAR_NULL:			if(bSKILL_ACT)	SkillReaction(sTARID,dwMR,bcontinue,bMove,vMoveTo);		break;
			case EMACTAR_SUMMON_ATTACK: if(bSK2ENERMY)	SkillReaction(sTARID,dwMR,bcontinue,bMove,vMoveTo);		break;
			case EMACTAR_PC_PVP:		if(bSK2ENERMY)	SkillReaction(sTARID,dwMR,bcontinue,bMove,vMoveTo);		break;
			case EMACTAR_MOB:			if(bSK2ENERMY)	SkillReaction(sTARID,dwMR,bcontinue,bMove,vMoveTo);		break;
			case EMACTAR_NPC:			break;
			case EMACTAR_PC_OUR:		if(bSK2OUR)		SkillReaction(sTARID,dwMR,bcontinue,bMove,vMoveTo);		break;
			case EMACTAR_PC_P2P:		break;
			case EMACTAR_PC_ANY:		if(bSK2ANYBODY)	SkillReaction(sTARID,dwMR,bcontinue,bMove,vMoveTo);		break;
			case EMACTAR_ITEM:			break;
			case EMACTAR_MATERIAL:		break;

				/* crow zone, Juver, 2018/02/21 */
			case EMACTAR_ZONE_NAME:	break;
			case EMACTAR_GATE_NAME:	break;
			};
		}
		else if ( dwML&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP) )
		{
			//	Note : 상대에 따른 반응.
			//
			switch ( m_sACTAR.emACTAR )
			{
			case EMACTAR_NULL:			break;
			case EMACTAR_SUMMON_ATTACK: PvPReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;
			case EMACTAR_PC_PVP:		PvPReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;
			case EMACTAR_MOB:			MobReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;
			case EMACTAR_NPC:			NpcReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;
			case EMACTAR_PC_OUR:		break;
			case EMACTAR_PC_P2P:		P2PReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;
			case EMACTAR_PC_ANY:		break;
			case EMACTAR_ITEM:			ItemReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;
			case EMACTAR_MATERIAL:		GatheringReaction(sTARID,dwML,bcontinue,bMove,vMoveTo);	break;

				/* crow zone, Juver, 2018/02/21 */
			case EMACTAR_ZONE_NAME:	break;
			case EMACTAR_GATE_NAME:	break;
			};
		}
		else if ( dwkeySPACE & DXKEY_DOWN )
		{
			m_sACTAR = FindNearItem ();
			STARGETID sTARID = m_sACTAR.sTARID;

			if ( m_sACTAR.emACTAR==EMACTAR_ITEM )
				ItemReaction(sTARID,DXKEY_DOWNED|DXKEY_UP,bcontinue,bMove,vMoveTo);
		}

		//when autopilot is active, scan for possible targets
		if ( isAutoPilotActive() == TRUE )
		{
			AutoPilotScanTarget( bcontinue, bMove, vMoveTo );
		}
	}

	//	Note : Reaction 에서 이동을 요청한 경우.
	//
	if ( bMove )
	{
		ActionMoveTo ( fTime, vMoveTo+D3DXVECTOR3(0,+5,0), vMoveTo+D3DXVECTOR3(0,-5,0), FALSE, TRUE );
		return S_OK;
	}

	//	Note : 일반 이동 처리.
	//
	BOOL bMOVEABLE = (m_sACTAR.emACTAR==EMACTAR_NULL);
	//				&& !IsACTION(GLAT_ATTACK) && !IsACTION(GLAT_SKILL);
	//				반응성 증가와 연속공격 취소를 원활하게 하기 위해서 액션 중간에도 취소가 가능하게 고침.
	if ( bMOVEABLE )
	{
		if ( dwML&DXKEY_UP )
		{
			CInnerInterface::GetInstance().DisableMinimapTarget();

			m_sREACTION.RESET();

			ReSetSTATE(EM_ACT_CONTINUEMOVE);
			ActionMoveTo ( fTime, vFromPt, vTargetPt );
		}
		else if ( dwML&DXKEY_DOWNED )
		{
			CInnerInterface::GetInstance().DisableMinimapTarget();

 			m_sREACTION.RESET();

			SetSTATE(EM_ACT_CONTINUEMOVE);
			ActionMoveTo ( fTime, vFromPt, vTargetPt, IsSTATE(EM_ACT_CONTINUEMOVE) );
		}
	}

	if ( !(dwML&DXKEY_DOWNED) && IsSTATE(EM_ACT_CONTINUEMOVE) )		ReSetSTATE(EM_ACT_CONTINUEMOVE);

	return S_OK;
}

HRESULT GLCharacter::UpateAnimation ( float fTime, float fElapsedTime )
{
	HRESULT hr=S_OK;

	if( !m_pSkinChar )	return E_FAIL;
	
	BOOL bLowSP = ( float(m_sSP.wNow) / float(m_sSP.wMax) ) <= GLCONST_CHAR::fLOWSP_MOTION;
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();

	BOOL bPeaceZone = pLand ? pLand->IsPeaceZone() : FALSE;
	if ( !bPeaceZone && IsSTATE(EM_ACT_PEACEMODE) )		bPeaceZone = TRUE;

	/*vehicle booster system, Juver, 2017/08/12 */
	BOOL bVehicleBooster = IsSTATE(EM_ACT_VEHICLE_BOOSTER);

	BOOL bFreeze = FALSE;

	EMANI_MAINTYPE emMType;
	EMANI_SUBTYPE emSType;

	/*skill transform, Juver, 2018/09/09 */
	if ( m_skill_transform_data.is_force_anim_valid() )
	{
		PANIMCONTNODE panimation_node = m_pSkinChar->GETANI ( AN_GESTURE, (EMANI_SUBTYPE)m_skill_transform_data.force_anim_id );
		if ( panimation_node )
		{
			m_pSkinChar->SELECTANI( AN_GESTURE, (EMANI_SUBTYPE)m_skill_transform_data.force_anim_id );
			
			goto animation_continue;
		}
	}

	switch ( m_Action )
	{
	case GLAT_IDLE:
		{
			m_fIdleTime += fElapsedTime;

			/* tyranny AFK disconnect, Juver, 2021/08/07 */
			m_fTyrannyAFKTimer += fElapsedTime;
			m_fCaptureTheFlagAFKTimer += fElapsedTime;

			if ( bPeaceZone && m_pSkinChar->GETANI ( AN_PLACID, AN_SUB_NONE ) )
			{
				emMType = AN_PLACID;
				emSType = AN_SUB_NONE;
				if ( m_bVehicle )
				{
					/*vehicle system, Juver, 2017/08/07 */
					int emType = m_sVehicle.m_emTYPE ;
					if ( emType == VEHICLE_TYPE_BOARD )
					{
						emSType = (EMANI_SUBTYPE) ( AN_SUB_HOVERBOARD );
					}else{
						emMType = CHECK_VEHICLE_MAIN( m_sVehicle.m_sVehicleID );
						/*vehicle booster system, Juver, 2017/08/12 */
						emSType = CHECK_VEHICLE_SUB( bVehicleBooster? AN_ATTACK : AN_PLACID );
					}
				}
				else  emSType = AN_SUB_NONE;
			}
			else
			{
				BOOL bPANT(FALSE); // 헐떡거림.

				if ( m_pSkinChar->GETANI ( AN_GUARD_L, m_emANISUBTYPE ) )
				{
					bPANT = bLowSP || m_bSTATE_PANT;
					for ( int i=0; i<EMBLOW_MULTI; ++i )
					{
						if ( m_sSTATEBLOWS[i].emBLOW == EMBLOW_STUN || m_sSTATEBLOWS[i].emBLOW == EMBLOW_MAD )		bPANT = TRUE;
					}
				}

				if ( bPANT )
				{
					emMType = AN_GUARD_L;
					emSType = m_emANISUBTYPE;
				}
				else
				{
					emMType = AN_GUARD_N;
					emSType = m_emANISUBTYPE;
				}
			}

			m_pSkinChar->SELECTANI ( emMType, emSType );
			m_bAttackable = TRUE;

			/*ABL system, Juver, 2017/06/02 */
			for ( int i=0; i<PIECE_SIZE; ++i )
			{
				if ( m_pSkinChar->m_pAttBone[i] )	
					m_pSkinChar->m_pAttBone[i]->SELECTANI( AN_ABL_STAY, AN_SUB_NONE );
			}

			/*vehicle system, Juver, 2017/08/07 */
			/*vehicle booster system, Juver, 2017/08/12 */
			if ( m_pSkinChar->m_pVehicle )	
			{
				m_pSkinChar->m_pVehicle->SELECTANI( bVehicleBooster? AN_VEHICLE_BOOSTER : AN_VEHICLE_STAY, AN_SUB_NONE );

				if ( bVehicleBooster && ( m_pSkinChar->ISENDANIM () || m_pSkinChar->m_pVehicle->ISENDANIM () ) )
					VehicleBoosterStateOff();
			}

		}break;

	case GLAT_MOVE:
		{
			emMType = IsSTATE(EM_ACT_RUN) ? AN_RUN : AN_WALK;
			emSType = bPeaceZone ? AN_SUB_NONE : m_emANISUBTYPE;
			if ( m_bVehicle )
			{
				/*vehicle system, Juver, 2017/08/07 */
				int emType = m_sVehicle.m_emTYPE;
				if ( emType == VEHICLE_TYPE_BOARD )
				{
					emSType = (EMANI_SUBTYPE) ( AN_SUB_HOVERBOARD );
				}else{
					emMType = CHECK_VEHICLE_MAIN( m_sVehicle.m_sVehicleID );
					/*vehicle booster system, Juver, 2017/08/12 */
					emSType = CHECK_VEHICLE_SUB( bVehicleBooster? AN_ATTACK : IsSTATE(EM_ACT_RUN) ? AN_RUN : AN_WALK );
				}
			}
			m_pSkinChar->SELECTANI ( emMType, emSType );

			/*ABL system, Juver, 2017/06/02 */
			for ( int i=0; i<PIECE_SIZE; ++i )
			{
				if ( m_pSkinChar->m_pAttBone[i] )	
					m_pSkinChar->m_pAttBone[i]->SELECTANI( AN_ABL_STAY, AN_SUB_NONE );
			}

			/*vehicle system, Juver, 2017/08/07 */
			/*vehicle booster system, Juver, 2017/08/12 */
			if ( m_pSkinChar->m_pVehicle )	
			{
				m_pSkinChar->m_pVehicle->SELECTANI( bVehicleBooster? AN_VEHICLE_BOOSTER : IsSTATE(EM_ACT_RUN) ? AN_VEHICLE_RUN : AN_VEHICLE_WALK, AN_SUB_NONE );

				if ( bVehicleBooster && ( m_pSkinChar->ISENDANIM () || m_pSkinChar->m_pVehicle->ISENDANIM () ) )
					VehicleBoosterStateOff();
			}
		}break;

	case GLAT_ATTACK:
		{
			EMSLOT emRHand = GetCurRHand();
			EMSLOT emLHand = GetCurLHand();

			/*skill transform, Juver, 2018/09/09 */
			EMANI_SUBTYPE emANISUBTYPE = CHECK_ATTACK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );
			m_pSkinChar->SELECTANI ( AN_ATTACK, emANISUBTYPE, NULL, m_dwANISUBSELECT );
			if ( m_pSkinChar->ISENDANIM () || m_bVehicle )
			{
				TurnAction ( GLAT_IDLE );

				/*ABL system, Juver, 2017/06/02 */
				for ( int i=0; i<PIECE_SIZE; ++i )
				{
					if ( m_pSkinChar->m_pAttBone[i] )	
						m_pSkinChar->m_pAttBone[i]->SELECTANI( AN_ABL_STAY, AN_SUB_NONE );
				}
			}
		}
		break;

	case GLAT_SKILL:
		{
			/*skill transform, Juver, 2018/09/09 */
			if ( m_skill_transform_data.is_skin_change_valid() )
				m_pSkinChar->SELECTANI ( AN_ATTACK, AN_SUB_NONE );
			else
				m_pSkinChar->SELECTSKILLANI ( GLCHARLOGIC_CLIENT::m_emANIMAINSKILL, GLCHARLOGIC_CLIENT::m_emANISUBSKILL );

			PSANIMCONTAINER pANIMCON = m_pSkinChar->GETCURANIM();
			if ( ( !(pANIMCON->m_dwFlag&ACF_LOOP) && m_pSkinChar->ISENDANIM () ) || m_bVehicle)
			{
				TurnAction ( GLAT_IDLE );
//				m_pSkinChar->m_bResetSkillAni = TRUE;

				/*ABL system, Juver, 2017/06/02 */
				for ( int i=0; i<PIECE_SIZE; ++i )
				{
					if ( m_pSkinChar->m_pAttBone[i] )	
						m_pSkinChar->m_pAttBone[i]->SELECTANI( AN_ABL_STAY, AN_SUB_NONE );
				}
			}	
		}
		break;

	case GLAT_TALK:
		{
			/* tyranny AFK disconnect, Juver, 2021/08/07 */
			m_fTyrannyAFKTimer += fElapsedTime;
			m_fCaptureTheFlagAFKTimer += fElapsedTime;

			BOOL bOK = m_pSkinChar->SELECTANI ( AN_GESTURE, EMANI_SUBTYPE(m_dwANISUBGESTURE) );
			if ( !bOK )
			{
				TurnAction ( GLAT_IDLE );
				break;
			}

			PSANIMCONTAINER pANIMCON = m_pSkinChar->GETCURANIM();
			if ( !(pANIMCON->m_dwFlag&ACF_LOOP) && m_pSkinChar->ISENDANIM () )
			{
				TurnAction ( GLAT_IDLE );
			}
		}
		break;

	case GLAT_SHOCK:
		{
			emMType = AN_SHOCK;	
			emSType = m_emANISUBTYPE;

			if ( m_bVehicle )
			{
				/*vehicle system, Juver, 2017/08/07 */
				int emType = m_sVehicle.m_emTYPE;
				if ( emType == VEHICLE_TYPE_BOARD )
				{
					emSType = (EMANI_SUBTYPE) ( AN_SUB_HOVERBOARD );
				}else{
					emMType = CHECK_VEHICLE_MAIN( m_sVehicle.m_sVehicleID );
					emSType = CHECK_VEHICLE_SUB( AN_SHOCK );
				}
			}

			m_pSkinChar->SELECTANI ( emMType, emSType );

			if ( m_pSkinChar->ISENDANIM () )	TurnAction ( GLAT_IDLE );

			/*vehicle system, Juver, 2017/08/07 */
			if ( m_pSkinChar->m_pVehicle )	
				m_pSkinChar->m_pVehicle->SELECTANI( AN_VEHICLE_SHOCK, AN_SUB_NONE );

		}break;
		
	case GLAT_PUSHPULL:
		m_pSkinChar->SELECTANI ( AN_SHOCK, GLCHARLOGIC_CLIENT::m_emANISUBTYPE );
		break;

		/*dash skill logic, Juver, 2017/06/17 */
	case GLAT_SKILL_DASH:
		{
			if ( m_dwActionAnim != AN_SUB_00_SIZE )
				m_pSkinChar->SELECTANI ( AN_SPEC, (EMANI_SUBTYPE)m_dwActionAnim );
		}break;

	case GLAT_FALLING:
		{
			emMType = AN_DIE;
			emSType = AN_SUB_NONE;

			if ( m_bVehicle )
			{
				/*vehicle system, Juver, 2017/08/07 */
				int emType = m_sVehicle.m_emTYPE;
				if ( emType == VEHICLE_TYPE_BOARD )
				{
					emSType = (EMANI_SUBTYPE) ( AN_SUB_HOVERBOARD );
				}else{
					emMType = CHECK_VEHICLE_MAIN( m_sVehicle.m_sVehicleID );
					emSType = CHECK_VEHICLE_SUB( AN_DIE );
				}
			}

			/*ABL system, Juver, 2017/06/02 */
			for ( int i=0; i<PIECE_SIZE; ++i )
			{
				if ( m_pSkinChar->m_pAttBone[i] )	
					m_pSkinChar->m_pAttBone[i]->SELECTANI( AN_ABL_FREE, AN_SUB_NONE );
			}

			/*vehicle system, Juver, 2017/08/07 */
			if ( m_pSkinChar->m_pVehicle )	
				m_pSkinChar->m_pVehicle->SELECTANI( AN_VEHICLE_DOWN, AN_SUB_NONE, EMANI_ENDFREEZE );

			m_pSkinChar->SELECTANI ( emMType, emSType, EMANI_ENDFREEZE );

			if ( m_pSkinChar->ISENDANIM () )
			{
				TurnAction ( GLAT_DIE );
			}
		}break;

	case GLAT_DIE:
		{
			bFreeze = TRUE;

			/*ABL system, Juver, 2017/06/02 */
			for ( int i=0; i<PIECE_SIZE; ++i )
			{
				if ( m_pSkinChar->m_pAttBone[i] )
					m_pSkinChar->m_pAttBone[i]->SELECTANI( AN_ABL_FREE, AN_SUB_NONE );
			}

			/*vehicle system, Juver, 2017/08/07 */
			if ( m_pSkinChar->m_pVehicle )	
			{
				m_pSkinChar->m_pVehicle->SELECTANI( AN_VEHICLE_DOWN, AN_SUB_NONE, EMANI_ENDFREEZE );
				m_pSkinChar->m_pVehicle->TOENDTIME();
			}


			m_pSkinChar->TOENDTIME();
		}break;

	case GLAT_CONFT_END:
		{
			EMANI_MAINTYPE emMTYPE(AN_CONFT_LOSS);
			if ( IsSTATE(EM_ACT_CONFT_WIN) )	emMTYPE = AN_CONFT_WIN;

			m_pSkinChar->SELECTANI ( emMTYPE, AN_SUB_NONE );
			if ( m_pSkinChar->ISENDANIM () )	TurnAction ( GLAT_IDLE );
		}
		break;

	case GLAT_GATHERING:
		{
			BOOL bOK = m_pSkinChar->SELECTANI ( AN_GATHERING, EMANI_SUBTYPE(m_dwANISUBGESTURE) );
			if ( !bOK )
			{
				TurnAction ( GLAT_IDLE );
				break;
			}

			PSANIMCONTAINER pANIMCON = m_pSkinChar->GETCURANIM();
			if ( !(pANIMCON->m_dwFlag&ACF_LOOP) && m_pSkinChar->ISENDANIM () )
			{
				TurnAction ( GLAT_IDLE );
			}
		}
		break;
	};

	/*skill transform, Juver, 2018/09/09 */
animation_continue:

	//	Note : 스킨 업데이트.
	//
	m_pSkinChar->SetPosition ( m_vPos );

	BOOL bContinue = IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL);

	//	Note : 상태 이상에 따라 모션 속도를 조정한다.
	//
	float fSkinAniElap = fElapsedTime;
	switch ( m_Action )
	{
	case GLAT_MOVE:
		fSkinAniElap *= ( GLCHARLOGIC_CLIENT::GETMOVE_ITEM() + GLCHARLOGIC_CLIENT::GETMOVEVELO() );
		break;

	case GLAT_ATTACK:
	case GLAT_SKILL:
		fSkinAniElap *= GLCHARLOGIC_CLIENT::GETATTVELO();
		fSkinAniElap += GLCHARLOGIC_CLIENT::GETATT_ITEM(); // 절대치
		break;
	};

	if ( !IsSTATE(EM_REQ_VISIBLENONE) )	m_pSkinChar->FrameMove ( fTime, fSkinAniElap, bContinue, bFreeze, !bPeaceZone );
	
	//	Note : 비 전투 지역일때 표시 안되야 하는 아이템(단검,투척) 인지 검사후 랜더 끄기.
	//
	if ( bPeaceZone )
	{
		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		SITEM *pItemR = GET_SLOT_ITEMDATA ( emRHand );
		SITEM *pItemL = GET_SLOT_ITEMDATA ( emLHand );

		if ( pItemR )
		{
			BOOL bRend = !( pItemR->sSuitOp.emAttack==ITEMATT_DAGGER || pItemR->sSuitOp.emAttack==ITEMATT_THROW );
			m_pSkinChar->SetPartRend ( PIECE_RHAND, bRend );
		}
		if ( pItemL )
		{
			BOOL bRend = !( pItemL->sSuitOp.emAttack==ITEMATT_DAGGER || pItemL->sSuitOp.emAttack==ITEMATT_THROW );
			m_pSkinChar->SetPartRend ( PIECE_LHAND, bRend );
		}
	}
	else
	{
		m_pSkinChar->SetPartRend ( PIECE_RHAND, TRUE );
		m_pSkinChar->SetPartRend ( PIECE_LHAND, TRUE );
	}

	return S_OK;
}

// *****************************************************
// Desc: 게이트 조사
// *****************************************************
DWORD GLCharacter::DetectGate ()
{
	if ( IsSTATE(EM_REQ_GATEOUT) )		return UINT_MAX;

	PLANDMANCLIENT pLandMClient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLandMClient )								return UINT_MAX;

	DxLandGateMan *pLandGateMan = &pLandMClient->GetLandGateMan();
	if ( !pLandGateMan )								return UINT_MAX;
		
	if ( pLandMClient->m_fAge<3.0f )					return UINT_MAX;

	PDXLANDGATE pLandGate = pLandGateMan->DetectGate ( GetPosition() );
	if ( !pLandGate )									return UINT_MAX;
		
	if ( pLandGate->GetFlags()&DxLandGate::GATE_OUT )	return pLandGate->GetGateID();

	return UINT_MAX;
}

// *****************************************************
// Desc: 게이트의 이름 조사
// *****************************************************
void GLCharacter::DetectGateToMapName ( std::vector<std::string> &vecString )
{
	vecString.clear();

	if ( IsSTATE(EM_REQ_GATEOUT) )						return;

	PLANDMANCLIENT pLandMClient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLandMClient )								return;

	DxLandGateMan *pLandGateMan = &pLandMClient->GetLandGateMan();
	if ( !pLandGateMan )								return;
		
	if ( pLandMClient->m_fAge<3.0f )					return;

	PDXLANDGATE pLandGate = pLandGateMan->DetectGate ( GetPosition() );
	if ( !pLandGate )									return;

	if ( pLandGate->GetFlags()&DxLandGate::GATE_OUT )
	{
		/* multi gate out, Juver, 2020/11/17 */
		if ( pLandGate->GetNewSystem() )
		{
			for ( DWORD i = 0; i<pLandGate->GetToMapNum(); ++i )
			{
				SNATIVEID sToMapID = pLandGate->GetToMapID(i);
				SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( sToMapID );
				if ( pMapNode )
				{
					DWORD dwNameINDEX = pLandGate->GetNameINDEX(i);
					std::string strNameKEY = pLandGate->GetNameKEY(i);

					std::string strTEMP("");

					if ( dwNameINDEX != DxLandGate::GATE_OUT_INDEX_NULL && strNameKEY.size() )
						strTEMP = ID2GAMEWORD( strNameKEY.c_str(), dwNameINDEX );

					if ( strTEMP.empty() )
						strTEMP = pMapNode->strMapName.c_str();

					vecString.push_back( strTEMP );

				}
			}
		}
		else
		{
			SNATIVEID sToMapID = pLandGate->GetToMapID( DxLandGate::DEFAULT_GATE_OUT_INDEX );
			SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( sToMapID );
			if ( pMapNode )
				vecString.push_back( pMapNode->strMapName );
		}
		
	}
}

float GLCharacter::GetMoveVelo ()
{
	float fDefaultVelo = IsSTATE(EM_ACT_RUN) ? GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fRUNVELO : GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fWALKVELO;
	float fMoveVelo = fDefaultVelo * ( GLCHARLOGIC_CLIENT::GETMOVEVELO() + GLCHARLOGIC_CLIENT::GETMOVE_ITEM() );
	return fMoveVelo;
}

void GLCharacter::VietnamCalculate()
{
	const int nMax1 = 180;
	const int nMax2 = 300;
	/*const int nMax1 = 30;
	const int nMax2 = 50;*/
	CInnerInterface::GetInstance().SET_VNGAINTYPE_GAUGE( 0, nMax2 );
   	if( m_sVietnamSystem.loginTime == 0 ) return;
	if( m_dwVietnamGainType == GAINTYPE_EMPTY ) 
	{
		CInnerInterface::GetInstance().SET_VNGAINTYPE_GAUGE( nMax2, nMax2 );
		return;
	}

	CTimeSpan gameTime( 0, (int)m_sVietnamSystem.gameTime / 60, (int)m_sVietnamSystem.gameTime % 60, 0 );


	CTime loginTime   = m_sVietnamSystem.loginTime;
	CTime crtTime     = GLGaeaClient::GetInstance().GetCurrentTime();
	CTimeSpan crtGameSpan;
	crtGameSpan			  = gameTime + ( crtTime - loginTime );

	int totalMinute  = (int)crtGameSpan.GetTotalMinutes();
	int totalSecond  = (int)crtGameSpan.GetTotalSeconds();

	CDebugSet::ToView ( 1, "Vietnam total Time M %d S %d", totalMinute, totalSecond );
	CDebugSet::ToView ( 2, "GameTime H %d M %d S %d Value %d", gameTime.GetTotalHours(), gameTime.GetTotalMinutes(), gameTime.GetTotalSeconds(),
						m_sVietnamSystem.gameTime );
	CDebugSet::ToView ( 3, "CrtTime Year %d Mon %d D %d H %d M %d S %d LoginTime Year %d Mon %d D %d H %d M %d S %d", 
							crtTime.GetYear(), crtTime.GetMonth(), crtTime.GetDay(), crtTime.GetHour(), crtTime.GetMinute(), crtTime.GetSecond(), 
							loginTime.GetYear(), loginTime.GetMonth(), loginTime.GetDay(), loginTime.GetHour(), loginTime.GetMinute(), loginTime.GetSecond() );


	if( m_dwVietnamGainType == GAINTYPE_MAX )
	{
		if( totalMinute > nMax1 )
			totalMinute = nMax1;
	}else if( m_dwVietnamGainType == GAINTYPE_HALF )
	{
		if( totalMinute < nMax1 )
			totalMinute = nMax1;
		if( totalMinute > nMax2 )
			totalMinute = nMax2;
	}

	if( totalMinute < 0 )  totalMinute = 0;
	
	CInnerInterface::GetInstance().SET_VNGAINTYPE_GAUGE( totalMinute, nMax2 );
	
	
	//	m_sVietnamSystem.currentGameTime = totalHours;
	//	m_sVietnamSystem.currentGameTime = totalMinutes;
	// 테스트는 초로 저장..
}

void GLCharacter::EventCalculate()
{
	if( m_bEventStart == FALSE )
	{
//		CDebugSet::ToView ( 1, "Event Start FALSE" );	
		return;
	}
	if( m_bEventApply == FALSE )
	{
//		CDebugSet::ToView ( 1, "Event Apply FALSE" );	
		return;
	}

	CTime crtTime   = GLGaeaClient::GetInstance().GetCurrentTime();
	CTime startTime = m_tLoginTime;
	CTimeSpan crtGameSpan;
	crtGameSpan					 = ( crtTime - startTime );

	if( m_bEventBuster == FALSE )
	{
		m_RemainEventTime  = m_EventStartTime - (int)crtGameSpan.GetTotalSeconds();
		m_RemainBusterTime = m_EventBusterTime;

		if( crtGameSpan.GetTotalSeconds() >= m_EventStartTime )
		{
//			m_bEventBuster = TRUE;	
			m_RemainEventTime = 0;
		}
	}

	if( m_bEventBuster == TRUE )
	{
		m_RemainEventTime  = 0;
		m_RemainBusterTime = (m_EventStartTime + m_EventBusterTime) - (int)crtGameSpan.GetTotalSeconds();
		if( m_RemainBusterTime > m_EventBusterTime )
			m_RemainBusterTime = m_EventBusterTime;

		if( crtGameSpan.GetTotalSeconds() >= m_EventStartTime + m_EventBusterTime )
		{
//			m_bEventBuster = FALSE;
//			m_tLoginTime   = crtTime.GetTime();

			m_RemainEventTime  = m_EventStartTime;
			m_RemainBusterTime = m_EventBusterTime;
		}
	}

	

	
	/*CDebugSet::ToView ( 1, "gameSpan: %d ;; bEventBuster: %d ;; startTime: %d ;; busterTime: %d", 
					    (int)crtGameSpan.GetTotalSeconds(), m_bEventBuster, m_EventStartTime, m_EventBusterTime );

	CDebugSet::ToView ( 2, "RemainEventTime: %d RemainBusterTime: %d", m_RemainEventTime, m_RemainBusterTime );*/

	
}

void GLCharacter::UpdateSpecialSkill()
{
	// 변신 스킬 체크
	if( m_dwTransfromSkill != SNATIVEID::ID_NULL )
	{
		if( m_sSKILLFACT[m_dwTransfromSkill].IsSpecialSkill( SKILL::EMSSTYPE_TRANSFORM ) )
		{
			// 쓰러지는 모션이면 변신해제
			STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
			if( IsACTION( GLAT_FALLING ) )
			{
				FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[m_dwTransfromSkill].sNATIVEID );
			}

			if( m_sSKILLFACT[m_dwTransfromSkill].sNATIVEID != NATIVEID_NULL() ) 
			{			
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[m_dwTransfromSkill].sNATIVEID.wMainID, m_sSKILLFACT[m_dwTransfromSkill].sNATIVEID.wSubID );
				if( pSkill && pSkill->m_sSPECIAL_SKILL.emSSTYPE == SKILL::EMSSTYPE_TRANSFORM )
				{
					if( pSkill->m_sSPECIAL_SKILL.strEffectName.size() != 0 )
					{
						if( m_sSKILLFACT[m_dwTransfromSkill].fAGE <= pSkill->m_sSPECIAL_SKILL.dwRemainSecond && 
							!m_sSKILLFACT[m_dwTransfromSkill].bRanderSpecialEffect )
						{
							D3DXMATRIX matTrans;							
							D3DXMatrixTranslation ( &matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
							DxEffGroupPlayer::GetInstance().NewEffGroup ( pSkill->m_sSPECIAL_SKILL.strEffectName.c_str(), matTrans, &sTargetID );
							m_sSKILLFACT[m_dwTransfromSkill].bRanderSpecialEffect = TRUE;
						}
					}
				}
			}else{
				DISABLESKEFF(m_dwTransfromSkill);
				UpdateSuit(TRUE);				
				m_dwTransfromSkill = SNATIVEID::ID_NULL;
			}

			
		}else{
			DISABLESKEFF(m_dwTransfromSkill);
			UpdateSuit(TRUE);
			m_dwTransfromSkill = SNATIVEID::ID_NULL;
		}
	}
}

void GLCharacter::SET_RECORD_CHAT()
{
	if( m_bRecordChat ) return;
	m_bRecordChat = TRUE;
	m_strRecordChat		= CInnerInterface::GetInstance().GET_RECORD_CHAT();
	m_recordStartTime	= GLGaeaClient::GetInstance().GetCurrentTime();

	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RECORD_CHAT_START") );
}

void GLCharacter::UPDATE_RECORD_CHAT( bool bCloseClient /* = FALSE */ )
{
	if( !m_bRecordChat ) return;
	CTime curTime = GLGaeaClient::GetInstance().GetCurrentTime();
	CTimeSpan timeSpan = curTime - m_recordStartTime;
	if( timeSpan.GetMinutes() >= 2 || bCloseClient ) 
	{
		m_strRecordChat += CInnerInterface::GetInstance().GET_RECORD_CHAT();

		{
			TCHAR szFullPathFileName[MAX_PATH] = {0};
			StringCchCopy( szFullPathFileName, MAX_PATH, SUBPATH::APP_ROOT );
			StringCchCat( szFullPathFileName, MAX_PATH, SUBPATH::SAVE_ROOT );
			CreateDirectory( szFullPathFileName, NULL );

			CHAR  szFileName[MAX_PATH] = {0};
			sprintf( szFileName, "chat_[%d%d%d%d%d].txt", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
													      curTime.GetHour(), curTime.GetMinute() );

			StringCchCat ( szFullPathFileName, MAX_PATH, szFileName );

			m_bRecordChat   = FALSE;

			CFile file;
			if ( ! file.Open( _T(szFullPathFileName), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary ) )// 파일 열기
			{

				CDebugSet::ToListView ( "RECORD_CHAT_FAILED" );
				return;
			}

			CString strSaveChat;			
			for( int i = 0; i < m_strRecordChat.GetLength(); i++ )
			{
				char szTempChar = m_strRecordChat.GetAt(i) ^ 0x2139;
				strSaveChat += szTempChar;
			}

			int nLength = strSaveChat.GetLength();
			file.Write(&nLength, sizeof(int));
			file.Write(strSaveChat.GetString(), nLength);
			file.Close();


			m_strRecordChat = "";
		}

		if( !bCloseClient )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RECORD_CHAT_END") );
		}

	}
}
/**MMR Rank Aura, Mhundz 01/02/2025**/
//void GLCharacter::MMRAuraEffects()
//{
//    const char* effects[] = {
//        "Rank0.egp",
//        "Rank1.egp",
//        "Rank2.egp",
//        "Rank3.egp",
//        "Rank4.egp",
//        "Rank5.egp",
//        "Rank6.egp",
//        "Rank7.egp",
//        "Rank8.egp",
//        "Rank9.egp"
//    };
//
//    int rank = -1;
//    for (int i = 9; i >= 0; --i) 
//    {
//        if (m_dwMMR >= GLCONST_CHAR::dwRank_Point[i]) 
//        {
//            rank = i;
//            break;
//        }
//    }
//
//    if (rank >= 0 && rank <= 9) 
//    {
//        for (int i = 0; i <= 9; ++i) 
//        {
//            if (i != rank) 
//            {
//                DxEffGroupPlayer::GetInstance().DeletePassiveEffect(
//                    effects[i],
//                    STARGETID(CROW_PC, m_dwGaeaID, m_vPos)
//                );
//            }
//        }
//
//        DxEffGroupPlayer::GetInstance().PassiveEffect(
//            effects[rank],
//            m_matTrans,
//            STARGETID(CROW_PC, m_dwGaeaID, m_vPos)
//        );
//    }
//}

void GLCharacter::UpdateLandEffect()
{
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if( !pLand ) return;

	SLEVEL_ETC_FUNC *pLevelEtcFunc = pLand->GetLevelEtcFunc();
	if( !pLevelEtcFunc ) return;
	if( !pLevelEtcFunc->m_bUseFunction[EMETCFUNC_LANDEFFECT] ) return;

	// 모든 버프를 취소해야할 경우의 처리
	for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
	{
		SLANDEFFECT landEffect = m_sLandEffect[i];
		if( !landEffect.IsUse() ) continue;
		if( landEffect.emLandEffectType == EMLANDEFFECT_CANCEL_ALLBUFF )
		{
			if( m_dwTransfromSkill != SNATIVEID::ID_NULL )
			{
				m_dwTransfromSkill = SNATIVEID::ID_NULL;
				UpdateSuit(TRUE);
			}
			for ( int i=0; i<SKILLFACT_SIZE; ++i )
			{
				FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[i].sNATIVEID );
				DISABLESKEFF(i);
			}			
			return;
		}
	}
}

HRESULT GLCharacter::FrameMove ( float fTime, float fElapsedTime )
{
	
	/**MMR Rank Aura, Mhundz 01/02/2025**/
	//MMRAuraEffects();
	
	HRESULT hr=S_OK;


	UPDATE_RECORD_CHAT();

#if defined(VN_PARAM) //vietnamtest%%%
	VietnamCalculate();
#endif

#ifndef CH_PARAM_USEGAIN
	EventCalculate();
#endif

	// 만약 기다리고 있다면
	if ( IsSTATE(EM_ACT_WAITING) )
	{
		if ( ++m_dwWAIT > EMWAIT_COUNT )
		{
			m_dwWAIT = 0;
			ReSetSTATE(EM_ACT_WAITING);

			//	Note : 맵에 진입함을 서버에 알려줌. ( 주위 환경 정보를 받기 위해서. )
			//
			GLMSG::SNETREQ_LANDIN NetMsgLandIn;
			NETSEND ( &NetMsgLandIn );
			CDebugSet::ToListView ( "GLMSG::SNETREQ_LANDIN" );

			//	Note : 서버에 준비되었음을 알림.
			//
			GLMSG::SNETREQ_READY NetMsgReady;
			NETSEND ( &NetMsgReady );
			CDebugSet::ToListView ( "GLMSG::SNETREQ_READY" );

			//	Note : 서버에 친구 리스트 전송 요청.
			GLFriendClient::GetInstance().ReqFriendList();

			/* award title, Juver, 2021/02/12 */
			GLAwardTitleClient::GetInstance().RequestData();

			// PET
			// 팻이 활동중에 맵을 이동했다면 다시 불러준다.
			if ( m_bIsPetActive )
			{
				 ReqReGenPet ();
				m_bIsPetActive = FALSE;
			}

			if ( m_bIsVehicleActive ) 
			{
				if ( ReqSetVehicle( true ) == S_OK ) m_bIsVehicleActive = FALSE;
			}

			// 탈것 장착하고 있다면 정보 요청 
			if ( m_PutOnItems[SLOT_VEHICLE].sNativeID != NATIVEID_NULL()  )
			{
				ReqVehicleUpdate();
			}

			// 인벤의 장착하고 있는 탈것 정보 요청
			if ( !m_bRecivedVehicleItemInfo ) 
			{
				ReqVehicleInvenUpdate();
				m_bRecivedVehicleItemInfo = true;
			}

			// 들고있는 팻카드 정보 요청
			if ( !m_bRecievedPetCardInfo )
			{
				// PET
				// 들고있는 팻카드의 정보를 요청한다.
				ReqPetCardInfo ();

#if defined KRT_PARAM || defined _RELEASED
				// PET
				// 시효만료로 사라진 팻카드의 팻정보 요청
				ReqPetReviveInfo ();
#endif
				m_bRecievedPetCardInfo = true;
			}
		}
	}

	//	Note : 살아있을 때만 갱신되는 것들.
	//
	if ( IsValidBody() )
	{
// 		bool bCHECK = CheckPASSIVITY ( fElapsedTime );
// 		if ( bCHECK )
// 		{
// 			//	Note : 수동적 행위 체크 박스 띄움.
// 			CInnerInterface::GetInstance().SetBlockProgramAlarm ( true );
// 		}
// 
// 		if ( IsBlockPASSIVITY() && !DxGlobalStage::GetInstance().GetBlockDetectState() )
// 		{
// 			DxGlobalStage::GetInstance().SetDetectedReport ();
// 
// 			GLMSG::SNET_BLOCK_DETECTED	NetMsg;
// 			NetMsg.dwDETECTED = UINT_MAX;
// 			NetMsg.dwCHARID = m_dwCharID;
// 			NETSEND ( &NetMsg );
// 
// 			DxGlobalStage::GetInstance().CloseGame();
// 		}

		m_cQuestPlay.FrameMove ( fTime, fElapsedTime );

		//	Note : 플래이어 조작 처리.
		//
		if ( !IsCtrlBlockBody() )
		{
			PlayerUpdate ( fTime, fElapsedTime );

			//	Note : 예약행위 검사후 수행.
			//		PlayerUpdate() 안에서는 인터페이스 우선 순위 때문에 처리 할수 없음.
			//
			if ( m_sREACTION.ISVALID() )
			{
				ReservedAction ( fTime );
			}
		}

		//	Note : 체력 상태 갱신.
		//
		float fCONFT_POINT_RATE(1.0f);
		if ( m_sCONFTING.IsCONFRONTING() )		fCONFT_POINT_RATE = m_sCONFTING.sOption.fHP_RATE;

		// 지형 효과 업데이트
		UpdateLandEffect();
		GLCHARLOGIC_CLIENT::UPDATE_DATA ( fTime, fElapsedTime, TRUE, fCONFT_POINT_RATE );

		if ( m_bCLUB_CERTIFY )
		{
			m_fCLUB_CERTIFY_TIMER += fElapsedTime;

			float fRER = m_fCLUB_CERTIFY_TIMER / GLCONST_CHAR::fCDCERTIFY_TIME;
			if ( fRER > 1.0f )	fRER = 1.0f;

			CInnerInterface::GetInstance().SET_CONFT_CONFIRM_PERCENT ( fRER );
		}

		/* skill turn/find invisible, Juver, 2020/04/26 */
		//force walk/run state on active invisible animation type
		//this code is here as m_wSkillTurnInvisibleAnimation is updated by GLCHARLOGIC_CLIENT::UPDATE_DATA every frame
		//this code needs to be moved somewhere so checking wont happen every frame

		if( m_bSkillTurnInvisibleLast != m_bSkillTurnInvisible ||
			m_wSkillTurnInvisibleAnimationLast != m_wSkillTurnInvisibleAnimation ||
			m_sSkillTurnInvisibleIDLast != m_sSkillTurnInvisibleID )
		{
			m_bSkillTurnInvisibleLast = m_bSkillTurnInvisible;
			m_wSkillTurnInvisibleAnimationLast = m_wSkillTurnInvisibleAnimation;
			m_sSkillTurnInvisibleIDLast = m_sSkillTurnInvisibleID;

			if ( m_bSkillTurnInvisible )
			{
				if ( m_wSkillTurnInvisibleAnimation != SKILL::EMSPEC_INVISIBLE_ANI_TYPE_NONE )
				{
					if ( m_wSkillTurnInvisibleAnimation == SKILL::EMSPEC_INVISIBLE_ANI_TYPE_WALK )
					{
						//force walk state
						if ( IsSTATE(EM_ACT_RUN))
						{
							ReqToggleRun();
						}
					}

					if ( m_wSkillTurnInvisibleAnimation == SKILL::EMSPEC_INVISIBLE_ANI_TYPE_RUN )
					{
						//force walk state
						if ( !IsSTATE(EM_ACT_RUN))
						{
							ReqToggleRun();
						}
					}
				}
			}
		}


	}

	m_fAccumTime += fElapsedTime;

	UpdateTargetEffect();

	/*pvp capture the flag, Juver, 2018/02/08 */
	CaptureTheFlagLocatorUpdate();

	/*game stats, Juver, 2017/06/22 */
	{
		static FLOAT _fLastTime = 0.0f;
		static DWORD _dwFrames  = 0L;

		 ++_dwFrames;

		if( fTime - _fLastTime > 1.0f )
		{
			float _fFPS    = _dwFrames / (fTime - _fLastTime);
			_fLastTime = fTime;
			_dwFrames  = 0L;

			CInnerInterface::GetInstance().GetGameStatsDisplay()->UpdateFPS( _fFPS );
		}
	}

	/*game stats, Juver, 2017/06/21 */
	{
		static FLOAT _fLastTime = 0.0f;

		if( fTime - _fLastTime > 1.0f )
		{
			if ( m_bRecievedPingAgent )
			{
				//send to agent
				GLMSG::SNETPC_PING_PACKET_AGENT netMsgPingAgent;
				NETSEND( &netMsgPingAgent ); 

				m_bRecievedPingAgent = false;
				m_clock_Packet_agent = clock();
			}
			
			if ( m_bRecievedPingField )
			{
				//send to field
				GLMSG::SNETPC_PING_PACKET_FIELD netMsgPingField;
				NETSENDTOFIELD( &netMsgPingField ); 

				m_bRecievedPingField = false;
				m_clock_Packet_field = clock();
			}

			_fLastTime = fTime;
		}
	}


	/*rightclick wear/unwear, Juver, 2017/06/24 */
	if ( m_fWearDelay < 1.0f )
		m_fWearDelay += fElapsedTime;

	/*pet fix add call delay, EJCode, 2018/11/28 */
	if ( m_fPetUseDelay < RPARAM::pet_call_delay )
		m_fPetUseDelay += fElapsedTime;

	/*npc shop, Juver, 2017/07/27 */
	if ( m_fNPCShopDelay < NPC_SHOP_DELAY )
		m_fNPCShopDelay += fElapsedTime;

	/*item exchange, Juver, 2017/10/13 */
	if ( m_fNPCItemExchangeDelay < NPC_ITEM_EXCHANGE_DELAY )
		m_fNPCItemExchangeDelay += fElapsedTime;

	/*12-9-14, Battle Pass - CNDev*/
	if ( m_fBattlePassDelay < BATTLEPASS_DELAY )
		m_fBattlePassDelay += fElapsedTime;
	/*12-16-24, daily login - CNDev*/
	if ( m_fDailyLoginDelay < DAILYLOGIN_DELAY )
		m_fDailyLoginDelay += fElapsedTime;
	/*activity system, Juver, 2017/11/05 */
	if ( m_fCharTitleDelay < CHARACTER_BADGE_TIME )
		m_fCharTitleDelay += fElapsedTime;

	/*charinfoview , Juver, 2017/11/11 */
	if ( m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
		m_fReqCharInfoTimer += fElapsedTime;
	
	/*inventory sort, Juver, 2018/07/07 */
	if ( m_fInventorySortTimer < REQ_INVENTORY_SORT_DELAY )
		m_fInventorySortTimer += fElapsedTime;

	/*item random option rebuild, Juver, 2018/07/04 */
	if ( RPARAM::item_random_option_rebuild_use_delay )
	{
		if ( m_item_random_option_rebuild_timer < RPARAM::item_random_option_rebuild_delay_time )
			m_item_random_option_rebuild_timer += fElapsedTime;
	}

	if ( m_fNPCContributionShopDelay < NPC_CONTRIBUTION_SHOP_DELAY )
		m_fNPCContributionShopDelay += fElapsedTime;

	if ( m_fMaxRVPreviewTimer < MAX_RV_PREVIEW_COOLTIME )
		m_fMaxRVPreviewTimer += fElapsedTime;

	///*hide costume, EJCode, 2018/11/19 */
	//if ( RPARAM::hide_costume_use )
	//{
	//	if ( m_hide_costume_timer < RPARAM::hide_costume_delay_time )
	//		m_hide_costume_timer += fElapsedTime;
	//}

	/*extreme auto tab, Juver, 2018/09/10 */
	if ( m_skill_change_weapon_tab_pending )
	{
		m_skill_change_weapon_tab_pending_time -= fElapsedTime;
		if ( m_skill_change_weapon_tab_pending_time <= 0.0f )
		{
			m_skill_change_weapon_tab_pending = FALSE;

			if ( m_skill_change_weapon_tab_pending_skill.skill_id != NATIVEID_NULL() )
			{
				NETSENDTOFIELD ( &m_skill_change_weapon_tab_pending_skill );
				m_skill_change_weapon_tab_pending_skill = GLMSG::SNETPC_REQ_SKILL();
				m_skill_change_weapon_tab_pending_skill.skill_id = NATIVEID_NULL();
			}
		}
	}

	/* 
	//disable last msg move 
	//scenario move to use skill visual bug
	//causing character still send msg move to last location pos target 
	if ( !IsSTATE(EM_ACT_CONTINUEMOVE) )
	{
		if ( memcmp(&m_sLastMsgMove,&m_sLastMsgMoveSend, sizeof(GLMSG::SNETPC_GOTO) ) )
		{
			m_fLastMsgMoveSend = fTime;
			m_sLastMsgMoveSend = m_sLastMsgMove;

			GLMSG::SNETPC_GOTO sMSG = m_sLastMsgMoveSend;
			NETSENDTOFIELD ( &sMSG );
		}
	}
	*/

	/*vehicle booster system, Juver, 2017/08/11 */
	if ( m_bVehicle )
	{
		if ( m_bBoosterCharge  )
		{
			if ( m_fBoosterTimer < GLCONST_CHAR::fVehicleBoosterChargeTime )
				m_fBoosterTimer += fElapsedTime;
		}

		if ( !m_bBoosterCharge && m_bBoosterStart )
		{
			if ( m_fBoosterTimer > 0.0f )
				m_fBoosterTimer -= fElapsedTime;
		}
	}

	/*product item, Juver, 2017/10/18 */
	if ( m_bItemCompoundTask )
	{
		m_fItemCompoundTaskTimer += fElapsedTime;

		if ( m_fItemCompoundTaskTimer >= m_fItemCompoundTaskTime )
		{
			m_bItemCompoundTask = FALSE;
			m_fItemCompoundTaskTimer = 0.0f;
			m_fItemCompoundTaskTime = 0.0f;
		}
	}

	/*pvp capture the flag, Juver, 2018/02/06 */
	if ( m_bCaptureTheFlagHoldFlag )
	{
		if ( m_fCaptureTheFlagHoldFlagTimer >= 0.0f )
			m_fCaptureTheFlagHoldFlagTimer -= fElapsedTime;
	}

	/*12-16-24, daily login - CNDev*/
	if ( m_bDailyLoginRemain )
	{
		if ( m_fDailyLoginRemain > 0.0f )
			m_fDailyLoginRemain -= fElapsedTime;
		else
			m_bDailyLoginRemain = false;
	}


	//	Note : 클럽마크 버전 확인.
	//
	ReqClubMarkInfo ( m_sCLUB.m_dwID, m_sCLUB.m_dwMarkVER );

	//	트레이드 상태 갱신.
	GLTradeClient::GetInstance().FrameMove ( fTime, fElapsedTime );

	//	Note : 타겟이 무효화 되는지 점검한다.
	//
	GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( m_sTargetID );
	if ( !pTarget )
	{
		m_sTargetID.dwID = EMTARGET_NULL;
		if ( IsACTION(GLAT_ATTACK) )	TurnAction ( GLAT_IDLE );
	}

	//	Note : Gate 검사.
	//
	DWORD dwDetectGate = DetectGate ();
	if ( dwDetectGate!=UINT_MAX )	CInnerInterface::GetInstance().SetGateOpen ( TRUE );

	// Note : 헤어를 Edit 해야 할 경우만 변경한다.
	if( m_bEnableHairSytle )	HairStyleUpdate();
	if( m_bEnableHairColor )	HairColorUpdate();
	if( m_bEnableFaceStyle )	FaceStyleUpdate();

	/*bike color , Juver, 2017/11/16 */
	if( m_bEnableBikeColor )	BikeColorUpdate();

	/*item color, Juver, 2018/01/10 */
	if( m_bEnableItemColor )	ItemColorUpdate();

	/* car, cart color, Juver, 2018/02/14 */
	if( m_bEnableCarColor )		CarColorUpdate();


	/*skill transform, Juver, 2018/09/09 */
	skill_transform_change_skin();

	//	Note : 에니메이션, 스킨 업데이트.
	//
	UpateAnimation ( fTime, fElapsedTime );

	//	Note : 행위별 처리.
	//
	switch ( m_Action )
	{
	case GLAT_IDLE:
		{
			if ( m_fIdleTime > 20.0f )
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				BOOL bPeaceZone = pLand ? pLand->IsPeaceZone() : FALSE;

				if ( !bPeaceZone && !IsSTATE(EM_ACT_PEACEMODE) )
				{
					ReqTogglePeaceMode();
				}

				/*away gesture, Juver, 2018/08/23 */
			/*	if ( m_fIdleTime > GLCONST_CHAR::fBECOME_AWAY_TIME && !m_sPMarket.IsOpen() )
				{
					if ( !IsACTION(GLAT_TALK) )
					{
						do_away_gesture();
					}
				} */ // Disabled - EZ Ran 4 12 2025 - JX
			}
		}
		break;

	case GLAT_TALK:
		break;

	case GLAT_MOVE:
		{
			//	Note : 케릭의 이동 업데이트.
			//
			m_actorMove.SetMaxSpeed ( GetMoveVelo () );
			hr = m_actorMove.Update ( fElapsedTime );
			if ( FAILED(hr) )	return E_FAIL;

			if ( !m_actorMove.PathIsActive() )
			{
				m_actorMove.Stop ();
				TurnAction ( GLAT_IDLE );
			}

			//	Note : 케릭의 현제 위치 업데이트.
			//
			m_vPos = m_actorMove.Position();

			//	Note : 케릭의 현제 방향 업데이트.
			//
			D3DXVECTOR3 vMovement = m_actorMove.NextPosition();
			if ( vMovement.x != FLT_MAX && vMovement.y != FLT_MAX && vMovement.z != FLT_MAX )
			{
				D3DXVECTOR3 vDirection = vMovement - m_vPos;
				if ( !DxIsMinVector(vDirection,0.2f) )
				{
					D3DXVec3Normalize ( &vDirection, &vDirection );
					m_vDir = vDirection;
				}
			}
		}
		break;

	case GLAT_ATTACK:
		{
			AttackProc ( fElapsedTime );

			//	Note : 케릭의 현재 방향 업데이트.
			//
			GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( m_sTargetID );
			if ( pTarget )
			{
				D3DXVECTOR3 vDirection = pTarget->GetPosition() - m_vPos;
				D3DXVec3Normalize ( &vDirection, &vDirection );
				m_vDir = vDirection;
			}
		}
		break;

	case GLAT_SKILL:
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL.wMainID, m_idACTIVESKILL.wSubID );
			if ( !pSkill )
			{
				TurnAction(GLAT_IDLE);
				break;
			}

			if ( !SkillProc ( fElapsedTime ) )	TurnAction(GLAT_IDLE);
		
			//	Note : 공격 방향으로 회전.
			//
			/*multi skill Juver, 2018/09/04 */
			m_vDir = UpdateSkillDirection ( m_vPos, m_vDir, m_idACTIVESKILL, m_vTARGET_POS_MAIN, m_sTARGET_IDS_MAIN );
		}
		break;

	case GLAT_SHOCK:
		break;

	case GLAT_PUSHPULL:
	case GLAT_SKILL_PUSH_PULL:		/* push skill logic, Juver, 2020/12/14 */	/* pull skill logic, Juver, 2020/12/14 */
	case GLAT_SKILL_DASH:			/*dash skill logic, Juver, 2017/06/17 */
		{
			m_actorMove.Update ( fElapsedTime );
			if ( !m_actorMove.PathIsActive() )
			{
				m_actorMove.Stop ();
				TurnAction ( GLAT_IDLE );
			}
		}
		break;

	case GLAT_FALLING:
		break;

	case GLAT_DIE:
		break;
	case GLAT_GATHERING:
		break;
	};

	/* skill position shift, Juver, 2020/12/15 */
	if ( m_bSkillPositionShift )
	{
		m_actorMove.Update ( fElapsedTime );
		if ( !m_actorMove.PathIsActive() )
		{
			m_actorMove.Stop ();
			m_bSkillPositionShift = false;
		}
	}

	if ( m_sCONFTING.IsCONFRONTING() )
	{
		if ( !m_sCONFTING.IsFIGHTING() )
		{
			DWORD dwCOUNT = m_sCONFTING.UPDATE(fElapsedTime);
			if ( EMCONFT_COUNT!=dwCOUNT )
			{
				CInnerInterface::GetInstance().SetFightBegin ( (int)(dwCOUNT+1) );
			}
		}
	}

	//	Note : 스킬 이펙트 업데이트.
	//
	FACTEFF::UpdateSkillEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS );

	UpdateSpecialSkill();



	//	?Item 지속 화면 효과. on/off
	if ( m_emOldQuestionType!=m_sQITEMFACT.emType )
	{
		m_emOldQuestionType = m_sQITEMFACT.emType;

		switch ( m_sQITEMFACT.emType )
		{
		case QUESTION_NONE:
			{
				//	?Item 화면 효과.
				DxEffProjMan::GetInstance().EnableGetItemEFF ( FALSE );

				//	?Item BGM.
				PLANDMANCLIENT pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
				if ( !pLandClient )		break;

				DxLandMan* pLandMan = pLandClient->GetLandMan();
				if ( !pLandMan )		break;

				const CString &strBgm = pLandMan->GetBgmFile();
				const char *szCurBgm = DxBgmSound::GetInstance().GetFile();
				if ( strBgm==szCurBgm )	break;

				DxBgmSound::GetInstance().ForceStop ();
				DxBgmSound::GetInstance().ClearFile ();

				DxBgmSound::GetInstance().SetFile ( strBgm );
				DxBgmSound::GetInstance().Play();
			}
			break;

		case QUESTION_SPEED_UP:
		case QUESTION_CRAZY:
		case QUESTION_ATTACK_UP:
		case QUESTION_EXP_UP:
		case QUESTION_LUCKY:
		case QUESTION_SPEED_UP_M:
		case QUESTION_MADNESS:
		case QUESTION_ATTACK_UP_M:
			{
				if ( ROPTION::bBuff )
					DxEffProjMan::GetInstance().EnableGetItemEFF ( TRUE );

				const std::string &strBgm = GLCONST_CHAR::strQITEM_BGM[m_sQITEMFACT.emType];
				if ( strBgm.empty() || strBgm=="null" )		break;

				const char *szCurBgm = DxBgmSound::GetInstance().GetFile();
				if ( strBgm==szCurBgm )	break;

				DxBgmSound::GetInstance().ForceStop ();
				DxBgmSound::GetInstance().ClearFile ();

				DxBgmSound::GetInstance().SetFile ( strBgm.c_str() );
				DxBgmSound::GetInstance().Play();
			}
			break;
		};
	}

	//	Note : 케릭의 현제 위치 업데이트.
	//
	m_vPos = m_actorMove.Position();

	//	Note : 현재 위치와 방향으로 Transform 메트릭스 계산.
	//
	D3DXMATRIX matTrans, matYRot;
	D3DXMatrixTranslation ( &matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
	float fThetaY = DXGetThetaYFromDirection ( m_vDir, m_vDirOrig );
	D3DXMatrixRotationY ( &matYRot, fThetaY );

	//D3DXMATRIX matScale;
	//D3DXMatrixScaling ( &matScale, 2, 2, 2 );

	m_matTrans = matYRot * matTrans;

	m_vMax = m_vPos + m_vMaxOrg;
	m_vMin = m_vPos + m_vMinOrg;

//	if( m_sQITEMFACT.IsACTIVE() ) m_sQITEMFACT.fTime -= fElapsedTime;

	//	Note : 현제 행동 상태 확인용.
	//
	EMANI_MAINTYPE emMAIN = AN_NONE;
	EMANI_SUBTYPE emSUB = AN_SUB_NONE;

	if ( m_pSkinChar )
	{
		emMAIN = m_pSkinChar->GETCURMTYPE();
		emSUB = m_pSkinChar->GETCURSTYPE();
	}
	
	if ( emMAIN < AN_TYPE_SIZE && emSUB < AN_SUB_00_SIZE )
	{
		CDebugSet::ToView ( 5, COMMENT::ANI_MAINTYPE_CHAR[m_pSkinChar->GETCURMTYPE()].c_str() );
		CDebugSet::ToView ( 6, COMMENT::ANI_SUBTYPE_WEAPON[m_pSkinChar->GETCURSTYPE()].c_str() );
		CDebugSet::ToView ( 7, COMMENT::ANI_SUBTYPE_SKILL[m_pSkinChar->GETCURSTYPE()].c_str() );
	}

	CDebugSet::ToView ( 8, COMMENT::ACTIONTYPE[m_Action].c_str() );

	CDebugSet::ToView ( 9, "char pos : %4.1f, %4.1f, %4.1f", m_vPos.x, m_vPos.y, m_vPos.z );

	/*dmk14 pk safetime*/
	//m_fMoveDelay += fElapsedTime;
	// 추적 유저일 경우에 정보들을 남긴다.
#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined ( HK_PARAM ) // ***Tracing Log print
	if( m_bTracingUser )
	{
		PrintTracingUserWindowInfo();
		PrintTracingUserProcessInfo( fElapsedTime );
	}
#endif 

	if ( m_fPetLastUseTime < PET_LAST_USE_TIME_DEFAULT ) 
		m_fPetLastUseTime += fElapsedTime;

	if ( m_fPetBlockTime < PET_BLOCK_TIME_DEFAULT ) 
		m_fPetBlockTime += fElapsedTime;

	if ( m_fCurrencyShopDelayTimer < RPARAM::fCurrencyShopPurchaseDelayTime )
		m_fCurrencyShopDelayTimer += fElapsedTime;

	/* party finder, Juver, 2020/01/03 */
	if ( m_fPartyFinderSearchDelayTimer < RPARAM::fPartyFinderSearchDelay )
		m_fPartyFinderSearchDelayTimer += fElapsedTime;

	if ( m_fPartyFinderJoinDelayTimer < RPARAM::fPartyFinderJoinDelay )
		m_fPartyFinderJoinDelayTimer += fElapsedTime;

	/* death hide, Juver, 2020/02/02 */
	DxGlobalStage::GetInstance().GetGameStage()->SetCharacterDead( IsDie() );


	/* tyranny AFK disconnect, Juver, 2021/08/07 */
	PLANDMANCLIENT pLandClient =  GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLandClient )
	{
		if ( pLandClient->m_bPVPTyrannyMap && RPARAM::bTyrannyAFKDC )
		{
			if ( m_fTyrannyAFKTimer >= RPARAM::fTyrannyAFKDCTime )
			{
				GLMSG::SNETPC_TYRANNY_AFK_DISCONNECT netMSG;
				netMSG.fTime = m_fTyrannyAFKTimer;
				NETSENDTOFIELD( &netMSG );

				m_fTyrannyAFKTimer = 0.0f;
			}
		}

		if ( pLandClient->m_bPVPCaptureTheFlagMap && RPARAM::bCaptureTheFlagAFKDC )
		{
			if ( m_fCaptureTheFlagAFKTimer >= RPARAM::fCaptureTheFlagAFKDCTime )
			{
				GLMSG::SNETPC_CAPTURE_THE_FLAG_AFK_DISCONNECT netMSG;
				netMSG.fTime = m_fCaptureTheFlagAFKTimer;
				NETSENDTOFIELD( &netMSG );

				m_fCaptureTheFlagAFKTimer = 0.0f;
			}
		}
	}

	if ( m_pAutoPilot )
		m_pAutoPilot->FrameMove(fElapsedTime);

	//does not do anything
	//just update timer for ui
	if ( m_bCaptchaWait )
	{
		if ( m_fCaptchaWaitTimer > 0.0f )
			m_fCaptchaWaitTimer -= fElapsedTime;
		
	}

	return S_OK;
}

HRESULT GLCharacter::FrameMoveForTool( float fTime, float fElapsedTime )
{
	HRESULT hr=S_OK;	

	//	Note : 에니메이션, 스킨 업데이트.
	//
//	UpateAnimation ( fTime, fElapsedTime );

	
	if ( !m_pSkinChar->ISENDANIM() )			
		m_pSkinChar->FrameMove ( fTime, fElapsedTime );

	//	Note : 스킬 이펙트 업데이트.
	//

	m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	//	Note : 현재 위치와 방향으로 Transform 메트릭스 계산.
	//
	D3DXMATRIX matTrans, matYRot;
	D3DXMatrixTranslation ( &matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
	float fThetaY = DXGetThetaYFromDirection ( m_vDir, m_vDirOrig );
	D3DXMatrixRotationY ( &matYRot, fThetaY );

	//D3DXMATRIX matScale;
	//D3DXMatrixScaling ( &matScale, 2, 2, 2 );

	m_matTrans = matYRot * matTrans;

	m_vMax = m_vPos + m_vMaxOrg;
	m_vMin = m_vPos + m_vMinOrg;
	return S_OK;
}

void GLCharacter::PrintTracingUserProcessInfo( float fElapsedTime )
{


		// 5분마다 한번씩
	if( m_fPrintProcessInfoMsgGap == -10.0f || m_fPrintProcessInfoMsgGap > 600 )
	{
		m_fPrintProcessInfoMsgGap = 0.0f;

		// 프로세스 스냅샷 핸들을 얻는다.
		HANDLE h_snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

		// 스냅샷 되었을때 프로세스 주소내에 열거된 리스트를 저장하는 구조체
		PROCESSENTRY32 entry_data;
		entry_data.dwSize = sizeof(entry_data);

		// 첫번째 프로세스의 정보를 얻는다.
		char continue_flag = Process32First(h_snap_shot, &entry_data);         
		while(continue_flag)
		{
			char temp[256] = {0, };

			char name[MAX_PATH];
			unsigned int id;
			unsigned int thread_counts;
			unsigned int parent_id;
			unsigned int priority;
			PROCESS_MEMORY_COUNTERS pmc;            

			// 프로세스 아이디를 이용하여 해당 프로세스 핸들 값을 얻는다.
			HANDLE hwnd = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,entry_data.th32ProcessID);

			if(hwnd != NULL){
				// 정상적으로 프로세스를 열었다면..
				// 프로세스의 경로명을 얻는다.
				// 정상적으로 경로명을 얻었다면 해당 경로명을 그렇지 못했다면 파일명만을 출력한다.
				if(GetModuleFileNameEx(hwnd, NULL, temp, 256)) 	strcpy(name, temp);
				else strcpy(name, entry_data.szExeFile);				
				GetProcessMemoryInfo( hwnd, &pmc, sizeof(pmc));
				CloseHandle(hwnd);
			} else {
				// 정상적으로 프로세스를 열지 못했다면..
				strcpy(name, entry_data.szExeFile);

			}
			id = entry_data.th32ProcessID;
			thread_counts = entry_data.cntThreads;
			parent_id = entry_data.th32ParentProcessID;
			priority = entry_data.pcPriClassBase;
			

			NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
			TracingMsg.nUserNum  = GetUserID();
			StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );
			CString strTemp;
			int memory = pmc.WorkingSetSize / 1024;
			strTemp.Format( "ProcessInfo, ProcessName[%s], Memory[%dKB], Thread count[%d]",
							 name, memory, thread_counts );

			StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );
			NETSEND ( &TracingMsg );

	
			// 다음 프로세스의 정보를 얻는다.

			continue_flag = Process32Next(h_snap_shot, &entry_data);

		}

		CloseHandle(h_snap_shot);
	}else{
		m_fPrintProcessInfoMsgGap += fElapsedTime;
	}
}

void GLCharacter::PrintTracingUserWindowInfo()
{
	BYTE dwID[] = {INVENTORY_WINDOW, CHARACTER_WINDOW, SKILL_WINDOW, PARTY_WINDOW, QUEST_WINDOW, CLUB_WINDOW, FRIEND_WINDOW, LARGEMAP_WINDOW};
	BYTE i;
	for( i = 0; i < 8; i++ )
	{
		m_bNewVisibleTracingUI[i] = CInnerInterface::GetInstance().IsVisibleGroup( (UIGUID)dwID[i] )? true:false;
		if( m_bOldVisibleTracingUI[i] != m_bNewVisibleTracingUI[i] )
		{
			m_bOldVisibleTracingUI[i] = m_bNewVisibleTracingUI[i];
			if( m_bNewVisibleTracingUI[i] == TRUE )
			{
				NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
				TracingMsg.nUserNum  = GetUserID();
				StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );
				CString strTemp;

				switch (i)
				{
				case 0:
					strTemp.Format( "Open Inventory Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 1:
					strTemp.Format( "Open Character Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 2:
					strTemp.Format( "Open Skill Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 3:
					strTemp.Format( "Open Party Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 4:
					strTemp.Format( "Open Quest Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 5:
					strTemp.Format( "Open Club Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 6:
					strTemp.Format( "Open Friend Window, [%d][%s]", GetUserID(), m_szName );
					break;
				case 7:
					strTemp.Format( "Open Mini Map, [%d][%s]", GetUserID(), m_szName );
					break;
				}
				StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

				NETSEND ( &TracingMsg );
			}
		}
	}
}

float GLCharacter::GetDirection ()
{
	return DXGetThetaYFromDirection ( m_vDir, m_vDirOrig );
}

HRESULT GLCharacter::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( !IsACTION( GLAT_MOVE ) )
	{
		if ( ROPTION::bClickEffect )
			DxEffExtra::GetInstance().RemoveClickEffect( GLCONST_CHAR::strCLICK_EFFECT.c_str() );
	}

	if ( IsSTATE(EM_REQ_VISIBLENONE) )	return S_FALSE;

	//	Note : m_bINVISIBLE 가 참일 경우 반투명하게 표현해야함.
	//
	bool bHALF_VISIBLE = m_bSkillTurnInvisible || IsSTATE(EM_REQ_VISIBLEOFF);
	if ( bHALF_VISIBLE )
	{
		DxEffcharDataMan::GetInstance().PutPassiveEffect ( m_pSkinChar, GLCONST_CHAR::strHALFALPHA_EFFECT.c_str(), &m_vDir );
	}
	else
	{
		DxEffcharDataMan::GetInstance().OutEffect ( m_pSkinChar, GLCONST_CHAR::strHALFALPHA_EFFECT.c_str() );
	}

	if ( m_pSkinChar )
	{
		/*vehicle system, Juver, 2017/08/07 */
		if ( m_bVehicle )
		{
			NavigationMesh* pNavi = GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh();
			if ( pNavi )
			{
				BOOL bCoA(FALSE);
				DWORD dwCoA(0);
				D3DXVECTOR3 vCoA(0,0,0);

				//current position
				pNavi->IsCollision ( D3DXVECTOR3(m_vPos.x,m_vPos.y+5.0f,m_vPos.z), D3DXVECTOR3(m_vPos.x,m_vPos.y-5.0f,m_vPos.z), vCoA, dwCoA, bCoA );
				if ( bCoA )
				{
					D3DXVECTOR3 vPosFront = m_vPos + m_vDir * 2.0f;
					D3DXVECTOR3 vDirFront = ( vPosFront - m_vPos );
					vDirFront.y = 0.0f;
					D3DXVec3Normalize (  &vDirFront, &vDirFront );

					D3DXVECTOR3 vPosBack = m_vPos - m_vDir * 2.0f;
					D3DXVECTOR3 vDirBack = ( vPosBack - m_vPos );
					vDirBack.y = 0.0f;
					D3DXVec3Normalize (  &vDirBack, &vDirBack );

					D3DXVECTOR3 vPosB = vPosFront + vDirFront * 2.0f;
					D3DXVECTOR3 vPosC = vPosBack + vDirBack * 2.0f;

					BOOL bCoB(FALSE);
					DWORD dwCoB(0);
					D3DXVECTOR3 vCoB(0,0,0);

					BOOL bCoC(FALSE);
					DWORD dwCoC(0);
					D3DXVECTOR3 vCoC(0,0,0);

					pNavi->IsCollision ( D3DXVECTOR3(vPosB.x,vPosB.y+5.0f,vPosB.z), D3DXVECTOR3(vPosB.x,vPosB.y-5.0f,vPosB.z), vCoB, dwCoB, bCoB );
					if ( !bCoB )
					{
						float fNewdxLength = 1.0f;
						while ( 0.0f < fNewdxLength )
						{
							vPosB = vPosFront + vDirFront * fNewdxLength;
							pNavi->IsCollision  ( D3DXVECTOR3(vPosB.x,vPosB.y+5.0f,vPosB.z), D3DXVECTOR3(vPosB.x,vPosB.y-5.0f,vPosB.z), vCoB, dwCoB, bCoB );
							if ( bCoB )		break;
							fNewdxLength -= 0.2f;
						};
					}

					pNavi->IsCollision ( D3DXVECTOR3(vPosC.x,vPosC.y+5.0f,vPosC.z), D3DXVECTOR3(vPosC.x,vPosC.y-5.0f,vPosC.z), vCoC, dwCoC, bCoC );
					if ( !bCoC )
					{
						float fNewdxLength = 1.0f;
						while ( 0.0f < fNewdxLength )
						{
							vPosC = vPosBack + vDirBack * fNewdxLength;
							pNavi->IsCollision  ( D3DXVECTOR3(vPosC.x,vPosC.y+5.0f,vPosC.z), D3DXVECTOR3(vPosC.x,vPosC.y-5.0f,vPosC.z), vCoC, dwCoC, bCoC );
							if ( bCoC )		break;
							fNewdxLength -= 0.2f;
						};
					}

					if ( bCoB && bCoC )
					{
						if ( pNavi->LineOfSightTest ( dwCoA, vCoA, dwCoB, vCoB ) && pNavi->LineOfSightTest ( dwCoA, vCoA, dwCoC, vCoC ) )
						{
							D3DXVECTOR3 vDIR = vCoB - vCoC;
							float fLength = D3DXVec3Length ( &vDIR );
							if ( fLength == 0 )		fLength = 0.001f;
							float fThetaX = (float) asin ( vDIR.y / fLength );
							D3DXMATRIX matX;
							D3DXMatrixRotationX ( &matX, fThetaX );
							m_matTrans = matX * m_matTrans;
						}
					}
				}
			}
		}

		/*change scale card, Juver, 2018/01/03 */
		//if ( m_bChangeCharScale )
		//	m_pSkinChar->SetScaleAdjust( m_fActiveCharScale );
		//else
		//	m_pSkinChar->SetScaleAdjust( m_fScaleRange );

		/*vehicle system, Juver, 2017/08/06 */
		if ( m_pSkinChar->m_pVehicle )
			m_pSkinChar->RenderVehicle( pd3dDevice, m_matTrans );
		else
			m_pSkinChar->Render( pd3dDevice, m_matTrans );
	}

	#ifdef _SYNC_TEST
	{
		
		EDITMESHS::RENDERSPHERE( pd3dDevice, m_actorMove.GetTargetPosition(), 2.f );
		EDITMESHS::RENDERSPHERE( pd3dDevice, m_actorMove.Position(), 2.f );
		EDITMESHS::RENDERSPHERE( pd3dDevice, m_vPos, 2.f );
		EDITMESHS::RENDERSPHERE( pd3dDevice, m_vServerPos, 2.0f );
	}
	#endif

	//D3DXVECTOR3 vMax(m_vPos.x+MAX_VIEWRANGE, m_vPos.y+20, m_vPos.z+MAX_VIEWRANGE);
	//D3DXVECTOR3 vMin(m_vPos.x-MAX_VIEWRANGE, m_vPos.y-20, m_vPos.z-MAX_VIEWRANGE);
	//EDITMESHS::RENDERAABB ( pd3dDevice,  vMax, vMin );

	if ( dxincommand::bDISP_CONSOLE )
	{
#ifdef _DEBUG
		CDebugSet::ToView( 20, "STATE_STUN %p %d %d", &m_bSTATE_STUN_ORIG, m_bSTATE_STUN_ORIG, m_cSTATE_STUN_CHECK.GetInt() );
		CDebugSet::ToView( 21, "SUM_PIERCE %p %d %d", &m_nSUM_PIERCE_ORIG, m_nSUM_PIERCE_ORIG, m_cSUM_PIERCE_CHECK.GetInt() );
		CDebugSet::ToView( 22, "SUM_TARGET_RANGE %p %g %g", &m_fSUM_TARGET_RANGE_ORIG, m_fSUM_TARGET_RANGE_ORIG, m_cSUM_TARGET_RANGE_CHECK.GetFloat() );
		CDebugSet::ToView( 23, "SUM_ATTACK_RANGE %p %g %g", &m_fSUM_ATTACK_RANGE_ORIG, m_fSUM_ATTACK_RANGE_ORIG, m_cSUM_ATTACK_RANGE_CHECK.GetFloat() );
		CDebugSet::ToView( 24, "SUM_APPLY_RANGE %p %g %g", &m_fSUM_APPLY_RANGE_ORIG, m_fSUM_APPLY_RANGE_ORIG, m_cSUM_APPLY_RANGE_CHECK.GetFloat() );


#endif // _DEBUG

	}





	

	return S_OK;
}

HRESULT GLCharacter::RenderShadow ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	
	if ( IsSTATE(EM_REQ_VISIBLENONE) )	return S_FALSE;
	if ( IsSTATE(EM_REQ_VISIBLEOFF) )	return S_FALSE;

	//	Note : 그림자 랜더링.
	//
	if ( m_pSkinChar )
	{
		DxShadowMap::GetInstance().RenderShadowCharMob ( m_pSkinChar, m_matTrans, pd3dDevice );
		DxSoftShadowMap::GetInstance().RenderShadowCharMob ( m_pSkinChar, m_matTrans, pd3dDevice );
	}

	return S_OK;
}

HRESULT GLCharacter::RenderReflect( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( IsSTATE(EM_REQ_VISIBLENONE) )	return S_FALSE;
	if ( IsSTATE(EM_REQ_VISIBLEOFF) )	return S_FALSE;

	//	Note : 반사 랜더링.
	//
	if ( m_pSkinChar )
	{
		DxEnvironment::GetInstance().RenderRefelctChar( m_pSkinChar, m_matTrans, pd3dDevice );
	}

	return S_OK;
}

HRESULT GLCharacter::InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( m_pSkinChar )
	{
		m_pSkinChar->InitDeviceObjects( pd3dDevice );
	}

	return S_OK;
}

HRESULT GLCharacter::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( m_pSkinChar )
	{
		m_pSkinChar->RestoreDeviceObjects ( pd3dDevice );
	}

	return S_OK;
}

HRESULT GLCharacter::InvalidateDeviceObjects ()
{
	if ( m_pSkinChar )
	{
		m_pSkinChar->InvalidateDeviceObjects ();
	}

	return S_OK;
}

HRESULT GLCharacter::DeleteDeviceObjects ()
{
	if ( m_pSkinChar )
	{
		m_pSkinChar->DeleteDeviceObjects ();
	}

	return S_OK;
}

EMELEMENT GLCharacter::GET_ITEM_ELMT ()
{
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	SITEM* pRHandItem = GET_SLOT_ITEMDATA(emRHand);
	SITEM* pLHandItem = GET_SLOT_ITEMDATA(emLHand);

	EMELEMENT emELEMENT(EMELEMENT_SPIRIT);
	if ( pRHandItem && pRHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		emELEMENT = STATE_TO_ELEMENT ( pRHandItem->sSuitOp.sBLOW.emTYPE );
	}
	else if ( pLHandItem && pLHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		emELEMENT = STATE_TO_ELEMENT ( pLHandItem->sSuitOp.sBLOW.emTYPE );
	}

	return emELEMENT;
}

void GLCharacter::StartAttackProc ()
{
	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLAND && pLAND->IsPeaceZone() )	return;


	if ( IsSTATE(EM_ACT_PEACEMODE) )
	{
		ReqTogglePeaceMode ();
	}

	//DoPASSIVITY ( TRUE );

	m_nattSTEP = 0;
	m_fattTIMER = 0.0f;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	/*skill transform, Juver, 2018/09/09 */
	EMANI_SUBTYPE emANISUBTYPE = CHECK_ATTACK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );
	DWORD dwAMOUNT = m_pSkinChar->GETANIAMOUNT ( AN_ATTACK, emANISUBTYPE );

	m_dwANISUBSELECT = m_dwANISUBCOUNT++;
	if ( m_dwANISUBCOUNT >= dwAMOUNT )		m_dwANISUBCOUNT = 0;

	BOOL bCONFT = ISCONFRONT_TAR ( m_sTargetID );

	//	Note : 공격 메시지 발생.
	//
	GLMSG::SNETPC_ATTACK NetMsg;
	NetMsg.emTarCrow = m_sTargetID.emCrow;
	NetMsg.dwTarID = m_sTargetID.dwID;
	NetMsg.dwAniSel = m_dwANISUBSELECT;
	NetMsg.dwFlags = NULL;

	NETSENDTOFIELD ( &NetMsg );

	m_bAttackable = FALSE;
}

void GLCharacter::AttackEffect ( const SANIMSTRIKE &sStrikeEff )
{
	BOOL bOk = GLGaeaClient::GetInstance().ValidCheckTarget ( m_sTargetID );
	if ( !bOk )	return;

	GLCOPY* pActor = GLGaeaClient::GetInstance().GetCopyActor ( m_sTargetID );
	if ( pActor )	pActor->ReceiveSwing ();

	/*ABL system, Juver, 2017/06/02 */
	if ( m_pSkinChar->m_pAttBone[sStrikeEff.m_emPiece] )	
		m_pSkinChar->m_pAttBone[sStrikeEff.m_emPiece]->SELECTANI( AN_ABL_ATTACK, AN_SUB_NONE );

	//	타겟의 위치.
	D3DXVECTOR3 vTARPOS = GLGaeaClient::GetInstance().GetTargetPos ( m_sTargetID );

	D3DXVECTOR3 vDir = vTARPOS - m_vPos;
	vDir.y = 0.f;
	D3DXVec3Normalize ( &vDir, &vDir );

	vTARPOS.y += 15.0f;

	D3DXVECTOR3 vTarDir = vTARPOS - m_vPos;
	float fLength = D3DXVec3Length ( &vTarDir );

	D3DXVec3Normalize ( &vTarDir, &vTarDir );
	D3DXVECTOR3 vTARPAR = m_vPos + vTarDir*fLength * 10.0f;

	//	타격 위치를 알아냄.
	/*ABL system, Juver, 2017/06/02 */
	D3DXVECTOR3 vOUT(0.0f,0.0f,0.0f);
	bOk = STRIKE_SELECT::SelectStrike( m_pSkinChar, vTARPAR, vOUT, sStrikeEff );
	if ( !bOk )	return;

	EMSLOT emSlot = PIECE_2_SLOT(sStrikeEff.m_emPiece);
	if( emSlot==SLOT_TSIZE )		return;
	if( emSlot==SLOT_RHAND )		emSlot = GetCurRHand();
	else if( emSlot==SLOT_LHAND )	emSlot = GetCurLHand();

	D3DXMATRIX matEffect;
	D3DXMatrixTranslation ( &matEffect, vOUT.x, vOUT.y, vOUT.z );

	STARGETID sTargetID = m_sTargetID;
	sTargetID.vPos = vTARPOS;

	//	Note : 무기의 속성 살펴봄.
	//
	EMELEMENT emELEMENT = GET_ITEM_ELMT ();

	if ( VALID_SLOT_ITEM(emSlot) )
	{
		SITEM *pItem = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emSlot).sNativeID);
		if ( pItem )
		{
			if ( pItem->sSuitOp.emAttack <= ITEMATT_NEAR )
			{
				std::string strEFFECT = pItem->GetTargetEffect();
				if ( strEFFECT.empty() )	strEFFECT = GLCONST_CHAR::GETSTRIKE_EFFECT(emELEMENT);

				DxEffGroupPlayer::GetInstance().NewEffGroup
				(
					strEFFECT.c_str(),
					matEffect,
					&sTargetID
				);

				DxEffGroupPlayer::GetInstance().NewEffBody ( GLCONST_CHAR::strAMBIENT_EFFECT.c_str(), &sTargetID, &vDir );
			}
			else
			{
				if ( pItem->sSuitOp.emAttack == ITEMATT_BOW )
				{
					EMSLOT emLHand = GetCurLHand();

					SITEM *pLHAND = NULL;
					if ( VALID_SLOT_ITEM(emLHand) )	pLHAND = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);
					if ( pLHAND && pLHAND->sBasicOp.emItemType == ITEM_ARROW )
					{
						DxEffSingleGroup* pEffSingleG = DxEffGroupPlayer::GetInstance().NewEffGroup
						(
							pLHAND->GetTargetEffect(),
							matEffect,
							&sTargetID
						);
						
						if ( pEffSingleG )
						{
							pEffSingleG->AddEffAfter ( EFFASINGLE, GLCONST_CHAR::GETSTRIKE_EFFECT(emELEMENT)  );
							pEffSingleG->AddEffAfter ( EFFABODY, std::string(pLHAND->GetTargBodyEffect()) );
							pEffSingleG->AddEffAfter ( EFFABODY, GLCONST_CHAR::strAMBIENT_EFFECT );
						}
					}
				}
				/*gun-bullet logic, Juver, 2017/05/27 */
				else if ( pItem->sSuitOp.emAttack == ITEMATT_GUN ||
					pItem->sSuitOp.emAttack == ITEMATT_RAILGUN ||
					pItem->sSuitOp.emAttack == ITEMATT_PORTALGUN )
				{
					EMSLOT emLHand = GetCurLHand();

					SITEM *pLHAND = NULL;
					if ( VALID_SLOT_ITEM(emLHand) )	pLHAND = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);
					if ( pLHAND && pLHAND->sBasicOp.emItemType == ITEM_BULLET )
					{
						DxEffSingleGroup* pEffSingleG = DxEffGroupPlayer::GetInstance().NewEffGroup
							(
							pLHAND->GetTargetEffect(),
							matEffect,
							&sTargetID
							);

						if ( pEffSingleG )
						{
							pEffSingleG->AddEffAfter ( EFFASINGLE, GLCONST_CHAR::GETSTRIKE_EFFECT(emELEMENT)  );
							pEffSingleG->AddEffAfter ( EFFABODY, std::string(pLHAND->GetTargBodyEffect()) );
							pEffSingleG->AddEffAfter ( EFFABODY, GLCONST_CHAR::strAMBIENT_EFFECT );
						}
					}
				}
			}
		}
	}
	else
	{
		//	보이지 않는 타갯일 경우 타격 이팩트는 생략됨.
		DxEffGroupPlayer::GetInstance().NewEffGroup
		(
			GLCONST_CHAR::GETSTRIKE_EFFECT(emELEMENT),
			matEffect,
			&sTargetID
		);

		DxEffGroupPlayer::GetInstance().NewEffBody ( GLCONST_CHAR::strAMBIENT_EFFECT.c_str(), &sTargetID, &vDir );
	}
}

BOOL GLCharacter::AttackProc ( float fElapsedTime )
{
	m_fattTIMER += fElapsedTime * m_fSTATE_MOVE;
	GASSERT(m_pSkinChar->GETCURANIMNODE());

	PANIMCONTNODE pAnicontNode = m_pSkinChar->GETCURANIMNODE();
	GASSERT(pAnicontNode);
	PSANIMCONTAINER pAnimCont = pAnicontNode->pAnimCont;

	if ( pAnimCont->m_wStrikeCount == 0 )	return FALSE;

	DWORD dwThisKey = DWORD(m_fattTIMER*UNITANIKEY_PERSEC);

	DWORD dwStrikeKey = pAnimCont->m_sStrikeEff[m_nattSTEP].m_dwFrame;
	if ( m_nattSTEP < pAnimCont->m_wStrikeCount )
	{
		if ( dwThisKey>=dwStrikeKey )
		{
			if ( GLGaeaClient::GetInstance().IsVisibleCV(m_sTargetID) )
			{
				//	Note : 공격시 이팩트 발휘..
				//
				AttackEffect ( pAnimCont->m_sStrikeEff[m_nattSTEP] );
			}

			m_nattSTEP++;
		}
	}

	return TRUE;
}

// *****************************************************
// Desc: 엑션을 취할 수 있는 대상인지 check
// *****************************************************
BOOL GLCharacter::IsReActionable ( const STARGETID &sTargetID, BOOL bENEMY )
{
	GLCOPY *pTARGET = GLGaeaClient::GetInstance().GetCopyActor(sTargetID);

	if ( !pTARGET )		return false;

	EMCROW emCROW = pTARGET->GetCrow();

	BOOL breaction(true);
	if ( bENEMY )
	{
		switch ( emCROW )
		{
		case CROW_PC:
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );

				/*pvp capture the flag, Juver, 2018/01/31 */
				BOOL bCaptureTheFlag = FALSE;
				if ( pLand )
					bCaptureTheFlag = pLand->m_bPVPCaptureTheFlagMap;

				/* open pk function, Juver, 2019/12/24 */
				bool bOpenPK = false;
				if ( pLand && pLand->IsOpenPK() )
				{
					bOpenPK = true;
				}

				SDROP_CHAR &sCHARDATA_TAR = pCHAR->GetCharData();

				if ( IsSafeZone() || pTARGET->IsSafeZone() ) return false;
				/* SafeTime Logic - Montage, 3-28-25 */
				if ( pCHAR && pCHAR->IsSafeTime() && GLCONST_CHAR::bSCHOOL_FREE_PK_Z_FORCED ) return true;
				if ( pCHAR && pCHAR->IsSafeTime() ) return false;

				if ( pLand && pLand->IsPKMap() && pLand->IsPKZone() ) return true;
				//	학교간 프리 PK.
				if ( !GLCONST_CHAR::bSCHOOL_FREE_PK_Z_FORCED ) 
				{
					bool bSCHOOL_FREEPK = GLGaeaClient::GetInstance().IsSchoolFreePk ();

					/*pvp capture the flag, Juver, 2018/01/31 */
					if ( bSCHOOL_FREEPK && m_wSchool!=sCHARDATA_TAR.wSchool && !bCaptureTheFlag )
					{
						return true;
					}
				}

				{
					/*pvp capture the flag, Juver, 2018/01/31 */
					bool bBRIGHTEVENT = GLGaeaClient::GetInstance().IsBRIGHTEVENT();
					if( bBRIGHTEVENT && !bCaptureTheFlag )
					{
						return true;
					}

					/* open pk function, Juver, 2019/12/25 */
					if( bOpenPK && !bCaptureTheFlag )
					{
						return true;
					}
				}

				if ( pLand && pLand->IsClubBattleZone() )
				{
					if ( m_sCLUB.IsBattle( pCHAR->GETCLUBID() ) ) return true;
					if ( m_sCLUB.IsBattleAlliance( pCHAR->GETALLIANCEID() ) ) return true;
				}


				//	대련시 공격 가능.
				breaction = ISCONFRONT_TAR ( sTargetID );
				
				//	pk 일 경우 공격 가능.
				if ( !breaction )	breaction = IS_PLAYHOSTILE ( pTARGET->GetCharID() );

				if ( !breaction )
				{
					// 선도전시 동맹클럽 맴버는 공격 불가.
					if ( pLand )
					{
						if ( pLand->m_bClubBattle || ( pLand->m_bPVPWoeMap && RPARAM::bWoeGVG == 1 ) )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
							BOOL bAlliance = m_sCLUB.IsAllianceGuild ( pCLIENT->GETCLUBID() );

							if ( bAlliance )
								breaction = false;
							else
								breaction = true;
						}
						else if ( pLand->m_bClubDeathMatch )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
							if ( m_sCLUB.m_dwID == pCLIENT->GETCLUBID() )	breaction = false;
							else breaction = true;
						}
						/*pvp tyranny, Juver, 2017/08/24 */
						else if ( pLand->m_bPVPTyrannyMap )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );

							if ( RPARAM::bCTFGVG )
							{
								if ( m_sCLUB.m_dwID == pCLIENT->GETCLUBID() )	
									breaction = false;
								else 
									breaction = true;
							}
							else
							{
								if ( m_wSchool == pCLIENT->GetCharData().wSchool )	
									breaction = false;
								else 
									breaction = true;
							}
						}
						/*school wars, Juver, 2018/01/19 */
						else if ( pLand->m_bPVPSchoolWarsMap )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
							if ( m_wSchool == pCLIENT->GetCharData().wSchool )	breaction = false;
							else breaction = true;
						}
						/*pvp capture the flag, Juver, 2018/01/31 */
						else if ( pLand->m_bPVPCaptureTheFlagMap )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
							if ( pCLIENT ){
								if ( m_wCaptureTheFlagTeam == pCLIENT->GetCharData().m_wCaptureTheFlagTeam )	
									breaction = false;
								else 
									breaction = true;
							}
						}
						/* pvp club death match, Juver, 2020/11/26 */
						else if ( pLand->m_bPVPClubDeathMatchLobbyMap || pLand->m_bPVPClubDeathMatchBattleMap )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
							if ( m_sCLUB.m_dwID == pCLIENT->GETCLUBID() )	breaction = false;
							else breaction = true;
						}

						else if ( pLand->m_bPVPPBGLobbyMap || pLand->m_bPVPPBGBattleMap )
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
							if ( GLPartyClient::GetInstance().GetPartyID() == pCLIENT->GetCharData().dwParty )	
								breaction = false;
							else 
								breaction = true;
						}
						
						/*woe Arc Development 08-06-2024*/
						else if (pLand->m_bPVPWoeMap && RPARAM::bWoeGVG == 0)
						{
							PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar(sTargetID.dwID);
							if (m_wSchool == pCLIENT->GetCharData().wSchool)	breaction = false;
							else breaction = true;
						}
					}
				}
			}

			break;

		case CROW_MOB:
			{
				if ( m_sCONFTING.IsCONFRONTING() ) breaction = false;
				else if ( IsSafeZone() ) breaction = false;
				else breaction = true;

				/*pvp tyranny, Juver, 2017/08/26 */
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				/*if ( pLand && pLand->m_bPVPTyrannyMap )
				{
					PGLCROWCLIENT pCrowClient = pLand->GetCrow( sTargetID.dwID );
					if ( pCrowClient && 
						pCrowClient->m_wTyrannyOwner != TYRANNY_SCHOOL_SIZE &&
						pCrowClient->m_wTyrannyOwner == m_wSchool )
					{
						breaction = false;
					}
				}*/

				/*crow school, Juver, 2018/08/02 */
				if ( pLand )
				{
					PGLCROWCLIENT pCrowClient = pLand->GetCrow( sTargetID.dwID );
					if ( pCrowClient && pCrowClient->m_pCrowData && 
						pCrowClient->m_pCrowData->m_sBasic.m_emCrowSchool != EMCROW_SCHOOL_SIZE &&
						pCrowClient->m_pCrowData->m_sBasic.m_emCrowSchool == m_wSchool )
					{
						breaction = false;
					}
				}
			}break;

			/*skill summon, Juver, 2017/10/08 */
		case CROW_SUMMON:
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();

				/*pvp capture the flag, Juver, 2018/01/31 */
				BOOL bCaptureTheFlag = FALSE;
				if ( pLand )
					bCaptureTheFlag = pLand->m_bPVPCaptureTheFlagMap;

				/* open pk function, Juver, 2019/12/24 */
				bool bOpenPK = false;
				if ( pLand && pLand->IsOpenPK() )
				{
					bOpenPK = true;
				}

				PGLANYSUMMON psummon_any = GLGaeaClient::GetInstance().GetSummon ( sTargetID.dwID );
				if ( psummon_any && psummon_any->IsTargetReady() )
				{
					PGLCHARCLIENT pchar_client = GLGaeaClient::GetInstance().GetChar ( psummon_any->m_dwOwner );
					if ( pchar_client )
					{
						if ( IsSafeZone() || pchar_client->IsSafeZone() ) return false;
	
						if ( !GLCONST_CHAR::bSCHOOL_FREE_PK_Z_FORCED ) 
						{
							/*pvp capture the flag, Juver, 2018/01/31 */
							if ( GLGaeaClient::GetInstance().IsSchoolFreePk () && m_wSchool!=pchar_client->GetCharData().wSchool && !bCaptureTheFlag )
								return true;
						}

						{
							/*pvp capture the flag, Juver, 2018/01/31 */
							bool bBRIGHTEVENT = GLGaeaClient::GetInstance().IsBRIGHTEVENT();
							if( bBRIGHTEVENT && !bCaptureTheFlag )	return true;

							/* open pk function, Juver, 2019/12/25 */
							if( bOpenPK && !bCaptureTheFlag )	return true;
						}

						PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();

						if ( pLand && pLand->IsClubBattleZone() )
						{
							if ( m_sCLUB.IsBattle( pchar_client->GETCLUBID() ) )				return true;
							if ( m_sCLUB.IsBattleAlliance( pchar_client->GETALLIANCEID() ) )	return true;
						}


						//	대련시 공격 가능.
						STARGETID starget_id_owner;
						starget_id_owner.dwID = pchar_client->GetCtrlID();
						starget_id_owner.emCrow = pchar_client->GetCrow();

						breaction = ISCONFRONT_TAR ( starget_id_owner );

						//	pk 일 경우 공격 가능.
						if ( !breaction )	breaction = IS_PLAYHOSTILE ( pchar_client->GetCharID() );

						if ( !breaction )
						{
							// 선도전시 동맹클럽 맴버는 공격 불가.
							if ( pLand )
							{
								if ( pLand->m_bClubBattle || ( pLand->m_bPVPWoeMap && RPARAM::bWoeGVG == 1 ))
								{
									BOOL bAlliance = m_sCLUB.IsAllianceGuild ( pchar_client->GETCLUBID() );

									if ( bAlliance )
										breaction = false;
									else
										breaction = true;
								}
								else if ( pLand->m_bClubDeathMatch )
								{
									if ( m_sCLUB.m_dwID == pchar_client->GETCLUBID() )	breaction = false;
									else breaction = true;
								}
								/*pvp tyranny, Juver, 2017/08/24 */
								else if ( pLand->m_bPVPTyrannyMap )
								{
									if ( RPARAM::bCTFGVG )
									{
										if ( m_sCLUB.m_dwID == pchar_client->GETCLUBID() )
											breaction = false;
										else 
											breaction = true;
									}
									else
									{
										if ( m_wSchool == pchar_client->GetCharData().wSchool )	
											breaction = false;
										else 
											breaction = true;
									}
								}
								/*school wars, Juver, 2018/01/19 */
								else if ( pLand->m_bPVPSchoolWarsMap )
								{
									if ( m_wSchool == pchar_client->GetCharData().wSchool )	breaction = false;
									else breaction = true;
								}
								/*pvp capture the flag, Juver, 2018/01/31 */
								else if ( pLand->m_bPVPCaptureTheFlagMap )
								{
									if ( m_wCaptureTheFlagTeam == pchar_client->GetCharData().m_wCaptureTheFlagTeam )	
										breaction = false;
									else 
										breaction = true;
								}
								/* pvp club death match, Juver, 2020/11/26 */
								else if ( pLand->m_bPVPClubDeathMatchLobbyMap|| pLand->m_bPVPClubDeathMatchBattleMap )
								{
									if ( m_sCLUB.m_dwID == pchar_client->GETCLUBID() )	breaction = false;
									else breaction = true;
								}
								else if ( pLand->m_bPVPPBGLobbyMap|| pLand->m_bPVPPBGBattleMap )
								{
									if ( GLPartyClient::GetInstance().GetPartyID() == pchar_client->GetCharData().dwParty )
										breaction = false;
									else 
										breaction = true;
								}
								
								/*woe Arc Development 08-06-2024*/
								else if (pLand->m_bPVPWoeMap && RPARAM::bWoeGVG == 0)
								{
									if (m_wSchool == pchar_client->GetCharData().wSchool)	breaction = false;
									else breaction = true;
								}
							}
						}
					}
				}
			}break;

		case CROW_NPC:
			breaction = false;
			break;

			/* crow zone, Juver, 2018/02/21 */
		case CROW_ZONE_NAME:
		case CROW_GATE_NAME:
			breaction = false;
			break;

		case CROW_MATERIAL:
			breaction = false;
			break;

		//case CROW_PET:	break;	// PetData

		default:
			breaction = false;
			break;
		};
	}
	else
	{
		switch ( emCROW )
		{
		case CROW_PC:
			{
				//	pc 에게 비공격 마법 같은 것은 대련자가 아닐 때만 가능. ( 일단 클라이언트는 검사 안함. )
				breaction = !IS_PLAYHOSTILE ( pTARGET->GetCharID() );

				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				if ( pLand && ( pLand->m_bPVPWoeMap && RPARAM::bWoeGVG == 1 ))
				{
					if ( pLand->m_bClubBattle || pLand->m_bClubDeathMatch || pLand->IsFreePK() )
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar(sTargetID.dwID);
						breaction = (pCLIENT->GETCLUBID()==m_dwGuild);
					}

					/*pvp tyranny, Juver, 2017/08/24 */
					if ( pLand->m_bPVPTyrannyMap )
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
						breaction = ( pCLIENT && m_wSchool == pCLIENT->GetCharData().wSchool );
					}

					/*school wars, Juver, 2018/01/19 */
					if ( pLand->m_bPVPSchoolWarsMap )
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
						breaction = ( pCLIENT && m_wSchool == pCLIENT->GetCharData().wSchool );
					}

					/*pvp capture the flag, Juver, 2018/01/31 */
					if ( pLand->m_bPVPCaptureTheFlagMap )
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar( sTargetID.dwID );
						breaction = ( pCLIENT && m_wCaptureTheFlagTeam == pCLIENT->GetCharData().m_wCaptureTheFlagTeam );
					}

					/* pvp club death match, Juver, 2020/11/26 */
					if ( pLand->m_bPVPClubDeathMatchLobbyMap || pLand->m_bPVPClubDeathMatchBattleMap )
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar(sTargetID.dwID);
						breaction = (pCLIENT->GETCLUBID()==m_dwGuild);
					}

					if ( pLand->m_bPVPPBGLobbyMap|| pLand->m_bPVPPBGBattleMap )
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar(sTargetID.dwID);
						breaction = (pCLIENT->GetCharData().dwParty==GLPartyClient::GetInstance().GetPartyID());
					}
					
					/*woe Arc Development 08-06-2024*/
					if (pLand && pLand->m_bPVPWoeMap && RPARAM::bWoeGVG == 0)
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar(sTargetID.dwID);
						breaction = (pCLIENT && m_wSchool == pCLIENT->GetCharData().wSchool);
					}
				}
				
			}
			break;
		
		case CROW_MOB:
			breaction = false;
			break;

		case CROW_NPC:
			breaction = m_sCONFTING.IsCONFRONTING() ? false : true;
			break;

			/* crow zone, Juver, 2018/02/21 */
		case CROW_ZONE_NAME:
		case CROW_GATE_NAME:
			breaction = false;
			break;

		case CROW_MATERIAL:
			breaction = true;
			break;

		//case CROW_PET:	break;	// PetData

		default:
			breaction = false;
			break;
		};
	}

	return breaction;
}

BOOL GLCharacter::ISCONFRONT_TAR ( const STARGETID &sTargetID )
{
	if ( sTargetID.emCrow!=CROW_PC )	return FALSE;

	if ( m_sCONFTING.IsFIGHTING() )
	{
		switch ( m_sCONFTING.emTYPE )
		{
		case EMCONFT_ONE:
			return ( sTargetID.dwID==m_sCONFTING.dwTAR_ID );
			break;

		case EMCONFT_PARTY:
			{
				SCONFTING_CLT::CONFT_MEM_ITER iter = m_sCONFTING.setConftMember.find(sTargetID.dwID);
				return ( iter!= m_sCONFTING.setConftMember.end() );
			}
			break;

		case EMCONFT_GUILD:
			{
				PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );
				if ( !pCHAR )						return FALSE;

				SCONFTING_CLT::CONFT_MEM_ITER iter = m_sCONFTING.setConftMember.find(pCHAR->GetCharData().dwCharID);
				return ( iter!= m_sCONFTING.setConftMember.end() );
			}
			break;
		};
	}

	return FALSE;
}

void GLCharacter::DELCONFRONT_MEMBER ( DWORD dwID )
{
	switch ( m_sCONFTING.emTYPE )
	{
	case EMCONFT_ONE:
		break;

	case EMCONFT_PARTY:
		{
			//	Note : 자기 파티원일 경우.
			GLPartyClient::GetInstance().ResetConfrontMember ( dwID );

			//	Note : 상대방 파티원일 경우.
			m_sCONFTING.DEL_CONFT_MEMBER ( dwID );
		}
		break;

	case EMCONFT_GUILD:
		{
			//	Note : 자기 클럽원일 경우.
			m_sCLUB.DELCONFT_MEMBER ( dwID );

			//	Note : 상대방 클럽원일 경우.
			m_sCONFTING.DEL_CONFT_MEMBER ( dwID );
		}
		break;
	};

	return;
}

void GLCharacter::NewConftBoundEffect ( const EMCONFT_TYPE emCONFT )
{
	//	Note : 이팩트 설정.
	//
	DxEffGroupPlayer::GetInstance().DeleteAllEff ( GLCONST_CHAR::strCONFRONT_BOUND.c_str() );

	D3DXMATRIX matTrans, matRotY;
	D3DXVECTOR3 vDist;
	D3DXMatrixIdentity ( &matTrans );

	for ( float fRotY=0.0f; fRotY<(D3DX_PI*2.0f); fRotY += (D3DX_PI*2.0f/24.0f) )
	{
		vDist = D3DXVECTOR3(1,0,0);
		D3DXMatrixRotationY(&matRotY,fRotY);
		D3DXVec3TransformCoord(&vDist,&vDist,&matRotY);

		float fCONFT_DIST(GLCONST_CHAR::fCONFRONT_ONE_DIST);
		switch ( emCONFT )
		{
		case EMCONFT_ONE:
			fCONFT_DIST = GLCONST_CHAR::fCONFRONT_ONE_DIST;
			break;
		case EMCONFT_PARTY:
			fCONFT_DIST = GLCONST_CHAR::fCONFRONT_PY_DIST;
			break;

		case EMCONFT_GUILD:
			fCONFT_DIST = GLCONST_CHAR::fCONFRONT_CLB_DIST;
			break;
		};

		vDist = m_sCONFTING.vPosition + vDist * fCONFT_DIST;

		BOOL bCollision(FALSE);
		D3DXVECTOR3 vCollision;
		DWORD dwCollisionID;

		NavigationMesh* pNaviMesh = GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh();
		pNaviMesh->IsCollision(vDist+D3DXVECTOR3(0,10000.0f,0),vDist+D3DXVECTOR3(0,-10000.0f,0),vCollision,dwCollisionID,bCollision);

		matTrans._41 = vDist.x;
		matTrans._42 = vCollision.y;
		matTrans._43 = vDist.z;

		DxEffGroupPlayer::GetInstance().NewEffGroup ( GLCONST_CHAR::strCONFRONT_BOUND.c_str(), matTrans );
	}
}

void GLCharacter::DelConftBoundEffect ()
{
	DxEffGroupPlayer::GetInstance().DeleteAllEff ( GLCONST_CHAR::strCONFRONT_BOUND.c_str() );
}

BOOL GLCharacter::IsVaildTradeInvenSpace ()
{
	GLInventory &sTradeBoxTar = GLTradeClient::GetInstance().GetTarTradeBox();

	GLInventory sInvenTemp;
	sInvenTemp.SetAddLine ( m_cInventory.GETAddLine(), true );
	sInvenTemp.Assign ( m_cInventory );

	//	Note : 자신의 교환 목록에 올라간 아이템을 인밴에서 제외.
	//
	GLInventory &sTradeBoxMy = GLTradeClient::GetInstance().GetMyTradeBox();
	GLInventory::CELL_MAP* pItemListMy = sTradeBoxMy.GetItemList();

	GLInventory::CELL_MAP_ITER iter = pItemListMy->begin();
	GLInventory::CELL_MAP_ITER iter_end = pItemListMy->end();
	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pTradeItem = (*iter).second;

		BOOL bOk = sInvenTemp.DeleteItem ( pTradeItem->wBackX, pTradeItem->wBackY );
		if ( !bOk )		return FALSE;
	}

	//	Note : 상대방의 교환 물품이 들어 갈수 있는지 검사.
	//
	GLInventory::CELL_MAP* pItemListTar = sTradeBoxTar.GetItemList();

	iter = pItemListTar->begin();
	iter_end = pItemListTar->end();
	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pTradeItem = (*iter).second;

		SITEM *pItem = GLItemMan::GetInstance().GetItem ( pTradeItem->sItemCustom.sNativeID );
		if ( !pItem )	return FALSE;

		WORD wPosX = 0, wPosY = 0;
		BOOL bOk = sInvenTemp.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )		return FALSE;

		bOk = sInvenTemp.InsertItem ( pTradeItem->sItemCustom, wPosX, wPosY );
		if ( !bOk )		return FALSE;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------------------
//								H	a	i	r				S	t	y	l	e	 &&	 C	o	l	o	r
//----------------------------------------------------------------------------------------------------------------------------------
void GLCharacter::HairStyleInitData()
{
	m_bEnableHairSytle = TRUE;
	
	m_wHairStyleTEMP = m_wHair;

	// 컬러 변화 중일 경우 건들지 않는다.
	if( !m_bEnableHairColor )
	{
		m_wHairColorTEMP = m_wHairColor;
	}
}

void GLCharacter::HairColorInitData()
{
	m_bEnableHairColor = TRUE;

	m_wHairColorTEMP = m_wHairColor;

	// 스타일 변화 중일 경우 건들지 않는다.
	if( !m_bEnableHairSytle )
	{
		m_wHairStyleTEMP = m_wHair;
	}
}

void GLCharacter::HairStyleChange( WORD wStyle )
{
	m_wHairStyleTEMP = wStyle; 

	HairStyleUpdate();
}

void GLCharacter::HairColorChange( WORD wColor )
{ 
	m_wHairColorTEMP = wColor; 

	HairColorUpdate();
}

void GLCharacter::HairStyleUpdate()
{
	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().FindData ( GLCONST_CHAR::szCharSkin[emIndex] );
	if ( !pSkinChar )	return;

	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	// 헤어스타일.
	if ( sCONST.dwHAIRNUM > m_wHairStyleTEMP )
	{
		std::string strHAIR_CPS = sCONST.strHAIR_CPS[m_wHairStyleTEMP];
	
		PDXCHARPART pCharPart = NULL;
		pCharPart = m_pSkinChar->GetPiece(PIECE_HAIR);		//	현재 장착 스킨.

		if( pCharPart && strcmp(strHAIR_CPS.c_str(),pCharPart->m_szFileName) )
		{
			m_pSkinChar->SetPiece( strHAIR_CPS.c_str(), m_pd3dDevice, NULL, NULL, TRUE );
		}
	}

	// Note : 이것을 호출하면서.. 머리형이 원상태로 바뀌어 버린다.
	//		그래서 FALSE 호출로 머리모양을 안바뀌게 한다.
	UpdateSuit( FALSE, TRUE );
}

void GLCharacter::HairColorUpdate()
{
	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().FindData ( GLCONST_CHAR::szCharSkin[emIndex] );
	if ( !pSkinChar )	return;

	m_pSkinChar->SetHairColor( m_wHairColorTEMP );
}

void GLCharacter::FaceStyleInitData()
{
	m_bEnableFaceStyle = TRUE;
	
	m_wFaceStyleTEMP = m_wFace;
}

void GLCharacter::FaceStyleChange( WORD wStyle )
{
	m_wFaceStyleTEMP = wStyle; 

	FaceStyleUpdate();
}

void GLCharacter::FaceStyleUpdate()
{
	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().FindData ( GLCONST_CHAR::szCharSkin[emIndex] );
	if ( !pSkinChar )	return;

	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	// 얼굴
	if ( sCONST.dwHEADNUM > m_wFaceStyleTEMP )
	{
		std::string strHEAD_CPS = sCONST.strHEAD_CPS[m_wFaceStyleTEMP];
	
		PDXCHARPART pCharPart = NULL;
		pCharPart = m_pSkinChar->GetPiece(PIECE_HEAD);		//	현재 장착 스킨.

		if ( pCharPart && strcmp(strHEAD_CPS.c_str(),pCharPart->m_szFileName) )
		{
			m_pSkinChar->SetPiece ( strHEAD_CPS.c_str(), m_pd3dDevice, 0x0, 0, TRUE );
		}
	}

	// Note : 이것을 호출하면서.. 머리형이 원상태로 바뀌어 버린다.
	//		그래서 FALSE 호출로 머리모양을 안바뀌게 한다.
	UpdateSuit( TRUE, FALSE );
}

BOOL GLCharacter::IsMarketClick( DWORD dwGaeaID )
{
	for( int i = 0; i < ( int ) m_vecMarketClick.size(); i++ )
	{
		if( m_vecMarketClick[ i ] == dwGaeaID )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void GLCharacter::InsertMarketClick( DWORD dwGaeaID )
{
	m_vecMarketClick.push_back( dwGaeaID );
}

void GLCharacter::InitAllSkillFact ()
{
	for ( int i=0; i<EMBLOW_MULTI; ++i )		DISABLEBLOW ( i );
	for ( int i=0; i<SKILLFACT_SIZE; ++i )		DISABLESKEFF ( i );

	m_dwTransfromSkill = SNATIVEID::ID_NULL;

	FACTEFF::DeleteEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS );
}

D3DXVECTOR3 GLCharacter::GetPosBodyHeight ()
{ 
	float fHeight = m_fHeight * m_fScaleRange;

	/*change scale card, Juver, 2018/01/03 */
	if ( m_bChangeCharScale )
		fHeight = m_fHeight * m_fActiveCharScale;

	/*vehicle system, Juver, 2017/08/09 */
	if ( m_bVehicle )
	{
		int emType = m_sVehicle.m_emTYPE;
		if ( emType == VEHICLE_TYPE_BOARD )	fHeight += 10.0f;
	}

	return D3DXVECTOR3( m_vPos.x, m_vPos.y+fHeight, m_vPos.z ); 
}

void GLCharacter::GetSkinAABB( D3DXVECTOR3& vMin, D3DXVECTOR3& vMax )
{ 
	if( m_pSkinChar )
		m_pSkinChar->GetAABBBox( vMax, vMin );
}

float GLCharacter::GetSkinScale()
{ 
	if( !m_pSkinChar )	return 1.0f;
	return m_pSkinChar->GetScale();
}

float GLCharacter::GetSkinHeight()
{ 
	if( !m_pSkinChar )	return 20.0f;
	return m_pSkinChar->m_fHeight;
}

LONGLONG GLCharacter::GetCalcTaxiCharge( int nSelectMap, int nSelectStop )
{
	GLTaxiStation& sTaxiStation = GLTaxiStation::GetInstance();

	STAXI_MAP* pTaxiMap = sTaxiStation.GetTaxiMap( nSelectMap );
	if ( !pTaxiMap )
	{
		GASSERT ( 0 && "pTaxiMap를 찾을수 없습니다." );
		return 0;
	}

	STAXI_STATION* pSTATION = pTaxiMap->GetStation( nSelectStop );
	if ( !pSTATION )
	{
		GASSERT ( 0 && "STATION을 찾을 수 없습니다." );
		return 0;
	}

	DWORD dwCurMapID = GLGaeaClient::GetInstance().GetActiveMapID().dwID;
	LONGLONG dwCharge = sTaxiStation.GetBasicCharge();

	if ( pSTATION->dwMAPID != dwCurMapID ) dwCharge += pSTATION->dwMapCharge;

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLand )
	{
		volatile float fSHOP_RATE = GetBuyRate();
		volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;
		dwCharge = LONGLONG ( (float)dwCharge * fSHOP_RATE_C );
	}

	return dwCharge;
}

bool GLCharacter::IsCoolTime( SNATIVEID sNativeID )
{
	__time64_t tCurTime = GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
	__time64_t tCoolTime = GetMaxCoolTime( sNativeID );

	if ( tCurTime < tCoolTime ) return true;

	return false;
}

bool GLCharacter::CheckCoolTime( SNATIVEID sNativeID )
{

	if ( IsCoolTime ( sNativeID ) )
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
		if ( !pItem )	return true;

		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("ITEM_COOLTIME"), pItem->GetName() );
		return true;
	}

	return false;
}

__time64_t	GLCharacter::GetMaxCoolTime ( SNATIVEID sNativeID )
{
	//	아이템 정보 가져옴.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( !pItem )			return 0;

	ITEM_COOLTIME* pCoolTimeType = GetCoolTime( (DWORD) pItem->sBasicOp.emItemType, EMCOOL_ITEMTYPE );
	ITEM_COOLTIME* pCoolTimeID = GetCoolTime( pItem->sBasicOp.sNativeID.dwID, EMCOOL_ITEMID );

	__time64_t tCoolType = 0;
	__time64_t tCoolID = 0;

	if ( pCoolTimeType )	tCoolType = pCoolTimeType->tCoolTime;
	if ( pCoolTimeID )		tCoolID = pCoolTimeID->tCoolTime;

	return (tCoolType > tCoolID) ? tCoolType : tCoolID;
}

ITEM_COOLTIME*	GLCharacter::GetCoolTime ( SNATIVEID sNativeID )
{
	//	아이템 정보 가져옴.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( !pItem )			return 0;

	ITEM_COOLTIME* pCoolTimeType = GetCoolTime( (DWORD) pItem->sBasicOp.emItemType, EMCOOL_ITEMTYPE );
	ITEM_COOLTIME* pCoolTimeID = GetCoolTime( pItem->sBasicOp.sNativeID.dwID, EMCOOL_ITEMID );

	__time64_t tCoolType = 0;
	__time64_t tCoolID = 0;

	if ( pCoolTimeType )	tCoolType = pCoolTimeType->tCoolTime;
	if ( pCoolTimeID )		tCoolID = pCoolTimeID->tCoolTime;

	return (tCoolType > tCoolID) ? pCoolTimeType : pCoolTimeID;
}



ITEM_COOLTIME*	GLCharacter::GetCoolTime( DWORD dwCoolID, EMCOOL_TYPE emCoolType )
{
	if ( emCoolType == EMCOOL_ITEMID )
	{
		COOLTIME_MAP_ITER pos = m_mapCoolTimeID.find(dwCoolID);
		if ( pos==m_mapCoolTimeID.end() )		return NULL;
		
		return &(*pos).second;
	}
	else if ( emCoolType == EMCOOL_ITEMTYPE )
	{
		COOLTIME_MAP_ITER pos = m_mapCoolTimeType.find(dwCoolID);
		if ( pos==m_mapCoolTimeType.end() )		return NULL;
		
		return &(*pos).second;
	}
	else	
		return NULL;
}

bool GLCharacter::SetCoolTime ( ITEM_COOLTIME& sCoolTime, EMCOOL_TYPE emCoolType )
{
	if ( emCoolType == EMCOOL_ITEMID )
	{
		m_mapCoolTimeID[sCoolTime.dwCoolID] = sCoolTime;
	}
	else if( emCoolType == EMCOOL_ITEMTYPE )
	{
		m_mapCoolTimeType[sCoolTime.dwCoolID] = sCoolTime;
	}

	return true;
}

const SITEMCUSTOM GLCharacter::GET_ITEM_MIX( int nIndex )
{
	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();

	if( !m_sItemMixPos[nIndex].VALID() )
		return sItemCustom;

	SINVENITEM* pResistItem = m_cInventory.GetItem( m_sItemMixPos[nIndex].wPosX, m_sItemMixPos[nIndex].wPosY );
	if( !pResistItem )
		return sItemCustom;

	sItemCustom = pResistItem->sItemCustom;

	return sItemCustom;
}

void GLCharacter::SetItemMixMoveItem( int nIndex )
{
	if ( nIndex >= ITEMMIX_ITEMNUM )
	{
		m_sPreInventoryItem.RESET();
		return;
	}

	if( m_sPreInventoryItem.VALID() )
	{
		for ( int i = 0; i < ITEMMIX_ITEMNUM; ++i )
		{
			if ( m_sItemMixPos[i].wPosX == m_sPreInventoryItem.wPosX &&
				m_sItemMixPos[i].wPosY == m_sPreInventoryItem.wPosY )
			{
				m_sPreInventoryItem.RESET();
				return;
			}

		}

		m_sItemMixPos[nIndex].SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
		
		m_sPreInventoryItem.RESET();
	}
	
	return;
}

void GLCharacter::ReSetItemMixItem( int nIndex )
{
	if ( nIndex >= ITEMMIX_ITEMNUM || m_sPreInventoryItem.VALID() )
	{
		m_sPreInventoryItem.RESET();
		return;
	}

	m_sItemMixPos[nIndex].RESET();
}

void GLCharacter::ResetItemMix()
{
	for ( int i = 0; i < ITEMMIX_ITEMNUM; ++i ) 
	{
		m_sItemMixPos[i].RESET();
	}
	m_sPreInventoryItem.RESET();
}

bool GLCharacter::ValidItemMixOpen()
{
	if ( CInnerInterface::GetInstance().IsVisibleGroup( ITEM_MIX_INVEN_WINDOW ) 
		|| CInnerInterface::GetInstance().IsVisibleGroup( ITEM_MIX_WINDOW ) )
		return true;
	return false;
}

DWORD GLCharacter::GetItemMixMoney()
{
	ITEM_MIX sItemMix;

	for( int i = 0; i < ITEMMIX_ITEMNUM; ++i ) 
	{
		sItemMix.sMeterialItem[i].sNID = GET_ITEM_MIX( i ).sNativeID;

		if( sItemMix.sMeterialItem[i].sNID != NATIVEID_NULL() )
			sItemMix.sMeterialItem[i].nNum = (BYTE)GET_ITEM_MIX( i ).wTurnNum;
	}

	GLItemMixMan::GetInstance().SortMeterialItem( sItemMix );

	const ITEM_MIX* pItemMix = GLItemMixMan::GetInstance().GetItemMix( sItemMix );
	if ( !pItemMix ) return UINT_MAX;

	return pItemMix->dwPrice;
}

bool GLCharacter::ValidWindowOpen()
{
	if ( ValidRebuildOpen() )					return true;	// ITEMREBUILD_MARK
	if ( ValidGarbageOpen() )					return true;	// 휴지통
	if ( ValidItemMixOpen() )					return true;	// 아이템 조합
	if ( IsOpenMiniGame() )						return true;	// 미니게임
	if ( GLTradeClient::GetInstance().Valid() )	return true;
	if ( ValidItemCompoundOpen() )				return true;	/*product item, Juver, 2017/10/17 */
	if ( ValidItemTransferOpen() )				return true;	/*item transfer card, Juver, 2018/01/18 */
	if ( ValidItemRandomOptionRebuildOpen() )	return true;	/*item random option rebuild, Juver, 2018/07/03 */
	if ( ValidChaosMachineOpen() )				return true;	/* chaos machine, Juver, 2021/07/09 */

	if ( isExchangeItemOpen() )	
		return true;

	return false;
}

void GLCharacter::UpdateTargetEffect()
{
	bool bCancel = true;

	if ( ROPTION::bTargetEffect )
	{
		if ( m_sACTARTarget.sTARID != STARGETID() )
		{
			GLCOPY* pActorTarget = GLGaeaClient::GetInstance().GetCopyActor ( m_sACTARTarget.sTARID );
			if ( pActorTarget )
			{
				STARGETID sTargetID ( m_sACTARTarget.sTARID.emCrow, m_sACTARTarget.sTARID.dwID, m_sACTARTarget.sTARID.vPos );
				D3DXMATRIX  matTrans, matScale, matWorld;
				D3DXVECTOR3 vMin, vMax;
				pActorTarget->GetSkinAABB( vMin, vMax );
				D3DXMatrixTranslation ( &matTrans, m_sACTARTarget.sTARID.vPos.x, m_sACTARTarget.sTARID.vPos.y, m_sACTARTarget.sTARID.vPos.z );
				D3DXMatrixScaling( &matScale, vMax.x/4.f, vMax.y/4.f, vMax.z/4.f );
				D3DXMatrixMultiply( &matWorld, &matScale, &matTrans );
				DxEffExtra::GetInstance().NewTargetEff ( GLCONST_CHAR::strTARGET_EFFECT.c_str(), matWorld, &sTargetID );

				D3DXVECTOR3	vTarPos = GLGaeaClient::GetInstance().GetTargetPos( m_sACTARTarget.sTARID );
				if ( vTarPos != VERROR() )
					m_sACTARTarget.sTARID.vPos = vTarPos;

				bCancel = false;
			}
		}
	}

	if ( bCancel )
	{
		DxEffExtra::GetInstance().RemoveTargetEffect( GLCONST_CHAR::strTARGET_EFFECT.c_str() );
		m_sACTARTarget = SACTIONTAR();
	}
}

BOOL GLCharacter::IsNpcQuest( DWORD dwGlobID )
{
	PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( dwGlobID );
	if ( !pCrow )		return FALSE;
	if ( !pCrow->IsValidBody() )		return FALSE;
	if ( !pCrow->IsHaveVisibleBody() )	return FALSE;
	if ( pCrow->m_pCrowData->m_sBasic.m_emCrow != CROW_NPC ) return FALSE;

	SNATIVEID sIDCROW = pCrow->m_pCrowData->m_sBasic.sNativeID;

	BOOL bFIND = FALSE;
	GLQuestPlay::MAPQUEST QuestProcMap = m_cQuestPlay.GetQuestProc();
	GLQuestPlay::MAPQUEST_ITER iterQuestProg = QuestProcMap.begin();
	for ( ; iterQuestProg != QuestProcMap.end (); ++iterQuestProg )
	{
		GLQUESTPROG* pQuestProg = (*iterQuestProg).second;
		if ( !pQuestProg ) continue;
		DWORD dwQuestID = pQuestProg->m_sNID.dwID;
		GLQUEST* pQuest = GLQuestMan::GetInstance().Find ( dwQuestID );
		if ( !pQuest ) continue;
		DWORD dwCurrentStep = pQuestProg->m_dwSTEP;
		GLQUEST_STEP* pQuestStep = pQuest->GetSTEP ( dwCurrentStep );
		if ( !pQuestStep )	continue;
		if ( !pQuestStep->IsNEED_NPCTALK() ) continue;
		if ( pQuestStep->m_dwNID_NPCTALK == sIDCROW.dwID )
		{
			//verify if able to talk to this npc
			//verify if quest id and quest step exist on this npc
			CNpcDialogueSet		m_DialogueSet;
			//client dont have dialogset data in m_pCrowdata so we need to load it
			BOOL bLoad = m_DialogueSet.Load ( pCrow->m_pCrowData->GetTalkFile() );
			if ( !bLoad )	continue;
			CNpcDialogueSet::DIALOGMAP* pDialogueMap = m_DialogueSet.GetDialogueMap();
			CNpcDialogueSet::DIALOGMAP_IT iterDialogMap = pDialogueMap->begin();
			for( ; iterDialogMap != pDialogueMap->end(); ++iterDialogMap )
			{
				CNpcDialogue* pNpcDialogue = (*iterDialogMap).second;
				if ( !pNpcDialogue )	continue;
				DWORD dwIndex = pNpcDialogue->FindConditionSatisfied (this);
				if ( dwIndex==UINT_MAX )	continue;
				CNpcDialogueCase* pCase = pNpcDialogue->GetDlgCase(dwIndex);	
				if ( !pCase )	continue;
				CNpcTalkControl* pNpcTalkControl = pCase->GetTalkControl ();
				if ( !pNpcTalkControl )	continue;
	
				CNpcTalkControl::NPCMAP* pMap = pNpcTalkControl->GetTalkMap ();
				CNpcTalkControl::NPCMAP_IT iterTalkControl = pMap->begin();
				for ( ; iterTalkControl != pMap->end(); ++iterTalkControl )
				{
					SNpcTalk* pNpcTalk = (*iterTalkControl).second;
					if ( !pNpcTalk )	continue;
					if ( !pNpcTalk->DoTEST(this) )	continue;
					if ( pNpcTalk->m_nACTION != SNpcTalk::EM_QUEST_STEP )	continue;
					if ( pNpcTalk->m_dwACTION_PARAM1 != dwQuestID )	continue;
					if ( pNpcTalk->m_dwACTION_PARAM2 != dwCurrentStep )	continue;

					bFIND = true;	
					break;		
				}		

				if ( bFIND )	
					break;
			}
		}

		if ( bFIND )	
			break;
	}

	return bFIND;
}

void GLCharacter::UpdateNPCQuestEffect()
{
	GLCROWCLIENTLIST *pCrowList = GLGaeaClient::GetInstance().GetActiveMap()->GetCrowList ();
	if ( !pCrowList )	return;
	GLCROWCLIENTNODE *pCrowCur = pCrowList->m_pHead;
	for ( ; pCrowCur; pCrowCur = pCrowCur->pNext )
	{
		PGLCROWCLIENT pCrow = pCrowCur->Data;
		if ( pCrow )
		{
			if ( !pCrow )		continue;
			if ( !pCrow->IsValidBody() )		continue;
			if ( !pCrow->IsHaveVisibleBody() )	continue;
			if ( pCrow->m_pCrowData->m_sBasic.m_emCrow != CROW_NPC ) continue;

			BOOL bHaveQuest = IsNpcQuest( pCrow->m_dwGlobID );
			if ( bHaveQuest )
			{
				STARGETID sTARID ( CROW_NPC, pCrow->m_dwGlobID, pCrow->GetPosition() );
				BOOL bExisting = DxEffExtra::GetInstance().FindPassiveEffect( GLCONST_CHAR::strCOMPLETE_NPC_EFFECT.c_str(), sTARID );
				if ( !bExisting ) 
				{
					D3DXMATRIX  matTrans;
					D3DXMatrixTranslation ( &matTrans, pCrow->GetPosition().x, pCrow->GetPosition().y, pCrow->GetPosition().z );
					matTrans._42 += pCrow->GetSkinHeight();
					DxEffExtra::GetInstance().PassiveEffect ( GLCONST_CHAR::strCOMPLETE_NPC_EFFECT.c_str(), matTrans, sTARID );
				}
			}
			else
			{
				STARGETID sTARID ( CROW_NPC, pCrow->m_dwGlobID, pCrow->GetPosition() );
				DxEffExtra::GetInstance().DeletePassiveEffect ( GLCONST_CHAR::strCOMPLETE_NPC_EFFECT.c_str(),sTARID );
			}
		}
	}
}

/*itemfood system, Juver, 2017/05/25 */
BOOL GLCharacter::ItemFoodAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT )
{
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )				return FALSE;
	if ( wLevel >= SKILL::MAX_SKILL_LEVEL )	return FALSE;
	if ( wSLOT >= FITEMFACT_SIZE )	return FALSE;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wLevel];

	BOOL bHOLD = FALSE;
	SFITEMFACT sSKILLEF;
	switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
	{
	case SKILL::EMFOR_VARHP:
	case SKILL::EMFOR_VARMP:
	case SKILL::EMFOR_VARSP:
	case SKILL::EMFOR_DEFENSE:
	case SKILL::EMFOR_HITRATE:
	case SKILL::EMFOR_AVOIDRATE:
	case SKILL::EMFOR_VARAP:
	case SKILL::EMFOR_VARDAMAGE:
	case SKILL::EMFOR_VARDEFENSE:
	case SKILL::EMFOR_PA:
	case SKILL::EMFOR_SA:
	case SKILL::EMFOR_MA:
	case SKILL::EMFOR_RESIST:
	case SKILL::EMFOR_SUMMONTIME: /*summon time, Juver, 2017/12/12 */
		bHOLD = TRUE;
		sSKILLEF.emTYPE = pSkill->m_sAPPLY.emBASIC_TYPE;
		sSKILLEF.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
	{
		if ( pSkill->m_sAPPLY.sImpacts[nImpact].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			sSKILLEF.sImpacts[nImpact].emADDON = pSkill->m_sAPPLY.sImpacts[nImpact].emADDON;
			sSKILLEF.sImpacts[nImpact].fADDON_VAR = pSkill->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[wLevel];
		}
	}

	for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
	{
		switch ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC )
		{
		case EMSPECA_REFDAMAGE:
		case EMSPECA_NONBLOW:
		case EMSPECA_PIERCE:
		case EMSPECA_TARRANGE:
		case EMSPECA_MOVEVELO:
		case EMSPECA_ATTACKVELO:
		case EMSPECA_SKILLDELAY:
		case EMSPECA_PSY_DAMAGE_REDUCE:
		case EMSPECA_MAGIC_DAMAGE_REDUCE:
		case EMSPECA_PSY_DAMAGE_REFLECTION:
		case EMSPECA_MAGIC_DAMAGE_REFLECTION:
		case EMSPECA_DEFENSE_SKILL_ACTIVE:
		case EMSPECA_CHANGE_ATTACK_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_CHANGE_APPLY_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_TALK_TO_NPC:			/*npc talk buff, Juver, 2017/10/03 */
		case EMSPECA_SPECIAL_ITEM_DROP:		/*special item drop buff, Juver, 2018/07/01 */
		case EMSPECA_ITEMDROP_RATE:			/*item drop rate buff, Juver, 2018/07/01 */
		case EMSPECA_MONEYDROP_RATE:		/*gold drop buff, Juver, 2018/07/01 */
		case EMSPECA_GETEXP_RATE:			/*exp rate buff, Juver, 2018/07/01 */
		case EMSPECA_AMPLIFY:				/* skill amplify, Juver, 2020/12/10 */
		case EMSPECA_STIGMA:				/* skill stigma, Juver, 2020/12/11 */
			bHOLD = TRUE;
			sSKILLEF.sSpecs[nSpec].emSPEC = pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC;
			sSKILLEF.sSpecs[nSpec].fSPECVAR1 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR1;
			sSKILLEF.sSpecs[nSpec].fSPECVAR2 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR2;
			sSKILLEF.sSpecs[nSpec].dwSPECFLAG = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwFLAG;
			sSKILLEF.sSpecs[nSpec].dwNativeID = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwNativeID;
			break;
		};
	}

	if ( bHOLD )
	{
		sSKILLEF.sNATIVEID	= skill_id;
		sSKILLEF.wLEVEL		= wLevel;
		sSKILLEF.fAGE		= sSKILL_DATA.fLIFE;

		m_sFITEMFACT[wSLOT] = sSKILLEF;

		return TRUE;
	}

	GASSERT ( 0 && "지속 스킬정보가 유효하지 않음." );
	return FALSE;
}

/*vehicle booster system, Juver, 2017/08/11 */
void GLCharacter::VehicleBoosterStart()
{
	if ( GLTradeClient::GetInstance().Valid() ) return;

	if ( !m_bVehicle )				return;
	if ( !m_sVehicle.m_bBooster )	return;
	if ( m_bBoosterStart )	return;

	if ( m_bBoosterCharge )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BIKE_BOOSTER_NOT_READY") );
		return;
	}

	GLMSG::SNETPC_REQ_VEHICLE_BOOSTER netmsgfield;
	netmsgfield.bStart = true;
	NETSENDTOFIELD ( &netmsgfield );
}

/*vehicle booster system, Juver, 2017/08/11 */
void GLCharacter::VehicleBoosterStateOn()
{
	if ( IsSTATE( EM_ACT_VEHICLE_BOOSTER ))	return;

	SetSTATE(EM_ACT_VEHICLE_BOOSTER);

	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

/*vehicle booster system, Juver, 2017/08/12 */
void GLCharacter::VehicleBoosterStateOff()
{
	if ( !IsSTATE( EM_ACT_VEHICLE_BOOSTER ))	return;

	ReSetSTATE(EM_ACT_VEHICLE_BOOSTER);

	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

/*system buffs, Juver, 2017/09/04 */
BOOL GLCharacter::SystemBuffAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT )
{
	if ( wSLOT >= SYSTEM_BUFF_SIZE )		return FALSE;
	m_sSYSTEM_BUFF[wSLOT].RESET();

	if ( skill_id == NATIVEID_NULL() )	return FALSE;

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )						return FALSE;
	if ( wLevel >= SKILL::MAX_SKILL_LEVEL )	return FALSE;

	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wLevel];

	BOOL bHOLD = FALSE;
	SSYSTEM_BUFF ssystem_buff;
	switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
	{
	case SKILL::EMFOR_VARHP:
	case SKILL::EMFOR_VARMP:
	case SKILL::EMFOR_VARSP:
	case SKILL::EMFOR_DEFENSE:
	case SKILL::EMFOR_HITRATE:
	case SKILL::EMFOR_AVOIDRATE:
	case SKILL::EMFOR_VARAP:
	case SKILL::EMFOR_VARDAMAGE:
	case SKILL::EMFOR_VARDEFENSE:
	case SKILL::EMFOR_PA:
	case SKILL::EMFOR_SA:
	case SKILL::EMFOR_MA:
	case SKILL::EMFOR_RESIST:
	case SKILL::EMFOR_SUMMONTIME: /*summon time, Juver, 2017/12/12 */
		bHOLD = TRUE;
		ssystem_buff.emTYPE = pSkill->m_sAPPLY.emBASIC_TYPE;
		ssystem_buff.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
	{
		if ( pSkill->m_sAPPLY.sImpacts[nImpact].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			ssystem_buff.sImpacts[nImpact].emADDON = pSkill->m_sAPPLY.sImpacts[nImpact].emADDON;
			ssystem_buff.sImpacts[nImpact].fADDON_VAR = pSkill->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[wLevel];
		}
	}

	for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
	{
		switch ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC )
		{
		case EMSPECA_REFDAMAGE:
		case EMSPECA_NONBLOW:
		case EMSPECA_PIERCE:
		case EMSPECA_TARRANGE:
		case EMSPECA_MOVEVELO:
		case EMSPECA_ATTACKVELO:
		case EMSPECA_SKILLDELAY:
		case EMSPECA_PSY_DAMAGE_REDUCE:
		case EMSPECA_MAGIC_DAMAGE_REDUCE:
		case EMSPECA_PSY_DAMAGE_REFLECTION:
		case EMSPECA_MAGIC_DAMAGE_REFLECTION:
		case EMSPECA_DEFENSE_SKILL_ACTIVE:
		case EMSPECA_CHANGE_ATTACK_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_CHANGE_APPLY_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_TALK_TO_NPC:			/*npc talk buff, Juver, 2017/10/03 */
		case EMSPECA_SPECIAL_ITEM_DROP:		/*special item drop buff, Juver, 2018/07/01 */
		case EMSPECA_ITEMDROP_RATE:			/*item drop rate buff, Juver, 2018/07/01 */
		case EMSPECA_MONEYDROP_RATE:		/*gold drop buff, Juver, 2018/07/01 */
		case EMSPECA_GETEXP_RATE:			/*exp rate buff, Juver, 2018/07/01 */
		case EMSPECA_AMPLIFY:				/* skill amplify, Juver, 2020/12/10 */
		case EMSPECA_STIGMA:				/* skill stigma, Juver, 2020/12/11 */
			bHOLD = TRUE;
			ssystem_buff.sSpecs[nSpec].emSPEC = pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC;
			ssystem_buff.sSpecs[nSpec].fSPECVAR1 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR1;
			ssystem_buff.sSpecs[nSpec].fSPECVAR2 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR2;
			ssystem_buff.sSpecs[nSpec].dwSPECFLAG = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwFLAG;
			ssystem_buff.sSpecs[nSpec].dwNativeID = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwNativeID;
			break;
		};
	}

	if ( bHOLD )
	{
		ssystem_buff.sNATIVEID	= skill_id;
		ssystem_buff.wLEVEL		= wLevel;

		m_sSYSTEM_BUFF[wSLOT] = ssystem_buff;

		return TRUE;
	}

	return FALSE;
}

/*bike color , Juver, 2017/11/16 */
void GLCharacter::BikeColorUpdate()
{
	if ( m_bEnableBikeColor && m_pSkinChar && m_pSkinChar->m_pVehicle && m_sVehicle.m_emTYPE == VEHICLE_TYPE_BIKE )
	{
		for ( int ncolor=0; ncolor<BIKE_COLOR_SLOT_PART_SIZE; ++ncolor )
		{
			EMPIECECHAR piece_color = BIKE_COLOR_SLOT_TO_PIECE( (BIKE_COLOR_SLOT_PART) ncolor );
			BOOL bColor1 = IS_BIKECOLOR_1( (BIKE_COLOR_SLOT_PART) ncolor );
			if ( bColor1 )
				m_pSkinChar->m_pVehicle->SetColor1( piece_color, m_wBikeColor[ncolor] );
			else
				m_pSkinChar->m_pVehicle->SetColor2( piece_color, m_wBikeColor[ncolor] );
		}
	}
}

/*bike color , Juver, 2017/11/16 */
void GLCharacter::BikeColorEnd()
{ 
	m_bEnableBikeColor = FALSE;
	
	for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
		m_wBikeColor[i] = 0;

	if ( m_pd3dDevice )
		UpdateSuit();
}

/*pvp capture the flag, Juver, 2018/02/08 */
void GLCharacter::CaptureTheFlagLocatorUpdate()
{
	if ( !m_bCaptureTheFlagLocatorOn || m_bCaptureTheFlagHoldFlag )
	{
		for( int i=0; i<CAPTURE_THE_FLAG_TEAM_SIZE; ++i )
		{
			DxEffExtra::GetInstance().RemoveLocatorAEffect( GLCONST_CHAR::strCaptureTheFlagDirectionEffect[i].c_str() );
			DxEffExtra::GetInstance().RemoveLocatorBEffect( GLCONST_CHAR::strCaptureTheFlagDirectionEffect[i].c_str() );
		}

		return;
	}

	STARGETID starget_id ( CROW_PC, m_dwGaeaID, m_vPos );

	if ( m_wCaptureTheFlagTeam == CAPTURE_THE_FLAG_TEAM_A )
	{
		D3DXVECTOR3 vtargetpos = GLPVPCaptureTheFlagClient::GetInstance().m_sFlagTarget[CAPTURE_THE_FLAG_TEAM_B].vPos;
		D3DXVECTOR3 vfindpos = GLGaeaClient::GetInstance().GetTargetPos( GLPVPCaptureTheFlagClient::GetInstance().m_sFlagTarget[CAPTURE_THE_FLAG_TEAM_B] );
		if ( vfindpos != VERROR() )	vtargetpos = vfindpos;

		D3DXVECTOR3 vdirection =  m_vPos - vtargetpos;
		D3DXVec3Normalize ( &vdirection, &vdirection );
		float ftheta = DXGetThetaYFromDirection ( vdirection, m_vDirOrig );
		D3DXMATRIX  matworld, mattrans, matrotate;
		D3DXMatrixTranslation ( &mattrans, m_vPos.x, m_vPos.y, m_vPos.z );
		D3DXMatrixRotationY ( &matrotate, ftheta );
		matworld = matrotate * mattrans;
		DxEffExtra::GetInstance().NewLocatorAEff ( GLCONST_CHAR::strCaptureTheFlagDirectionEffect[CAPTURE_THE_FLAG_TEAM_B].c_str(), matworld, &starget_id );
		return;
	}

	if ( m_wCaptureTheFlagTeam == CAPTURE_THE_FLAG_TEAM_B )
	{
		D3DXVECTOR3 vtargetpos = GLPVPCaptureTheFlagClient::GetInstance().m_sFlagTarget[CAPTURE_THE_FLAG_TEAM_A].vPos;
		D3DXVECTOR3 vfindpos = GLGaeaClient::GetInstance().GetTargetPos( GLPVPCaptureTheFlagClient::GetInstance().m_sFlagTarget[CAPTURE_THE_FLAG_TEAM_A] );
		if ( vfindpos != VERROR() )	vtargetpos = vfindpos;

		D3DXVECTOR3 vdirection =  m_vPos - vtargetpos;
		D3DXVec3Normalize ( &vdirection, &vdirection );
		float ftheta = DXGetThetaYFromDirection ( vdirection, m_vDirOrig );
		D3DXMATRIX  matworld, mattrans, matrotate;
		D3DXMatrixTranslation ( &mattrans, m_vPos.x, m_vPos.y, m_vPos.z );
		D3DXMatrixRotationY ( &matrotate, ftheta );
		matworld = matrotate * mattrans;
		DxEffExtra::GetInstance().NewLocatorAEff ( GLCONST_CHAR::strCaptureTheFlagDirectionEffect[CAPTURE_THE_FLAG_TEAM_A].c_str(), matworld, &starget_id );
		return;
	}
}

/*skill transform, Juver, 2018/09/09 */
void GLCharacter::skill_transform_change_skin()
{
	if ( !m_pd3dDevice )	return;
	if ( !m_pSkinChar )		return;

	EMCHARINDEX chadr_inex = CharClassToIndex(m_emClass);
	std::string skin_file = GLCONST_CHAR::szCharSkin[chadr_inex];

	if ( m_skill_transform_data.is_skin_change_valid() )
	{
		skin_file = GLTransformSkinSet::GetInstance().get_skin( m_skill_transform_data.skin_change_id );	
	}

	if ( skin_file.size() != 0 )
	{
		std::string active_skin_file = m_pSkinChar->GetFileName();
		if ( strcmp( skin_file.c_str(), active_skin_file.c_str() ) != 0 )
		{
			DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( skin_file.c_str(), m_pd3dDevice, TRUE );
			if ( !pSkinChar )	return;

			SAFE_DELETE(m_pSkinChar);
			m_pSkinChar = new DxSkinChar;
			m_pSkinChar->SetCharData ( pSkinChar, m_pd3dDevice, TRUE );

			UpdateSuit( TRUE );

			m_pSkinChar->GetAABBBox( m_vMaxOrg, m_vMinOrg );
			m_fHeight = m_vMaxOrg.y - m_vMinOrg.y;

			m_pSkinChar->SELECTANI ( AN_GUARD_N, AN_SUB_NONE );

			if ( !m_pSkinChar->GETCURANIMNODE() )
				m_pSkinChar->DEFAULTANI();

			ReSelectAnimation ();

			EMSLOT emRHand = GetCurRHand();
			EMSLOT emLHand = GetCurLHand();
			m_emANISUBTYPE = CHECK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );

			FACTEFF::ReNewEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT, m_sSTATEBLOWS, m_matTrans, m_vDir );
		}
	}
}

/*client tick count check, EJCode, 2018/11/26 */
void GLCharacter::client_tick_count_check( DWORD key1, DWORD key2 )
{
	GLMSG::SNETPC_CLIENT_TICK_COUNT_CHECK_RESULT	net_msg_server;
	net_msg_server.result = ::GetTickCount();
	net_msg_server.key1 = key1;
	net_msg_server.key2 = key2;
	NETSENDTOFIELD( &net_msg_server );
}

bool GLCharacter::isPetLastUseTime()
{
	if ( m_fPetLastUseTime < PET_LAST_USE_TIME_DEFAULT )	return true;

	return false;
}

void GLCharacter::resetPetLastUseTime()
{
	m_fPetLastUseTime = 0.0f;

	CInnerInterface::GetInstance().HideGroup( MAPMOVE_DISPLAY );
	CInnerInterface::GetInstance().HideGroup( MULTI_MAPMOVE_DISPLAY );	/* multi gate out, Juver, 2020/11/19 */
	CInnerInterface::GetInstance().SetCompetitionJoin( false );
}

bool GLCharacter::isValidPetBlockTime()
{
	if ( m_fPetBlockTime < PET_BLOCK_TIME_DEFAULT )	return true;

	return false;
}

void GLCharacter::resetPetBlockTime()
{
	m_fPetBlockTime = 0.0f;
}

/* personal lock system, Juver, 2019/12/06 */
BOOL GLCharacter::isPersonalLock( EMPERSONAL_LOCK emLock )
{
	/* user flag restricted, Juver, 2020/04/21 */
	if ( m_bUserFlagRestricted )			return TRUE;

	if ( !RPARAM::bUsePersonalLock )		return FALSE;
	if ( emLock >= EMPERSONAL_LOCK_SIZE )	return FALSE;
	if ( m_sPersonalLock[emLock].bLock )	return TRUE;

	return FALSE;
}

/* post system, Juver, 2021/02/05 */
bool GLCharacter::IsPOSTBOXUSEABLE ( const EMPOSTBOXID_TYPE emIDType, const SNATIVEID& sPostBoxID )
{
	// Note : 우편함 사용 불가능이면
	if ( !RPARAM::bUsePost)
	{
		return false;
	}

	if ( !IsValidBody() )
	{
		return false;
	}

	if ( emIDType == EMPOSTBOXID_TYPE_NONE || sPostBoxID == NATIVEID_NULL() )
	{
		return false;
	}

	// Note : 우편함 아이디가 있다면
	switch ( emIDType )
	{
	default:
		return false;

	case EMPOSTBOXID_TYPE_NPC:
		{
			PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap();

			if ( !pLandMan )
			{
				return false;
			}

			// 우체통과의 거리 체크
			GLCrowClient* pCrowClient =  pLandMan->GetCrow(sPostBoxID.dwID);
			if (!pCrowClient || !pCrowClient->m_pCrowData)
			{
				return false;
			}

			if (pCrowClient->m_pCrowData->m_sBasic.m_emCrow != CROW_NPC || 
				pCrowClient->m_pCrowData->m_sBasic.m_emNPCType != EMNPC_TYPE_POSTBOX )
			{
				return false;
			}

			const D3DXVECTOR3 &vTarPos = pCrowClient->GetPosition();

			float fMoveDist = FLT_MAX;
			float fDist = D3DXVec3Length ( &D3DXVECTOR3(GetPosition() - vTarPos) );
			if ( IsACTION(GLAT_MOVE) )
				fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) );

			// 우편 이용이 가능한 범위
			WORD wTakeRange   = pCrowClient->GetBodyRadius() + GETBODYRADIUS() + 30;
			WORD wTakeAbleDis = (WORD)(wTakeRange + GLCONST_POST::fPOSTBOX_DISTANCE);

			// 지금 위치가 대화가 불가능 하면
			if ( fDist>wTakeAbleDis )
			{
				return false;
			}
		}
		break;

	case EMPOSTBOXID_TYPE_ITEM_CARD:
		{
			SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sPostBoxID );
			if ( !pITEM )
			{
				return false;
			}

			// rm #1559 우편연결카드 수량 1개시 우편UI 뜨지 않는 문제..
			// 이미 차감후에 아래코드 검사하면 아무것도 없어서 무조건 false 임.
			// 이미 서버에서 정상 처리되어 오기때문에 여기서 이걸 할필요 없을것으로 보임.
			//SINVENITEM* pInvenItem = m_cInventory.FindItem( ITEM_POSTBOX_CARD );
			//if ( !pInvenItem )
			//{
			//	return false;
			//}
		}
		break;
	}

	return true;
}

BOOL GLCharacter::IsPurchaseSkill( SNATIVEID sSkillID )
{
	//skill already learned 
	SCHARSKILL* pCharSkill = GETLEARNED_SKILL( sSkillID );
	if ( pCharSkill )
		return FALSE;

	//skill has no purchase configuration
	SSKILL_PURCHASE* pPurchase = GLSkillPurchase::GetInstance().getSkillPurchase( sSkillID );
	if ( !pPurchase )
		return FALSE;

	//skill stats failed
	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSkillID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
		return FALSE;

	//cost check
	if ( m_lnMoney < pPurchase->dwCost )
		return FALSE;

	return TRUE;
}

void GLCharacter::ReqPurchaseSkill( SNATIVEID sSkillID )
{
	if ( IsPurchaseSkill( sSkillID ) == FALSE )	return;

	GLMSG::SNETPC_REQ_PURCHASE_SKILL NetMsg;
	NetMsg.sSkillID = sSkillID;
	NETSENDTOFIELD ( &NetMsg );
}

BOOL GLCharacter::isAutoPotionBlockManual()
{
	//auto potion is disabled
	if ( RPARAM::bUseAutoPotion == FALSE )
		return FALSE;

	// auto potion in the current map is off
	if ( GLGaeaClient::GetInstance().GetActiveMap()->isUseAutoPotion() == FALSE )
		return FALSE;

	// auto potion allow manual use of potion
	if ( GLGaeaClient::GetInstance().GetActiveMap()->isAutoPotionManual() == TRUE )
		return FALSE;

	return TRUE;
}

void GLCharacter::ReqSetAutoPotion( float fHP, float fMP, float fSP )
{
	if ( RPARAM::bUseAutoPotion == FALSE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_POTION_FB_FAIL") );
		return;
	}

	if ( GLGaeaClient::GetInstance().GetActiveMap()->isUseAutoPotion() == FALSE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_POTION_FB_NOT_MAP") );
		return;
	}

	if ( fHP <= 0.0f || fMP <= 0.0f || fSP <= 0.0f )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_POTION_FB_INVALID_VALUE") );
		return;
	}

	GLMSG::SNETPC_REQ_AUTO_POTION netMsg;
	netMsg.fHP = fHP;
	netMsg.fMP = fMP;
	netMsg.fSP = fSP;
	NETSENDTOFIELD ( &netMsg );
}

BOOL GLCharacter::isAutoPilotActive()
{
	if ( m_pAutoPilot && m_pAutoPilot->isActive() == TRUE )
		return TRUE;
	else
		return FALSE;
}


void GLCharacter::AutoPilotScanTarget(bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo)
{
	if ( m_pAutoPilot == NULL )
		return;

	if ( m_pAutoPilot->m_bValidTarget == FALSE )
	{
		//find target

		DETECTMAP_RANGE	pairRange;
		SACTIONTAR sACTAR_MOB;
		STARGETID sTID_FOUND;

		DETECTMAP* pDetectMap = NULL;
		DWORD emCrow = CROW_EX_MOB;

		pDetectMap = GLGaeaClient::GetInstance().DetectCrow(emCrow, m_pAutoPilot->m_vPos, m_pAutoPilot->m_fRange );
		if (pDetectMap)
		{
			pairRange = std::equal_range(pDetectMap->begin(), pDetectMap->end(), CROW_MOB, STAR_ORDER());
			if (pairRange.first != pairRange.second)
			{
				sTID_FOUND = CFINDER::GetInstance().FindClosedCrow(pairRange, m_pAutoPilot->m_vPos);

				sACTAR_MOB.emACTAR	= EMACTAR_MOB;
				sACTAR_MOB.sTARID	= sTID_FOUND;
			}
		}

		if (sACTAR_MOB.emACTAR != EMACTAR_NULL)
		{
			m_pAutoPilot->m_sTargetID = sACTAR_MOB.sTARID;
			m_pAutoPilot->m_bValidTarget = TRUE;
		}
		else
		{
			if ( IsACTION(GLAT_MOVE) == FALSE  && IsACTION(GLAT_SKILL) == FALSE )
			{
				//no target was found
				//lets to back to starting position
				bMove = TRUE;
				vMoveTo = m_pAutoPilot->m_vPos;
			}
			
		}
		return;
	}

	//framemove was not able to find an available skill
	if ( m_pAutoPilot->m_sSkillRun.IsValidNativeID() == FALSE )
		return;



	m_sRunSkill		= m_pAutoPilot->m_sSkillRun;
	m_sActiveSkill	= m_sRunSkill;

	if ( StateStunGet() )	
		return;

	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLAND && pLAND->IsPeaceZone() )	
		return;

	STARGETID sTargetID = m_pAutoPilot->m_sTargetID;

	//	Note : 리엑션 취소.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	m_idACTIVESKILL = NATIVEID_NULL();

	//	Note : 익힌 스킬인지 검사.
	//
	if ( !ISLEARNED_SKILL ( m_sActiveSkill ) && !IsDefenseSkill() )	
		return;

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

	float fMoveDist = FLT_MAX;
	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );	// 내가 이동한 거리
	if ( IsACTION(GLAT_MOVE) )	
		fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) ); 

	//	Note : 스킬 정보 가져옴.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sActiveSkill.wMainID, m_sActiveSkill.wSubID );
	if ( !pSkill )								
		return;

	EMIMPACT_TAR emTARGET = pSkill->m_sBASIC.emIMPACT_TAR;

	
#if /*defined( BUILD_EP7 ) || */defined( BUILD_EP6 ) || defined( BUILD_EP4 )
	if ( emTARGET==TAR_SPEC || emTARGET==TAR_SELF_TOSPEC )
#else
	if( emTARGET != TAR_SELF )
#endif 
	{
		//GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID.emCrow, sTargetID.dwID );
		//if ( pTarget )
		{
			WORD wAttackRange = /*pTarget->GetBodyRadius() +*/ CHARACTER_BODY_RADIUS + GETSKILLRANGE_TAR(*pSkill) + 2;
			WORD wAttackAbleDis = wAttackRange + 2;

			/* skill collision check, Juver, 2021/07/11 */
			if ( RPARAM::bSCCC )
			{
				const D3DXVECTOR3 _vDeviation(0.0f, 10.0f, 0.0f);
				BOOL _bColl = m_actorMove.LineOfSightTest ( vTarPos + _vDeviation, vTarPos - _vDeviation );
				if ( !_bColl )
				{
					//this target cannot be targeted
					m_pAutoPilot->m_bValidTarget = FALSE;
					return;
				}
			}

			// 공격이 가능하지 않을 경우
			if ( fDist>wAttackAbleDis )
			{
				// 타겟이 움직인 거리가 공격 가능거리보다 더 멀면 다시 타겟정보 갱신
				if ( fMoveDist > wAttackAbleDis )
				{
					//	Note : 이동지점 지정.
					//
					D3DXVECTOR3 vPos = vTarPos - m_vPos;
					D3DXVec3Normalize ( &vPos, &vPos );

					bMove = TRUE;
					vMoveTo = vTarPos - vPos*wAttackRange;
					vMoveTo.y = vTarPos.y;

					//	Note : 리엑션 등록.
					//
					m_sREACTION.emREACTION = REACT_SKILL;
					m_sREACTION.bCONTINUE = bcontinue;
					m_sREACTION.sTARID = sTargetID;
					m_sREACTION.vMARK_POS = vMoveTo;
				}
				// 아니면 예전 정보로 갱신
				else
				{
					m_sREACTION = sREC_TEMP;
				}

				//target too far
				m_pAutoPilot->m_bValidTarget = FALSE;
				return;
			}
		}
	}


	//	지형과 충돌하는지 검사.
	//	
	if ( emTARGET!=TAR_SELF && sTargetID.dwID!=EMTARGET_NULL )
	{
		D3DXVECTOR3 vPos1 = GetPosition();	vPos1.y += 15.0f;
		D3DXVECTOR3 vPos2 = vTarPos;		vPos2.y += 15.0f;
		
		BOOL bCollision = TRUE;
		D3DXVECTOR3 vCollision;
		LPDXFRAME pDxFrame = NULL;

		DxLandMan *pLandMan = GLGaeaClient::GetInstance().GetActiveMap()->GetLandMan();
		pLandMan->IsCollision ( vPos1, vPos2, vCollision, bCollision, pDxFrame, FALSE );
		if ( bCollision )						
			return;
	}

	PANIMCONTNODE pAnicont = m_pSkinChar->GETANI ( pSkill->m_sEXT_DATA.emANIMTYPE, pSkill->m_sEXT_DATA.emANISTYPE );
	if ( !pAnicont )
		return;

	WORD wStrikeNum = pAnicont->pAnimCont->m_wStrikeCount;

	//	Note : 스킬 수행 가능 검사.
	//
	/*extreme auto tab, Juver, 2018/09/10 */
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	BOOL skill_change_weapon_tab = FALSE;
	EMSKILLCHECK emSkillCheck = GLCHARLOGIC_CLIENT::CHECHSKILL ( m_sActiveSkill, 1, IsDefenseSkill(), emRHand, emLHand );
	if ( emSkillCheck == EMSKILL_NOTITEM && ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W ))
	{
		if ( RPARAM::allow_skill_tab_change )
		{
			/*extreme auto tab, Juver, 2018/09/10 */
			emRHand = IsUseArmSub()? SLOT_RHAND:SLOT_RHAND_S;
			emLHand = IsUseArmSub()? SLOT_LHAND:SLOT_LHAND_S;

			emSkillCheck = GLCHARLOGIC_CLIENT::CHECHSKILL ( m_sActiveSkill, 1, IsDefenseSkill(), emRHand, emLHand );

			if ( emSkillCheck==EMSKILL_OK || emSkillCheck==EMSKILL_NOTSP)
				skill_change_weapon_tab = TRUE;
		}
	}

	if ( emSkillCheck!=EMSKILL_OK && emSkillCheck!=EMSKILL_NOTSP)
	{
		/*extreme auto tab, Juver, 2018/09/10 */
		skill_change_weapon_tab = FALSE;

		return;
	}


	//defaults target pos?
	//server needs this to have value
	m_vTARGET_POS_MAIN = sTargetID.vPos;
	m_vTARGET_POS_SUB = sTargetID.vPos;

	//	Note : Skill 목표를 선택함.
	//
	/*multi skill Juver, 2018/09/04 */
	BOOL bOK = SelectSkillTarget_Main ( sTargetID, &sTargetID.vPos );
	if ( !bOK )
	{
		/*extreme auto tab, Juver, 2018/09/10 */
		skill_change_weapon_tab = FALSE;

		return;
	}

	/*multi skill Juver, 2018/09/04 */
	SelectSkillTarget_Sub( sTargetID, &sTargetID.vPos );

	//	Note : 방향 회전이 필요할 경우 방향을 튼다.
	//
	if ( emTARGET==TAR_SPEC || emTARGET==TAR_SELF_TOSPEC )
	{
		/*multi skill Juver, 2018/09/04 */
		if ( m_sTARGET_IDS_MAIN[0].wID != USHRT_MAX )
		{
			STARGETID sRotateTarID ( static_cast<EMCROW>(m_sTARGET_IDS_MAIN[0].wCrow), static_cast<DWORD>(m_sTARGET_IDS_MAIN[0].wID) );
			GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sRotateTarID );
			if ( pTarget )
			{
				D3DXVECTOR3 vDirection = pTarget->GetPosition() - m_vPos;
				D3DXVec3Normalize ( &vDirection, &vDirection );
				m_vDir = vDirection;
			}
		}

		//	Note : 현제 위치와 방향으로 Transform 메트릭스 계산.
		//
		D3DXMATRIX matTrans, matYRot;
		D3DXMatrixTranslation ( &matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
		float fThetaY = DXGetThetaYFromDirection ( m_vDir, m_vDirOrig );
		D3DXMatrixRotationY ( &matYRot, fThetaY );

		m_matTrans = matYRot * matTrans;
	}
	/*specific skill target, Juver, 2018/08/27 */
	else if ( emTARGET==TAR_ZONE || emTARGET==TAR_SPECIFIC )
	{
		/*multi skill Juver, 2018/09/04 */
		D3DXVECTOR3 vDirection = m_vTARGET_POS_MAIN - m_vPos;
		D3DXVec3Normalize ( &vDirection, &vDirection );
		m_vDir = vDirection;
	}
	else if ( emTARGET==TAR_SELF )
	{
		//	Note : 방향 회전 필요 없음.
	}

	/*extreme auto tab, Juver, 2018/09/10 */
	if ( skill_change_weapon_tab && RPARAM::allow_skill_tab_change )
	{
		SetUseArmSub( !IsUseArmSub() );

		CInnerInterface::GetInstance().SetArmSwap( IsUseArmSub() );
		GLCHARLOGIC_CLIENT::INIT_DATA ( FALSE, FALSE, m_sCONFTING.sOption.fHP_RATE );
		UpdateSuit( TRUE );
		ReSelectAnimation();

		if ( RPARAM::allow_skill_tab_change_buff_reset )
		{
			// 무기에 따른 버프를 초기화 한다.
			DisableSkillFact();
		}

		GLMSG::SNETPC_SKILL_TAB_CHANGE	net_msg_server;
		net_msg_server.sub_arms = IsUseArmSub();
		NETSENDTOFIELD( &net_msg_server );

		m_skill_change_weapon_tab_pending = TRUE;
		m_skill_change_weapon_tab_pending_time = SKILL_CHANGE_WEAPON_TAB_PENDING_TIME;
	}

	//	Note : 스킬이 발동됨.
	//
	
	TurnAction ( GLAT_SKILL );

	if ( bcontinue )
	{
		m_sREACTION.emREACTION = REACT_SKILL;
		m_sREACTION.bCONTINUE = true;
		m_sREACTION.sTARID = sTargetID;
		m_sREACTION.vMARK_POS = m_vPos;
	}

}

void GLCharacter::ReqExchangeItemOpen()
{
	m_sExchangeItemInvenPos.RESET();
	m_sPreInventoryItem.RESET();
	m_bExchangeItemOpen = true;
}

void GLCharacter::ReqExchangeItemClose()
{
	m_sExchangeItemInvenPos.RESET();
	m_sPreInventoryItem.RESET();
	m_bExchangeItemOpen = false;
}

HRESULT GLCharacter::ReqExchangeItemMoveItem()
{
	if( m_sPreInventoryItem.VALID() )
	{
		SITEMCUSTOM sPreItem = GET_PREHOLD_ITEM();
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sPreItem.sNativeID );
		if( !pItem )	
		{
			m_sPreInventoryItem.RESET();
			return E_FAIL;
		}

		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
		{
			GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
			if ( pMyPet && pMyPet->IsVALID () && sPreItem.dwPetID == pMyPet->m_dwPetID )
			{
				m_sPreInventoryItem.RESET();
				return E_FAIL;
			}
		}

		if( pItem )
			m_sExchangeItemInvenPos.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );

		m_sPreInventoryItem.RESET();
	}

	return S_OK;
}

void	GLCharacter::ReqExchangeItemResetItem()
{
	m_sPreInventoryItem.RESET();
	m_sExchangeItemInvenPos.RESET();
}

const SITEMCUSTOM& GLCharacter::GetExchangeItemItem()
{
	static SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();

	if( !m_sExchangeItemInvenPos.VALID() )	
		return sItemCustom;

	SINVENITEM* pResistItem = m_cInventory.GetItem( m_sExchangeItemInvenPos.wPosX, m_sExchangeItemInvenPos.wPosY );
	if( !pResistItem )	
		return sItemCustom;

	sItemCustom = pResistItem->sItemCustom;
	return sItemCustom;
}

HRESULT GLCharacter::ReqExchangeItem( SNATIVEID sSelectedID, WORD wX, WORD wY )
{
	if( !m_sExchangeItemInvenPos.VALID() )
		return S_FALSE;

	SITEM* pItemCheck = GLItemMan::GetInstance().GetItem( sSelectedID );
	if ( !pItemCheck )
		return S_FALSE;

	GLMSG::SNET_REQ_EXCHANGE_ITEM NetMsg;
	NetMsg.wPosX = m_sExchangeItemInvenPos.wPosX;
	NetMsg.wPosY = m_sExchangeItemInvenPos.wPosY;
	NetMsg.wCardPosX = wX;
	NetMsg.wCardPosY = wY;
	NetMsg.sID = sSelectedID;
	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}