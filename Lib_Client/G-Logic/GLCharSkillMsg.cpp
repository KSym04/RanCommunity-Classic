#include "stdafx.h"
#include "./GLChar.h"

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

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#include "./GLSkillPurchase.h"

#include "./PVPPartyBattleGroundsField.h"

/*woe Arc Development 08-06-2024*/
#include "GLPVPWoeField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define SKILL_REVISION_RANGE 20
//4-24-2021 increased to 30 to compensate client position desync
//#define SKILL_REVISION_RANGE 30
//5-30-2021 moved to rparam

HRESULT GLChar::MsgReqLearnSkill ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_LEARNSKILL *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL *) nmg;

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) )	return S_FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_SKILL )	return E_FAIL;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		//	이미 습득한 스킬.
		GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = EMSKILL_LEARN_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		//	스킬 습득 요구 조건을 충족하지 못합니다.
		GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = emSKILL_LEARNCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	//	Note : 스킬 습득함.
	//
	if( pItem->sDrugOp.tTIME_LMT != 0 )
		LEARN_SKILL(sSKILL_ID, pItem->sBasicOp.sNativeID, CTime::GetCurrentTime().GetTime() );
	else
		LEARN_SKILL(sSKILL_ID, NATIVEID_NULL(), 0 );

	//	Note : 스킬 습득으로 인한 퀘스트 시작 점검.
	//
	QuestStartFromGetSKILL ( sSKILL_ID );

	//	Note : 소모성 아이템 제거.
	//
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY, true );

	//	스킬 배움 성공.
	GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
	NetMsgFB.skill_id = sSKILL_ID;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;
	if( pItem->sDrugOp.tTIME_LMT != 0 ) 
	{
		NetMsgFB.tBORNTIME = CTime::GetCurrentTime().GetTime();
		NetMsgFB.item_id = pItem->sBasicOp.sNativeID;
	}
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	스킬 배울때 소모된 포인트 업데이트.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//	Note : passive skill 의 경우 케릭에 적용되는 속성값을 클라이언트들에 동기화.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKILL_ID );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA.Assign( m_sSUM_PASSIVE );
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkillUp ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_SKILLUP *pNetMsg = (GLMSG::SNETPC_REQ_SKILLUP *) nmg;

	EMSKILL_LEARNCHECK emSKILL_LVLUPCHECK = EMSKILL_LEARN_UNKNOWN;
	SCHARSKILL* pCHARSKILL = GETLEARNED_SKILL(pNetMsg->skill_id);
	if ( !pCHARSKILL )
	{
		//	정상적으로 발생 할 수 없는 상황.
		return E_FAIL;
	}

	emSKILL_LVLUPCHECK = CHECKLEARNABLE_SKILL(pNetMsg->skill_id);
	if ( emSKILL_LVLUPCHECK!=EMSKILL_LEARN_OK )
	{
		//	랩업 조건이 부족합니다. FB 메시지.
		GLMSG::SNETPC_REQ_SKILLUP_FB	NetMsgFB;
		NetMsgFB.sSkill.sNativeID = pNetMsg->skill_id;
		NetMsgFB.emCHECK = emSKILL_LVLUPCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//	Note : 스킬 LEVEL UP.
	//
	WORD wToLevel = pCHARSKILL->wLevel + 1;
	LVLUP_SKILL ( pNetMsg->skill_id, wToLevel );

	//	랩업. FB 메시지.
	GLMSG::SNETPC_REQ_SKILLUP_FB	NetMsgFB;
	
	NetMsgFB.sSkill.sNativeID = pNetMsg->skill_id;
	NetMsgFB.sSkill.wLevel = wToLevel;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;

	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	스킬 배울때 소모된 포인트 업데이트.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//	Note : passive skill 의 경우 케릭에 적용되는 속성값을 클라이언트들에 동기화.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->skill_id );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA.Assign( m_sSUM_PASSIVE );
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkill ( NET_MSG_GENERIC* nmg )
{
	static bool bFirst = true;
	//static int nCnt = 0;

	//CONSOLEMSG_WRITE( "MsgReqSkill Start" );

	if ( !IsValidBody() )												return E_FAIL;
	if ( m_pLandMan && m_pLandMan->IsPeaceZone() )						return E_FAIL;

	if ( m_bSTATE_PANT || m_bSTATE_STUN )								return E_FAIL;

	//anti shit skillcut-as
	//if ( m_bIsSkillProcess )											return E_FAIL;
			
	//packet receive used skill on client already to cancel walk additional visual bug fix
	TurnAction ( GLAT_SKILL );

#if !defined(KR_PARAM) && !defined(KRT_PARAM)

	// 딜래이 체크
	if ( m_fSkillDelay < 0.3f )											return E_FAIL;  // 05.11.30  0.5f -> 0.3f

#endif

	GLMSG::SNETPC_REQ_SKILL *pNetMsg = (GLMSG::SNETPC_REQ_SKILL *) nmg;
	SNATIVEID skill_id = pNetMsg->skill_id;

	// 발동 스킬일경우 현재 발동 스킬인지 검사
	if ( pNetMsg->bDefenseSkill )
	{
		if ( pNetMsg->skill_id != m_sDefenseSkill.m_dwSkillID ) return E_FAIL;
		if ( !m_bDefenseSkill ) return E_FAIL;
	}

	//	스킬 정보 가져옴.
	const PGLSKILL pskill_main = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pskill_main )	return E_FAIL;

	WORD skill_level_main = 0;
	if ( pNetMsg->bDefenseSkill )
	{
		skill_level_main = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		SCHARSKILL* pCharSkill = GETLEARNED_SKILL(skill_id);
		if ( pCharSkill )
		{
			skill_level_main = pCharSkill->wLevel;
		}
	}

	/*dmk14 anti shit added fix*/
	const SKILL::CDATA_LVL &sSKILL_DATA = pskill_main->m_sAPPLY.sDATA_LVL[ skill_level_main ];

	WORD wMAX_TAR = 0;
	if( pskill_main->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC )			wMAX_TAR = sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1;
	else																wMAX_TAR = sSKILL_DATA.wTARNUM;

	if( pNetMsg->wTARGET_NUM_MAIN > wMAX_TAR )							pNetMsg->wTARGET_NUM_MAIN = wMAX_TAR;

	/*double dmg*/
	bool bFound = false;
	for ( WORD i=0; i<(pNetMsg->wTARGET_NUM_MAIN-1); ++i )
	{
		if( pNetMsg->sTARGET_IDS_MAIN[i].wID == USHRT_MAX )		continue;
		for( WORD x=(i+1); x<pNetMsg->wTARGET_NUM_MAIN;++x )
		{
			if( pNetMsg->sTARGET_IDS_MAIN[i].wID == pNetMsg->sTARGET_IDS_MAIN[x].wID )	
			{
				bFound = true;
				break;
			}
		}
		if( bFound )	break;
	}
	if( bFound )	return E_FAIL;

	if( pNetMsg->fAttVelo > GLCHARLOGIC_SERVER::GETATTVELO() && 
		m_fAttVeloSkill == GLCHARLOGIC_SERVER::GETATTVELO() )
	{
		return E_FAIL;
	}
	if( m_fAttVeloSkill != GLCHARLOGIC_SERVER::GETATTVELO() )	m_fAttVeloSkill = GLCHARLOGIC_SERVER::GETATTVELO();

	if( pNetMsg->fAttVeloItem > GLCHARLOGIC_SERVER::GETATT_ITEM() && 
		m_fAttVeloItem == GLCHARLOGIC_SERVER::GETATT_ITEM() )
	{
		return E_FAIL;
	}
	if( m_fAttVeloItem != GLCHARLOGIC_SERVER::GETATT_ITEM() )	m_fAttVeloItem = GLCHARLOGIC_SERVER::GETATT_ITEM();
	//end

	//check skill hash
	if ( ValidateSkillHash( pskill_main, pNetMsg->szHash ) == FALSE )	return E_FAIL;


	/*SkillGotoIdle();*/

	bool bFreePKMap		= m_pLandMan->IsFreePK();
	bool bGuidBattleMap = m_pLandMan->m_bGuidBattleMap;
	bool bBRIGHTEVENT   = GLGaeaServer::GetInstance().IsBRIGHTEVENT( m_sMapID );	/*dmk14 freepk*/
	bool bPVPWoeMap = m_pLandMan->m_bPVPWoeMap;

	/* open pk function, Juver, 2019/12/24 */
	bool bOpenPK = false;
	if ( m_pLandMan && m_pLandMan->IsOpenPK() )
	{
		bOpenPK = true;
	}

	bool bClubDeathMatch = false; 
	bool bPVPTyranny = false;			/*pvp tyranny, Juver, 2017/08/24 */
	bool bPVPSchoolWars = false;		/*school wars, Juver, 2018/01/19 */
	bool bPVPCaptureTheFlag = false;	/*pvp capture the flag, Juver, 2018/01/31 */
	bool bPVPClubDeathMatch = false;	/* pvp club death match, Juver, 2020/11/25 */
	bool bPVPWoe = false;


	if ( m_pLandMan->m_bClubDeathMatchMap )
	{
		GLClubDeathMatch* pCDM = GLClubDeathMatchFieldMan::GetInstance().Find( m_pLandMan->m_dwClubMapID );
		if ( pCDM && pCDM->IsBattle() ) bClubDeathMatch = true;	
	}

	/*pvp tyranny, Juver, 2017/08/24 */
	if ( m_pLandMan->m_bPVPTyrannyMap && GLPVPTyrannyField::GetInstance().IsBattle() )
		bPVPTyranny = true;	

	/*school wars, Juver, 2018/01/19 */
	if ( m_pLandMan->m_bPVPSchoolWarsMap && GLPVPSchoolWarsField::GetInstance().IsBattle() )
		bPVPSchoolWars = true;	

	/*pvp capture the flag, Juver, 2018/01/31 */
	if ( m_pLandMan->m_bPVPCaptureTheFlagMap && GLPVPCaptureTheFlagField::GetInstance().IsBattle() )
		bPVPCaptureTheFlag = true;	

	/*woe Arc Development 08-06-2024*/
	if (m_pLandMan->m_bPVPWoeMap && GLPVPWoeField::GetInstance().IsBattle())
		bPVPWoe = true;

	/* pvp club death match, Juver, 2020/11/25 */
	if ( ( m_pLandMan->m_bPVPClubDeathMatchLobbyMap || m_pLandMan->m_bPVPClubDeathMatchBattleMap ) && 
		PVPClubDeathMatchField::GetInstance().IsBattle() )
	{
		bPVPClubDeathMatch = true;
	}
	
	/* pvp party battle grounds, Juver, 2023/01/20 */
	bool bPBPPBG = false;
	if ( ( m_pLandMan->m_bPVPPBGLobbyMap || m_pLandMan->m_bPVPPBGBattleMap ) && 
		PVPPBG::ManagerField::GetInstance().IsBattle() )
	{
		bPBPPBG = true;
	}

	STARID_VEC vectargets_check_main;
	STARID_VEC vectargets_check_sub;
	vectargets_check_main.reserve( EMTARGET_NET );
	vectargets_check_sub.reserve( EMTARGET_NET );

	BOOL bSkillCheck1 = TRUE;
	BOOL bSkillCheck2 = TRUE;

	bSkillCheck1 = SkillTargetCheck( pskill_main, skill_level_main, pNetMsg->sTARGET_IDS_MAIN, pNetMsg->wTARGET_NUM_MAIN, pNetMsg->vTARGET_POS_MAIN, FALSE, vectargets_check_main );
	
	PGLSKILL pskill_sub = GLSkillMan::GetInstance().GetData ( pskill_main->m_sEXT_DATA.idMultiTargetSkill );
	if ( pskill_sub )
	{
		WORD skill_level_sub = skill_level_main;
		
		bSkillCheck2 = SkillTargetCheck( pskill_sub, skill_level_sub, pNetMsg->sTARGET_IDS_SUB, pNetMsg->wTARGET_NUM_SUB, pNetMsg->vTARGET_POS_SUB, TRUE, vectargets_check_sub );
	}

	GLMSG::SNETPC_REQ_SKILL_TARGET_FB NetMsgTargetFB;
	NetMsgTargetFB.skill_id = pNetMsg->skill_id;

	/*multi skill Juver, 2018/09/05 */
	NetMsgTargetFB.vTARGET_POS_MAIN = pNetMsg->vTARGET_POS_MAIN;
	NetMsgTargetFB.vTARGET_POS_SUB = pNetMsg->vTARGET_POS_SUB;

	for ( DWORD i=0; i<vectargets_check_main.size(); ++i )		
		NetMsgTargetFB.ADDTARGET_MAIN ( vectargets_check_main[i] );

	for ( DWORD i=0; i<vectargets_check_sub.size(); ++i )		
		NetMsgTargetFB.ADDTARGET_SUB ( vectargets_check_sub[i] );

	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgTargetFB );

	if ( !bSkillCheck1 )
	{
		SkillGotoIdle();
		return E_FAIL;
	}

	if ( !bSkillCheck2 )
	{
		SkillGotoIdle();
		return E_FAIL;
	}

	//	Note : 타겟 지정.
	//
	/*multi skill Juver, 2018/09/05 */
	m_TargetID.vPos = pNetMsg->vTARGET_POS_MAIN;
	m_TargetID.emCrow = pNetMsg->sTARGET_IDS_MAIN[0].GETCROW();
	m_TargetID.dwID = pNetMsg->sTARGET_IDS_MAIN[0].GETID();

	// 대련 종료후 무적타임이면 공격 무시
	if ( m_sCONFTING.IsPOWERFULTIME () )
	{
		SkillGotoIdle();
		return E_FAIL;
	}
	
	//	Note : 타겟의 유효성 점검.
	//

	STARID_VEC vectargets_main;
	STARID_VEC vectargets_sub;
	vectargets_main.reserve( vectargets_check_main.size() );
	vectargets_main.reserve( vectargets_check_sub.size() );

	/*multi skill Juver, 2018/09/05 */
	if ( pskill_main )
	{
		bool benermy = ( SIDE_ENEMY==pskill_main->m_sBASIC.emIMPACT_SIDE );

		/*dmk14 anti shit variable declaration*/
		bool bFirstAttFail = false;
		D3DXVECTOR3 vTarOrigin(0,0,0);
		D3DXVECTOR3 vTarOriginAngle(0,0,0);
		//end

		for ( DWORD i=0; i<vectargets_check_main.size(); ++i )
		{
			STARID sTarID = vectargets_check_main[i];
			STARGETID sTARID(sTarID.GETCROW(),sTarID.GETID());

			GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTARID );
			if ( !pTARGET )	continue;

			// 공격스킬만 검사
			if ( benermy )
			{
				//anti shit code by dmk14
				{			
					if(i==0)	
					{
						vTarOrigin = pTARGET->GetPosition();
						vTarOriginAngle = pTARGET->GetPosition() - GetPosition();
					}

					D3DXVECTOR3 vTarDir = pTARGET->GetPosition() - GetPosition();
					float fDir = DXGetThetaYFromDirection ( vTarDir, vTarOriginAngle );

					//DEFAULT RANGE
					WORD wRangeAllowance = 15;
					WORD wSplashRange = 10;
					//if( pTARGET->IsAction(GLAT_MOVE) )		wRangeAllowance += 5;

					D3DXVECTOR3 vTarPos = pTARGET->GetPosition();
					float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
					WORD wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pskill_main) + 4;
					WORD wAttackAbleDis = wAttackRange + wRangeAllowance;

					//Target
					switch ( pskill_main->m_sBASIC.emIMPACT_TAR )
					{
					//self
					case TAR_SELF:
						{
							//Area
							switch ( pskill_main->m_sBASIC.emIMPACT_REALM )
							{
								//Target Area
								case REALM_ZONE:	
									{
										vTarPos = pTARGET->GetPosition();
										fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
										wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + sSKILL_DATA.wAPPLYRANGE + 4;
										wAttackAbleDis = wAttackRange + wRangeAllowance;
									}
									break;
							};
						}
						break;
					//enemy
					case TAR_SPEC:	
						{
							//Area
							switch ( pskill_main->m_sBASIC.emIMPACT_REALM )
							{
								//Target Area
								case REALM_ZONE:		//SKT_TARSPECENERMY
									{
										if( i==0 )
										{
											vTarPos = pTARGET->GetPosition();
											fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
											wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pskill_main) + 4;
											wAttackAbleDis = wAttackRange + wRangeAllowance;
										}
										else
										{
											vTarPos = pTARGET->GetPosition();
											fDist = D3DXVec3Length ( &D3DXVECTOR3(vTarOrigin-vTarPos) );
											wAttackRange = GETBODYRADIUS() + sSKILL_DATA.wAPPLYRANGE + 4;
											wAttackAbleDis = wAttackRange + wSplashRange;
										}
									}
									break;
								//Around Target
								case REALM_FANWIZE:		//SKT_TARSPECFANWIZE
									{
										if( i==0 )
										{
											vTarPos = pTARGET->GetPosition();
											fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
											wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pskill_main) + 4;
											wAttackAbleDis = wAttackRange + wRangeAllowance;
										}
										else
										{
											vTarPos = pTARGET->GetPosition();
											fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
											wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pskill_main,skill_level_main) + 4; 
											wAttackAbleDis = wAttackRange + wSplashRange;
										}
								
										if ( sSKILL_DATA.wAPPLYANGLE!= 0 && sSKILL_DATA.wAPPLYANGLE!= 360 )
										{
											const float fApplyAngle = D3DXToRadian(sSKILL_DATA.wAPPLYANGLE) / 2.0f;
											if( fabs(fDir) > fApplyAngle )	wAttackAbleDis = 0;		//out of angle
										}
									}
									break;
							};
						}
						break;
					//from self to target
					case TAR_SELF_TOSPEC:	
						{
							switch ( pskill_main->m_sBASIC.emIMPACT_SIDE )
							{
								case SIDE_ENEMY:
									{
										if( i==0 )
										{
											vTarPos = pTARGET->GetPosition();
											fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
											wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pskill_main) + 4;
											wAttackAbleDis = wAttackRange + wRangeAllowance;
										}
										else
										{
											vTarPos = pTARGET->GetPosition();
											fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
											wAttackRange =  pTARGET->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pskill_main,skill_level_main) + 4;
											wAttackAbleDis = wAttackRange+wSplashRange;
										}
									}
									break;
							}
						}
						break;
					//specific loc
					case TAR_ZONE:	
						{
							switch ( pskill_main->m_sBASIC.emIMPACT_SIDE )
							{
								case SIDE_OUR:
								case SIDE_ANYBODY:
									{
										vTarPos = pTARGET->GetPosition();
										fDist = D3DXVec3Length ( &D3DXVECTOR3(pNetMsg->vTARGET_POS_MAIN-vTarPos) );
										wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + sSKILL_DATA.wAPPLYRANGE+4;
										wAttackAbleDis = wAttackRange+wRangeAllowance;
									}
									break;
								case SIDE_ENEMY:
									{
										vTarPos = pTARGET->GetPosition();
										fDist = D3DXVec3Length ( &D3DXVECTOR3(pNetMsg->vTARGET_POS_MAIN-vTarPos) );
										wAttackRange = sSKILL_DATA.wAPPLYRANGE+4;
										wAttackAbleDis = wAttackRange+wRangeAllowance;
									}
									break;
							}
						}
						break;
					};
			
					if ( fDist > wAttackAbleDis )
					{
						if( i==0 )	{
							bFirstAttFail = true;
						}

						continue;
					}
				}//anti shit code by dmk14

#if !defined(KR_PARAM) && !defined(KRT_PARAM) && !defined(JP_PARAM)

				// 타겟과의 거리 검사
				D3DXVECTOR3 vTarPos = pTARGET->GetPosition();
				float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
				WORD wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + wSkillRange + 7;
				if ( ISLONGRANGE_ARMS() )	 wAttackRange += (WORD) GETSUM_TARRANGE();
				WORD wAttackAbleDis = wAttackRange + 20;

				// 공격 유효거리에 있는 케릭터 검사
				if ( fDist > wAttackAbleDis )
				{
					//	Note : 자신에게.
					GLMSG::SNETPC_ATTACK_AVOID NetMsg;
					NetMsg.emTarCrow	= sTARID.emCrow;
					NetMsg.dwTarID		= sTARID.dwID;

					GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

					//	Note : 주변 클라이언트들에게 메세지 전송.
					//
					GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
					NetMsgBrd.dwGaeaID	= m_dwGaeaID;
					NetMsgBrd.emTarCrow	= sTARID.emCrow;
					NetMsgBrd.dwTarID	= sTARID.dwID;

					SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
					continue;
				}

#endif

			}

			bool bREACTIONALBE  = IsReActionable( pTARGET, benermy );			
			if ( !bREACTIONALBE )												continue;

			bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
			if ( GetSchool() == pTARGET->GetSchool() )	bSCHOOL_FREEPK = false;

			//	Note : 학원간 자유 pk일 때는 다른 학원생을 부활 시킬수 없음.
			if ( pskill_main->m_sAPPLY.IsSpec( EMSPECA_REBIRTH ) && bSCHOOL_FREEPK ) continue;
		
			if ( pTARGET->GetCrow() == CROW_PC )
			{
				PGLCHAR pCHAR = GLGaeaServer::GetInstance().GetChar ( sTARID.dwID );
				if ( pCHAR )
				{
					// 부활일 경우에 상대방이 부활을 가능할 경우에만 실행 된다.
					if( pskill_main->m_sBASIC.sNATIVEID.wMainID == 14 && pskill_main->m_sBASIC.sNATIVEID.wSubID == 15 && pCHAR->m_bNon_Rebirth == TRUE )
					{
						//	Note : 스킬 구동 실패 FB 메시지.
						{
							GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
							strcpy( NetMsgFB.szName, pCHAR->m_szName );
							NetMsgFB.emSKILL_FB = EMSKILL_NOTREBIRTH;
							GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
						}
						{
							GLMSG::SNET_MSG_REQ_SKILL_REVIVEL_FAILED NetMsgFB;
							strcpy( NetMsgFB.szName, m_szName );
							GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
						}

						return E_FAIL;
					}
					// 대련 종료후 무적타임이면 공격 무시
					if ( pCHAR->m_sCONFTING.IsPOWERFULTIME () )					continue;

					bool bClubBattle = false;
					
					if ( m_pLandMan->IsClubBattleZone() )
					{
						GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
						GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
						GLCLUB *pTarClub = cClubMan.GetClub ( pCHAR->m_dwGuild );

						if ( pMyClub && pTarClub )
						{
							bool bClub = pMyClub->IsBattle( pCHAR->m_dwGuild );
							bool bAlliance = pMyClub->IsBattleAlliance ( pTarClub->m_dwAlliance );

							bClubBattle = ( bClub || bAlliance );
						}
					}


					/*pvp tyranny, Juver, 2017/08/24 */
					/*school wars, Juver, 2018/01/19 */
					/*pvp capture the flag, Juver, 2018/01/31 */
					/* open pk function, Juver, 2019/12/24 */
					/* pvp club death match, Juver, 2020/11/25 */
					if ( !(bGuidBattleMap || 
						bClubDeathMatch ||
						bSCHOOL_FREEPK ||
						bFreePKMap ||
						bBRIGHTEVENT ||
						IsConflictTarget(pTARGET) || 
						bClubBattle || 
						bPVPTyranny || 
						bPVPSchoolWars ||
						bPVPCaptureTheFlag || 
						bPVPClubDeathMatch || 
						bPVPWoe ||
						bPBPPBG || 
						bOpenPK ) )
					{
						//	Note : 상대방이 범죄자가 아니고 정당방위 시간이 지정되지 않았을때는 
						//		자신이 적대행위를 시 작함.
						if ( !IS_PLAYHOSTILE(pCHAR->m_dwCharID) && !pCHAR->ISOFFENDER() && benermy )
						{
							int nBRIGHT(GLCONST_CHAR::nPK_TRY_BRIGHT_POINT), nLIFE(GLCONST_CHAR::nPK_TRY_LIVING_POINT);

							//	Note : 속성수치 변화.
							m_nBright += nBRIGHT;

							GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
							NetMsg.nBright = m_nBright;
							GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

							GLMSG::SNETPC_UPDATE_BRIGHT_BRD NetMsgBrd;
							NetMsgBrd.dwGaeaID = m_dwGaeaID;
							NetMsgBrd.nBright = m_nBright;
							SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

							//	Note : 생활점수 변화.
							m_nLiving += nLIFE;

							GLMSG::SNETPC_UPDATE_LP NetMsgLp;
							NetMsgLp.nLP = m_nLiving;
							GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgLp);

							//	Note : 적대행위 로그 기록.
							GLITEMLMT::GetInstance().ReqAction
							(
								m_dwCharID,					//	당사자.
								EMLOGACT_HOSTILE,			//	행위.
								ID_CHAR, pCHAR->m_dwCharID,	//	상대방.
								0,							//	exp
								nBRIGHT,					//	bright
								nLIFE,						//	life
								0							//	money
							);
						}
					}

					if ( benermy && !IsConflictTarget(pTARGET) )
					{
						//	Note : 적대 행위자에게 피해자 등록. ( pk )
						AddPlayHostile ( pCHAR->m_dwCharID, TRUE, bClubBattle );

						//	Note : 적대 피해자에게 적대자 등록. ( PK )
						pCHAR->AddPlayHostile ( m_dwCharID, FALSE, bClubBattle );

						STARGETID sTargetID = STARGETID ( CROW_PC, m_dwGaeaID, m_vPos );
					
					
						// 공격자와 공격대상자 모두 팻을 공격모드로 변경
						PGLPETFIELD pEnemyPet = GLGaeaServer::GetInstance().GetPET ( pCHAR->m_dwPetGUID );
						if ( pEnemyPet && pEnemyPet->IsValid () && !pEnemyPet->IsSTATE ( EM_PETACT_ATTACK ) )
						{
							D3DXVECTOR3 vOwnerPos, vDist;
							float fDist;
							vOwnerPos = pCHAR->GetPosition ();
							vDist = pEnemyPet->m_vPos - vOwnerPos;
							fDist = D3DXVec3Length(&vDist);

							// 일정거리 안에 있으면 
							if ( fDist <= GLCONST_PET::fWalkArea )
							{
								pEnemyPet->ReSetAllSTATE ();
								pEnemyPet->SetSTATE ( EM_PETACT_ATTACK );

								GLMSG::SNETPET_ATTACK NetMsg;
								NetMsg.sTarID = sTargetID;
								GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsg );
							
								GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
								NetMsgBRD.dwGUID = pEnemyPet->m_dwGUID;
								NetMsgBRD.sTarID = sTargetID;
								pCHAR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
							}

						}

						// 맞는자만 Summon Attack 처리
						/*skill summon, Juver, 2017/10/09 */
						for ( int ii=0; ii<SKILL_SUMMON_MAX_CLIENT_NUM; ++ii )
						{
							PGLSUMMONFIELD pEnemySummon = GLGaeaServer::GetInstance().GetSummon ( pCHAR->m_dwSummonGUID_FLD[ii] );
							if ( pEnemySummon && pEnemySummon->IsValid () && !pEnemySummon->IsSTATE ( EM_SUMMONACT_ATTACK ) && 
								pEnemySummon->GetAttackTarget( sTargetID ) && pEnemySummon->IsTargetReady() )
							{
								/*D3DXVECTOR3 vOwnerPos, vDist;
								float fDist;
								vOwnerPos = pCHAR->GetPosition ();
								vDist = pEnemySummon->m_vPos - vOwnerPos;
								fDist = D3DXVec3Length(&vDist);

								if ( fDist <= pEnemySummon->m_fWalkArea )*/
								{
									pEnemySummon->SetAttackTarget( sTargetID );
								}
							}
						}
					

						PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
						if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
						{
							D3DXVECTOR3 vDist;
							float fDist;
							vDist = pMyPet->m_vPos - m_vPos;
							fDist = D3DXVec3Length(&vDist);

							// 일정거리 안에 있으면 
							if ( fDist <= GLCONST_PET::fWalkArea )
							{
								pMyPet->ReSetAllSTATE ();
								pMyPet->SetSTATE ( EM_PETACT_ATTACK );

								GLMSG::SNETPET_ATTACK NetMsg;
								NetMsg.sTarID = m_TargetID;
								GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
							
								GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
								NetMsgBRD.dwGUID = pMyPet->m_dwGUID;
								NetMsgBRD.sTarID = m_TargetID;
								SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
							}
						}
					}
				}
			}

			BOOL bexist = FALSE;
			for( int n=0; n<(int)vectargets_main.size(); ++n )
			{
				if ( sTarID.wCrow == vectargets_main[n].wCrow &&  sTarID.wID == vectargets_main[n].wID )
				{
					bexist = TRUE;
					break;
				}
			}

			if ( !bexist )	vectargets_main.push_back( sTarID );
		}
	}
	



	/*multi skill Juver, 2018/09/05 */
	if ( pskill_main )
	{
		PGLSKILL pskill_sub = GLSkillMan::GetInstance().GetData ( pskill_main->m_sEXT_DATA.idMultiTargetSkill );
		if ( pskill_sub )
		{
			WORD skill_level_sub = skill_level_main;
			
			bool benermy = ( SIDE_ENEMY==pskill_sub->m_sBASIC.emIMPACT_SIDE );

			WORD wSkillRange = GETSKILLRANGE_TAR(*pskill_sub);	

			for ( DWORD i=0; i<vectargets_check_sub.size(); ++i )
			{
				STARID sTarID = vectargets_check_sub[i];
				STARGETID sTARID(sTarID.GETCROW(),sTarID.GETID());
				GLACTOR* pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTARID );
				if ( !pTARGET )	continue;

				// 공격스킬만 검사
				if ( benermy )
				{

#if !defined(KR_PARAM) && !defined(KRT_PARAM) && !defined(JP_PARAM)

					// 타겟과의 거리 검사
					D3DXVECTOR3 vTarPos = pTARGET->GetPosition();
					float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
					WORD wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + wSkillRange + 7;
					if ( ISLONGRANGE_ARMS() )	 wAttackRange += (WORD) GETSUM_TARRANGE();
					WORD wAttackAbleDis = wAttackRange + 20;

					// 공격 유효거리에 있는 케릭터 검사
					if ( fDist > wAttackAbleDis )
					{
						//	Note : 자신에게.
						GLMSG::SNETPC_ATTACK_AVOID NetMsg;
						NetMsg.emTarCrow	= sTARID.emCrow;
						NetMsg.dwTarID		= sTARID.dwID;

						GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

						//	Note : 주변 클라이언트들에게 메세지 전송.
						//
						GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
						NetMsgBrd.dwGaeaID	= m_dwGaeaID;
						NetMsgBrd.emTarCrow	= sTARID.emCrow;
						NetMsgBrd.dwTarID	= sTARID.dwID;

						SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
						continue;
					}
#endif

				}

				bool bREACTIONALBE  = IsReActionable( pTARGET, benermy );			
				if ( !bREACTIONALBE )												continue;

				bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
				if ( GetSchool() == pTARGET->GetSchool() )	bSCHOOL_FREEPK = false;

				//	Note : 학원간 자유 pk일 때는 다른 학원생을 부활 시킬수 없음.
				if ( pskill_sub->m_sAPPLY.IsSpec( EMSPECA_REBIRTH ) && bSCHOOL_FREEPK ) continue;
		
				if ( pTARGET->GetCrow() == CROW_PC )
				{
					PGLCHAR pCHAR = GLGaeaServer::GetInstance().GetChar ( sTARID.dwID );
					if ( pCHAR )
					{
						// 부활일 경우에 상대방이 부활을 가능할 경우에만 실행 된다.
						if( pskill_sub->m_sBASIC.sNATIVEID.wMainID == 14 && pskill_sub->m_sBASIC.sNATIVEID.wSubID == 15 && pCHAR->m_bNon_Rebirth == TRUE )
						{
							//	Note : 스킬 구동 실패 FB 메시지.
							{
								GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
								strcpy( NetMsgFB.szName, pCHAR->m_szName );
								NetMsgFB.emSKILL_FB = EMSKILL_NOTREBIRTH;
								GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
							}
							{
								GLMSG::SNET_MSG_REQ_SKILL_REVIVEL_FAILED NetMsgFB;
								strcpy( NetMsgFB.szName, m_szName );
								GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
							}

							return E_FAIL;
						}
						// 대련 종료후 무적타임이면 공격 무시
						if ( pCHAR->m_sCONFTING.IsPOWERFULTIME () )					continue;

						bool bClubBattle = false;
						
						if ( m_pLandMan->IsClubBattleZone() )
						{
							GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
							GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
							GLCLUB *pTarClub = cClubMan.GetClub ( pCHAR->m_dwGuild );

							if ( pMyClub && pTarClub )
							{
								bool bClub = pMyClub->IsBattle( pCHAR->m_dwGuild );
								bool bAlliance = pMyClub->IsBattleAlliance ( pTarClub->m_dwAlliance );

								bClubBattle = ( bClub || bAlliance );
							}
						}

						/*pvp tyranny, Juver, 2017/08/24 */
						/*school wars, Juver, 2018/01/19 */
						/*pvp capture the flag, Juver, 2018/01/31 */
						/* open pk function, Juver, 2019/12/24 */
						/* pvp club death match, Juver, 2020/11/25 */
						if ( !(bGuidBattleMap ||
							bClubDeathMatch ||
							bSCHOOL_FREEPK ||
							bFreePKMap ||
							bBRIGHTEVENT ||
							IsConflictTarget(pTARGET) || 
							bClubBattle ||
							bPVPTyranny || 
							bPVPSchoolWars || 
							bPVPCaptureTheFlag || 
							bPVPClubDeathMatch || 
							bPBPPBG ||
							bPVPWoe ||
							bOpenPK ) )
						{
							//	Note : 상대방이 범죄자가 아니고 정당방위 시간이 지정되지 않았을때는 
							//		자신이 적대행위를 시 작함.
							if ( !IS_PLAYHOSTILE(pCHAR->m_dwCharID) && !pCHAR->ISOFFENDER() && benermy )
							{
								int nBRIGHT(GLCONST_CHAR::nPK_TRY_BRIGHT_POINT), nLIFE(GLCONST_CHAR::nPK_TRY_LIVING_POINT);

								//	Note : 속성수치 변화.
								m_nBright += nBRIGHT;

								GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
								NetMsg.nBright = m_nBright;
								GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

								GLMSG::SNETPC_UPDATE_BRIGHT_BRD NetMsgBrd;
								NetMsgBrd.dwGaeaID = m_dwGaeaID;
								NetMsgBrd.nBright = m_nBright;
								SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

								//	Note : 생활점수 변화.
								m_nLiving += nLIFE;

								GLMSG::SNETPC_UPDATE_LP NetMsgLp;
								NetMsgLp.nLP = m_nLiving;
								GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgLp);

								//	Note : 적대행위 로그 기록.
								GLITEMLMT::GetInstance().ReqAction
								(
									m_dwCharID,					//	당사자.
									EMLOGACT_HOSTILE,			//	행위.
									ID_CHAR, pCHAR->m_dwCharID,	//	상대방.
									0,							//	exp
									nBRIGHT,					//	bright
									nLIFE,						//	life
									0							//	money
								);
							}
						}

						if ( benermy && !IsConflictTarget(pTARGET) )
						{
							//	Note : 적대 행위자에게 피해자 등록. ( pk )
							AddPlayHostile ( pCHAR->m_dwCharID, TRUE, bClubBattle );

							//	Note : 적대 피해자에게 적대자 등록. ( PK )
							pCHAR->AddPlayHostile ( m_dwCharID, FALSE, bClubBattle );

							STARGETID sTargetID = STARGETID ( CROW_PC, m_dwGaeaID, m_vPos );
					
					
							// 공격자와 공격대상자 모두 팻을 공격모드로 변경
							PGLPETFIELD pEnemyPet = GLGaeaServer::GetInstance().GetPET ( pCHAR->m_dwPetGUID );
							if ( pEnemyPet && pEnemyPet->IsValid () && !pEnemyPet->IsSTATE ( EM_PETACT_ATTACK ) )
							{
								D3DXVECTOR3 vOwnerPos, vDist;
								float fDist;
								vOwnerPos = pCHAR->GetPosition ();
								vDist = pEnemyPet->m_vPos - vOwnerPos;
								fDist = D3DXVec3Length(&vDist);

								// 일정거리 안에 있으면 
								if ( fDist <= GLCONST_PET::fWalkArea )
								{
									pEnemyPet->ReSetAllSTATE ();
									pEnemyPet->SetSTATE ( EM_PETACT_ATTACK );

									GLMSG::SNETPET_ATTACK NetMsg;
									NetMsg.sTarID = sTargetID;
									GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsg );
							
									GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
									NetMsgBRD.dwGUID = pEnemyPet->m_dwGUID;
									NetMsgBRD.sTarID = sTargetID;
									pCHAR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
								}

							}

							// 맞는자만 Summon Attack 처리
							/*skill summon, Juver, 2017/10/09 */
							for ( int ii=0; ii<SKILL_SUMMON_MAX_CLIENT_NUM; ++ii )
							{
								PGLSUMMONFIELD pEnemySummon = GLGaeaServer::GetInstance().GetSummon ( pCHAR->m_dwSummonGUID_FLD[ii] );
								if ( pEnemySummon && pEnemySummon->IsValid () && !pEnemySummon->IsSTATE ( EM_SUMMONACT_ATTACK ) && 
									pEnemySummon->GetAttackTarget( sTargetID ) && pEnemySummon->IsTargetReady() )
								{
									/*D3DXVECTOR3 vOwnerPos, vDist;
									float fDist;
									vOwnerPos = pCHAR->GetPosition ();
									vDist = pEnemySummon->m_vPos - vOwnerPos;
									fDist = D3DXVec3Length(&vDist);

									if ( fDist <= pEnemySummon->m_fWalkArea )*/
									{
										pEnemySummon->SetAttackTarget( sTargetID );
									}
								}
							}
					

							PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
							if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
							{
								D3DXVECTOR3 vDist;
								float fDist;
								vDist = pMyPet->m_vPos - m_vPos;
								fDist = D3DXVec3Length(&vDist);

								// 일정거리 안에 있으면 
								if ( fDist <= GLCONST_PET::fWalkArea )
								{
									pMyPet->ReSetAllSTATE ();
									pMyPet->SetSTATE ( EM_PETACT_ATTACK );

									GLMSG::SNETPET_ATTACK NetMsg;
									NetMsg.sTarID = m_TargetID;
									GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
							
									GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
									NetMsgBRD.dwGUID = pMyPet->m_dwGUID;
									NetMsgBRD.sTarID = m_TargetID;
									SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
								}
							}
						}
					}
				}

				BOOL bexist = FALSE;
				for( int n=0; n<(int)vectargets_sub.size(); ++n )
				{
					if ( sTarID.wCrow == vectargets_sub[n].wCrow &&  sTarID.wID == vectargets_sub[n].wID )
					{
						bexist = TRUE;
						break;
					}
				}

				if ( !bexist )	vectargets_sub.push_back( sTarID );
			}
		}
	}

	/*skill validity check, Juver, 2017/11/26 */
	m_wTARGET_NUM_MAIN = (WORD)vectargets_main.size();
	m_wTARGET_NUM_SUB = (WORD)vectargets_sub.size();

	for( int n=0; n<EMTARGET_NET; ++n )
	{
		m_sTARGET_IDS_MAIN[n].wCrow = 0;
		m_sTARGET_IDS_MAIN[n].wID = USHRT_MAX;

		m_sTARGET_IDS_SUB[n].wCrow = 0;
		m_sTARGET_IDS_SUB[n].wID = USHRT_MAX;
	}

	for( int n=0; n<(int)vectargets_main.size(); ++n )
	{
		if ( n >= EMTARGET_NET )	continue;
		m_sTARGET_IDS_MAIN[n] = vectargets_main[n];
	}

	for( int n=0; n<(int)vectargets_sub.size(); ++n )
	{
		if ( n >= EMTARGET_NET )	continue;
		m_sTARGET_IDS_SUB[n] = vectargets_sub[n];
	}

	m_vTARGET_POS_MAIN = pNetMsg->vTARGET_POS_MAIN;
	m_vTARGET_POS_SUB = pNetMsg->vTARGET_POS_SUB;

	/*multi skill Juver, 2018/09/05 */
	if ( m_wTARGET_NUM_MAIN == 0 )
	{
		//	Note : 자신에게.
		GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
		NetMsg.emTarCrow		= m_TargetID.emCrow;
		NetMsg.dwTarID			= m_TargetID.dwID;
		NetMsg.nDamage			= 0;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

		SkillGotoIdle();

		return E_FAIL;
	}

	const SKILL::SEXT_DATA &sEXT_DATA = pskill_main->m_sEXT_DATA;

	//	Note : 스킬 구동 조건 검사.
	//
	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[sEXT_DATA.emANIMTYPE][sEXT_DATA.emANISTYPE];
	if ( vecAniAttack.empty() )
	{
		// 캐릭터의 스킬에니메이션이 없습니다.
		DEBUGMSG_WRITE ( _T("Can't find skill animation. char[%d] animation[%d][%d]"), m_CHARINDEX, sEXT_DATA.emANIMTYPE, sEXT_DATA.emANISTYPE );
		return E_FAIL;
	}

	const SANIATTACK &sAniAttack = vecAniAttack[0];
	WORD wStrikeNum = sAniAttack.m_wDivCount;
	if ( wStrikeNum==0 )
	{
		// 스킬 에니메이션에 타격 지점이 지정되지 않았습니다
		DEBUGMSG_WRITE ( _T("Can't find target position of skill animation. char[%d] animation[%d][%d]"), m_CHARINDEX, sEXT_DATA.emANIMTYPE, sEXT_DATA.emANISTYPE );
		return E_FAIL;
	}

	/*extreme auto tab, Juver, 2018/09/10 */
	EMSLOT em_right_hand = GetCurRHand();
	EMSLOT em_left_hand = GetCurLHand();
	EMSKILLCHECK emCHECK = GLCHARLOGIC_SERVER::CHECHSKILL ( skill_id, 1, pNetMsg->bDefenseSkill, em_right_hand, em_left_hand );
	if ( emCHECK != EMSKILL_OK && emCHECK != EMSKILL_NOTSP )
	{
		//	Note : 스킬 구동 실패 FB 메시지.
		GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
		NetMsgFB.emSKILL_FB = emCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	//	Note : SKILL 구동.
	//
	SETACTIVESKILL ( skill_id );
	SetDefenseSkill( pNetMsg->bDefenseSkill );
	if ( pNetMsg->bDefenseSkill ) m_bDefenseSkill = false;

	m_SKILLMTYPE = sEXT_DATA.emANIMTYPE;
	m_SKILLSTYPE = sEXT_DATA.emANISTYPE;

	BOOL bLowSP =  (emCHECK==EMSKILL_NOTSP) ? TRUE : FALSE;
	PreStrikeProc ( TRUE, bLowSP );

	if ( !pskill_main->m_sBASIC.IsRunningCast() ) TurnAction ( GLAT_SKILL );
	else
	{
		if ( m_fAccumTime > 0.5f )
		{
			SkillProc ( m_idACTIVESKILL, false );
		}
	}

	// 스킬이 정상적으로 발동되면 딜래이를 초기화
	m_fSkillDelay = 0.0f;
	
	//	Note : 스킬 사용시 소모값들 소모시킴.
	//
	GLCHARLOGIC_SERVER::ACCOUNTSKILL ( skill_id, 1, true );

	/* skill delay addition, Juver, 2020/12/09 */
	ApplySkillDelay( skill_id );

	//	Note : SKILL 구동 성공시 - 자신의 주변 Char에게 자신의 Msg를 전달.
	//
	WORD wLevel;
	if ( pNetMsg->bDefenseSkill )	wLevel = m_sDefenseSkill.m_wLevel;
	else wLevel = GETLEARNED_SKILL(pNetMsg->skill_id)->wLevel;

	GLMSG::SNETPC_REQ_SKILL_BRD NetMsgBRD;
	NetMsgBRD.emCrow = GETCROW();
	NetMsgBRD.dwID = m_dwGaeaID; //- 시전자 정보. 
	NetMsgBRD.skill_id = pNetMsg->skill_id;
	NetMsgBRD.wLEVEL = wLevel; //- 시전 스킬 종류.

	/*multi skill Juver, 2018/09/05 */
	NetMsgBRD.vTARGET_POS_MAIN = pNetMsg->vTARGET_POS_MAIN;
	NetMsgBRD.vTARGET_POS_SUB = pNetMsg->vTARGET_POS_SUB;

	for ( WORD i=0; i<m_wTARGET_NUM_MAIN; ++i )		
		NetMsgBRD.ADDTARGET_MAIN ( m_sTARGET_IDS_MAIN[i] );

	for ( WORD i=0; i<m_wTARGET_NUM_SUB; ++i )		
		NetMsgBRD.ADDTARGET_SUB ( m_sTARGET_IDS_SUB[i] );

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );



	//	Note : 스킬 구동 성공 FB 메시지.
	GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
	NetMsgFB.emSKILL_FB = EMSKILL_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//CONSOLEMSG_WRITE( "MsgReqSkill End [%u]", skill_id.dwID );

	// 몹을 공격하면 팻이 공격모션을 취함
	if ( m_TargetID.emCrow==CROW_MOB )
	{
		PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
		if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
		{
			D3DXVECTOR3 vDist;
			float fDist;
			vDist = pMyPet->m_vPos - m_vPos;
			fDist = D3DXVec3Length(&vDist);

			// 일정거리 안에 있으면 
			if ( fDist <= GLCONST_PET::fWalkArea )
			{
				GLMSG::SNETPET_ATTACK NetMsg;
				NetMsg.sTarID = m_TargetID;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
				
				GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
				NetMsgBRD.dwGUID = m_dwPetGUID;
				NetMsgBRD.sTarID = m_TargetID;
				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
			}
		}
	}

	/*skill summon, Juver, 2017/10/11 */
	ReTargetSummon( STARID(m_TargetID.emCrow,m_TargetID.dwID) );

	/* skill turn/find invisible, Juver, 2020/04/26 */
	if ( m_bSkillTurnInvisible && m_bSkillTurnInvisibleDisableInCombat )
	{
		//cancel invisibility buff when skill target type is for enemy
		//buffs and party skills allowed
		if ( pskill_main && pskill_main->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
		{
			SkillBuffCancel( m_sSkillTurnInvisibleID );
		}	
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkillCancel ( NET_MSG_GENERIC* nmg )
{
	//CONSOLEMSG_WRITE( "MsgReqSkillCancel" );



	//	Note : 액션 취소.
	if ( IsACTION(GLAT_SKILL) )		TurnAction ( GLAT_IDLE );

	//	Note : 자신의 주변 Char에게 자신의 Msg를 전달.
	//
	GLMSG::SNETPC_SKILL_CANCEL_BRD NetMsgBRD;
	NetMsgBRD.dwGaeaID = m_dwGaeaID;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_SET *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_SET *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : 배운 스킬이 아닐 경우 취소됨.
	if ( !ISLEARNED_SKILL(pNetMsg->skill_id) )	return E_FAIL;

	//	Note : 슬롯에 넣어줌.
	m_sSKILLQUICK[pNetMsg->wSLOT] = pNetMsg->skill_id;

	//	Note : 클라이언트에 통보.
	GLMSG::SNETPC_REQ_SKILLQUICK_FB NetMsgFB;
	NetMsgFB.wSLOT = pNetMsg->wSLOT;
	NetMsgFB.skill_id = m_sSKILLQUICK[pNetMsg->wSLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQReSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_RESET *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_RESET *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : 슬롯에 넣어줌.
	m_sSKILLQUICK[pNetMsg->wSLOT] = NATIVEID_NULL();

	//	Note : 클라이언트에 통보.
	GLMSG::SNETPC_REQ_SKILLQUICK_FB NetMsgFB;
	NetMsgFB.wSLOT = pNetMsg->wSLOT;
	NetMsgFB.skill_id = m_sSKILLQUICK[pNetMsg->wSLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQSetActive ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : 액티브된 슬롯 스킬을 지정.
	m_wSKILLQUICK_ACT = pNetMsg->wSLOT;

	return S_OK;
}

/* skill turn/find invisible, Juver, 2020/04/26 */
void GLChar::SkillBuffCancel( SNATIVEID sSkillID )
{
	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( sSkillID );
	if ( !pskill )					return;
	if ( !pskill->IsSkillFact() )	return;
	if ( pskill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )		return;

	BOOL buff_exist = FALSE;
	for( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		const SSKILLFACT& skill_fact = m_sSKILLFACT[i];
		if ( skill_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( skill_fact.sNATIVEID == sSkillID )	
			buff_exist = TRUE;
	}

	if ( !buff_exist )	return;

	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
		if ( m_sSKILLFACT[i].sNATIVEID != sSkillID )		continue;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;

		DISABLESKEFF( i );
		NetMsgSkillBrd.bRESET[i] = true;
	}

	NetMsgSkillBrd.dwID = m_dwGaeaID;
	NetMsgSkillBrd.emCrow = CROW_PC;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgSkillBrd );
}

/* skill effect release, Juver, 2020/04/30 */
void GLChar::SkillEffectRelease( SNATIVEID sSkillID )
{
	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( sSkillID );
	if ( !pskill )					return;
	if ( !pskill->IsSkillFact() )	return;

	BOOL buff_exist = FALSE;
	for( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		const SSKILLFACT& skill_fact = m_sSKILLFACT[i];
		if ( skill_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( skill_fact.sNATIVEID == sSkillID )	
			buff_exist = TRUE;
	}

	if ( !buff_exist )	return;

	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
		if ( m_sSKILLFACT[i].sNATIVEID != sSkillID )		continue;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;

		DISABLESKEFF( i );
		NetMsgSkillBrd.bRESET[i] = true;
	}

	NetMsgSkillBrd.dwID = m_dwGaeaID;
	NetMsgSkillBrd.emCrow = CROW_PC;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgSkillBrd );
}

/* skill effect release, Juver, 2020/04/30 */
void GLChar::SkillEffectReleaseCheck()
{
	if ( m_mapEffectRelease.empty() )	return;

	std::vector<SNATIVEID> vecRelease;

	SEFFECT_RELEASE_DATA_MAP_ITER it_b = m_mapEffectRelease.begin();
	SEFFECT_RELEASE_DATA_MAP_ITER it_e = m_mapEffectRelease.end();
	for( ; it_b != it_e; ++it_b )
	{
		SEFFECT_RELEASE_DATA& sData = (*it_b).second;
		if ( RANDOM_POS > sData.fRate )	continue;
		vecRelease.push_back(sData.sidSkill);
	}

	if ( vecRelease.empty() )	return;

	for ( size_t i=0; i<vecRelease.size(); ++i )
	{
		const SNATIVEID& sid = vecRelease[i];
		SkillEffectRelease( sid );

		SEFFECT_RELEASE_DATA_MAP_ITER it = m_mapEffectRelease.find( sid.dwID );
		if ( it != m_mapEffectRelease.end() )
		{
			m_mapEffectRelease.erase( it );
		}
	}
}

/* skill delay addition, Juver, 2020/12/09 */
void GLChar::ApplySkillDelay( SNATIVEID _skill_id )
{
	if ( m_mapSkillDelay.empty() )		return;

	DELAY_MAP_ITER it = m_SKILLDELAY.find( _skill_id.dwID );
	if ( it == m_SKILLDELAY.end() )		return;

	float &fDelay = (*it).second;


	float fAdjust = 1.0f;
	bool bValid = false;

	SKILL_DELAY_DATA_MAP_ITER it_b = m_mapSkillDelay.begin();
	SKILL_DELAY_DATA_MAP_ITER it_e = m_mapSkillDelay.end();
	for ( ; it_b != it_e; ++it_b )
	{
		SSKILL_DELAY_DATA& sData  = (*it_b).second;

		float fChance = sData.fChance/100.0f;
		float fCurrent = RANDOM_POS;

		if ( fCurrent > fChance )		
			continue;

		fAdjust += sData.fDelay;
		bValid = true;
	}

	if ( bValid )
	{
		fDelay = fDelay * fAdjust;

		GLMSG::SNETPC_F2C_SKILL_DELAY_ADJUSTMENT NetMsgFB;
		NetMsgFB.sSkillID	= _skill_id;
		NetMsgFB.fAdjust	= fAdjust;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
	}
}

/* skill delay addition, Juver, 2020/12/09 */
void GLChar::ApplySkillDelayOnce( SNATIVEID _skill_id, float fSkillDelay, float fSkillChance )
{
	if ( m_SKILLDELAY.empty() )	return;

	GLMSG::SNETPC_F2C_SKILL_DELAY_ADJUSTMENT_MULTI NetMsgUpdate;

	DELAY_MAP_ITER it_b = m_SKILLDELAY.begin();
	DELAY_MAP_ITER it_e = m_SKILLDELAY.end();

	for ( ; it_b != it_e; ++it_b )
	{
		SNATIVEID skillID = (*it_b).first;
		float &fDelay = (*it_b).second;

		if ( skillID == _skill_id )
			continue;

		float fCurChance = fSkillChance/100.0f;
		float fCurRate = RANDOM_POS;

		if ( fCurRate > fCurChance )		
			continue;

		float fAdjust = 1.0f;
		fAdjust += fSkillDelay;

		fDelay = fDelay * fAdjust;

		NetMsgUpdate.ADD( skillID, fAdjust );
		if ( NetMsgUpdate.wDataNum == SKILL_DELAY_MULTI_UPDATE_MAX_ARRAY )
		{
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgUpdate );
			NetMsgUpdate.RESET();
		}
	}

	if ( NetMsgUpdate.wDataNum != 0 )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgUpdate );
		NetMsgUpdate.RESET();
	}
}

/* skill stigma, Juver, 2020/12/13 */
void GLChar::ProcessSkillStigmaTypeSkill( const SNATIVEID& _sSkillID )
{
	if ( !m_pLandMan )				return;
	if ( !m_pGLGaeaServer )			return;
	if ( m_mapSkillStigma.empty() )	return;

	SCHARSKILL sSkill;
	WORD wSKILL_LVL_MAIN = 0;

	if ( IsDefenseSkill() )
	{
		if ( _sSkillID != m_sDefenseSkill.m_dwSkillID ) return;
		sSkill.sNativeID = _sSkillID;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
		wSKILL_LVL_MAIN = m_sDefenseSkill.m_wLevel;;

	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( _sSkillID.dwID );
		if ( learniter==m_ExpSkills.end() )	return;
		sSkill = (*learniter).second;
		wSKILL_LVL_MAIN = sSkill.wLevel;
	}

	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[m_SKILLMTYPE][m_SKILLSTYPE];
	if ( vecAniAttack.empty() )	return;

	const SANIATTACK &sAniAttack = vecAniAttack[0];

	SKILL_STIGMA_DATA_MAP_ITER it1_b = m_mapSkillStigma.begin();
	SKILL_STIGMA_DATA_MAP_ITER it1_e = m_mapSkillStigma.end();

	for ( ; it1_b != it1_e; ++it1_b)
	{
		SKILL_STIGMA_DATA& sData = (*it1_b).second;
		if ( sData.sSkillID == _sSkillID )	continue;;

		if ( sData.dwAttackFlag & SKILL::EMSPEC_STIGMA_TYPE_FLAG_SKILL )
		{
			TARID_SET_ITER it2_b = sData.setTargetID.begin();
			TARID_SET_ITER it2_e = sData.setTargetID.end();

			for ( ; it2_b != it2_e; ++it2_b )
			{
				const STARGETID& sTarget = (*it2_b);

				GLACTOR* pACTOR = m_pGLGaeaServer->GetTarget ( m_pLandMan, sTarget );
				if ( !pACTOR )									continue;

				D3DXVECTOR3 vDist = m_vPos - pACTOR->GetPosition();
				float fTarLength = D3DXVec3Length(&vDist);
				if ( fTarLength > sData.fDistance )				continue;

				PGLSKILL pskill_main = GLSkillMan::GetInstance().GetData ( _sSkillID );
				if ( pskill_main )
				{
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
							continue;
						}
					}

					SKILL::CDATA_LVL &sSKILL_DATA = pskill_main->m_sAPPLY.sDATA_LVL[wSKILL_LVL_MAIN];

					for ( WORD j=0; j<sSKILL_DATA.wAPPLYNUM; ++j )
					{
						if ( pACTOR->GetNowHP()==0 )				continue;

						DWORD dwDamageFlag = DAMAGE_TYPE_NONE;

						int nVAR_HP(0), nVAR_MP(0), nVAR_SP(0);

						short nRESIST = pACTOR->GETRESIST().GetElement(pskill_main->m_sAPPLY.emELEMENT);
						if ( nRESIST>99 )	nRESIST = 99;

						switch ( pskill_main->m_sAPPLY.emBASIC_TYPE )
						{
						case SKILL::EMFOR_HP:
							{
								if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
								{
									DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
									dwDamageFlag = CALCDAMAGE ( nVAR_HP, m_dwGaeaID, sTarget, m_pLandMan, pskill_main, wSKILL_LVL_MAIN, dwWeather, sAniAttack.m_wDivCount );
									nVAR_HP = int( nVAR_HP*sData.fDamage);
									nVAR_HP = - nVAR_HP;
								}
							}break;

						case SKILL::EMFOR_MP:
							{
								if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
								{
									int nVAR = int(sSKILL_DATA.fBASIC_VAR*sData.fDamage);
									nVAR = - nVAR;
									nVAR_MP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
								}
							}break;

						case SKILL::EMFOR_SP:
							{
								if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
								{
									int nVAR = int(sSKILL_DATA.fBASIC_VAR*sData.fDamage);
									nVAR = - nVAR;
									nVAR_SP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
								}
							}break;
						};

						SSKILLACT sSKILLACT;
						sSKILLACT.sID = STARGETID(CROW_PC,m_dwGaeaID);
						sSKILLACT.sID_TAR = sTarget;
						sSKILLACT.fDELAY = pskill_main->m_sEXT_DATA.fDELAY4DAMAGE;
						sSKILLACT.emAPPLY = pskill_main->m_sBASIC.emAPPLY;
						sSKILLACT.dwDamageFlag = dwDamageFlag;			

						sSKILLACT.nVAR_HP = nVAR_HP;
						sSKILLACT.nVAR_MP = nVAR_MP;
						sSKILLACT.nVAR_SP = nVAR_SP;

						if ( sSKILLACT.IsATTACK() && sSKILLACT.VALID() )
						{
							m_pLandMan->RegSkillAct ( sSKILLACT );
						}
					}
				}

				/*multi skill Juver, 2018/09/04 */
				if ( pskill_main )
				{
					PGLSKILL pskill_sub = GLSkillMan::GetInstance().GetData ( pskill_main->m_sEXT_DATA.idMultiTargetSkill );
					if ( pskill_sub )
					{
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
								continue;
							}
						}

						int wSKILL_LVL_SUB = wSKILL_LVL_MAIN;

						SKILL::CDATA_LVL &sSKILL_DATA = pskill_sub->m_sAPPLY.sDATA_LVL[wSKILL_LVL_SUB];

						SNATIVEID sUseSkillID = pskill_sub->m_sBASIC.sNATIVEID;

						for ( WORD j=0; j<sSKILL_DATA.wAPPLYNUM; ++j )
						{
							if ( pACTOR->GetNowHP()==0 )				continue;

							DWORD dwDamageFlag = DAMAGE_TYPE_NONE;

							int nVAR_HP(0), nVAR_MP(0), nVAR_SP(0);

							short nRESIST = pACTOR->GETRESIST().GetElement(pskill_sub->m_sAPPLY.emELEMENT);
							if ( nRESIST>99 )	nRESIST = 99;

							switch ( pskill_sub->m_sAPPLY.emBASIC_TYPE )
							{
							case SKILL::EMFOR_HP:
								{
									if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
									{
										DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
										dwDamageFlag = CALCDAMAGE ( nVAR_HP, m_dwGaeaID, sTarget, m_pLandMan, pskill_sub, wSKILL_LVL_SUB, dwWeather, sAniAttack.m_wDivCount );
										nVAR_HP = int( nVAR_HP*sData.fDamage);
										nVAR_HP = - nVAR_HP;
									}
								}break;

							case SKILL::EMFOR_MP:
								{
									if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
									{
										int nVAR = int(sSKILL_DATA.fBASIC_VAR*sData.fDamage);
										nVAR = - nVAR;
										nVAR_MP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
									}
								}break;

							case SKILL::EMFOR_SP:
								{
									if ( sSKILL_DATA.fBASIC_VAR < 0.0f )
									{
										int nVAR = int(sSKILL_DATA.fBASIC_VAR*sData.fDamage);
										nVAR = - nVAR;
										nVAR_SP -= (int) ( nVAR - (nVAR*nRESIST/100.0f*GLCONST_CHAR::fRESIST_G ) );
									}
								}break;
							};

							SSKILLACT sSKILLACT;
							sSKILLACT.sID = STARGETID(CROW_PC,m_dwGaeaID);
							sSKILLACT.sID_TAR = sTarget;
							sSKILLACT.fDELAY = pskill_sub->m_sEXT_DATA.fDELAY4DAMAGE;
							sSKILLACT.emAPPLY = pskill_sub->m_sBASIC.emAPPLY;
							sSKILLACT.dwDamageFlag = dwDamageFlag;			

							sSKILLACT.nVAR_HP = nVAR_HP;
							sSKILLACT.nVAR_MP = nVAR_MP;
							sSKILLACT.nVAR_SP = nVAR_SP;

							if ( sSKILLACT.IsATTACK() && sSKILLACT.VALID() )
							{
								m_pLandMan->RegSkillAct ( sSKILLACT );
							}
						}
					}
				}
			}

		}

	}
}

/* skill stigma, Juver, 2020/12/13 */
void GLChar::ProcessSkillStigmaTypeAttack( const DWORD dwDivCount, const BOOL bLowSP )
{
	if ( !m_pLandMan )				return;
	if ( !m_pGLGaeaServer )			return;
	if ( m_mapSkillStigma.empty() )	return;

	SKILL_STIGMA_DATA_MAP_ITER it1_b = m_mapSkillStigma.begin();
	SKILL_STIGMA_DATA_MAP_ITER it1_e = m_mapSkillStigma.end();

	for ( ; it1_b != it1_e; ++it1_b)
	{
		SKILL_STIGMA_DATA& sData = (*it1_b).second;
		if ( sData.dwAttackFlag & SKILL::EMSPEC_STIGMA_TYPE_FLAG_ATTACK )
		{
			TARID_SET_ITER it2_b = sData.setTargetID.begin();
			TARID_SET_ITER it2_e = sData.setTargetID.end();

			for ( ; it2_b != it2_e; ++it2_b )
			{
				const STARGETID& sTarget = (*it2_b);

				GLACTOR *pACTOR = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTarget );
				if ( pACTOR )
				{
					D3DXVECTOR3 vDist = m_vPos - pACTOR->GetPosition();
					float fTarLength = D3DXVec3Length(&vDist);
					if ( fTarLength > sData.fDistance )	continue;

					int nDamage = 0;
					DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
					dwDamageFlag = CALCDAMAGE ( nDamage, m_dwGaeaID, sTarget, m_pLandMan, NULL, 0, NULL, dwDivCount );

					float	fDAMAGE_RATE(1.0f);
					if ( bLowSP )						
						fDAMAGE_RATE *= (1-GLCONST_CHAR::fLOWSP_DAMAGE);

					nDamage = int(nDamage*fDAMAGE_RATE);
					if ( nDamage < 1 )	nDamage = 1;

					if ( sTarget.emCrow == CROW_PC )
					{
						PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( sTarget.dwID );
						if ( pCHAR )
						{
							if ( pCHAR->m_sCONFTING.IsPOWERFULTIME () )
								continue;
						}

						nDamage = int(nDamage*GLCONST_CHAR::fPK_POINT_DEC_PHY[ CharClassToClassIndex( m_emClass )]);
						if ( nDamage==0 )	nDamage = 1;
					}

					nDamage = int(nDamage*sData.fDamage);
					if ( nDamage < 1 ) nDamage = 1;

					bool bShock = ( dwDamageFlag & DAMAGE_TYPE_SHOCK );
					ToDamage ( sTarget, nDamage, bShock );

					GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
					NetMsg.emTarCrow		= sTarget.emCrow;
					NetMsg.dwTarID			= sTarget.dwID;
					NetMsg.nDamage			= nDamage;
					NetMsg.dwDamageFlag		= dwDamageFlag;
					m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

					GLMSG::SNETPC_ATTACK_DAMAGE_BRD NetMsgBrd;
					NetMsgBrd.dwGaeaID		= m_dwGaeaID;
					NetMsgBrd.emTarCrow		= sTarget.emCrow;
					NetMsgBrd.dwTarID		= sTarget.dwID;
					NetMsgBrd.nDamage		= nDamage;
					NetMsgBrd.dwDamageFlag	= dwDamageFlag;
					SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
				}
			}
		}
	}
}

void GLChar::SkillGotoIdle()
{
	if ( m_actorMove.PathIsActive() )
	{
		if ( IsACTION(GLAT_MOVE) ) 
			m_actorMove.Stop();

		GLMSG::SNETPC_REQ_SKILL_GO_TO_IDLE_BRD NetMsgBRD;
		NetMsgBRD.emCrow = GETCROW();
		NetMsgBRD.dwID = m_dwGaeaID; 
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
	}
}

HRESULT GLChar::MsgReqPurchaseSkill ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_PURCHASE_SKILL *pNetMsg = (GLMSG::SNETPC_REQ_PURCHASE_SKILL *) nmg;

	SNATIVEID sSKILL_ID = pNetMsg->sSkillID;

	GLMSG::SNETPC_REQ_PURCHASE_SKILL_FB	NetMsgFB;
	NetMsgFB.sSkillID = sSKILL_ID;
	NetMsgFB.emFB = EMREQ_PURCHASE_SKILL_FB_FAIL;
	
	//skill already learned 
	SCHARSKILL* pCharSkill = GETLEARNED_SKILL( sSKILL_ID );
	if ( pCharSkill )
	{
		NetMsgFB.emFB = EMREQ_PURCHASE_SKILL_FB_ALREADY_LEARNED;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//skill has no purchase configuration
	SSKILL_PURCHASE* pPurchase = GLSkillPurchase::GetInstance().getSkillPurchase( sSKILL_ID );
	if ( !pPurchase )
	{
		NetMsgFB.emFB = EMREQ_PURCHASE_SKILL_FB_CANNOT_PURCHASE;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//skill stats failed
	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		NetMsgFB.emFB = EMREQ_PURCHASE_SKILL_FB_REQ_STATS;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//cost check
	if ( m_lnMoney < pPurchase->dwCost )
	{
		NetMsgFB.emFB = EMREQ_PURCHASE_SKILL_FB_REQ_COST;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}


	//	Note : 스킬 습득함.
	//
	LEARN_SKILL(sSKILL_ID, NATIVEID_NULL(),0);

	//	Note : 스킬 습득으로 인한 퀘스트 시작 점검.
	//
	QuestStartFromGetSKILL ( sSKILL_ID );

	CheckMoneyUpdate( m_lnMoney, pPurchase->dwCost, FALSE, "SKILL_PURCHASE" );
	m_bMoneyUpdate = TRUE;

	m_lnMoney -= pPurchase->dwCost;

	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgMoney );

	//	스킬 배움 성공.
	GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB2;
	NetMsgFB2.skill_id = sSKILL_ID;
	NetMsgFB2.emCHECK = EMSKILL_LEARN_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB2 );

	//	스킬 배울때 소모된 포인트 업데이트.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//	Note : passive skill 의 경우 케릭에 적용되는 속성값을 클라이언트들에 동기화.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKILL_ID );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA.Assign( m_sSUM_PASSIVE );
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}