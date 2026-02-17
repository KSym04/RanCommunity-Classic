#include "stdafx.h"

#include "./GLChar.h"
#include "./GLCharData.h"
#include "./GLGaeaServer.h"
#include "./GLSchoolFreePK.h"
#include "./GLClubDeathMatch.h"

/*pvp tyranny, Juver, 2017/08/24 */
#include "./GLPVPTyrannyField.h"

/*school wars, Juver, 2018/01/19 */
#include "./GLPVPSchoolWarsField.h"

/*pvp capture the flag, Juver, 2018/01/31 */
#include "./GLPVPCaptureTheFlagField.h"

/* pvp club death match, Juver, 2020/11/26 */
#include "./PVPClubDeathMatchField.h"

#include "./GLAutoPotion.h"
#include "./GLAutoPilotS.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "./UserTypeDefine.h"
#include "./RTimerDefine.h"

#include "./PVPPartyBattleGroundsField.h"

/*dmk14 freepk*/
#include "GLFreePK.h"

/*woe Arc Development 08-06-2024*/
#include "GLPVPWoeField.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BOX_R	(4)

GLChar::GLChar () :

	/*multi skill Juver, 2018/09/04 */
	m_wTARGET_NUM_MAIN(0),
	m_wTARGET_NUM_SUB(0),
	m_vTARGET_POS_MAIN(0.0f,0.0f,0.0f),
	m_vTARGET_POS_SUB(0.0f,0.0f,0.0f),

	m_dwANISUBSELECT(0),

	m_dwActState(NULL),
	m_Action(GLAT_IDLE),

	m_fIdleTime(0.0f),
	m_fMBR_TIMER(0.0f),
	m_fSTATE_TIMER(0.0f),
	m_fPERIOD_TIMER(0.0f),
	m_fQUEST_TIMER(60.0f),
	m_fITEM_TIMER(0.0f),
	m_fSAVEDB_TIMER(0.0f),
	m_fattTIMER(0.0f),
	m_fMoveDelay(0.0f),
	m_fBRIGHT_TIMER(0.0f),

	m_sAssault(CROW_PC,GAEAID_NULL),
	m_lnLastSendExp(0),

	m_lnLastSendVNExp(0),

	m_dwViewFrame(0),
	m_fAge(0.0f),
	m_fGenAge(0.0f),
	m_nDECEXP(0),
	m_vPos(0,0,0),
	
	m_dwClientID(0),
	m_dwGaeaID(0),
	m_dwPartyID(PARTY_NULL),

	m_dwCeID(0),

	m_wHealingNum(0),

	m_pLandMan(NULL),
	
	m_pGaeaNode(NULL),
	m_pLandNode(NULL),

	m_pQuadNode(NULL),
	m_pCellNode(NULL),

	m_SKILLMTYPE(AN_SKILL_A),
	m_SKILLSTYPE(AN_SUB_00),

	m_bUsedStorageCard(false),

	m_bNon_Rebirth(false),

	/*charinfoview , Juver, 2017/11/12 */
	m_bPrivateStats(false),

	m_fSkillDelay(0.0f),

	m_dwPetGUID(UINT_MAX),
	m_dwPetID(0),

	//m_dwSummonGUID(UINT_MAX),
	//m_dwSummonID(0),

	m_bEntryFailed(FALSE),


	m_bProtectPutOnItem(false),
	m_bGetPetFromDB(false),
	m_bGetVehicleFromDB ( false ),
	m_fVehicleTimer( 0.0f ),

	m_ItemRemodelPosX(EM_INVENSIZE_X),
	m_ItemRemodelPosY(EM_INVENSIZE_Y),
	m_ItemGarbagePosX(EM_INVENSIZE_X),
	m_ItemGarbagePosY(EM_INVENSIZE_Y),

	m_ChinaGainType(255),
	m_dwVietnamGainType(GAINTYPE_MAX),

	m_bEventStart(FALSE),
	m_bEventApply(FALSE),
	m_hCheckStrDLL( NULL ),
	m_pCheckString( NULL ),

	m_sidLastMapCheck(false),
	m_pGLGaeaServer( NULL ),
	m_bItemShopOpen ( false ),
	m_bDefenseSkill ( false ),

	m_fContinuousDamageCheck(0.0f), /*continuous damage skill logic, Juver, 2017/06/10 */
	m_fSummonCheckTime(0.0f),		/*skill summon, Juver, 2017/10/10 */

	/*contribution/activity bonus, Juver, 2017/10/14 */
	m_bUseContributionPointTimer(FALSE),
	m_bUseActivityPointTimer(FALSE),
	m_fContributionPointBonusTimer(0.0f),
	m_fActivityPointBonusTimer(0.0f),

	m_bInstanceDropOut(FALSE),
	m_fInstanceDropTimer(0.0f),

	/*product item, Juver, 2017/10/18 */
	m_bItemCompoundTask(FALSE),
	m_dwItemCompoundTaskID(UINT_MAX),
	m_fItemCompoundTaskTime(0.0f),
	m_fItemCompoundTaskTimer(0.0f),

	/*activity system, Juver, 2017/11/02 */
	m_sActivityCheckMapID(NATIVEID_NULL()),
	m_bActivityMapCheckTimer(FALSE),
	m_fActivityMapCheckTimer(0.0f),

	/*clubwar verify non push or pull, Juver, 2017/12/13 */
	m_bClubWarVerify(FALSE),
	m_fClubWarVerifyTimer(0.0f),

	/*pvp capture the flag, Juver, 2018/02/06 */
	m_fCaptureTheFlagDetectGateTimer(0.0f),
	m_fCaptureTheFlagHoldFlagTimer(0.0f),

	/*inventory sort, Juver, 2018/07/07 */
	m_fInventorySortTimer(0.0f),

	/*quest auto start in map, Juver, 2018/07/16 */
	m_sQuestCheckMapID(false),
	m_bQuestMapCheckTimer(FALSE),
	m_fQuestMapCheckTimer(0.0f),

	/*item random option rebuild, Juver, 2018/07/03 */
	m_item_random_option_rebuild_timer(0.0f),
	m_item_random_option_rebuild_active_task(FALSE),

	///*hide costume, EJCode, 2018/11/18 */
	//m_hide_costume(false),
	//m_hide_costume_timer(0.0f),

	/*client tick count check, EJCode, 2018/11/28 */
	m_client_tick_count_check_send_timer(0.0f),
	m_client_tick_count_check_error_count(0),
	m_client_tick_count_check_last_tick(0),

	/*pet fix add call delay, EJCode, 2018/11/28 */
	m_fPetUseDelay(0.0f),

	m_fCurrencyShopDelayTimer(CURRENCY_SHOP_TIME_DEFAULT),

	/* personal lock system, Juver, 2019/12/06 */
	m_fPersonalLockDelayTimer(PERSONAL_LOCK_DELAY_TIME_DEFAULT),

	/* force auto revive, Juver, 2020/12/23 */
	m_fForceAutoReviveTimer(0.0f),

	/*12-16-24, daily login - CNDev*/
	m_bDailyLoginTimer(false),
	m_fDailyLoginTimer(0.0f),
	m_fMaxRVPreviewTimer(MAX_RV_PREVIEW_COOLTIME),

	m_pAutoPotion(NULL),
	m_fAccumTime(0.0f)
{


	m_sSaveMapID = NATIVEID_NULL();
	m_ARoundSlot.SetChar ( this );


	m_China15Time = CTime::GetCurrentTime();
	DWORD nMaxClient = GLGaeaServer::GetInstance().GetMaxClient();

	GASSERT(nMaxClient);
	m_arrayFieldPC = new PSFIELDCROW[nMaxClient];

	memset ( m_arrayFieldPC, 0, sizeof(PSFIELDCROW)*nMaxClient );
	memset ( m_arrayFieldCROW, 0, sizeof(PSFIELDCROW)*MAXCROW );
	memset ( m_arrayFieldMATERIAL, 0, sizeof(PSFIELDCROW)*MAXCROW );
	memset ( m_arrayFieldITEM, 0, sizeof(PSFIELDCROW)*MAXITEM );
	memset ( m_arrayFieldMONEY, 0, sizeof(PSFIELDCROW)*MAXMONEY );

	m_setLandEffectNum.clear();


	// PET
	m_arrayFieldPET = new PSFIELDCROW[nMaxClient];
	memset ( m_arrayFieldPET, 0, sizeof(PSFIELDCROW)*nMaxClient );

	m_arrayFieldSummon = new PSFIELDCROW[nMaxClient];
	memset ( m_arrayFieldSummon, 0, sizeof(PSFIELDCROW)*nMaxClient );

	MGOddEvenInit();

#ifdef TH_PARAM
	HMODULE m_hCheckStrDLL = LoadLibrary("ThaiCheck.dll");

	if ( m_hCheckStrDLL )
	{
		m_pCheckString = ( BOOL (_stdcall*)(CString)) GetProcAddress(m_hCheckStrDLL, "IsCompleteThaiChar");
	}
#endif

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		m_dwSummonGUID_FLD[i] = GAEAID_NULL;
	}

	m_pAutoPotion = new GLAutoPotion;
	m_pAutoPotion->Reset();

	m_pAutoPilot = new GLAutoPilotS(this);
	m_pAutoPilot->Reset();

	m_sCombatRecord.resetUpdate();
}

GLChar::~GLChar ()
{
	delete[] m_arrayFieldPC;

	// PET
	delete[] m_arrayFieldPET;


	// Summon
	delete[] m_arrayFieldSummon;

#ifdef TH_PARAM
	if( m_hCheckStrDLL ) FreeLibrary( m_hCheckStrDLL );
#endif

	delete m_pAutoPotion;
	m_pAutoPotion = NULL;

	delete m_pAutoPilot;
	m_pAutoPilot = NULL;
}

DWORD GLChar::GetBonusExp () const
{
	return GLCONST_CHAR::GETEXP_PC(m_wLevel);
}

void GLChar::RESET_DATA ()
{
	int i = 0;

	GLCHARLOGIC_SERVER::RESET_DATA();

	m_actorMove.ResetMovedData();
	m_actorMove.Stop();
	m_actorMove.Release();

	m_vDir = D3DXVECTOR3(1,0,0);

	m_TargetID = STARGETID(CROW_PC,GAEAID_NULL);
	m_sNPCID   = STARGETID(CROW_PC,GAEAID_NULL);

	/*multi skill Juver, 2018/09/04 */
	m_wTARGET_NUM_MAIN = 0;
	m_wTARGET_NUM_SUB = 0;
	m_vTARGET_POS_MAIN = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vTARGET_POS_SUB = D3DXVECTOR3(0.0f,0.0f,0.0f);
	for ( i=0; i<EMTARGET_NET; ++i )
	{
		m_sTARGET_IDS_MAIN[i] = STARID(CROW_PC,GAEAID_NULL);
		m_sTARGET_IDS_SUB[i] = STARID(CROW_PC,GAEAID_NULL);
	}

	m_dwANISUBSELECT = 0;

	m_Action = GLAT_IDLE;
	m_dwActState = NULL;
	m_fIdleTime = 0.0f;
	m_fMBR_TIMER = 0.0f;
	m_fSTATE_TIMER = 0.0f;
	m_fPERIOD_TIMER = 0.0f;
	m_fQUEST_TIMER = 60.0f;	// Ã³À½ ÇÑ¹ø ¹«Á¶°Ç ½ÇÇàµÇ°Ô ÇÏ±â À§ÇÏ¿©

	m_fattTIMER = 0.0f;
	m_fBRIGHT_TIMER = 0.0f;

	m_fMoveDelay = 0.0f;
	m_sAssault = STARGETID(CROW_PC,GAEAID_NULL);

	m_ARoundSlot.Reset();

	m_cDamageLog.clear();

	m_dwThaiCCafeClass	= 0;
	// ¸»·¹ÀÌ½Ã¾Æ PC¹æ ÀÌº¥Æ®
	m_nMyCCafeClass		= 0;

	m_bVietnamLevelUp = false;

	m_ChinaGainType    = 255;

	m_bTracingUser     = FALSE;

	m_dwVietnamGainType = GAINTYPE_MAX;

	m_bEventStart	   = FALSE;

	m_bEventApply	   = FALSE;

	m_sChinaTime.Init();
	m_sEventTime.Init();
	m_sVietnamSystem.Init();

	

	m_fAge = 0.0f;
	m_fGenAge = 0.0f;
	m_vPos = D3DXVECTOR3(0,0,0);

	m_dwClientID = 0;
	m_dwGaeaID = GAEAID_NULL;
	m_dwPartyID = PARTY_NULL;

	m_sMapID    = SNATIVEID(false);
	m_sOldMapID = SNATIVEID(false);
	m_dwCeID = 0;
	
	m_pLandMan = NULL;

	m_pGaeaNode = NULL;
	m_pLandNode = NULL;

	m_pQuadNode = NULL;
	m_pCellNode = NULL;

	m_sTrade.Reset();
	m_sPMarket.DoMarketClose();
	m_sCONFTING.RESET();

	m_sHITARRAY.clear();
	m_SKILLMTYPE = AN_SKILL_A;
	m_SKILLSTYPE = AN_SUB_00;

	m_bUsedStorageCard = false;

	m_bNon_Rebirth     = false;

	/*charinfoview , Juver, 2017/11/12 */
	m_bPrivateStats = false;

	m_bGetPetFromDB    = false;

	m_dwPetGUID = UINT_MAX;
	m_dwPetID	= 0;

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		m_dwSummonGUID_FLD[i] = GAEAID_NULL;
	}

	m_bEntryFailed = FALSE;

	m_setLandEffectNum.clear();

	m_bProtectPutOnItem = false;

	m_China15Time = CTime::GetCurrentTime();

	MGOddEvenInit();

	m_sVehicle.RESET();
	m_bGetVehicleFromDB = false;
	m_fVehicleTimer = 0.0f;

	m_bItemShopOpen = false;
	m_bDefenseSkill = false;

	DWORD nMaxClient = GLGaeaServer::GetInstance().GetMaxClient();


	memset ( m_arrayFieldPC, 0, sizeof(PSFIELDCROW)*nMaxClient );
	memset ( m_arrayFieldCROW, 0, sizeof(PSFIELDCROW)*MAXCROW );
	memset ( m_arrayFieldMATERIAL, 0, sizeof(PSFIELDCROW)*MAXCROW );
	memset ( m_arrayFieldITEM, 0, sizeof(PSFIELDCROW)*MAXITEM );
	memset ( m_arrayFieldMONEY, 0, sizeof(PSFIELDCROW)*MAXMONEY );

	memset ( m_arrayFieldPET, 0, sizeof(PSFIELDCROW)*nMaxClient );
	memset ( m_arrayFieldSummon, 0, sizeof(PSFIELDCROW)*nMaxClient );

	/*skill summon, Juver, 2017/10/10 */
	m_fSummonCheckTime = 0.0f; 

	m_bInstanceDropOut = FALSE;
	m_fInstanceDropTimer = 0.0f;

	/*product item, Juver, 2017/10/18 */
	m_bItemCompoundTask = FALSE;
	m_dwItemCompoundTaskID = UINT_MAX;
	m_fItemCompoundTaskTime = 0.0f;
	m_fItemCompoundTaskTimer = 0.0f;

	/*activity system, Juver, 2017/11/02 */
	m_sActivityCheckMapID = NATIVEID_NULL();
	m_bActivityMapCheckTimer = FALSE;
	m_fActivityMapCheckTimer = 0.0f;

	/*clubwar verify non push or pull, Juver, 2017/12/13 */
	m_bClubWarVerify = FALSE;
	m_fClubWarVerifyTimer = 0.0f;

	/*pvp capture the flag, Juver, 2018/02/06 */
	m_fCaptureTheFlagDetectGateTimer = 0.0f;
	m_fCaptureTheFlagHoldFlagTimer = 0.0f;

	/*inventory sort, Juver, 2018/07/07 */
	m_fInventorySortTimer = 0.0f;

	/*quest auto start in map, Juver, 2018/07/16 */
	m_sQuestCheckMapID = NATIVEID_NULL();
	m_bQuestMapCheckTimer = FALSE;
	m_fQuestMapCheckTimer = 0.0f;

	///*hide costume, EJCode, 2018/11/18 */
	//m_hide_costume = false;
	//m_hide_costume_timer = 0.0f;

	/*client tick count check, EJCode, 2018/11/26 */
	m_client_tick_count_check_send_timer = 0.0f;
	m_client_tick_count_check_error_count = 0;
	m_client_tick_count_check_last_tick = 0;

	/*pet fix add call delay, EJCode, 2018/11/28 */
	m_fPetUseDelay = 0.0f;

	m_fCurrencyShopDelayTimer = RPARAM::fCurrencyShopPurchaseDelayTime;

	m_fPersonalLockDelayTimer = RPARAM::fPersonalLockCoolDownTime;

	/* force auto revive, Juver, 2020/12/24 */
	m_fForceAutoReviveTimer = 0.0f;

	m_fMaxRVPreviewTimer = MAX_RV_PREVIEW_COOLTIME;

	if ( m_pAutoPotion )
		m_pAutoPotion->Reset();

	if ( m_pAutoPilot )
		m_pAutoPilot->Reset();

	m_sCombatRecord.resetUpdate();
	/*12-16-24, daily login - CNDev*/
	m_bDailyLoginTimer = false;
	m_fDailyLoginTimer = 0.0f;
}

void GLChar::SetGLGaeaServer( GLGaeaServer *pGLGaeaServer )
{
	m_pGLGaeaServer = pGLGaeaServer;
}

HRESULT GLChar::CreateChar( GLLandMan* pLandMan, D3DXVECTOR3 &vPos, PCHARDATA2 pCharData, LPDIRECT3DDEVICEQ pd3dDevice, BOOL bNEW )
{
	HRESULT hr;
	//*/*/*/*
	m_vPos = vPos;

	

	m_dwThaiCCafeClass = pCharData->m_dwThaiCCafeClass;
	// ¸»·¹ÀÌ½Ã¾Æ PC¹æ ÀÌº¥Æ®
	m_nMyCCafeClass = pCharData->m_nMyCCafeClass;

	m_sChinaTime	   = pCharData->m_sChinaTime;
	m_sEventTime	   = pCharData->m_sEventTime;
	m_sVietnamSystem   = pCharData->m_sVietnamSystem;

	m_bTracingUser	   = pCharData->m_bTracingUser;

//	m_sEventState	   = m_pGLGaeaServer->m_sEventState;

	m_bEventStart = FALSE;
	m_bEventApply = FALSE;
	if( m_pGLGaeaServer->m_sEventState.bEventStart == TRUE )
	{
		// ÀÌº¥Æ®°¡ »õ·Î ½ÃÀÛ µÊ
		CTime	  crtTime     = CTime::GetCurrentTime();
		CTimeSpan crtGameSpan;
		CTime	  loginTime			 = m_sEventTime.loginTime;
		crtGameSpan					 = ( crtTime - loginTime );
		m_sEventTime.currentGameTime = crtGameSpan.GetTimeSpan();

		if( crtGameSpan.GetTotalSeconds() >= m_pGLGaeaServer->m_sEventState.EventPlayTime )
		{
			m_bEventStart = TRUE;
		}

		if( !(m_pGLGaeaServer->m_sEventState.MinEventLevel > pCharData->m_wLevel || 
			  m_pGLGaeaServer->m_sEventState.MaxEventLevel < pCharData->m_wLevel ) )
		{
			m_bEventApply = TRUE;
		}
	}


	hr = SetData ( pCharData, bNEW );
	if ( FAILED(hr) )	return E_FAIL;

	m_bEntryFailed = FALSE;
	// ÁÖ¼®µ¹·Á
	// ÁøÀÔÁ¶°Ç °Ë»ç
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		if( pLandMan->GetMapID() != NATIVEID_NULL() )
		{
			const SLEVEL_REQUIRE* pLEVEL_REQUIRE = GLGaeaServer::GetInstance().GetLevelRequire(pLandMan->GetMapID().dwID);
			EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
			if( pLEVEL_REQUIRE ) 
			{
				emReqFail = pLEVEL_REQUIRE->ISCOMPLETE ( this );
			}else{
				TEXTCONSOLEMSG_WRITE( "[ERROR]Level Data Load failed! charID[%s] mapID[%d][%d] EMFail[%d]", m_szName, pLandMan->GetMapID().wMainID, pLandMan->GetMapID().wSubID, emReqFail );
			}
			if ( emReqFail != EMREQUIRE_COMPLETE )
			{
				TEXTCONSOLEMSG_WRITE( "[INFO]Entry failed! charID[%s] mapID[%d][%d] EMFail[%d]", m_szName, pLandMan->GetMapID().wMainID, pLandMan->GetMapID().wSubID, emReqFail );
				m_bEntryFailed = TRUE;
			}
		}else{
			TEXTCONSOLEMSG_WRITE( "[ERROR]Land ID is NATIVEID NULL! charID[%s]", m_szName );
		}
	}


	m_setLandEffectNum.clear();
	


	hr = SetNavi ( pLandMan->GetNavi(), m_vPos );
	if ( FAILED(hr) )	return E_FAIL;

	m_lnLastSendExp = m_sExperience.lnNow;

	m_lnLastSendVNExp = m_lVNGainSysExp;

	m_bServerStorage = TRUE;	

	DWORD Num1 = m_cInventory.GetNumItems();
	DWORD Num2 = m_cVietnamInventory.GetNumItems ();

	m_fMoveDelay = 0.0f;
	CTime cTIME_CUR = CTime::GetCurrentTime();

	//	Note : Ã¢°íÀÇ À¯È¿¼º °Ë»ç.
	//
	for ( WORD i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i )
	{
		CTime cSTORAGE(m_tSTORAGE[i]);

		bool bVALID(true);
		if ( m_tSTORAGE[i]==0 || cSTORAGE<cTIME_CUR )	bVALID = false;
		
		m_bSTORAGE[i] = bVALID;
	}

	//	Note : ÇÁ¸®¹Ì¾ö ¿©ºÎ °Ë»ç.
	//
	bool bPREMIUM(true);
	CTime tPREMIUM(m_tPREMIUM);
	if ( m_tPREMIUM==0 || tPREMIUM<cTIME_CUR )	bPREMIUM = false;
	m_bPREMIUM = bPREMIUM;

	m_cInventory.SetAddLine ( GetOnINVENLINE(), true );
	// º£Æ®³² ÀÎº¥Åä¸®´Â ¼¼·Î°¡ 10Ä­ÀÌ¹Ç·Î 6Ä­À» ´õ Ãß°¡ÇÑ´Ù.
#if defined(VN_PARAM) //vietnamtest%%%
	m_cVietnamInventory.SetAddLine ( 6, true );
#endif


	//DoQuestCheckLeaveMap();


	SetSTATE(EM_ACT_WAITING);

	/*contribution/activity bonus, Juver, 2017/10/14 */
	//m_bUseContributionPointTimer = GLCONST_CHAR::bContributionPointBonusTimer;
	//m_fContributionPointBonusTimer = 0.0f;
	//m_fContributionPointBonusTimeMax = GLCONST_CHAR::fContributionPointBonusTime;

	//m_bUseActivityPointTimer = GLCONST_CHAR::bActivityPointBonusTimer;
	//m_fActivityPointBonusTimer = 0.0f;
	//m_fActivityPointBonusTimeMax = GLCONST_CHAR::fActivityPointBonusTime;
	
	/* Codex, Archie 02/16/24 */
	CODEX_STATS(1,1);


//#ifdef _DEBUG
//
//	/* user flag verified, Juver, 2020/02/25 */
//	TEXTCONSOLEMSG_WRITE ( "[%u]%s m_bUserFlagVerified=%d", m_dwCharID, m_szName, m_bUserFlagVerified );
//
//	/* user flag restricted, Juver, 2020/04/21 */
//	TEXTCONSOLEMSG_WRITE ( "[%u]%s m_bUserFlagRestricted=%d", m_dwCharID, m_szName, m_bUserFlagRestricted );
//
//#endif


	if ( m_pAutoPotion )
		m_pAutoPotion->Reset();

	if ( m_pAutoPilot )
		m_pAutoPilot->Reset();

	return S_OK;
}

HRESULT GLChar::SetNavi ( NavigationMesh* pNavi, D3DXVECTOR3 &vPos )
{
	m_vPos = vPos;
	if ( m_actorMove.PathIsActive() )		m_actorMove.Stop();

	m_actorMove.Create ( pNavi, vPos, -1 );
	m_actorMove.SetMaxSpeed ( GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fWALKVELO );

	return S_OK;
}

HRESULT GLChar::SetData ( PCHARDATA2 pCharData, BOOL bNEW )
{
	SCHARDATA2::Assign ( *pCharData );
	
	if ( !GLCHARLOGIC_SERVER::INIT_DATA(bNEW,TRUE) )	return E_FAIL;

	return S_OK;
}

HRESULT GLChar::SavePosition ()
{
	//	Note : Á×Àº»óÅÂÀÏ ¶§´Â ÇöÁ¦ Æ÷Áö¼ÇÀ» ÀúÀåÇÏÁö ¾Ê´Â´Ù.
	//
	if ( IsDie() )
	{
		m_sSaveMapID = NATIVEID_NULL();
	}
	//	Note : ÇöÁ¦ À§Ä¡ ÀúÀå.
	//
	else
	{
		m_sSaveMapID = m_sMapID;
		m_vSavePos = m_vPos;
	}

	return S_OK;
}

//	Note : Á÷Àü À§Ä¡¸¦ ÀúÀå.
//
HRESULT GLChar::SaveLastCall ()
{
	if ( !m_pLandMan )	return S_FALSE;

	/*pvp tyranny, Juver, 2017/08/24 */
	/*school wars, Juver, 2018/01/19 */
	/*pvp capture the flag, Juver, 2018/01/31 */
	/* pvp club death match, Juver, 2020/11/25 */
	//	Note : ¸¸¾à ¼±µµ Å¬·´Àü¿ë ¸ãÀÏ °æ¿ì Á÷Àü À§Ä¡¸¦ ÀúÀåÇÏÁö ¾ÊÀ½.
	if ( !m_pLandMan->m_bGuidBattleMap && 
		!m_pLandMan->m_bClubDeathMatchMap && 
		!m_pLandMan->m_bPVPTyrannyMap && 
		!m_pLandMan->m_bPVPSchoolWarsMap && 
		!m_pLandMan->m_bPVPCaptureTheFlagMap &&
		!m_pLandMan->m_bPVPClubDeathMatchLobbyMap && 
		!m_pLandMan->m_bPVPPBGLobbyMap &&
		!m_pLandMan->m_bPVPPBGBattleMap &&
		!m_pLandMan->m_bPVPWoeMap )
	{
		m_sLastCallMapID = m_sMapID;
		m_vLastCallPos = m_vPos;

		GLMSG::SNETPC_UPDATE_LASTCALL NetMsg;
		NetMsg.sLastCallMapID = m_sLastCallMapID;
		NetMsg.vLastCallPos = m_vLastCallPos;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
	}

	return S_OK;
}

HRESULT GLChar::LoadStorageFromDB ()
{
	if ( m_bServerStorage )	return S_OK;

	GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan();
	if ( pDBMan )	pDBMan->ReadUserInven ( &GetCharData2() );
	m_bServerStorage = TRUE;

	return S_OK;
}

HRESULT GLChar::SetPosition ( D3DXVECTOR3 &vPos )
{
	m_actorMove.SetPosition ( vPos, -1 );
	if ( m_actorMove.PathIsActive() )		m_actorMove.Stop();

	m_vPos = m_actorMove.Position();

	return S_OK;
}

BOOL GLChar::IsValidBody () const
{
	if ( IsSTATE(EM_ACT_DIE) )			return FALSE;
	if ( IsSTATE(EM_ACT_WAITING) )		return FALSE;
	if ( IsSTATE(EM_REQ_LOGOUT) )		return FALSE;

	return ( m_Action < GLAT_FALLING );
}

BOOL GLChar::IsDie () const
{
	if ( !IsSTATE(EM_ACT_WAITING) )
	{
		if ( m_Action==GLAT_DIE )		return TRUE;
		if ( IsSTATE(EM_ACT_DIE) )		return TRUE;
	}

	return FALSE;
}

/* skill turn/find invisible, Juver, 2020/04/26 */
BOOL GLChar::IsVisibleDetect ( const bool bSkillShowInvisible, const WORD wSkillShowInvisibleLevel, const WORD wSkillShowInvisibleRange, const D3DXVECTOR3& vPos )
{
	if ( IsSTATE(EM_REQ_VISIBLENONE) || IsSTATE(EM_REQ_VISIBLEOFF) )	return FALSE;

	/* skill turn/find invisible, Juver, 2020/04/26 */
	if ( m_bSkillTurnInvisible )
	{
		if ( !bSkillShowInvisible )		
			return FALSE;

		if ( m_wSkillTurnInvisibleLevel > wSkillShowInvisibleLevel )
			return FALSE;

		WORD wDistance = WORD(D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vPos) ));
		if ( wDistance > wSkillShowInvisibleRange )			
			return FALSE;
	}

	return TRUE;
}

BOOL GLChar::IsVaildTradeData ()
{
	//	±Ý¾× À¯È¿¼º °Ë»ç.
	if ( m_lnMoney < m_sTrade.GetMoney() )		return FALSE;

	GLInventory &sTradeBox = m_sTrade.GetTradeBox();
	GLInventory::CELL_MAP* pItemList = sTradeBox.GetItemList();

	GLInventory::CELL_MAP_ITER iter = pItemList->begin();
	GLInventory::CELL_MAP_ITER iter_end = pItemList->end();
	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pTradeItem = (*iter).second;
		SINVENITEM* pInvenItem = m_cInventory.GetItem ( pTradeItem->wBackX, pTradeItem->wBackY );
		if ( !pInvenItem )														return FALSE;

		if ( memcmp ( &pTradeItem->sItemCustom, &pInvenItem->sItemCustom, sizeof(SITEMCUSTOM) ) )	return FALSE;
	}

	return TRUE;
}

BOOL GLChar::IsVaildTradeInvenSpace ( GLInventory &sTradeBoxTar )
{
	GLInventory sInvenTemp;
	sInvenTemp.SetAddLine ( m_cInventory.GETAddLine(), true );
	sInvenTemp.Assign ( m_cInventory );

	//	Note : ÀÚ½ÅÀÇ ±³È¯ ¸ñ·Ï¿¡ ¿Ã¶ó°£ ¾ÆÀÌÅÛÀ» ÀÎ¹ê¿¡¼­ Á¦¿Ü.
	//
	GLInventory &sTradeBoxMy = m_sTrade.GetTradeBox();
	GLInventory::CELL_MAP* pItemListMy = sTradeBoxMy.GetItemList();

	GLInventory::CELL_MAP_ITER iter = pItemListMy->begin();
	GLInventory::CELL_MAP_ITER iter_end = pItemListMy->end();
	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pTradeItem = (*iter).second;

		BOOL bOk = sInvenTemp.DeleteItem ( pTradeItem->wBackX, pTradeItem->wBackY );
		if ( !bOk )		return FALSE;
	}

	//	Note : »ó´ë¹æÀÇ ±³È¯ ¹°Ç°ÀÌ µé¾î °¥¼ö ÀÖ´ÂÁö °Ë»ç.
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

BOOL GLChar::DoTrade ( GLTrade &sTrade, DWORD dwFromCharID )
{

	LONGLONG lnMoneyOut = m_sTrade.GetMoney();
	if ( m_lnMoney < lnMoneyOut ) return FALSE;
	// Anti CE Trade GoldBug
	if ( m_lnMoney < 0 || lnMoneyOut < 0 ) return FALSE;	
	// Anti CE Trade GoldBug
	CheckMoneyUpdate( m_lnMoney, lnMoneyOut, FALSE, "[Buy] Do Trade Out Money." );
	// Anti CE Trade GoldBug
	if ( m_lnMoney < 0 || lnMoneyOut < 0 )
		return FALSE;
	else
		m_lnMoney -= lnMoneyOut;



	LONGLONG lnMoneyIn = sTrade.GetMoney();
	// Anti CE Trade GoldBug
	if ( m_lnMoney < 0 || lnMoneyIn < 0 ) return FALSE;		

	CheckMoneyUpdate( m_lnMoney, lnMoneyIn, TRUE, "[Sell] Do Trade In Money." );
	
	//Anti - Cheat Trade Bug
	if ( m_lnMoney < 0 || lnMoneyIn < 0 )
	{
		return FALSE;
		m_bMoneyUpdate = TRUE;
	}
	else
	{
		m_lnMoney += lnMoneyIn;

		m_bMoneyUpdate = TRUE;
	}

	// Trade Gold Announcement, JDevelopments, 6/29/2025
	PGLCHAR pFromChar = m_pGLGaeaServer->GetCharID(dwFromCharID);
	if (lnMoneyIn > 0 && pFromChar && pFromChar->m_szName[0] != '\0' && m_szName[0] != '\0')
	{
		if (lnMoneyIn > 250000)
		{
			CString strText;
			strText.Format("[%s] successfully traded %I64d gold to [%s]!", 
				pFromChar->m_szName, lnMoneyIn, m_szName);
	
			if (strText.GetLength() < 255)
			{
				//CDebugSet::ToFileWithTime("_trade_gold.txt", "[%d]%s to [%d]%s | Amount: %I64d", 
				RLOG::LogFile("_trade_gold.txt", "[%d]%s to [%d]%s | Amount: %I64d", 
					pFromChar->m_dwCharID, pFromChar->m_szName, m_dwCharID, m_szName, lnMoneyIn);
	
				GLMSG::SNET_SERVER_GENERALCHAT SendData;
				SendData.SETTEXT(strText.GetString());
				SendData.bNotify = true;
				SendData.iColor = 6;
				GLGaeaServer::GetInstance().SENDTOALLCLIENT(&SendData);
			}
		}
	}


	//
	LONGLONG lnMoneyDx = (lnMoneyIn-lnMoneyOut);
	if ( lnMoneyDx!=0 )
	{
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, dwFromCharID, ID_CHAR, m_dwCharID, lnMoneyDx, EMITEM_ROUTE_CHAR );
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
	}

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if ( m_bTracingUser )
	{
		NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
		TracingMsg.nUserNum  = GetUserID();
		StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

		CString strTemp;
		PGLCHAR pFromChar = m_pGLGaeaServer->GetCharID( dwFromCharID );
		if ( pFromChar != NULL )
		{
			strTemp.Format( "Do Trade! From, [%s][%s], Target[%s][%s], RecvMoney:[%I64d], SendMoney:[%I64d], HaveMoney:[%I64d], Target HaveMoney:[%I64d]",
					 m_szUID, m_szName, pFromChar->m_szUID, pFromChar->m_szName, lnMoneyIn, lnMoneyOut, m_lnMoney, pFromChar->m_lnMoney );
		}else{
			strTemp.Format( "Do Trade! From, [%s][%s], Target Not Found, RecvMoney:[%I64d], SendMoney:[%I64d], HaveMoney:[%I64d]",
					 m_szUID, m_szName, lnMoneyIn, lnMoneyOut, m_lnMoney );
		}

		StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

		m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
	}
#endif

	//	Note : �ݾ� ��ȭ Ŭ���̾�Ʈ�� �˸�.
	//
	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );



	GLInventory &sTradeBoxMy = m_sTrade.GetTradeBox();
	GLInventory::CELL_MAP* pItemListMy = sTradeBoxMy.GetItemList();
	
	SINVENITEM* pTradeItem = NULL;
	BOOL bOk = FALSE;
	GLMSG::SNETPC_INVEN_DELETE NetMsgDelete;
	GLInventory::CELL_MAP_ITER iter = pItemListMy->begin();
	GLInventory::CELL_MAP_ITER iter_end = pItemListMy->end();
	for ( ; iter!=iter_end; ++iter )
	{
		pTradeItem = (*iter).second;

		bOk = m_cInventory.DeleteItem ( pTradeItem->wBackX, pTradeItem->wBackY );
		if ( !bOk )
		{
			return FALSE;
		}
		else
		{
			NetMsgDelete.wPosX = pTradeItem->wBackX;
			NetMsgDelete.wPosY = pTradeItem->wBackY;
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgDelete );
		}
	}


	GLInventory::CELL_MAP* pItemListTar = sTrade.GetTradeBox().GetItemList();

	pTradeItem = NULL;
	SITEM* pItem = NULL;
	WORD wPosX = 0, wPosY = 0;

	SINVENITEM* pInvenItem = NULL;
	GLMSG::SNETPC_INVEN_INSERT NetMsgInert;

	iter = pItemListTar->begin();
	iter_end = pItemListTar->end();	
	for ( ; iter!=iter_end; ++iter )
	{
		pTradeItem = (*iter).second;
		pItem = GLItemMan::GetInstance().GetItem ( pTradeItem->sItemCustom.sNativeID );
		if ( !pItem )	return FALSE;

		wPosX = 0;
		wPosY = 0;
		bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )		return FALSE;

		bOk = m_cInventory.InsertItem ( pTradeItem->sItemCustom, wPosX, wPosY );
		if ( !bOk )		return FALSE;



		pInvenItem = m_cInventory.GetItem ( wPosX, wPosY );

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, dwFromCharID, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInvenItem->sItemCustom.wTurnNum );

		//	Note : ������ ��ȭ�� Ŭ���̾�Ʈ�� �˸�.
		//		
		NetMsgInert.Data = *pInvenItem;
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgInert );
	}

	return TRUE;
}
// *****************************************************
// Desc: ½Ã°£Á¦ÇÑ ¾ÆÀÌÅÛ ¾÷µ¥ÀÌÆ®
// *****************************************************
void GLChar::RESET_TIMELMT_ITEM ()
{
	std::vector<GLInventory::CELL_KEY>	vecDELETE;

	const CTime cTIME_CUR = CTime::GetCurrentTime();

	SINVENITEM* pInvenItem = NULL;
	SITEM* pITEM = NULL;
	GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
	GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;

	// ÆÖ ¾ÆÀÌÅÛ ½½·Ô
	PGLPETFIELD pMyPet = m_pGLGaeaServer->GetPET ( m_dwPetGUID );
	if ( pMyPet && pMyPet->IsValid () ) pMyPet->UpdateTimeLmtItem ( this );

	// Å»°Í ¾ÆÀÌÅÛ ½½·Ô 
	if ( m_sVehicle.IsActiveValue() ) m_sVehicle.UpdateTimeLmtItem( this );

//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;
	GLITEMLMT & glItemmt = GLITEMLMT::GetInstance();
	GLItemMan & glItemMan = GLItemMan::GetInstance();
	
	// ÀÎº¥Åä¸® ¾ÆÀÌÅÛ
	GLInventory::CELL_MAP* pMapItem = m_cInventory.GetItemList();
	GLInventory::CELL_MAP_CITER iter = pMapItem->begin();
	GLInventory::CELL_MAP_CITER iter_end = pMapItem->end();
	for ( ; iter!=iter_end; ++iter )
	{
		pInvenItem = (*iter).second;
		pITEM = glItemMan.GetItem ( pInvenItem->sItemCustom.sNativeID );
		if ( !pITEM )	continue;

		bool bDELETE(false);

		// ½ÃÇÑºÎ ¾ÆÀÌÅÛ
		if ( pITEM->IsTIMELMT() )
		{
			CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
			CTime cTIME_LMT(pInvenItem->sItemCustom.tBORNTIME);
			cTIME_LMT += cSPAN;

			if ( cTIME_CUR > cTIME_LMT )
			{
				vecDELETE.push_back ( (*iter).first );
				bDELETE = true;

				if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && pInvenItem->sItemCustom.dwVehicleID != 0 )
				{
					// ¾Ç¼¼»ç¸®¸¦ Ã¼Å©ÈÄ ÆÖÀ» DB¿¡¼­ »èÁ¦ÇØÁØ´Ù.
					GLVEHICLE* pNewVehicle = new GLVEHICLE();
					CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
													 pInvenItem->sItemCustom.dwVehicleID, 
													 m_dwClientID, 
													 m_dwCharID, 
													 true );
					GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );				

				}

				// ÆÖÄ«µåÀÏ °æ¿ì
				if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD && pInvenItem->sItemCustom.dwPetID != 0 )
				{
					// È°µ¿ÁßÀÌ¸é »ç¶óÁö°Ô ÇØÁØ´Ù.
					if ( pMyPet && pInvenItem->sItemCustom.dwPetID == pMyPet->m_dwPetID ) 
					{
						m_pGLGaeaServer->ReserveDropOutPet ( SDROPOUTPETINFO(pMyPet->m_dwGUID,true,false) );

						// ÆÖÀÇ ¾Ç¼¼»ç¸®¸¦ °Ë»çÇÑ´Ù.
						pMyPet->UpdateTimeLmtItem ( this );

						// ³Ö¾îÁÙ²¨ ³Ö¾îÁØ´Ù.
						for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
						{
							CItemDrop cDropItem;
							cDropItem.sItemCustom = pMyPet->m_PutOnItems[i];
							if ( IsInsertToInven ( &cDropItem ) ) InsertToInven ( &cDropItem );
							else
							{
								if ( m_pLandMan )
								{
									m_pLandMan->DropItem ( m_vPos, 
														   &(cDropItem.sItemCustom), 
														   EMGROUP_ONE, 
														   m_dwGaeaID );
								}
							}
						}

						CDeletePet* pDeletePet = new CDeletePet ( m_dwCharID, pInvenItem->sItemCustom.dwPetID );
						GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
						if ( pDBMan ) pDBMan->AddJob ( pDeletePet );
						
						// ÆÖÀÌ »èÁ¦µÇ¸é ºÎÈ°Á¤º¸¸¦ Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë·ÁÁÜ.
						CGetRestorePetList *pGetRestorePetList = new CGetRestorePetList ( m_dwCharID, m_dwClientID );
						if ( pDBMan ) pDBMan->AddJob ( pGetRestorePetList );
					}
					else
					{
						// ¾Ç¼¼»ç¸®¸¦ Ã¼Å©ÈÄ ÆÖÀ» DB¿¡¼­ »èÁ¦ÇØÁØ´Ù.
						GLPET* pNewPet = new GLPET ();
						CGetPet* pGetPet = new CGetPet ( pNewPet, 
														 pInvenItem->sItemCustom.dwPetID, 
														 m_dwClientID, 
														 m_dwCharID,
														 pInvenItem->wPosX,
														 pInvenItem->wPosY,
														 true );
						GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
						if ( pDBMan ) pDBMan->AddJob ( pGetPet );
					}
				}
			}
		}

		// ÄÚ½ºÆ¬
		if ( !bDELETE && pInvenItem->sItemCustom.nidDISGUISE!=SNATIVEID(false) )
		{
			if ( pInvenItem->sItemCustom.tDISGUISE != 0 )
			{
				CTime cTIME_LMT(pInvenItem->sItemCustom.tDISGUISE);
				if ( cTIME_LMT.GetYear()!=1970 )
				{
					if ( cTIME_CUR > cTIME_LMT )
					{
						//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ¾Ë¸².						
						NetMsgInvenDelTimeLmt.nidITEM = pInvenItem->sItemCustom.nidDISGUISE;
						m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

						//	Note : ÄÚ½ºÅù Á¤º¸ ¸®¼Â.
						pInvenItem->sItemCustom.tDISGUISE = 0;
						pInvenItem->sItemCustom.nidDISGUISE = SNATIVEID(false);

						/*item color, Juver, 2018/01/08 */
						SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
						if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
						{
							pInvenItem->sItemCustom.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
							pInvenItem->sItemCustom.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
						}
						else
						{
							pInvenItem->sItemCustom.wColor1 = ITEMCOLOR_WHITE;
							pInvenItem->sItemCustom.wColor2 = ITEMCOLOR_WHITE;
						}

						//	Note : Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë¸².						
						NetItemUpdate.wPosX = pInvenItem->wPosX;
						NetItemUpdate.wPosY = pInvenItem->wPosY;
						NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
						m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

						//	Note : ·Î±× ÀúÀå.
						glItemmt.ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );
					}
				}
			}
		}
	}

	if ( !vecDELETE.empty() )
	{
		for ( DWORD i=0; i<vecDELETE.size(); ++i )
		{
			const GLInventory::CELL_KEY &cKEY = vecDELETE[i];
			SINVENITEM sINVENITEM = *m_cInventory.GetItem(cKEY.first,cKEY.second);

			SNATIVEID nidITEM = sINVENITEM.sItemCustom.sNativeID;

			//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ·Î±× ³²±è.
			glItemmt.ReqItemRoute ( sINVENITEM.sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, sINVENITEM.sItemCustom.wTurnNum );

			//	Note : ¾ÆÀÌÅÛ »èÁ¦.
			//
			m_cInventory.DeleteItem ( cKEY.first, cKEY.second );

			//	[ÀÚ½Å¿¡°Ô] ÇØ´ç ¾ÆÀÌÅÆÀ» ÀÎ¹ê¿¡¼­ »èÁ¦.
			GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
			NetMsgInvenDel.wPosX = cKEY.first;
			NetMsgInvenDel.wPosY = cKEY.second;
			m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

			//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ¾Ë¸².
			GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
			NetMsgInvenDelTimeLmt.nidITEM = nidITEM;
			m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

			//	Note : ÄÚ½ºÅù º¹±¸.
			if ( sINVENITEM.sItemCustom.nidDISGUISE!=SNATIVEID(false) )
			{
				SITEM *pONE = glItemMan.GetItem ( sINVENITEM.sItemCustom.nidDISGUISE );
				if ( !pONE )		continue;

				//	ÀÎº¥ À§Ä¡.
				WORD wInsertPosX = cKEY.first, wInsertPosY = cKEY.second;

				SITEMCUSTOM sITEM_NEW;
				sITEM_NEW.sNativeID = sINVENITEM.sItemCustom.nidDISGUISE;
				CTime cTIME = CTime::GetCurrentTime();
				if ( sINVENITEM.sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
				{
					cTIME = CTime(sINVENITEM.sItemCustom.tDISGUISE);

					CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
					cTIME -= tLMT;
				}

				//	Note : ¾ÆÀÌÅÛ »ý¼º.
				//
				sITEM_NEW.tBORNTIME = cTIME.GetTime();
				sITEM_NEW.wTurnNum = 1;
				sITEM_NEW.cGenType = EMGEN_ACTION_SYSTEM;
				sITEM_NEW.cChnID = (BYTE)m_pGLGaeaServer->GetServerChannel();
				sITEM_NEW.cFieldID = (BYTE)m_pGLGaeaServer->GetFieldSvrID();
				sITEM_NEW.lnGenNum = glItemmt.RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

				/*item color, Juver, 2018/01/08 */
				sITEM_NEW.wColor1 = sINVENITEM.sItemCustom.wColor1;
				sITEM_NEW.wColor2 = sINVENITEM.sItemCustom.wColor2;

				//	Note : ÀÎº¥¿¡ µé¾î°¥ °ø°£ °Ë»ç.
				BOOL bOk = m_cInventory.IsInsertable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
				if ( !bOk )		continue;

				//	Note : ÀÎº¥¿¡ ³ÖÀ½.
				m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
				SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

				GLMSG::SNETPC_INVEN_INSERT NetItemInsert;
				NetItemInsert.Data = *pINSERT_ITEM;
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetItemInsert);

				//	Note : ºÐ¸®µÈ ÄÚ½ºÅù ·Î±×.
				glItemmt.ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );
			}
		}
	}

	// ¾ÆÀÌÅÛ ½½·Ô Á¤º¸ °»½Å
	for ( int i=0; i<SLOT_TSIZE; ++i )
	{
		EMSLOT emSLOT = (EMSLOT) i;
		if( m_PutOnItems[emSLOT].sNativeID==NATIVEID_NULL() ) continue;

		SITEMCUSTOM sCUSTOM = m_PutOnItems[emSLOT];

		SITEM *pITEM = glItemMan.GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )								continue;

		bool bDELETE(false);

		if ( pITEM->IsTIMELMT() )
		{
			CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
			CTime cTIME_LMT(sCUSTOM.tBORNTIME);
			cTIME_LMT += cSPAN;

			if ( cTIME_CUR > cTIME_LMT )
			{
				//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ·Î±× ³²±è.
				glItemmt.ReqItemRoute ( sCUSTOM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, sCUSTOM.wTurnNum );

				//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ¾Ë¸².
				GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
				NetMsgInvenDelTimeLmt.nidITEM = sCUSTOM.sNativeID;
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

				if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && sCUSTOM.dwVehicleID != 0 )
				{
					m_pGLGaeaServer->SaveVehicle( m_dwClientID, m_dwGaeaID, false );

                	// ¾Ç¼¼»ç¸®¸¦ Ã¼Å©ÈÄ ÆÖÀ» DB¿¡¼­ »èÁ¦ÇØÁØ´Ù.
					GLVEHICLE* pNewVehicle = new GLVEHICLE();
					CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
													 sCUSTOM.dwVehicleID, 
													 m_dwClientID, 
													 m_dwCharID, 
													 true );

					GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );
				}

				//	¾ÆÀÌÅÛ Á¦°Å.
				RELEASE_SLOT_ITEM ( emSLOT );

				//	[ÀÚ½Å¿¡°Ô]  ¾ÆÀÌÅÆ Á¦°Å.
				GLMSG::SNETPC_PUTON_RELEASE NetMsg_Release(emSLOT);
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg_Release);

				//	Note : ÁÖº¯ »ç¶÷µé¿¡°Ô ¾ÆÀÌÅÆ Á¦°Å.
				GLMSG::SNETPC_PUTON_RELEASE_BRD NetMsgReleaseBrd;
				NetMsgReleaseBrd.dwGaeaID = m_dwGaeaID;
				NetMsgReleaseBrd.emSlot = emSLOT;
				SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgReleaseBrd) );

				bDELETE = true;

				//	Note : ÄÚ½ºÅù º¹±¸.
				if ( sCUSTOM.nidDISGUISE!=SNATIVEID(false) )
				{
					SITEM *pONE = glItemMan.GetItem ( sCUSTOM.nidDISGUISE );
					if ( !pONE )		continue;

					SITEMCUSTOM sITEM_NEW;
					sITEM_NEW.sNativeID = sCUSTOM.nidDISGUISE;
					CTime cTIME = CTime::GetCurrentTime();
					if ( sCUSTOM.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
					{
						cTIME = CTime(sCUSTOM.tDISGUISE);

						CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
						cTIME -= tLMT;
					}

					//	Note : ¾ÆÀÌÅÛ »ý¼º.
					//
					sITEM_NEW.tBORNTIME = cTIME.GetTime();
					sITEM_NEW.wTurnNum = 1;
					sITEM_NEW.cGenType = EMGEN_ACTION_SYSTEM;
					sITEM_NEW.cChnID = (BYTE)m_pGLGaeaServer->GetServerChannel();
					sITEM_NEW.cFieldID = (BYTE)m_pGLGaeaServer->GetFieldSvrID();
					sITEM_NEW.lnGenNum = glItemmt.RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

					/*item color, Juver, 2018/01/08 */
					sITEM_NEW.wColor1 = sCUSTOM.wColor1;
					sITEM_NEW.wColor2 = sCUSTOM.wColor2;

					//	Note : º¹Àå Âø¿ë.
					SLOT_ITEM ( sITEM_NEW, emSLOT );

					//	Note : ÀÚ½Å¿¡°Ô Âø¿ë¾ÆÀÌÅÛ º¯°æ.
					GLMSG::SNETPC_PUTON_UPDATE NetMsgUpdate;
					NetMsgUpdate.emSlot = emSLOT;
					NetMsgUpdate.sItemCustom = sITEM_NEW;
					m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgUpdate);

					//	Note : ÁÖº¯ »ç¶÷µé¿¡°Ô Âø¿ë¾ÆÀÌÅÛ º¯°æ.
					GLMSG::SNETPC_PUTON_UPDATE_BRD NetMsgUpdateBrd;
					NetMsgUpdateBrd.dwGaeaID = m_dwGaeaID;
					NetMsgUpdateBrd.emSlot = emSLOT;
					NetMsgUpdateBrd.sItemClient.Assign ( sITEM_NEW );
					SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgUpdateBrd) );

					//	Note : ºÐ¸®µÈ ÄÚ½ºÅù ·Î±×.
					glItemmt.ReqItemRoute ( sITEM_NEW, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sITEM_NEW.wTurnNum );
				}
			}
		}
		
		if ( !bDELETE && VALID_SLOT_ITEM(emSLOT) && sCUSTOM.nidDISGUISE!=SNATIVEID(false) )
		{
			if ( sCUSTOM.tDISGUISE != 0 )
			{
				CTime cTIME_LMT(sCUSTOM.tDISGUISE);
				if ( cTIME_LMT.GetYear()!=1970 )
				{
					if ( cTIME_CUR > cTIME_LMT )
					{
						//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ¾Ë¸².
						GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
						NetMsgInvenDelTimeLmt.nidITEM = sCUSTOM.nidDISGUISE;
						m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

						//	Note : ÄÚ½ºÅù Á¤º¸ ¸®¼Â.
						sCUSTOM.tDISGUISE = 0;
						sCUSTOM.nidDISGUISE = SNATIVEID(false);
						SLOT_ITEM ( sCUSTOM, emSLOT );			//	½ÇÁ¦·Î Àû¿ë.

						//	Note : ÀÚ½Å¿¡°Ô Âø¿ë¾ÆÀÌÅÛ º¯°æ.
						GLMSG::SNETPC_PUTON_UPDATE NetMsgUpdate;
						NetMsgUpdate.emSlot = emSLOT;
						NetMsgUpdate.sItemCustom = sCUSTOM;
						m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgUpdate);

						//	Note : ÁÖº¯ »ç¶÷µé¿¡°Ô Âø¿ë¾ÆÀÌÅÛ º¯°æ.
						GLMSG::SNETPC_PUTON_UPDATE_BRD NetMsgUpdateBrd;
						NetMsgUpdateBrd.dwGaeaID = m_dwGaeaID;
						NetMsgUpdateBrd.emSlot = emSLOT;
						NetMsgUpdateBrd.sItemClient.Assign ( sCUSTOM );
						SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgUpdateBrd) );
					}
				}
			}
		}
	}

	//	Note : ÀÎº¥Åä¸® Ãß°¡¶óÀÎ À¯È¿¼º °Ë»ç.
	//
	GLMSG::SNETPC_STORAGE_STATE NetMsgStorageState;
	for ( WORD i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i )
	{
		bool bVALID(true);
		CTime cSTORAGE(m_tSTORAGE[i]);

		if ( m_tSTORAGE[i]==0 )		bVALID = false;
		if ( cSTORAGE<cTIME_CUR )
		{
			bVALID = false;
		}

		m_bSTORAGE[i] = bVALID;
		NetMsgStorageState.bVALID[i] = bVALID;
	}
	m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgStorageState);

	bool bPREMIUM(true);
	CTime cPREMIUM(m_tPREMIUM);
	if ( m_tPREMIUM==0 )			bPREMIUM = false;
	if ( cPREMIUM<cTIME_CUR )
	{
		bPREMIUM = false;
	}

	m_bPREMIUM = bPREMIUM;

	GLMSG::SNETPC_PREMIUM_STATE NetMsgPremium;
	NetMsgPremium.bPREMIUM = bPREMIUM;
	m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgPremium);

	//	Note : ÇöÀç È°¼ºÈ­µÈ ÀÎº¥ ¶óÀÎ ¼³Á¤.
	//
	m_cInventory.SetAddLine ( GetOnINVENLINE(), true );

	// Ã¢°í ¾ÆÀÌÅÛ À¯È¿¼º °»½Å
	for ( WORD idx = 0; idx < EMSTORAGE_CHANNEL; ++idx )
	{
		GLInventory::CELL_MAP* pMapItem = m_cStorage[idx].GetItemList();
		GLInventory::CELL_MAP_CITER iter = pMapItem->begin();
		GLInventory::CELL_MAP_CITER iter_end = pMapItem->end();

		vecDELETE.clear();

		for ( ; iter!=iter_end; ++iter )
		{
			SINVENITEM *pInvenItem = (*iter).second;

			SITEM *pITEM = glItemMan.GetItem ( pInvenItem->sItemCustom.sNativeID );
			if ( !pITEM )								continue;

			bool bDELETE(false);

			// ½ÃÇÑºÎ ¾ÆÀÌÅÛ
			if ( pITEM->IsTIMELMT() )
			{
				CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
				CTime cTIME_LMT(pInvenItem->sItemCustom.tBORNTIME);
				cTIME_LMT += cSPAN;

				if ( cTIME_CUR > cTIME_LMT )
				{
					vecDELETE.push_back ( (*iter).first );
					bDELETE = true;

					// ÆÖÄ«µåÀÏ °æ¿ì
					if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD && pInvenItem->sItemCustom.dwPetID != 0 )
					{
						// ¾Ç¼¼»ç¸®¸¦ Ã¼Å©ÈÄ ÆÖÀ» DB¿¡¼­ »èÁ¦ÇØÁØ´Ù.
						GLPET* pNewPet = new GLPET ();
						CGetPet* pGetPet = new CGetPet ( pNewPet, 
														 pInvenItem->sItemCustom.dwPetID, 
														 m_dwClientID, 
														 m_dwCharID,
														 pInvenItem->wPosX,
														 pInvenItem->wPosY,
														 true );
						GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
						if ( pDBMan ) pDBMan->AddJob ( pGetPet );
					}

					// Å»°ÍÀÏ °æ¿ì
					if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && pInvenItem->sItemCustom.dwVehicleID != 0 )
					{
						// ¾Ç¼¼»ç¸®¸¦ Ã¼Å©ÈÄ Å»°ÍÀ» DB¿¡¼­ »èÁ¦ÇØÁØ´Ù.
						GLVEHICLE* pNewVehicle = new GLVEHICLE();
						CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
													 pInvenItem->sItemCustom.dwVehicleID, 
													 m_dwClientID,
													 m_dwCharID, 
													 true );
						GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
						if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );		
					}
				}
			}
			
			// ÄÚ½ºÆ¬
			if ( !bDELETE && pInvenItem->sItemCustom.nidDISGUISE!=SNATIVEID(false) )
			{
				if ( pInvenItem->sItemCustom.tDISGUISE != 0 )
				{
					CTime cTIME_LMT(pInvenItem->sItemCustom.tDISGUISE);
					if ( cTIME_LMT.GetYear()!=1970 )
					{
						if ( cTIME_CUR > cTIME_LMT )
						{
							//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ¾Ë¸².
							GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
							NetMsgInvenDelTimeLmt.nidITEM = pInvenItem->sItemCustom.nidDISGUISE;
							m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

							//	Note : ÄÚ½ºÅù Á¤º¸ ¸®¼Â.
							pInvenItem->sItemCustom.tDISGUISE = 0;
							pInvenItem->sItemCustom.nidDISGUISE = SNATIVEID(false);

							/*item color, Juver, 2018/01/08 */
							SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
							if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
							{
								pInvenItem->sItemCustom.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
								pInvenItem->sItemCustom.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
							}
							else
							{
								pInvenItem->sItemCustom.wColor1 = ITEMCOLOR_WHITE;
								pInvenItem->sItemCustom.wColor2 = ITEMCOLOR_WHITE;
							}

							//	Note : Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë¸².
							GLMSG::SNETPC_STORAGE_ITEM_UPDATE NetItemUpdate;
							NetItemUpdate.dwChannel = idx;
							NetItemUpdate.wPosX = pInvenItem->wPosX;
							NetItemUpdate.wPosY = pInvenItem->wPosY;
							NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
							m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

							//	Note : ·Î±× ÀúÀå.
							glItemmt.ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );
						}
					}
				}
			}
		}

		if ( !vecDELETE.empty() )
		{
			for ( DWORD i=0; i<vecDELETE.size(); ++i )
			{
				const GLInventory::CELL_KEY &cKEY = vecDELETE[i];
				SINVENITEM sINVENITEM = *m_cStorage[idx].GetItem(cKEY.first,cKEY.second);

				SNATIVEID nidITEM = sINVENITEM.sItemCustom.sNativeID;

				//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ·Î±× ³²±è.
				glItemmt.ReqItemRoute ( sINVENITEM.sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, sINVENITEM.sItemCustom.wTurnNum );

				//	Note : ¾ÆÀÌÅÛ »èÁ¦.
				//
				m_cStorage[idx].DeleteItem ( cKEY.first, cKEY.second );

				//	[ÀÚ½Å¿¡°Ô] ÇØ´ç ¾ÆÀÌÅÆÀ» ÀÎ¹ê¿¡¼­ »èÁ¦.
				GLMSG::SNETPC_STORAGE_DELETE NetMsgStorageDel;
				NetMsgStorageDel.dwChannel = idx;
				NetMsgStorageDel.wPosX = cKEY.first;
				NetMsgStorageDel.wPosY = cKEY.second;
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgStorageDel);

				//	½Ã°£ Á¦ÇÑÀ¸·Î ¾ÆÀÌÅÛ »èÁ¦ ¾Ë¸².
				GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
				NetMsgInvenDelTimeLmt.nidITEM = nidITEM;
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

				//	Note : ÄÚ½ºÅù º¹±¸.
				if ( sINVENITEM.sItemCustom.nidDISGUISE!=SNATIVEID(false) )
				{
					SITEM *pONE = glItemMan.GetItem ( sINVENITEM.sItemCustom.nidDISGUISE );
					if ( !pONE )		continue;

					//	ÀÎº¥ À§Ä¡.
					WORD wInsertPosX = cKEY.first, wInsertPosY = cKEY.second;

					SITEMCUSTOM sITEM_NEW;
					sITEM_NEW.sNativeID = sINVENITEM.sItemCustom.nidDISGUISE;
					CTime cTIME = CTime::GetCurrentTime();
					if ( sINVENITEM.sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
					{
						cTIME = CTime(sINVENITEM.sItemCustom.tDISGUISE);

						CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
						cTIME -= tLMT;
					}

					//	Note : ¾ÆÀÌÅÛ »ý¼º.
					//
					sITEM_NEW.tBORNTIME = cTIME.GetTime();
					sITEM_NEW.wTurnNum = 1;
					sITEM_NEW.cGenType = EMGEN_ACTION_SYSTEM;
					sITEM_NEW.cChnID = (BYTE)m_pGLGaeaServer->GetServerChannel();
					sITEM_NEW.cFieldID = (BYTE)m_pGLGaeaServer->GetFieldSvrID();
					sITEM_NEW.lnGenNum = glItemmt.RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

					/*item color, Juver, 2018/01/08 */
					sITEM_NEW.wColor1 = sINVENITEM.sItemCustom.wColor1;
					sITEM_NEW.wColor2 = sINVENITEM.sItemCustom.wColor2;

					//	Note : ÀÎº¥¿¡ µé¾î°¥ °ø°£ °Ë»ç.
					BOOL bOk = m_cStorage[idx].IsInsertable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
					if ( !bOk )		continue;

					//	Note : ÀÎº¥¿¡ ³ÖÀ½.
					m_cStorage[idx].InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
					SINVENITEM *pINSERT_ITEM = m_cStorage[idx].GetItem ( wInsertPosX, wInsertPosY );

					GLMSG::SNETPC_STORAGE_INSERT NetItemInsert;
					NetItemInsert.dwChannel = idx;
					NetItemInsert.Data = *pINSERT_ITEM;
					m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetItemInsert);

					//	Note : ºÐ¸®µÈ ÄÚ½ºÅù ·Î±×.
					glItemmt.ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );
				}
			}
		}
	}
}

void GLChar::RESET_TIMELMT_SKILL()
{
	const CTime cTIME_CUR = CTime::GetCurrentTime();

	SITEM* pITEM = NULL;
	GLItemMan & glItemMan = GLItemMan::GetInstance();

	SKILL_MAP_CITER iter = m_ExpSkills.begin ();
	SKILL_MAP_CITER iter_end = m_ExpSkills.end ();

	for ( ; iter!=iter_end; ++iter )
	{
		const SCHARSKILL &sSkill = (*iter).second;
		
		if ( sSkill.tBORNTIME != 0 )
		{
			pITEM = glItemMan.GetItem ( sSkill.sItemID );
			if ( !pITEM )	continue;

			CTimeSpan cSPAN( pITEM->sDrugOp.tTIME_LMT );
			CTime cTIME_LMT( sSkill.tBORNTIME );
			cTIME_LMT += cSPAN;

			if ( cTIME_CUR > cTIME_LMT )
			{
				SKILL_MAP_ITER iter_del = m_ExpSkills.find ( sSkill.sNativeID.dwID );
				m_ExpSkills.erase( iter_del );

				GLMSG::SNETPC_SKILL_EXPIRE_FB	NetMsgFB;
				NetMsgFB.sSkillID = sSkill.sNativeID;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			}
		}
	}
}

// *****************************************************
// Desc: ¾ÆÀÌÅÛ Á¶°Ç Ã¼Å© ( ½Ã°£Á¦ÇÑ°ú °°ÀÌ ÀÛµ¿ ) 
// *****************************************************
void GLChar::RESET_CHECK_ITEM ()
{

	// ¾ÆÀÌÅÛ ½½·Ô Âø¿ë Á¶°ÇÀ» Ã¼Å©ÇÑ´Ù.
	for ( int i=0; i<SLOT_HOLD; ++i )
	{
		EMSLOT emSLOT = (EMSLOT) i;
		
		if ( m_PutOnItems[emSLOT].sNativeID == NATIVEID_NULL() ) continue;

		SITEMCUSTOM sCUSTOM = m_PutOnItems[emSLOT];

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )	continue;

		bool bRelease(false);

		// ºñÄÚ½ºÆ¬
		if ( sCUSTOM.nidDISGUISE == NATIVEID_NULL() )
		{
			if ( !SIMPLE_CHECK_ITEM( sCUSTOM.sNativeID, false ) )	bRelease = true;
		}
		// ÄÚ½ºÆ¬
		else 
		{
			if ( !SIMPLE_CHECK_ITEM ( sCUSTOM.sNativeID, sCUSTOM.nidDISGUISE, false ) ) bRelease = true;
		}

		if ( bRelease ) 
		{
			EMSLOT emRHand = GetCurRHand();
			EMSLOT emLHand = GetCurLHand();

			if ( emSLOT == emRHand ) 
			{
				SITEM *pITEM_LEFT=NULL, *pITEM_RIGHT=NULL;
				if ( VALID_SLOT_ITEM(emLHand) )	pITEM_LEFT = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);
				if ( VALID_SLOT_ITEM(emRHand) )	pITEM_RIGHT = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emRHand).sNativeID);

				if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_BOW) || 
					(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_SPEAR) ||

					/*gun-bullet logic, Juver, 2017/05/27 */
					(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_GUN) || 
					(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_RAILGUN) ||
					(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_PORTALGUN) )
				{
					if ( pITEM_LEFT )	//	È­»ì, ºÎÀû »©³»¾ß ÇÔ.
					{
						SITEMCUSTOM sITEM_LHAND = GET_SLOT_ITEM(emLHand);

						CItemDrop cDropItem;
						cDropItem.sItemCustom = m_PutOnItems[emLHand];

						if ( IsInsertToInvenEx ( &cDropItem ) )
						{
							InsertToInvenEx( &cDropItem );
						}
						else
						{
							if ( m_pLandMan )
							{
								m_pLandMan->DropItem ( m_vPos, 
														&(cDropItem.sItemCustom), 
														EMGROUP_ONE, 
														m_dwGaeaID );
							}
						}

						//	¿ÞÂÊ ½½·Ô Áö¿öÁÜ.
						RELEASE_SLOT_ITEM(emLHand);

						//	[ÀÚ½Å¿¡°Ô] SLOT¿¡ ÀÖ¾ú´ø ¾ÆÀÌÅÆ Á¦°Å.
						GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(emLHand);
						m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);				
					}

				}
			}
			
			// Å»°Í ½½·ÔÀÏ°æ¿ì ½½·ÔÀÇ ¾ÆÀÌÅÛ Á¤¸®
			if ( emSLOT == SLOT_VEHICLE &&  m_sVehicle.IsActiveValue() )
			{
				for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
				{
					if ( m_sVehicle.m_PutOnItems[i].sNativeID == NATIVEID_NULL() ) continue;
					
					// ÀÎº¥ »ðÀÔ
					CItemDrop cDropItem;
					cDropItem.sItemCustom = m_sVehicle.m_PutOnItems[i];
					
					if ( IsInsertToInvenEx ( &cDropItem ) )
					{
						InsertToInvenEx ( &cDropItem );
					}
					else
					{
						if ( m_pLandMan )
						{
							m_pLandMan->DropItem ( m_vPos, 
													&(cDropItem.sItemCustom), 
													EMGROUP_ONE, 
													m_dwGaeaID );
						}
					}

					m_sVehicle.m_PutOnItems[i].sNativeID = NATIVEID_NULL();
					
					// Å»°Í ¸±¸®Áî 
					//	[ÀÚ½Å¿¡°Ô] ÇØ´ç ¾ÆÀÌÅÆÀ» ÀÎ¹ê¿¡¼­ »èÁ¦.
					GLMSG::SNET_VEHICLE_ACCESSORY_DELETE NetMsg;
					NetMsg.dwVehicleNum	= m_sVehicle.m_dwGUID;
					NetMsg.accetype = (VEHICLE_ACCESSORY_TYPE)i;
					m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg);

					// ÁÖº¯¿¡ ¾Ë¸².
					GLMSG::SNET_VEHICLE_ACCESSORY_DELETE_BRD NetMsgBrd;
					NetMsgBrd.dwGaeaID = m_dwGaeaID;
					NetMsgBrd.dwVehicleNum = m_sVehicle.m_dwGUID;
					NetMsgBrd.accetype = (VEHICLE_ACCESSORY_TYPE)i;
					SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgBrd) );				
				}

				m_pGLGaeaServer->SaveVehicle( m_dwClientID, m_dwGaeaID, false );
			}
			// Å»°Í Á¤º¸°¡ È°¼ºÈ­ µÇÁö ¾Ê¾ÒÀ»°æ¿ì¿¡´Â Á¦°Å ÇÏÁö ¾Ê´Â´Ù.
			else if ( emSLOT == SLOT_VEHICLE &&  !m_sVehicle.IsActiveValue() )
			{
				continue;
			}

			//	[ÀÚ½Å¿¡°Ô] SLOT¿¡ ÀÖ¾ú´ø ¾ÆÀÌÅÆ Á¦°Å.
			GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(emSLOT);
			m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

			//	[¸ðµÎ¿¡°Ô] ÀÚ½ÅÀÇ Âø¿ë º¹ÀåÀÌ ¹Ù²ñÀ» ¾Ë¸².
			GLMSG::SNETPC_PUTON_RELEASE_BRD NetMsg_PutOn_Release_Brd;
			NetMsg_PutOn_Release_Brd.dwGaeaID = m_dwGaeaID;
			NetMsg_PutOn_Release_Brd.emSlot = emSLOT;
			SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsg_PutOn_Release_Brd) );	


			// ÀÎº¥¿¡ »ðÀÔ
			CItemDrop cDropItem;
			cDropItem.sItemCustom = m_PutOnItems[i];

			if ( IsInsertToInvenEx ( &cDropItem ) )
			{
				InsertToInvenEx ( &cDropItem );
			}
			else
			{
				if ( m_pLandMan )
				{
					m_pLandMan->DropItem ( m_vPos, 
											&(cDropItem.sItemCustom), 
											EMGROUP_ONE, 
											m_dwGaeaID );
				}
			}

			RELEASE_SLOT_ITEM ( emSLOT );			
		}
	}

	// Å»°Í ¾Ç¼¼¼­¸® Á¶°Ç Ã¼Å©
	if ( m_sVehicle.IsActiveValue() )
	{
		for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
		{
			if ( m_sVehicle.m_PutOnItems[i].sNativeID == NATIVEID_NULL() ) continue;
			
            if ( !SIMPLE_CHECK_ITEM( m_sVehicle.m_PutOnItems[i].sNativeID, false ) )
			{
				
				// ÀÎº¥ »ðÀÔ
				CItemDrop cDropItem;
				cDropItem.sItemCustom = m_sVehicle.m_PutOnItems[i];
				
				if ( IsInsertToInvenEx ( &cDropItem ) )
				{
					InsertToInvenEx ( &cDropItem );
				}
				else
				{
					if ( m_pLandMan )
					{
						m_pLandMan->DropItem ( m_vPos, 
												&(cDropItem.sItemCustom), 
												EMGROUP_ONE, 
												m_dwGaeaID );
					}
				}

				m_sVehicle.m_PutOnItems[i].sNativeID = NATIVEID_NULL();
				
				// Å»°Í ¸±¸®Áî 
				//	[ÀÚ½Å¿¡°Ô] ÇØ´ç ¾ÆÀÌÅÆÀ» ÀÎ¹ê¿¡¼­ »èÁ¦.
				GLMSG::SNET_VEHICLE_ACCESSORY_DELETE NetMsg;
				NetMsg.dwVehicleNum	= m_sVehicle.m_dwGUID;
				NetMsg.accetype = (VEHICLE_ACCESSORY_TYPE)i;
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg);

				// ÁÖº¯¿¡ ¾Ë¸².
				GLMSG::SNET_VEHICLE_ACCESSORY_DELETE_BRD NetMsgBrd;
				NetMsgBrd.dwGaeaID = m_dwGaeaID;
				NetMsgBrd.dwVehicleNum = m_sVehicle.m_dwGUID;
				NetMsgBrd.accetype = (VEHICLE_ACCESSORY_TYPE)i;
				SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgBrd) );				
			}
		}
	}
}

void GLChar::TurnAction ( EMACTIONTYPE toAction )
{
	//	Note : ÀÌÀü ¾×¼Ç Ãë¼Ò.
	//

	switch ( m_Action )
	{
	case GLAT_IDLE:
		break;

	case GLAT_ATTACK:
		m_fIdleTime = 0.0f;
		break;

	case GLAT_SKILL:
		m_fIdleTime = 0.0f;
		m_bIsSkillProcess = false;		//anti shit skillcut-as
		break;
	case GLAT_GATHERING:
		break;

	case GLAT_SKILL_PUSH_PULL:		/* push skill logic, Juver, 2020/12/14 */	/* pull skill logic, Juver, 2020/12/14 */
	case GLAT_SKILL_DASH:			/*dash skill logic, Juver, 2017/06/17 */
		{
			if ( toAction == GLAT_DIE || toAction == GLAT_FALLING ){
				if ( m_actorMove.PathIsActive() )		
					m_actorMove.Stop();
			}
		}break;

	default:
		break;
	};

	//CONSOLEMSG_WRITE( "TurnAction [%d]", toAction );

	//	Note : ¾×¼Ç ÃÊ±âÈ­.
	//
	m_fIdleTime = 0.0f;
	m_Action = toAction;

    switch ( m_Action )
	{
	case GLAT_IDLE:
		break;

	case GLAT_MOVE:
		m_wHealingNum = 0;
		break;

	case GLAT_ATTACK:
		m_fattTIMER = 0.0f;
		break;

	case GLAT_SKILL:
		m_fattTIMER = 0.0f;
		break;

	case GLAT_SHOCK:
		break;

	case GLAT_PUSHPULL:
	case GLAT_SKILL_PUSH_PULL:			/* push skill logic, Juver, 2020/12/14 */	/* pull skill logic, Juver, 2020/12/14 */
	case GLAT_SKILL_DASH:				/*dash skill logic, Juver, 2017/06/17 */
		break;

	case GLAT_FALLING:
		{
			TurnAction ( GLAT_DIE );

			/*skill transform, Juver, 2018/09/08 */
			m_skill_transform_data.Reset();

			/* skill position shift, Juver, 2020/12/15 */
			m_bSkillPositionShift = false;
		}break;

	case GLAT_DIE:
		{
			/*skill transform, Juver, 2018/09/08 */
			m_skill_transform_data.Reset();

			/* skill position shift, Juver, 2020/12/15 */
			m_bSkillPositionShift = false;
		}break;
	
	case GLAT_GATHERING:
		break;

	default:
		break;
	};

	/*dmk14 visual bug fix*/
	if ( m_actorMove.PathIsActive() )
	{
		/* push skill logic, Juver, 2020/12/14 */
		/* pull skill logic, Juver, 2020/12/14 */
		/*dash skill logic, Juver, 2017/06/17 */
		/* skill position shift, Juver, 2020/12/15 */
		if ( !IsACTION(GLAT_MOVE) && !IsACTION(GLAT_PUSHPULL) && !IsACTION(GLAT_SKILL_PUSH_PULL) && !IsACTION(GLAT_SKILL_DASH) && !m_bSkillPositionShift ) m_actorMove.Stop();
	}
}

void GLChar::AddPlayHostile ( DWORD dwCHARID, BOOL bBAD, BOOL bClubBattle )
{
	bool bBRIGHTEVENT   = m_pGLGaeaServer->IsBRIGHTEVENT( m_sMapID );		/*dmk14 freepk*/
	bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
	bool bGuidBattleMap = m_pLandMan->m_bGuidBattleMap;
	bool bClubDeathMatch = m_pLandMan->m_bClubDeathMatchMap;

	/* open pk function, Juver, 2019/12/24 */
	bool bOpenPK = false;
	if ( m_pLandMan && m_pLandMan->IsOpenPK() )
	{
		bOpenPK = true;
	}

	PGLCHAR pCHAR = m_pGLGaeaServer->GetCharID ( dwCHARID );
	if ( !pCHAR )		return;

	if ( pCHAR->IsSTATE( EM_ACT_DIE ) ) return;
	if ( IsSTATE( EM_ACT_DIE ) ) return;
	
	if ( GetSchool() == pCHAR->GetSchool() )	bSCHOOL_FREEPK = false;

	/*pvp tyranny, Juver, 2017/08/24 */
	/*school wars, Juver, 2018/01/19 */
	/*pvp capture the flag, Juver, 2018/01/31 */
	/* open pk function, Juver, 2019/12/24 */
	/* pvp club death match, Juver, 2020/11/25 */
	// DESC : À¯Àú°¡ ¸¹Àº ´ë¸¸¿¡¼­´Â ÇÐ¿ø°£ ÀÚÀ¯PKÀÏ °æ¿ì Àû´ëÀÚ°¡ ±âÇÏ±Þ¼öÀûÀ¸·Î ´Ã¾î³ª¸ç
	//			ºÒ¾ÈÇÑ ¼º´ÉÀ» °¡Áø HASH_MAPÀ» »ç¿ëÇÏ´Â Çö µ¥ÀÌÅ¸ ±¸Á¶·Î¼­´Â »ç¿ëÇÏ±â ºÒ°¡´ÉÇÏ´Ù.			            
	if ( !( bBRIGHTEVENT || 
		bSCHOOL_FREEPK || 
		bGuidBattleMap || 
		bClubDeathMatch ||
		m_pLandMan->m_bPVPTyrannyMap || 
		m_pLandMan->m_bPVPSchoolWarsMap || 
		m_pLandMan->m_bPVPCaptureTheFlagMap ||
		m_pLandMan->m_bPVPClubDeathMatchLobbyMap ||
		m_pLandMan->m_bPVPClubDeathMatchBattleMap || 
		m_pLandMan->m_bPVPPBGLobbyMap || 
		m_pLandMan->m_bPVPPBGBattleMap || 
		m_pLandMan->m_bPVPWoeMap ||
		bOpenPK ) )
	{
		//	Note : Àû´ë ÇàÀ§ÀÚ µî·Ï. ( pk )
		ADD_PLAYHOSTILE ( dwCHARID, bBAD );
	}
	// ÇÐ¿ø°£ ÀÚÀ¯ÇÇÄÉÀÌ ÁøÇàÁßÀÌ¸ç »ó´ë°¡ °°Àº ÇÐ±³ÀÏ°æ¿ì Àû´ëÇàÀ§ ´ë»óÀÚ·Î ±¸ºÐÇÏ¿© °ü¸®ÇÔ.
	if ( GLSchoolFreePK::GetInstance().IsON() && !bSCHOOL_FREEPK )
	{
		ADD_PLAYHOSTILE ( dwCHARID, bBAD );
	}

	// ÇÐ¿ø°£ ÀÚÀ¯ÇÇÄÉÀÌ ÁøÇàÁß »ó´ë°¡ ·¾ÀÌ 20º¸´Ù ³·À»¶§ Àû´ëÇàÀ§ ´ë»óÀÚ·Î ±¸ºÐÇÏ¿© °ü¸®ÇÔ.
	if ( bSCHOOL_FREEPK )
	{
		if ( pCHAR->GetLevel() < GLCONST_CHAR::nPK_LIMIT_LEVEL || GetLevel() < GLCONST_CHAR::nPK_LIMIT_LEVEL )
		{
			ADD_PLAYHOSTILE ( dwCHARID, bBAD );
		}
	}

	//	Note : Àû´ëÀÚ µî·Ï ¾Ë¸².
	GLMSG::SNETPC_PLAYERKILLING_ADD NetMsgAdd;
	NetMsgAdd.dwCharID = dwCHARID;
	NetMsgAdd.wSchoolID = pCHAR->m_wSchool;
	NetMsgAdd.bBAD = bBAD;
	NetMsgAdd.bClubBattle = bClubBattle;
	NetMsgAdd.fTime = 0.0f;

	StringCchCopy ( NetMsgAdd.szName, CHAR_SZNAME, pCHAR->m_szName );

	m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgAdd);
}

// *****************************************************
// Desc: ´ë·ÃÁßÀÎ ´ë»óÀÎÁö Check
// *****************************************************
bool GLChar::IsConflictTarget ( GLACTOR* pTARGET )
{
	//GASSERT(pTARGET&&"GLChar::IsConflictTarget()");
	if ( !pTARGET )				return false;

	EMCROW emCROW = pTARGET->GetCrow();
	if ( emCROW!=CROW_PC )		return false;

	SCONFTING* pCONFTING = pTARGET->GETCONFTING();
	if ( !pCONFTING )			return false;

	if ( m_sCONFTING.IsFIGHTING() && pCONFTING->IsFIGHTING() )
	{
		if ( m_sCONFTING.emTYPE!=pCONFTING->emTYPE )	return false;

		switch ( pCONFTING->emTYPE )
		{
		case EMCONFT_ONE:
			return ( m_dwGaeaID==pCONFTING->dwTAR_ID );

		case EMCONFT_PARTY:
			return ( m_dwPartyID==pCONFTING->dwTAR_ID );

		case EMCONFT_GUILD:
			return ( m_dwGuild==pCONFTING->dwTAR_ID );
		};
	}

	return false;
}

// *****************************************************
// Desc: ¾×¼ÇÀ» ÃëÇÒ ¼ö ÀÖ´Â ´ë»óÀÎÁö check
// *****************************************************
bool GLChar::IsReActionable ( GLACTOR* pTARGET, bool benermy, bool bsummon )
{
	GASSERT(pTARGET&&"GLChar::IsReActionable()");
	if ( !pTARGET )					return false;
	if ( !m_pLandMan )				return false;

	// ¼ºÇâÀÌº¥Æ® ÁßÀÎÁö
	bool bBRIGHTEVENT = m_pGLGaeaServer->IsBRIGHTEVENT( m_sMapID );		/*dmk14 freepk*/

	EMCROW emCROW = pTARGET->GetCrow();
	bool bPKZone = m_pLandMan->IsPKZone();
	bool bGuidBattleMap = m_pLandMan->m_bGuidBattleMap;
	bool bClubDeathMatch = m_pLandMan->m_bClubDeathMatchMap;
	bool bPVPTyranny = m_pLandMan->m_bPVPTyrannyMap;
	bool bPVPSchoolWars = m_pLandMan->m_bPVPSchoolWarsMap;
	bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
	bool bClubBattleZone = m_pLandMan->IsClubBattleZone();
	bool bPVPWoe = m_pLandMan->m_bPVPWoeMap;
	
	/* open pk function, Juver, 2019/12/24 */
	bool bOpenPK = false;
	if ( m_pLandMan && m_pLandMan->IsOpenPK() )
	{
		bOpenPK = true;
	}

	if ( GetSchool() == pTARGET->GetSchool() )	bSCHOOL_FREEPK = false;

	bool bREACTION(true);

	/*pvp capture the flag, Juver, 2018/01/31 */
	BOOL bCaptureTheFlag = m_pLandMan->m_bPVPCaptureTheFlagMap;

	/* pvp club death match, Juver, 2020/11/25 */
	BOOL bPVPClubDeathMatch = FALSE;
	if ( m_pLandMan->m_bPVPClubDeathMatchLobbyMap || m_pLandMan->m_bPVPClubDeathMatchBattleMap )
		bPVPClubDeathMatch = TRUE;

	BOOL bPVPPBG = FALSE;
	if ( m_pLandMan->m_bPVPPBGLobbyMap || m_pLandMan->m_bPVPPBGBattleMap )
		bPVPPBG = TRUE;

	// »ó´ë°¡ ÀûÀÎ °æ¿ì
	if ( benermy )
	{
		switch ( emCROW )
		{
		case CROW_PC:
			// Absolute Non-PK State Check.
			if ( GLCONST_CHAR::bPKLESS ) return false;

			// Desc : Àý´ë ¾ÈÀüÇÑ ½Ã°£ (10ÃÊ)
			if ( !pTARGET->IsSafeTime() && !IsSafeTime() && !pTARGET->IsSafeZone() && !IsSafeZone() )
			{
				// °°Àº ÆÄÆ¼ÀÇ ¸â¹öÀÎÁö
				GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
				GLPARTY_FIELD* sMyParty = sPartyFieldMan.GetParty ( m_dwPartyID );
				if ( sMyParty )
				{
					if ( sMyParty->ISMEMBER ( pTARGET->GetCtrlID() ) )
					{
						return false;
					}
				}

				// ¼­·Î ´ë·ÃÁßÀÎÁö
				bREACTION = IsConflictTarget ( pTARGET );
				if ( !bREACTION )
				{
					SCONFTING* pCONFTING = pTARGET->GETCONFTING();
					if ( m_sCONFTING.IsCONFRONTING() || pCONFTING->IsCONFRONTING() )
					{
						return false;
					}
				}
				else
				{
					return true;
				}

				// Àû´ë°ü°èÀÎÁö Ã¼Å©
				DWORD dwCHARID = pTARGET->GetCharID();
				bREACTION = IS_PLAYHOSTILE ( dwCHARID );
				if ( bREACTION ) return true;
	
				// 20 Lev ÀÌ»óÀÏ¶§
				if ( pTARGET->GetLevel() >= GLCONST_CHAR::nPK_LIMIT_LEVEL && GLCONST_CHAR::bPK_MODE && bPKZone)
				{
					// PK °¡´ÉÇÑ ±¸¿ª ( ÇÐ±³°£ Free PK )
					/*pvp capture the flag, Juver, 2018/01/31 */
					if ( bSCHOOL_FREEPK && !bCaptureTheFlag )						return true;
					
					// ¼ºÇâ ÀÌº¥Æ® ÁßÀÌ¸é
					/*pvp capture the flag, Juver, 2018/01/31 */
					if ( bBRIGHTEVENT && ( m_dwPartyID != pTARGET->GetPartyID() ) && !bCaptureTheFlag )	return true;

					/* open pk function, Juver, 2019/12/25 */
					if ( bOpenPK && ( m_dwPartyID != pTARGET->GetPartyID() ) && !bCaptureTheFlag )	return true;
											
					// ±×³É °­Á¦°ø°Ý
					if ( bsummon )
					{
						if ( bBRIGHTEVENT && !bCaptureTheFlag )	bREACTION = true;

						/* open pk function, Juver, 2019/12/25 */
						if ( bOpenPK && !bCaptureTheFlag )	bREACTION = true;
					}
					else	
						bREACTION = true;
				}

				PGLCHAR pChar = m_pGLGaeaServer->GetChar( pTARGET->GetCtrlID() );
				GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
				GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
				GLCLUB *pTarClub = NULL;
				if ( pChar ) pTarClub = cClubMan.GetClub ( pChar->m_dwGuild );

				// ¼±µµÀü¿¡¼­ °°Àº µ¿¸Í Å¬·´ÀÌ¸é °ø°Ý ºÒ°¡´É
				if ( GLCONST_CHAR::bPK_MODE )
				{
					if ( bGuidBattleMap )
					{
						bREACTION = true;

						if ( pMyClub && pTarClub )
						{
							BOOL bAlliance = pMyClub->IsAllianceGuild ( pTarClub->m_dwID );
							if ( bAlliance )	bREACTION = false;
						} // end of if
					}
					else if ( bClubDeathMatch )
					{
						if ( m_dwGuild == pChar->m_dwGuild ) bREACTION = false;
						else bREACTION = true;
					}
					/*pvp tyranny, Juver, 2017/08/24 */
					else if ( m_pLandMan->m_bPVPTyrannyMap )
					{
						if ( RPARAM::bCTFGVG )
						{
							if ( m_dwGuild == pChar->m_dwGuild )
								bREACTION = false;
							else 
								bREACTION = true;
						}
						else
						{
							if ( m_wSchool == pChar->m_wSchool ) 
								bREACTION = false;
							else 
								bREACTION = true;
						}	
					}
					/*school wars, Juver, 2018/01/19 */
					else if ( m_pLandMan->m_bPVPSchoolWarsMap )
					{
						if ( m_wSchool == pChar->m_wSchool ) bREACTION = false;
						else bREACTION = true;
					}
					/*pvp capture the flag, Juver, 2018/01/31 */
					else if ( m_pLandMan->m_bPVPCaptureTheFlagMap )
					{
						if ( m_wCaptureTheFlagTeam == pChar->m_wCaptureTheFlagTeam ) 
							bREACTION = false;
						else 
							bREACTION = true;
					}
						/*woe Arc Development 08-06-2024*/
					else if (m_pLandMan->m_bPVPWoeMap && RPARAM::bWoeGVG == 0)
					{
						if (m_wSchool == pChar->m_wSchool) bREACTION = false;
						else bREACTION = true;
					}
					/* pvp club death match, Juver, 2020/11/25 */
					else if ( bPVPClubDeathMatch )
					{
						if ( m_dwGuild == pChar->m_dwGuild ) bREACTION = false;
						else bREACTION = true;
					}
					else if ( bPVPPBG )
					{
						if ( m_dwPartyID == pChar->m_dwPartyID ) 
							bREACTION = false;
						else 
							bREACTION = true;
					}

					if ( bPKZone && bClubBattleZone )
					{
						if ( pMyClub && pTarClub )
						{
							if ( pMyClub->IsBattle( pTarClub->m_dwID ) ) return true;
							if ( pMyClub->IsBattleAlliance( pTarClub->m_dwAlliance ) ) return true;
						}
					}
				}

			} // end of if
			else
				bREACTION = false;
			break;
		
			/*skill summon, Juver, 2017/10/11 */
		case CROW_SUMMON:
			{
				// Absolute Non-PK State Check.
				if ( GLCONST_CHAR::bPKLESS ) return false;

				PGLSUMMONFIELD psummon_field = GLGaeaServer::GetInstance().GetSummon ( pTARGET->GetCtrlID() );
				if ( psummon_field && psummon_field->m_pOwner )
				{
					if( !psummon_field->IsTargetReady() )	return false;

					// Desc : Àý´ë ¾ÈÀüÇÑ ½Ã°£ (10ÃÊ)
					if ( !psummon_field->m_pOwner->IsSafeTime() && !IsSafeTime() && !psummon_field->m_pOwner->IsSafeZone() && !IsSafeZone() )
					{
						// °°Àº ÆÄÆ¼ÀÇ ¸â¹öÀÎÁö
						GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
						GLPARTY_FIELD* sMyParty = sPartyFieldMan.GetParty ( m_dwPartyID );
						if ( sMyParty )
						{
							if ( sMyParty->ISMEMBER ( psummon_field->m_pOwner->GetCtrlID() ) )
							{
								return false;
							}
						}

						// ¼­·Î ´ë·ÃÁßÀÎÁö
						bREACTION = IsConflictTarget ( psummon_field->m_pOwner );
						if ( !bREACTION )
						{
							SCONFTING* pCONFTING = psummon_field->m_pOwner->GETCONFTING();
							if ( m_sCONFTING.IsCONFRONTING() || pCONFTING->IsCONFRONTING() )
							{
								return false;
							}
						}
						else
						{
							return true;
						}

						// Àû´ë°ü°èÀÎÁö Ã¼Å©
						DWORD dwCHARID = psummon_field->m_pOwner->GetCharID();
						bREACTION = IS_PLAYHOSTILE ( dwCHARID );
						if ( bREACTION ) return true;

						// 20 Lev ÀÌ»óÀÏ¶§
						if ( psummon_field->m_pOwner->GetLevel() >= GLCONST_CHAR::nPK_LIMIT_LEVEL && GLCONST_CHAR::bPK_MODE && bPKZone)
						{
							// PK °¡´ÉÇÑ ±¸¿ª ( ÇÐ±³°£ Free PK )
							/*pvp capture the flag, Juver, 2018/01/31 */
							if ( bSCHOOL_FREEPK && !bCaptureTheFlag )											return true;

							// ¼ºÇâ ÀÌº¥Æ® ÁßÀÌ¸é
							/*pvp capture the flag, Juver, 2018/01/31 */
							if ( bBRIGHTEVENT && ( m_dwPartyID != psummon_field->m_pOwner->GetPartyID() ) && !bCaptureTheFlag )	return true;

							/* open pk function, Juver, 2019/12/25 */
							if ( bOpenPK && ( m_dwPartyID != psummon_field->m_pOwner->GetPartyID() ) && !bCaptureTheFlag )	return true;

							// ±×³É °­Á¦°ø°Ý
							bREACTION = true;		
						}

						//PGLCHAR pChar = m_pGLGaeaServer->GetChar( pTARGET->GetCtrlID() );
						GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
						GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
						GLCLUB *pTarClub = NULL;
						if ( psummon_field->m_pOwner ) pTarClub = cClubMan.GetClub ( psummon_field->m_pOwner->m_dwGuild );

						// ¼±µµÀü¿¡¼­ °°Àº µ¿¸Í Å¬·´ÀÌ¸é °ø°Ý ºÒ°¡´É
						if ( GLCONST_CHAR::bPK_MODE )
						{
							if ( bGuidBattleMap && ( bPVPWoe  && RPARAM::bWoeGVG == 1 ) )
							{
								bREACTION = true;

								if ( pMyClub && pTarClub )
								{
									BOOL bAlliance = pMyClub->IsAllianceGuild ( pTarClub->m_dwID );
									if ( bAlliance )	bREACTION = false;
								} // end of if
							}
							else if ( bClubDeathMatch )
							{
								if ( m_dwGuild == psummon_field->m_pOwner->m_dwGuild ) bREACTION = false;
								else bREACTION = true;
							}
							/*pvp tyranny, Juver, 2017/08/24 */
							else if ( m_pLandMan->m_bPVPTyrannyMap )
							{
								if ( RPARAM::bCTFGVG )
								{
									if ( m_dwGuild == psummon_field->m_pOwner->m_dwGuild )
										bREACTION = false;
									else 
										bREACTION = true;
								}
								else
								{
									if ( m_wSchool == psummon_field->m_pOwner->m_wSchool ) 
										bREACTION = false;
									else 
										bREACTION = true;
								}
							}
							/*school wars, Juver, 2018/01/19 */
							else if ( m_pLandMan->m_bPVPSchoolWarsMap )
							{
								if ( m_wSchool == psummon_field->m_pOwner->m_wSchool ) bREACTION = false;
								else bREACTION = true;
							}
							/*pvp capture the flag, Juver, 2018/01/31 */
							else if ( m_pLandMan->m_bPVPCaptureTheFlagMap )
							{
								if ( m_wCaptureTheFlagTeam == psummon_field->m_pOwner->m_wCaptureTheFlagTeam ) 
									bREACTION = false;
								else 
									bREACTION = true;
							}
							/* pvp club death match, Juver, 2020/11/25 */
							else if ( bPVPClubDeathMatch )
							{
								if ( m_dwGuild == psummon_field->m_pOwner->m_dwGuild )
									bREACTION = false;
								else 
									bREACTION = true;
							}
							else if ( bPVPPBG )
							{
								if ( m_dwPartyID == psummon_field->m_pOwner->m_dwPartyID ) 
									bREACTION = false;
								else 
									bREACTION = true;
							}
							else if (m_pLandMan->m_bPVPWoeMap)
							{
								if (m_wSchool == psummon_field->m_pOwner->m_wSchool) bREACTION = false;
								else bREACTION = true;
							}

							if ( bPKZone && bClubBattleZone )
							{
								if ( pMyClub && pTarClub )
								{
									if ( pMyClub->IsBattle( pTarClub->m_dwID ) ) return true;
									if ( pMyClub->IsBattleAlliance( pTarClub->m_dwAlliance ) ) return true;
								}
							}
						}

					} // end of if
					else
						bREACTION = false;
				}
			}break;

		case CROW_MOB:
			{
				if ( m_sCONFTING.IsCONFRONTING() )	bREACTION = false;
				else if ( IsSafeZone() ) 	bREACTION = false;
				else bREACTION = true;

				/*pvp tyranny, Juver, 2017/08/26 */
				/*if ( emCROW == CROW_MOB && m_pLandMan && m_pLandMan->m_bPVPTyrannyMap  )
				{
					PGLCROW pCROW = m_pLandMan->GetCrow( pTARGET->GetCtrlID() );
					if ( pCROW && 
						pCROW->m_wTyrannyOwner != TYRANNY_SCHOOL_SIZE &&
						pCROW->m_wTyrannyOwner == m_wSchool )
					{
						bREACTION = false;
					}
				}*/

				/*crow school, Juver, 2018/08/02 */
				if ( emCROW == CROW_MOB && m_pLandMan )
				{
					PGLCROW pCROW = m_pLandMan->GetCrow( pTARGET->GetCtrlID() );
					if ( pCROW && pCROW->m_pCrowData && 
						pCROW->m_pCrowData->m_sBasic.m_emCrowSchool != EMCROW_SCHOOL_SIZE &&
						pCROW->m_pCrowData->m_sBasic.m_emCrowSchool == m_wSchool )
					{
						bREACTION = false;
					}
				}
				/*woe Arc Development 08-06-2024*/
				if (emCROW == CROW_MOB && m_pLandMan->m_bPVPWoeMap)
				{
					PGLCROW pCROW = m_pLandMan->GetCrow( pTARGET->GetCtrlID() );
					if (pCROW->m_wWoeOwner == m_dwGuild)
					{
						bREACTION = false;
					}
				}
			}break;

		case CROW_NPC:
			bREACTION = false;
			break;

			/* crow zone, Juver, 2018/02/21 */
		case CROW_ZONE_NAME:
		case CROW_GATE_NAME:
			bREACTION = false;
			break;

		case CROW_MATERIAL:
			bREACTION = false;
			break;

		//case CROW_PET:	break;	// PetData

		default:
			bREACTION = false;
			break;
		};
	}
	// »ó´ë°¡ ÀûÀÌ ¾Æ´Ñ °æ¿ì
	else
	{
		switch ( emCROW )
		{
		case CROW_PC:
			{
				//	pc ¿¡°Ô ºñ°ø°Ý ¸¶¹ý °°Àº °ÍÀº ´ë·ÃÀÚ°¡ ¾Æ´Ò ¶§¸¸ °¡´É.
				bREACTION = !IsConflictTarget ( pTARGET );

				//	»ó´ë¹æÀÌ ´ë·ÃÁßÀÏ ¶§´Â ÀÚ±â°¡ ´ë·ÃÁßÀÌ°í °°Àº ÆíÀÏ¶§ °¡´É.
				SCONFTING* pTAR_CONFT = pTARGET->GETCONFTING();
				if ( pTAR_CONFT->IsCONFRONTING() )
				{
					bREACTION = m_sCONFTING.IsSAMENESS(pTAR_CONFT->emTYPE,pTAR_CONFT->dwTAR_ID);
				}

				bool bHOSTILE = IS_PLAYHOSTILE ( pTARGET->GetCharID() );
				if ( bHOSTILE )	bREACTION = false;
			}
			break;
		
		case CROW_SUMMON:
		case CROW_MOB:
			bREACTION = false;
			break;

		case CROW_NPC:
			bREACTION = m_sCONFTING.IsCONFRONTING() ? false : true;
			break;

			/* crow zone, Juver, 2018/02/21 */
		case CROW_ZONE_NAME:
		case CROW_GATE_NAME:
			bREACTION = false;
			break;

		case CROW_MATERIAL:
			bREACTION = true;
			break;

		//case CROW_PET:	break;	// PetData

		default:
			bREACTION = false;
			break;
		};
	}

	return bREACTION;
}


inline DWORD GLChar::ToDamage ( const STARGETID &sTargetID, const int nDAMAGE, const BOOL bShock )
{
	GLACTOR* pACTOR_TAR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTargetID );
	if ( !pACTOR_TAR )	return 0;

	//	Note : PC¿¡°Ô damage¸¦ ÁÖ·Á¸é ´ë·ÃÀÚ¿©¾ß °¡´É.
	//		´ë·Ã Á¾·á Á÷ÈÄ damage À¯ÀÔ Â÷´ÜÀ» À§ÇØ¼­.
	//
	if ( !IsReActionable(pACTOR_TAR) )	return 0;

	return pACTOR_TAR->ReceiveDamage ( GETCROW(), m_dwGaeaID, nDAMAGE, bShock );
}

void GLChar::PreStrikeProc ( BOOL bSkill, BOOL bLowSP )
{
	SANIATTACK sAniAttack;
	STARGETID sTargetID;

	BOOL bCheckHit = FALSE;

	if ( !bSkill )
	{
		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		/*skill transform, Juver, 2018/09/09 */
		EMANI_SUBTYPE emANISUBTYPE = CHECK_ATTACK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );

		GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
		VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[AN_ATTACK][emANISUBTYPE];
		if ( vecAniAttack.empty() )	return; // FALSE;

		//	Note : Æ¯Á¤ °ø°Ý ¿¡´Ï¸ÞÀÌ¼Ç ¼³Á¤ ¼±ÅÃ.
		//
		sAniAttack = vecAniAttack[0];

		sTargetID = m_TargetID;
		bCheckHit = TRUE;
	}
	else
	{
		//	Note : ½ºÅ³ Á¤º¸ °¡Á®¿È.
		//
		PGLSKILL pSkill = NULL;
		pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL.wMainID, m_idACTIVESKILL.wSubID );

		GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
		VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[pSkill->m_sEXT_DATA.emANIMTYPE][pSkill->m_sEXT_DATA.emANISTYPE];
		if ( vecAniAttack.empty() )	return; // FALSE;

		//	Note : Æ¯Á¤ °ø°Ý ¿¡´Ï¸ÞÀÌ¼Ç ¼³Á¤ ¼±ÅÃ.
		//
		sAniAttack = vecAniAttack[0];

		if ( pSkill->m_sBASIC.emAPPLY != SKILL::EMAPPLY_MAGIC )
		{
			//	Note : ½ºÅ³ Å¸°ÙÀ» ÇÏ³ª¸¸ À¯È¿ÇÏ°Ô È®ÀÎÀ» ÇÏ°í ÀÖÀ½.
			//	NEED : ´ÙÁß Å¸°ÙÀÏ °æ¿ì ´Ù½Ã È®ÀÎÇØ¾ßÇÔ.
			/*multi skill Juver, 2018/09/04 */
			sTargetID = STARGETID(static_cast<EMCROW>(m_sTARGET_IDS_MAIN[0].wCrow),static_cast<DWORD>(m_sTARGET_IDS_MAIN[0].wID));
			bCheckHit = TRUE;
		}
		else
		{
			sTargetID.dwID = EMTARGET_NULL;
		}
	}

	//	Å¥ ÃÊ±âÈ­
	m_sHITARRAY.clear ();

	for ( int i = 0; i < sAniAttack.m_wDivCount; i++ )
	{
		bool bhit = true;
		if ( bCheckHit && (sTargetID.dwID!=EMTARGET_NULL) )
		{
			if ( !CHECKHIT(sTargetID,m_pLandMan,bLowSP) )		bhit = false;
		}

		m_sHITARRAY.push_back ( SSTRIKE(sAniAttack.m_wDivFrame[i],bhit,cast_bool(bLowSP)) );
	}
}

void GLChar::AvoidProc ( const SNATIVEID skill_id, const BOOL bLowSP )
{
	//CONSOLEMSG_WRITE( "AvoidProc Start [%u]", skill_id.dwID );

	if ( skill_id==NATIVEID_NULL() )
	{
		if ( !bLowSP )
		{
			WORD wDisSP = m_wSUM_DisSP + GLCONST_CHAR::wBASIC_DIS_SP;
			m_sSP.DECREASE ( wDisSP );
		}

		GLACTOR *pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, m_TargetID );
		if ( pACTOR )	pACTOR->ReceiveDamage ( CROW_PC, m_dwGaeaID, 0, FALSE);
	}
	else
	{
		//	Note : SP°¡ ºÎÁ·ÇÏÁö ¾ÊÀ»¶§¿¡´Â SP ¼Ò¸ð½ÃÅ´.
		//
		if ( !bLowSP )
		{
			//	Note : Ä³¸¯ÅÍ°¡ ¹è¿î ½ºÅ³ Á¤º¸ °¡Á®¿È.
			SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
			if( learniter==m_ExpSkills.end() )								return;	// ¹è¿î ½ºÅ³ÀÌ ¾Æ´Ò °æ¿ì.
			SCHARSKILL &sSkill = (*learniter).second;

			//	½ºÅ³ Á¤º¸ °¡Á®¿È.
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
			if ( !pSkill )													return;
			SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[sSkill.wLevel];

			WORD wDisSP = m_wSUM_DisSP + sSKILL_DATA.wUSE_SP;
			m_sSP.DECREASE ( wDisSP );
		}

		/*multi skill Juver, 2018/09/04 */
		for ( WORD i=0; i<m_wTARGET_NUM_MAIN; ++i )
		{
			STARGETID sTargetID ( (EMCROW)m_sTARGET_IDS_MAIN[i].wCrow, (WORD)m_sTARGET_IDS_MAIN[i].wID );

			GLACTOR *pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTargetID );
			if ( pACTOR )	pACTOR->ReceiveDamage ( CROW_PC, m_dwGaeaID, 0, FALSE);
		}

		/*multi skill Juver, 2018/09/04 */
		for ( WORD i=0; i<m_wTARGET_NUM_SUB; ++i )
		{
			STARGETID sTargetID ( (EMCROW)m_sTARGET_IDS_SUB[i].wCrow, (WORD)m_sTARGET_IDS_SUB[i].wID );

			GLACTOR *pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTargetID );
			if ( pACTOR )	pACTOR->ReceiveDamage ( CROW_PC, m_dwGaeaID, 0, FALSE);
		}
	}

	//	Note : ÀÚ½Å¿¡°Ô.
	GLMSG::SNETPC_ATTACK_AVOID NetMsg;
	NetMsg.emTarCrow	= m_TargetID.emCrow;
	NetMsg.dwTarID		= m_TargetID.dwID;

	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

	//	Note : ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	//
	GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID	= m_dwGaeaID;
	NetMsgBrd.emTarCrow	= m_TargetID.emCrow;
	NetMsgBrd.dwTarID	= m_TargetID.dwID;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
}

void GLChar::DamageProc ( int nDAMAGE, const DWORD dwDamageFlag, const BOOL bLowSP )
{
	//	Note : µ¥¹ÌÁö »êÃâ.
	//				( sp ºÎÁ·½Ã µ¥¹ÌÁö °¨»ê. )
	float	fDAMAGE_RATE(1.0f);
	if ( bLowSP )						fDAMAGE_RATE *= (1-GLCONST_CHAR::fLOWSP_DAMAGE);

	nDAMAGE = int(nDAMAGE*fDAMAGE_RATE);
	if ( nDAMAGE < 1 )	nDAMAGE = 1;

	// ´ë·Ã Á¾·áÈÄ ¹«ÀûÅ¸ÀÓÀÌ¸é °ø°Ý ¹«½Ã
	PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( m_TargetID.dwID );
	if ( pCHAR )
	{
		if ( pCHAR->m_sCONFTING.IsPOWERFULTIME () )
			return;
	}

	//	Note : SP ¼Ò¸ð.
	//
	if ( !bLowSP )
	{
		WORD wDisSP = m_wSUM_DisSP + GLCONST_CHAR::wBASIC_DIS_SP;
		m_sSP.DECREASE ( wDisSP );

		GLMSG::SNETPC_UPDATE_SP NetMsg;
		NetMsg.wNowSP = m_sSP.wNow;
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsg );
	}

	if ( m_TargetID.emCrow==CROW_PC )
	{
		nDAMAGE = int(nDAMAGE*GLCONST_CHAR::fPK_POINT_DEC_PHY[ CharClassToClassIndex( m_emClass )]);
		if ( nDAMAGE==0 )	nDAMAGE = 1;
	}

	//	Note : ´ë»óÀÚ¿¡°Ô ´ë¹ÌÁö°ª Àû¿ë.
	//
	bool bShock = ( dwDamageFlag & DAMAGE_TYPE_SHOCK );
	ToDamage ( m_TargetID, nDAMAGE, bShock );

	//	Note : ÀÚ½Å¿¡°Ô.
	GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
	NetMsg.emTarCrow		= m_TargetID.emCrow;
	NetMsg.dwTarID			= m_TargetID.dwID;
	NetMsg.nDamage			= nDAMAGE;
	NetMsg.dwDamageFlag		= dwDamageFlag;
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

	//	Note : ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	//
	GLMSG::SNETPC_ATTACK_DAMAGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID		= m_dwGaeaID;
	NetMsgBrd.emTarCrow		= m_TargetID.emCrow;
	NetMsgBrd.dwTarID		= m_TargetID.dwID;
	NetMsgBrd.nDamage		= nDAMAGE;
	NetMsgBrd.dwDamageFlag	= dwDamageFlag;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, m_TargetID );
	if ( !pACTOR )	return;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	//	Note : »óÅÂ ÀÌ»ó À¯¹ß.
	//
	SITEM* pRHandItem = GET_SLOT_ITEMDATA(emRHand);
	SITEM* pLHandItem = GET_SLOT_ITEMDATA(emLHand);

	if ( pRHandItem && pRHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		ITEM::SSATE_BLOW &sBlow = pRHandItem->sSuitOp.sBLOW;

		//	Note : ¹ß»ý È®À² °è»ê.
		//
		short nBLOWRESIST = pACTOR->GETRESIST().GetElement ( STATE_TO_ELEMENT(sBlow.emTYPE) );
		if ( nBLOWRESIST>99 )	nBLOWRESIST = 99;

//		float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBlow.emTYPE, GLPeriod::GetInstance().GetWeather(), m_pLandMan->IsWeatherActive() );
		DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
		float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBlow.emTYPE, dwWeather, m_pLandMan->IsWeatherActive() );

		BOOL bBLOW = FALSE;
		if ( !(pACTOR->GETHOLDBLOW()&STATE_TO_DISORDER(sBlow.emTYPE)) )
			bBLOW = GLOGICEX::CHECKSTATEBLOW ( sBlow.fRATE*fPOWER, GETLEVEL(), pACTOR->GetLevel(), nBLOWRESIST );

		if ( bBLOW )
		{
			//	Note : »óÅÂÀÌ»ó ¹ß»ý.
			//	
			SSTATEBLOW sSTATEBLOW;
			float fLIFE = sBlow.fLIFE * fPOWER;
			fLIFE = ( fLIFE - (fLIFE*nBLOWRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );

			sSTATEBLOW.emBLOW = sBlow.emTYPE;
			sSTATEBLOW.fAGE = fLIFE;
			sSTATEBLOW.fSTATE_VAR1 = sBlow.fVAR1;
			sSTATEBLOW.fSTATE_VAR2 = sBlow.fVAR2;

			pACTOR->STATEBLOW ( sSTATEBLOW );

			//	Note : »óÅÂÀÌ»ó ¹ß»ý Msg.
			//
			GLMSG::SNETPC_STATEBLOW_BRD NetMsgState;
			NetMsgState.emCrow = m_TargetID.emCrow;
			NetMsgState.dwID = m_TargetID.dwID;
			NetMsgState.emBLOW = sBlow.emTYPE;
			NetMsgState.fAGE = fLIFE;
			NetMsgState.fSTATE_VAR1 = sBlow.fVAR1;
			NetMsgState.fSTATE_VAR2 = sBlow.fVAR2;

			//	Note : '½ºÅ³´ë»ó'ÀÇ ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
			//
			pACTOR->SNDMSGAROUND ( (NET_MSG_GENERIC*) &NetMsgState );

			//	Note : ½ºÅ³ ´ë»ó¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
			if ( m_TargetID.emCrow==CROW_PC )	m_pGLGaeaServer->SENDTOCLIENT ( pACTOR->GETCLIENTID (), &NetMsgState );
		}
	}
	else if ( pLHandItem && pLHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		ITEM::SSATE_BLOW &sBlow = pLHandItem->sSuitOp.sBLOW;

		//	Note : ¹ß»ý È®À² °è»ê.
		//
		short nBLOWRESIST = pACTOR->GETRESIST().GetElement ( STATE_TO_ELEMENT(sBlow.emTYPE) );
		if ( nBLOWRESIST>99 )	nBLOWRESIST = 99;

//		float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBlow.emTYPE, GLPeriod::GetInstance().GetWeather(), m_pLandMan->IsWeatherActive() );
		DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
		float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBlow.emTYPE, dwWeather, m_pLandMan->IsWeatherActive() );

		BOOL bBLOW = FALSE;
		if ( !(pACTOR->GETHOLDBLOW()&STATE_TO_DISORDER(sBlow.emTYPE)) )
			bBLOW = GLOGICEX::CHECKSTATEBLOW ( sBlow.fRATE*fPOWER, GETLEVEL(), pACTOR->GetLevel(), nBLOWRESIST );

		if ( bBLOW )
		{
			//	Note : »óÅÂÀÌ»ó ¹ß»ý.
			//	 
			SSTATEBLOW sSTATEBLOW;
			float fLIFE = sBlow.fLIFE * fPOWER;
			fLIFE = ( fLIFE - (fLIFE*nBLOWRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );

			sSTATEBLOW.emBLOW = sBlow.emTYPE;
			sSTATEBLOW.fAGE = fLIFE;
			sSTATEBLOW.fSTATE_VAR1 = sBlow.fVAR1;
			sSTATEBLOW.fSTATE_VAR2 = sBlow.fVAR2;

			pACTOR->STATEBLOW ( sSTATEBLOW );

			//	Note : »óÅÂÀÌ»ó ¹ß»ý Msg.
			//
			GLMSG::SNETPC_STATEBLOW_BRD NetMsgState;
			NetMsgState.emCrow = m_TargetID.emCrow;
			NetMsgState.dwID = m_TargetID.dwID;
			NetMsgState.emBLOW = sBlow.emTYPE;
			NetMsgState.fAGE = fLIFE;
			NetMsgState.fSTATE_VAR1 = sBlow.fVAR1;
			NetMsgState.fSTATE_VAR2 = sBlow.fVAR2;

			//	Note : '½ºÅ³´ë»ó'ÀÇ ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
			//
			pACTOR->SNDMSGAROUND ( (NET_MSG_GENERIC*) &NetMsgState );

			//	Note : ½ºÅ³ ´ë»ó¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
			if ( m_TargetID.emCrow==CROW_PC )	m_pGLGaeaServer->SENDTOCLIENT ( pACTOR->GETCLIENTID (), &NetMsgState );
		}
	}

	/* skill illusion, Juver, 2021/01/17 */
	pACTOR->ReactDamage( this, dwDamageFlag );

	// °­ÇÑ Å¸°Ý
	if ( dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW ) 
	{
		STARGETID dwActorID = STARGETID(CROW_PC,m_dwGaeaID);
		m_pLandMan->PushPullAct( dwActorID, m_TargetID, GLCONST_CHAR::fCRUSH_BLOW_RANGE );
	}

	/*curse skill logic, Juver, 2017/06/11 */
	if ( m_pLandMan && m_bCurse && m_fCurseDamage != 0.0f )
	{
		int _nCurseDamage = int(nDAMAGE*m_fCurseDamage);

		SSKILLACT sSKILLACTCURSE;
		sSKILLACTCURSE.sID = m_TargetID;
		sSKILLACTCURSE.sID_TAR = STARGETID(CROW_PC,m_dwGaeaID);
		sSKILLACTCURSE.fDELAY = 0.2f;
		sSKILLACTCURSE.emAPPLY = SKILL::EMAPPLY_PHY_LONG;
		sSKILLACTCURSE.nVAR_HP = -_nCurseDamage;
		sSKILLACTCURSE.dwDamageFlag = DAMAGE_TYPE_NONE;		

		if ( sSKILLACTCURSE.VALID() )
		{
			m_pLandMan->RegSkillAct ( sSKILLACTCURSE );
		}
	}

}

void GLChar::DamageReflectionProc ( int nDAMAGE, STARGETID sACTOR )
{
	/*block back damage, Juver, 2017/12/12 */
	if ( RPARAM::bBlockDamageBack && m_pLandMan )
	{
		GLACTOR* pactor_target = GLGaeaServer::GetInstance().GetTarget( m_pLandMan, sACTOR );
		if ( pactor_target )
		{
			if ( pactor_target->IsBlockDamageBack() )	return;
			pactor_target->SetBlockDamageBack( true );
		}
	}

	if ( sACTOR.emCrow==CROW_PC )
	{
		nDAMAGE = int(nDAMAGE*GLCONST_CHAR::fPK_POINT_DEC_PHY[ CharClassToClassIndex( m_emClass )]);
		if ( nDAMAGE==0 )	nDAMAGE = 1;
	}

	ToDamage ( sACTOR, nDAMAGE, FALSE );

	//	Note : ÀÚ½Å¿¡°Ô.
	GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
	NetMsg.emTarCrow		= sACTOR.emCrow;
	NetMsg.dwTarID			= sACTOR.dwID;
	NetMsg.nDamage			= nDAMAGE;
	NetMsg.dwDamageFlag		= DAMAGE_TYPE_NONE;
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

	//	Note : ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	//
	GLMSG::SNETPC_ATTACK_DAMAGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID		= m_dwGaeaID;
	NetMsgBrd.emTarCrow		= sACTOR.emCrow;
	NetMsgBrd.dwTarID		= sACTOR.dwID;
	NetMsgBrd.nDamage		= nDAMAGE;
	NetMsgBrd.dwDamageFlag	= DAMAGE_TYPE_NONE;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
}

void GLChar::DefenseSkill( SNATIVEID sNativeID, WORD wLevel, STARGETID sACTOR )
{
	if ( sNativeID == NATIVEID_NULL() ) return;

	GLMSG::SNETPC_DEFENSE_SKILL_ACTIVE NetMsg;
	NetMsg.emTarCrow = sACTOR.emCrow;
	NetMsg.dwTarID = sACTOR.dwID;
	NetMsg.sNativeID = sNativeID;
	NetMsg.wLevel = wLevel;
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

	m_bDefenseSkill	= true;

    return;
}

BOOL GLChar::AttackProcess ( float fElapsedTime )
{
	m_fattTIMER += fElapsedTime*GLCHARLOGIC_SERVER::GETATTVELO();
	m_fattTIMER += GLCHARLOGIC_SERVER::GETATT_ITEM();

	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	/*skill transform, Juver, 2018/09/09 */
	EMANI_SUBTYPE emANISUBTYPE = CHECK_ATTACK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[AN_ATTACK][emANISUBTYPE];
	if ( vecAniAttack.empty() )	return FALSE;

	//	Note : °ø°Ý ¸ð¼Ç Á¾·ù°¡ º¯°æµÈ °ÍÀ¸·Î º¸ÀÓ. °ø°Ý Ãë¼Ò. ( Àåºñ ÂûÅ¹. )
	//
	if ( vecAniAttack.size() <= m_dwANISUBSELECT )
	{
		TurnAction ( GLAT_IDLE );

		//GLMSG::SNET_ACTION_BRD NetMsgBrd;
		//NetMsgBrd.emCrow = CROW_PC;
		//NetMsgBrd.dwID = m_dwGaeaID;
		//NetMsgBrd.emAction = GLAT_IDLE;
		//m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgBrd );

		//SendMsgViewAround ( (NET_MSG_GENERIC *) &NetMsgBrd );

		return FALSE;
	}

	if ( !ISLONGRANGE_ARMS() )
	{
		GLACTOR *pTARGET = m_pGLGaeaServer->GetTarget ( m_pLandMan, m_TargetID );
		if ( !pTARGET )
		{
			TurnAction ( GLAT_IDLE );

			//	Note : ÀÚ½Å¿¡°Ô.
			GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
			NetMsg.emTarCrow		= m_TargetID.emCrow;
			NetMsg.dwTarID			= m_TargetID.dwID;
			NetMsg.nDamage			= 0;
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );
			SendMsgViewAround ( (NET_MSG_GENERIC *) &NetMsg );

			////	Note : ¸ð¼Ç º¯°æ.
			//GLMSG::SNET_ACTION_BRD NetMsgBrd;
			//NetMsgBrd.emCrow = CROW_PC;
			//NetMsgBrd.dwID = m_dwGaeaID;
			//NetMsgBrd.emAction = GLAT_IDLE;
			//m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgBrd );

			////	Note : ¸ð¼Ç º¯°æ (¸ðµÎ¿¡°Ô).
			//SendMsgViewAround ( (NET_MSG_GENERIC *) &NetMsgBrd );

			return FALSE;
		}

		D3DXVECTOR3 vDist = m_vPos - pTARGET->GetPosition();
		float fTarLength = D3DXVec3Length(&vDist);

		WORD wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETATTACKRANGE() + 2;
		if ( wAttackRange*GLCONST_CHAR::fREACT_VALID_SCALE < (WORD)(fTarLength) )
		{
			TurnAction ( GLAT_IDLE );

			//	Note : ÀÚ½Å¿¡°Ô.
			GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
			NetMsg.emTarCrow		= m_TargetID.emCrow;
			NetMsg.dwTarID			= m_TargetID.dwID;
			NetMsg.nDamage			= 0;
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );
			SendMsgViewAround ( (NET_MSG_GENERIC *) &NetMsg );

			////	Note : ¸ð¼Ç º¯°æ.
			//GLMSG::SNET_ACTION_BRD NetMsgBrd;
			//NetMsgBrd.emCrow = CROW_PC;
			//NetMsgBrd.dwID = m_dwGaeaID;
			//NetMsgBrd.emAction = GLAT_IDLE;
			//m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgBrd );

			////	Note : ¸ð¼Ç º¯°æ (¸ðµÎ¿¡°Ô).
			//SendMsgViewAround ( (NET_MSG_GENERIC *) &NetMsgBrd );

			return FALSE;
		}
	}


	//	Note : Æ¯Á¤ °ø°Ý ¿¡´Ï¸ÞÀÌ¼Ç ¼³Á¤ ¼±ÅÃ.
	//
	const SANIATTACK &sAniAttack = vecAniAttack[m_dwANISUBSELECT];

	DWORD dwThisKey = sAniAttack.m_dwSTime + DWORD(m_fattTIMER*UNITANIKEY_PERSEC);
	
	if ( !m_sHITARRAY.empty() )
	{
		SSTRIKE sStrike = *m_sHITARRAY.begin();

		if ( dwThisKey >= sStrike.wDivKey )
		{
			if ( sStrike.bHit )
			{
				int nDamage = 0;
				DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
				dwDamageFlag = CALCDAMAGE ( nDamage, m_dwGaeaID, m_TargetID, m_pLandMan, NULL, 0, NULL, sAniAttack.m_wDivCount );
				DamageProc ( nDamage, dwDamageFlag, sStrike.bLowSP );

				/* skill stigma, Juver, 2020/12/13 */
				ProcessSkillStigmaTypeAttack( sAniAttack.m_wDivCount, sStrike.bLowSP );
			}
			else
			{
				AvoidProc ( NATIVEID_NULL(), sStrike.bLowSP );
			}

			//	Å¥ µ¥ÀÌÅ¸°¡ Ã³¸®µÈ °æ¿ì »èÁ¦ÇÑ´Ù.
			m_sHITARRAY.pop_front ();
		}	
	}

	//	ÀÌ»óÁõ»ó¿¡ ÀÇÇØ µ¥ÀÌÅ¸°¡ ÀüÇô Ã³¸®µÇÁö ¾ÊÀº°æ¿ì,
	//	¸¶Áö¸·¿¡ ÇÑ²¨¹ø¿¡ Ã³¸®ÇÏ°í, FALSE¸¦ ¸®ÅÏÇÑ´Ù.
	if ( dwThisKey >= sAniAttack.m_dwETime )
	{
		if ( !m_sHITARRAY.empty() )
		{
			while ( !m_sHITARRAY.empty() )
			{
				SSTRIKE sStrike = *m_sHITARRAY.begin();
				
				if ( sStrike.bHit )
				{
					int nDamage = 0;
					DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
					dwDamageFlag = CALCDAMAGE ( nDamage, m_dwGaeaID, m_TargetID, m_pLandMan, NULL, 0, NULL, sAniAttack.m_wDivCount );
					DamageProc ( nDamage, dwDamageFlag, sStrike.bLowSP );

					/* skill stigma, Juver, 2020/12/13 */
					ProcessSkillStigmaTypeAttack( sAniAttack.m_wDivCount, sStrike.bLowSP );
				}
				else
				{
					AvoidProc ( NATIVEID_NULL(), sStrike.bLowSP );
				}

				//	Å¥ µ¥ÀÌÅ¸°¡ Ã³¸®µÈ °æ¿ì »èÁ¦ÇÑ´Ù.
				m_sHITARRAY.pop_front ();
			}
		}

		return FALSE;
	}

	//	Note : ¾ÆÁ÷ °ø°ÝÀÌ Á¾·á µÇÁö ¾Ê¾ÒÀ¸³ª hit´Â ¿Ï·áµÌ°í ¿¡´ÏÅ°°¡ ÀÏÁ¤ ¼ö·® ÀÌÇÏÀÏ °æ¿ì
	//		°ø°ÝÀÌ Á¾·áµÈ°É·Î °£ÁÖ. ( Å¬¶óÀÌ¾ðÆ®¿Í ¼­¹ö°£¿¡ µô·¹ÀÌ °¨¾È. )
	if ( m_sHITARRAY.empty() )
	{
		//DWORD dwDISKEY = sAniAttack.m_dwETime - dwThisKey;
		//if ( dwDISKEY <= 5 )		return FALSE;

		return FALSE;
	}

	return TRUE;
}

// *****************************************************
// Desc: ºÎÈ°½ºÅ³ ½Ãµ¿
// *****************************************************
WORD GLChar::DoRevive2Skill ( SNATIVEID skill_id, WORD wSKILL_LVL, WORD wSPEC )
{
	if ( IsValidBody() )					return 0;
	if ( SKILL::MAX_SKILL_LEVEL <= wSKILL_LVL )	return 0;
	if ( !IsSTATE(EM_ACT_DIE) )				return 0;
	if ( wSPEC >= SKILL::MAX_SPEC )			return 0;

	//	½ºÅ³ Á¤º¸ °¡Á®¿È.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id );
	const SKILL::SSPEC &sSPEC = pSkill->m_sAPPLY.sSpecs[wSPEC].sSPEC[wSKILL_LVL];

    //	Note : ºÎÈ° map°ú ºÎÈ° gate ÁöÁ¤, À§Ä¡´Â ÀÇ¹Ì ¾ø´Â °ª.
	BOOL bOk = m_pGLGaeaServer->RequestReBirth ( m_dwGaeaID, m_pLandMan->GetMapID(), UINT_MAX, m_vPos );
	if ( !bOk )
	{
		DEBUGMSG_WRITE ( "Ä³¸¯ÅÍ ºÎÈ° ½ÃµµÁß¿¡ ¿À·ù°¡ ¹ß»ý! m_pGLGaeaServer->RequestReBirth ()" );
		return 0;
	}

	//	ºÎÈ° ¿Ï·á Ã¼Å©.
	ReSetSTATE(EM_ACT_DIE);

	//	ºÎÈ°½Ã Ã¤·Â È¸º¹.
	m_fGenAge = 0.0f;
	GLCHARLOGIC_SERVER::INIT_RECOVER(int(sSPEC.fVAR1));
	TurnAction ( GLAT_IDLE );

	//	Note : ºÎÈ°½Ã °æÇèÄ¡ °¨¼Ò.
	ReBirthDecExp ();

	// PET
	// ¸ÊÀÌµ¿½Ã Pet ¹× Vehicle »èÁ¦
	m_pGLGaeaServer->ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,true) );
	//m_pGLGaeaServer->ReserveDropOutSummon ( SDROPOUTSUMMONINFO(m_dwSummonGUID,true) );
	m_pGLGaeaServer->SaveVehicle( m_dwClientID, m_dwGaeaID, true );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		m_pGLGaeaServer->ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );


	//	ºÎÈ° È®ÀÎ ¸Þ½ÃÁö.
	GLMSG::SNETPC_REQ_REBIRTH_FB NetMsg;
	NetMsg.sMapID = m_pLandMan->GetMapID();
	NetMsg.vPos = GetPosition();
	NetMsg.wNowHP = m_sHP.wNow;
	NetMsg.wNowMP = m_sMP.wNow;
	NetMsg.wNowSP = m_sSP.wNow;

	m_pGLGaeaServer->SENDTOAGENT(m_dwClientID,&NetMsg);
	return m_sHP.wNow;
}

// *****************************************************
// Desc: ½ºÅ³ ¿µÇâÀ» ÄÉ¸¯ÅÍ¿¡ Àû¿ë½ÃÅ²´Ù.
// *****************************************************
void GLChar::SkillProc ( SNATIVEID skill_id, BOOL bLowSP )
{
	//CONSOLEMSG_WRITE( "SkillProc Start [%u]", skill_id.dwID );

	//	Note : Ä³¸¯ÅÍ°¡ ¹è¿î ½ºÅ³ Á¤º¸ °¡Á®¿È.
	//
	SCHARSKILL sSkill;
	WORD wSKILL_LVL_MAIN = 0;
	m_fAccumTime = 0.0f;

	if ( IsDefenseSkill() )
	{
		if ( skill_id != m_sDefenseSkill.m_dwSkillID ) return;
		sSkill.sNativeID = skill_id;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
		wSKILL_LVL_MAIN = m_sDefenseSkill.m_wLevel;;

	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
		if ( learniter==m_ExpSkills.end() )	return;
		sSkill = (*learniter).second;
		wSKILL_LVL_MAIN = sSkill.wLevel;
	}

	//	Note : ¿¡´Ï¸ÞÀÌ¼Ç Á¤º¸.
	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[m_SKILLMTYPE][m_SKILLSTYPE];
	if ( vecAniAttack.empty() )	return;

	//	Note : Æ¯Á¤ °ø°Ý ¿¡´Ï¸ÞÀÌ¼Ç ¼³Á¤ ¼±ÅÃ. (¼öÁ¤¿ä)
	//
	const SANIATTACK &sAniAttack = vecAniAttack[0];

	//	½ºÅ³ Á¤º¸ °¡Á®¿È.
	/*multi skill Juver, 2018/09/04 */
	PGLSKILL pskill_main = GLSkillMan::GetInstance().GetData ( skill_id );
	if ( pskill_main )
	{
		SKILL::CDATA_LVL &sSKILL_DATA = pskill_main->m_sAPPLY.sDATA_LVL[wSKILL_LVL_MAIN];

		//	Note : SP°¡ ºÎÁ·ÇÏÁö ¾ÊÀ»¶§¿¡´Â SP ¼Ò¸ð½ÃÅ´.
		//
		if ( !bLowSP )
		{
			WORD wDisSP = m_wSUM_DisSP + sSKILL_DATA.wUSE_SP;
			m_sSP.DECREASE ( wDisSP );
		}

		int nEXP_NUM = 0;
		int nALL_LVL = 0;
		int nALL_VARHP = 0, nALL_FULLHP = 0;
		int nALL_VARMP = 0, nALL_FULLMP = 0;
		int nALL_VARSP = 0, nALL_FULLSP = 0;

		WORD wMAX_TAR = 0;
		if ( pskill_main->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC )		wMAX_TAR = sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1;
		else if ( pskill_main->m_sBASIC.emIMPACT_REALM == REALM_FANWIZE )	wMAX_TAR = sSKILL_DATA.wTARNUM * ( sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1 );
		else																wMAX_TAR = sSKILL_DATA.wTARNUM;

		if ( m_wTARGET_NUM_MAIN > wMAX_TAR )		m_wTARGET_NUM_MAIN = wMAX_TAR;

		/*dash skill logic, Juver, 2017/06/17 */
		BOOL bACTDASH = FALSE;

		/* skill position shift, Juver, 2020/12/15 */
		BOOL bPositionShift = FALSE;

		/*skill teleport, Juver, 2018/09/06 */
		BOOL bTeleport = FALSE;

		//	Note : ½ºÅ³ÀÌ Àû¿ëµÇ´Â Å¸°Ù °Ë»ç.
		//
		STARGETID sTARID;
		for ( WORD i=0; i<m_wTARGET_NUM_MAIN; ++i )
		{
			const STARID &_sTARID = m_sTARGET_IDS_MAIN[i];

			sTARID.emCrow = _sTARID.GETCROW();
			sTARID.dwID = _sTARID.GETID();
			GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTARID );
			if ( !pACTOR )									continue;

			bool bCrushBlowOne = false;

			/* immune skill logic, Juver, 2020/12/24 */
			DAMAGE_SPEC*	pDamageSpec = pACTOR->GetDamageSpecPtr();
			if ( pDamageSpec )
			{
				bool bImmune = false;

				if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_SHORT ) && pskill_main->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_SHORT )
					bImmune = true;

				if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_LONG ) && pskill_main->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_LONG )
					bImmune = true;

				if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_MAGIC ) && pskill_main->m_sBASIC.emAPPLY == SKILL::EMAPPLY_MAGIC )
					bImmune = true;

				if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_NORMAL ) && pskill_main->m_sBASIC.emACTION == SKILL::EMACTION_NORMAL )
					bImmune = true;

				if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_BUFF ) && pskill_main->m_sBASIC.emACTION == SKILL::EMACTION_BUFF )
					bImmune = true;

				if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_DEBUFF ) && pskill_main->m_sBASIC.emACTION == SKILL::EMACTION_DEBUFF )
					bImmune = true;

				if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_LIMIT ) && pskill_main->m_sBASIC.emACTION == SKILL::EMACTION_LIMIT )
					bImmune = true;

				if ( bImmune )
				{
					GLMSG::SNETPC_ATTACK_AVOID NetMsg;
					NetMsg.emTarCrow	= sTARID.emCrow;
					NetMsg.dwTarID		= sTARID.dwID;

					GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

					GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
					NetMsgBrd.dwGaeaID	= m_dwGaeaID;
					NetMsgBrd.emTarCrow	= sTARID.emCrow;
					NetMsgBrd.dwTarID	= sTARID.dwID;

					SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

					continue;
				}
			}

			for ( WORD j=0; j<sSKILL_DATA.wAPPLYNUM; ++j )
			{
				if ( pACTOR->GetNowHP()==0 )				continue;

				//	Note : ¹°¸® µ¥¹ÌÁö ¹ß»ý.
				//
				DWORD dwDamageFlag = DAMAGE_TYPE_NONE;

				int nVAR_HP(0), nVAR_MP(0), nVAR_SP(0);

				//	ÀúÇ×Ä¡.
				short nRESIST = pACTOR->GETRESIST().GetElement(pskill_main->m_sAPPLY.emELEMENT);
				if ( nRESIST>99 )	nRESIST = 99;

				//	SKILL ±âº» Àû¿ë.
				//
				switch ( pskill_main->m_sAPPLY.emBASIC_TYPE )
				{
				case SKILL::EMFOR_HP:
					//	´ë»ó¿¡°Ô ÇÇÇØ¸¦ ÁÖ´Â ½ºÅ³.
					if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
					{
						//					bCRITICAL = CALCDAMAGE ( nVAR_HP, bShock, sTARID, m_pLandMan, pSkill, wSKILL_LVL, GLPeriod::GetInstance().GetWeather(), sAniAttack.m_wDivCount );
						DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
						dwDamageFlag = CALCDAMAGE ( nVAR_HP, m_dwGaeaID, sTARID, m_pLandMan, pskill_main, wSKILL_LVL_MAIN, dwWeather, sAniAttack.m_wDivCount );
						
						/* skill illusion, Juver, 2021/01/17 */
						pACTOR->ReactDamage( this, dwDamageFlag );
						
						nVAR_HP = - nVAR_HP;
					}
					//	´ë»óÀ» È¸º¹½ÃÄÑ ÁÖ´Â ½ºÅ³.
					else
					{
						int nDX = pACTOR->GetMaxHP() - pACTOR->GetNowHP();
						nVAR_HP += (int) min(nDX,sSKILL_DATA.fBASIC_VAR);
					}
					break;

				case SKILL::EMFOR_MP:
					//	´ë»ó¿¡°Ô ÇÇÇØ¸¦ ÁÖ´Â ½ºÅ³.
					if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
					{
						int nVAR = int(-sSKILL_DATA.fBASIC_VAR);
						nVAR_MP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
					}
					//	´ë»óÀ» È¸º¹½ÃÄÑ ÁÖ´Â ½ºÅ³.
					else
					{
						int nDX = pACTOR->GetMaxMP() - pACTOR->GetNowMP();
						nVAR_MP += (int) min(nDX,sSKILL_DATA.fBASIC_VAR);
					}
					break;

				case SKILL::EMFOR_SP:
					//	´ë»ó¿¡°Ô ÇÇÇØ¸¦ ÁÖ´Â ½ºÅ³.
					if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
					{
						int nVAR = int(-sSKILL_DATA.fBASIC_VAR);
						nVAR_SP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
					}
					//	´ë»óÀ» È¸º¹½ÃÄÑ ÁÖ´Â ½ºÅ³.
					else
					{
						int nDX = pACTOR->GetMaxSP() - pACTOR->GetNowSP();
						nVAR_SP += (int) min(nDX,sSKILL_DATA.fBASIC_VAR);
					}
					break;
				};

				if ( dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW ) bCrushBlowOne = true;

				//	Note : SP ºÎÁ·½Ã¿¡´Â º¯È­ °ªÀ» ¹ÝÀ¸·Î Á¶Á¤.
				//
				if ( bLowSP )
				{
					nVAR_HP /= 2;
					nVAR_MP /= 2;
					nVAR_SP /= 2;
				}

				//	Note : ½ºÅ³ Æ¯¼ö ±â´É.
				//
				int nGATHER_HP(0), nGATHER_MP(0), nGATHER_SP(0);

				for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
				{
					const SKILL::SSPEC &sSKILL_SPEC = pskill_main->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_MAIN];

					switch ( pskill_main->m_sAPPLY.sSpecs[nSpec].emSPEC )
					{
					case EMSPECA_HP_GATHER:
						nGATHER_HP += int ( abs(nVAR_HP) * sSKILL_SPEC.fVAR1 );
						break;

					case EMSPECA_MP_GATHER:
						nGATHER_MP += int ( abs(nVAR_MP) * sSKILL_SPEC.fVAR1 );
						break;

					case EMSPECA_SP_GATHER:
						nGATHER_SP += int ( abs(nVAR_SP) * sSKILL_SPEC.fVAR1 );
						break;
					};
				}


				//	Note : ÀÚ±â ÀÚ½Å¿¡°Ô ¾´ ½ºÅ³Àº Á¦¿ÜÇÏ°í EXP ÀÚ·á¸¦ ¼öÁý.
				//
				if ( sTARID!=STARGETID(CROW_PC,m_dwGaeaID) )
				{
					++nEXP_NUM;
					nALL_LVL += pACTOR->GetLevel ();

					nALL_VARHP += nVAR_HP;
					nALL_VARMP += nVAR_MP;
					nALL_VARSP += nVAR_SP;

					nALL_FULLHP += pACTOR->GetMaxHP();
					nALL_FULLMP += pACTOR->GetMaxMP();
					nALL_FULLSP += pACTOR->GetMaxSP();
				}

				//	Note : ½ºÅ³ ±âº» ¿µÇâ.
				//
				SSKILLACT sSKILLACT;
				sSKILLACT.sID = STARGETID(CROW_PC,m_dwGaeaID);
				sSKILLACT.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
				sSKILLACT.fDELAY = pskill_main->m_sEXT_DATA.fDELAY4DAMAGE;
				sSKILLACT.emAPPLY = pskill_main->m_sBASIC.emAPPLY;
				sSKILLACT.dwDamageFlag = dwDamageFlag;			

				//	Note : Ã¼·Â º¯È­ ÀúÀå.
				//
				sSKILLACT.nVAR_HP = nVAR_HP;
				sSKILLACT.nVAR_MP = nVAR_MP;
				sSKILLACT.nVAR_SP = nVAR_SP;

				//	Note : Ã¼·Â Èí¼ö ÀúÀå.
				//
				sSKILLACT.nGATHER_HP = nGATHER_HP;
				sSKILLACT.nGATHER_MP = nGATHER_MP;
				sSKILLACT.nGATHER_SP = nGATHER_SP;

				if ( sSKILLACT.VALID() )
				{
					m_pLandMan->RegSkillAct ( sSKILLACT );
				}
			}

			//	Note : ½ºÅ³ º¸Á¶ ¿µÇâ.
			//
			SSKILLACTEX sSKILLACTEX;
			sSKILLACTEX.sID = STARGETID(CROW_PC,m_dwGaeaID);
			sSKILLACTEX.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
			sSKILLACTEX.fDELAY = pskill_main->m_sEXT_DATA.fDELAY4DAMAGE;

			sSKILLACTEX.idSKILL = skill_id;
			sSKILLACTEX.wSKILL_LVL = wSKILL_LVL_MAIN;

			//  °­ÇÑ Å¸°Ý È®·ü
			if ( bCrushBlowOne )
			{
				sSKILLACTEX.bCrushBlow = bCrushBlowOne;
				sSKILLACTEX.fPUSH_PULL = GLCONST_CHAR::fCRUSH_BLOW_RANGE;
			}

			//	Note : »óÅÂ ÀÌ»ó À¯¹ß.
			//
			ITEM::SSATE_BLOW sBLOW;
			sBLOW.emTYPE = EMBLOW_NONE;

			if ( pskill_main->m_sAPPLY.emELEMENT==EMELEMENT_ARM )
			{
				SITEM *pITEM = GET_ELMT_ITEM ();
				if ( pITEM )	sBLOW = pITEM->sSuitOp.sBLOW;
			}
			else
			{
				const SKILL::SSTATE_BLOW &sSKILL_BLOW = pskill_main->m_sAPPLY.sSTATE_BLOW[wSKILL_LVL_MAIN];

				sBLOW.emTYPE = pskill_main->m_sAPPLY.emSTATE_BLOW;
				sBLOW.fRATE = sSKILL_BLOW.fRATE;
				sBLOW.fLIFE = pskill_main->m_sAPPLY.sDATA_LVL[wSKILL_LVL_MAIN].fLIFE;
				sBLOW.fVAR1 = sSKILL_BLOW.fVAR1;
				sBLOW.fVAR2 = sSKILL_BLOW.fVAR2;
			}

			if ( sBLOW.emTYPE!=EMBLOW_NONE )
			{
				//	Note : ¹ß»ý È®À² °è»ê.
				//
				short nBLOWRESIST = pACTOR->GETRESIST().GetElement ( STATE_TO_ELEMENT(sBLOW.emTYPE) );
				if ( nBLOWRESIST>99 )	nBLOWRESIST = 99;

				//			float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBLOW.emTYPE, GLPeriod::GetInstance().GetWeather(), m_pLandMan->IsWeatherActive() );
				DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
				float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBLOW.emTYPE, dwWeather, m_pLandMan->IsWeatherActive() );

				BOOL bBLOW(FALSE);
				if ( !(pACTOR->GETHOLDBLOW()&STATE_TO_DISORDER(sBLOW.emTYPE)) )
				{
					bBLOW = GLOGICEX::CHECKSTATEBLOW ( sBLOW.fRATE * fPOWER, GETLEVEL(), pACTOR->GetLevel(), nBLOWRESIST );
				}

				if ( bBLOW )
				{
					//	Note : »óÅÂÀÌ»ó ¹ß»ý.
					//	
					SSTATEBLOW sSTATEBLOW;
					float fLIFE = sBLOW.fLIFE * fPOWER;
					fLIFE = ( fLIFE - (fLIFE*nBLOWRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );

					sSTATEBLOW.emBLOW = sBLOW.emTYPE;
					sSTATEBLOW.fAGE = fLIFE;
					sSTATEBLOW.fSTATE_VAR1 = sBLOW.fVAR1;
					sSTATEBLOW.fSTATE_VAR2 = sBLOW.fVAR2;

					//	Note : »óÅÂ ÀÌ»ó ¹ß»ý ÀúÀå.
					//
					sSKILLACTEX.sSTATEBLOW = sSTATEBLOW;
				}
			}

			//	Note : ½ºÅ³ Æ¯¼ö ±â´É.
			//
			BOOL bActionMovement = FALSE;
			BOOL bCUREFLAG = FALSE;
			BOOL bREMOVEFLAG = FALSE;
			BOOL bREBIRTH = FALSE;

			/*skill vehicle off, Juver, 2018/09/07 */
			BOOL skill_vehicle_off = FALSE;

			/*skill pet off, Juver, 2018/09/07 */
			BOOL skill_pet_off = FALSE;

			/* skill delay addition, Juver, 2020/12/09 */
			BOOL bSkillDelay = FALSE;

			/* skill buff duration change, Juver, 2020/12/16 */
			BOOL bDurationChange = FALSE;

			if ( bCrushBlowOne )	bActionMovement = TRUE;

			SNATIVEID sUseSkillID = pskill_main->m_sBASIC.sNATIVEID;

			for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
			{
				const SKILL::SSPEC &sSKILL_SPEC = pskill_main->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_MAIN];
				switch ( pskill_main->m_sAPPLY.sSpecs[nSpec].emSPEC )
				{
				case EMSPECA_PUSHPULL:
					{
						if ( bActionMovement )	break;
						if ( !pACTOR->IsValidBody() )				break;
						if ( RANDOM_POS > sSKILL_SPEC.fVAR2 )		break;	//	Note : ¹ß»ýÈ®À² Àû¿ë.
						sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR1;
						sSKILLACTEX.bPushPull = true;
						bActionMovement = TRUE;
					}break;

				case EMSPECA_CRUSHING_BLOW:
					{
						if ( bActionMovement )	break;
						if ( sSKILL_SPEC.fVAR2 != 0.0f && RANDOM_POS > sSKILL_SPEC.fVAR2 )		break;
						sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR1;
						sSKILLACTEX.bCrushingBlow = true;
						bActionMovement = TRUE;
					}break;

					/* push skill logic, Juver, 2020/12/14 */
				case EMSPECA_PUSH:
					{
						if ( bActionMovement )			break;
						if ( !pACTOR->IsValidBody() )	break;
						if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
						sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //distance
						sSKILLACTEX.bPush = true;
						bActionMovement = TRUE;
					}break;

					/* pull skill logic, Juver, 2020/12/14 */
				case EMSPECA_PULL:
					{
						if ( bActionMovement )			break;
						if ( !pACTOR->IsValidBody() )	break;
						if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
						sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //pull speed
						sSKILLACTEX.bPull = true;
						bActionMovement = TRUE;
					}break;

					/* dash skill logic, Juver, 2020/12/14 */
				case EMSPECA_SKILL_DASH:
					{
						if ( bACTDASH )					break;
						if ( !pACTOR->IsValidBody() )	break;
						if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
						sSKILLACTEX.fDashSpeed = sSKILL_SPEC.fVAR2; //dash speed
						sSKILLACTEX.dwActionAnim = sSKILL_SPEC.dwFLAG; //animation
						sSKILLACTEX.bDash = true;
						bACTDASH = TRUE;
					}break;

					/*skill teleport, Juver, 2018/09/06 */
				case EMSPECA_TELEPORT:
					{
						if ( bTeleport )			break;
						if ( !pACTOR->IsValidBody() )	break;
						sSKILLACTEX.bUseTeleport = true;
						sSKILLACTEX.vTeleportPos = m_vTARGET_POS_MAIN;
						bTeleport = TRUE;
					}break;

					/*skill position shift, Juver, 2018/09/06 */
				case EMSPECA_POSITION_SHIFT:
					{
						if ( bPositionShift )			break;
						if ( !pACTOR->IsValidBody() )	break;
						if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;

						sSKILLACTEX.fPositionShiftSpeed = sSKILL_SPEC.fVAR2;

						if ( sSKILL_SPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_SWAP )
							sSKILLACTEX.bPositionShiftSwap = true;
						else if ( sSKILL_SPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_DASH )
							sSKILLACTEX.bPosisionShiftDash = true;
						else if ( sSKILL_SPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_PULL )
							sSKILLACTEX.bPositionShiftPull = true;

						bPositionShift = TRUE;
					}break;

				case EMSPECA_HP_DIV:
				case EMSPECA_MP_DIV:
				case EMSPECA_SP_DIV:
					break;

				case EMSPECA_RECBLOW:
					{
						//	Note : »óÅÂ ÀÌ»ó Ä¡·á.
						//
						if ( bCUREFLAG )	break;
						sSKILLACTEX.dwCUREFLAG |= pskill_main->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_MAIN].dwFLAG;
						bCUREFLAG = TRUE;
					}break;

					/*debuff skill logic, Juver, 2017/06/10 */
				case EMSPECA_BUFF_REMOVE:
					{
						if ( bREMOVEFLAG )	break;
						if ( sSKILLACTEX.bDebuff )	break;
						sSKILLACTEX.dwRemoveFlag = pskill_main->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_MAIN].dwFLAG;
						sSKILLACTEX.bDebuff = true;
						bREMOVEFLAG = TRUE;
					}break;

				case EMSPECA_REBIRTH:
					{
						if ( bREBIRTH )	break;

						if ( sTARID.emCrow==CROW_PC )
						{
							PGLCHAR pPC = m_pGLGaeaServer->GetChar(sTARID.dwID);
							if ( pPC )
							{
								BOOL bBlockResu = FALSE;

								if ( m_pLandMan )
								{
									/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
									if ( m_pLandMan->m_bPVPTyrannyMap && m_wSchool != pPC->m_wSchool )
										bBlockResu = GLPVPTyrannyField::GetInstance().isDisableOtherSchoolResu();

									if ( m_pLandMan->m_bPVPTyrannyMap && m_dwGuild != pPC->m_dwGuild )
										bBlockResu = GLPVPTyrannyField::GetInstance().isDisableOtherGuildResu();

									/*PPVPSchoolWars enemy resu, EJCode, 2018/10/22 */
									if ( m_pLandMan->m_bPVPSchoolWarsMap && m_wSchool != pPC->m_wSchool )
										bBlockResu = GLPVPSchoolWarsField::GetInstance().isDisableOtherSchoolResu();

									/*PVPCaptureTheFlag enemy resu, EJCode, 2018/10/23 */
									if ( m_pLandMan->m_bPVPCaptureTheFlagMap && m_wCaptureTheFlagTeam != pPC->m_wCaptureTheFlagTeam )
										bBlockResu = GLPVPCaptureTheFlagField::GetInstance().isDisableOtherTeamResu();

									/* pvp club death match, Juver, 2020/11/25 */
									if ( m_pLandMan->m_bPVPClubDeathMatchBattleMap && m_dwGuild != pPC->m_dwGuild )
										bBlockResu = PVPClubDeathMatchField::GetInstance().isDisableOtherGuildResu();

									if ( m_pLandMan->m_bPVPPBGBattleMap && m_dwPartyID != pPC->m_dwPartyID )
										bBlockResu = PVPPBG::ManagerField::GetInstance().isDisableOtherResu();
								}

								if ( !bBlockResu )
								{
									WORD wRECOVE = pPC->DoRevive2Skill(skill_id,wSKILL_LVL_MAIN, (WORD)nSpec );
									if ( 0<wRECOVE)
									{
										++nEXP_NUM;
										nALL_LVL += pPC->GetLevel ();
										nALL_VARHP += wRECOVE;
										nALL_FULLHP += pPC->GetMaxHP();

										if ( m_pLandMan )
										{
											/*pvp tyranny, Juver, 2017/08/24 */
											if ( m_pLandMan->m_bPVPTyrannyMap )
											{
												GLPVPTyrannyField::GetInstance().ScoreResu( m_sMapID, m_dwCharID );
												/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
												GLPVPTyrannyField::GetInstance().AddTyrannyScoreResu( m_dwCharID );
												GLPVPTyrannyField::GetInstance().AddTyrannyResuScoreResu( m_dwCharID );
												GLPVPTyrannyField::GetInstance().AddTyrannyClubScoreResu( m_dwGuild );
											}

											/*school wars, Juver, 2018/01/20 */
											if ( m_pLandMan->m_bPVPSchoolWarsMap )
												GLPVPSchoolWarsField::GetInstance().ScoreResu( m_dwCharID );

											/*pvp capture the flag, Juver, 2018/02/01 */
											if ( m_pLandMan->m_bPVPCaptureTheFlagMap )
												GLPVPCaptureTheFlagField::GetInstance().ScoreResu( m_dwCharID );

											/* pvp club death match, Juver, 2020/11/25 */
											if ( m_pLandMan->m_bPVPClubDeathMatchBattleMap )
												PVPClubDeathMatchField::GetInstance().ScoreResu( m_dwCharID );

											if ( m_pLandMan->m_bGuidBattleMap )
											{
												GLGuidanceFieldMan::GetInstance().ScoreResu( m_pLandMan->m_dwClubMapID, m_dwCharID );
											}

											if ( m_pLandMan->m_bPVPPBGBattleMap )
												PVPPBG::ManagerField::GetInstance().ScoreResu( m_dwCharID );

											bool bPVPTyrannyCombatRecord = false;
											if ( m_pLandMan->m_bPVPTyrannyMap && 
												GLPVPTyrannyField::GetInstance().IsBattle() && 
												GLPVPTyrannyField::GetInstance().isCombatRecord() )
											{
												bPVPTyrannyCombatRecord = true;
												UpdateResuIncrease( m_dwCharID );
											}

											bool bPVPSchoolWarsCombatRecord = false;
											if ( m_pLandMan->m_bPVPSchoolWarsMap && 
												GLPVPSchoolWarsField::GetInstance().IsBattle() && 
												GLPVPSchoolWarsField::GetInstance().isCombatRecord() )
											{
												bPVPSchoolWarsCombatRecord = true;
												UpdateResuIncrease( m_dwCharID );
											}

											bool bPVPCaptureTheFlagCombatRecord = false;
											if ( m_pLandMan->m_bPVPCaptureTheFlagMap && 
												GLPVPCaptureTheFlagField::GetInstance().IsBattle() && 
												GLPVPCaptureTheFlagField::GetInstance().isCombatRecord() )
											{
												bPVPCaptureTheFlagCombatRecord = true;
												UpdateResuIncrease( m_dwCharID );
											}

											bool bPVPClubDeathMatchCombatRecord = false;
											if ( m_pLandMan->m_bPVPClubDeathMatchBattleMap && 
												PVPClubDeathMatchField::GetInstance().IsBattle() && 
												PVPClubDeathMatchField::GetInstance().isCombatRecord() )
											{
												bPVPClubDeathMatchCombatRecord = true;
												UpdateResuIncrease( m_dwCharID );
											}

											bool bPVPPBGCombatRecord = false;
											if ( m_pLandMan->m_bPVPPBGBattleMap && 
												PVPPBG::ManagerField::GetInstance().IsBattle() && 
												PVPPBG::ManagerField::GetInstance().isCombatRecord() )
											{
												bPVPPBGCombatRecord = true;
												UpdateResuIncrease( m_dwCharID );
											}
											
											/*Top Resu, Mhundz */
											//UpdateResuIncrease( m_dwCharID );

											procResu( 
												m_dwCharID, 
												bPVPTyrannyCombatRecord, 
												m_pLandMan->m_bGuidBattleMap, 
												bPVPCaptureTheFlagCombatRecord, 
												( bPVPClubDeathMatchCombatRecord || m_pLandMan->m_bClubDeathMatchMap ),
												bPVPSchoolWarsCombatRecord, 
												m_pLandMan->isPKMapCount() );
										}
										
									}
								}
								
							}
						}

						bREBIRTH = TRUE;
					}break;

					/*skill vehicle off, Juver, 2018/09/07 */
				case  EMSPECA_VEHICLE_OFF:
					{
						if ( skill_vehicle_off )	break;

						if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;

						if ( sTARID.emCrow == CROW_PC )
						{
							PGLCHAR pchar_target = m_pGLGaeaServer->GetChar(sTARID.dwID);
							if ( pchar_target )	pchar_target->skill_remove_vehicle();
						}

						skill_vehicle_off = TRUE;
					}break;

					/*skill pet off, Juver, 2018/09/07 */
				case  EMSPECA_PET_OFF:
					{
						if ( skill_pet_off )	break;

						if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;

						if ( sTARID.emCrow == CROW_PC )
						{
							PGLCHAR pchar_target = m_pGLGaeaServer->GetChar(sTARID.dwID);
							if ( pchar_target )	pchar_target->skill_remove_pet();
						}

						skill_pet_off = TRUE;
					}break;

					/* skill delay addition, Juver, 2020/12/09 */
				case  EMSPECA_SKILLDELAY:
					{
						if ( bSkillDelay )	break;

						if ( sSKILL_SPEC.dwFLAG & SKILL::SPEC_SKILL_DELAY_TYPE_SINGLE )
						{
							if ( sTARID.emCrow == CROW_PC )
							{
								PGLCHAR pchar_target = m_pGLGaeaServer->GetChar(sTARID.dwID);
								if ( pchar_target )	pchar_target->ApplySkillDelayOnce( sUseSkillID, sSKILL_SPEC.fVAR1, sSKILL_SPEC.fVAR2 );
							}

							bSkillDelay = TRUE;
						}
					}break;

					/* skill buff duration change, Juver, 2020/12/16 */
				case  EMSPECA_DURATION_CHANGE:
					{
						if ( bDurationChange )	break;

						if ( sSKILL_SPEC.fVAR1 != 0.0f )
						{
							if ( pACTOR )
								pACTOR->ReceiveSkillBuffDurationChange( sUseSkillID, sSKILL_SPEC.dwFLAG, sSKILL_SPEC.fVAR1 );

							bDurationChange = TRUE;
						}
					}break;
				};
			}


			switch (pskill_main->m_sAPPLY.emBASIC_TYPE)
			{
			case SKILL::EMFOR_CURE:
				//	Note : »óÅÂ ÀÌ»ó Ä¡·á.
				//
				sSKILLACTEX.dwCUREFLAG |= pskill_main->m_sAPPLY.dwCUREFLAG;
				break;
			};

			if ( sSKILLACTEX.VALID() )
			{
				m_pLandMan->RegSkillActEx ( sSKILLACTEX );
				nALL_VARHP;
			}

			//	Note : È¸º¹¿¡ ±â¿©ÇßÀ» ¶§¸¸ °æÇèÄ¡ ¹ß»ý.	( °ø°Ý½Ã´Â Á¦¿Ü )
			//
			int nSUM_EXP = 0;
			bool bRECOVE = (nALL_VARHP>=0&&nALL_VARMP>=0&&nALL_VARSP&&0) && (nALL_VARHP+nALL_VARMP+nALL_VARSP)>0;

			if ( bRECOVE )
			{
				//	½ÇÁ¦ º¯È­·®ÀÌ ÀÖÀ» ¶§¸¸ °æÇèÄ¡¸¦ »êÃâ.
				if ( nALL_VARHP>0 )
					nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARHP,nALL_FULLHP,pACTOR->GetBonusExp());

				if ( nALL_VARMP>0 )
					nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARMP,nALL_FULLMP,pACTOR->GetBonusExp());

				if ( nALL_VARSP>0 )
					nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARSP,nALL_FULLSP,pACTOR->GetBonusExp());

				nSUM_EXP += int (nSUM_EXP*GLCONST_CHAR::fREC_EXP_RATE);
			}

			//	Note : °æÇèÄ¡ È¹µæ·® Àû¿ë.
			//
			if ( nSUM_EXP )
			{
				// ¾ÆÀÌÅÛ ¿¡µðÆ®¿¡¼­ ÀÔ·ÂÇÑ °æÇèÄ¡ ¹èÀ² Àû¿ë (ÁØÇõ)
				nSUM_EXP = int( (float)nSUM_EXP * GetExpMultipleRateFromSlotItem() );

				m_sExperience.lnNow += nSUM_EXP;
				m_sExperience.LIMIT();
			}
		}

		/*skill summon, Juver, 2017/10/09 */
		if ( m_pGLGaeaServer && pskill_main->m_sSPECIAL_SKILL.emSSTYPE == SKILL::EMSSTYPE_SUMMON )
		{
			SKILL::SSKILL_SUMMON_DATA &summon_data = pskill_main->m_sSPECIAL_SKILL.sSummon[wSKILL_LVL_MAIN];

			SUMMON_DATA_SKILL sdata;
			sdata.bSummon = TRUE;
			sdata.sTarget = m_sTARGET_IDS_MAIN[0];
			sdata.sidSkill = pskill_main->m_sBASIC.sNATIVEID;

			sdata.Assign( summon_data );
			m_pGLGaeaServer->CreateSummon( sdata, m_dwGaeaID );
		}
	}
	
	/*multi skill Juver, 2018/09/04 */
	if ( pskill_main )
	{
		PGLSKILL pskill_sub = GLSkillMan::GetInstance().GetData ( pskill_main->m_sEXT_DATA.idMultiTargetSkill );
		if ( pskill_sub )
		{
			int wSKILL_LVL_SUB = wSKILL_LVL_MAIN;

			SKILL::CDATA_LVL &sSKILL_DATA = pskill_sub->m_sAPPLY.sDATA_LVL[wSKILL_LVL_SUB];

			int nEXP_NUM = 0;
			int nALL_LVL = 0;
			int nALL_VARHP = 0, nALL_FULLHP = 0;
			int nALL_VARMP = 0, nALL_FULLMP = 0;
			int nALL_VARSP = 0, nALL_FULLSP = 0;

			WORD wMAX_TAR = 0;
			if ( pskill_sub->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC )			wMAX_TAR = sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1;
			else if ( pskill_sub->m_sBASIC.emIMPACT_REALM == REALM_FANWIZE )	wMAX_TAR = sSKILL_DATA.wTARNUM * ( sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1 );
			else																wMAX_TAR = sSKILL_DATA.wTARNUM;

			if ( m_wTARGET_NUM_SUB > wMAX_TAR )		m_wTARGET_NUM_SUB = wMAX_TAR;

			if ( m_wTARGET_NUM_SUB == 0 && pskill_sub->m_sBASIC.emIMPACT_TAR == TAR_SELF )
			{
				m_wTARGET_NUM_SUB = 1;
			}


			SNATIVEID sUseSkillID = pskill_sub->m_sBASIC.sNATIVEID;

			/*dash skill logic, Juver, 2017/06/17 */
			BOOL bACTDASH = FALSE;

			/* skill position shift, Juver, 2020/12/15 */
			BOOL bPositionShift = FALSE;

			/*skill teleport, Juver, 2018/09/06 */
			BOOL bTeleport = FALSE;

			//	Note : ½ºÅ³ÀÌ Àû¿ëµÇ´Â Å¸°Ù °Ë»ç.
			//
			STARGETID sTARID;
			for ( WORD i=0; i<m_wTARGET_NUM_SUB; ++i )
			{
				const STARID &_sTARID = m_sTARGET_IDS_SUB[i];

				sTARID.emCrow = _sTARID.GETCROW();
				sTARID.dwID = _sTARID.GETID();
				GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTARID );
				if ( !pACTOR )									continue;

				bool bCrushBlowOne = false;

				/* immune skill logic, Juver, 2020/12/24 */
				DAMAGE_SPEC*	pDamageSpec = pACTOR->GetDamageSpecPtr();
				if ( pDamageSpec )
				{
					bool bImmune = false;

					if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_SHORT ) && pskill_sub->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_SHORT )
						bImmune = true;

					if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_LONG ) && pskill_sub->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_LONG )
						bImmune = true;

					if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_MAGIC ) && pskill_sub->m_sBASIC.emAPPLY == SKILL::EMAPPLY_MAGIC )
						bImmune = true;

					if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_NORMAL ) && pskill_sub->m_sBASIC.emACTION == SKILL::EMACTION_NORMAL )
						bImmune = true;

					if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_BUFF ) && pskill_sub->m_sBASIC.emACTION == SKILL::EMACTION_BUFF )
						bImmune = true;

					if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_DEBUFF ) && pskill_sub->m_sBASIC.emACTION == SKILL::EMACTION_DEBUFF )
						bImmune = true;

					if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_LIMIT ) && pskill_sub->m_sBASIC.emACTION == SKILL::EMACTION_LIMIT )
						bImmune = true;

					if ( bImmune )
					{
						GLMSG::SNETPC_ATTACK_AVOID NetMsg;
						NetMsg.emTarCrow	= sTARID.emCrow;
						NetMsg.dwTarID		= sTARID.dwID;

						GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

						GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
						NetMsgBrd.dwGaeaID	= m_dwGaeaID;
						NetMsgBrd.emTarCrow	= sTARID.emCrow;
						NetMsgBrd.dwTarID	= sTARID.dwID;

						SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

						continue;
					}
				}

				for ( WORD j=0; j<sSKILL_DATA.wAPPLYNUM; ++j )
				{
					if ( pACTOR->GetNowHP()==0 )				continue;

					//	Note : ¹°¸® µ¥¹ÌÁö ¹ß»ý.
					//
					DWORD dwDamageFlag = DAMAGE_TYPE_NONE;

					int nVAR_HP(0), nVAR_MP(0), nVAR_SP(0);

					//	ÀúÇ×Ä¡.
					short nRESIST = pACTOR->GETRESIST().GetElement(pskill_sub->m_sAPPLY.emELEMENT);
					if ( nRESIST>99 )	nRESIST = 99;

					//	SKILL ±âº» Àû¿ë.
					//
					switch ( pskill_sub->m_sAPPLY.emBASIC_TYPE )
					{
					case SKILL::EMFOR_HP:
						//	´ë»ó¿¡°Ô ÇÇÇØ¸¦ ÁÖ´Â ½ºÅ³.
						if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
						{
							//					bCRITICAL = CALCDAMAGE ( nVAR_HP, bShock, sTARID, m_pLandMan, pSkill, wSKILL_LVL, GLPeriod::GetInstance().GetWeather(), sAniAttack.m_wDivCount );
							DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
							dwDamageFlag = CALCDAMAGE ( nVAR_HP, m_dwGaeaID, sTARID, m_pLandMan, pskill_sub, wSKILL_LVL_SUB, dwWeather, sAniAttack.m_wDivCount );
							
							/* skill illusion, Juver, 2021/01/17 */
							pACTOR->ReactDamage( this, dwDamageFlag );
							
							nVAR_HP = - nVAR_HP;
						}
						//	´ë»óÀ» È¸º¹½ÃÄÑ ÁÖ´Â ½ºÅ³.
						else
						{
							int nDX = pACTOR->GetMaxHP() - pACTOR->GetNowHP();
							nVAR_HP += (int) min(nDX,sSKILL_DATA.fBASIC_VAR);
						}
						break;

					case SKILL::EMFOR_MP:
						//	´ë»ó¿¡°Ô ÇÇÇØ¸¦ ÁÖ´Â ½ºÅ³.
						if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
						{
							int nVAR = int(-sSKILL_DATA.fBASIC_VAR);
							nVAR_MP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
						}
						//	´ë»óÀ» È¸º¹½ÃÄÑ ÁÖ´Â ½ºÅ³.
						else
						{
							int nDX = pACTOR->GetMaxMP() - pACTOR->GetNowMP();
							nVAR_MP += (int) min(nDX,sSKILL_DATA.fBASIC_VAR);
						}
						break;

					case SKILL::EMFOR_SP:
						//	´ë»ó¿¡°Ô ÇÇÇØ¸¦ ÁÖ´Â ½ºÅ³.
						if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
						{
							int nVAR = int(-sSKILL_DATA.fBASIC_VAR);
							nVAR_SP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
						}
						//	´ë»óÀ» È¸º¹½ÃÄÑ ÁÖ´Â ½ºÅ³.
						else
						{
							int nDX = pACTOR->GetMaxSP() - pACTOR->GetNowSP();
							nVAR_SP += (int) min(nDX,sSKILL_DATA.fBASIC_VAR);
						}
						break;
					};

					if ( dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW ) bCrushBlowOne = true;

					//	Note : SP ºÎÁ·½Ã¿¡´Â º¯È­ °ªÀ» ¹ÝÀ¸·Î Á¶Á¤.
					//
					if ( bLowSP )
					{
						nVAR_HP /= 2;
						nVAR_MP /= 2;
						nVAR_SP /= 2;
					}

					//	Note : ½ºÅ³ Æ¯¼ö ±â´É.
					//
					int nGATHER_HP(0), nGATHER_MP(0), nGATHER_SP(0);

					for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
					{
						const SKILL::SSPEC &sSKILL_SPEC = pskill_sub->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_SUB];

						switch ( pskill_sub->m_sAPPLY.sSpecs[nSpec].emSPEC )
						{
						case EMSPECA_HP_GATHER:
							nGATHER_HP += int ( abs(nVAR_HP) * sSKILL_SPEC.fVAR1 );
							break;

						case EMSPECA_MP_GATHER:
							nGATHER_MP += int ( abs(nVAR_MP) * sSKILL_SPEC.fVAR1 );
							break;

						case EMSPECA_SP_GATHER:
							nGATHER_SP += int ( abs(nVAR_SP) * sSKILL_SPEC.fVAR1 );
							break;
						};
					}


					//	Note : ÀÚ±â ÀÚ½Å¿¡°Ô ¾´ ½ºÅ³Àº Á¦¿ÜÇÏ°í EXP ÀÚ·á¸¦ ¼öÁý.
					//
					if ( sTARID!=STARGETID(CROW_PC,m_dwGaeaID) )
					{
						++nEXP_NUM;
						nALL_LVL += pACTOR->GetLevel ();

						nALL_VARHP += nVAR_HP;
						nALL_VARMP += nVAR_MP;
						nALL_VARSP += nVAR_SP;

						nALL_FULLHP += pACTOR->GetMaxHP();
						nALL_FULLMP += pACTOR->GetMaxMP();
						nALL_FULLSP += pACTOR->GetMaxSP();
					}

					//	Note : ½ºÅ³ ±âº» ¿µÇâ.
					//
					SSKILLACT sSKILLACT;
					sSKILLACT.sID = STARGETID(CROW_PC,m_dwGaeaID);
					sSKILLACT.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
					sSKILLACT.fDELAY = pskill_sub->m_sEXT_DATA.fDELAY4DAMAGE;
					sSKILLACT.emAPPLY = pskill_sub->m_sBASIC.emAPPLY;
					sSKILLACT.dwDamageFlag = dwDamageFlag;			

					//	Note : Ã¼·Â º¯È­ ÀúÀå.
					//
					sSKILLACT.nVAR_HP = nVAR_HP;
					sSKILLACT.nVAR_MP = nVAR_MP;
					sSKILLACT.nVAR_SP = nVAR_SP;

					//	Note : Ã¼·Â Èí¼ö ÀúÀå.
					//
					sSKILLACT.nGATHER_HP = nGATHER_HP;
					sSKILLACT.nGATHER_MP = nGATHER_MP;
					sSKILLACT.nGATHER_SP = nGATHER_SP;

					if ( sSKILLACT.VALID() )
					{
						m_pLandMan->RegSkillAct ( sSKILLACT );
					}
				}

				//	Note : ½ºÅ³ º¸Á¶ ¿µÇâ.
				//
				SSKILLACTEX sSKILLACTEX;
				sSKILLACTEX.sID = STARGETID(CROW_PC,m_dwGaeaID);
				sSKILLACTEX.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
				sSKILLACTEX.fDELAY = pskill_sub->m_sEXT_DATA.fDELAY4DAMAGE;

				sSKILLACTEX.idSKILL = pskill_sub->m_sBASIC.sNATIVEID;
				sSKILLACTEX.wSKILL_LVL = wSKILL_LVL_SUB;

				//  °­ÇÑ Å¸°Ý È®·ü
				if ( bCrushBlowOne )
				{
					sSKILLACTEX.bCrushBlow = bCrushBlowOne;
					sSKILLACTEX.fPUSH_PULL = GLCONST_CHAR::fCRUSH_BLOW_RANGE;
				}

				//	Note : »óÅÂ ÀÌ»ó À¯¹ß.
				//
				ITEM::SSATE_BLOW sBLOW;
				sBLOW.emTYPE = EMBLOW_NONE;

				if ( pskill_sub->m_sAPPLY.emELEMENT==EMELEMENT_ARM )
				{
					SITEM *pITEM = GET_ELMT_ITEM ();
					if ( pITEM )	sBLOW = pITEM->sSuitOp.sBLOW;
				}
				else
				{
					const SKILL::SSTATE_BLOW &sSKILL_BLOW = pskill_sub->m_sAPPLY.sSTATE_BLOW[wSKILL_LVL_SUB];

					sBLOW.emTYPE = pskill_sub->m_sAPPLY.emSTATE_BLOW;
					sBLOW.fRATE = sSKILL_BLOW.fRATE;
					sBLOW.fLIFE = pskill_sub->m_sAPPLY.sDATA_LVL[wSKILL_LVL_SUB].fLIFE;
					sBLOW.fVAR1 = sSKILL_BLOW.fVAR1;
					sBLOW.fVAR2 = sSKILL_BLOW.fVAR2;
				}

				if ( sBLOW.emTYPE!=EMBLOW_NONE )
				{
					//	Note : ¹ß»ý È®À² °è»ê.
					//
					short nBLOWRESIST = pACTOR->GETRESIST().GetElement ( STATE_TO_ELEMENT(sBLOW.emTYPE) );
					if ( nBLOWRESIST>99 )	nBLOWRESIST = 99;

					//			float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBLOW.emTYPE, GLPeriod::GetInstance().GetWeather(), m_pLandMan->IsWeatherActive() );
					DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
					float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBLOW.emTYPE, dwWeather, m_pLandMan->IsWeatherActive() );

					BOOL bBLOW(FALSE);
					if ( !(pACTOR->GETHOLDBLOW()&STATE_TO_DISORDER(sBLOW.emTYPE)) )
					{
						bBLOW = GLOGICEX::CHECKSTATEBLOW ( sBLOW.fRATE * fPOWER, GETLEVEL(), pACTOR->GetLevel(), nBLOWRESIST );
					}

					if ( bBLOW )
					{
						//	Note : »óÅÂÀÌ»ó ¹ß»ý.
						//	
						SSTATEBLOW sSTATEBLOW;
						float fLIFE = sBLOW.fLIFE * fPOWER;
						fLIFE = ( fLIFE - (fLIFE*nBLOWRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );

						sSTATEBLOW.emBLOW = sBLOW.emTYPE;
						sSTATEBLOW.fAGE = fLIFE;
						sSTATEBLOW.fSTATE_VAR1 = sBLOW.fVAR1;
						sSTATEBLOW.fSTATE_VAR2 = sBLOW.fVAR2;

						//	Note : »óÅÂ ÀÌ»ó ¹ß»ý ÀúÀå.
						//
						sSKILLACTEX.sSTATEBLOW = sSTATEBLOW;
					}
				}

				//	Note : ½ºÅ³ Æ¯¼ö ±â´É.
				//
				BOOL bActionMovement = FALSE;
				BOOL bCUREFLAG = FALSE;
				BOOL bREMOVEFLAG = FALSE;
				BOOL bREBIRTH = FALSE;

				/*skill vehicle off, Juver, 2018/09/07 */
				BOOL skill_vehicle_off = FALSE;

				/*skill pet off, Juver, 2018/09/07 */
				BOOL skill_pet_off = FALSE;

				/* skill delay addition, Juver, 2020/12/09 */
				BOOL bSkillDelay = FALSE;

				/* skill buff duration change, Juver, 2020/12/16 */
				BOOL bDurationChange = FALSE;

				if ( bCrushBlowOne )	bActionMovement = TRUE;

				for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
				{
					const SKILL::SSPEC &sSKILL_SPEC = pskill_sub->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_SUB];
					switch ( pskill_sub->m_sAPPLY.sSpecs[nSpec].emSPEC )
					{
					case EMSPECA_PUSHPULL:
						{
							if ( bActionMovement )	break;
							if ( !pACTOR->IsValidBody() )				break;
							if ( RANDOM_POS > sSKILL_SPEC.fVAR2 )		break;	//	Note : ¹ß»ýÈ®À² Àû¿ë.
							sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR1;
							sSKILLACTEX.bPushPull = true;
							bActionMovement = TRUE;
						}break;

					case EMSPECA_CRUSHING_BLOW:
						{
							if ( bActionMovement )	break;
							if ( sSKILL_SPEC.fVAR2 != 0.0f && RANDOM_POS > sSKILL_SPEC.fVAR2 )		break;
							sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR1;
							sSKILLACTEX.bCrushingBlow = true;
							bActionMovement = TRUE;
						}break;

						/* push skill logic, Juver, 2020/12/14 */
					case EMSPECA_PUSH:
						{
							if ( bActionMovement )			break;
							if ( !pACTOR->IsValidBody() )	break;
							if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
							sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //distance
							sSKILLACTEX.bPush = true;
							bActionMovement = TRUE;
						}break;

						/* pull skill logic, Juver, 2020/12/14 */
					case EMSPECA_PULL:
						{
							if ( bActionMovement )			break;
							if ( !pACTOR->IsValidBody() )	break;
							if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
							sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //pull speed
							sSKILLACTEX.bPull = true;
							bActionMovement = TRUE;
						}break;

						/* dash skill logic, Juver, 2020/12/14 */
					case EMSPECA_SKILL_DASH:
						{
							if ( bACTDASH )					break;
							if ( !pACTOR->IsValidBody() )	break;
							if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
							sSKILLACTEX.fDashSpeed = sSKILL_SPEC.fVAR2; //dash speed
							sSKILLACTEX.dwActionAnim = sSKILL_SPEC.dwFLAG; //animation
							sSKILLACTEX.bDash = true;
							bACTDASH = TRUE;
						}break;

						/*skill teleport, Juver, 2018/09/06 */
					case EMSPECA_TELEPORT:
						{
							if ( bTeleport )			break;
							if ( !pACTOR->IsValidBody() )	break;
							sSKILLACTEX.bUseTeleport = true;
							sSKILLACTEX.vTeleportPos = m_vTARGET_POS_SUB;
							bTeleport = TRUE;
						}break;

						/*skill position shift, Juver, 2018/09/06 */
					case EMSPECA_POSITION_SHIFT:
						{
							if ( bPositionShift )			break;
							if ( !pACTOR->IsValidBody() )	break;
							if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;

							sSKILLACTEX.fPositionShiftSpeed = sSKILL_SPEC.fVAR2;

							if ( sSKILL_SPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_SWAP )
								sSKILLACTEX.bPositionShiftSwap = true;
							else if ( sSKILL_SPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_DASH )
								sSKILLACTEX.bPosisionShiftDash = true;
							else if ( sSKILL_SPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_PULL )
								sSKILLACTEX.bPositionShiftPull = true;

							bPositionShift = TRUE;
						}break;

					case EMSPECA_HP_DIV:
					case EMSPECA_MP_DIV:
					case EMSPECA_SP_DIV:
						break;

					case EMSPECA_RECBLOW:
						{
							//	Note : »óÅÂ ÀÌ»ó Ä¡·á.
							//
							if ( bCUREFLAG )	break;
							sSKILLACTEX.dwCUREFLAG |= pskill_sub->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_SUB].dwFLAG;
							bCUREFLAG = TRUE;
						}break;

						/*debuff skill logic, Juver, 2017/06/10 */
					case EMSPECA_BUFF_REMOVE:
						{
							if ( bREMOVEFLAG )	break;
							if ( sSKILLACTEX.bDebuff )	break;
							sSKILLACTEX.dwRemoveFlag = pskill_sub->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL_SUB].dwFLAG;
							sSKILLACTEX.bDebuff = true;
							bREMOVEFLAG = TRUE;
						}break;

					case EMSPECA_REBIRTH:
						{
							if ( bREBIRTH )	break;

							if ( sTARID.emCrow==CROW_PC )
							{
								PGLCHAR pPC = m_pGLGaeaServer->GetChar(sTARID.dwID);
								if ( pPC )
								{
									BOOL bBlockResu = FALSE;

									if ( m_pLandMan )
									{
										/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
										if ( m_pLandMan->m_bPVPTyrannyMap && m_wSchool != pPC->m_wSchool )
											bBlockResu = GLPVPTyrannyField::GetInstance().isDisableOtherSchoolResu();

										if ( m_pLandMan->m_bPVPTyrannyMap && m_dwGuild != pPC->m_dwGuild )
											bBlockResu = GLPVPTyrannyField::GetInstance().isDisableOtherGuildResu();

										/*PPVPSchoolWars enemy resu, EJCode, 2018/10/22 */
										if ( m_pLandMan->m_bPVPSchoolWarsMap && m_wSchool != pPC->m_wSchool )
											bBlockResu = GLPVPSchoolWarsField::GetInstance().isDisableOtherSchoolResu();

										/*PVPCaptureTheFlag enemy resu, EJCode, 2018/10/23 */
										if ( m_pLandMan->m_bPVPCaptureTheFlagMap && m_wCaptureTheFlagTeam != pPC->m_wCaptureTheFlagTeam )
											bBlockResu = GLPVPCaptureTheFlagField::GetInstance().isDisableOtherTeamResu();

										/* pvp club death match, Juver, 2020/11/25 */
										if ( m_pLandMan->m_bPVPClubDeathMatchBattleMap && m_dwGuild != pPC->m_dwGuild )
											bBlockResu = PVPClubDeathMatchField::GetInstance().isDisableOtherGuildResu();

										if ( m_pLandMan->m_bPVPPBGBattleMap && m_dwPartyID != pPC->m_dwPartyID )
											bBlockResu = PVPPBG::ManagerField::GetInstance().isDisableOtherResu();
									}

									if ( !bBlockResu )
									{
										WORD wRECOVE = pPC->DoRevive2Skill(pskill_sub->m_sBASIC.sNATIVEID,wSKILL_LVL_SUB, (WORD)nSpec );
										if ( 0<wRECOVE)
										{
											++nEXP_NUM;
											nALL_LVL += pPC->GetLevel ();
											nALL_VARHP += wRECOVE;
											nALL_FULLHP += pPC->GetMaxHP();

											if ( m_pLandMan )
											{
												/*pvp tyranny, Juver, 2017/08/24 */
												if ( m_pLandMan->m_bPVPTyrannyMap )
													GLPVPTyrannyField::GetInstance().ScoreResu( m_sMapID, m_dwCharID );

												/*school wars, Juver, 2018/01/20 */
												if ( m_pLandMan->m_bPVPSchoolWarsMap )
													GLPVPSchoolWarsField::GetInstance().ScoreResu( m_dwCharID );

												/*pvp capture the flag, Juver, 2018/02/01 */
												if ( m_pLandMan->m_bPVPCaptureTheFlagMap )
													GLPVPCaptureTheFlagField::GetInstance().ScoreResu( m_dwCharID );

												/* pvp club death match, Juver, 2020/11/25 */
												if ( m_pLandMan->m_bPVPClubDeathMatchBattleMap )
													PVPClubDeathMatchField::GetInstance().ScoreResu( m_dwCharID );

												if ( m_pLandMan->m_bPVPPBGBattleMap )
													PVPPBG::ManagerField::GetInstance().ScoreResu( m_dwCharID );
													
												if (m_pLandMan->m_bPVPWoeMap)
												{
													GLPVPWoeField::GetInstance().AddWoeResuScore(m_dwCharID);
												}

											}
											
										}
									}
								}
							}

							bREBIRTH = TRUE;
						}break;

						/*skill vehicle off, Juver, 2018/09/07 */
					case  EMSPECA_VEHICLE_OFF:
						{
							if ( skill_vehicle_off )	break;

							if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;

							if ( sTARID.emCrow == CROW_PC )
							{
								PGLCHAR pchar_target = m_pGLGaeaServer->GetChar(sTARID.dwID);
								if ( pchar_target )	pchar_target->skill_remove_vehicle();
							}

							skill_vehicle_off = TRUE;
						}break;

						/*skill pet off, Juver, 2018/09/07 */
					case  EMSPECA_PET_OFF:
						{
							if ( skill_pet_off )	break;

							if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;

							if ( sTARID.emCrow == CROW_PC )
							{
								PGLCHAR pchar_target = m_pGLGaeaServer->GetChar(sTARID.dwID);
								if ( pchar_target )	pchar_target->skill_remove_pet();
							}

							skill_pet_off = TRUE;
						}break;

						/* skill delay addition, Juver, 2020/12/09 */
					case  EMSPECA_SKILLDELAY:
						{
							if ( bSkillDelay )	break;

							if ( sSKILL_SPEC.dwFLAG & SKILL::SPEC_SKILL_DELAY_TYPE_SINGLE )
							{
								if ( sTARID.emCrow == CROW_PC )
								{
									PGLCHAR pchar_target = m_pGLGaeaServer->GetChar(sTARID.dwID);
									if ( pchar_target )	pchar_target->ApplySkillDelayOnce( sUseSkillID, sSKILL_SPEC.fVAR1, sSKILL_SPEC.fVAR2 );
								}

								bSkillDelay = TRUE;
							}
						}break;

						/* skill buff duration change, Juver, 2020/12/16 */
					case  EMSPECA_DURATION_CHANGE:
						{
							if ( bDurationChange )	break;

							if ( sSKILL_SPEC.fVAR1 != 0.0f )
							{
								if ( pACTOR )
									pACTOR->ReceiveSkillBuffDurationChange( sUseSkillID, sSKILL_SPEC.dwFLAG, sSKILL_SPEC.fVAR1 );

								bDurationChange = TRUE;
							}
						}break;
					};
				}


				switch (pskill_sub->m_sAPPLY.emBASIC_TYPE)
				{
				case SKILL::EMFOR_CURE:
					//	Note : »óÅÂ ÀÌ»ó Ä¡·á.
					//
					sSKILLACTEX.dwCUREFLAG |= pskill_sub->m_sAPPLY.dwCUREFLAG;
					break;
				};

				if ( sSKILLACTEX.VALID() )
				{
					m_pLandMan->RegSkillActEx ( sSKILLACTEX );
					nALL_VARHP;
				}

				//	Note : È¸º¹¿¡ ±â¿©ÇßÀ» ¶§¸¸ °æÇèÄ¡ ¹ß»ý.	( °ø°Ý½Ã´Â Á¦¿Ü )
				//
				int nSUM_EXP = 0;
				bool bRECOVE = (nALL_VARHP>=0&&nALL_VARMP>=0&&nALL_VARSP&&0) && (nALL_VARHP+nALL_VARMP+nALL_VARSP)>0;

				if ( bRECOVE )
				{
					//	½ÇÁ¦ º¯È­·®ÀÌ ÀÖÀ» ¶§¸¸ °æÇèÄ¡¸¦ »êÃâ.
					if ( nALL_VARHP>0 )
						nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARHP,nALL_FULLHP,pACTOR->GetBonusExp());

					if ( nALL_VARMP>0 )
						nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARMP,nALL_FULLMP,pACTOR->GetBonusExp());

					if ( nALL_VARSP>0 )
						nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARSP,nALL_FULLSP,pACTOR->GetBonusExp());

					nSUM_EXP += int (nSUM_EXP*GLCONST_CHAR::fREC_EXP_RATE);
				}

				//	Note : °æÇèÄ¡ È¹µæ·® Àû¿ë.
				//
				if ( nSUM_EXP )
				{
					// ¾ÆÀÌÅÛ ¿¡µðÆ®¿¡¼­ ÀÔ·ÂÇÑ °æÇèÄ¡ ¹èÀ² Àû¿ë (ÁØÇõ)
					nSUM_EXP = int( (float)nSUM_EXP * GetExpMultipleRateFromSlotItem() );

					m_sExperience.lnNow += nSUM_EXP;
					m_sExperience.LIMIT();
				}
			}

			/*skill summon, Juver, 2017/10/09 */
			if ( m_pGLGaeaServer && pskill_sub->m_sSPECIAL_SKILL.emSSTYPE == SKILL::EMSSTYPE_SUMMON )
			{
				SKILL::SSKILL_SUMMON_DATA &summon_data = pskill_sub->m_sSPECIAL_SKILL.sSummon[wSKILL_LVL_SUB];

				SUMMON_DATA_SKILL sdata;
				sdata.bSummon = TRUE;
				sdata.sTarget = m_sTARGET_IDS_SUB[0];
				sdata.sidSkill = pskill_sub->m_sBASIC.sNATIVEID;

				sdata.Assign( summon_data );
				m_pGLGaeaServer->CreateSummon( sdata, m_dwGaeaID );
			}
		}
	}
	


	

	//	Note : ½ºÅ³ »ç¿ë¿¡ µû¸¥ ¼Ò¸ð·® FB MSG.
	//
	GLMSG::SNETPC_SKILLCONSUME_FB NetMsgFB;
	EMSLOT emLHand = GetCurLHand();
	SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
	if ( pItem && (pItem->sDrugOp.bInstance) )		NetMsgFB.wTurnNum = m_PutOnItems[emLHand].wTurnNum;
	NetMsgFB.wNowHP = m_sHP.wNow;
	NetMsgFB.wNowMP = m_sMP.wNow;
	NetMsgFB.wNowSP = m_sSP.wNow;
	NetMsgFB.wNowCP = m_sCombatPoint.wNow; /*combatpoint logic, Juver, 2017/05/28 */

	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsgFB );

	//	Note : ¼Ò¸ð¼º ¾ÆÀÌÅÆ À¯È¿¼º °Ë»çÈÄ, »ç¶óÁú¶§ ¸Þ½ÃÁö ¹ß»ý.
	//
	CheckInstanceItem ();
}


// *****************************************************
// Desc: ½Ãµ¿ÁßÀÎ ½ºÅ³°ü·Ã Á¤º¸ °»½Å
// *****************************************************
BOOL GLChar::SkillProcess ( float fElapsedTime )
{
	//CONSOLEMSG_WRITE( "SkillProcess Start [%u]", m_idACTIVESKILL.dwID );

	//	Note : Ä³¸¯ÅÍ°¡ ¹è¿î ½ºÅ³ Á¤º¸ °¡Á®¿È.
	//
	WORD wSKILL_LVL_MAIN;
	SCHARSKILL sSkill;

	if ( IsDefenseSkill() )
	{
		if ( m_idACTIVESKILL != m_sDefenseSkill.m_dwSkillID ) return FALSE;
		sSkill.sNativeID = m_idACTIVESKILL;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
		wSKILL_LVL_MAIN = sSkill.wLevel;
	}
	else
	{
        SKILL_MAP_ITER learniter = m_ExpSkills.find ( m_idACTIVESKILL.dwID );
		if ( learniter==m_ExpSkills.end() )		return FALSE;
		sSkill = (*learniter).second;
		wSKILL_LVL_MAIN = sSkill.wLevel;
	}

	// ´ë·Ã Á¾·áÈÄ ¹«ÀûÅ¸ÀÓÀÌ¸é °ø°Ý ¹«½Ã
	if ( m_sCONFTING.IsPOWERFULTIME () ) return FALSE;

	//	Note : ½ºÅ³ÀÌ Àû¿ëµÇ´Â Å¸°Ù °Ë»ç.
	//
	DWORD dwVALIDNUM(0); // À¯È¿ Å¸°Ù

	//	½ºÅ³ Á¤º¸ °¡Á®¿È.
	/*multi skill Juver, 2018/09/05 */
	PGLSKILL pskill_main = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL );
	if ( pskill_main )
	{
		//	ÀÚ½ÅÀÇ µ¿Ã¼¹Ý°æ + ½ºÅ³Å¸°Ù¿µ¿ª + ½ºÅ³Àû¿ë¿µ¿ª + 20(¿©À¯°ª)
		float fSkillRange = (float) ( GETBODYRADIUS() + GETSKILLRANGE_TAR(*pskill_main) + GETSKILLRANGE_APPLY(*pskill_main,wSKILL_LVL_MAIN) + 20 );

		for ( WORD i=0; i<m_wTARGET_NUM_MAIN; ++i )
		{
			STARGETID sTARID;
			sTARID.emCrow = m_sTARGET_IDS_MAIN[i].GETCROW();
			sTARID.dwID = m_sTARGET_IDS_MAIN[i].GETID();
			GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTARID );
			if ( !pACTOR )					continue;

			// ´ë·Ã Á¾·áÈÄ ¹«ÀûÅ¸ÀÓÀÌ¸é °ø°Ý ¹«½Ã
			PGLCHAR pChar = m_pGLGaeaServer->GetChar ( sTARID.dwID );
			if ( pChar )
			{
				if ( pChar->m_sCONFTING.IsPOWERFULTIME () )
					continue;
			}

			// Å¸°Ù¹Ý°æ + ½ºÅ³ À¯È¿¹Ý°æ
			float fReActionRange = (float) ( pACTOR->GetBodyRadius() + fSkillRange );

			// ½ºÅ³ ¹ßµ¿ÀÚ¿Í Å¸°Ù°úÀÇ °Å¸®
			D3DXVECTOR3 vDist = m_vPos - pACTOR->GetPosition();  
			float fDist = D3DXVec3Length(&vDist);

			if ( fReActionRange*GLCONST_CHAR::fREACT_VALID_SCALE < fDist )
			{
				m_sTARGET_IDS_MAIN[i].wID = USHRT_MAX;
			}
			else
			{
				dwVALIDNUM++;
			}
		}
	}

	/*multi skill Juver, 2018/09/05 */
	if ( pskill_main )
	{
		PGLSKILL pskill_sub = GLSkillMan::GetInstance().GetData ( pskill_main->m_sEXT_DATA.idMultiTargetSkill );
		if ( pskill_sub )
		{
			WORD wSKILL_LVL_SUB = wSKILL_LVL_MAIN;

			//	ÀÚ½ÅÀÇ µ¿Ã¼¹Ý°æ + ½ºÅ³Å¸°Ù¿µ¿ª + ½ºÅ³Àû¿ë¿µ¿ª + 20(¿©À¯°ª)
			float fSkillRange = (float) ( GETBODYRADIUS() + GETSKILLRANGE_TAR(*pskill_sub) + GETSKILLRANGE_APPLY(*pskill_sub,wSKILL_LVL_SUB) + 20 );

			for ( WORD i=0; i<m_wTARGET_NUM_SUB; ++i )
			{
				STARGETID sTARID;
				sTARID.emCrow = m_sTARGET_IDS_SUB[i].GETCROW();
				sTARID.dwID = m_sTARGET_IDS_SUB[i].GETID();
				GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTARID );
				if ( !pACTOR )					continue;

				// ´ë·Ã Á¾·áÈÄ ¹«ÀûÅ¸ÀÓÀÌ¸é °ø°Ý ¹«½Ã
				PGLCHAR pChar = m_pGLGaeaServer->GetChar ( sTARID.dwID );
				if ( pChar )
				{
					if ( pChar->m_sCONFTING.IsPOWERFULTIME () )
						continue;
				}

				// Å¸°Ù¹Ý°æ + ½ºÅ³ À¯È¿¹Ý°æ
				float fReActionRange = (float) ( pACTOR->GetBodyRadius() + fSkillRange );

				// ½ºÅ³ ¹ßµ¿ÀÚ¿Í Å¸°Ù°úÀÇ °Å¸®
				D3DXVECTOR3 vDist = m_vPos - pACTOR->GetPosition();  
				float fDist = D3DXVec3Length(&vDist);

				if ( fReActionRange*GLCONST_CHAR::fREACT_VALID_SCALE < fDist )
				{
					m_sTARGET_IDS_SUB[i].wID = USHRT_MAX;
				}
				else
				{
					dwVALIDNUM++;
				}
			}
		}
	}
	
	

	// ½ºÅ³ À¯È¿¹Ý°æ ³»¿¡ Å¸°ÙÀÌ ¾øÀ» °æ¿ì
	if ( dwVALIDNUM==0 )
	{
		TurnAction ( GLAT_IDLE );

		//	Note : ÀÚ½Å¿¡°Ô.
		GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
		NetMsg.emTarCrow		= m_TargetID.emCrow;
		NetMsg.dwTarID			= m_TargetID.dwID;
		NetMsg.nDamage			= 0;
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );
	}

	m_fattTIMER += fElapsedTime*GLCHARLOGIC_SERVER::GETATTVELO();
	m_fattTIMER += GLCHARLOGIC_SERVER::GETATT_ITEM();

	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[m_SKILLMTYPE][m_SKILLSTYPE];
	if ( vecAniAttack.empty() )	return FALSE;

	//	Note : Æ¯Á¤ °ø°Ý ¿¡´Ï¸ÞÀÌ¼Ç ¼³Á¤ ¼±ÅÃ. (¼öÁ¤¿ä)
	//
	const SANIATTACK &sAniAttack = vecAniAttack[0];

	//
	//
	int nTotalKeys = int(sAniAttack.m_dwETime) - int(sAniAttack.m_dwSTime);
	//anti shit skillcut-as
	DWORD dwThisKey = sAniAttack.m_dwSTime + DWORD(m_fattTIMER*UNITANIKEY_PERSEC);

	if ( !m_sHITARRAY.empty() )
	{
		SSTRIKE sStrike = *m_sHITARRAY.begin();

		if ( dwThisKey >= sStrike.wDivKey )
		{
			if ( sStrike.bHit )
			{
				//CONSOLEMSG_WRITE( "SkillProcess m_sHITARRAY1 SkillProc[%u]", m_idACTIVESKILL.dwID );
				SkillProc ( m_idACTIVESKILL, sStrike.bLowSP );

				/* skill stigma, Juver, 2020/12/13 */
				ProcessSkillStigmaTypeSkill( m_idACTIVESKILL );

				//TurnAction ( GLAT_IDLE );
			}
			else
			{
				//CONSOLEMSG_WRITE( "SkillProcess m_sHITARRAY1 AvoidProc[%u]", m_idACTIVESKILL.dwID );
				AvoidProc ( m_idACTIVESKILL, sStrike.bLowSP );
				//TurnAction ( GLAT_IDLE );
			}

			//	Å¥ µ¥ÀÌÅ¸°¡ Ã³¸®µÈ °æ¿ì »èÁ¦ÇÑ´Ù.
			m_sHITARRAY.pop_front ();
		}	
	}

	//	ÀÌ»óÁõ»ó¿¡ ÀÇÇØ µ¥ÀÌÅ¸°¡ ÀüÇô Ã³¸®µÇÁö ¾ÊÀº°æ¿ì,
	//	¸¶Áö¸·¿¡ ÇÑ²¨¹ø¿¡ Ã³¸®ÇÏ°í, FALSE¸¦ ¸®ÅÏÇÑ´Ù.
	//
	if ( dwThisKey >= sAniAttack.m_dwETime )
	{
		if ( !m_sHITARRAY.empty() )
		{
			while ( !m_sHITARRAY.empty() )
			{
				SSTRIKE sStrike = *m_sHITARRAY.begin();

				if ( sStrike.bHit )		
				{
					//CONSOLEMSG_WRITE( "SkillProcess m_sHITARRAY2 SkillProc[%u]", m_idACTIVESKILL.dwID );
					SkillProc ( m_idACTIVESKILL, sStrike.bLowSP );

					/* skill stigma, Juver, 2020/12/13 */
					ProcessSkillStigmaTypeSkill( m_idACTIVESKILL );

					//TurnAction ( GLAT_IDLE );
				}
				else
				{
					//CONSOLEMSG_WRITE( "SkillProcess m_sHITARRAY2 AvoidProc[%u]", m_idACTIVESKILL.dwID );
					AvoidProc ( m_idACTIVESKILL, sStrike.bLowSP );
					//TurnAction ( GLAT_IDLE );
				}

				//	Å¥ µ¥ÀÌÅ¸°¡ Ã³¸®µÈ °æ¿ì »èÁ¦ÇÑ´Ù.
				m_sHITARRAY.pop_front ();
			}
		}

		if ( !sAniAttack.IsLOOP() )	return FALSE;

		//	Note : ½ºÅ³ ¹Ýº¹.
		//
		/*extreme auto tab, Juver, 2018/09/10 */
		EMSLOT em_right_hand = GetCurRHand();
		EMSLOT em_left_hand = GetCurLHand();
		EMSKILLCHECK emCHECK = GLCHARLOGIC_SERVER::CHECHSKILL ( m_idACTIVESKILL, 1, IsDefenseSkill(), em_right_hand, em_left_hand );
		if ( emCHECK != EMSKILL_OK && emCHECK != EMSKILL_NOTSP )
		{
			//	Note : ½ºÅ³ ±¸µ¿ ½ÇÆÐ FB ¸Þ½ÃÁö.
			GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
			NetMsgFB.emSKILL_FB = emCHECK;
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return FALSE;
		}

		//	Note : SKILL ±¸µ¿.
		//
		SETACTIVESKILL ( m_idACTIVESKILL );

		BOOL bLowSP =  (emCHECK==EMSKILL_NOTSP) ? TRUE : FALSE;
		PreStrikeProc ( TRUE, bLowSP );

		//	Note : ½ºÅ³ »ç¿ë½Ã ¼Ò¸ð°ªµé ¼Ò¸ð½ÃÅ´.
		//
		GLCHARLOGIC_SERVER::ACCOUNTSKILL ( m_idACTIVESKILL, 1, true );

		m_fattTIMER = 0.0f;
	}

	return TRUE;
}

inline HRESULT GLChar::UpdateClientState ( float fElapsedTime )
{
	//	Note : Áõ°¡¼öÄ¡¸¦ Å¬¶óÀÌ¾ðÆ®°¡ ¿¹ÃøÀ» ÇÏ°Ô µÇ¸é? ¸Þ¼¼Áö ¹ß»ýÀ» ¾ÈÇØµµ µÈ´Ù.
	//		´Ü ¾à°£ÀÇ ¿ÀÂ÷°¡ ¹ß»ýÇÒ ¼ÒÁö°¡ ÀÖÀ½.
	//
	m_fSTATE_TIMER += fElapsedTime;
	if ( m_fSTATE_TIMER>1.6f )
	{
		m_fSTATE_TIMER = 0.0f;

		//	Note : ÀÚ½Å, ´ë·ÃÀÚ¿¡°Ô hp,mp °ª °»½Å.
		//
		//	ÆÄÆ¼¿ø¿¡°Ô´Â hp,mp °»½ÅÇÏÁö ¾Ê´Â´Ù.
		MsgSendUpdateState ( false );
	}

	m_fMBR_TIMER += fElapsedTime;

	if ( m_fMBR_TIMER > 1.6f )
	{
		m_fMBR_TIMER = 0.0f;

		GLPartyFieldMan& sPartyFieldMan = m_pGLGaeaServer->GetPartyMan();
		if ( sPartyFieldMan.GetParty ( m_dwPartyID ) )
		{
			GLMSG::SNET_PARTY_MBR_POS NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.vPos = m_actorMove.Position();
			NetMsg.sHP = m_sHP;
			NetMsg.sMP = m_sMP;
			NetMsg.wLevel = m_wLevel;

			sPartyFieldMan.SendMsgToMember ( m_dwPartyID, (NET_MSG_GENERIC*) &NetMsg );
		}
	}

	return S_OK;
}

VOID GLChar::UpdateLandEffect()
{
	std::set<int> setOldLandEffectNum = m_setLandEffectNum;
	std::set<int>::iterator iter_EffNum;
	m_setLandEffectNum.clear();

	SLEVEL_ETC_FUNC *pLevelEtcFunc = m_pLandMan->GetLevelEtcFunc();
	if( !pLevelEtcFunc ) return;
	if( !pLevelEtcFunc->m_bUseFunction[EMETCFUNC_LANDEFFECT] ) return;

	int nActiveNum = 0;
	for( int i=0; i<EMLANDEFFECT_MULTI; ++i )
	{
		if ( m_sLandEffect[i].IsUse() )
			nActiveNum ++;
	}

	for( int i = 0; i < (int)pLevelEtcFunc->m_vecLandEffect.size(); i++ )
	{
		SLANDEFFECT &landEffect = pLevelEtcFunc->m_vecLandEffect[i];
		if( !landEffect.IsUse() )	continue;

		if( m_vPos.x > landEffect.vMinPos.x &&
			m_vPos.z > landEffect.vMinPos.z &&
			m_vPos.x < landEffect.vMaxPos.x &&
			m_vPos.z < landEffect.vMaxPos.z  )
		{
			/* non vend area, Juver, 2021/01/17 */
			if ( landEffect.bUseYPos )
			{
				if ( !( m_vPos.y > landEffect.vMinPos.y && m_vPos.y < landEffect.vMaxPos.y ) )
				{
					continue;
				}
			}

			/* land effect school logic, Juver, 2019/12/28 */
			/* land effect ctf team requirement, Juver, 2020/09/22 */
			if ( landEffect.dwSchool&index2school(m_wSchool) && 
				landEffect.dwCTFTeam&NSCTFTeam::CTFFlagToEnum(m_wCaptureTheFlagTeam) )
			{
				m_setLandEffectNum.insert(i);
			}
		}
	}

	BOOL bUpdateLandEff = FALSE;
	if( setOldLandEffectNum.size() != m_setLandEffectNum.size() )
	{
		bUpdateLandEff = TRUE;
	}else{
		iter_EffNum = m_setLandEffectNum.begin();
		for( ; iter_EffNum != m_setLandEffectNum.end(); ++iter_EffNum )
		{
			if( setOldLandEffectNum.find( *iter_EffNum ) == setOldLandEffectNum.end() )
			{
				bUpdateLandEff = TRUE; 
				break;
			}
		}
	}

	if ( nActiveNum != (int)m_setLandEffectNum.size() )
	{
		//CONSOLEMSG_WRITE( "%d %s land effect update", m_dwCharID, m_szName );
		bUpdateLandEff = TRUE;
	}

	if( bUpdateLandEff )
	{
		DISABLEALLLANDEFF();

		GLMSG::SNETPC_LANDEFFECT NetMsg;
		int iNum = 0;
		iter_EffNum = m_setLandEffectNum.begin();
		for( ; iter_EffNum != m_setLandEffectNum.end(); ++iter_EffNum )
		{
			SLANDEFFECT landEffect = pLevelEtcFunc->FindLandEffect(*iter_EffNum);
			if( !landEffect.IsUse() ) continue;
			ADDLANDEFF( landEffect, iNum );

			if ( iNum < EMLANDEFFECT_MULTI )
			{
				NetMsg.sLandEffect[iNum] = landEffect;
			}
			
			iNum++;
		}

		NetMsg.dwGaeaID		= m_dwGaeaID;	
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );
		SendMsgViewAround( (NET_MSG_GENERIC*) &NetMsg ); 	
	}


	// ¸ðµç ¹öÇÁ¸¦ Ãë¼ÒÇØ¾ßÇÒ °æ¿ìÀÇ Ã³¸®
	for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
	{
		SLANDEFFECT landEffect = m_sLandEffect[i];
		if( !landEffect.IsUse() ) continue;
		if( landEffect.emLandEffectType == EMLANDEFFECT_CANCEL_ALLBUFF )
		{
			DisableSkillFact();
			break;
		}
	}
}

BOOL GLChar::ShockProcess ( float fElapsedTime )
{
	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecShock = ConstCharClass.m_ANIMATION[AN_SHOCK][AN_SUB_NONE];
	if ( vecShock.empty() )	return FALSE;

	m_fIdleTime += fElapsedTime * ( GETMOVEVELO() + GETMOVE_ITEM() );
			
	SANIATTACK &sAniShock = vecShock[0];

	int wTotalKeys = int(sAniShock.m_dwETime) - int(sAniShock.m_dwSTime);
	int wThisKey = int(m_fIdleTime*UNITANIKEY_PERSEC);
	
	if ( wThisKey >= wTotalKeys )	return FALSE;

	return TRUE;
}

float GLChar::GetMoveVelo ()
{
	float fDefaultVelo = IsSTATE(EM_ACT_RUN) ? GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fRUNVELO : GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fWALKVELO;
	float fMoveVelo = fDefaultVelo * ( GLCHARLOGIC_SERVER::GETMOVEVELO() + GLCHARLOGIC_SERVER::GETMOVE_ITEM() ) ;
	return fMoveVelo;
}

void GLChar::ResetConfront ( EMCONFRONT_END emEND )
{
	//	Note : Ã¼·Â È¯¿ø.
	//
	DoConftStateRestore();

	//	Note : ´ë·Ã Á¾·á¸¦ (ÀÚ½Å) ¿¡ÀÌÀüÆ®¿¡ ¾Ë¸².
	GLMSG::SNETPC_CONFRONT_END2_AGT NetMsgMyAgt;
	NetMsgMyAgt.emEND = emEND;
	m_pGLGaeaServer->SENDTOAGENT ( m_dwClientID, &NetMsgMyAgt );

	//	Note : (ÀÚ½Å) Å¬¶óÀÌ¾ðÆ®¿¡.
	GLMSG::SNETPC_CONFRONT_END2_CLT NetMsgMyClt;
	NetMsgMyClt.emEND = emEND;
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsgMyClt );

	//	ÆÄÆ¼¿¡ ÀÖ´Â '´ë·Ã' Âü°¡Á¤º¸¸¦ ¸®¼Â.
	GLMSG::SNETPC_CONFRONT_END2_CLT_MBR NetMsgTarMbr;

	switch ( m_sCONFTING.emTYPE )
	{
	case EMCONFT_PARTY:
		{
			GLPARTY_FIELD *pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);
			if ( pParty )
			{
				NetMsgTarMbr.dwID = m_dwGaeaID;

				//	´ë·Ã ¸®½ºÆ®¿¡¼­ Á¦°Å.
				pParty->MBR_CONFRONT_LEAVE(m_dwGaeaID);

				//	ÆÄÆ¼¿ø¿¡°Ô ´ë·Ã¿¡¼­ ºüÁ® ³ª°¨À» ¾Ë¸².
				m_pGLGaeaServer->SENDTOPARTYCLIENT ( m_dwPartyID, &NetMsgTarMbr );

				//	´ë·Ã »ó´ë¹æ ÆÄÆ¼¿ø¿¡°Ô.
				GLPARTY_FIELD *pPartyConft = m_pGLGaeaServer->GetParty ( pParty->m_dwconftPARTYID );
				if ( pPartyConft )
				{
					//	ÆÄÆ¼¿ø¿¡°Ô ´ë·Ã¿¡¼­ ºüÁ® ³ª°¨À» ¾Ë¸².
					m_pGLGaeaServer->SENDTOPARTYCLIENT ( pPartyConft->m_dwPARTYID, &NetMsgTarMbr );
				}
			}
		}
		break;

	case EMCONFT_GUILD:
		{
			GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
			GLCLUB *pCLUB = cClubMan.GetClub ( m_dwGuild );
			if ( pCLUB )
			{
				NetMsgTarMbr.dwID = m_dwCharID;

				//	Note : ÀÚ½ÅÀÇ Å¬·´ Á¤º¸¿¡¼­ ´ë·ÃÂü°¡ ¸â¹ö¿¡¼­ Á¦°Å.
				pCLUB->DELCONFT_MEMBER ( m_dwCharID );

				//	Note : ÀÚ½ÅÀÇ Å¬·´¿ø¿¡°Ô.
				m_pGLGaeaServer->SENDTOCLUBCLIENT ( m_dwGuild, &NetMsgTarMbr );

				//	Note : »ó´ë¹æ Å¬·´¿ø¿¡°Ô.
				GLCLUB *pTAR_CLUB = cClubMan.GetClub ( m_sCONFTING.dwTAR_ID );
				if ( pTAR_CLUB )
				{
					m_pGLGaeaServer->SENDTOCLUBCLIENT ( m_sCONFTING.dwTAR_ID, &NetMsgTarMbr );
				}
			}
		}
		break;
	};

	//	Note : ÁÖº¯ À¯´Öµé¿¡ ´ë·ÃÁ¾·á Åëº¸.
	//
	GLMSG::SNETPC_CONFRONT_END2_CLT_BRD NetMsgTarBrd;
	NetMsgTarBrd.dwGaeaID = m_dwGaeaID;
	NetMsgTarBrd.emEND = emEND;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgTarBrd );

	float conft_safe_time = 10.0f;
	if ( m_pLandMan )
	{
		/*pvp tyranny, Juver, 2017/08/24 */
		/*school wars, Juver, 2018/01/19 */
		/*pvp capture the flag, Juver, 2018/01/31 */
		/* pvp club death match, Juver, 2020/11/25 */
		if ( m_pLandMan->m_bGuidBattleMap ||
			m_pLandMan->m_bClubDeathMatchMap || 
			m_pLandMan->m_bPVPTyrannyMap || 
			m_pLandMan->m_bPVPSchoolWarsMap || 
			m_pLandMan->m_bPVPCaptureTheFlagMap ||
			m_pLandMan->m_bPVPClubDeathMatchLobbyMap ||
			m_pLandMan->m_bPVPClubDeathMatchBattleMap ||
			m_pLandMan->m_bPVPPBGLobbyMap ||
			m_pLandMan->m_bPVPPBGBattleMap )
		{
			conft_safe_time = 0.5f;

			/*CDebugSet::ToFileWithTime( "conft.txt", "char:%s safe:%g cw:%d cdm:%d tw:%d sw:%d ctf:%d cdm2_l:%d cdm2_b:%d pbg_l:%d pbg_b:%d", 
				m_szName, 
				conft_safe_time,
				m_pLandMan->m_bGuidBattleMap, 
				m_pLandMan->m_bClubDeathMatchMap, 
				m_pLandMan->m_bPVPTyrannyMap,
				m_pLandMan->m_bPVPSchoolWarsMap, 
				m_pLandMan->m_bPVPCaptureTheFlagMap,
				m_pLandMan->m_bPVPClubDeathMatchLobbyMap, 
				m_pLandMan->m_bPVPClubDeathMatchBattleMap,
				m_pLandMan->m_bPVPPBGLobbyMap,
				m_pLandMan->m_bPVPPBGBattleMap );*/
		}
	}

	m_sCONFTING.RESET ();
	m_sCONFTING.SETPOWERFULTIME ( conft_safe_time );

	/*skill summon, Juver, 2017/10/10 */
	if ( m_pGLGaeaServer )
	{
		for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		{
			PGLSUMMONFIELD pMySummon = m_pGLGaeaServer->GetSummon( m_dwSummonGUID_FLD[i] );
			if ( pMySummon && pMySummon->IsValid() )
			{
				m_pGLGaeaServer->DropOutSummon ( m_dwSummonGUID_FLD[i] );
			}
		}
	}	
}

void GLChar::VietnamGainCalculate()
{

	if ( IsSTATE(EM_ACT_WAITING) )		return; 
	if ( m_dwVietnamGainType == GAINTYPE_EMPTY )		return;
	// Áß±¹ ´©Àû ½Ã°£ º° ¼öÀÍ º¯°æ
	CTimeSpan gameTime( 0, (int)m_sVietnamSystem.gameTime / 60, (int)m_sVietnamSystem.gameTime % 60, 0 );
	CTime	  crtTime     = CTime::GetCurrentTime();
	CTimeSpan crtGameSpan, crtGame15Span;
	CTime	  loginTime   = m_sVietnamSystem.loginTime;
	crtGameSpan			  = gameTime + ( crtTime - loginTime );

//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;


	LONGLONG totalHours	  = crtGameSpan.GetTotalHours();


	// 5½Ã°£ ÀÌ»óÀÌ¸é °è»êÇÏÁö ¾Ê°í ³Ñ¾î°£´Ù.
	// ¼öÀÍ µî±ÞÀÌ ¹Ù²î¸é ¸Þ½ÃÁö¸¦ º¸³¿
	if( totalHours >= 5 && m_dwVietnamGainType != GAINTYPE_EMPTY )
	{
		m_dwVietnamGainType = GAINTYPE_EMPTY;	

		GLMSG::SNETPC_VIETNAM_GAINTYPE NetMsg;
		NetMsg.dwGainType = (BYTE)GAINTYPE_EMPTY;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
		// ¼öÀÍ 50%
	}else if( totalHours >= 3 && m_dwVietnamGainType != GAINTYPE_HALF )
	{	
		m_dwVietnamGainType = GAINTYPE_HALF;	

		GLMSG::SNETPC_VIETNAM_GAINTYPE NetMsg;
		NetMsg.dwGainType = (BYTE)GAINTYPE_HALF;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
	}
	m_sVietnamSystem.currentGameTime = totalHours;
}

void GLChar::ChinaGainCalculate()
{
	if ( IsSTATE(EM_ACT_WAITING) )		return; 
	// 18¼¼ ÀÌ»óÀÌ¸é ¸®ÅÏÇÑ´Ù.
	if( m_sChinaTime.userAge == 1 ) return;

//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;

	// Áß±¹ ´©Àû ½Ã°£ º° ¼öÀÍ º¯°æ
	CTimeSpan gameTime( 0, (int)m_sChinaTime.gameTime / 60, (int)m_sChinaTime.gameTime % 60, 0 );
	CTime	  crtTime     = CTime::GetCurrentTime();
	CTimeSpan crtGameSpan, crtGame15Span;
	CTime	  loginTime   = m_sChinaTime.loginTime;
	crtGameSpan			  = gameTime + ( crtTime - loginTime );

	// ¼öÀÍ 0%
	//if( m_ChinaGainType == 2 )
	//{
	//	crtGame15Span = crtTime - m_China15Time;
	//	// 15ºÐÀÌ Áö³ª¸é ¸Þ½ÃÁö¸¦ º¸³¿
	//	if( crtGame15Span.GetTotalMinutes() > 15 )
	//	{
	//		m_China15Time	= CTime::GetCurrentTime();

	//		GLMSG::SNETPC_CHINA_GAINTYPE NetMsg;
	//		NetMsg.dwGainType = 3;
	//		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
	//	}
	//}else
	if( m_ChinaGainType != 2 )
	{
		LONGLONG totalHours	  = crtGameSpan.GetTotalHours();
//		LONGLONG totalMinutes = crtGameSpan.GetTotalMinutes();
//		LONGLONG totalSecond  = crtGameSpan.GetTotalSeconds();

		// ¼öÀÍ µî±ÞÀÌ ¹Ù²î¸é ¸Þ½ÃÁö¸¦ º¸³¿
		if( totalHours >= 5 && m_ChinaGainType != GAINTYPE_EMPTY )
			//if( totalSecond >= 60 && m_ChinaGainType != 2 )
		{
			m_China15Time	= CTime::GetCurrentTime();
			m_ChinaGainType = GAINTYPE_EMPTY;	

			GLMSG::SNETPC_CHINA_GAINTYPE NetMsg;
			NetMsg.dwGainType = GAINTYPE_EMPTY;
			m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
			// ¼öÀÍ 50%
		}else if( totalHours >= 3 && m_ChinaGainType != GAINTYPE_HALF  )
			//}else if( totalSecond >= 50 && m_ChinaGainType != 2 )
		{	
			m_ChinaGainType = GAINTYPE_HALF;

			GLMSG::SNETPC_CHINA_GAINTYPE NetMsg;
			NetMsg.dwGainType = GAINTYPE_HALF;
			m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
		}else if( totalHours >= 1 && m_ChinaGainType == 255 )
			//}else if( totalSecond >= 40 && m_ChinaGainType != 2 )
		{
			m_ChinaGainType = 0;

			GLMSG::SNETPC_CHINA_GAINTYPE NetMsg;
			NetMsg.dwGainType = 4;
			m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsg );
		}
	}
	m_sChinaTime.currentGameTime = crtGameSpan.GetTotalMinutes();
}

void GLChar::EventCalculate()
{
	if ( IsSTATE(EM_ACT_WAITING) )		return; 

	SEventState sEventState = m_pGLGaeaServer->m_sEventState;

	if( sEventState.bEventStart == FALSE ) return;

	// ÀÌº¥Æ® Àû¿ë ·¹º§ÀÌ ¾Æ´Ï¸é ¸®ÅÏ
	if( m_bEventApply == FALSE ) return;

	CTime	  crtTime     = CTime::GetCurrentTime();
	CTimeSpan crtGameSpan;
	CTime	  loginTime			 = m_sEventTime.loginTime;
	crtGameSpan					 = ( crtTime - loginTime );
	m_sEventTime.currentGameTime = crtGameSpan.GetTimeSpan();

	// ÀÌº¥Æ®°¡ »õ·Î ½ÃÀÛ µÊ
	if( m_bEventStart == FALSE )
	{
		// Å×½ºÆ® ½Ã¿£ Second·Î ÇÑ´Ù.
		if( crtGameSpan.GetTotalSeconds() >= sEventState.EventPlayTime )
		{
			m_bEventStart = TRUE;

			//	Note : ÀÚ½ÅÀÇ Å¬¶óÀÌ¾ðÆ®¿¡.
			GLMSG::SNET_GM_LIMIT_EVENT_APPLY_START NetMsg;
			NetMsg.loginTime = m_sEventTime.loginTime;
			//dmk14 btg info
			NetMsg.expGain_Rate = sEventState.fExpGainRate;
			NetMsg.itemGain_Rate = sEventState.fItemGainRate;

			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*)&NetMsg );

		}
	}

	if( m_bEventStart == TRUE )
	{
		if( crtGameSpan.GetTotalSeconds() >= sEventState.EventPlayTime + sEventState.EventBusterTime )
		{
			m_bEventStart = FALSE;
			m_sEventTime.loginTime = crtTime.GetTime();

			// »õ·Î °»½ÅµÈ ÀÌº¥Æ® ½Ã°£
			GLMSG::SNET_GM_LIMIT_EVENT_RESTART NetMsg;
			NetMsg.restartTime = crtTime.GetTime();

			m_pGLGaeaServer->SENDTOAGENT(m_dwClientID,&NetMsg);

			//	Note : ÀÚ½ÅÀÇ Å¬¶óÀÌ¾ðÆ®¿¡.
			GLMSG::SNET_GM_LIMIT_EVENT_APPLY_END NetMsg2;
			NetMsg2.loginTime = m_sEventTime.loginTime;
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*)&NetMsg2 );
		}
	}




}

HRESULT GLChar::FrameMove ( float fTime, float fElapsedTime )
{
	GASSERT(m_pLandMan);
	//Pandora
	if( m_sPandoraStatus.fReqDelay > 0.0f )
        m_sPandoraStatus.fReqDelay -= fElapsedTime;

//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;

#if defined(VN_PARAM) //vietnamtest%%%
	// º£Æ®³² Å½´Ð ¹æÁö °è»ê
	VietnamGainCalculate();
#endif

#ifdef CH_PARAM_USEGAIN
	// Áß±¹ Å½´Ð ¹æÁö °è»ê
	ChinaGainCalculate();
#else
	// ¸®¹ÌÆ® ÀÌº¥Æ® °è»ê
	EventCalculate();
#endif

	if ( !IsSTATE(EM_ACT_WAITING) && m_bEntryFailed )
	{
		//  ÁøÀÔÁ¶°ÇÀÌ ¸ÂÁö¾Ê¾Æ ´Ù¸¥ °÷À¸·Î ÀÌµ¿ ÇØ¾ßÇÏ´Â °æ¿ì ¸Þ½ÃÁö¸¦ º¸³½´Ù.
		GLMSG::SNETENTRY_FAILED NetMsg;
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsg );

		m_bEntryFailed = FALSE;
	}

	

	// Å»°Í ¹èÅÍ¸® Ã¼Å©
	UpdateVehicle( fElapsedTime );


	if ( !IsSTATE(EM_GETVA_AFTER) && !(m_pGLGaeaServer->IsReserveServerStop ()) )	return S_FALSE;

	

	m_fAge += fElapsedTime;
	m_fGenAge += fElapsedTime;

	m_fSkillDelay += fElapsedTime;
	/*PK Streak Sigaw - Jxyyy */
	m_fPkStreakTimer += fElapsedTime;

	if( m_fPkStreakTimer >= 100.0f )
	{
		m_dwPkCounter = 0;
		m_dwResuCounter = 0;
		m_fPkStreakTimer = 0;
	}
	/*PK Streak Sigaw - Jxyyy End */
	/*continuous damage skill logic, Juver, 2017/06/10 */
	if ( m_pLandMan )
	{
		for( CONTINUOUS_DAMAGE_DATA_MAP_ITER iterdamage = m_mapContinuousDamage.begin();
			iterdamage != m_mapContinuousDamage.end(); ++iterdamage )
		{
			SCONTINUOUS_DAMAGE_DATA& sContinuousDamageData = (*iterdamage).second;
			sContinuousDamageData.fAge += fElapsedTime;

			if ( sContinuousDamageData.fAge >= sContinuousDamageData.fInterval )
			{
				SSKILLACT sSKILLACTCD;
				sSKILLACTCD.sID.dwID = sContinuousDamageData.dwCrowID;
				sSKILLACTCD.sID.emCrow = (EMCROW)sContinuousDamageData.wCrow;
				sSKILLACTCD.sID.vPos = m_vPos;
				sSKILLACTCD.sID_TAR.dwID = m_dwGaeaID;
				sSKILLACTCD.sID_TAR.emCrow = GETCROW();
				sSKILLACTCD.sID_TAR.vPos = m_vPos;

				sSKILLACTCD.fDELAY = 0.0f;
				sSKILLACTCD.emAPPLY = SKILL::EMAPPLY_PHY_LONG;
				sSKILLACTCD.nVAR_HP = (int)-sContinuousDamageData.fVar;
				sSKILLACTCD.dwDamageFlag = DAMAGE_TYPE_NONE;		

				if ( sSKILLACTCD.VALID() )
				{
					m_pLandMan->RegSkillAct ( sSKILLACTCD );
				}

				sContinuousDamageData.fAge = 0.0f;
			}
		}

		m_fContinuousDamageCheck += fElapsedTime;
		if ( m_fContinuousDamageCheck >= 1.0f )
		{
			for( CONTINUOUS_DAMAGE_DATA_MAP_ITER iterdamage = m_mapContinuousDamage.begin();
				iterdamage != m_mapContinuousDamage.end(); ++iterdamage )
			{
				SCONTINUOUS_DAMAGE_DATA sContinuousDamageData = (*iterdamage).second;

				BOOL _bFIND = FALSE;
				for( int _sf=0; _sf<SKILLFACT_SIZE; ++_sf )
				{
					if ( m_sSKILLFACT[_sf].sNATIVEID != NATIVEID_NULL() &&
						m_sSKILLFACT[_sf].sNATIVEID == sContinuousDamageData.sidSkill )
						_bFIND  = TRUE;
				}

				if ( !_bFIND )
					CONTINUOUSDAMAGE_DELETE( sContinuousDamageData.sidSkill );
			}

			m_fContinuousDamageCheck = 0.0f;
		}
	}

	/*vehicle booster system, Juver, 2017/08/11 */
	if ( m_bVehicle )
	{
		if ( m_bBoosterCharge )
		{
			m_fBoosterTimer += fElapsedTime;
			if ( m_fBoosterTimer >= GLCONST_CHAR::fVehicleBoosterChargeTime )
			{
				VehicleBoosterStateCharge(false);
			}
		}

		if ( !m_bBoosterCharge && m_bBoosterStart )
		{
			m_fBoosterTimer -= fElapsedTime;
			if ( m_fBoosterTimer <= 0.0f )
			{
				VehicleBoosterStateStart(false);
				VehicleBoosterStateCharge(true);
			}
		}
	}
	
	/*block back damage, Juver, 2017/12/12 */
	if ( m_bBlockBackDamage && RPARAM::bBlockDamageBack )
	{
		m_fBlockBackDamageTime += fElapsedTime;
		if ( m_fBlockBackDamageTime >= RPARAM::fBlockDamageBackTimer )
		{
			m_bBlockBackDamage = false;
			m_fBlockBackDamageTime = 0.0f;
		}
	}

	/* skill stigma, Juver, 2020/12/13 */
	SkillStigmaCheck();

	/*clubwar verify non push or pull, Juver, 2017/12/13 */
	if ( m_bClubWarVerify )
	{
		m_fClubWarVerifyTimer += fElapsedTime;
		if ( m_fClubWarVerifyTimer >= float( GLCONST_CHAR::fCDCERTIFY_TIME + 10.0f ) )
		{
			m_bClubWarVerify = FALSE;
			m_fClubWarVerifyTimer = 0.0f;
		}
	}

	/*inventory sort, Juver, 2018/07/07 */
	if ( m_fInventorySortTimer < REQ_INVENTORY_SORT_DELAY )
		m_fInventorySortTimer += fElapsedTime;

	/*item random option rebuild, Juver, 2018/07/04 */
	if ( RPARAM::item_random_option_rebuild_use_delay )
	{
		if ( m_item_random_option_rebuild_timer < RPARAM::item_random_option_rebuild_delay_time )
			m_item_random_option_rebuild_timer += fElapsedTime;
	}

	///*hide costume, EJCode, 2018/11/19 */
	//if ( RPARAM::hide_costume_use )
	//{
	//	if ( m_hide_costume_timer < RPARAM::hide_costume_delay_time )
	//		m_hide_costume_timer += fElapsedTime;
	//}

	/*client tick count check, EJCode, 2018/11/26 */
	if ( RPARAM::client_tick_count_check )
	{
		m_client_tick_count_check_send_timer += fElapsedTime;
		if ( m_client_tick_count_check_send_timer >= RPARAM::client_tick_count_timer )
		{
			client_tick_count_check_send();

			m_client_tick_count_check_send_timer = 0.0f;
		}
	}

	/*pet fix add call delay, EJCode, 2018/11/28 */
	if ( m_fPetUseDelay < RPARAM::pet_call_delay )
		m_fPetUseDelay += fElapsedTime;

	if ( m_fCurrencyShopDelayTimer < RPARAM::fCurrencyShopPurchaseDelayTime )
		m_fCurrencyShopDelayTimer += fElapsedTime;

	if ( m_fPersonalLockDelayTimer < RPARAM::fPersonalLockCoolDownTime )
		m_fPersonalLockDelayTimer += fElapsedTime;

	//	Æ®·¹ÀÌµå »óÅÂ °»½Å.
	if ( m_sTrade.Valid() )
	{
		m_sTrade.FrameMove ( fTime, fElapsedTime );

		PGLCHAR pChar = m_pGLGaeaServer->GetChar ( m_sTrade.GetTarget() );
		if ( !pChar )
		{
			GLMSG::SNET_TRADE_CANCEL_TAR NetMsg;
			NetMsg.emCancel = EMTRADE_CANCEL_NORMAL;

			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsg );

			m_sTrade.Reset ();
		}
	}

	//	DB¿¡ Á¤º¸ ÀúÀåÇÒ ½Ã°£ÀÌ µÇ¾ú´ÂÁö °Ë»çÈÄ¿¡ ÀúÀå ¼öÇà.
	//
	m_fSAVEDB_TIMER += fElapsedTime;
	if ( m_fSAVEDB_TIMER > 1800.0f )
	{
		m_fSAVEDB_TIMER = 0.0f;
		m_pGLGaeaServer->SaveCharDB ( m_dwGaeaID );
	}

	if ( m_sCONFTING.IsCONFRONTING() )
	{
		//	Note : ´ë·Ã À¯È¿¼º °Ë»ç.
		//
		bool bvalid(false);
		PGLCHAR pTAR_ONE(NULL);
		switch ( m_sCONFTING.emTYPE )
		{
		case EMCONFT_ONE:
			{
				pTAR_ONE = m_pGLGaeaServer->GetChar ( m_sCONFTING.dwTAR_ID );
				if ( pTAR_ONE )		bvalid = true;
			}
			break;

		case EMCONFT_PARTY:
			{
				bool bpartyA(false), bpartyB(false);
				if ( PARTY_NULL!=m_dwPartyID && m_pGLGaeaServer->GetParty(m_dwPartyID) )						bpartyA = true;
				if ( PARTY_NULL!=m_sCONFTING.dwTAR_ID && m_pGLGaeaServer->GetParty(m_sCONFTING.dwTAR_ID) )	bpartyB = true;

				if ( bpartyA && bpartyB )	bvalid = true;
			}
			break;

		case EMCONFT_GUILD:
			{
				GLClubMan &sClubMan = m_pGLGaeaServer->GetClubMan();

				bool bclubA(false), bclubB(false);
				if ( CLUB_NULL!=m_dwGuild && sClubMan.GetClub(m_dwGuild) )							bclubA = true;
				if ( CLUB_NULL!=m_sCONFTING.dwTAR_ID && sClubMan.GetClub(m_sCONFTING.dwTAR_ID) )	bclubB = true;

				if ( bclubA && bclubB )	bvalid = true;
			}
			break;
		};
		

		if ( !bvalid )
		{
			ResetConfront ( EMCONFRONT_END_FAIL );
		}
		else
		{
			bool bFIGHT = m_sCONFTING.UPDATE(fElapsedTime);
			if ( bFIGHT )
			{
				//	Note : ´ë·Ã FIGHT ¾Ë¸².
				//
				GLMSG::SNETPC_CONFRONT_FIGHT2_CLT NetMsg;
				m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsg );

				/* pet duel use bug Juver 2017/06/27 */
				PGLPETFIELD pMyPet = m_pGLGaeaServer->GetPET ( m_dwPetGUID );
				if ( pMyPet && pMyPet->IsValid () ) 
				{
					m_pGLGaeaServer->DropOutPET ( pMyPet->m_dwGUID, false, false );

					/*pet skill check, Juver, 2017/12/01 */
					CDebugSet::ToFileWithTime( "_petcheck.txt", "[%u]%s pet activated in duel start", m_dwCharID, m_szName );
				}
			}

			switch ( m_sCONFTING.emTYPE )
			{
			case EMCONFT_ONE:
				{
					if ( m_sCONFTING.sOption.bBOUND )
					{
						D3DXVECTOR3 vDistance = m_vPos - m_sCONFTING.vPosition;
						float fDistance = D3DXVec3Length ( &vDistance );

						if ( GLCONST_CHAR::fCONFRONT_ONE_DIST < fDistance )
						{
							ReceiveLivingPoint ( GLCONST_CHAR::nCONFRONT_LOSS_LP );

							ResetConfront ( EMCONFRONT_END_DISLOSS );
							pTAR_ONE->ResetConfront ( EMCONFRONT_END_DISWIN );
						}
					}

					//	´ë·ÃÀÌ 'Á¶°Ç'¿¡ ÀÇÇØ¼­ Ãë¼ÒµÌÀ» °æ¿ì.
					if ( !m_sCONFTING.IsCONFRONTING() )		break;

					//	Note : ¼­·Î °°Àº ÆÄÆ¼ÀÎÁö °Ë»ç.
					//
					if ( GetPartyID()!=PARTY_NULL && pTAR_ONE->GetPartyID()==GetPartyID() )
					{
						ResetConfront ( EMCONFRONT_END_PARTY );
					}
				}
				break;

			case EMCONFT_PARTY:
				{
					if ( m_sCONFTING.sOption.bBOUND )
					{
						D3DXVECTOR3 vDistance = m_vPos - m_sCONFTING.vPosition;
						float fDistance = D3DXVec3Length ( &vDistance );

						if ( GLCONST_CHAR::fCONFRONT_PY_DIST < fDistance )
						{
							ReceiveLivingPoint ( GLCONST_CHAR::nCONFRONT_LOSS_LP );

							ResetConfront ( EMCONFRONT_END_DISLOSS );
						}
					}
				}
				break;

			case EMCONFT_GUILD:
				{
					if ( m_sCONFTING.sOption.bBOUND )
					{
						D3DXVECTOR3 vDistance = m_vPos - m_sCONFTING.vPosition;
						float fDistance = D3DXVec3Length ( &vDistance );

						if ( GLCONST_CHAR::fCONFRONT_CLB_DIST < fDistance )
						{
							ReceiveLivingPoint ( GLCONST_CHAR::nCONFRONT_CLB_LOSS_LP );

							ResetConfront ( EMCONFRONT_END_DISLOSS );
						}
					}
				}
				break;
			};
		}
	}
	// ´ë·ÃÁ¾·áÈÄ µô·¹ÀÌ ½Ã°£ °»½Å
	else if ( m_sCONFTING.IsPOWERFULTIME () )
	{
		m_sCONFTING.UPDATEPWRFULTIME ( fElapsedTime );
	}

	//	Note : Àû´ë ÇàÀ§ÀÚÀÇ Å¸ÀÌ¸Ó °»½Å ¹× °ü¸®.
	{
		GLMSG::SNETPC_PLAYERKILLING_DEL NetMsgDel;		
		SPLAYHOSTILE* pHOSTILE = NULL;
		for ( MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.begin() ; pos!=m_mapPlayHostile.end() ; )
		{
			pHOSTILE = pos->second;
			if( !pHOSTILE )
			{
				//	Note : Àû´ëÀÚ »èÁ¦ ¾Ë¸².				
				NetMsgDel.dwCharID = pos->first;
				m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgDel );

				CDebugSet::ToLogFile( "GLChar::FrameMove, pos->second = NULL" );
				m_mapPlayHostile.erase( pos++ );
				continue;
			}

			pHOSTILE->fTIME -= fElapsedTime;
			if ( pHOSTILE->fTIME < 0 )
			{
				//	Note : Àû´ëÀÚ »èÁ¦ ¾Ë¸².				
				NetMsgDel.dwCharID = pos->first;
				m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgDel );

				SAFE_DELETE( pHOSTILE );
				m_mapPlayHostile.erase( pos++ );
				continue;
			}
			++pos;
		}

		if ( IsValidBody() && m_nBright < 0 )
		{
			m_fBRIGHT_TIMER += fElapsedTime;
			if ( m_fBRIGHT_TIMER >= GLCONST_CHAR::fPK_POINT_DEC_RATE )
			{
				m_fBRIGHT_TIMER = 0.0f;

				//	Note : ¼Ó¼º¼öÄ¡ º¯È­.
				m_nBright += 1;

				GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
				NetMsg.nBright = m_nBright;
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg);

				GLMSG::SNETPC_UPDATE_BRIGHT_BRD NetMsgBrd;
				NetMsgBrd.dwGaeaID = m_dwGaeaID;
				NetMsgBrd.nBright = m_nBright;
				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
			}
		}
	}

	//	½Ã°£ °»½Å.
	//
	m_fPERIOD_TIMER += fElapsedTime;
	if ( m_fPERIOD_TIMER > 360.0f )
	{
		m_fPERIOD_TIMER = 0.0f;
		MsgSendPeriod ();
	}

	m_fITEM_TIMER += fElapsedTime;
	if ( m_fITEM_TIMER > 1800.0f )
	{
		m_fITEM_TIMER = 0.0f;
		RESET_TIMELMT_ITEM ();
		RESET_CHECK_ITEM ();

		RESET_TIMELMT_SKILL();
	}

	/*qbox check, Juver, 2017/12/05 */
	m_fQBoxCheckTimer += fElapsedTime;
	if ( m_fQBoxCheckTimer >= QUESTION_BOX_CHECK_TIMER )
	{
		m_fQBoxCheckTimer = 0.0f;

		if ( m_sQITEMFACT.IsACTIVE() && m_pLandMan )
		{
			if ( !m_sQITEMFACT.bMobGen && !m_pLandMan->IsQBoxEnabled() )
			{
				CDebugSet::ToFileWithTime( "_qbox_check.txt", "[%d]%s qbox running in non qbox map", m_dwCharID, m_szName );

				m_sQITEMFACT.RESET();

				GLMSG::SNETPC_QITEMFACT_END_BRD	NetMsgBrd;
				NetMsgBrd.dwGaeaID = m_dwGaeaID;
				m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgBrd );
				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
			}
		}
	}

	/*pvp capture the flag, Juver, 2018/02/06 */
	if ( m_bCaptureTheFlagHoldFlag )
	{
		m_fCaptureTheFlagDetectGateTimer += fElapsedTime;
		if ( m_fCaptureTheFlagDetectGateTimer >= CAPTURE_THE_FLAG_DETECT_GATE_TIMER )
		{
			PVPCaptureTheFlagDetectGate();
			m_fCaptureTheFlagDetectGateTimer = 0.0f;
		}

		m_fCaptureTheFlagHoldFlagTimer += fElapsedTime;
		if ( m_fCaptureTheFlagHoldFlagTimer >= CAPTURE_THE_FLAG_HOLD_MAX_TIME )
		{
			PVPCaptureTheFlagHoldFlag( false );
			m_fCaptureTheFlagHoldFlagTimer = 0.0f;
		}
	}

	/*quest auto start in map, Juver, 2018/07/16 */
	if ( m_bQuestMapCheckTimer )
	{
		m_fQuestMapCheckTimer += fElapsedTime;
		if ( m_fQuestMapCheckTimer >= QUEST_MAP_CHECK_TIMER )
		{
			QuestStartFromMapReach( m_sQuestCheckMapID );

			m_bQuestMapCheckTimer = FALSE;
			m_fQuestMapCheckTimer = 0.0f;
		}
	}


	/*activity system, Juver, 2017/11/02 */
	if ( m_bActivityMapCheckTimer )
	{
		m_fActivityMapCheckTimer += fElapsedTime;
		if ( m_fActivityMapCheckTimer >= ACTIVITY_MAP_CHECK_TIMER )
		{
			DoActivityMapReach( m_sActivityCheckMapID );

			m_bActivityMapCheckTimer = FALSE;
			m_fActivityMapCheckTimer = 0.0f;
		}
	}

	if ( m_fMaxRVPreviewTimer < MAX_RV_PREVIEW_COOLTIME )
		m_fMaxRVPreviewTimer += fElapsedTime;


	if ( m_sCombatRecord.updateTimer(fElapsedTime) == true )
	{
		GLMSG::SNETPC_UPDATE_COMBAT_RECORD net_msg_client;
		net_msg_client.sCombatRecord = m_sCombatRecord;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
	}
	
	m_fMoveDelay += fElapsedTime;

	if ( m_sQITEMFACT.IsACTIVE() )
	{
		bool bOK = UPDATE_QITEMFACT(fElapsedTime);
		if ( !bOK )
		{
			GLMSG::SNETPC_QITEMFACT_END_BRD	NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;

			//	Note : Á¾·áµÇ¾úÀ» °æ¿ì ÀÚ½Å¿¡°Ô ¾Ë¸².
			m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgBrd );

			//	Note : Á¾·áµÇ¾úÀ» °æ¿ì ÁÖº¯ »ç¶÷¿¡°Ô ¾Ë¸².
			SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
		}
	}

	if ( m_lnLastSendExp != m_sExperience.lnNow )
	{
		m_lnLastSendExp = m_sExperience.lnNow;

		//	Note : °æÇèÄ¡ º¯È­ Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë·ÁÁÜ.
		//
		GLMSG::SNETPC_UPDATE_EXP NetMsgExp;
		NetMsgExp.lnNowExp = m_sExperience.lnNow;
		m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgExp );

		if ( m_sExperience.ISOVER() )
		{
			//	Note : ·¦¾÷ ¼öÇà. ( ·ÎÄÃ ¸Þ½ÃÁö ¹ß»ý. )
			//
			GLMSG::SNETPC_REQ_LEVELUP NetMsg;
			MsgReqLevelUp ( (NET_MSG_GENERIC*) &NetMsg );

			m_lnLastSendExp = m_sExperience.lnNow;


		}else{
			m_bVietnamLevelUp = FALSE; 
		}
	}

	if ( m_lnLastSendVNExp != m_lVNGainSysExp )
	{
		m_lnLastSendVNExp = m_lVNGainSysExp;

		//	Note : °æÇèÄ¡ º¯È­ Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë·ÁÁÜ.
		//
		GLMSG::SNETPC_VIETNAM_GAIN_EXP NetMsgExp;
		NetMsgExp.gainExp = m_lVNGainSysExp;
		m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgExp );

	}

#if defined(_RELEASED) || defined(TW_PARAM) || defined(HK_PARAM)
	if( m_lnTempMoney != m_lnMoney )
	{
		if( m_bMoneyUpdate )
		{
			m_bMoneyUpdate = FALSE;
		}else{
			// ÇØÅ·À¸·Î ·Î±×¸¦ ³²±è
			HACKINGLOG_WRITE( "Different Update Money!!, Account[%s], ID[%s], Money %I64d, TempMoney %I64d, Money Gap %I64d", 
								m_szUID, m_szName, m_lnMoney, m_lnTempMoney, m_lnMoney - m_lnTempMoney );	
		}
		m_lnTempMoney = m_lnMoney;
	}else{
		m_bMoneyUpdate = FALSE;
	}

	if( m_lnTempStorageMoney != m_lnStorageMoney )
	{
		if( m_bStorageMoneyUpdate )
		{
			m_bStorageMoneyUpdate = FALSE;
		}else{
			// ÇØÅ·À¸·Î ·Î±×¸¦ ³²±è
			HACKINGLOG_WRITE( "Different Update Storage Money!!, Account[%s], ID[%s], Storage Money %I64d, TempStorage Money %I64d, Storage Money Gap %I64d", 
							   m_szUID, m_szName, m_lnStorageMoney, m_lnTempStorageMoney, m_lnStorageMoney - m_lnTempStorageMoney );	
		}
		m_lnTempStorageMoney = m_lnStorageMoney;
	}else{
		m_bStorageMoneyUpdate = FALSE;
	}
#endif

	//	Note : °ø°Ý ¸ñÇ¥ÀÇ À¯È¿¼º °Ë»ç.
	//		(ÁÖÀÇ) Å¸°¹Á¤º¸ »ç¿ëÀü ¹Ýµå½Ã È£ÃâÇÏ¿© À¯È¿¼º °Ë»ç ÇÊ¿ä.
	//
	if ( m_TargetID.dwID != EMTARGET_NULL )
	{
		if ( !m_pGLGaeaServer->ValidCheckTarget(m_pLandMan,m_TargetID) )
			m_TargetID.dwID = EMTARGET_NULL;
	}
	
	if ( m_TargetID.dwID == EMTARGET_NULL && IsACTION(GLAT_ATTACK) )	TurnAction ( GLAT_IDLE );

	if ( IsValidBody () )
	{
		DoQuestCheckLimitTime ( fTime, fElapsedTime );

		if ( m_sHP.wNow>0 )
		{
			float fCONFT_POINT_RATE(1.0f);
			if ( m_sCONFTING.IsCONFRONTING() )		fCONFT_POINT_RATE = m_sCONFTING.sOption.fHP_RATE;


			// ÁöÇü ÀÌ»óÈ¿°ú ¾÷µ¥ÀÌÆ®
			UpdateLandEffect();

			
			//	Note : ±âÃÊ LOGIC Á¤º¸¸¦ ¾÷´ëÀÌÆ®.
			//
			GLCHARLOGIC_SERVER::UPDATE_DATA ( fTime, fElapsedTime, FALSE, fCONFT_POINT_RATE );

			UpdateClientState ( fElapsedTime );

			m_ARoundSlot.Update ( m_pLandMan );

			DoQuestReachZone();

			/*activity system, Juver, 2017/11/02 */
			if ( m_sActivityCheckMapID != m_sMapID )
			{
				m_sActivityCheckMapID = m_sMapID;

				m_bActivityMapCheckTimer = TRUE;
				m_fActivityMapCheckTimer = 0.0f;
			}

			/*quest auto start in map, Juver, 2018/07/16 */
			if ( m_sQuestCheckMapID != m_sMapID )
			{
				m_sQuestCheckMapID = m_sMapID;

				m_bQuestMapCheckTimer = TRUE;
				m_fQuestMapCheckTimer = 0.0f;
			}

			AutoPotionUpdate();
		}
		else
		{
			//	Note : ¾²·¯Áú¶§ º¸»ó ¹ß»ý. ( °æÇèÄ¡ + ¾ÆÀÌÅÛ + ±Ý¾× )
			DoFalling ();

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
			if ( m_bTracingUser )
			{
				NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
				TracingMsg.nUserNum  = GetUserID();
				StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

				CString strTemp;
				strTemp.Format( "Dead!!, [%s][%s], Money:[%I64d]", m_szUID, m_szName, m_lnMoney );
				StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

				m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
			}
#endif
		}


	}

	if ( m_pAutoPotion )
		m_pAutoPotion->FrameMove( fElapsedTime );


	/* force auto revive, Juver, 2020/12/23 */
	ForceAutoReviveCheck( fElapsedTime );
	
	/* pk combo, Juver, 2021/02/09 */
	PKComboUpdate( fElapsedTime );


	// ¸ÊÀÇ À§Ä¡°¡ ¹Ù²¼À»¶§ Äù½ºÆ® ½ÇÆÐ ¿©ºÎ¸¦ ÆÇ´ÜÇÑ´Ù.
	if( m_sMapID != m_sOldMapID )
	{
		DoQuestCheckLeaveMap();

		m_sOldMapID = m_sMapID;

		/*pvp capture the flag, Juver, 2018/02/07 */
		if ( m_bCaptureTheFlagHoldFlag )
		{
			GLPVPCaptureTheFlagField::GetInstance().FlagResetBase( m_dwCharID, CAPTURE_THE_FLAG_FLAG_HOLD_STATUS_LEFTBATTLE );
		}
	}

	/*skill summon, Juver, 2017/10/10 */
	m_fSummonCheckTime += fElapsedTime;
	if ( m_fSummonCheckTime >= SUMMON_VALID_CHECK_TIME && m_pGLGaeaServer )
	{
		for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		{
			if ( m_dwSummonGUID_FLD[i] != GAEAID_NULL )
			{
				PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_dwSummonGUID_FLD[i] );
				if ( !psummon_field )
				{
					CDebugSet::ToFileWithTime( "_summon.txt", "summon id stuck no longer connected to summon id:%d", m_dwSummonGUID_FLD[i]);
					m_dwSummonGUID_FLD[i] = GAEAID_NULL;
				}
			}
		}
	}	

	/*contribution/activity bonus, Juver, 2017/10/14 */
	//if( m_bUseContributionPointTimer )
	//{
	//	m_fContributionPointBonusTimer += fElapsedTime;
	//	if ( m_fContributionPointBonusTimer >= m_fContributionPointBonusTimeMax )
	//	{
	//		m_fContributionPointBonusTimer = 0.0f;
	//		m_dwContributionPoint += GLCONST_CHAR::wContributionPointBonusAdd;
	//		/*12-9-14, Battle Pass - CNDev*/
	//		DoBattlePassGainContributionPoint( m_dwContributionPoint );
	//		
	//		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netmsgclient;
	//		netmsgclient.bNotice = true;
	//		netmsgclient.dwPoint = m_dwContributionPoint;
	//		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netmsgclient );
	//	}
	//}

	/*contribution/activity bonus, Juver, 2017/10/14 */
	/*if ( m_bUseActivityPointTimer )
	{
		m_fActivityPointBonusTimer += fElapsedTime;
		if ( m_fActivityPointBonusTimer >= m_fActivityPointBonusTimeMax )
		{
			m_fActivityPointBonusTimer = 0.0f;
			m_dwActivityPoint += GLCONST_CHAR::wActivityPointBonusAdd;

			GLMSG::SNETPC_UPDATE_ACTIVITY_POINT netmsgclient;
			netmsgclient.bNotice = true;
			netmsgclient.dwPoint = m_dwActivityPoint;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netmsgclient );
		}
	}*/

	/*product item, Juver, 2017/10/18 */
	if ( m_bItemCompoundTask )
	{
		m_fItemCompoundTaskTimer += fElapsedTime;

		if ( m_fItemCompoundTaskTimer >= m_fItemCompoundTaskTime )
		{
			m_bItemCompoundTask = FALSE;
			m_fItemCompoundTaskTimer = 0.0f;
			m_fItemCompoundTaskTime = 0.0f;

			DoItemCompoundTask();
		}
	}

	/*12-16-24, daily login - CNDev*/
	if( m_bDailyLogin )
	{
		if ( !m_bDailyLoginTimer )
		{
			GLMSG::SNETPC_DAILYLOGIN_NOTIFY_CLIENT netmsgclient;
			netmsgclient.fRemain = RPARAM::fDailyLoginTimeMax;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netmsgclient );

			m_bDailyLoginTimer = true;
		}

		m_fDailyLoginTimer += fElapsedTime;
		if ( m_fDailyLoginTimer >= RPARAM::fDailyLoginTimeMax )
		{
			m_fDailyLoginTimer = 0.0f;
			UpdateDailyLogin();
		}
	}
	switch ( m_Action )
	{
	case GLAT_IDLE:
		m_fIdleTime += fElapsedTime;
		break;

	case GLAT_TALK:
		break;

	case GLAT_MOVE:
		{
			//	Note : ÄÉ¸¯ÀÇ ÀÌµ¿ ¾÷µ¥ÀÌÆ®.
			//
			m_actorMove.SetMaxSpeed ( GetMoveVelo () );
			m_actorMove.Update ( fElapsedTime );
			if ( !m_actorMove.PathIsActive() )
			{
				m_actorMove.Stop ();
				TurnAction ( GLAT_IDLE );
			}

			//	Note : ÄÉ¸¯ÀÇ ÇöÁ¦ À§Ä¡ ¾÷µ¥ÀÌÆ®.
			//
			m_vPos = m_actorMove.Position();

			//	Note : ÄÉ¸¯ÀÇ ÇöÁ¦ ¹æÇâ ¾÷µ¥ÀÌÆ®.
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
			if ( !AttackProcess ( fElapsedTime ) )
			{
				TurnAction ( GLAT_IDLE );
			}

			GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, m_TargetID );
			if ( pACTOR )
			{
				D3DXVECTOR3 vDirection = pACTOR->GetPosition() - m_vPos;
				D3DXVec3Normalize ( &vDirection, &vDirection );
				m_vDir = vDirection;
			}
		}
		break;

	case GLAT_SKILL:
		{
			//anti shit skillcut-as
			m_bIsSkillProcess = SkillProcess ( fElapsedTime );
			//if( !SkillProcess ( fElapsedTime ) )		
			if( !m_bIsSkillProcess )
				TurnAction ( GLAT_IDLE );
		}
		break;

	case GLAT_SHOCK:
		{
			if( !ShockProcess ( fElapsedTime ) )		TurnAction ( GLAT_IDLE );
		}
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

			//	Note : MobÀÇ ÇöÁ¦ À§Ä¡ ¾÷µ¥ÀÌÆ®.
			//
			m_vPos = m_actorMove.Position();
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


	//	Note : MobÀÇ ÇöÁ¦ À§Ä¡ ¾÷µ¥ÀÌÆ®.
	//
	m_vPos = m_actorMove.Position();

	return S_OK;
}

void GLChar::STATEBLOW ( const SSTATEBLOW &sStateBlow )
{
	int nIndex = 0;

	if ( sStateBlow.emBLOW <= EMBLOW_SINGLE )	nIndex = 0;
	else										nIndex = sStateBlow.emBLOW-EMBLOW_SINGLE;

	m_sSTATEBLOWS[nIndex] = sStateBlow;

	//	»óÅÂÀÌ»ó ( ±âÀý ) À» ¹ÞÀ»¶§ ÇöÁ¦ ÇàÀ§ Áß´Ü.
	if ( sStateBlow.emBLOW == EMBLOW_STUN )
	{
		//	Note : ¾²·¯ÁüÀ» Å¬¶óÀÌ¾ðÆ® µé¿¡°Ô ¾Ë¸².
		GLMSG::SNET_ACTION_BRD NetMsgBrd;
		NetMsgBrd.emCrow	= CROW_PC;
		NetMsgBrd.dwID		= m_dwGaeaID;
		NetMsgBrd.emAction	= GLAT_IDLE;

		//	Note : ÁÖº¯ÀÇ Char ¿¡°Ô.
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

		//	Note : ÀÚ½ÅÀÇ Å¬¶óÀÌ¾ðÆ®¿¡.
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsgBrd );

		TurnAction ( GLAT_IDLE );
	}
}

void GLChar::CURE_STATEBLOW ( DWORD dwCUREFLAG )
{
	BOOL bChanged = FALSE;

	for ( int i=0; i<EMBLOW_MULTI; ++i )
	{
		if ( m_sSTATEBLOWS[i].emBLOW==EMBLOW_NONE )		continue;

		bChanged = TRUE;
		if ( STATE_TO_DISORDER(m_sSTATEBLOWS[i].emBLOW)&dwCUREFLAG )
		{
			m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE;
		}
	}

	if ( !bChanged )	return;

	//	Note : »óÅÂ ÀÌ»ó º¯È­ MsgFB.
	//
	GLMSG::SNETPC_CURESTATEBLOW_BRD NetMsgBRD;
	NetMsgBRD.dwID = m_dwGaeaID;
	NetMsgBRD.emCrow = CROW_PC;
	NetMsgBRD.dwCUREFLAG = dwCUREFLAG;

	//	Note : ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	//
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );

	//	Note : ´ë»ó¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgBRD );
}

/*debuff skill logic, Juver, 2017/06/10 */
void GLChar::BUFF_REMOVE( DWORD dwBuffFlag )
{
	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
	
        PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;
		if ( pSkill->m_sBASIC.emACTION != (SKILL::EMACTION)dwBuffFlag ) continue;
		
		DISABLESKEFF( i );
		NetMsgSkillBrd.bRESET[i] = true;
	}
	
	NetMsgSkillBrd.dwID = m_dwGaeaID;
	NetMsgSkillBrd.emCrow = CROW_PC;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgSkillBrd );
}

void GLChar::VAR_BODY_POINT ( const EMCROW emACrow, const DWORD dwAID, const BOOL bPartySkill, int nvar_hp, int nvar_mp, int nvar_sp )
{
	if ( m_sHP.wNow == 0 )	return;

	WORD wDxHP = m_sHP.VARIATION ( nvar_hp );
	m_sMP.VARIATION ( nvar_mp );
	m_sSP.VARIATION ( nvar_sp );

	m_sAssault.emCrow = emACrow;
	m_sAssault.dwID = dwAID;

	BOOL bDamage = nvar_hp<0;
	
	if ( emACrow == CROW_PC && nvar_hp<0 )
	{
		PGLCHAR pChar = m_pGLGaeaServer->GetChar ( dwAID );
		if ( pChar )
		{
			AddDamageLog ( m_cDamageLog, dwAID, pChar->m_dwUserID, wDxHP );
		}
	}

	if ( emACrow == CROW_PC && nvar_hp>0 )
	{
		PGLCHAR pChar = m_pGLGaeaServer->GetChar ( dwAID );
		if ( pChar )
		{
			if ( m_pLandMan )
			{
				/*pvp tyranny, Juver, 2017/08/24 */
				if ( m_pLandMan->m_bPVPTyrannyMap )
					GLPVPTyrannyField::GetInstance().ScoreHeal( m_sMapID, pChar->m_dwCharID, wDxHP, m_dwCharID );

				/*school wars, Juver, 2018/01/20 */
				if ( m_pLandMan->m_bPVPSchoolWarsMap )
					GLPVPSchoolWarsField::GetInstance().ScoreHeal( pChar->m_dwCharID, wDxHP, m_dwCharID );

				/*pvp capture the flag, Juver, 2018/02/01 */
				if ( m_pLandMan->m_bPVPCaptureTheFlagMap )
					GLPVPCaptureTheFlagField::GetInstance().ScoreHeal( pChar->m_dwCharID, wDxHP, m_dwCharID );

				if ( m_pLandMan->m_bPVPPBGBattleMap )
					PVPPBG::ManagerField::GetInstance().ScoreHeal( pChar->m_dwCharID, wDxHP, m_dwCharID );
			}
		}
	}

	// ¼ÒÈ¯¼ö°¡ ¶§·ÈÀ» °æ¿ì ¼ÒÈ¯¼ö ÁÖÀÎÀÌ ¶§¸°°É·Î µî·Ï
	if ( emACrow == CROW_SUMMON && nvar_hp<0 )
	{
		PGLSUMMONFIELD pSummon = GLGaeaServer::GetInstance().GetSummon ( dwAID );
		if( pSummon )
		{
			AddDamageLog ( m_cDamageLog, dwAID, pSummon->m_pOwner->m_dwUserID, wDxHP );
		}
	}

	//	[ÆÄÆ¼¿øµé¿¡°Ô]
	if ( m_dwPartyID!=PARTY_NULL )
	{
		GLMSG::SNET_PARTY_MBR_POINT NetMsg;
		NetMsg.dwGaeaID = m_dwGaeaID;
		NetMsg.sHP = m_sHP;
		NetMsg.sMP = m_sMP;
		m_pGLGaeaServer->SENDTOPARTYCLIENT ( m_dwPartyID, (NET_MSG_GENERIC*) &NetMsg );
	}

	if ( nvar_hp<0 && IsACTION(GLAT_GATHERING) )
	{
		GLMSG::SNET_ACTION_BRD NetMsgBrd;
		NetMsgBrd.emCrow	= CROW_PC;
		NetMsgBrd.dwID		= m_dwGaeaID;
		NetMsgBrd.emAction	= GLAT_IDLE;

		//	Note : ÁÖº¯ÀÇ Char ¿¡°Ô.
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsgBrd );		
		TurnAction ( GLAT_IDLE );
	}

	if ( bDamage )
	{
		/* skill effect release, Juver, 2020/04/30 */
		SkillEffectReleaseCheck();
	}
}

DWORD GLChar::SELECT_SKILLSLOT ( SNATIVEID skill_id )
{
	SKILLREALFACT_SIZE;
	DWORD dwSELECT = UINT_MAX;

	//	Note : ½ºÅ³ÀÌ µé¾î°¥ ±âº» ½½·Ô ¼±ÅÃ.
	//
	for ( DWORD i=0; i<SKILLREALFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID==skill_id )
		{
			return dwSELECT = i;
		}
	}

	float fAGE = FLT_MAX;
	for ( DWORD i=0; i<SKILLREALFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID==SNATIVEID(false) )
		{
			return dwSELECT = i;
		}

		if ( m_sSKILLFACT[i].fAGE < fAGE )
		{
			fAGE = m_sSKILLFACT[i].fAGE;
			dwSELECT = i;
		}
	}

	return dwSELECT;
}

BOOL GLChar::RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, DWORD &dwSELECT, WORD _wCasterCrow, DWORD _dwCasterID )
{
	dwSELECT = SKILLFACT_SIZE;

	//	½ºÅ³ Á¤º¸ °¡Á®¿È.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )				return FALSE;
	if ( wlevel >= SKILL::MAX_SKILL_LEVEL )	return FALSE;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wlevel];

	/*pvp capture the flag, Juver, 2018/02/08 */
	if ( m_bCaptureTheFlagHoldFlag && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_BUFF )	return FALSE;

	BOOL bHOLD = FALSE;
	SSKILLFACT sSKILLEF;
	sSKILLEF._wCasterCrow = _wCasterCrow;
	sSKILLEF._dwCasterID = _dwCasterID;

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
			sSKILLEF.sImpacts[nImpact].fADDON_VAR = pSkill->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[wlevel];
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
		case EMSPECA_TURN_INVISIBLE:		/* skill turn/find invisible, Juver, 2020/04/26 */
		case EMSPECA_SHOW_INVISIBLE:		/* skill turn/find invisible, Juver, 2020/04/26 */
		case EMSPECA_ATTACKVELO:
		case EMSPECA_SKILLDELAY:
		case EMSPECA_PSY_DAMAGE_REDUCE:
		case EMSPECA_MAGIC_DAMAGE_REDUCE:
		case EMSPECA_PSY_DAMAGE_REFLECTION:
		case EMSPECA_MAGIC_DAMAGE_REFLECTION:
		case EMSPECA_DEFENSE_SKILL_ACTIVE:
		case EMSPECA_PROHIBIT_POTION:		/*prohibit potion skill logic, Juver, 2017/06/06 */
		case EMSPECA_PROHIBIT_SKILL:		/*prohibit skill logic, Juver, 2017/06/06 */
		case EMSPECA_CHANGE_ATTACK_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_CHANGE_APPLY_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_STUN:					/*skill stun logic, Juver, 2017/06/06 */
		case EMSPECA_CONTINUOUS_DAMAGE:		/*continuous damage skill logic, Juver, 2017/06/10 */
		case EMSPECA_CURSE:					/*curse skill logic, Juver, 2017/06/10 */
		case EMSPECA_TALK_TO_NPC:			/*npc talk buff, Juver, 2017/10/03 */
		case EMSPECA_IGNORE_DAMAGE:			/*ignore damage, Juver, 2017/12/12 */
		case EMSPECA_SPECIAL_ITEM_DROP:		/*special item drop buff, Juver, 2018/07/01 */
		case EMSPECA_ITEMDROP_RATE:			/*item drop rate buff, Juver, 2018/07/01 */
		case EMSPECA_MONEYDROP_RATE:		/*gold drop buff, Juver, 2018/07/01 */
		case EMSPECA_GETEXP_RATE:			/*exp rate buff, Juver, 2018/07/01 */
		case EMSPECA_VEHICLE_OFF:			/*skill vehicle off, Juver, 2018/09/07 */
		case EMSPECA_PET_OFF:				/*skill pet off, Juver, 2018/09/07 */
		case EMSPECA_TRANSFORM:				/*skill transform, Juver, 2018/09/08 */
		case EMSPECA_EFFECT_RELEASE:		/* skill effect release, Juver, 2020/04/30 */
		case EMSPECA_AMPLIFY:				/* skill amplify, Juver, 2020/12/10 */
		case EMSPECA_STIGMA:				/* skill stigma, Juver, 2020/12/11 */
		case EMSPECA_HOSTILE_ON:			/* skill hostile, Juver, 2020/12/16 */
		case EMSPECA_HOSTILE_OFF:			/* skill hostile, Juver, 2020/12/16 */
		case EMSPECA_DURATION_CHANGE:		/* skill buff duration change, Juver, 2020/12/16 */
		case EMSPECA_IMMUNE:				/* immune skill logic, Juver, 2020/12/24 */
		case EMSPECA_ILLUSION:				/* skill illusion, Juver, 2021/01/17 */
			bHOLD = TRUE;
			sSKILLEF.sSpecs[nSpec].emSPEC = pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC;
			sSKILLEF.sSpecs[nSpec].fSPECVAR1 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].fVAR1;
			sSKILLEF.sSpecs[nSpec].fSPECVAR2 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].fVAR2;
			sSKILLEF.sSpecs[nSpec].dwSPECFLAG = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].dwFLAG;
			sSKILLEF.sSpecs[nSpec].dwNativeID = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].dwNativeID;
			break;
		};
	}

	//	Note : Áö¼ÓÇü ½ºÅ³ÀÏ °æ¿ì ÃÖÀû ½½·ÔÀ» Ã£¾Æ¼­ ½ºÅ³ È¿°ú¸¦ ³Ö¾îÁÜ.
	//
	if ( bHOLD )
	{
		sSKILLEF.sNATIVEID	= skill_id;
		sSKILLEF.wLEVEL		= wlevel;
		sSKILLEF.fAGE		= sSKILL_DATA.fLIFE;

		/* skill buff duration change, Juver, 2020/12/17 */
		if ( pSkill )
		{
			switch ( pSkill->m_sBASIC.emACTION )
			{
			case SKILL::EMACTION_NORMAL:
				sSKILLEF.fAGE += m_sSkillDurationChange.fSecondsNormal;
				break;

			case SKILL::EMACTION_BUFF:
				sSKILLEF.fAGE += m_sSkillDurationChange.fSecondsBuff;
				break;

			case SKILL::EMACTION_DEBUFF:
				sSKILLEF.fAGE += m_sSkillDurationChange.fSecondsDebuff;
				break;

			case SKILL::EMACTION_LIMIT:
				sSKILLEF.fAGE += m_sSkillDurationChange.fSecondsLimit;
				break;
			};
		}
		

		dwSELECT = SELECT_SKILLSLOT ( skill_id );
		m_sSKILLFACT[dwSELECT] = sSKILLEF;
	}


	if ( m_pLandMan && dwSELECT < SKILLFACT_SIZE )
	{
		const SSKILLFACT& _sSkillFact = m_sSKILLFACT[dwSELECT];

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			switch ( _sSkillFact.sSpecs[nSpec].emSPEC )
			{
				/* skill stigma, Juver, 2020/12/12 */
			case EMSPECA_STIGMA:
				{
					STARGETID sTargetID ( (EMCROW)_wCasterCrow, _dwCasterID );

					GLACTOR *pACTOR = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
					if ( pACTOR )	
						pACTOR->LinkSkillStigmaTarget ( CROW_PC, m_dwGaeaID, _sSkillFact.sNATIVEID, _sSkillFact.sSpecs[nSpec].fSPECVAR1, _sSkillFact.sSpecs[nSpec].fSPECVAR2, _sSkillFact.sSpecs[nSpec].dwSPECFLAG, m_sMapID );
				}break;

				/* skill illusion, Juver, 2021/01/17 */
			case EMSPECA_ILLUSION:
				{
					m_sSkillIllusion.sSkillID			= _sSkillFact.sNATIVEID;
					m_sSkillIllusion.fIllusionJumpBound	= _sSkillFact.sSpecs[nSpec].fSPECVAR1 * 10.0f;
					m_sSkillIllusion.wIllusionRemain	= (WORD)_sSkillFact.sSpecs[nSpec].fSPECVAR2;
				}break;
			}
		}
	}


	return TRUE;
}

BOOL GLChar::RECEIVE_DRUGFACT ( EMITEM_DRUG emDRUG, WORD wCURE, BOOL bRATIO )
{
	//SKILL::EMTYPES emTYPE[3] = { SKILL::EMFOR_VARHP, SKILL::EMFOR_VARMP, SKILL::EMFOR_VARSP };
	//SNATIVEID nidNO[3] = { SNATIVEID(35,USHRT_MAX), SNATIVEID(35,USHRT_MAX), SNATIVEID(35,USHRT_MAX) };

	//if ( bRATIO )
	//{
	//	wCURE = m_sHP.wMax * wCURE / 100;
	//}

	//switch ( emDRUG )
	//{
	//case ITEM_DRUG_HP:
	//	nidNO[0].wSubID = 0;
	//	break;

	//case ITEM_DRUG_MP:
	//	nidNO[1].wSubID = 1;
	//	break;

	//case ITEM_DRUG_SP:
	//	nidNO[2].wSubID = 2;
	//	break;

	//case ITEM_DRUG_HP_MP:
	//	nidNO[0].wSubID = 0;
	//	nidNO[1].wSubID = 1;
	//	break;

	//case ITEM_DRUG_MP_SP:
	//	nidNO[1].wSubID = 1;
	//	nidNO[2].wSubID = 2;
	//	break;

	//case ITEM_DRUG_HP_MP_SP:
	//	nidNO[0].wSubID = 0;
	//	nidNO[1].wSubID = 1;
	//	nidNO[2].wSubID = 2;
	//	break;
	//};

	//for ( int i=0; i<3; ++i )
	//{
	//	const SNATIVEID &skill = nidNO[i];

	//	if ( skill.wSubID == USHRT_MAX )		continue;

	//	SSKILLFACT sSKILLEF;
	//	sSKILLEF.emTYPE = emTYPE[i];
	//	
	//	//	½ºÅ³ Á¤º¸ °¡Á®¿È.
	//	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill );
	//	if ( !pSkill )				return FALSE;
	//	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[0];

	//	float fRATE = wCURE/float(m_sHP.wMax);
	//	if ( fRATE>1.0f )	fRATE = 1.0f;

	//	float fLIFE = sSKILL_DATA.fLIFE;
	//	if ( fLIFE==0.0f )	fLIFE = 0.01f;

	//	sSKILLEF.sNATIVEID	= skill;
	//	sSKILLEF.wLEVEL		= 0;
	//	sSKILLEF.fAGE		= fLIFE;
	//	sSKILLEF.fMVAR		= fRATE/fLIFE;

	//	WORD wSELECT = SKILLREALFACT_SIZE + i;
	//	m_sSKILLFACT[wSELECT] = sSKILLEF;

	//	GLMSG::SNETPC_REQ_SKILLHOLDEX_BRD NetMsgBRD;
	//	NetMsgBRD.emCrow = GetCrow();
	//	NetMsgBRD.dwID = GetCtrlID();
	//	NetMsgBRD.wSLOT = wSELECT;
	//	NetMsgBRD.sSKILLEF = sSKILLEF;

	//	//	Note : '½ºÅ³´ë»ó'ÀÇ ÁÖº¯ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	//	//
	//	SNDMSGAROUND ( (NET_MSG_GENERIC*) &NetMsgBRD );

	//	//	Note : ½ºÅ³ ´ë»ó¿¡°Ô ¸Þ¼¼Áö Àü¼Û.
	//	m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID(), &NetMsgBRD );
	//}

	return TRUE;
}

void GLChar::ReBirthDecExp ()
{
	if ( m_nDECEXP==0 )		return;

	m_lnReExp = GetReExp();

	if ( (m_sExperience.lnNow-m_nDECEXP) < 0 )	m_sExperience.lnNow = 0;
	else	m_sExperience.lnNow -= m_nDECEXP;

	//	Note : °æÇèÄ¡ º¯È­ Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë·ÁÁÜ.
	//
	GLMSG::SNETPC_UPDATE_EXP NetMsgExp;
	NetMsgExp.lnNowExp = m_sExperience.lnNow;
	m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgExp );
}

LONGLONG GLChar::GetReExp ()
{
	LONGLONG nDecExp = 0;
	LONGLONG nReExp = 0;

	nDecExp = m_nDECEXP;

	if ( nDecExp > m_sExperience.lnNow ) nDecExp = m_sExperience.lnNow;
	if ( nDecExp <= 0 )	return 0;


	float fRExpRecovery = GLOGICEX::GLDIE_RECOVERYEXP( m_wLevel );
	nReExp = (LONGLONG)(nDecExp * fRExpRecovery);

	if ( nReExp < 0  ) nReExp = 0;	
	if ( nDecExp < nReExp ) nReExp = nDecExp;

	return nReExp;

}
LONGLONG GLChar::GetReExpMoney ( LONGLONG nReExp )
{
	LONGLONG nDecMoney = 0;

	if ( nReExp <= 0 ) return 0;

	float fRMoney = GLOGICEX::GLDIE_EXPMONEY( m_wLevel );
	nDecMoney = (LONGLONG)(nReExp * fRMoney);

	return nDecMoney;
}

void GLChar::DROP_PUTONITEM ( DWORD _dwNUM, float _fRATE )
{
//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;
	GLITEMLMT & glItemmt = GLITEMLMT::GetInstance();

	for ( DWORD i=0; i<_dwNUM; ++i )
	{
		if ( RANDOM_GEN(_fRATE) )
		{
			DWORD dwSLOT_NUM(0);

			DWORD dwSLOT_LIST[SLOT_NSIZE_S_2];
			for ( int j=0; j<SLOT_NSIZE_S_2; ++j )
			{
				EMSLOT emSLOT = EMSLOT(j);

				if ( VALID_SLOT_ITEM(emSLOT) && !GET_SLOT_ITEM(emSLOT).IsGM_GENITEM() )
				{
					SITEM* pITEM = GET_SLOT_ITEMDATA(emSLOT);
					
					//	°Å·¡¿É¼Ç
					if ( pITEM && pITEM->sBasicOp.IsTHROW() )
					{
						SITEM* pitem_costume = GLItemMan::GetInstance().GetItem( GET_SLOT_ITEM(emSLOT).nidDISGUISE );
						if( pitem_costume && !pitem_costume->sBasicOp.IsTHROW() )	continue;

						dwSLOT_LIST[dwSLOT_NUM++] = j;
					}
				}
			}

			if ( dwSLOT_NUM==0 )					break;

			DWORD dwSLOT = (DWORD) ( rand()%dwSLOT_NUM );
			
			if ( dwSLOT >= SLOT_NSIZE_S_2 )				break;

			dwSLOT = dwSLOT_LIST[dwSLOT];
			if ( !VALID_SLOT_ITEM(EMSLOT(dwSLOT)) )	break;

			// º¸µå´Â µå¶øµÇÁö ¾Ê´Â´Ù.
			if ( dwSLOT == SLOT_VEHICLE )			break;

			SITEMCUSTOM sITEM_CUSTOM = m_PutOnItems[dwSLOT];
			
			D3DXVECTOR3 vGenPos = D3DXVECTOR3(m_vPos) + D3DXVECTOR3(RANDOM_NUM*4.0f,0.0f,RANDOM_NUM*4.0f);
			BOOL bCollision;
			D3DXVECTOR3 vCollision;
			bCollision = m_pLandMan->IsCollisionNavi
			(
				D3DXVECTOR3(vGenPos)+D3DXVECTOR3(0,+5,0),
				D3DXVECTOR3(vGenPos)+D3DXVECTOR3(0,-5,0),
				vCollision
			);

			if ( bCollision )
			{
				//	Note :¾ÆÀÌÅÛÀÇ ¼ÒÀ¯ ÀÌÀü °æ·Î ±â·Ï.
				//
				glItemmt.ReqItemRoute ( sITEM_CUSTOM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_GROUND, sITEM_CUSTOM.wTurnNum );


				PGLPETFIELD pMyPet = m_pGLGaeaServer->GetPET ( m_dwPetGUID );
				if ( pMyPet && pMyPet->IsValid () ) 
				{
					/*dual pet skill, Juver, 2017/12/27 */
					int nPetMID_A = pMyPet->m_sActiveSkillID_A.wMainID;
					int nPetSID_A = pMyPet->m_sActiveSkillID_A.wSubID;
                    // Note : ÇöÀç Æê ½ºÅ³ È®ÀÎ ·Î±× 
					glItemmt.ReqAction ( m_dwCharID,		// ´ç»çÀÚ.
										EMLOGACT_PET_SKILL, // ÇàÀ§.
										ID_CHAR, 0,  0,				
										nPetMID_A,			// Pet Skill MID
										nPetSID_A, 			// Pet Skill SID
										0 );		

					/*dual pet skill, Juver, 2017/12/27 */
					int nPetMID_B = pMyPet->m_sActiveSkillID_B.wMainID;
					int nPetSID_B = pMyPet->m_sActiveSkillID_B.wSubID;
					// Note : ÇöÀç Æê ½ºÅ³ È®ÀÎ ·Î±× 
					glItemmt.ReqAction ( m_dwCharID,		// ´ç»çÀÚ.
										EMLOGACT_PET_SKILL, // ÇàÀ§.
										ID_CHAR, 0,  0,				
										nPetMID_B,			// Pet Skill MID
										nPetSID_B, 			// Pet Skill SID
										0 );		
				}


				//	¾ÆÀÌÅÛ Á¦°Å.
				GLCHARLOGIC_SERVER::RELEASE_SLOT_ITEM ( (EMSLOT) dwSLOT );

				//	[ÀÚ½Å¿¡°Ô] SLOT¿¡ ÀÖ¾ú´ø ¾ÆÀÌÅÆ Á¦°Å.
				GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release((EMSLOT)dwSLOT);
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

				//	¾ÆÀÌÅÛ ¹ß»ý.
				m_pLandMan->DropItem ( vGenPos, &sITEM_CUSTOM );

				//generate serial based on time and charid
				CTime cTime = CTime::GetCurrentTime();
				CString strTime = cTime.Format( _T("%Y%m%d[%H%M%S]") );

				CString strSerial;
				strSerial.Format( "[%d]%s", m_dwCharID, strTime.GetString() );

				GLMSG::SNETPC_CAPTURE_PUTON_DROP NetMsg_Capture_Puton_Drop;
				StringCchCopy( NetMsg_Capture_Puton_Drop.szSerial, LOG_PUTON_SERIAL_SIZE+1, strSerial.GetString() );
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsg_Capture_Puton_Drop);

				bool bActivePet = false;
				SNATIVEID sActivePetSkill_A(false);
				SNATIVEID sActivePetSkill_B(false);

				if ( pMyPet && pMyPet->IsValid () ) 
				{
					bActivePet = true;
					sActivePetSkill_A = pMyPet->m_sActiveSkillID_A;
					sActivePetSkill_B = pMyPet->m_sActiveSkillID_B;
				}

				//file log
				RLOG::LogFile( "_drop_puton.txt", "[char:%u]%s item:[%d/%d] gen:%d map:%u/%u fact:%u/%u %u/%u pet:%d %u/%u %u/%u serial:%s", 
					m_dwCharID, m_szName, sITEM_CUSTOM.sNativeID.wMainID, sITEM_CUSTOM.sNativeID.wSubID, sITEM_CUSTOM.cGenType, m_sMapID.wMainID, m_sMapID.wSubID,
					m_sPETSKILLFACT_A.sNATIVEID.wMainID, m_sPETSKILLFACT_A.sNATIVEID.wSubID, m_sPETSKILLFACT_B.sNATIVEID.wMainID, m_sPETSKILLFACT_B.sNATIVEID.wSubID,
					bActivePet, sActivePetSkill_A.wMainID, sActivePetSkill_A.wSubID, sActivePetSkill_B.wMainID, sActivePetSkill_B.wSubID, strSerial.GetString() );

				//database log
				CLogPutOnDrop *pdb_action = new CLogPutOnDrop( m_dwClientID, m_dwCharID, m_szName, strSerial.GetString(), sITEM_CUSTOM.sNativeID, sITEM_CUSTOM.cGenType, m_sMapID, m_sPETSKILLFACT_A.sNATIVEID, m_sPETSKILLFACT_B.sNATIVEID, bActivePet, sActivePetSkill_A, sActivePetSkill_B );
				
				if( GLGaeaServer::GetInstance().GetDBMan() )
					GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

				
			}
		}
	}
}

void GLChar::GenerateReward ( bool bCONFT /*=false*/, bool bCONFT_SCHOOL /*=false*/ )
{
	/*combatpoint logic, Juver, 2017/05/29 */
	if ( !bCONFT && !bCONFT_SCHOOL && m_pGLGaeaServer )
	{
		//reset cp when dead
		if ( m_pLandMan && m_pLandMan->isCPResetDeath() )
		{
			m_sCombatPoint.wNow = 0;
			GLMSG::SNETPC_UPDATE_CP NetMsgUpdateCP;
			NetMsgUpdateCP.wNowCP = m_sCombatPoint.wNow;
			NetMsgUpdateCP.bDie = TRUE;
			m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgUpdateCP );
		}
		
		if ( m_sAssault.emCrow==CROW_PC )
		{
			GLChar* pChar = m_pGLGaeaServer->GetChar( m_sAssault.dwID );
			if ( pChar ){
				pChar->ReceiveCP( (int)GLCONST_CHAR::wCombatPoint_PK, false );
			}
		}
		else if ( m_sAssault.emCrow==CROW_SUMMON )
		{
			PGLSUMMONFIELD pSummon = m_pGLGaeaServer->GetSummon ( m_sAssault.dwID );
			if( pSummon ) {
				if ( pSummon->m_pOwner ){
					pSummon->m_pOwner->ReceiveCP( (int)GLCONST_CHAR::wCombatPoint_PK, true );
				}
			}
		}
	}
	
	//	Note : °æÇèÄ¡ ¼Õ»ó Ã³¸®.
	//
	bool bBRIGHTEVENT = m_pGLGaeaServer->IsBRIGHTEVENT( m_sMapID );		/*dmk14 freepk*/

	m_nDECEXP = 0;
	// ¹üÁËÀÚ ÀÌ°Å³ª ¸÷ÇÑÅ× Á×¾úÀ»°æ¿ì
	/*skill summon, Juver, 2017/10/10 */
	if ( ISOFFENDER() || ( m_sAssault.emCrow!=CROW_PC && m_sAssault.emCrow!=CROW_SUMMON ) )
	{
		m_nDECEXP = __int64 ( GLOGICEX::GLDIE_DECEXP(GETLEVEL()) * 0.01f * GET_LEVELUP_EXP() );
		
		if ( bCONFT )
		{
			//m_nDECEXP /= 10;	//	´ë·Ã½Ã °æÇèÄ¡ ¼Õ»óÀº ¹Ý°¨½ÃÅ²´Ù.
			m_nDECEXP = 0;		//	´ë·Ã½Ã °æÇèÄ¡ ¼Õ»óÀº ¾øÀ½.
		}
		else
		{
			m_nDECEXP = __int64 ( m_nDECEXP * GET_PK_DECEXP_RATE()/100.0f );
		}
	}

	//	Note : °æÇèÄ¡ ¹ß»ý.
	//
	if ( m_nDECEXP > 0 )
	{
		//STARGETID mytarid(GETCROW(),m_dwGaeaID);

		//if ( m_sAssault.emCrow == CROW_PC && m_sAssault.dwID!=GAEAID_NULL )
		//{
		//	PGLCHAR pChar = m_pGLGaeaServer->GetChar ( m_sAssault.dwID );
		//	if ( pChar )
		//	{
		//		pChar->ReceiveKillExp ( mytarid, bCONFT_SCHOOL );
		//	}
		//}

		//if ( !m_cDamageLog.empty() )
		//{
		//	DAMAGELOG_ITER iter = m_cDamageLog.begin ();
		//	DAMAGELOG_ITER iter_end = m_cDamageLog.end ();
		//	for ( ; iter!=iter_end; ++iter )
		//	{
		//		SDAMAGELOG &sDamageLog = (*iter).second;

		//		PGLCHAR pChar = m_pGLGaeaServer->GetChar ( sDamageLog.dwGaeaID );
		//		if ( !pChar )	continue;
		//		if ( sDamageLog.dwUserID != pChar->m_dwUserID )		continue;

		//		pChar->ReceiveAttackExp ( mytarid, sDamageLog.dwDamage, FALSE, bCONFT_SCHOOL );
		//	}
		//}
	}

	m_cDamageLog.clear ();

	/*pvp capture the flag, Juver, 2018/02/07 */
	if ( m_pLandMan && m_pLandMan->m_bPVPCaptureTheFlagMap && m_bCaptureTheFlagHoldFlag )
	{
		GLPVPCaptureTheFlagField::GetInstance().FlagResetBase( m_dwCharID, CAPTURE_THE_FLAG_FLAG_HOLD_STATUS_DEAD );
	}

	// ¼ºÇâ ÀÌº¥Æ® Áß¿¡´Â °æÇèÄ¡ °¨¼ÒÇÏÁö ¾Ê°í ¾ÆÀÌÅÛµµ µå¶øµÇÁö ¾ÊÀ½
	if ( bBRIGHTEVENT ) 
	{
		m_nDECEXP = 0; 
		return;
	}

	// Å¬·´ ¹èÆ²ÁßÀÌ¸é °æÇèÄ¡ ¼Õ½Ç ¹× ¾ÆÀÌÅÛ µå¶ø ¾øÀ½
	if ( m_sAssault.emCrow==CROW_PC && m_pLandMan->IsClubBattleZone() )
	{
		GLClubMan &cClubMan = GLGaeaServer::GetInstance().GetClubMan();
		GLCLUB *pCLUB = cClubMan.GetClub ( m_dwGuild );
		if ( pCLUB )
		{
			GLChar* pChar = GLGaeaServer::GetInstance().GetChar( m_sAssault.dwID );
			if ( pChar )
			{
				if ( pCLUB->IsBattle( pChar->m_dwGuild ) )
				{
					m_nDECEXP = 0;
					return;
				}
				
				GLCLUB *pCLUB_Tar = cClubMan.GetClub ( pChar->m_dwGuild );
				if ( pCLUB_Tar && pCLUB->IsBattleAlliance( pCLUB_Tar->m_dwAlliance ) )
				{
					m_nDECEXP = 0;
					return;
				}
			}
		}
	}

	/*skill summon, Juver, 2017/10/10 */
	if ( m_sAssault.emCrow==CROW_SUMMON && m_pLandMan->IsClubBattleZone() )
	{
		GLClubMan &cClubMan = GLGaeaServer::GetInstance().GetClubMan();
		GLCLUB *pCLUB = cClubMan.GetClub ( m_dwGuild );
		if ( pCLUB )
		{
			PGLSUMMONFIELD psummon_field = GLGaeaServer::GetInstance().GetSummon( m_sAssault.dwID );
			if ( psummon_field && psummon_field->m_pOwner )
			{
				if ( pCLUB->IsBattle( psummon_field->m_pOwner->m_dwGuild ) )
				{
					m_nDECEXP = 0;
					return;
				}

				GLCLUB *pCLUB_Tar = cClubMan.GetClub ( psummon_field->m_pOwner->m_dwGuild );
				if ( pCLUB_Tar && pCLUB->IsBattleAlliance( pCLUB_Tar->m_dwAlliance ) )
				{
					m_nDECEXP = 0;
					return;
				}
			}
		}
	}

	// °æÇèÄ¡ NonDrop ¸ÊÀÏ°æ¿ì
	if ( !m_pLandMan->IsDecreaseExpMap () ) m_nDECEXP = 0;

	//	Note : ´ë·ÃÀÌ ¾Æ´Ï°í µå·Ó¸ÊÀÏ °æ¿ì ¾ÆÀÌÅÛ µå·Ó. && m_bProtectPutOnItem(ÆÖÀÇ ¾ÆÀÌÅÛ º¸È£½ºÅ³)
	//
	if ( m_pLandMan && m_pLandMan->IsItemDrop() && !bCONFT && !m_bProtectPutOnItem )
	{
		//	¹üÁËÀÚÀÏ °æ¿ì
		if ( ISOFFENDER() )
		{
			DWORD dwDROP_NUM = GET_PK_ITEMDROP_NUM();
			float fDROP_RATE = GET_PK_ITEMDROP_RATE();
			DROP_PUTONITEM ( dwDROP_NUM, fDROP_RATE );
		}
		//	ÀÏ¹ÝÀûÀ¸·Î Á×À»°æ¿ì.
		else if ( GLCONST_CHAR::fNONPK_ITEM_DROP!=0 )
		{
			DROP_PUTONITEM ( 1, GLCONST_CHAR::fNONPK_ITEM_DROP );
		}
	}

	//killed by mob
	if ( m_pLandMan && m_sAssault.emCrow==CROW_MOB )
	{
		PGLCROW pCrow = m_pLandMan->GetCrow(m_sAssault.dwID);
		if ( pCrow )
			m_sCombatRecord.increasePVEDeaths();
	}
}

void GLChar::DelPlayHostile ()
{
//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;

	GLMSG::SNETPC_PLAYERKILLING_DEL NetMsgDel;
	PGLCHAR pCHAR = NULL;
	for ( MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.begin(); pos != m_mapPlayHostile.end(); ++pos )
	{
		//	Note : Àû´ëÀÚ »èÁ¦ ¾Ë¸².		
		NetMsgDel.dwCharID = pos->first;
		m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgDel );

		pCHAR = m_pGLGaeaServer->GetCharID ( pos->first );
		if( pCHAR )
			pCHAR->DelPlayHostile ( m_dwCharID );

		if( !pos->second )
			CDebugSet::ToLogFile( "GLChar::DelPlayHostile, pos->second = NULL" );
	}

	DEL_PLAYHOSTILE_ALL();
}

void GLChar::DelPlayHostile ( DWORD dwCharID )
{
	bool bDEL = DEL_PLAYHOSTILE ( dwCharID );
	if ( bDEL )
	{
		//	Note : Àû´ëÀÚ »èÁ¦ ¾Ë¸².
		GLMSG::SNETPC_PLAYERKILLING_DEL NetMsgDel;
		NetMsgDel.dwCharID = dwCharID;
		m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgDel );

		/*skill summon, Juver, 2017/10/10 */
		if ( m_pLandMan )
		{
			for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
			{
				PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_dwSummonGUID_FLD[i] );
				if ( psummon_field )
				{
					if ( psummon_field->GetAttackTar().emCrow == CROW_PC )
					{
						PGLCHAR pchar_field = m_pGLGaeaServer->GetChar( psummon_field->GetAttackTar().dwID );
						if ( pchar_field && pchar_field->m_dwCharID == dwCharID )
						{
							psummon_field->ResetAttackTarget();
						}
					}
				}
			}
		}
		
	}
}

//	Æ¯Á¤ Å¬·´ÀÇ Àû´ëÀÚ¸¦ ¸ðµÎ Áö¿î´Ù.
void GLChar::DelPlayHostileClub( DWORD dwClubID )
{
	PGLCHAR pCHAR = NULL;

	GLMSG::SNETPC_PLAYERKILLING_DEL NetMsgDel;
	
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.begin();
	MAPPLAYHOSTILE_ITER pos_end = m_mapPlayHostile.end();
	MAPPLAYHOSTILE_ITER del;

	GLClubMan &sClubMan = m_pGLGaeaServer->GetClubMan();
	GLCLUB *pClub = sClubMan.GetClub(m_dwGuild);
	if ( !pClub )	return;
	
	for ( ; pos != pos_end;  )
	{
		del = pos++;
		//	Note : Àû´ëÀÚ »èÁ¦ ¾Ë¸².	
		pCHAR = m_pGLGaeaServer->GetCharID ( del->first );
		if ( !pCHAR ) continue;

		GLCLUB *pClub_S = sClubMan.GetClub(pCHAR->m_dwGuild);
		if ( !pClub_S )	return;

		if ( pClub->IsBattle( pCHAR->m_dwGuild ) || pClub->IsBattleAlliance( pClub_S->m_dwAlliance ) )
		{
			NetMsgDel.dwCharID = del->first;
			m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgDel );
			
			pCHAR->DelPlayHostile ( m_dwCharID );
		}

		m_mapPlayHostile.erase( del );
	}
}


void GLChar::DoFalling ()
{
//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;

	if ( m_sCONFTING.IsCONFRONTING() )
	{
		switch ( m_sCONFTING.emTYPE )
		{
		case EMCONFT_ONE:
			{
				/*skill summon, Juver, 2017/10/10 */
				BOOL bDie = FALSE;
				if ( m_pGLGaeaServer && m_sAssault.emCrow == CROW_SUMMON )
				{
					PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_sAssault.dwID );
					if ( psummon_field && psummon_field->m_pOwner )
					{
						if ( psummon_field->m_pOwner->m_dwGaeaID != m_sCONFTING.dwTAR_ID )
							bDie = TRUE;
					}
				}
				else
				{
					STARGETID sCONFTID(CROW_PC,m_sCONFTING.dwTAR_ID);
					if( m_sAssault != sCONFTID )	bDie = TRUE;
				}

				//	Note : ´ë·ÃµµÁß ´ë·ÃÀÚ°¡ ¾Æ´Ñ Ä³¸¯ÅÍ³ª ¸÷¿¡°Ô Á×À» °æ¿ì.
				//
				if( bDie )
				{
					//	Note : »ó´ë¹æ ´ë·ÃÀÚ ID ¹é¾÷. Reset ½Ã¿¡ ¹«È¿È­µÇ±â ¶§¹®¿¡ ¹Ì¸® ¹é¾÷.
					//
					const DWORD dwTAR_ID = m_sCONFTING.dwTAR_ID;

					//	Note : (ÀÚ½Å) ´ë·Ã Á¾·á.
					//
					ResetConfront(EMCONFRONT_END_FAIL);

					//	Note : ´ë·Ã »ó´ë¹æ Ã£À½.
					//
					PGLCHAR pCONFT = m_pGLGaeaServer->GetChar(dwTAR_ID);
					if ( pCONFT )
					{
						//	Note : ´ë·Ã »ó´ë¹æÀÌ ¿µÇâÀ» ÁØ °æÇèÄ¡ ·Î±× Á¦°Å½ÃÅ´.
						//
						DAMAGELOG_ITER iter;
						iter = m_cDamageLog.find ( pCONFT->m_dwUserID );
						if ( iter!=m_cDamageLog.end() )		m_cDamageLog.erase ( iter );

						//	Note : (»ó´ë¹æ) ´ë·Ã Á¾·á.
						//
						pCONFT->ResetConfront ( EMCONFRONT_END_FAIL );
					}

					goto _DIE_ACTION;
				}

				//	Note : ¾²·¯Áú¶§ º¸»ó ¹ß»ý. ( °æÇèÄ¡ + ¾ÆÀÌÅÛ + ±Ý¾× µî µî )
				//
				GenerateReward ( true, m_sCONFTING.sOption.bSCHOOL );

				//	Note : »ó´ë¹æ ´ë·ÃÀÚ ID ¹é¾÷. Reset ½Ã¿¡ ¹«È¿È­µÇ±â ¶§¹®¿¡ ¹Ì¸® ¹é¾÷.
				//
				const DWORD dwTAR_ID = m_sCONFTING.dwTAR_ID;
				const SCONFT_OPTION sOption = m_sCONFTING.sOption;

				//	Note : (ÀÚ½Å) ´ë·Ã Á¾·á.
				//
				ResetConfront(EMCONFRONT_END_LOSS);

				//	Note : ´ë·Ã »ó´ë¹æ Ã£À½.
				//
				PGLCHAR pCONFT = m_pGLGaeaServer->GetChar(dwTAR_ID);
				if ( pCONFT==NULL )			return;

				//	Note : (»ó´ë¹æ) ´ë·Ã Á¾·á.
				//
				EMCONFRONT_END emCONFT_END = (pCONFT->GETHP()>0) ? EMCONFRONT_END_WIN : EMCONFRONT_END_LOSS;
				pCONFT->ResetConfront ( emCONFT_END );

				//	Note : ÀÚ½Å »ýÈ°Á¡¼ö º¯È­.
				//
				if ( GLCONST_CHAR::nCONFRONT_LOSS_LP!=0 && GLCONST_CHAR::nCONFRONT_WIN_LP!=0 )
				{
					float fLIFE_P(0);

					fLIFE_P = GLCONST_CHAR::nCONFRONT_LOSS_LP - (GETLEVEL()-pCONFT->GETLEVEL())/2.0f
						+ (sOption.fHP_RATE-sOption.fTAR_HP_RATE);
					LIMIT ( fLIFE_P, 0.0f, -20.0f );

					ReceiveLivingPoint ( (int)fLIFE_P );

					//	Note : »ó´ë¹æ »ýÈ°Á¡¼ö º¯È­.
					//
					fLIFE_P = GLCONST_CHAR::nCONFRONT_WIN_LP + (GETLEVEL()-pCONFT->GETLEVEL())/2.0f
						+ (sOption.fHP_RATE-sOption.fTAR_HP_RATE);
					LIMIT ( fLIFE_P, 20.0f, 0.0f );

					pCONFT->ReceiveLivingPoint ( (int)fLIFE_P );
				}
			}
			break;

		case EMCONFT_PARTY:
			{
				bool breward(false);
				if ( m_sAssault.emCrow==CROW_PC )
				{
					GLPARTY_FIELD *pConftParty = m_pGLGaeaServer->GetParty(m_sCONFTING.dwTAR_ID);
					if ( pConftParty )
					{
						const GLPARTY_FNET *pPARTY = pConftParty->GETMEMBER(m_sAssault.dwID);
						if ( pPARTY )	breward = pPARTY->m_bConfront;
					}
				}

				/*skill summon, Juver, 2017/10/10 */
				if ( m_pGLGaeaServer && m_sAssault.emCrow==CROW_SUMMON )
				{
					PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_sAssault.dwID );
					if ( psummon_field && psummon_field->m_pOwner )
					{
						GLPARTY_FIELD *pConftParty = m_pGLGaeaServer->GetParty(m_sCONFTING.dwTAR_ID);
						if ( pConftParty )
						{
							const GLPARTY_FNET *pPARTY = pConftParty->GETMEMBER(psummon_field->m_pOwner->m_dwGaeaID);
							if ( pPARTY )	breward = pPARTY->m_bConfront;
						}
					}
				}

				if ( !breward )
				{
					m_cDamageLog.clear();

					//	Note : (ÀÚ½Å) ´ë·Ã Á¾·á.
					//
					ResetConfront(EMCONFRONT_END_NOTWIN);

					goto _DIE_ACTION;
				}

				//	Note : ¾²·¯Áú¶§ º¸»ó ¹ß»ý. ( °æÇèÄ¡ + ¾ÆÀÌÅÛ + ±Ý¾× µî µî )
				//
				GenerateReward ( true, m_sCONFTING.sOption.bSCHOOL );

				//	Note : (ÀÚ½Å) ´ë·Ã Á¾·á.
				//
				ResetConfront(EMCONFRONT_END_NOTWIN);
			}
			break;

		case EMCONFT_GUILD:
			{
				bool breward(false);
				if ( m_sAssault.emCrow==CROW_PC )
				{
					GLClubMan &sClubMan = m_pGLGaeaServer->GetClubMan();
					PGLCHAR pCHAR = m_pGLGaeaServer->GetChar(m_sAssault.dwID);

					GLCLUB *pConftClub = sClubMan.GetClub(m_sCONFTING.dwTAR_ID);
					if ( pConftClub && pCHAR )
					{
						breward = pConftClub->ISCONFT_MEMBER ( pCHAR->m_dwCharID );
					}
				}

				/*skill summon, Juver, 2017/10/10 */
				if ( m_pGLGaeaServer && m_sAssault.emCrow==CROW_SUMMON )
				{
					PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_sAssault.dwID );
					if ( psummon_field && psummon_field->m_pOwner )
					{
						GLClubMan &sClubMan = m_pGLGaeaServer->GetClubMan();
						PGLCHAR pCHAR = m_pGLGaeaServer->GetChar(psummon_field->m_pOwner->m_dwGaeaID);

						GLCLUB *pConftClub = sClubMan.GetClub(m_sCONFTING.dwTAR_ID);
						if ( pConftClub && pCHAR )
						{
							breward = pConftClub->ISCONFT_MEMBER ( pCHAR->m_dwCharID );
						}
					}
				}

				if ( !breward )
				{
					m_cDamageLog.clear();

					//	Note : (ÀÚ½Å) ´ë·Ã Á¾·á.
					//
					ResetConfront(EMCONFRONT_END_NOTWIN);

					goto _DIE_ACTION;
				}

				//	Note : ¾²·¯Áú¶§ º¸»ó ¹ß»ý. ( °æÇèÄ¡ + ¾ÆÀÌÅÛ + ±Ý¾× µî µî )
				//
				GenerateReward ( true );

				//	Note : (ÀÚ½Å) ´ë·Ã Á¾·á.
				//
				ResetConfront(EMCONFRONT_END_NOTWIN);
			}
			break;
		}

		//	Memo :	´ë·ÃÁßÀÌ¶ó¸é ¾Æ·¡ÄÚµå´Â ½ÇÇàÇÏÁö ¾Ê´Â´Ù.
		return;
	}

	if ( m_sQITEMFACT.IsACTIVE() )
	{
		m_sQITEMFACT.RESET();

		GLMSG::SNETPC_QITEMFACT_END_BRD	NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;

		//	Note : Á¾·áµÇ¾úÀ» °æ¿ì ÀÚ½Å¿¡°Ô ¾Ë¸².
		m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgBrd );

		//	Note : Á¾·áµÇ¾úÀ» °æ¿ì ÁÖº¯ »ç¶÷¿¡°Ô ¾Ë¸².
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

_DIE_ACTION:

	//	Note : player °¡ Á×¿´À» °æ¿ì.
	DWORD dwKILL_ID(0);

	PGLCHAR pCHAR  =  NULL;

	/*skill summon, Juver, 2017/10/10 */
	if ( m_sAssault.emCrow==CROW_PC )
		pCHAR = m_pGLGaeaServer->GetChar(m_sAssault.dwID);
	else if (  m_sAssault.emCrow==CROW_SUMMON )
	{
		PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_sAssault.dwID );
		if ( psummon_field && psummon_field->m_pOwner )
			pCHAR = psummon_field->m_pOwner;
	}

	if ( pCHAR )
	{
		dwKILL_ID = pCHAR->m_dwCharID;
		int nBRIGHT(0), nLIFE(0);

		bool bClubBattle = false;
		bool bAllianceBattle = false;
		GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
		GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
		GLCLUB *pTarClub = cClubMan.GetClub ( pCHAR->m_dwGuild );
		
		if ( pMyClub && pTarClub )
		{
			bClubBattle =  pMyClub->IsBattle( pCHAR->m_dwGuild );
			bAllianceBattle = pMyClub->IsBattleAlliance ( pTarClub->m_dwAlliance );
		}

		bool bClubDeathMatch = false;

		if ( m_pLandMan->m_bClubDeathMatchMap )
		{
			GLClubDeathMatch* pCDM = GLClubDeathMatchFieldMan::GetInstance().Find( m_pLandMan->m_dwClubMapID );
			if( pCDM && pCDM->IsBattle() ) bClubDeathMatch = true;
		}


		/*pvp tyranny, Juver, 2017/08/24 */
		bool bPVPTyranny = false;
		bool bPVPTyrannyCombatRecord = false;
		if ( m_pLandMan->m_bPVPTyrannyMap && GLPVPTyrannyField::GetInstance().IsBattle() )
		{
			bPVPTyranny = true;

			if ( GLPVPTyrannyField::GetInstance().isCombatRecord() )
			{
				bPVPTyrannyCombatRecord = true;
				pCHAR->PKIncreaseScore( m_dwCharID );
				PKIncreaseDeath( pCHAR->m_dwCharID );

				pCHAR->UpdatePKRankingsScore( pCHAR->m_dwCharID );
				UpdatePKRankingsDeath( m_dwCharID );

				pCHAR->UpdateMMRIncrease( m_dwCharID );
				UpdateMMRDecrease( pCHAR->m_dwCharID );
			}
		}

		/*school wars, Juver, 2018/01/19 */
		bool bPVPSchoolWars = false;
		bool bPVPSchoolWarsCombatRecord = false;
		if ( m_pLandMan->m_bPVPSchoolWarsMap && GLPVPSchoolWarsField::GetInstance().IsBattle() )
		{
			bPVPSchoolWars = true;

			if ( GLPVPSchoolWarsField::GetInstance().isCombatRecord() )
			{
				bPVPSchoolWarsCombatRecord = true;
				pCHAR->PKIncreaseScore( m_dwCharID );
				PKIncreaseDeath( pCHAR->m_dwCharID );

				pCHAR->UpdatePKRankingsScore( pCHAR->m_dwCharID );
				UpdatePKRankingsDeath( m_dwCharID );

				pCHAR->UpdateMMRIncrease( m_dwCharID );
				UpdateMMRDecrease( pCHAR->m_dwCharID );
			}
		}

		/*pvp capture the flag, Juver, 2018/01/31 */
		bool bPVPCaptureTheFlag = false;
		bool bPVPCaptureTheFlagCombatRecord = false;
		if ( m_pLandMan->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagField::GetInstance().IsBattle() )
		{
			bPVPCaptureTheFlag = true;

			if ( GLPVPCaptureTheFlagField::GetInstance().isCombatRecord() )
			{
				bPVPCaptureTheFlagCombatRecord = true;
				pCHAR->PKIncreaseScore( m_dwCharID );
				PKIncreaseDeath( pCHAR->m_dwCharID );

				pCHAR->UpdatePKRankingsScore( pCHAR->m_dwCharID );
				UpdatePKRankingsDeath( m_dwCharID );

				pCHAR->UpdateMMRIncrease( m_dwCharID );
				UpdateMMRDecrease( pCHAR->m_dwCharID );
			}
		}
		
		/*woe Arc Development 08-06-2024*/
		bool bPVPWoe = false;
		if (m_pLandMan->m_bPVPWoeMap && GLPVPWoeField::GetInstance().IsBattle())
		{
			bPVPWoe = true;
			//pCHAR->PKIncreaseScore( m_dwCharID );
			//PKIncreaseDeath( pCHAR->m_dwCharID );
		}

		/* pvp club death match, Juver, 2020/11/25 */
		bool bPVPClubDeathMatch = false;
		bool bPVPClubDeathMatchCombatRecord = false;
		if ( m_pLandMan->m_bPVPClubDeathMatchBattleMap && PVPClubDeathMatchField::GetInstance().IsBattle() )
		{
			bPVPClubDeathMatch = true;

			if ( PVPClubDeathMatchField::GetInstance().isCombatRecord() )
			{
				bPVPClubDeathMatchCombatRecord = true;
				pCHAR->PKIncreaseScore( m_dwCharID );
				PKIncreaseDeath( pCHAR->m_dwCharID );

				pCHAR->UpdatePKRankingsScore( pCHAR->m_dwCharID );
				UpdatePKRankingsDeath( m_dwCharID );

				pCHAR->UpdateMMRIncrease( m_dwCharID );
				UpdateMMRDecrease( pCHAR->m_dwCharID );
			}
		}

		bool bPVPPBG = false;
		bool bPVPPBGCombatRecord = false;
		if ( m_pLandMan->m_bPVPPBGBattleMap && PVPPBG::ManagerField::GetInstance().IsBattle() )
		{
			bPVPPBG = true;

			if ( PVPPBG::ManagerField::GetInstance().isCombatRecord() )
			{
				bPVPPBGCombatRecord = true;
				pCHAR->PKIncreaseScore( m_dwCharID );
				PKIncreaseDeath( pCHAR->m_dwCharID );

				pCHAR->UpdatePKRankingsScore( pCHAR->m_dwCharID );
				UpdatePKRankingsDeath( m_dwCharID );

				pCHAR->UpdateMMRIncrease( m_dwCharID );
				UpdateMMRDecrease( pCHAR->m_dwCharID );
			}
		}

		// Å¬·´¹èÆ²½Ã Á×¾úÀ» °æ¿ì Ã³¸®
		if ( ( bClubBattle || bAllianceBattle ) && m_pLandMan->IsClubBattleZone() )
		{
			ClubBattleSetPoint( m_dwGuild, pCHAR->m_dwGuild, bAllianceBattle );

			DWORD dwClubID_P, dwClubID_S;
			if ( bAllianceBattle ) 
			{
				dwClubID_P = pMyClub->m_dwAlliance;
				dwClubID_S = pTarClub->m_dwAlliance;
			}
			else
			{
				dwClubID_P = m_dwGuild;
				dwClubID_S = pCHAR->m_dwGuild;
			}

			GLMSG::SNET_CLUB_BATTLE_POINT_UPDATE NetMsg;

			NetMsg.dwBattleClubID = dwClubID_S;
			NetMsg.bKillPoint = false;
			
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsg );

			NetMsg.dwBattleClubID = dwClubID_P;
			NetMsg.bKillPoint = true;
												
			m_pGLGaeaServer->SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsg );
		
		}
		//	CDM »óÈ²
		else if ( bClubDeathMatch && m_pGLGaeaServer->IsClubDMStarted() )
		{
			GLClubDeathMatch* pCDM = GLClubDeathMatchFieldMan::GetInstance().Find( m_pLandMan->m_dwClubMapID );	
			if ( pCDM )
			{
				pCDM->AddCDMScore( pCHAR->m_dwGuild, m_dwGuild );

				//	POINT UPDATE ³ª¶û, »ó´ë¹æ
				GLMSG::SNET_CLUB_DEATHMATCH_POINT_UPDATE NetMsg;
				NetMsg.bKillPoint = false;					
				m_pGLGaeaServer->SENDTOCLUBCLIENT_ONMAP ( m_pLandMan->GetMapID().dwID, m_dwGuild, &NetMsg );

				NetMsg.bKillPoint = true;														
				m_pGLGaeaServer->SENDTOCLUBCLIENT_ONMAP ( pCHAR->m_pLandMan->GetMapID().dwID, pCHAR->m_dwGuild, &NetMsg );

			}

		}
		/*pvp tyranny, Juver, 2017/08/24 */
		else if ( bPVPTyranny )
		{
			GLPVPTyrannyField::GetInstance().ScoreKill( m_sMapID, m_dwCharID, pCHAR->m_dwCharID );
			/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
			GLPVPTyrannyField::GetInstance().AddTyrannyScore( pCHAR->m_dwCharID, m_dwCharID );
			GLPVPTyrannyField::GetInstance().AddTyrannyResuScore( pCHAR->m_dwCharID, m_dwCharID );
			GLPVPTyrannyField::GetInstance().AddTyrannyClubScore( pCHAR->m_dwGuild, m_dwGuild );

			/*pCHAR->PKIncreaseScore( m_dwCharID );
			PKIncreaseDeath( pCHAR->m_dwCharID );*/
		}
		/*school wars, Juver, 2018/01/19 */
		else if ( bPVPSchoolWars )
		{
			GLPVPSchoolWarsField::GetInstance().ScoreKill( m_dwCharID, pCHAR->m_dwCharID );
			GLPVPSchoolWarsField::GetInstance().AddBattleRanking( pCHAR->m_dwCharID, m_dwCharID );

			//pCHAR->PKIncreaseScore( m_dwCharID );
			//PKIncreaseDeath( pCHAR->m_dwCharID );
		}
		/*pvp capture the flag, Juver, 2018/02/01 */
		else if ( bPVPCaptureTheFlag )
		{
			GLPVPCaptureTheFlagField::GetInstance().ScoreKill( m_dwCharID, pCHAR->m_dwCharID );
			/*pCHAR->PKIncreaseScore( m_dwCharID );
			PKIncreaseDeath( pCHAR->m_dwCharID );*/
		}
		/* pvp club death match, Juver, 2020/11/25 */
		else if ( bPVPClubDeathMatch )
		{
			PVPClubDeathMatchField::GetInstance().ScoreKill( m_dwCharID, pCHAR->m_dwCharID );
			/*pCHAR->PKIncreaseScore( m_dwCharID );
			PKIncreaseDeath( pCHAR->m_dwCharID );*/
		}
		else if ( bPVPPBG )
		{
			PVPPBG::ManagerField::GetInstance().ScoreKill( m_dwCharID, pCHAR->m_dwCharID );
			/*pCHAR->PKIncreaseScore( m_dwCharID );
			PKIncreaseDeath( pCHAR->m_dwCharID );*/
		}
		else if( m_pGLGaeaServer->IsBRIGHTEVENT( m_sMapID ) )
		{
			m_pGLGaeaServer->m_cFreePK.AddBattleRanking( pCHAR->m_dwCharID, m_dwCharID );
			/*pCHAR->PKIncreaseScore( m_dwCharID );
			PKIncreaseDeath( pCHAR->m_dwCharID );*/
		}
		//	Note : ¼­·Î Àû´ë ÇàÀ§ÀÚÀÏ °æ¿ì.
		else if ( !IS_HOSTILE_ACTOR(pCHAR->m_dwCharID) && pCHAR->IS_HOSTILE_ACTOR(m_dwCharID) )
		{
			//	Note : ¹üÁËÀÚ µî±ÞÀÎ °æ¿ì »ó´ë¹æ¿¡°Ô »ýÈ°Á¡¼ö ÁÜ.
			//

			if ( ISOFFENDER() )
			{
				DWORD dwMY_PK_LEVEL = GET_PK_LEVEL();
				if ( dwMY_PK_LEVEL != UINT_MAX && dwMY_PK_LEVEL>0 )
				{
					//	Note : »ýÈ°Á¡¼ö º¯È­.
					nLIFE = 7-(GLCONST_CHAR::EMPK_STATE_LEVEL-dwMY_PK_LEVEL);
					pCHAR->ReceiveLivingPoint ( nLIFE );
				}
			}
			//	ÀÚ½ÅÀÇ µî±ÞÀÌ ÀÏ¹ÝÇÐ»ýÀÏ °æ¿ì, »ó´ë¹æ ¼Ó¼º, »ýÁ¡ Á¶Àý.
			else
			{
				bool bGuidBattleMap = m_pLandMan->m_bGuidBattleMap;
				bool bPVPTyranny = m_pLandMan->m_bPVPTyrannyMap;
				bool bPVPSchoolWars = m_pLandMan->m_bPVPSchoolWarsMap;
				bool bFreePKMap = m_pLandMan->IsFreePK();
				bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
				bool bBRIGHTEVENT = m_pGLGaeaServer->IsBRIGHTEVENT( m_sMapID ); // ¼ºÇâ ÀÌº¥Æ®¿ë		/*dmk14 freepk*/		
				
				/* open pk function, Juver, 2019/12/24 */
				bool bOpenPK = false;
				if ( m_pLandMan && m_pLandMan->IsOpenPK() )
				{
					bOpenPK = true;
				}

				if ( GetSchool() == pCHAR->GetSchool() )	bSCHOOL_FREEPK = false;

				/* open pk function, Juver, 2019/12/24 */
				if ( !(bGuidBattleMap||bSCHOOL_FREEPK||bFreePKMap) && !( bBRIGHTEVENT || bOpenPK ) )
				{
					nBRIGHT = GLCONST_CHAR::nPK_KILL_BRIGHT_POINT;
					nLIFE = GLCONST_CHAR::nPK_KILL_LIVING_POINT;

					//	Note : ¼Ó¼º¼öÄ¡ º¯È­.
					pCHAR->m_nBright += nBRIGHT;

					GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
					NetMsg.nBright = pCHAR->m_nBright;
					m_pGLGaeaServer->SENDTOCLIENT(pCHAR->m_dwClientID,&NetMsg);

					GLMSG::SNETPC_UPDATE_BRIGHT_BRD NetMsgBrd;
					NetMsgBrd.dwGaeaID = pCHAR->m_dwGaeaID;
					NetMsgBrd.nBright = pCHAR->m_nBright;
					pCHAR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

					//	Note : »ýÈ°Á¡¼ö º¯È­.
					pCHAR->m_nLiving += nLIFE;

					GLMSG::SNETPC_UPDATE_LP NetMsgLp;
					NetMsgLp.nLP = pCHAR->m_nLiving;
					m_pGLGaeaServer->SENDTOCLIENT(pCHAR->m_dwClientID,&NetMsgLp);
				}
			}
		}

		//	Note : Á×¿´À» °æ¿ì ·Î±× ±â·Ï.
		GLITEMLMT::GetInstance().ReqAction
		(
			dwKILL_ID,					//	´ç»çÀÚ.
			EMLOGACT_KILL,				//	ÇàÀ§.
			ID_CHAR, m_dwCharID,		//	»ó´ë¹æ.
			0,							//	exp
			nBRIGHT,					//	bright
			nLIFE,						//	life
			0							//	money
		);

		/*activity system, Juver, 2017/11/01 */
		if ( m_pLandMan )
		{
			pCHAR->DoActivityMapKill( m_pLandMan->GetMapID() );
			/*12-9-14, Battle Pass - CNDev*/
			pCHAR->DoBattlePassMapKill( m_pLandMan->GetMapID() );
		}
		
		/*pk info, Juver, 2017/11/17 */
	//	pCHAR->PKIncreaseScore( m_dwCharID );
	//	PKIncreaseDeath( pCHAR->m_dwCharID );
		
		/*Top MMR, Mhundz */
		//pCHAR->UpdateMMRIncrease( m_dwCharID );
		//UpdateMMRDecrease( pCHAR->m_dwCharID );
		
		//pCHAR->UpdatePKRankingsScore( pCHAR->m_dwCharID );
		//UpdatePKRankingsDeath( m_dwCharID );
		
		BOOL bClubWars = FALSE;

		if ( m_pLandMan && m_pLandMan->m_bGuidBattleMap )
		{
			GLGuidanceFieldMan::GetInstance().ScoreKill( m_pLandMan->m_dwClubMapID, m_dwCharID, pCHAR->m_dwCharID );
			bClubWars = TRUE;
			
			/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
			GLGuidance* pGuidance = GLGuidanceFieldMan::GetInstance().Find(m_pLandMan->m_dwClubMapID);
			pGuidance->AddClubWarScore( pCHAR->m_dwCharID, m_dwCharID );
			//pGuidance->AddClubWarResuScore( pCHAR->m_dwCharID, m_dwCharID );
			pGuidance->AddClubWarClubScore( pCHAR->m_dwGuild, m_dwGuild );

			//pCHAR->PKIncreaseScore( m_dwCharID );
			//PKIncreaseDeath( pCHAR->m_dwCharID );
		}

		if ( pCHAR && m_pLandMan && m_pLandMan->IsShowPkMark() )
		{
			GLClubMan& cClubMan = m_pGLGaeaServer->GetClubMan();

			GLMSG::SNET_F2C_PKMARK_BRD NetMsg;
			DWORD dwClubIDA = CLUB_NULL;
			DWORD dwClubMarVerA = 0;
			if( pCHAR->m_dwGuild != CLUB_NULL )
			{
				GLCLUB *pCLUB = cClubMan.GetClub ( pCHAR->m_dwGuild );
				if( pCLUB )
				{
					dwClubIDA = pCHAR->m_dwGuild;
					dwClubMarVerA = pCLUB->m_dwMarkVER;
				}
			}
			NetMsg.sData.SetDataA( pCHAR->m_dwCharID, pCHAR->m_szName, pCHAR->m_wSchool, CharClassToIndex( pCHAR->m_emClass ), dwClubIDA, dwClubMarVerA );

			DWORD dwClubIDB = CLUB_NULL;
			DWORD dwClubMarVerB = 0;
			if( m_dwGuild != CLUB_NULL )
			{
				GLCLUB *pCLUB = cClubMan.GetClub ( m_dwGuild );
				if( pCLUB )
				{
					dwClubIDB = m_dwGuild;
					dwClubMarVerB = pCLUB->m_dwMarkVER;
				}
			}
			NetMsg.sData.SetDataB( m_dwCharID, m_szName, m_wSchool, CharClassToIndex( m_emClass ), dwClubIDB, dwClubMarVerB );

			EMSLOT emRHand = pCHAR->GetCurRHand();
			const SITEMCUSTOM &sRHAND = GET_SLOT_ITEM(emRHand);
		
			SITEM* pItem = GLItemMan::GetInstance().GetItem( sRHAND.sNativeID );
			if ( pItem )
			{
				NetMsg.sData.SetWeapon(pItem->sSuitOp.emAttack);
			}
			
			m_pGLGaeaServer->SENDNOTIFICATION_ONMAP(m_sMapID.dwID, &NetMsg);
		}

		BOOL bPKMapCount = FALSE;

		if ( m_pLandMan && m_pLandMan->isPKMapCount() )
		{
			bPKMapCount = TRUE;
		}

		pCHAR->procPKKill( 
			m_dwCharID, 
			bPVPTyrannyCombatRecord, 
			bClubWars, 
			bPVPCaptureTheFlagCombatRecord, 
			(bClubDeathMatch||bPVPClubDeathMatchCombatRecord),
			bPVPSchoolWarsCombatRecord, 
			bPKMapCount );

		procPKDeath( 
			pCHAR->m_dwCharID, 
			bPVPTyrannyCombatRecord, 
			bClubWars, 
			bPVPCaptureTheFlagCombatRecord, 
			(bClubDeathMatch||bPVPClubDeathMatchCombatRecord),
			bPVPSchoolWarsCombatRecord, 
			bPKMapCount );


	}

	//	Note : Àû´ë ÇàÀ§ÀÚ »èÁ¦.
	DelPlayHostile();

	//	Note : ¾²·¯Áú¶§ º¸»ó ¹ß»ý. ( °æÇèÄ¡ + ¾ÆÀÌÅÛ + ±Ý¾× )
	GenerateReward ();

	// ½Ã°£Á¦ ±ÍÈ¥ÁÖ Âø¿ë½Ã ¹«Á¶²« °æÇèÄ¡¸¦ 0À¸·Î ¸¸µç´Ù.
	if ( m_nDECEXP > 0 )
	{
		bool bItemRebirth = ISREVIVE() ? true : false;
		if ( bItemRebirth )
		{
			// ¼Ò¸ð¼º ±ÍÈ¥ÁÖÀÌ¸é ÀÚµ¿ ±ÍÈ¥ÁÖ »ç¿ë
			/*ep8 puton slots, Juver, 2018/03/30 */
			SITEM* pITEM = GET_SLOT_ITEMDATA(SLOT_LOGIC_NECKLACE);
			if ( !pITEM->ISINSTANCE() ) m_nDECEXP = 0;		
		}
	}


	m_cDamageLog.clear();

	/*dmk14 clear skill delay when died*/
	if( m_pLandMan->isResetSkillOnDie() )	m_SKILLDELAY.clear();
	

	////	Note : Á×À»¶§ ·Î±× ±â·Ï.
	// ºÎÈ°ÇÏ±â ¹öÆ°ÀÌ ³ª¿ÀÁö ¾Ê¾Æ ·Î±×¸¦ ¸ø³²±â´Â °æ¿ì°¡ ÀÖ¾î¼­ Á×À¸¸é ¹«Á¶°Ç ³²±ä´Ù.
	{
		EMIDTYPE emKILL = ID_MOB;
		if ( m_sAssault.emCrow==CROW_PC )	emKILL = ID_CHAR;

		GLITEMLMT::GetInstance().ReqAction
		(
			m_dwCharID,					//	´ç»çÀÚ.
			EMLOGACT_DIE,				//	ÇàÀ§.
			emKILL, dwKILL_ID,			//	»ó´ë¹æ.
			m_nDECEXP,					// exp
			0,							// bright
			0,							// life
			0							// money
		);
	}

	//	Note : Äù½ºÆ® ÁøÇà ¸ñ·ÏÁß Á×À»½Ã ½ÇÆÐ ¿É¼Ç °Ë»çÈÄ Ã³¸®.
	DoQuestCheckLimitDie ();

	//	¾²·¯Áü ¼³Á¤.
	SetSTATE(EM_ACT_DIE);

	//	Note : ¾²·¯ÁüÀ» Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¾Ë¸².
	GLMSG::SNET_ACTION_BRD NetMsgBrd;
	NetMsgBrd.emCrow	= CROW_PC;
	NetMsgBrd.dwID		= m_dwGaeaID;
	NetMsgBrd.emAction	= GLAT_FALLING;

	//	Note : ÁÖº¯ÀÇ Char ¿¡°Ô.
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	//	Note : ÀÚ½ÅÀÇ Å¬¶óÀÌ¾ðÆ®¿¡.
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsgBrd );

	//	ÇàÀ§ º¯È­.
	TurnAction ( GLAT_FALLING );

	// ¹èÆ²·Î¾â ÀÌº¥Æ®ÀÏ¶§ ¾²·¯Áö¸é Åõ¸í¸ðµå·Î ÀüÈ¯ÇÑ´Ù.
	if ( GLCONST_CHAR::bBATTLEROYAL )
	{
		SetSTATE(EM_REQ_VISIBLEOFF);

		//	Note : (¿¡ÀÌÁ¯Æ®¼­¹ö) ¸Þ¼¼Áö ¹ß»ý.
		//
		GLMSG::SNETPC_ACTSTATE NetMsgFld;
		NetMsgFld.dwActState = m_dwActState;
		m_pGLGaeaServer->SENDTOAGENT ( m_dwClientID, &NetMsgFld );
	}

	// ÆÖ Sad ¾×¼Ç
	PGLPETFIELD pMyPet = m_pGLGaeaServer->GetPET ( m_dwPetGUID );
	if ( pMyPet && pMyPet->IsValid () )
	{
		pMyPet->ReSetAllSTATE ();
		pMyPet->SetSTATE ( EM_PETACT_SAD );
		
		// Å¬¶óÀÌ¾ðÆ®¿¡ ¾Ë¸²
		GLMSG::SNETPET_SAD PetNetMsg;
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &PetNetMsg );

		GLMSG::SNETPET_SAD_BRD PetNetMsgBrd;
		PetNetMsgBrd.dwGUID   = pMyPet->m_dwGUID;
		PetNetMsgBrd.dwOwner  = m_dwGaeaID;

		SendMsgViewAround ( (NET_MSG_GENERIC*) &PetNetMsgBrd );
	}

	// Á×¾úÀ» °æ¿ì ¼ÒÈ¯¼ö¸¦ ÇØÁ¦

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		PGLSUMMONFIELD pMySummon = m_pGLGaeaServer->GetSummon( m_dwSummonGUID_FLD[i] );
		if ( pMySummon && pMySummon->IsValid() )
		{
			m_pGLGaeaServer->DropOutSummon ( m_dwSummonGUID_FLD[i] );
		}
	}	
}

HRESULT GLChar::TourchQItem ( DWORD dwGlobID )
{
	PITEMDROP pItemDrop = m_pLandMan->GetItem ( dwGlobID );
	if ( !pItemDrop )	return S_FALSE;

	/*qbox check, Juver, 2017/12/05 */
	if ( !pItemDrop->bMobGen && !m_pLandMan->IsQBoxEnabled() )
	{
		CDebugSet::ToFileWithTime( "_qbox_check.txt", "[%d]%s attempt to loot qbox in non qbox map", m_dwCharID, m_szName );
		return S_FALSE;
	}

//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;
	GLItemMan & glItemMan = GLItemMan::GetInstance();
	
	const SITEM *pItemData = glItemMan.GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItemData )	return S_FALSE;

	if ( !pItemDrop->IsTakeItem(m_dwPartyID,m_dwGaeaID) )	return S_FALSE;

	if ( pItemData->sBasicOp.emItemType!=ITEM_QITEM )		return S_FALSE;

	const ITEM::SQUESTIONITEM &sQUESTIONITEM = pItemData->sQuestionItem;

	//	Note : ?¾ÆÀÌÅÛÀÇ ¿µÇâ. ÆÄÆ¼ÀÏ °æ¿ì ¿µÇâÀÌ ÆÄÆ¼¿ø¿¡°Ô ¹ÌÄ£´Ù.
	//
	switch ( sQUESTIONITEM.emType )
	{
	case QUESTION_SPEED_UP:
	case QUESTION_CRAZY:
	case QUESTION_ATTACK_UP:
	case QUESTION_EXP_UP:
	case QUESTION_LUCKY:
	case QUESTION_SPEED_UP_M:
	case QUESTION_MADNESS:
	case QUESTION_ATTACK_UP_M:
		{
			SNATIVEID sNativeID;

			// ¼­ºñ½º ÇÁ·Î¹ÙÀÌ´õ°¡ ÇØ¿ÜÀÎ °æ¿ì
			if ( m_pGLGaeaServer->m_nServiceProvider != SP_KOREA )
			{
                if ( sQUESTIONITEM.emType == QUESTION_SPEED_UP_M ||
				     sQUESTIONITEM.emType == QUESTION_MADNESS    ||
				     sQUESTIONITEM.emType == QUESTION_ATTACK_UP_M )
				{
                    sNativeID = pItemData->sBasicOp.sNativeID;
				}
				else
				{
                    sNativeID = glItemMan.GetRandomQItemID ( pItemDrop->sItemCustom.sNativeID );
				}
			}
			else
			{
				sNativeID = glItemMan.GetRandomQItemID ( pItemDrop->sItemCustom.sNativeID );
			}
				
			GLPARTY_FIELD* pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);

			// »õ·Ó°Ô Ãß°¡ÇÑ ?¾ÆÀÌÅÛÀÏ °æ¿ì Á¾Àü°ú °°Àº ¹æ¹ýÀ¸·Î Ã³¸®
			if ( sNativeID == NATIVEID_NULL() )
			{
				if ( pParty )
				{
					GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
					GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
					for ( ; pos!=end; ++pos )
					{
						PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
						if ( !pCHAR )	continue;
						if ( pCHAR->m_sMapID!=m_sMapID )	continue;

						D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
						float fDist = D3DXVec3Length ( &vDist );
						if ( MAX_VIEWRANGE < fDist )		continue;

						/*qbox check, Juver, 2017/12/05 */
						pCHAR->RecieveQItemFact ( pItemDrop->sItemCustom.sNativeID, pItemDrop->bMobGen );
					}
				}
				else
				{
					/*qbox check, Juver, 2017/12/05 */
					RecieveQItemFact ( pItemDrop->sItemCustom.sNativeID, pItemDrop->bMobGen );
				}	
			}
			else
			{
				const SITEM *pItem = glItemMan.GetItem( sNativeID );
				if ( !pItem ) return S_FALSE;

				if ( pParty )
				{
					GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
					GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
					for ( ; pos!=end; ++pos )
					{
						PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
						if ( !pCHAR )	continue;
						if ( pCHAR->m_sMapID!=m_sMapID )	continue;

						D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
						float fDist = D3DXVec3Length ( &vDist );
						if ( MAX_VIEWRANGE < fDist )		continue;

						/*qbox check, Juver, 2017/12/05 */
						pCHAR->RecieveQItemFact ( sNativeID , pItemDrop->bMobGen);
					}
				}
				else
				{
					/*qbox check, Juver, 2017/12/05 */
					RecieveQItemFact ( sNativeID, pItemDrop->bMobGen );
				}
			}

			/*activity system, Juver, 2017/11/04 */
			if ( m_sQITEMFACT.emType != QUESTION_NONE && m_sQITEMFACT.emType != QUESTION_SIZE )
			{
				DoActivityObtainQBox( m_sQITEMFACT.emType );
			}
		}
		break;

	case QUESTION_EXP_GET:
		{
			SQITEM_FACT sFACT;
			sFACT.emType = sQUESTIONITEM.emType;

			GLMSG::SNETPC_QITEMFACT_BRD NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;
			NetMsgBrd.sFACT = sFACT;

			GLPARTY_FIELD* pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);
			if ( pParty )
			{
				GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
				GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
				for ( ; pos!=end; ++pos )
				{
					PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
					if ( !pCHAR )						continue;
					if ( pCHAR->m_sMapID!=m_sMapID )	continue;

					D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
					float fDist = D3DXVec3Length ( &vDist );
					if ( MAX_VIEWRANGE < fDist )		continue;

					int nRecExp = int ( pCHAR->m_sExperience.lnMax * sQUESTIONITEM.fExp/100.0f );
					pCHAR->ReceiveExp ( nRecExp, false );
					m_pGLGaeaServer->SENDTOCLIENT(pCHAR->m_dwClientID,&NetMsgBrd);
				}
			}
			else
			{
				int nRecExp = int ( m_sExperience.lnMax * sQUESTIONITEM.fExp/100.0f );
				ReceiveExp ( nRecExp, false );
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgBrd);
			}

			/*activity system, Juver, 2017/11/04 */
			DoActivityObtainQBox( sQUESTIONITEM.emType );
		}
		break;

	case QUESTION_BOMB:
		{
			SQITEM_FACT sFACT;
			sFACT.emType = sQUESTIONITEM.emType;

			GLMSG::SNETPC_QITEMFACT_BRD NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;
			NetMsgBrd.sFACT = sFACT;

			GLPARTY_FIELD* pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);
			if ( pParty )
			{
				GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
				GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
				for ( ; pos!=end; ++pos )
				{
					PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
					if ( !pCHAR )	continue;
					if ( pCHAR->m_sMapID!=m_sMapID )	continue;

					D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
					float fDist = D3DXVec3Length ( &vDist );
					if ( MAX_VIEWRANGE < fDist )		continue;

					pCHAR->ReceiveDamage ( CROW_ITEM, 0, sQUESTIONITEM.wParam1, FALSE );
					m_pGLGaeaServer->SENDTOCLIENT(pCHAR->m_dwClientID,&NetMsgBrd);
				}
			}
			else
			{
				ReceiveDamage ( CROW_ITEM, 0, sQUESTIONITEM.wParam1, FALSE );
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgBrd);
			}
		}
		break;

	case QUESTION_MOBGEN:
		{
			SQITEM_FACT sFACT;
			sFACT.emType = sQUESTIONITEM.emType;

			GLMSG::SNETPC_QITEMFACT_BRD NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;
			NetMsgBrd.sFACT = sFACT;

			GLPARTY_FIELD* pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);
			if ( pParty )
			{
				GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
				GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
				for ( ; pos!=end; ++pos )
				{
					PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
					if ( !pCHAR )	continue;
					if ( pCHAR->m_sMapID!=m_sMapID )	continue;

					D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
					float fDist = D3DXVec3Length ( &vDist );
					if ( MAX_VIEWRANGE < fDist )		continue;

					m_pGLGaeaServer->SENDTOCLIENT(pCHAR->m_dwClientID,&NetMsgBrd);
				}
			}
			else
			{
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgBrd);
			}

			m_pLandMan->DropCrow ( SNATIVEID(sQUESTIONITEM.wParam1,sQUESTIONITEM.wParam2), pItemDrop->vPos.x, pItemDrop->vPos.z );

			/*activity system, Juver, 2017/11/04 */
			DoActivityObtainQBox( sQUESTIONITEM.emType );
		}
		break;

	case QUESTION_HEAL:
		{
			SQITEM_FACT sFACT;
			sFACT.emType = sQUESTIONITEM.emType;

			GLMSG::SNETPC_QITEMFACT_BRD NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;
			NetMsgBrd.sFACT = sFACT;

			GLPARTY_FIELD* pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);
			if ( pParty )
			{
				GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
				GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
				for ( ; pos!=end; ++pos )
				{
					PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
					if ( !pCHAR )	continue;
					if ( pCHAR->m_sMapID!=m_sMapID )	continue;

					D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
					float fDist = D3DXVec3Length ( &vDist );
					if ( MAX_VIEWRANGE < fDist )		continue;

					m_pGLGaeaServer->SENDTOCLIENT(pCHAR->m_dwClientID,&NetMsgBrd);

					pCHAR->VAR_BODY_POINT ( CROW_PC, m_dwGaeaID, FALSE, USHRT_MAX, USHRT_MAX, USHRT_MAX );
				}
			}
			else
			{
				VAR_BODY_POINT ( CROW_PC, m_dwGaeaID, FALSE, USHRT_MAX, USHRT_MAX, USHRT_MAX );
				m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgBrd);
			}

			/*activity system, Juver, 2017/11/04 */
			DoActivityObtainQBox( sQUESTIONITEM.emType );
		}
		break;
	};

	//	Note : ¾ÆÀÌÅÆ Land¿¡¼­ »èÁ¦.
	m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );

	return S_OK;
}

// ¾ÆÀÌÅÛÀÇ °æÇèÄ¡ ¹èÀ² ±¸ÇÏ±â...¾ÆÀÌÅÛ¿¡ ºÙÀº °æÇèÄ¡ ¹èÀ²À» ¸ðµÎ ´õÇÑ´Ù.
float GLChar::GetExpMultipleRateFromSlotItem()
{
	float fExpMultipleRateMax = 1.0f;

	GLItemMan & glItemMan = GLItemMan::GetInstance();

	for( int i=0; i<SLOT_TSIZE; ++i )
	{
		const SITEMCUSTOM &sItemCustoms = m_PutOnItems[i];

		if( sItemCustoms.sNativeID == SNATIVEID( false ) )
			continue;

		SITEM* pItem = glItemMan.GetItem( sItemCustoms.sNativeID );
		if( !pItem )
			continue;

		float fExpMultipleRate = pItem->GetExpMultiple();
		if(  1.0f < fExpMultipleRate )
			fExpMultipleRateMax += fExpMultipleRate - 1.0f;
	}

	return fExpMultipleRateMax < 1.0f ? 1.0f : fExpMultipleRateMax;
}

void GLChar::SENDBOXITEMINFO ()
{
	GLMSG::SNET_GET_CHARGEDITEM_FROMDB_FB NetMsgFB;

	NetMsgFB.emFB = EMREQ_CHARGEDITEM_FROMDB_FB_OK;

	//	¾ÆÀÌÅÛ ˜Þ¿¡¼­ ±¸ÀÔÇÑ ¹°Ç° ¸ñ·Ï Å¬¶óÀÌ¾ðÆ®¿¡ Àü¼Û

//	GLGaeaServer & glGaeaServer = m_pGLGaeaServer;

	MAPSHOP::reverse_iterator pos = m_mapCharged.rbegin();
	MAPSHOP::reverse_iterator end = m_mapCharged.rend();

	for ( ; pos!=end; ++pos )
	{
		SHOPPURCHASE &sPURCHASE = (*pos).second;

		StringCchCopy ( NetMsgFB.szPurKey, PURKEY_LENGTH+1, sPURCHASE.strPurKey.GetString() );
		NetMsgFB.nidITEM = SNATIVEID(sPURCHASE.wItemMain,sPURCHASE.wItemSub);
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );
	}

	NetMsgFB.emFB = EMREQ_CHARGEDITEM_FROMDB_FB_END;
	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );
	return;
}

HRESULT GLChar::MGOddEvenOK( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg )
{
	GLMSG::SNETPC_MGAME_ODDEVEN_FB NetMsgFB;

	// ¼ÒÀ¯ ±Ý¾× Ã¼Å©
	if( pNetMsg->uiBattingMoney > m_lnMoney )
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_MONEY_FAIL;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

		return S_FALSE;
	}

	// ÃÖ´ë ¹èÆÃ °¡´É ±Ý¾× Ã¼Å©
	if( pNetMsg->uiBattingMoney > MINIGAME_ODDEVEN::uiMaxBattingMoney )
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_MAXBATTING;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

		return S_FALSE;
	}

	MGOddEvenInit();

	m_ui64BattingMoney = pNetMsg->uiBattingMoney;

	// ¹èÆÃ ¿Ï·á °ÔÀÓ ½ÃÀÛ
	NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_OK;
	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MGOddEvenCANCEL( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg )
{
	GLMSG::SNETPC_MGAME_ODDEVEN_FB NetMsgFB;
	NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_FAIL;
	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MGOddEvenSelect( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg )
{
	m_nOddEven = pNetMsg->emCase;

	return S_OK;
}

HRESULT GLChar::MGOddEvenAGAIN( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg )
{
	GLMSG::SNETPC_MGAME_ODDEVEN_FB NetMsgFB;

	if( pNetMsg->uiBattingMoney > m_lnMoney ) // ¼ÒÀ¯ ±Ý¾× Ã¼Å©
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_FAIL;
	}
	else
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_AGAIN_OK;
		NetMsgFB.wRound = m_wMGOddEvenCurRnd+1;

		m_ui64BattingMoney = m_ui64DividendMoney;
	}

	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MGOddEvenShuffle( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg )
{
	std::vector<int> vecShuffleBuffer;
	vecShuffleBuffer.reserve( 100 );

	{ // È°·ü¿¡ µû¸¥ ·£´ý ¹è¿­ »ý¼º
		int nSuccessCnt = (int)(MINIGAME_ODDEVEN::fSuccessRate[m_wMGOddEvenCurRnd]*100.0); // ¼º°ø È®·ü
		int i = 0;

		for( ; i<nSuccessCnt; ++i )
			vecShuffleBuffer.push_back( m_nOddEven );

		for( ; i<100; ++i)
			vecShuffleBuffer.push_back( EMMGAME_ODDEVEN_ODD - m_nOddEven );

		std::random_shuffle( vecShuffleBuffer.begin(), vecShuffleBuffer.end() );
	}

	srand ( (UINT)time( NULL ) );

	int nDiceNumber = rand() % 100;						// 0~99±îÁö ¼ýÀÚ¸¦ ·£´ýÇÏ°Ô ¼±ÅÃÇÑ´Ù.
	int nHabbyNumber = vecShuffleBuffer[nDiceNumber];	// ¹è¿­¿¡¼­ ¼±ÅÃ

	GLITEMLMT::GetInstance().ReqMoneyExc(	ID_CHAR, m_dwCharID, 
											ID_CHAR, 0, 
											m_ui64BattingMoney, EMITEM_ROUTE_ODDEVEN );

	GLMSG::SNETPC_MGAME_ODDEVEN_FB NetMsgFB;

	if( nHabbyNumber == m_nOddEven ) // ¸Â´Ù¸é ¹è´ç±Ý Áö±Þ
	{
		// ¹è´ç±Ý °è»ê
		m_ui64DividendMoney = (LONGLONG)(m_ui64BattingMoney * MINIGAME_ODDEVEN::fReturnRate[m_wMGOddEvenCurRnd]);

		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_WIN;
		NetMsgFB.ui64DividendMoney = m_ui64DividendMoney;
		NetMsgFB.ui64ActualMoney = m_ui64DividendMoney - m_ui64BattingMoney; // ½Ç¼ö·É ±Ý¾× = ¹è´ç±Ý - ¹èÆÃ±Ý
		
		CheckMoneyUpdate( m_lnMoney, NetMsgFB.ui64ActualMoney, TRUE, "Odd Event Shuffle." );
		m_bMoneyUpdate = TRUE;

		m_lnMoney += NetMsgFB.ui64ActualMoney;		
		m_wMGOddEvenCurRnd++;

		GLITEMLMT::GetInstance().ReqMoneyExc(	ID_CHAR, m_dwCharID, 
												ID_CHAR, m_wMGOddEvenCurRnd, 
												NetMsgFB.ui64ActualMoney, EMITEM_ROUTE_ODDEVEN );
	}
	else
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_LOSE;
		NetMsgFB.ui64DividendMoney = m_ui64BattingMoney;

		CheckMoneyUpdate( m_lnMoney, NetMsgFB.ui64ActualMoney, FALSE, "Odd Event Shuffle." );
		m_bMoneyUpdate = TRUE;

		m_lnMoney -= m_ui64BattingMoney;
		

		GLITEMLMT::GetInstance().ReqMoneyExc(	ID_CHAR, m_dwCharID, 
												ID_MOB, m_wMGOddEvenCurRnd, 
												m_ui64BattingMoney, EMITEM_ROUTE_ODDEVEN );
	}

	GLITEMLMT::GetInstance().ReqMoneyExc(	ID_CHAR, m_dwCharID, 
											ID_CHAR, 0, 
											m_lnMoney, EMITEM_ROUTE_ODDEVEN );

	if( nHabbyNumber )	NetMsgFB.emCase = EMMGAME_ODDEVEN_ODD;
	else				NetMsgFB.emCase = EMMGAME_ODDEVEN_EVEN;

	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MGOddEvenFinish( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg )
{
	GLMSG::SNETPC_MGAME_ODDEVEN_FB NetMsgFB;

	// ¹Ýº¹ È¸¼ö¸¦ Ã¼Å©
	if( m_wMGOddEvenCurRnd >= MINIGAME_ODDEVEN::MAX_ROUND )
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_MAXROUND;
	}
	else
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_FINISH_OK;
		NetMsgFB.wRound = m_wMGOddEvenCurRnd;
		NetMsgFB.ui64DividendMoney = m_ui64DividendMoney;
	}

	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::UpdateVehicle( float fElapsedTime )
{
	if ( !m_bVehicle || !m_sVehicle.IsActiveValue() ) return S_OK;
	m_fVehicleTimer += fElapsedTime;
	// ÆÖÀÇ Æ÷¸¸µµ °»½Å
	if ( m_fVehicleTimer > 3600.0f/GLPeriod::REALTIME_TO_VBR )
	{
		m_fVehicleTimer = 0;
		m_sVehicle.m_nFull -= GLCONST_VEHICLE::nFullDecrVehicle[m_sVehicle.m_emTYPE];
		if ( m_sVehicle.m_nFull < 0 ) m_sVehicle.m_nFull = 0;

		// Å¬¶óÀÌ¾ðÆ® Æ÷¸¸µµ °»½Å
		GLMSG::SNET_VEHICLE_UPDATE_CLIENT_BATTERY NetMsg;
		NetMsg.nFull = m_sVehicle.m_nFull;

		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsg );

		if ( m_sVehicle.IsNotEnoughFull() )	ActiveVehicle( false, false );
	}
	
	return S_OK;
}

BOOL GLChar::CheckMoneyUpdate( const LONGLONG lnCurrentMoney, const LONGLONG lnUpdateMoney, const bool bPlus, const char* msg )
{
	if( lnUpdateMoney < 100000000 && lnUpdateMoney >= 0 )
	{
		return FALSE;
	}

	char szTempChar[256] = {0,};
	std::string str;
	sprintf( szTempChar, "[%s][%s] %s", m_szUID, m_szName, msg );
	str = szTempChar;
	
	if( lnUpdateMoney < 0 )
	{
		str += " Not Correct Update Money Value!!";
	}

	if( bPlus )	sprintf( szTempChar, " Current Money %I64d Update Money %I64d Plus!!", lnCurrentMoney, lnUpdateMoney );
	else		sprintf( szTempChar, " Current Money %I64d Update Money %I64d Minus!!", lnCurrentMoney, lnUpdateMoney );
	str += szTempChar;

	BILLIONUPDATE_WRITE( str.c_str() );

	return TRUE;
}

void GLChar::ClubBattleSetPoint( DWORD dwClub_P, DWORD dwClub_S, bool bAlliancBattle )
{

	GLClubMan& cClubMan = GLGaeaServer::GetInstance().GetClubMan();
	GLCLUB* pClub_P = cClubMan.GetClub( dwClub_P );
	GLCLUB* pClub_S = cClubMan.GetClub( dwClub_S );

	if ( !pClub_P || !pClub_S ) return;

	if ( bAlliancBattle ) 
	{
		GLCLUB* pClub_M = cClubMan.GetClub( pClub_P->m_dwAlliance );
		if ( !pClub_M ) return;

		CLUB_ALLIANCE_ITER pos = pClub_M->m_setAlliance.begin();
		CLUB_ALLIANCE_ITER end = pClub_M->m_setAlliance.end();
		for ( ; pos!=end; ++pos )
		{
			const GLCLUBALLIANCE &sALLIANCE = *pos;

			GLCLUB *pCLUB = cClubMan.GetClub ( sALLIANCE.m_dwID );
			if ( !pCLUB ) continue;
			
			GLCLUBBATTLE* pClubBattle = pCLUB->GetClubBattle( pClub_S->m_dwAlliance );
			if ( !pClubBattle ) continue;

			pClubBattle->m_wDeathPointTemp++;
			pClubBattle->m_bKillUpdate = true;
		}

		pClub_M = cClubMan.GetClub( pClub_S->m_dwAlliance );
		if ( !pClub_M ) return;

		pos = pClub_M->m_setAlliance.begin();
		end = pClub_M->m_setAlliance.end();
		for ( ; pos!=end; ++pos )
		{
			const GLCLUBALLIANCE &sALLIANCE = *pos;

			GLCLUB *pCLUB = cClubMan.GetClub ( sALLIANCE.m_dwID );
			if ( !pCLUB ) continue;
			
			GLCLUBBATTLE* pClubBattle = pCLUB->GetClubBattle( pClub_P->m_dwAlliance );
			if ( !pClubBattle ) continue;

			pClubBattle->m_wKillPointTemp++;
			pClubBattle->m_bKillUpdate = true;
		}
	}
	else
	{
		GLCLUBBATTLE* pClubBattle = pClub_P->GetClubBattle( dwClub_S );
		if ( !pClubBattle ) return;

		pClubBattle->m_wDeathPointTemp++;
		pClubBattle->m_bKillUpdate = true;
		
		pClubBattle = pClub_S->GetClubBattle( dwClub_P );
		if ( !pClubBattle ) return;
				
		pClubBattle->m_wKillPointTemp++;
		pClubBattle->m_bKillUpdate = true;
	}
	
	return;
}

ITEM_COOLTIME*	GLChar::GetCoolTime( DWORD dwCoolID, EMCOOL_TYPE emCoolType )
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

bool GLChar::IsCoolTime( SNATIVEID sNativeID )
{
	__time64_t tCurTime = CTime::GetCurrentTime().GetTime();
	__time64_t tCoolTime = GetMaxCoolTime( sNativeID );

	if ( tCurTime < tCoolTime ) return true;

	return false;
}

bool GLChar::CheckCoolTime( SNATIVEID sNativeID )
{
	if ( IsCoolTime ( sNativeID ) )
	{
		GLMSG::SNET_ITEM_COOLTIME_ERROR NetMsg;
		NetMsg.sNativeID = sNativeID;

		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);
		return true;

	}
	return false;
}

__time64_t	GLChar::GetMaxCoolTime ( SNATIVEID sNativeID )
{
	//	¾ÆÀÌÅÛ Á¤º¸ °¡Á®¿È.
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


bool GLChar::SetCoolTime( SNATIVEID sNativeID , EMCOOL_TYPE emCoolType )
{
	//	¾ÆÀÌÅÛ Á¤º¸ °¡Á®¿È.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( !pItem )			return false;

	if ( !pItem->sBasicOp.IsCoolTime() ) return false;

	// ÄðÅ¸ÀÓ ¼ÂÆÃ
	__time64_t  tCurTime = CTime::GetCurrentTime().GetTime();
	__time64_t  tCoolTime = tCurTime + pItem->sBasicOp.dwCoolTime;

	ITEM_COOLTIME* pCoolTime = GetCoolTime( sNativeID.dwID, emCoolType );
	if ( pCoolTime && tCoolTime < pCoolTime->tCoolTime ) return false;

	ITEM_COOLTIME sCoolTime;
	sCoolTime.dwID = pItem->sBasicOp.sNativeID.dwID;
	sCoolTime.tUseTime = tCurTime;
	sCoolTime.tCoolTime = tCoolTime;	

	if ( emCoolType == EMCOOL_ITEMID )
	{
		sCoolTime.dwCoolID = pItem->sBasicOp.sNativeID.dwID;
		m_mapCoolTimeID[sCoolTime.dwCoolID] = sCoolTime;
	}
	else if( emCoolType == EMCOOL_ITEMTYPE )
	{
		sCoolTime.dwCoolID = (DWORD)pItem->sBasicOp.emItemType;
		m_mapCoolTimeType[sCoolTime.dwCoolID] = sCoolTime;
	}

	GLMSG::SNETPC_ITEM_COOLTIME_UPDATE NetMsg;			
	NetMsg.dwID = sCoolTime.dwID;
	NetMsg.dwCoolID = sCoolTime.dwCoolID;
	NetMsg.tCoolTime = sCoolTime.tCoolTime;
	NetMsg.tUseTime = sCoolTime.tUseTime;
	NetMsg.emCoolType = emCoolType;

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

	return true;
}

bool GLChar::UpdateNpcCommission( LONGLONG lnCommission )
{
	if ( lnCommission < 0 ) return false;
	
	CheckMoneyUpdate( m_lnStorageMoney, lnCommission, TRUE, "Add Npc Commission");
	m_lnStorageMoney += lnCommission;
	m_bStorageMoneyUpdate = TRUE;
			
	GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_USER, m_dwUserID, lnCommission, EMITEM_ROUTE_NPCCOME );

	GLMSG::SNETPC_REQ_STORAGE_UPDATE_MONEY NetMsgStorage;
	NetMsgStorage.lnMoney = m_lnStorageMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID,&NetMsgStorage);

	return true;
}


/*itemfood system, Juver, 2017/05/25 */
WORD GLChar::ItemFoodFindSlot ( SNATIVEID skill_id )
{
	WORD wSELECT = FITEMFACT_SIZE;

	for ( WORD i=0; i<FITEMFACT_SIZE; ++i )
	{
		//if exist just use the slot
		if ( m_sFITEMFACT[i].sNATIVEID==skill_id 
			&& m_sFITEMFACT[i].bENABLE )
			return wSELECT = i;
	}

	for ( WORD i=0; i<FITEMFACT_SIZE; ++i )
	{
		if ( m_sFITEMFACT[i].sNATIVEID==SNATIVEID(false) &&
			m_sFITEMFACT[i].bENABLE )
			return wSELECT = i;
	}

	return wSELECT;
}

WORD GLChar::ItemFoodAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT )
{
	PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( skill_id );
	if ( !pSKILL )	
		return FITEMFACT_SIZE;
	
	if ( wLevel >= SKILL::MAX_SKILL_LEVEL )
		return FITEMFACT_SIZE;

	if ( wSLOT == FITEMFACT_SIZE )	
		return FITEMFACT_SIZE;

	SNATIVEID _sidSKILL = pSKILL->m_sBASIC.sNATIVEID;

	BOOL bHOLD = FALSE;
	SFITEMFACT sSKILLEF;
	sSKILLEF.bENABLE = TRUE;

	SKILL::CDATA_LVL &sSKILL_DATA = pSKILL->m_sAPPLY.sDATA_LVL[wLevel]; 
	switch ( pSKILL->m_sAPPLY.emBASIC_TYPE )
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
		sSKILLEF.emTYPE = pSKILL->m_sAPPLY.emBASIC_TYPE;
		sSKILLEF.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
	{
		if ( pSKILL->m_sAPPLY.sImpacts[nImpact].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			sSKILLEF.sImpacts[nImpact].emADDON = pSKILL->m_sAPPLY.sImpacts[nImpact].emADDON;
			sSKILLEF.sImpacts[nImpact].fADDON_VAR = pSKILL->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[wLevel];
		}
	}

	for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
	{
		switch ( pSKILL->m_sAPPLY.sSpecs[nSpec].emSPEC )
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
			sSKILLEF.sSpecs[nSpec].emSPEC = pSKILL->m_sAPPLY.sSpecs[nSpec].emSPEC;
			sSKILLEF.sSpecs[nSpec].fSPECVAR1 = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR1;
			sSKILLEF.sSpecs[nSpec].fSPECVAR2 = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR2;
			sSKILLEF.sSpecs[nSpec].dwSPECFLAG = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwFLAG;
			sSKILLEF.sSpecs[nSpec].dwNativeID = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwNativeID;
			break;
		};
	}

	if ( !bHOLD )
		return FITEMFACT_SIZE;

	sSKILLEF.sNATIVEID	= pSKILL->m_sBASIC.sNATIVEID;
	sSKILLEF.wLEVEL		= wLevel;
	sSKILLEF.fAGE		= sSKILL_DATA.fLIFE;

	m_sFITEMFACT[wSLOT] = sSKILLEF;

	return wSLOT;
}

//check change of map id
void GLChar::CheckChangeMap()
{
	if( !m_pLandMan )	return;

	if ( m_sidLastMapCheck != m_pLandMan->GetMapID() )
	{
		m_sidLastMapCheck = m_pLandMan->GetMapID();


		/*combatpoint logic, Juver, 2017/05/29 */
		if ( m_pLandMan->IsCPReset() )
		{
			m_sCombatPoint.wNow = 0;
			GLMSG::SNETPC_UPDATE_CP NetMsgUpdateCP;
			NetMsgUpdateCP.wNowCP = m_sCombatPoint.wNow;
			NetMsgUpdateCP.bReset = TRUE;
			m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgUpdateCP );
		}
	}
}

/*combatpoint logic, Juver, 2017/05/29 */
void GLChar::ReceiveCP ( int nGen, bool bSummonReceive/*=false */)
{
	int nGenCP = nGen;
	if ( bSummonReceive ) nGenCP /= 3; 
	if ( nGenCP > 0 )
	{
		m_sCombatPoint.wNow += (WORD)nGenCP;

		/*official random value, Juver, 2018/06/12 */
		m_sCombatPoint.wNow += get_additional_gain_cp();

		m_sCombatPoint.LIMIT();

		GLMSG::SNETPC_UPDATE_CP NetMsgUpdateCP;
		NetMsgUpdateCP.wNowCP = m_sCombatPoint.wNow;
		m_pGLGaeaServer->SENDTOCLIENT ( GETCLIENTID (), &NetMsgUpdateCP );
	}
}

/*vehicle booster system, Juver, 2017/08/10 */
HRESULT GLChar::MsgReqVehicleBooster( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_VEHICLE_BOOSTER* pNetMsg = ( GLMSG::SNETPC_REQ_VEHICLE_BOOSTER* ) nmg;

	if ( !m_bVehicle )					return E_FAIL;
	if ( !m_sVehicle.m_bBooster )		return E_FAIL;
	if ( m_bBoosterStart )				return E_FAIL;
	if ( m_bBoosterCharge )				return E_FAIL;
	
	VehicleBoosterStateStart( true );

	return S_OK;
}

/*system buffs, Juver, 2017/09/05 */
WORD GLChar::SystemBuffFindSlot ( SNATIVEID skill_id )
{
	WORD wSELECT = SYSTEM_BUFF_SIZE;

	for ( WORD i=0; i<SYSTEM_BUFF_SIZE; ++i )
	{
		//if exist just use the slot
		if ( m_sSYSTEM_BUFF[i].sNATIVEID==skill_id )
			return wSELECT = i;
	}

	for ( WORD i=0; i<SYSTEM_BUFF_SIZE; ++i )
	{
		if ( m_sSYSTEM_BUFF[i].sNATIVEID==SNATIVEID(false) )
			return wSELECT = i;
	}

	return wSELECT;
}

/*system buffs, Juver, 2017/09/05 */
void GLChar::SystemBuffAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT )
{
	if ( wSLOT >= SYSTEM_BUFF_SIZE )	return;

	m_sSYSTEM_BUFF[wSLOT].RESET();

	PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( skill_id );
	if ( !pSKILL )						return;

	if ( wLevel >= SKILL::MAX_SKILL_LEVEL )	return;

	SNATIVEID _sidSKILL = pSKILL->m_sBASIC.sNATIVEID;

	BOOL bHOLD = FALSE;
	SSYSTEM_BUFF ssystem_buff;

	SKILL::CDATA_LVL &sSKILL_DATA = pSKILL->m_sAPPLY.sDATA_LVL[wLevel]; 
	switch ( pSKILL->m_sAPPLY.emBASIC_TYPE )
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
		ssystem_buff.emTYPE = pSKILL->m_sAPPLY.emBASIC_TYPE;
		ssystem_buff.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
	{
		if ( pSKILL->m_sAPPLY.sImpacts[nImpact].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			ssystem_buff.sImpacts[nImpact].emADDON = pSKILL->m_sAPPLY.sImpacts[nImpact].emADDON;
			ssystem_buff.sImpacts[nImpact].fADDON_VAR = pSKILL->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[wLevel];
		}
	}

	for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
	{
		switch ( pSKILL->m_sAPPLY.sSpecs[nSpec].emSPEC )
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
			ssystem_buff.sSpecs[nSpec].emSPEC = pSKILL->m_sAPPLY.sSpecs[nSpec].emSPEC;
			ssystem_buff.sSpecs[nSpec].fSPECVAR1 = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR1;
			ssystem_buff.sSpecs[nSpec].fSPECVAR2 = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].fVAR2;
			ssystem_buff.sSpecs[nSpec].dwSPECFLAG = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwFLAG;
			ssystem_buff.sSpecs[nSpec].dwNativeID = pSKILL->m_sAPPLY.sSpecs[nSpec].sSPEC[wLevel].dwNativeID;
			break;
		};
	}

	if ( !bHOLD )	return;

	ssystem_buff.sNATIVEID	= pSKILL->m_sBASIC.sNATIVEID;
	ssystem_buff.wLEVEL		= wLevel;

	m_sSYSTEM_BUFF[wSLOT] = ssystem_buff;

}

/*system buffs, Juver, 2017/09/05 */
void GLChar::SystemBuffReset ( WORD wSLOT )
{
	if ( wSLOT >= SYSTEM_BUFF_SIZE )
	{
		for( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
			m_sSYSTEM_BUFF[i].RESET();

		return;
	}

	m_sSYSTEM_BUFF[wSLOT].RESET();
}

/*pvp tyranny, Juver, 2017/08/24 */
BOOL GLChar::PVPTyrannyRevive( DWORD dwGateID )
{
	if ( !m_pLandMan )	return FALSE;

	DxLandGateMan* pgateman = &m_pLandMan->GetLandGateMan ();
	if ( !pgateman )	return FALSE;

	DxLandGate* pgate = pgateman->FindLandGate ( dwGateID );
	if ( !pgate )		return FALSE;

	BOOL brebirth = GLGaeaServer::GetInstance().RequestReBirth ( m_dwGaeaID, m_pLandMan->GetMapID(), dwGateID, m_vPos );
	if ( !brebirth )	return FALSE;

	ReSetSTATE(EM_ACT_DIE);

	m_fGenAge = 0.0f;
	GLCHARLOGIC_SERVER::INIT_RECOVER(100);
	TurnAction ( GLAT_IDLE );	

	GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,true) );
	//GLGaeaServer::GetInstance().ReserveDropOutSummon ( SDROPOUTSUMMONINFO(m_dwSummonGUID,true) );
	GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, true );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		GLGaeaServer::GetInstance().ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );

	GLMSG::SNETPC_REQ_REBIRTH_FB netmsgagent;
	netmsgagent.sMapID = m_pLandMan->GetMapID();
	netmsgagent.vPos = GetPosition();
	netmsgagent.wNowHP = m_sHP.wNow;
	netmsgagent.wNowMP = m_sMP.wNow;
	netmsgagent.wNowSP = m_sSP.wNow;

	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&netmsgagent);

	GLMSG::SNETPC_REQ_REVIVE_FB netmsgclient;
	netmsgclient.bEventRevive = true;
	netmsgclient.emFB = EMREQ_REVIVE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgclient);

	GLITEMLMT::GetInstance().ReqAction( m_dwCharID,	EMLOGACT_RESTORE, ID_CHAR, 0, m_nDECEXP, 0, 0, 0 );

	return TRUE;
}
/*woe Arc Development 08-06-2024*/
BOOL GLChar::PVPWoeRevive(DWORD dwGateID)
{
	if (!m_pLandMan)	return FALSE;

	DxLandGateMan* pgateman = &m_pLandMan->GetLandGateMan();
	if (!pgateman)	return FALSE;

	DxLandGate* pgate = pgateman->FindLandGate(dwGateID);
	if (!pgate)		return FALSE;

	BOOL brebirth = GLGaeaServer::GetInstance().RequestReBirth(m_dwGaeaID, m_pLandMan->GetMapID(), dwGateID, m_vPos);
	if (!brebirth)	return FALSE;

	ReSetSTATE(EM_ACT_DIE);

	m_fGenAge = 0.0f;
	//GLCHARLOGIC_CLIENT::INIT_RECOVER(50);
	GLCHARLOGIC_SERVER::INIT_RECOVER(50);
	TurnAction(GLAT_IDLE);

	GLGaeaServer::GetInstance().ReserveDropOutPet(SDROPOUTPETINFO(m_dwPetGUID, true, true));
	GLGaeaServer::GetInstance().SaveVehicle(m_dwClientID, m_dwGaeaID, true);

	/*skill summon, Juver, 2017/10/09 */
	for (int i = 0; i < SKILL_SUMMON_MAX_CLIENT_NUM; ++i)
		GLGaeaServer::GetInstance().ReserveDropOutSummon(m_dwSummonGUID_FLD[i]);

	GLMSG::SNETPC_REQ_REBIRTH_FB netmsgagent;
	netmsgagent.sMapID = m_pLandMan->GetMapID();
	netmsgagent.vPos = GetPosition();
	netmsgagent.wNowHP = m_sHP.wNow;
	netmsgagent.wNowMP = m_sMP.wNow;
	netmsgagent.wNowSP = m_sSP.wNow;

	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID, &netmsgagent);

	GLMSG::SNETPC_REQ_REVIVE_FB netmsgclient;
	netmsgclient.bEventRevive = true;
	netmsgclient.emFB = EMREQ_REVIVE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgclient);

	GLITEMLMT::GetInstance().ReqAction(m_dwCharID, EMLOGACT_RESTORE, ID_CHAR, 0, m_nDECEXP, 0, 0, 0);

	return TRUE;
}

/*skill summon, Juver, 2017/10/09 */
WORD GLChar::GetFreeSummon()
{
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_dwSummonGUID_FLD[i] == GAEAID_NULL )
		{
			return i;
			break;
		}
	}

	return SKILL_SUMMON_MAX_CLIENT_NUM;
}

/*skill summon, Juver, 2017/10/09 */
BOOL GLChar::isMySummon( DWORD dwID )
{
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_dwSummonGUID_FLD[i] != GAEAID_NULL && m_dwSummonGUID_FLD[i] == dwID )
		{
			return TRUE;
			break;
		}
	}

	return FALSE;
}

/*skill summon, Juver, 2017/10/11 */
void GLChar::ReTargetSummon( STARID sid_target )
{
	if ( !m_pLandMan )		return;
	if ( !m_pGLGaeaServer )	return;

	STARGETID starget_id;
	starget_id.dwID = sid_target.GETID();
	starget_id.emCrow = sid_target.GETCROW();

	GLACTOR* pactor_field = m_pGLGaeaServer->GetTarget( m_pLandMan, starget_id );
	if ( !pactor_field )	return;

	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		PGLSUMMONFIELD psummon_field = m_pGLGaeaServer->GetSummon( m_dwSummonGUID_FLD[i] );
		if ( !psummon_field )					continue;
		if ( !psummon_field->IsValid() )		continue;
		if ( !psummon_field->IsTargetReady() )	continue;
		
		//if ( psummon_field->GetAttackTar().dwID != EMTARGET_NULL )	continue;
		if ( psummon_field->IsSTATE ( EM_SUMMONACT_ATTACK ) )	continue;
		if ( !psummon_field->GetAttackTarget( starget_id ) )	continue;

		psummon_field->SetAttackTarget( starget_id );
	}
}

/*skill summon, Juver, 2017/10/11 */
void GLChar::summon_remove_all()
{
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		GLGaeaServer::GetInstance().ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );
}

/*pk info, Juver, 2017/11/17 */
void GLChar::procPKKill( 
	DWORD dwCharID, 
	BOOL bTyranny, 
	BOOL bClubWars, 
	BOOL bCaptureTheFlag, 
	BOOL bClubDeathMatch,
	BOOL bSchoolWars,
	BOOL bPKMap )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	
		return;

	//m_dwPKScore ++;

	if ( bTyranny )
		m_sCombatRecord.increaseTyrannyKills();

	if ( bClubWars )
		m_sCombatRecord.increaseClubWarKills();

	if ( bCaptureTheFlag )
		m_sCombatRecord.increaseCaptureTheFlagKills();

	if ( bClubDeathMatch )
		m_sCombatRecord.increaseClubDeathMatchKills();

	if ( bSchoolWars )
		m_sCombatRecord.increaseSchoolWarsKills();

	if ( bPKMap )
		m_sCombatRecord.increasePKMapKills();

	m_sCombatRecord.increasePVPKills();

	/* pk combo, Juver, 2021/02/09 */
	PKComboIncrease();

	m_dwPkCounter++;
	if (m_dwPkCounter >= 4)
	{
		GLMSG::SNETPC_PK_STREAK NetMsg;
		StringCchCopy(NetMsg.szName, CHAR_SZNAME, m_szName);
		NetMsg.dwPkCounter = m_dwPkCounter;
		//GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsg );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMapID.dwID, &NetMsg);
	}

	////planned to remove this pk notification to reduce network traffic but players might question why the feature is removed
	////so it is kept for now, an evaluation if this impacts player experience is needed
	//GLMSG::SNETPC_UPDATE_PK_KILL net_msg_client;
	////net_msg_client.dwVal = m_dwPKScore;
	//StringCchCopy( net_msg_client.szName, CHAR_SZNAME+1, pchar_target->m_szName );
	//GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );
}

/*pk info, Juver, 2017/11/17 */
void GLChar::procPKDeath(
	DWORD dwCharID, 
	BOOL bTyranny, 
	BOOL bClubWars, 
	BOOL bCaptureTheFlag, 
	BOOL bClubDeathMatch,
	BOOL bSchoolWars,
	BOOL bPKMap )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	
		return;

	//m_dwPKDeath ++;

	if ( bTyranny )
		m_sCombatRecord.increaseTyrannyDeaths();

	if ( bClubWars )
		m_sCombatRecord.increaseClubWarDeaths();

	if ( bCaptureTheFlag )
		m_sCombatRecord.increaseCaptureTheFlagDeaths();

	if ( bClubDeathMatch )
		m_sCombatRecord.increaseClubDeathMatchDeaths();

	if ( bSchoolWars )
		m_sCombatRecord.increaseSchoolWarsDeaths();

	if ( bPKMap )
		m_sCombatRecord.increasePKMapDeaths();

	m_sCombatRecord.increasePVPDeaths();

	/* pk combo, Juver, 2021/02/09 */
	PKComboReset();


	if (m_dwPkCounter >= 4)
	{
		GLMSG::SNETPC_PK_STREAK_KILL NetMsg;
		StringCchCopy(NetMsg.szName, CHAR_SZNAME, m_szName);
		StringCchCopy(NetMsg.szNameKiller, CHAR_SZNAME, pchar_target->m_szName);
		NetMsg.dwPkCounter = m_dwPkCounter;
		//GLGaeaServer::GetInstance().SENDTOALLCLIENT( &NetMsg );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMapID.dwID, &NetMsg);
	}
	m_dwPkCounter = 0;

	////planned to remove this pk notification to reduce network traffic but players might question why the feature is removed
	////so it is kept for now, an evaluation if this impacts player experience is needed
	//GLMSG::SNETPC_UPDATE_PK_DEATH net_msg_client;
	////net_msg_client.dwVal = m_dwPKDeath;
	//StringCchCopy( net_msg_client.szName, CHAR_SZNAME+1, pchar_target->m_szName );
	//GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );
}

void GLChar::procResu( 
	DWORD dwCharID, 
	BOOL bTyranny, 
	BOOL bClubWars, 
	BOOL bCaptureTheFlag, 
	BOOL bClubDeathMatch, 
	BOOL bSchoolWars, 
	BOOL bPKMap )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	
		return;

	if ( bTyranny )
		m_sCombatRecord.increaseTyrannyResu();

	if ( bClubWars )
		m_sCombatRecord.increaseClubWarResu();

	if ( bCaptureTheFlag )
		m_sCombatRecord.increaseCaptureTheFlagResu();

	if ( bClubDeathMatch )
		m_sCombatRecord.increaseClubDeathMatchResu();

	if ( bSchoolWars )
		m_sCombatRecord.increaseSchoolWarsResu();

	if ( bPKMap )
		m_sCombatRecord.increasePKMapResu();

	m_sCombatRecord.increasePVPResu();

	m_dwResuCounter++;
	if (m_dwResuCounter >= 4) // kaw na bahala dito
	{
		GLMSG::SNETPC_RESU_STREAK NetMsg;
		StringCchCopy(NetMsg.szName, CHAR_SZNAME, m_szName);
		NetMsg.dwResuCounter = m_dwResuCounter;
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMapID.dwID, &NetMsg);
	}

}

/*block back damage, Juver, 2017/12/12 */
void GLChar::SetBlockDamageBack( bool bSet )
{
	m_bBlockBackDamage = bSet;
	m_fBlockBackDamageTime = 0.0f;
}

/* skill stigma, Juver, 2020/12/12 */
void GLChar::LinkSkillStigmaTarget ( const EMCROW _emACrow, const DWORD _dwAID, const SNATIVEID& _sSkillID, const float& fDistance, const float& fDamage, const DWORD& dwAttackFlag, const SNATIVEID& sMapID )
{
	if ( m_sMapID != sMapID )	return;

	STARGETID sTargetID ( _emACrow, _dwAID );

	GLACTOR *pACTOR = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTargetID );
	if ( !pACTOR )				return;

	SKILL_STIGMA_DATA_MAP_ITER it_1 = m_mapSkillStigma.find( _sSkillID.dwID );
	if ( it_1 != m_mapSkillStigma.end() )
	{
		SKILL_STIGMA_DATA& sData = (*it_1).second;

		TARID_SET_ITER it_2 = sData.setTargetID.find( sTargetID );
		if ( it_2 == sData.setTargetID.end() )
		{
			sData.setTargetID.insert( sTargetID );
		}
	}
	else
	{
		SKILL_STIGMA_DATA sData;
		sData.sSkillID = _sSkillID;
		sData.fDistance = fDistance;
		sData.fDamage = fDamage;
		sData.sMapID = sMapID;
		sData.dwAttackFlag = dwAttackFlag;
		sData.setTargetID.insert( sTargetID );

		if ( sData.isValid() )
		{
			m_mapSkillStigma.insert( std::make_pair( sData.sSkillID.dwID, sData ) );
		}
	}
}

/* skill stigma, Juver, 2020/12/13 */
BOOL GLChar::CheckSkillStigmaExist( const EMCROW _emACrow, const DWORD _dwAID, const SNATIVEID& _sSkillID ) const
{
	BOOL bFOUND = FALSE;

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( m_sSKILLFACT[i]._wCasterCrow != (WORD)_emACrow )		
			continue;

		if ( m_sSKILLFACT[i]._dwCasterID != _dwAID )		
			continue;

		for ( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			if ( m_sSKILLFACT[i].sSpecs[nSpec].emSPEC == EMSPECA_STIGMA )
			{
				bFOUND = TRUE;
				break;
			}
		}

		if ( bFOUND )
			break;
		
	}

	return bFOUND;
}

/* skill buff duration change, Juver, 2020/12/16 */
void GLChar::ReceiveSkillBuffDurationChange( const SNATIVEID& _sSkillID, const DWORD& _dwFlags, const float& fSeconds )
{
	GLMSG::SNETPC_F2C_SKILL_BUFF_DURATION_CHANGE_TIME_MULTIPLE_BRD NetMsgBrd;
	NetMsgBrd.dwID = m_dwGaeaID;
	NetMsgBrd.emCrow = CROW_PC;

	bool bValid = FALSE;

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
		if ( m_sSKILLFACT[i].sNATIVEID == _sSkillID )		continue;

		 PGLSKILL pSkill = GLSkillMan::GetInstance().GetData( m_sSKILLFACT[i].sNATIVEID );
		 if ( pSkill )
		 {
			 bool bApply = false;

			 if ( pSkill->m_sBASIC.emACTION == SKILL::EMACTION_NORMAL && (_dwFlags&SKILL::EMACTION_FLAG_NORMAL) )
				 bApply = true;

			 if ( pSkill->m_sBASIC.emACTION == SKILL::EMACTION_BUFF && (_dwFlags&SKILL::EMACTION_FLAG_BUFF) )
				 bApply = true;

			 if ( pSkill->m_sBASIC.emACTION == SKILL::EMACTION_DEBUFF && (_dwFlags&SKILL::EMACTION_FLAG_DEBUFF) )
				 bApply = true;

			 if ( pSkill->m_sBASIC.emACTION == SKILL::EMACTION_LIMIT && (_dwFlags&SKILL::EMACTION_FLAG_LIMIT) )
				 bApply = true;


			 if ( bApply )
			 {
				 m_sSKILLFACT[i].fAGE += fSeconds;

				 NetMsgBrd.bApply[i] = true;
				 NetMsgBrd.fTime[i] = fSeconds;

				 bValid = true;
			 }
		 }
	}

	if ( bValid )
	{
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgBrd );
	}
}

void GLChar::ReactDamage( GLACTOR* pAttacker, DWORD dwDamageFlags )
{
	/* skill illusion, Juver, 2021/01/17 */
	if( dwDamageFlags & DAMAGE_TYPE_ILLUSION )
	{
		D3DXVECTOR3 vPos = pAttacker->GetPosition();
		if( m_sSkillIllusion.fIllusionJumpBound > 0 )
			vPos += ( pAttacker->GetDirection() * -m_sSkillIllusion.fIllusionJumpBound );

		if ( m_pLandMan )
		{
			STARGETID dwActorID = STARGETID(CROW_PC,m_dwGaeaID);
			m_pLandMan->SkillTeleport( dwActorID, dwActorID, vPos );
		}
	} 
}

/*pk info, Juver, 2017/11/17 */
void GLChar::PKIncreaseScore( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;

	/* PK Streak Sigaw - Jxyy */

	m_dwPKScore ++;
	GLMSG::SNETPC_UPDATE_PK_KILL net_msg_client;
	net_msg_client.dwVal = m_dwPKScore;
	StringCchCopy( net_msg_client.szName, CHAR_SZNAME+1, pchar_target->m_szName );
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );

	

	/* PK Streak Sigaw - Jxyy End */
}

/*pk info, Juver, 2017/11/17 */
void GLChar::PKIncreaseDeath( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;
	/* PK Streak Sigaw - Jxyy */
	
	m_dwPKDeath++;
	GLMSG::SNETPC_UPDATE_PK_DEATH net_msg_client;
	net_msg_client.dwVal = m_dwPKDeath;
	StringCchCopy( net_msg_client.szName, CHAR_SZNAME+1, pchar_target->m_szName );
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );
}
void GLChar::UpdatePKRankingsScore( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;

	/*LG-7 Ranking*/
	{
		STOP_RANK_KILL sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdate(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 1 || pchar_target->m_emClass == 64)
	{
		STOP_RANK_KILL_BR sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateBr(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 2 || pchar_target->m_emClass == 128)
	{
		STOP_RANK_KILL_SW sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateSw(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 4 || pchar_target->m_emClass == 256)
	{
		STOP_RANK_KILL_AR sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateAr(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 8 || pchar_target->m_emClass == 512)
	{
		STOP_RANK_KILL_SH sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateSh(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 16 || pchar_target->m_emClass == 32)
	{
		STOP_RANK_KILL_EX sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateEx(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 1024 || pchar_target->m_emClass == 2048)
	{
		STOP_RANK_KILL_SC sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateSc(sPKWIN, TRUE);
	}

	if ( pchar_target->m_emClass == 4096 || pchar_target->m_emClass == 8192)
	{
		STOP_RANK_KILL_AS sPKWIN;
		sPKWIN.dwChaNum		= pchar_target->m_dwCharID;
		StringCchCopy(sPKWIN.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKWIN.wChaLevel	= pchar_target->m_wLevel;
		sPKWIN.nChaClass	= (int)pchar_target->m_emClass;
		sPKWIN.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKWIN.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKWIN.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKWIN.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKWIN.dwChaPKWin	= pchar_target->m_dwPKScore;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateAs(sPKWIN, TRUE);
	}
}

/*pk info, Juver, 2017/11/17 */
void GLChar::UpdatePKRankingsDeath( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;

	/*LG-7 Ranking*/
	{
		STOP_RANK_KILL sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdate(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 1 || pchar_target->m_emClass == 64)
	{
		STOP_RANK_KILL_BR sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateBr(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 2 || pchar_target->m_emClass == 128)
	{
		STOP_RANK_KILL_SW sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateSw(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 4 || pchar_target->m_emClass == 256)
	{
		STOP_RANK_KILL_AR sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateAr(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 8 || pchar_target->m_emClass == 512)
	{
		STOP_RANK_KILL_SH sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateSh(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 16 || pchar_target->m_emClass == 32)
	{
		STOP_RANK_KILL_EX sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateEx(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 1024 || pchar_target->m_emClass == 2048)
	{
		STOP_RANK_KILL_SC sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateSc(sPKLOSS, FALSE);
	}

	if ( pchar_target->m_emClass == 4096 || pchar_target->m_emClass == 8192)
	{
		STOP_RANK_KILL_AS sPKLOSS;
		sPKLOSS.dwChaNum	= pchar_target->m_dwCharID;
		StringCchCopy(sPKLOSS.szChaName, CHAR_SZNAME, pchar_target->m_szName);
		sPKLOSS.wChaLevel	= pchar_target->m_wLevel;
		sPKLOSS.nChaClass	= (int)pchar_target->m_emClass;
		sPKLOSS.wChaSchool	= pchar_target->m_wSchool;
		{
			BOOL bGuild = pchar_target->m_dwGuild ? TRUE : FALSE;
			if (bGuild)
			{
				sPKLOSS.wGuNum		= (WORD)pchar_target->m_dwGuild;
				GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
				GLCLUB* pCLUB		= cClubMan.GetClub(pchar_target->m_dwGuild);
				if (pCLUB)
				{
					sPKLOSS.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
					StringCchCopy(sPKLOSS.szGuName, CHAR_SZNAME, pCLUB->m_szName);
				}
			}
		}
		sPKLOSS.dwChaPKLoss = pchar_target->m_dwPKDeath;
		GLGaeaServer::GetInstance().ReqGlobalRankingKillUpdateAs(sPKLOSS, FALSE);
	}
}
/*Top Resu, Mhundz */
void GLChar::UpdateResuIncrease( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;
		
	/*if (( m_pLandMan->m_bPVPTyrannyMap && GLPVPTyrannyField::GetInstance().IsBattle() ) ||
		( m_pLandMan->m_bPVPSchoolWarsMap && GLPVPSchoolWarsField::GetInstance().IsBattle() ) || 
		( m_pLandMan->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagField::GetInstance().IsBattle() ) || 
		( m_pLandMan->m_bGuidBattleMap && m_pGLGaeaServer->IsClubBattleStarted()) ||
		( m_pLandMan->m_bClubDeathMatchMap && m_pGLGaeaServer->IsClubDMStarted()) )*/
	{
		m_dwResu++;
		m_dwMMR++;//add MMR for Resu

		// Maximum Value for MMR
        const DWORD MAX_MMR = GLCONST_CHAR::dwMMRMax;
        if (m_dwMMR > MAX_MMR) {
            m_dwMMR = MAX_MMR;
        }
	}


	

	GLMSG::SNETPC_UPDATE_RESU_SCORE2 net_msg_client;
	net_msg_client.wVal = m_dwResu;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );

	GLMSG::SNETPC_UPDATE_MMR_SCORE net_msg_client2;
	net_msg_client2.wVal = m_dwMMR;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client2 );

	GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_UPDATE NetMsgResu;
	NetMsgResu.sTopResu.dwChaNum	= m_dwCharID;
	StringCchCopy(NetMsgResu.sTopResu.szChaName, CHAR_SZNAME, m_szName);
	NetMsgResu.sTopResu.wChaLevel	= m_wLevel;
	NetMsgResu.sTopResu.nChaClass	= (int)m_emClass;
	NetMsgResu.sTopResu.wChaSchool	= m_wSchool;
	{
		BOOL bGuild = m_dwGuild ? TRUE : FALSE;
		if (bGuild)
		{
			NetMsgResu.sTopResu.wGuNum	= (WORD)m_dwGuild;
			GLClubMan& cClubMan		= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB			= cClubMan.GetClub(m_dwGuild);
			if (pCLUB)
			{
				NetMsgResu.sTopResu.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
				StringCchCopy(NetMsgResu.sTopResu.szGuName, CHAR_SZNAME, pCLUB->m_szName);
			}
		}
	}
	NetMsgResu.sTopResu.wResu = m_dwResu;
	m_pGLGaeaServer->SENDTOAGENT(&NetMsgResu);

	GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_UPDATE NetMsgMMR;
	NetMsgMMR.sTopMMR.dwChaNum	= m_dwCharID;
	StringCchCopy(NetMsgMMR.sTopMMR.szChaName, CHAR_SZNAME, m_szName);
	NetMsgMMR.sTopMMR.wChaLevel	= m_wLevel;
	NetMsgMMR.sTopMMR.nChaClass	= (int)m_emClass;
	NetMsgMMR.sTopMMR.wChaSchool	= m_wSchool;
	{
		BOOL bGuild = m_dwGuild ? TRUE : FALSE;
		if (bGuild)
		{
			NetMsgMMR.sTopMMR.wGuNum	= (WORD)m_dwGuild;
			GLClubMan& cClubMan		= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB			= cClubMan.GetClub(m_dwGuild);
			if (pCLUB)
			{
				NetMsgMMR.sTopMMR.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
				StringCchCopy(NetMsgMMR.sTopMMR.szGuName, CHAR_SZNAME, pCLUB->m_szName);
			}
		}
	}
	NetMsgMMR.sTopMMR.wMMR = m_dwMMR;
	m_pGLGaeaServer->SENDTOAGENT(&NetMsgMMR);
}
/*Top MMR, Mhundz */
void GLChar::UpdateMMRIncrease( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;
		
	if (( m_pLandMan->m_bPVPTyrannyMap && GLPVPTyrannyField::GetInstance().IsBattle() ) ||
		( m_pLandMan->m_bPVPSchoolWarsMap && GLPVPSchoolWarsField::GetInstance().IsBattle() ) || 
		( m_pLandMan->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagField::GetInstance().IsBattle() ) || 
		( m_pLandMan->m_bGuidBattleMap && m_pGLGaeaServer->IsClubBattleStarted()) ||
		( m_pLandMan->m_bClubDeathMatchMap && m_pGLGaeaServer->IsClubDMStarted()) )
	{
		
		// Increase MMR by 4
		m_dwMMR += 4;

		// Maximum Value for MMR
        const DWORD MAX_MMR = GLCONST_CHAR::dwMMRMax;
        if (m_dwMMR > MAX_MMR) {
            m_dwMMR = MAX_MMR;
        }

		// Send notification about the MMR increase - JX
        CString strMessage;
		strMessage.Format("Update MMR +4");
        
        GLMSG::SNET_SERVER_GENERALCHAT netmsgchat;
        netmsgchat.SETTEXT(strMessage.GetString());
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgchat);

		// Check if MMR reached the next rank threshold with a 4-point margin
		int nextRankIndex = CalculateVal(m_dwMMR - 4); // Check previous rank threshold
		int currentRankIndex = CalculateVal(m_dwMMR);  // Current rank based on MMR

		if (nextRankIndex != currentRankIndex) // Rank-up condition met
		{
			CString strMessage;
			strMessage.Format("'%s' has reached %s Rank", m_szName, GLCONST_CHAR::strRank_Name[currentRankIndex].c_str());

			GLMSG::SNET_SERVER_GENERALCHAT NetMsgMMRRankUp;
			NetMsgMMRRankUp.SETTEXT(strMessage.GetString());
			NetMsgMMRRankUp.bNotify = true;
			NetMsgMMRRankUp.iColor = 13;
			m_pGLGaeaServer->SENDTOALLCLIENT(&NetMsgMMRRankUp);
		}

	}

	GLMSG::SNETPC_UPDATE_MMR_SCORE net_msg_client;
	net_msg_client.wVal = m_dwMMR;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );

	GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_UPDATE NetMsgMMR;
	NetMsgMMR.sTopMMR.dwChaNum	= m_dwCharID;
	StringCchCopy(NetMsgMMR.sTopMMR.szChaName, CHAR_SZNAME, m_szName);
	NetMsgMMR.sTopMMR.wChaLevel	= m_wLevel;
	NetMsgMMR.sTopMMR.nChaClass	= (int)m_emClass;
	NetMsgMMR.sTopMMR.wChaSchool	= m_wSchool;
	{
		BOOL bGuild = m_dwGuild ? TRUE : FALSE;
		if (bGuild)
		{
			NetMsgMMR.sTopMMR.wGuNum	= (WORD)m_dwGuild;
			GLClubMan& cClubMan		= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB			= cClubMan.GetClub(m_dwGuild);
			if (pCLUB)
			{
				NetMsgMMR.sTopMMR.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
				StringCchCopy(NetMsgMMR.sTopMMR.szGuName, CHAR_SZNAME, pCLUB->m_szName);
			}
		}
	}
	NetMsgMMR.sTopMMR.wMMR = m_dwMMR;
	m_pGLGaeaServer->SENDTOAGENT(&NetMsgMMR);
}
/*Top MMR, Mhundz */
void GLChar::UpdateMMRDecrease( DWORD dwCharID )
{
	PGLCHAR pchar_target = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar_target )	return;
		
	if (( m_pLandMan->m_bPVPTyrannyMap && GLPVPTyrannyField::GetInstance().IsBattle() ) ||
		( m_pLandMan->m_bPVPSchoolWarsMap && GLPVPSchoolWarsField::GetInstance().IsBattle() ) || 
		( m_pLandMan->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagField::GetInstance().IsBattle() ) || 
		( m_pLandMan->m_bGuidBattleMap && m_pGLGaeaServer->IsClubBattleStarted()) ||
		( m_pLandMan->m_bClubDeathMatchMap && m_pGLGaeaServer->IsClubDMStarted()) )
	{


		// Ensure m_dwMMR does not go below 0
		if (m_dwMMR >= 5) 
		{
			m_dwMMR -= 5;
		} else {
			m_dwMMR = 0;
		}

		// Send notification about the MMR decrease - JX
        CString strMessage;
		strMessage.Format("Update MMR -5");
        
        GLMSG::SNET_SERVER_GENERALCHAT netmsgchat;
        netmsgchat.SETTEXT(strMessage.GetString());
//        netmsgchat.bNotify = true;
 //       netmsgchat.iColor = 7;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgchat);

		//output log for monitor data
		/*CString szTemp2;
		szTemp2.Format("%s After MMR : %d  (Decrease)", szTemp, m_dwMMR);
		CDebugSet::ToFileWithTime("_mmrScore.txt",szTemp2);*/
		//
	}

	GLMSG::SNETPC_UPDATE_MMR_SCORE net_msg_client;
	net_msg_client.wVal = m_dwMMR;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client );

	GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_UPDATE NetMsgMMR;
	NetMsgMMR.sTopMMR.dwChaNum	= m_dwCharID;
	StringCchCopy(NetMsgMMR.sTopMMR.szChaName, CHAR_SZNAME, m_szName);
	NetMsgMMR.sTopMMR.wChaLevel	= m_wLevel;
	NetMsgMMR.sTopMMR.nChaClass	= (int)m_emClass;
	NetMsgMMR.sTopMMR.wChaSchool	= m_wSchool;
	{
		BOOL bGuild = m_dwGuild ? TRUE : FALSE;
		if (bGuild)
		{
			NetMsgMMR.sTopMMR.wGuNum	= (WORD)m_dwGuild;
			GLClubMan& cClubMan		= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB			= cClubMan.GetClub(m_dwGuild);
			if (pCLUB)
			{
				NetMsgMMR.sTopMMR.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
				StringCchCopy(NetMsgMMR.sTopMMR.szGuName, CHAR_SZNAME, pCLUB->m_szName);
			}
		}
	}
	NetMsgMMR.sTopMMR.wMMR = m_dwMMR;
	m_pGLGaeaServer->SENDTOAGENT(&NetMsgMMR);
}
//////////////////////////////////////////////////////////////////////////////////////
/*skill summon, Juver, 2017/10/10 */
void GLChar::RemoveSummonBuff( DWORD dwSummonID )
{
	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
	BOOL bValidErase = FALSE;
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;

		if ( m_sSKILLFACT[i]._wCasterCrow != CROW_SUMMON )	continue;
		if ( m_sSKILLFACT[i]._dwCasterID != dwSummonID )	continue;

		DISABLESKEFF( i );
		NetMsgSkillBrd.bRESET[i] = true;
		bValidErase = TRUE;
	}

	if ( bValidErase )
	{
		NetMsgSkillBrd.dwID = m_dwGaeaID;
		NetMsgSkillBrd.emCrow = CROW_PC;

		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
		m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgSkillBrd );
	}
}

/*clubwar verify non push or pull, Juver, 2017/12/13 */
void GLChar::SetClubWarVerify( BOOL bSET )
{
	if ( RPARAM::bClubWarVerifyDisablePushPull )
	{
		m_bClubWarVerify = bSET;
		m_fClubWarVerifyTimer = 0.0f;
	}
}

/*school wars, Juver, 2018/01/20 */
BOOL GLChar::PVPSchoolWarsRevive( DWORD dwGateID )
{
	if ( !m_pLandMan )	return FALSE;

	DxLandGateMan* pgateman = &m_pLandMan->GetLandGateMan ();
	if ( !pgateman )	return FALSE;

	DxLandGate* pgate = pgateman->FindLandGate ( dwGateID );
	if ( !pgate )		return FALSE;

	BOOL brebirth = GLGaeaServer::GetInstance().RequestReBirth ( m_dwGaeaID, m_pLandMan->GetMapID(), dwGateID, m_vPos );
	if ( !brebirth )	return FALSE;

	ReSetSTATE(EM_ACT_DIE);

	m_fGenAge = 0.0f;
	GLCHARLOGIC_SERVER::INIT_RECOVER(100);
	TurnAction ( GLAT_IDLE );	

	GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,true) );
	//GLGaeaServer::GetInstance().ReserveDropOutSummon ( SDROPOUTSUMMONINFO(m_dwSummonGUID,true) );
	GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, true );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		GLGaeaServer::GetInstance().ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );

	GLMSG::SNETPC_REQ_REBIRTH_FB netmsgagent;
	netmsgagent.sMapID = m_pLandMan->GetMapID();
	netmsgagent.vPos = GetPosition();
	netmsgagent.wNowHP = m_sHP.wNow;
	netmsgagent.wNowMP = m_sMP.wNow;
	netmsgagent.wNowSP = m_sSP.wNow;

	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&netmsgagent);

	GLMSG::SNETPC_REQ_REVIVE_FB netmsgclient;
	netmsgclient.bEventRevive = true;
	netmsgclient.emFB = EMREQ_REVIVE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgclient);

	GLITEMLMT::GetInstance().ReqAction( m_dwCharID,	EMLOGACT_RESTORE, ID_CHAR, 0, m_nDECEXP, 0, 0, 0 );

	return TRUE;
}

/*pvp capture the flag, Juver, 2018/02/01 */
BOOL GLChar::PVPCaptureTheFlagRevive( DWORD dwGateID )
{
	if ( !m_pLandMan )	return FALSE;

	DxLandGateMan* pgateman = &m_pLandMan->GetLandGateMan ();
	if ( !pgateman )	return FALSE;

	DxLandGate* pgate = pgateman->FindLandGate ( dwGateID );
	if ( !pgate )		return FALSE;

	BOOL brebirth = GLGaeaServer::GetInstance().RequestReBirth ( m_dwGaeaID, m_pLandMan->GetMapID(), dwGateID, m_vPos );
	if ( !brebirth )	return FALSE;

	ReSetSTATE(EM_ACT_DIE);

	m_fGenAge = 0.0f;
	GLCHARLOGIC_SERVER::INIT_RECOVER(100);
	TurnAction ( GLAT_IDLE );	

	GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,true) );
	//GLGaeaServer::GetInstance().ReserveDropOutSummon ( SDROPOUTSUMMONINFO(m_dwSummonGUID,true) );
	GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, true );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		GLGaeaServer::GetInstance().ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );

	GLMSG::SNETPC_REQ_REBIRTH_FB netmsgagent;
	netmsgagent.sMapID = m_pLandMan->GetMapID();
	netmsgagent.vPos = GetPosition();
	netmsgagent.wNowHP = m_sHP.wNow;
	netmsgagent.wNowMP = m_sMP.wNow;
	netmsgagent.wNowSP = m_sSP.wNow;

	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&netmsgagent);

	GLMSG::SNETPC_REQ_REVIVE_FB netmsgclient;
	netmsgclient.bEventRevive = true;
	netmsgclient.emFB = EMREQ_REVIVE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgclient);

	GLITEMLMT::GetInstance().ReqAction( m_dwCharID,	EMLOGACT_RESTORE, ID_CHAR, 0, m_nDECEXP, 0, 0, 0 );

	return TRUE;
}

/*pvp capture the flag, Juver, 2018/02/06 */
void GLChar::PVPCaptureTheFlagDetectGate()
{
	if ( !m_pLandMan )					return;
	if ( !m_bCaptureTheFlagHoldFlag )	return;
	if ( !m_pLandMan->m_bPVPCaptureTheFlagMap )	return;

	DxLandGateMan *plandgateman = &m_pLandMan->GetLandGateMan();
	if ( !plandgateman )		return;

	PDXLANDGATE pdetectedgate = plandgateman->DetectGate ( GetPosition() );
	if ( !pdetectedgate )		return;

	GLPVPCaptureTheFlagField::GetInstance().FlagToBase( m_dwCharID, pdetectedgate->GetGateID() ); 
}

/*pvp capture the flag, Juver, 2018/02/07 */
void GLChar::PVPCaptureTheFlagHoldFlag( bool bHold )
{
	m_bCaptureTheFlagHoldFlag = bHold;

	m_fCaptureTheFlagHoldFlagTimer = 0.0f;
	m_fCaptureTheFlagDetectGateTimer = 0.0f;

	GLMSG::SNETPC_CAPTURE_THE_FLAG_F2C_PLAYER_FLAG_HOLD netmsgbrd;
	netmsgbrd.dwGaeaID = m_dwGaeaID;
	netmsgbrd.bHold = m_bCaptureTheFlagHoldFlag;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netmsgbrd);
	SendMsgViewAround ( (NET_MSG_GENERIC*) &netmsgbrd );

	if ( m_bCaptureTheFlagHoldFlag )
	{
		//remove buffs
		GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
		BOOL bUpdateBuffs = FALSE;
		for ( int i=0; i<SKILLFACT_SIZE; ++i )
		{
			if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
			if ( !pSkill ) continue;
			DISABLESKEFF( i );
			NetMsgSkillBrd.bRESET[i] = true;
			bUpdateBuffs = TRUE;
		}

		if ( bUpdateBuffs )
		{
			NetMsgSkillBrd.dwID = m_dwGaeaID;
			NetMsgSkillBrd.emCrow = CROW_PC;

			SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
			m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgSkillBrd );
		}
	}
}

/*skill vehicle off, Juver, 2018/09/07 */
void GLChar::skill_remove_vehicle()
{
	if ( GLGaeaServer::GetInstance().IsReserveServerStop() )	return;
	if ( !m_sVehicle.IsActiveValue() )	return;	

	GLMSG::SNETPC_ACTIVE_VEHICLE_FB NetMsgFB;

	if ( !m_bVehicle ) 
	{
		NetMsgFB.bActive = false;
		NetMsgFB.emFB = EMVEHICLE_SET_FB_RESET;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return;
	}

	m_bVehicle = FALSE;		
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();
	/*skill transform, Juver, 2018/09/09 */
	m_emANISUBTYPE = CHECK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );

	/*vehicle booster system, Juver, 2017/08/11 */
	VehicleBoosterStateReset();

	// ¹öÇÁ½ºÅ³ Á¦°Å
	for ( int i = 0; i < SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData( m_sSKILLFACT[i].sNATIVEID );

		if ( pSkill && pSkill->m_sBASIC.emIMPACT_SIDE != SIDE_ENEMY )
		{
			DISABLESKEFF( i );
		}	
	}

	// Äù¼Ç¹öÇÁ Á¾·á 
	m_sQITEMFACT.RESET();

	GLCHARLOGIC_SERVER::INIT_DATA ( FALSE, FALSE );
	ReSelectAnimation();

	NetMsgFB.bActive = false;
	NetMsgFB.emFB = EMVEHICLE_SET_FB_OK;
	NetMsgFB.bLeaveFieldServer = false;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	// ÁÖº¯¿¡ ¾Ë¸²
	GLMSG::SNETPC_ACTIVE_VEHICLE_BRD NetMsgBRD;

	NetMsgBRD.bActive = false;
	NetMsgBRD.dwGaeaID = m_dwGaeaID;

	SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgBRD) );
}

/*skill pet off, Juver, 2018/09/07 */
void GLChar::skill_remove_pet()
{
	if ( !m_pGLGaeaServer )	return;

	PGLPETFIELD ppet_field = m_pGLGaeaServer->GetPET ( m_dwPetGUID );
	if ( ppet_field )
	{
		m_pGLGaeaServer->DropOutPET ( m_dwPetGUID, false, false );
	}
}

/*extreme auto tab, Juver, 2018/09/10 */
void GLChar::skill_tab_change_broadcast( NET_MSG_GENERIC* nmg )
{
	if ( !RPARAM::allow_skill_tab_change )	return;

	if( IsUseArmSub() ) SetUseArmSub( FALSE );
	else				SetUseArmSub( TRUE );
	INIT_DATA ( FALSE, FALSE, m_sCONFTING.sOption.fHP_RATE );

	if ( RPARAM::allow_skill_tab_change_buff_reset )
	{
		// ¹«±â¿¡ µû¸¥ ¹öÇÁ¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
		DisableSkillFact();
	}

	GLMSG::SNETPC_REQ_SKILL_TAB_CHANGE_BRD net_msg_brd;
	net_msg_brd.emCrow = GETCROW();
	net_msg_brd.dwID = m_dwGaeaID;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &net_msg_brd );

	GLMSG::SNETPC_SKILL_TAB_CHANGE_FB net_msg_client;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_client);
}

/*client tick count check, EJCode, 2018/11/28 */
void GLChar::client_tick_count_check_send()
{
	if ( !RPARAM::client_tick_count_check )	return;

	if ( m_pGLGaeaServer )
	{
		if ( m_pGLGaeaServer->IsEmulatorMode() )	return;

		srand ( (unsigned)time(NULL) );

		GLMSG::SNETPC_CLIENT_TICK_COUNT_CHECK_REQUEST net_msg_client;
		net_msg_client.key1 = (DWORD)rand() % 32000 + 10001;
		net_msg_client.key2 = (DWORD)rand() % 32000 + 10001;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &net_msg_client);
	}
}

/* personal lock system, Juver, 2019/12/06 */
BOOL GLChar::isPersonalLock( EMPERSONAL_LOCK emLock )
{
	/* user flag restricted, Juver, 2020/04/21 */
	if ( m_bUserFlagRestricted )			return TRUE;

	if ( !RPARAM::bUsePersonalLock )		return FALSE;
	if ( emLock >= EMPERSONAL_LOCK_SIZE )	return FALSE;
	if ( m_sPersonalLock[emLock].bLock )	return TRUE;

	return FALSE;
}


/* skill stigma, Juver, 2020/12/12 */
void GLChar::SkillStigmaCheck()
{
	if ( m_mapSkillStigma.empty() )	return;
	if ( !m_pLandMan )				return;

	SKILL_STIGMA_DATA_MAP_ITER it1_b = m_mapSkillStigma.begin();
	SKILL_STIGMA_DATA_MAP_ITER it1_e = m_mapSkillStigma.end();
	SKILL_STIGMA_DATA_MAP_ITER it1_d;

	for ( ; it1_b != it1_e; )
	{
		SKILL_STIGMA_DATA& sData = (*it1_b).second;
		it1_d = it1_b++;

		TARID_SET_ITER it2_b = sData.setTargetID.begin();
		TARID_SET_ITER it2_e = sData.setTargetID.end();
		TARID_SET_ITER it2_d;

		for ( ; it2_b != it2_e; )
		{
			const STARGETID& sTarget = (*it2_b);
			it2_d = it2_b++;

			BOOL bDeleteTarget = FALSE;

			GLACTOR *pACTOR = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTarget );
			if ( pACTOR )
			{
				//check if buff still exist in target
				//buff might have ended or removed
				BOOL bExist = pACTOR->CheckSkillStigmaExist( CROW_PC, m_dwGaeaID, sData.sSkillID );
				if ( !bExist )
					bDeleteTarget = TRUE;
			}
			else
			{
				//target invalid, delete target
				bDeleteTarget = TRUE;
			}
				

			if ( bDeleteTarget )
				sData.setTargetID.erase ( it2_d );
		}

		BOOL bDeleteStigma = FALSE;

		//player moved to different map, targets becomes invalid, delete data
		if ( m_sMapID != sData.sMapID )
			bDeleteStigma = TRUE;

		//no more targets, delete data
		if ( sData.setTargetID.empty() )	
			bDeleteStigma = TRUE;

		if ( bDeleteStigma )
			m_mapSkillStigma.erase ( it1_d );
	}
}

/*12-16-24, daily login - CNDev*/
void GLChar::UpdateDailyLogin()
{
	CTime cCurrentTime = CTime::GetCurrentTime();

	if (m_cDailyLoginTimer.GetDay() == cCurrentTime.GetDay())	return;
	if (m_dwDayStreakCounter >= DAILYLOGIN_MAX_DAY)	return;

	m_cDailyLoginTimer = cCurrentTime;
	m_dwDayStreakCounter++;
	m_bDailyLogin = false;

	GLMSG::SNETPC_DAILYLOGIN_UPDATE netmsgclient;
	netmsgclient.dwStreak = m_dwDayStreakCounter;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgclient);
}

/* Add ItemShop, Ssodomain, 11-09-2023, Start */
void GLChar::SENDBOXITEMSHOPINFO ()
{
	GLMSG::SNET_GET_ITEMSHOP_FROMDB_FB NetMsgFB;

	NetMsgFB.emFB = EMREQ_ITEMSHOP_FROMDB_FB_OK;

	ITEMMAPSHOP::reverse_iterator pos = m_mapItemShop.rbegin();
	ITEMMAPSHOP::reverse_iterator end = m_mapItemShop.rend();

	for ( ; pos!=end; ++pos )
	{
		ITEMSHOP &sItemShop = (*pos).second;
		StringCchCopy ( NetMsgFB.szPurKey, PURKEY_LENGTH+1, sItemShop.strItemNum.GetString() );
		NetMsgFB.nidITEM = SNATIVEID(sItemShop.sID.wMainID,sItemShop.sID.wSubID);
		NetMsgFB.wPrice = sItemShop.wItemPrice;
		NetMsgFB.wStock = sItemShop.wItemStock;
		NetMsgFB.wCtg	= sItemShop.wItemCtg;
		NetMsgFB.wCurrency = sItemShop.wItemCurrency;
		m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );
	}
	NetMsgFB.emFB = EMREQ_ITEMSHOP_FROMDB_FB_END;
	m_pGLGaeaServer->SENDTOCLIENT( m_dwClientID, &NetMsgFB );
	return;
}
/* Add ItemShop, Ssodomain, 11-09-2023, End */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MMR Rank
int GLChar::CalculateVal ( DWORD dwCalc )
{
	
		 if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[9] ) return 9;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[8] ) return 8;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[7] ) return 7;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[6] ) return 6;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[5] ) return 5;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[4] ) return 4;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[3] ) return 3;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[2] ) return 2;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[1] ) return 1;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[0] ) return 0;
	else return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////