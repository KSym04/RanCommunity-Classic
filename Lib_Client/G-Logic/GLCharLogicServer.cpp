#include "stdafx.h"
#include "./GLCharLogicServer.h"

#include "./GLItemMan.h"
#include "./GLQuest.h"
#include "./GLQuestMan.h"

#include "./GLLandMan.h"
#include "./GLGaeaServer.h"
#include "./GLSchoolFreePK.h"
#include "./GLCodex.h" /* Codex, Archie 02/16/24 */

/*item user requirement, Juver, 2018/05/24 */
#include "./GLItemUser.h"

#include "./UserTypeDefine.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void GLCHARLOGIC_SERVER::RESET_DATA ()
{
	int i=0;

	SCHARDATA2::Assign ( SCHARDATA2() );

	m_CHARINDEX = GLCI_BRAWLER_M;
	m_SKILLDELAY.clear();

	for ( i=0; i<SKILLFACT_SIZE; ++i )
		m_sSKILLFACT[i] = SSKILLFACT();

	m_dwHOLDBLOW = NULL;
	m_bSTATEBLOW = false;

	for ( i=0; i<EMBLOW_MULTI; ++i )
		m_sSTATEBLOWS[i] = SSTATEBLOW();

	m_fSTATE_MOVE = 1.0f;
	m_fSTATE_DELAY = 1.0f;

	m_fSTATE_DAMAGE = 1.0f;

	m_bSTATE_PANT = false;
	m_bSTATE_STUN = false;
	
	m_fSKILL_MOVE = 0.0f;
	m_fOPTION_MOVE = 0.0f;

	m_sDefenseSkill.RESET();

	m_sSUMSTATS = SCHARSTATS();

	m_wSUM_AP = 0;
	m_wSUM_DP = 0;
	m_wSUM_PA = 0;
	m_wSUM_SA = 0;
	m_wSUM_MA = 0;

	m_sSUMRESIST = SRESIST();
	m_sSUMRESIST_SKILL = SRESIST();
	m_sSUMITEM = SSUM_ITEM();
	m_sSUM_PASSIVE = SPASSIVE_SKILL_DATA_SERVER();
	
	m_nSUM_PIERCE = 0;
	m_fSUM_TARGET_RANGE = 0;
	
	/*skill range spec logic, Juver, 2017/06/06 */
	m_fSUM_ATTACK_RANGE = 0.0f;
	m_fSUM_APPLY_RANGE = 0.0f;

	m_wACCEPTP = 0;
	m_wSUM_DisSP = 0;

	m_nHIT = 0;
	m_nSUM_HIT = 0;
	m_nAVOID = 0;
	m_nSUM_AVOID = 0;

	m_nDEFENSE_BODY = 0;
	m_nDEFENSE = 0;
	m_nDEFENSE_SKILL = 0;

	m_gdDAMAGE = GLPADATA();
	m_gdDAMAGE_SKILL = GLPADATA();
	m_gdDAMAGE_PHYSIC = GLPADATA();

	m_fDamageRate = 1.0f;
	m_fDefenseRate = 1.0f;

	m_fINCR_HP = 0.0f;
	m_fINCR_MP = 0.0f;
	m_fINCR_SP = 0.0f;

	m_fATTVELO = 0.0f;
	m_wATTRANGE = 0;
	m_wSUM_ATTRANGE = 0;
	m_fSKILLDELAY = 0.0f;

	m_emITEM_ATT = ITEMATT_NOTHING;

	m_emANISUBTYPE = AN_SUB_NONE;

	m_idACTIVESKILL = SNATIVEID(false);
	m_emANIMAINSKILL = AN_GUARD_N;
	m_emANISUBSKILL = AN_SUB_NONE;

	m_bUseArmSub = FALSE;

	for ( i=0; i<SLOT_TSIZE; ++i )
		m_pITEMS[i] = NULL;

	m_fIncHP = 0.0f;
	m_fIncMP = 0.0f;

	DEL_PLAYHOSTILE_ALL();	

	m_bVehicle = FALSE;
	m_fVehicleSpeedRate = 0.0f;
	m_fVehicleSpeedVol = 0.0f;

	m_sDamageSpec.RESET();

	m_ExpSkills.clear();

	m_bSafeZone = false;

	/*prohibit potion skill logic, Juver, 2017/06/06 */
	m_bProhibitPotion = false;

	/*prohibit skill logic, Juver, 2017/06/06 */
	m_bProhibitSkill = false; 

	/*continuous damage skill logic, Juver, 2017/06/10 */
	m_mapContinuousDamage.clear();

	/*curse skill logic, Juver, 2017/06/10 */
	m_bCurse = false;
	m_fCurseDamage = 0.0f;

	/*vehicle booster system, Juver, 2017/08/11 */
	m_bBoosterCharge = false;
	m_bBoosterStart = false;
	m_fBoosterTimer = 0.0f;

	/*skill vehicle off, Juver, 2018/09/07 */
	m_skill_vehicle_off = false;

	/*skill pet off, Juver, 2018/09/07 */
	m_skill_pet_off = false;

	/*skill transform, Juver, 2018/09/08 */
	m_skill_transform_data.Reset();

	/* skill turn/find invisible, Juver, 2020/04/25 */
	m_bSkillTurnInvisible = false;
	m_wSkillTurnInvisibleLevel = 0;
	m_wSkillTurnInvisibleAnimation = SKILL::EMSPEC_INVISIBLE_ANI_TYPE_NONE;
	m_bSkillTurnInvisibleDisableInCombat = false;
	m_sSkillTurnInvisibleID = NATIVEID_NULL();
	m_bSkillShowInvisible = false;
	m_wSkillShowInvisibleLevel = 0;
	m_wSkillShowInvisibleRange = 0;

	/* skill effect release, Juver, 2020/04/30 */
	m_mapEffectRelease.clear();

	DISABLEALLLANDEFF();

	/* costume combine map restriction, Juver, 2020/09/12 */
	m_bCostumeCombineRestrictMoveSpeed = false;
	m_bCostumeCombineRestrictAttackSpeed = false;

	/* skill delay addition, Juver, 2020/12/09 */
	m_mapSkillDelay.clear();

	/* skill amplify, Juver, 2020/12/10 */
	m_sSkillAmplifyMelee.RESET();
	m_sSkillAmplifyMissile.RESET();
	m_sSkillAmplifyMagic.RESET();

	/* skill stigma, Juver, 2020/12/12 */
	m_mapSkillStigma.clear();

	/* skill position shift, Juver, 2020/12/15 */
	m_bSkillPositionShift = false;

	/* skill hostile, Juver, 2020/12/16 */
	m_bSkillHostile = false;

	/* skill buff duration change, Juver, 2020/12/17 */
	m_sSkillDurationChange.Reset();

	/* non vend area, Juver, 2021/01/17 */
	m_bNonVendArea = false;

	/* skill illusion, Juver, 2021/01/17 */
	m_sSkillIllusion.Reset();

	/* post system, Juver, 2021/02/05 */
	m_sPOSTBOXID	  = NATIVEID_NULL();
	m_emPOSTBOXIDTYPE = EMPOSTBOXID_TYPE_NONE;

	/* pk combo, Juver, 2021/02/09 */
	m_sPKComboState.Reset();

	/* anti shit */
	m_fAttVeloSkill = 0.0f;
	m_fAttVeloItem = 0.0f;

	//offline vend
	m_bOffVend = false;
}

void GLCHARLOGIC_SERVER::INIT_NEW_CHAR ( const EMCHARINDEX _emCIndex, const DWORD dwUserID, const DWORD dwServerID, const char* szCharName,
								 const WORD _wSchool, const WORD wHair, const WORD wFace, const WORD _wHairColor, const WORD _wSex, const float _fScaleRange )
{
	EMCHARINDEX emCIndex = _emCIndex;
	WORD wSchool = _wSchool;

	if ( emCIndex>=GLCI_NUM_8CLASS )			emCIndex = GLCI_BRAWLER_M;
	if ( wSchool>=GLCONST_CHAR::wSCHOOLNUM )	wSchool = 0;

	SCHARDATA2 &CharData2 = GLCONST_CHAR::GET_CHAR_DATA2 ( wSchool, emCIndex );

	SCHARDATA2::Assign ( CharData2 );
	m_cInventory.SetItemGenTime();

	m_dwUserID		= dwUserID;			// 유저번호.
	m_dwServerID	= dwServerID;		// 서버그룹.
	StringCchCopy ( m_szName, CHAR_SZNAME, szCharName );	// 캐릭터명.

	m_wSchool		= wSchool;
	m_wHair			= wHair;
	m_wFace			= wFace;
	m_wHairColor	= _wHairColor;
	m_wSex			= _wSex;
	m_fScaleRange	= _fScaleRange;

	INIT_DATA ( TRUE, TRUE );

	const GLCONST_CHARCLASS &cCONST = GLCONST_CHAR::cCONSTCLASS[emCIndex];

	if ( cCONST.dwHAIRNUM <= m_wHair )				m_wHair = 0;
	if ( cCONST.dwHEADNUM <= m_wFace )				m_wFace = 0;
	
	//	Note : 초기 시작 위치 설정. ( 학교에 따라 틀림. )
	//
	if ( GLCONST_CHAR::wSCHOOLNUM <= m_wSchool )	m_wSchool = 0;

	GLCONST_CHAR::nidSTARTMAP[m_wSchool];
	GLCONST_CHAR::dwSTARTGATE[m_wSchool];
}

const SCHARSTATS& GLCHARLOGIC_SERVER::GETSTATS_ADD () const
{
	static SCHARSTATS sSTATS_SUM;
	sSTATS_SUM = m_sStats + m_sSUMITEM.sStats;
	
	return sSTATS_SUM;
}

const SCHARSTATS& GLCHARLOGIC_SERVER::GETSTATS_ITEM () const
{
	return m_sSUMITEM.sStats;
}

void GLCHARLOGIC_SERVER::OptionMoveUp()
{
	m_fOPTION_MOVE += 0.1f;

	if( m_fOPTION_MOVE >= 3.0f )
		m_fOPTION_MOVE = 3.0f;
}

void GLCHARLOGIC_SERVER::OptionMoveDown()
{
	m_fOPTION_MOVE -= 0.1f;

	if( m_fOPTION_MOVE <= 0.0f )
		m_fOPTION_MOVE = 0.0f;
}

void GLCHARLOGIC_SERVER::SUM_ADDITION ( float fCONFT_POINT_RATE )
{
	int i=0;
	int nLEVEL = GETLEVEL();												//	케릭터의 래벨.
	EMCHARINDEX emCI = GETCHARINDEX ();										//	케릭터의 종류 인덱스.
	const GLCONST_CHARCLASS &cCHARCONST = GLCONST_CHAR::cCONSTCLASS[emCI];	//	케릭터의 상수들.
	int ZBLEVEL = (nLEVEL-1);												//	Zero base Level.

	//	Note : 페시브 스킬 가산값 합산.
	//
	SUM_PASSIVE ();

	//	Note : 아이템에 붙은 부가 효과들 합산.
	//
	SUM_ITEM ();

	//	Note : 기본 스텟 값 계산.
	//		STATS = 초기수치 + 래벨에 따른 자동증가수치 + 캐릭터 가용포인트로 분베된 수치 + 아이템 옵션.
	//
	m_sSUMSTATS.RESET();
	m_sSUMSTATS = cCHARCONST.sBEGIN_STATS + cCHARCONST.sLVLUP_STATS*ZBLEVEL + m_sStats + m_sSUMITEM.sStats;
	
	//	Note : 기공, 기방 계산.
	m_wSUM_AP = WORD ( ( cCHARCONST.wBEGIN_AP + cCHARCONST.fLVLUP_AP*ZBLEVEL ) * cCHARCONST.fCONV_AP );
	m_wSUM_DP = WORD ( ( cCHARCONST.wBEGIN_DP + cCHARCONST.fLVLUP_DP*ZBLEVEL ) * cCHARCONST.fCONV_DP );
	
	//	Note : 격투치, 사격치.
	m_wPA = WORD ( ( cCHARCONST.wBEGIN_PA + cCHARCONST.fLVLUP_PA*ZBLEVEL ) * cCHARCONST.fCONV_PA );
	m_wSA = WORD ( ( cCHARCONST.wBEGIN_SA + cCHARCONST.fLVLUP_SA*ZBLEVEL ) * cCHARCONST.fCONV_SA );

	//	Note : 격투치, 사격치, 마력치에 STATS 값 반영.
	m_wPA += WORD ( m_sSUMSTATS.wPow * cCHARCONST.fPA_POW + m_sSUMSTATS.wDex * cCHARCONST.fPA_DEX );
	m_wSA += WORD ( m_sSUMSTATS.wPow * cCHARCONST.fSA_POW + m_sSUMSTATS.wDex * cCHARCONST.fSA_DEX );
	m_wMA = WORD ( m_sSUMSTATS.wDex * cCHARCONST.fMA_DEX + m_sSUMSTATS.wSpi * cCHARCONST.fMA_SPI + m_sSUMSTATS.wInt * cCHARCONST.fMA_INT );

	//	Note : 격투치, 사격치, 마력치에 SUM_ITEM 값 반영.
	int nSUM_PA = m_sSUMITEM.nPA + m_sSUM_PASSIVE.m_nPA + m_dwMeleeIncrease; /* Codex - Jxyy 2024/7/28 */
	GLOGICEX::VARIATION ( m_wPA, USHRT_MAX, nSUM_PA );	// 변화되는 값을 0 <= x < 0xffff 까지로 제한.

	int nSUM_SA = m_sSUMITEM.nSA + m_sSUM_PASSIVE.m_nSA + m_dwShootingIncrease; /* Codex - Jxyy 2024/7/28 */
	GLOGICEX::VARIATION ( m_wSA, USHRT_MAX, nSUM_SA );	// 변화되는 값을 0 <= x < 0xffff 까지로 제한.

	int nSUM_MA = m_sSUMITEM.nMA + m_sSUM_PASSIVE.m_nMA + m_dwEnergyIncrease; /* Codex - Jxyy 2024/7/28 */
	GLOGICEX::VARIATION ( m_wMA, USHRT_MAX, nSUM_MA );	// 변화되는 값을 0 <= x < 0xffff 까지로 제한.

	//	Note : 합산 수치 설정.
	//
	m_wSUM_PA = m_wPA;
	m_wSUM_SA = m_wSA;
	m_wSUM_MA = m_wMA;

	//	Note : HP, MP, SP 총량 확정.
	//
	m_sHP.wMax = WORD( ( m_sSUMSTATS.wStr * cCHARCONST.fHP_STR + m_sSUMITEM.nHP + m_sSUM_PASSIVE.m_nHP ) );
	m_sHP.wMax = WORD( m_sHP.wMax * (1 + m_sSUM_PASSIVE.m_fHP_RATE) * fCONFT_POINT_RATE );
	m_sHP.wMax += m_dwHPIncrease; /* Codex - Jxyy 2024/7/28 */
	if (m_sHP.wMax < m_dwHPIncrease || m_sHP.wMax > USHRT_MAX) m_sHP.wMax = USHRT_MAX;
	// m_sHP.LIMIT();

	m_sMP.wMax = WORD( ( m_sSUMSTATS.wSpi * cCHARCONST.fMP_SPI + m_sSUMITEM.nMP + m_sSUM_PASSIVE.m_nMP ) );
	m_sMP.wMax = WORD( m_sMP.wMax * (1 + m_sSUM_PASSIVE.m_fMP_RATE) * fCONFT_POINT_RATE );
	m_sMP.wMax += m_dwMPIncrease; /* Codex - Jxyy 2024/7/28 */
	m_sMP.LIMIT();

	m_sSP.wMax = WORD( ( m_sSUMSTATS.wSta * cCHARCONST.fSP_STA + m_sSUMITEM.nSP + m_sSUM_PASSIVE.m_nSP ) );
	m_sSP.wMax = WORD( m_sSP.wMax * (1 + m_sSUM_PASSIVE.m_fSP_RATE) * fCONFT_POINT_RATE );
	m_sSP.wMax += m_dwSPIncrease; /* Codex - Jxyy 2024/7/28 */
	m_sSP.LIMIT();

	/*combatpoint logic, Juver, 2017/05/28 */
	/*item max CP, Juver, 2018/07/09 */
	m_sCombatPoint.wMax = WORD (GLCONST_CHAR::GETCPMAX(m_dwReborn) + m_sSUMITEM.wadd_max_cp );
	m_sCombatPoint.LIMIT();
	
	//	Note : 명중율, 회피율 계산.
	m_nHIT = int ( m_sSUMSTATS.wDex*cCHARCONST.fHIT_DEX + m_sSUMITEM.nHitRate + m_sSUM_PASSIVE.m_nHIT + m_dwHitrateIncrease ); /* Codex - Jxyy 2024/7/28 */
	m_nAVOID = int ( m_sSUMSTATS.wDex*cCHARCONST.fAVOID_DEX + m_sSUMITEM.nAvoidRate + m_sSUM_PASSIVE.m_nAVOID + m_dwAvoidrateIncrease ); /* Codex - Jxyy 2024/7/28 */

	m_nDEFENSE_BODY = int ( m_wSUM_DP + m_sSUMSTATS.wDex*cCHARCONST.fDEFENSE_DEX );

	//	Note : 방어력 ( DP + POW*계수 + ITEM_DEF ) * 공간적응 보정.
	m_nDEFENSE_SKILL = m_nDEFENSE = int ( m_nDEFENSE_BODY + m_sSUMITEM.nDefense + m_sSUM_PASSIVE.m_nDEFENSE  + m_dwDefenseIncrease ); /* Codex - Jxyy 2024/7/28 */

	//	Note : 공격력.
	m_gdDAMAGE.wMax = m_gdDAMAGE.wLow = int ( m_wSUM_AP + m_sSUM_PASSIVE.m_nDAMAGE + m_dwAttackIncrease ); /* Codex - Jxyy 2024/7/28 */
	m_gdDAMAGE_SKILL = m_gdDAMAGE;

	m_gdDAMAGE_PHYSIC = m_gdDAMAGE_SKILL;
	m_gdDAMAGE_PHYSIC.wLow += m_sSUMITEM.gdDamage.wLow;
	m_gdDAMAGE_PHYSIC.wMax += m_sSUMITEM.gdDamage.wMax;

	if ( ISLONGRANGE_ARMS() )	m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_SA );	//	장거리 공격.
	else						m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_PA );	//	근접 공격.

	//	Note : 저항력. ( 현제는 아이탬에만 영향을 받음. )
	m_sSUMRESIST.RESET();
	m_sSUMRESIST = m_sSUM_PASSIVE.m_sSUMRESIST + m_sSUMITEM.sResist + m_dwResistanceIncrease; // Codex - JX

	//	Note : HP, MP, SP 회복율 산출.
	m_fINCR_HP = GLCONST_CHAR::fHP_INC_PER + m_sSUMITEM.fIncR_HP + m_sSUM_PASSIVE.m_fINCR_HP;
	m_fINCR_MP = GLCONST_CHAR::fMP_INC_PER + m_sSUMITEM.fIncR_MP + m_sSUM_PASSIVE.m_fINCR_MP;
	m_fINCR_SP = GLCONST_CHAR::fSP_INC_PER + m_sSUMITEM.fIncR_SP + m_sSUM_PASSIVE.m_fINCR_SP;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	if ( m_pITEMS[emRHand] )
	{
		m_emITEM_ATT = m_pITEMS[emRHand]->sSuitOp.emAttack;
		m_wATTRANGE = m_pITEMS[emRHand]->sSuitOp.wAttRange;
	}
	else
	{
		m_emITEM_ATT = ITEMATT_NOTHING;
		m_wATTRANGE = GLCONST_CHAR::wMAXATRANGE_SHORT;
	}

	//	Note : 모자라는 수치들 합산. ( STATS, LEVEL )
	//
	m_wACCEPTP = 0;
	m_wACCEPTP += CALC_ACCEPTP ( GET_SLOT_NID(emLHand) );
	m_wACCEPTP += CALC_ACCEPTP ( GET_SLOT_NID(emRHand) );

	//	Note : "SP소비로 충당되는 요구수치" + "아이템 사용시 소비 SP 합산"
	//
	m_wSUM_DisSP = m_wACCEPTP;

	SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
	SITEM* pLHAND = GET_SLOT_ITEMDATA ( emLHand );

	if ( pRHAND )	m_wSUM_DisSP += pRHAND->sSuitOp.wReqSP;
	if ( pLHAND )	m_wSUM_DisSP += pLHAND->sSuitOp.wReqSP;

	//m_sHP.LIMIT ();
	m_sMP.LIMIT ();
	m_sSP.LIMIT ();
}

void GLCHARLOGIC_SERVER::SUM_ITEM ()
{
	m_sSUMITEM.RESET();

	for ( int i=0; i<SLOT_NSIZE_S_2; i++ )
	{
		EMSLOT emSLOT = static_cast<EMSLOT>(i);
		if ( !VALID_SLOT_ITEM(emSLOT) )					continue;

		const SITEMCUSTOM& sItemCustom = GET_SLOT_ITEM ( emSLOT );

		SITEM &sItem = *m_pITEMS[emSLOT];

		//	부적은 물리 공격에는 가산이 되지 않음.
		//if ( sItem.sBasicOp.emItemType==ITEM_CHARM )	continue;

		if ( RPARAM::bCheckWearUse )
		{
			EMCHECK_WEAR emCheckWear = CHECK_WEAR_ITEM_SLOT( sItem.sBasicOp.sNativeID, emSLOT );
			if ( emCheckWear != EMCHECK_WEAR_PASS )		continue;
		}
		
		for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
		{
			switch ( sItem.sSuitOp.sADDON[addon].emTYPE )
			{
			case EMADD_NONE:
				break;
			case EMADD_HITRATE:
				m_sSUMITEM.nHitRate		+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_AVOIDRATE:
				m_sSUMITEM.nAvoidRate	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_DAMAGE:
				m_sSUMITEM.gdDamage.wLow+= sItem.sSuitOp.sADDON[addon].nVALUE;
				m_sSUMITEM.gdDamage.wMax+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_DEFENSE:
				m_sSUMITEM.nDefense		+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_HP:
				m_sSUMITEM.nHP			+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_MP:
				m_sSUMITEM.nMP			+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_SP:
				m_sSUMITEM.nSP			+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_STATS_POW:
				m_sSUMITEM.sStats.wPow	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_STR:
				m_sSUMITEM.sStats.wStr	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_SPI:
				m_sSUMITEM.sStats.wSpi	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_DEX:
				m_sSUMITEM.sStats.wDex	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_INT:
				m_sSUMITEM.sStats.wInt	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_STA:
				m_sSUMITEM.sStats.wSta	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

				/*item max CP, Juver, 2018/07/09 */
			case EMADD_MAX_CP:
				m_sSUMITEM.wadd_max_cp	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
				
			};
		}

		m_sSUMITEM.nHP += (int) sItemCustom.GETOptVALUE(EMR_OPT_HP);
		m_sSUMITEM.nMP += (int) sItemCustom.GETOptVALUE(EMR_OPT_MP);
		m_sSUMITEM.nSP += (int) sItemCustom.GETOptVALUE(EMR_OPT_SP);

		//	Note : 변화율 효과.
		switch ( sItem.sSuitOp.sVARIATE.emTYPE )
		{
		case EMVAR_HP:
			m_sSUMITEM.fIncR_HP += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_MP:
			m_sSUMITEM.fIncR_MP += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_SP:
			m_sSUMITEM.fIncR_SP += sItem.sSuitOp.sVARIATE.fVariate;
			break;

		case EMVAR_AP:
			m_sSUMITEM.fIncR_HP += sItem.sSuitOp.sVARIATE.fVariate;
			m_sSUMITEM.fIncR_MP += sItem.sSuitOp.sVARIATE.fVariate;
			m_sSUMITEM.fIncR_SP += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_MOVE_SPEED:
			// 나중에 합산으로 변경
			if ( emSLOT != SLOT_VEHICLE ) m_sSUMITEM.fIncR_MoveSpeed += sItemCustom.GETMOVESPEEDR();
			else if ( m_bVehicle ) m_sSUMITEM.fIncR_MoveSpeed += m_fVehicleSpeedRate + sItemCustom.GETMOVESPEEDR();;
			break;
		case EMVAR_ATTACK_SPEED:
			m_sSUMITEM.fIncR_AtkSpeed	+= sItem.sSuitOp.sVARIATE.fVariate;	
			break;
		case EMVAR_CRITICAL_RATE:
			m_sSUMITEM.fIncR_Critical += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_CRUSHING_BLOW:
			m_sSUMITEM.fIncR_CrushingBlow += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		};

		m_sSUMITEM.fIncR_HP += sItemCustom.GETOptVALUE(EMR_OPT_HP_INC) + sItemCustom.GETOptVALUE(EMR_OPT_HMS_INC);
		m_sSUMITEM.fIncR_MP += sItemCustom.GETOptVALUE(EMR_OPT_MP_INC) + sItemCustom.GETOptVALUE(EMR_OPT_HMS_INC);
		m_sSUMITEM.fIncR_SP += sItemCustom.GETOptVALUE(EMR_OPT_SP_INC) + sItemCustom.GETOptVALUE(EMR_OPT_HMS_INC);

		//	Note : 변화량 효과.
		switch ( sItem.sSuitOp.sVOLUME.emTYPE )
		{
		case EMVAR_HP:
			m_sSUMITEM.fInc_HP += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_MP:
			m_sSUMITEM.fInc_MP += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_SP:
			m_sSUMITEM.fInc_SP += sItem.sSuitOp.sVOLUME.fVolume;
			break;

		case EMVAR_AP:
			m_sSUMITEM.fInc_HP += sItem.sSuitOp.sVOLUME.fVolume;
			m_sSUMITEM.fInc_MP += sItem.sSuitOp.sVOLUME.fVolume;
			m_sSUMITEM.fInc_SP += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_MOVE_SPEED:
			// 나중에 합산으로 변경
			if ( emSLOT != SLOT_VEHICLE ) m_sSUMITEM.fInc_MoveSpeed += sItemCustom.GETMOVESPEED();
			else if	( m_bVehicle ) m_sSUMITEM.fInc_MoveSpeed += m_fVehicleSpeedVol + sItemCustom.GETMOVESPEED();
			break;
		case EMVAR_ATTACK_SPEED:
			m_sSUMITEM.fInc_AtkSpeed	+= sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_CRITICAL_RATE:
			m_sSUMITEM.fInc_Critical += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_CRUSHING_BLOW:
			m_sSUMITEM.fInc_CrushingBlow += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		};

		// 이동속도 랜덤옵션 적용( 파츠는 위에서 적용 m_fVehicleSpeedVol ) 
//		if ( emSLOT != SLOT_VEHICLE ) m_sSUMITEM.fInc_MoveSpeed += sItemCustom.GETOptVALUE( EMR_OPT_MOVE_SPEED );
//		else if ( m_bVehicle ) m_sSUMITEM.fInc_MoveSpeed += sItemCustom.GETOptVALUE( EMR_OPT_MOVE_SPEED );

		//	Note : 아이템 기본 능력 ADD 수치.
		//
		m_sSUMITEM.gdDamage			+= sItemCustom.GETDAMAGE();
		m_sSUMITEM.nDefense			+= sItemCustom.GETDEFENSE();

		m_sSUMITEM.nAvoidRate		+= sItemCustom.GETAVOIDRATE();
		m_sSUMITEM.nHitRate			+= sItemCustom.GETHITRATE();

		/*official random value, Juver, 2018/06/09 */
		m_sSUMITEM.sStats.wPow	+= sItemCustom.GET_STATS_POW();
		m_sSUMITEM.sStats.wStr	+= sItemCustom.GET_STATS_STR();
		m_sSUMITEM.sStats.wSpi	+= sItemCustom.GET_STATS_SPI();
		m_sSUMITEM.sStats.wDex	+= sItemCustom.GET_STATS_DEX();
		m_sSUMITEM.sStats.wSta	+= sItemCustom.GET_STATS_STA();

		//	 Note : 연마로 인한 [+] 수치 합산.
		//
		m_sSUMITEM.sResist.nElectric	+= sItemCustom.GETRESIST_ELEC();
		m_sSUMITEM.sResist.nFire		+= sItemCustom.GETRESIST_FIRE();
		m_sSUMITEM.sResist.nIce			+= sItemCustom.GETRESIST_ICE();
		m_sSUMITEM.sResist.nPoison		+= sItemCustom.GETRESIST_POISON();
		m_sSUMITEM.sResist.nSpirit		+= sItemCustom.GETRESIST_SPIRIT();

		//	Note : 기력치에 공격력 가산.
		//
		m_sSUMITEM.nMA	+= sItemCustom.GETMaDAMAGE();

		/*official random value, Juver, 2018/06/11 */
		m_sSUMITEM.nPA	+= sItemCustom.get_pa_damage();
		m_sSUMITEM.nSA	+= sItemCustom.get_sa_damage();

		m_sSUMITEM.nadd_gain_hp += (int)sItemCustom.get_potion_hp();
		m_sSUMITEM.nadd_gain_mp += (int)sItemCustom.get_potion_mp();
		m_sSUMITEM.nadd_gain_sp += (int)sItemCustom.get_potion_sp();
		m_sSUMITEM.nadd_gain_cp += (int)sItemCustom.get_potion_cp();

		/*additional RV, Juver, 2018/06/14 */
		m_sSUMITEM.fadd_movement_speed += sItemCustom.get_ex_move_speed();
		m_sSUMITEM.fadd_attack_speed += sItemCustom.get_ex_attack_speed();

		m_sSUMITEM.fadd_critical_hit_chance += sItemCustom.get_ex_critical_hit_chance();
		m_sSUMITEM.fadd_critical_hit_damage += sItemCustom.get_ex_critical_hit_damage();
		m_sSUMITEM.fadd_crushing_blow_chance += sItemCustom.get_ex_crushing_blow_chance();
		m_sSUMITEM.fadd_crushing_blow_damage += sItemCustom.get_ex_crushing_blow_damage();

		/*item max CP, Juver, 2018/07/09 */
		m_sSUMITEM.wadd_max_cp += sItemCustom.get_max_cp();

		float fExpMultipleRate = sItem.GetExpMultiple();
		if(  1.0f < fExpMultipleRate )
			m_sSUMITEM.fExpRate += fExpMultipleRate - 1.0f;

		/*costume combine stats, Juver, 2017/09/01 */
		//suit type check
		SITEM*	pItemDisguiseCombine = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
		if ( pItemDisguiseCombine && pItemDisguiseCombine->sBasicOp.IsDISGUISE() && pItemDisguiseCombine->sSuitOp.emSuit == sItem.sSuitOp.emSuit )
		{
			for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
			{
				switch ( pItemDisguiseCombine->sSuitOp.sADDON[addon].emTYPE )
				{
				case EMADD_NONE:
					break;
				case EMADD_HITRATE:
					m_sSUMITEM.nHitRate		+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_AVOIDRATE:
					m_sSUMITEM.nAvoidRate	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_DAMAGE:
					m_sSUMITEM.gdDamage.wLow+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					m_sSUMITEM.gdDamage.wMax+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_DEFENSE:
					m_sSUMITEM.nDefense		+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_HP:
					m_sSUMITEM.nHP			+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_MP:
					m_sSUMITEM.nMP			+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_SP:
					m_sSUMITEM.nSP			+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_STATS_POW:
					m_sSUMITEM.sStats.wPow	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_STATS_STR:
					m_sSUMITEM.sStats.wStr	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_STATS_SPI:
					m_sSUMITEM.sStats.wSpi	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_STATS_DEX:
					m_sSUMITEM.sStats.wDex	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_STATS_INT:
					m_sSUMITEM.sStats.wInt	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				case EMADD_STATS_STA:
					m_sSUMITEM.sStats.wSta	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;

					/*item max CP, Juver, 2018/07/09 */
				case EMADD_MAX_CP:
					m_sSUMITEM.wadd_max_cp	+= pItemDisguiseCombine->sSuitOp.sADDON[addon].nVALUE;
					break;
				};
			}

			switch ( pItemDisguiseCombine->sSuitOp.sVARIATE.emTYPE )
			{
			case EMVAR_HP:
				m_sSUMITEM.fIncR_HP += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				break;
			case EMVAR_MP:
				m_sSUMITEM.fIncR_MP += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				break;
			case EMVAR_SP:
				m_sSUMITEM.fIncR_SP += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				break;
			case EMVAR_AP:
				m_sSUMITEM.fIncR_HP += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				m_sSUMITEM.fIncR_MP += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				m_sSUMITEM.fIncR_SP += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				break;

			case EMVAR_MOVE_SPEED:
				{
					/* costume combine map restriction, Juver, 2020/09/14 */
					if ( !m_bCostumeCombineRestrictMoveSpeed )
						m_sSUMITEM.fIncR_MoveSpeed += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				}break;

			case EMVAR_ATTACK_SPEED:
				{
					/* costume combine map restriction, Juver, 2020/09/14 */
					if ( !m_bCostumeCombineRestrictAttackSpeed )
						m_sSUMITEM.fIncR_AtkSpeed	+= pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;	
				}break;

			case EMVAR_CRITICAL_RATE:
				m_sSUMITEM.fIncR_Critical += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				break;
			case EMVAR_CRUSHING_BLOW:
				m_sSUMITEM.fIncR_CrushingBlow += pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
				break;
			};

			switch ( pItemDisguiseCombine->sSuitOp.sVOLUME.emTYPE )
			{
			case EMVAR_HP:
				m_sSUMITEM.fInc_HP += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				break;
			case EMVAR_MP:
				m_sSUMITEM.fInc_MP += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				break;
			case EMVAR_SP:
				m_sSUMITEM.fInc_SP += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				break;
			case EMVAR_AP:
				m_sSUMITEM.fInc_HP += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				m_sSUMITEM.fInc_MP += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				m_sSUMITEM.fInc_SP += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				break;

			case EMVAR_MOVE_SPEED:
				{
					/* costume combine map restriction, Juver, 2020/09/14 */
					if ( !m_bCostumeCombineRestrictMoveSpeed )
						m_sSUMITEM.fInc_MoveSpeed += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				}break;

			case EMVAR_ATTACK_SPEED:
				{
					/* costume combine map restriction, Juver, 2020/09/14 */
					if ( !m_bCostumeCombineRestrictAttackSpeed )
						m_sSUMITEM.fInc_AtkSpeed	+= pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				}break;

			case EMVAR_CRITICAL_RATE:
				m_sSUMITEM.fInc_Critical += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				break;
			case EMVAR_CRUSHING_BLOW:
				m_sSUMITEM.fInc_CrushingBlow += pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
				break;
			};


			float fExpMultipleRate = pItemDisguiseCombine->GetExpMultiple();
			if(  1.0f < fExpMultipleRate )
				m_sSUMITEM.fExpRate += fExpMultipleRate - 1.0f;
		}


	}


}

void GLCHARLOGIC_SERVER::SUM_PASSIVE ()
{
	m_sSUM_PASSIVE = SPASSIVE_SKILL_DATA_SERVER();
	
	if ( m_bVehicle ) return;

	if ( m_ExpSkills.empty() ) return;

	SKILL_MAP_ITER iter = m_ExpSkills.begin ();
	SKILL_MAP_ITER iter_end = m_ExpSkills.end ();
	for ( ; iter!=iter_end; ++iter )
	{
		const SCHARSKILL &sCharSkill = (*iter).second;
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sCharSkill.sNativeID.wMainID, sCharSkill.sNativeID.wSubID );
		if ( !pSkill )											continue;
		if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE )	continue;

		//	Note : 아이템 착용 조건 검사.
		//
		bool bvalid_left(true), bvalid_right(true);
		const bool bHiddenWeapon	= pSkill->m_sLEARN.bHiddenWeapon;
		GLITEM_ATT emITEM_LEFT = ITEMATT_NOTHING;
		GLITEM_ATT emITEM_RIGHT = ITEMATT_NOTHING;
		GLSKILL_ATT emSKILL_LEFT = pSkill->m_sBASIC.emUSE_LITEM;
		GLSKILL_ATT emSKILL_RIGHT = pSkill->m_sBASIC.emUSE_RITEM;

		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		if ( emSKILL_LEFT!=SKILLATT_NOCARE )
		{
			bvalid_left = false;
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem )
			{
				emITEM_LEFT = pItem->sSuitOp.emAttack;
				bvalid_left = CHECHSKILL_ITEM(emSKILL_LEFT,emITEM_LEFT,bHiddenWeapon);
			}
		}

		if ( emSKILL_RIGHT!=SKILLATT_NOCARE )
		{
			bvalid_right = false;
			SITEM* pItem = GET_SLOT_ITEMDATA(emRHand);
			if ( pItem )
			{
				emITEM_RIGHT = pItem->sSuitOp.emAttack;
				bvalid_right = CHECHSKILL_ITEM(emSKILL_RIGHT,emITEM_RIGHT,bHiddenWeapon);
			}
		}

		if ( !(bvalid_left&&bvalid_right) )						continue;

		const SKILL::CDATA_LVL &sDATA_LVL = pSkill->m_sAPPLY.sDATA_LVL[sCharSkill.wLevel];
		
		

		switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
		{
		case SKILL::EMFOR_HP:
			m_sSUM_PASSIVE.m_nHP += WORD ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_MP:
			m_sSUM_PASSIVE.m_nMP += WORD ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_SP:
			m_sSUM_PASSIVE.m_nSP += WORD ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_VARHP:
			m_sSUM_PASSIVE.m_fINCR_HP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_VARMP:
			m_sSUM_PASSIVE.m_fINCR_MP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_VARSP:
			m_sSUM_PASSIVE.m_fINCR_SP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_DEFENSE:
			m_sSUM_PASSIVE.m_nDEFENSE += int ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_HITRATE:
			m_sSUM_PASSIVE.m_nHIT += int ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_AVOIDRATE:
			m_sSUM_PASSIVE.m_nAVOID += int ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_VARAP:
			m_sSUM_PASSIVE.m_fINCR_HP += sDATA_LVL.fBASIC_VAR;
			m_sSUM_PASSIVE.m_fINCR_MP += sDATA_LVL.fBASIC_VAR;
			m_sSUM_PASSIVE.m_fINCR_SP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_VARDAMAGE:
			m_sSUM_PASSIVE.m_nDAMAGE += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_VARDEFENSE:
			m_sSUM_PASSIVE.m_nDEFENSE += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_PA:
			m_sSUM_PASSIVE.m_nPA += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_SA:
			m_sSUM_PASSIVE.m_nSA += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_MA:
			m_sSUM_PASSIVE.m_nMA += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_HP_RATE:
			m_sSUM_PASSIVE.m_fHP_RATE += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_MP_RATE:
			m_sSUM_PASSIVE.m_fMP_RATE += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_SP_RATE:
			m_sSUM_PASSIVE.m_fSP_RATE += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_RESIST:
			m_sSUM_PASSIVE.m_sSUMRESIST += (int)(sDATA_LVL.fBASIC_VAR);
			break;

			/*summon time, Juver, 2017/12/12 */
		case SKILL::EMFOR_SUMMONTIME:
			m_sSUM_PASSIVE.m_nSummonTime += int ( sDATA_LVL.fBASIC_VAR );
			break;
		};

		for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
		{
			const float &fADDON = pSkill->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[sCharSkill.wLevel];

			switch ( pSkill->m_sAPPLY.sImpacts[nImpact].emADDON )
			{
			case EMIMPACTA_HITRATE:
				m_sSUM_PASSIVE.m_nHIT += int ( fADDON );
				break;
			case EMIMPACTA_AVOIDRATE:
				m_sSUM_PASSIVE.m_nAVOID += int ( fADDON );
				break;

			case EMIMPACTA_DAMAGE:
				m_sSUM_PASSIVE.m_nDAMAGE += int ( fADDON );
				break;

			case EMIMPACTA_DEFENSE:
				m_sSUM_PASSIVE.m_nDEFENSE += int ( fADDON );
				break;

			case EMIMPACTA_VARHP:
				m_sSUM_PASSIVE.m_fINCR_HP += fADDON;
				break;
			case EMIMPACTA_VARMP:
				m_sSUM_PASSIVE.m_fINCR_MP += fADDON;
				break;
			case EMIMPACTA_VARSP:
				m_sSUM_PASSIVE.m_fINCR_SP += fADDON;
				break;

			case EMIMPACTA_VARAP:
				m_sSUM_PASSIVE.m_fINCR_HP += fADDON;
				m_sSUM_PASSIVE.m_fINCR_MP += fADDON;
				m_sSUM_PASSIVE.m_fINCR_SP += fADDON;
				break;

			case EMIMPACTA_DAMAGE_RATE:
				m_sSUM_PASSIVE.m_fDAMAGE_RATE += fADDON;
				break;

			case EMIMPACTA_DEFENSE_RATE:
				m_sSUM_PASSIVE.m_fDEFENSE_RATE += fADDON;
				break;

			case EMIMPACTA_PA:
				m_sSUM_PASSIVE.m_nPA += int(fADDON);
				break;

			case EMIMPACTA_SA:
				m_sSUM_PASSIVE.m_nSA += int(fADDON);
				break;

			case EMIMPACTA_MA:
				m_sSUM_PASSIVE.m_nMA += int(fADDON);
				break;

			case EMIMPACTA_HP_RATE:
				m_sSUM_PASSIVE.m_fHP_RATE += fADDON;
				break;

			case EMIMPACTA_MP_RATE:
				m_sSUM_PASSIVE.m_fMP_RATE += fADDON;
				break;

			case EMIMPACTA_SP_RATE:
				m_sSUM_PASSIVE.m_fSP_RATE += fADDON;
				break;

			case EMIMPACTA_RESIST:
				m_sSUM_PASSIVE.m_sSUMRESIST += (int)fADDON;
				break;


			};
		}
		

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			const SKILL::SSPEC &sSPEC = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[sCharSkill.wLevel];

			switch ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC )
			{
			case EMSPECA_PIERCE:
				m_sSUM_PASSIVE.m_nPIERCE += int ( sSPEC.fVAR1 );
				break;

			case EMSPECA_TARRANGE:
				m_sSUM_PASSIVE.m_fTARGET_RANGE += sSPEC.fVAR1;
				break;

			case EMSPECA_MOVEVELO:
				m_sSUM_PASSIVE.m_fMOVEVELO += sSPEC.fVAR1;
				break;

			case EMSPECA_ATTACKVELO:
				//	공격 속도 계념이므로 ( 공격 시간을 -0.1 (-10%) 감소 시키기 위해서는 부호 반전을 하여야함. )
				m_sSUM_PASSIVE.m_fATTVELO -= sSPEC.fVAR1;
				break;

				/* skill delay addition, Juver, 2020/12/08 */
			case EMSPECA_SKILLDELAY:
				{
					m_sSUM_PASSIVE.m_fSKILLDELAY += sSPEC.fVAR1;
				}break;

			case EMSPECA_PSY_DAMAGE_REDUCE:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReduce < sSPEC.fVAR1 )
					m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReduce = sSPEC.fVAR1;
				break;

			case EMSPECA_MAGIC_DAMAGE_REDUCE:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReduce < sSPEC.fVAR1 )
					m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReduce = sSPEC.fVAR1;
				break;

			case EMSPECA_PSY_DAMAGE_REFLECTION:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReflection < sSPEC.fVAR1 )
				{
					m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReflection = sSPEC.fVAR1;
					m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReflectionRate = sSPEC.fVAR2;
				}
				break;

			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReflection < sSPEC.fVAR1 )
				{
					m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReflection = sSPEC.fVAR1;
					m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReflectionRate = sSPEC.fVAR2;
				}
				break;
			case EMSPECA_DEFENSE_SKILL_ACTIVE:
				{				
				}
				break;

				/*skill range spec logic, Juver, 2017/06/06 */
			case EMSPECA_CHANGE_ATTACK_RANGE:	
				{
					m_sSUM_PASSIVE.m_fATTACK_RANGE += sSPEC.fVAR1;	
				}break;

			case EMSPECA_CHANGE_APPLY_RANGE:	
				{
					m_sSUM_PASSIVE.m_fAPPLY_RANGE += sSPEC.fVAR1;	
				}break;
			};
		}

		
	}
}

void GLCHARLOGIC_SERVER::INIT_RECOVER ( int nRECOVER )
{
	m_sHP.CHECKMIN ( (m_sHP.wMax*nRECOVER)/100 + 1 );
	m_sMP.CHECKMIN ( (m_sMP.wMax*nRECOVER)/100 + 1 );
	m_sSP.CHECKMIN ( (m_sSP.wMax*nRECOVER)/100 + 1 );

	for ( int i=0; i<EMBLOW_MULTI; ++i )		DISABLEBLOW ( i );
	for ( int i=0; i<SKILLFACT_SIZE; ++i )		DISABLESKEFF ( i );

	/*dual pet skill, Juver, 2017/12/27 */
	m_sPETSKILLFACT_A.RESET ();
	m_sPETSKILLFACT_B.RESET ();

	DISABLEALLLANDEFF();
}

void GLCHARLOGIC_SERVER::RELEASE_SLOT_ITEM ( EMSLOT _slot )
{
	m_PutOnItems[_slot] = SITEMCUSTOM ( SNATIVEID(false) );
	
	//	Note : 아이템 포인터 초기화.
	//
	m_pITEMS[_slot] = NULL;
}

void GLCHARLOGIC_SERVER::SLOT_ITEM ( const SITEMCUSTOM &sItemCustom, EMSLOT _slot )
{
	m_PutOnItems[_slot] = sItemCustom;

	//	Note : 아이템 포인터 초기화.
	//
	if ( VALID_SLOT_ITEM(_slot) )
		m_pITEMS[_slot] =  GLItemMan::GetInstance().GetItem ( m_PutOnItems[_slot].sNativeID );
}

void GLCHARLOGIC_SERVER::HOLD_ITEM ( const SITEMCUSTOM &sItemCustom )
{
	m_PutOnItems[SLOT_HOLD] = sItemCustom;

	//	Note : 아이템 포인터 초기화.
	//
	if ( VALID_SLOT_ITEM(SLOT_HOLD) )
		m_pITEMS[SLOT_HOLD] =  GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
}

void GLCHARLOGIC_SERVER::RELEASE_HOLD_ITEM ()
{
	m_PutOnItems[SLOT_HOLD] = SITEMCUSTOM ( SNATIVEID(false) );

	//	Note : 아이템 포인터 초기화.
	//
	m_pITEMS[SLOT_HOLD] = NULL;
}


//	왼손, 오른손 아이템중에서 'BLOW' 속성이 있는 아이템을 반환.
SITEM* GLCHARLOGIC_SERVER::GET_ELMT_ITEM ()
{
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	SITEM* pRHandItem = GET_SLOT_ITEMDATA(emRHand);
	SITEM* pLHandItem = GET_SLOT_ITEMDATA(emLHand);

	if ( pRHandItem && pRHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		return pRHandItem;
	}
	else if ( pLHandItem && pLHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		return pLHandItem;
	}

	return NULL;
}

BOOL GLCHARLOGIC_SERVER::INIT_DATA ( BOOL bNEW, BOOL bReGen, float fCONFT_POINT_RATE, bool bInitNowExp )
{
	int i = 0;
	m_CHARINDEX = GETCHARINDEX ();
	const GLCONST_CHARCLASS &cCHARCONST = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];

	//	Note : 아이템 포인터 가져오기.
	//
	memset ( m_pITEMS, 0x00, sizeof(SITEM*)*SLOT_TSIZE );
	for ( int i=0; i<SLOT_TSIZE; i++ )
	{
		if ( m_PutOnItems[i].sNativeID != NATIVEID_NULL() )
		{
			m_pITEMS[i] = GLItemMan::GetInstance().GetItem ( m_PutOnItems[i].sNativeID );

			//	아이템 테이블에 없는 아이템은 삭제.
			if ( !m_pITEMS[i] )		RELEASE_SLOT_ITEM ( EMSLOT(i) );
		}
	}

	//	Note : ADD 값 계산.
	SUM_ADDITION( fCONFT_POINT_RATE );

	if ( bNEW )
	{
		m_bServerStorage = TRUE;

		//m_wStatsPoint = GLCONST_CHAR::wLVL_STATS_P;

		m_sHP.TO_FULL ();
		m_sMP.TO_FULL ();
		m_sSP.TO_FULL ();
	}
	else if ( bReGen )
	{
		INIT_RECOVER ();
	}

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	/*skill transform, Juver, 2018/09/09 */
	m_emANISUBTYPE = CHECK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand], m_skill_transform_data.is_skin_change_valid() );

	GLITEM_ATT emRHAtt = ITEMATT_NOTHING;
	GLITEM_ATT emLHAtt = ITEMATT_NOTHING;

	if ( m_pITEMS[emRHand] )		emRHAtt = m_pITEMS[emRHand]->sSuitOp.emAttack;
	if ( m_pITEMS[emLHand] )		emLHAtt = m_pITEMS[emLHand]->sSuitOp.emAttack;

	if ( emRHAtt==ITEMATT_NOTHING )		m_wATTRANGE = GLCONST_CHAR::wMAXATRANGE_SHORT;
	else								m_wATTRANGE = m_pITEMS[emRHand]->sSuitOp.wAttRange;

	//	현재 레벨에서 보유가능한 최대 경험치 계산.
	m_sExperience.lnMax = GET_LEVELUP_EXP();
	if( bInitNowExp ) m_sExperience.LIMIT();
	
	return TRUE;
}

BOOL GLCHARLOGIC_SERVER::CHECKHIT ( const STARGETID &cTargetID, const GLLandMan* pLandMan, const BOOL bLowSP )
{
	int nAVOID = 0;
	EMBRIGHT emBright;

	if ( cTargetID.dwID == EMTARGET_NULL )	return FALSE;

	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
	if ( !pActor )						return FALSE;
	
	nAVOID = pActor->GetAvoid ();
	emBright = pActor->GetBright ();

	EM_BRIGHT_FB bFB = GLOGICEX::GLSPACEGAP ( GETBRIGHT(), emBright, pLandMan->GETBRIGHT() );	
	int nHitRate = GLOGICEX::GLHITRATE ( GETHIT(), nAVOID, bFB );
	if ( bLowSP )		nHitRate = int(nHitRate*(1.0f-GLCONST_CHAR::fLOWSP_HIT_DROP));

	return ( nHitRate >= (RANDOM_POS*100) );
}

DWORD GLCHARLOGIC_SERVER::CALCDAMAGE(
	int &rResultDAMAGE,
	const DWORD dwGaeaID,
	const STARGETID &cTargetID,
	const GLLandMan* pLandMan,
	const GLSKILL* pSkill,
	const DWORD dwskill_lev,
	const DWORD dwWeatherFlag,
	const int dwDivCount)
{
#if defined(KRT_PARAM) || defined(KR_PARAM) || defined(CH_PARAM) || defined(TH_PARAM) || defined(ID_PARAM) || defined(JP_PARAM) || defined(MY_PARAM) || defined(MYE_PARAM) || defined(PH_PARAM) || defined ( GS_PARAM ) || defined(_RELEASED)
	return CALCDAMAGE_20060328(
		rResultDAMAGE,
		dwGaeaID,
		cTargetID,
		pLandMan,
		pSkill,
		dwskill_lev,
		dwWeatherFlag,
		dwDivCount);
#else
	return CALCDAMAGE_2004(
		rResultDAMAGE,
		dwGaeaID,
		cTargetID,
		pLandMan,
		pSkill,
		dwskill_lev,
		dwWeatherFlag,
		dwDivCount);
#endif
}

// 2006-03-28 Jgkim 기획팀 요청으로 데미지 공식 변경함
DWORD GLCHARLOGIC_SERVER::CALCDAMAGE_20060328(
	int& rResultDAMAGE,
	const DWORD dwGaeaID,
	const STARGETID &cTargetID,
	const GLLandMan* pLandMan,
	const GLSKILL* pSkill,
	const DWORD dwskill_lev,
	const DWORD dwWeatherFlag,
	const int dwDivCount)
{
	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
	if ( !pActor )	return DAMAGE_TYPE_NONE;

	/*ignore damage, Juver, 2017/12/12 */
	if ( pActor->IsIgnoreDamage() )
	{
		rResultDAMAGE = 1;
		return DAMAGE_TYPE_NONE;
	}

	int nDEFENSE = pActor->GetDefense ();
	int nDEFAULT_DEFENSE = pActor->GetBodyDefense ();
	int nITEM_DEFENSE = pActor->GetItemDefense ();

	int nLEVEL = pActor->GetLevel ();
	float fSTATE_DAMAGE = pActor->GETSTATE_DAMAGE ();
	const SRESIST &sRESIST = pActor->GETRESIST (); // 상대의 방어구 저항값

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	WORD wGRADE = 0;
	const SITEMCUSTOM &sRHAND = GET_SLOT_ITEM(emRHand);
	if ( sRHAND.sNativeID != SNATIVEID(false) )
		wGRADE = sRHAND.GETGRADE(EMGRINDING_DAMAGE);

	int nCrushingBlow = (int)( m_sSUMITEM.fIncR_CrushingBlow * 100 );
	nCrushingBlow += (int)( m_sSUMITEM.fInc_CrushingBlow );

	DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
	bool bShock = false;
	bool bCrushingBlow = false;
	bool bCritical = false; 
	bool bPsyDamage = true;

	const DAMAGE_SPEC& sDamageSpec = pActor->GetDamageSpec();
	DAMAGE_SPEC* pDamageSpec = pActor->GetDamageSpecPtr();
	float fDamageReduce = sDamageSpec.m_fPsyDamageReduce;
	float fDamageReflection = sDamageSpec.m_fPsyDamageReflection;
	float fDamageReflectionRate = sDamageSpec.m_fPsyDamageReflectionRate;

	DEFENSE_SKILL sDefenseSkill = pActor->GetDefenseSkill();

	/* skill amplify, Juver, 2020/12/11 */
	SSKILL_AMPLIFY sSkillAmplifyMelee = pActor->GetSkillAmplifyMelee();
	SSKILL_AMPLIFY sSkillAmplifyMissile = pActor->GetSkillAmplifyMissile();
	SSKILL_AMPLIFY sSkillAmplifyMagic = pActor->GetSkillAmplifyMagic();
	
	/* skill illusion, Juver, 2021/01/17 */
	SKILL_ILLUSION* pIllusion = pActor->GetSkillIllusion();
	if( pIllusion && pIllusion->bActive )
	{
		if( pIllusion->wIllusionRemain > 0 )
		{
			float fCurrentTime = GLGaeaServer::GetInstance().GetCurrentFrameTime();
			float fElapsedTime = fCurrentTime - pIllusion->fIllusionLastTime;
			if( fElapsedTime > GLCONST_CHAR::fIllusionInterval )
			{
				pIllusion->fIllusionLastTime = fCurrentTime;
				pIllusion->wIllusionRemain--;
				rResultDAMAGE = 1;
				return DAMAGE_TYPE_ILLUSION;
			} 
		}
	}

	GLPADATA gdDamage = m_gdDAMAGE_SKILL;
	if ( pSkill )
	{
		const SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[dwskill_lev];
		
		/* immune skill logic, Juver, 2020/12/24 */
		if ( pDamageSpec )
		{
			bool bImmune = false;

			if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_SHORT ) && pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_SHORT )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_LONG ) && pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_LONG )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_MAGIC ) && pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_MAGIC )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_NORMAL ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_NORMAL )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_BUFF ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_BUFF )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_DEBUFF ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_DEBUFF )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_LIMIT ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_LIMIT )
				bImmune = true;

			if ( bImmune )
			{
				rResultDAMAGE = 0;
				return DAMAGE_TYPE_NONE; /*DAMAGE_TYPE_IMMUNE;*/
			}
		}

		//	Note : 스킬 사용시 부적이 소모되면, 부적의 공격치 가산.
		//
		if ( sSKILL_DATA.wUSE_CHARMNUM > 0 )
		{
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem )
			{
				GLPADATA &sDATA = pItem->sSuitOp.gdDamage;
				gdDamage.wLow += sDATA.wLow;
				gdDamage.wHigh += sDATA.wHigh;
			}
		}

		float fRESIST_G = 0;
		switch ( pSkill->m_sBASIC.emAPPLY )
		{
			//	근접 공격.
		case SKILL::EMAPPLY_PHY_SHORT:
			gdDamage.wLow += m_sSUMITEM.gdDamage.wLow;
			gdDamage.wMax += m_sSUMITEM.gdDamage.wMax;
			
			gdDamage.VAR_PARAM ( m_wSUM_PA );
			// 물리 공격시 속성 저항치 적용 비율
			// default.charset 에서 가져온
			// fRESIST_PHYSIC_G 의 값
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			break;

			//	장거리 공격.
		case SKILL::EMAPPLY_PHY_LONG:
			gdDamage.wLow += m_sSUMITEM.gdDamage.wLow;
			gdDamage.wMax += m_sSUMITEM.gdDamage.wMax;

			gdDamage.VAR_PARAM ( m_wSUM_SA );
			// 물리 공격시 속성 저항치 적용 비율
			// default.charset 에서 가져온
			// fRESIST_PHYSIC_G 의 값
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			fDamageReflection = 0.0f;
			fDamageReflectionRate = 0.0f;			
			break;

			//	마법 공격.
    	case SKILL::EMAPPLY_MAGIC:
			//nDEFENSE = 0;						//	마법 공격일때는 일반 방어력 무시됨.
			//nDEFAULT_DEFENSE = 0;				//	마법 공격일때는 일반 방어력 무시됨.
			//nITEM_DEFENSE = 0;					//	마법 공격일때는 일반 방어력 무시됨.

			/*int-def logic, Juver, 2017/09/30 */
			nDEFENSE = int( GLCONST_CHAR::fIntDefRate * nDEFENSE );
			nDEFAULT_DEFENSE = int( GLCONST_CHAR::fIntDefRate * nDEFAULT_DEFENSE );
			nITEM_DEFENSE = int( GLCONST_CHAR::fIntDefRate * nITEM_DEFENSE );

			if ( nDEFENSE < 0 )			nDEFENSE = 0;
			if ( nDEFAULT_DEFENSE < 0 )	nDEFAULT_DEFENSE = 0;
			if ( nITEM_DEFENSE < 0 )	nITEM_DEFENSE = 0;

			gdDamage.VAR_PARAM ( m_wSUM_MA );
			// 마법 공격시 속성 저항치 적용 비율
			// default.charset 에서 가져온
			// fRESIST_G 의 값
			fRESIST_G = GLCONST_CHAR::fRESIST_G;
			fDamageReduce = sDamageSpec.m_fMagicDamageReduce;
			fDamageReflection = sDamageSpec.m_fMagicDamageReflection;
			fDamageReflectionRate = sDamageSpec.m_fMagicDamageReflectionRate;
			bPsyDamage = false;

			break;

		default:
			GASSERT(0&&"CALCDAMAGE() 물리 데미지가 산출되지 않는 스킬.");
			break;
		};

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			if ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC == EMSPECA_CRUSHING_BLOW )	
			{
				const SKILL::SSPEC &sSKILL_SPEC = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[dwskill_lev];
				nCrushingBlow += (int) ( sSKILL_SPEC.fVAR2 * 100 ) ;
			}
		}
		

		EMELEMENT emELMT(EMELEMENT_SPIRIT);
		if ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM )
		{
			SITEM *pITEM = GET_ELMT_ITEM ();
			if ( pITEM )	emELMT = STATE_TO_ELEMENT(pITEM->sSuitOp.sBLOW.emTYPE);
		}
		else
		{
			emELMT = pSkill->m_sAPPLY.emELEMENT;
		}

		short nRESIST = sRESIST.GetElement ( emELMT );
		if ( nRESIST>99 )	nRESIST = 99;
		float fPOWER = GLOGICEX::WEATHER_ELEMENT_POW ( emELMT, dwWeatherFlag, pLandMan->IsWeatherActive() );

		//	스킬의 데미지 계산에 영향을 주는 값이 기본인지 특수인지 검사.
		float fSKILL_VAR = sSKILL_DATA.fBASIC_VAR;

		// 2006-03-28 Jgkim기획팀 요청으로 주석처리, 데미지 공식변경됨
		// int nVAR = abs ( int(fSKILL_VAR*fPOWER) );
		// nVAR = nVAR - (int) ( nVAR*nRESIST/100.0f*fRESIST_G );
		// if ( nVAR<0 )	nVAR = 0;

		// //	Note : 대미지가 증산된다, 대미지 반영율.
		// gdDamage.wLow += WORD ( nVAR + (gdDamage.wLow*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
		// gdDamage.wHigh += WORD ( nVAR + (gdDamage.wHigh*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
		
		
		// 스킬데미지 = 기본스킬데미지*날씨에 따른 데미지반영율
		int nVAR = abs ( int(fSKILL_VAR*fPOWER) );

		// 반영율
		float fGrade = (float) wGRADE / GLCONST_CHAR::fDAMAGE_GRADE_K;

		// 최소/최대 데미지 =  스킬데미지 + (최소/최대 데미지) * 반영율
		gdDamage.wLow  += WORD (nVAR + ((float) gdDamage.wLow  * fGrade));
		gdDamage.wHigh += WORD (nVAR + ((float) gdDamage.wHigh * fGrade));
		
		// 전체저항값 = 상대의 저항값 / 100 * 속성저항치 적용비율
		float fResistTotal = (float) ((float) nRESIST * 0.01f * fRESIST_G);

		// 최소/최대 데미지 = 데미지-(데미지*전체저항값)
		gdDamage.wLow  -= (WORD) ((float) gdDamage.wLow  * fResistTotal);
		gdDamage.wHigh -= (WORD) ((float) gdDamage.wHigh * fResistTotal);
		
		// 데미지가 0 보다 작으면 0 으로 고저한다.
		if (gdDamage.wLow <0) gdDamage.wLow  = 0;
		if (gdDamage.wHigh<0) gdDamage.wHigh = 0;
	}
	else
	{
		gdDamage.wLow += m_sSUMITEM.gdDamage.wLow;
		gdDamage.wMax += m_sSUMITEM.gdDamage.wMax;

		if ( ISLONGRANGE_ARMS() )	gdDamage.VAR_PARAM ( m_wSUM_SA );	//	장거리 공격.
		else						gdDamage.VAR_PARAM ( m_wSUM_PA );	//	근접 공격.
	}

	//	공격력. 변화율 반영.
	gdDamage.wLow  = WORD ( gdDamage.wLow  * m_fDamageRate );
	gdDamage.wHigh = WORD ( gdDamage.wHigh * m_fDamageRate );

	//	Note : 추가 보정치 산출.
	int nExtFORCE = 0;
	int ndxLvl = nLEVEL - GETLEVEL();
	if ( ndxLvl > 0 )			nExtFORCE = int(RANDOM_POS*ndxLvl/10);

	//	Note : Critical 발생 확율.
	if ( ndxLvl > 5 )		ndxLvl = 5;
	if ( ndxLvl < -5 )		ndxLvl = -5;

	//	Note : Critical 발생 확율.
	int nPerHP = 10;
	if ( GETHP() > 0 && GETMAXHP() > 0 )
		nPerHP = ((GETHP()*100)/GETMAXHP());

	if ( nPerHP <= 10 )	nPerHP = 10;
	int nPercentCri = 1000 / nPerHP - 10 + ndxLvl;
	nPercentCri += (int)( m_sSUMITEM.fIncR_Critical * 100 );
	nPercentCri += (int)( m_sSUMITEM.fInc_Critical );

	/*additional RV, Juver, 2018/06/15 */
	nPercentCri += (int)( m_sSUMITEM.fadd_critical_hit_chance );
	nCrushingBlow += (int)( m_sSUMITEM.fadd_crushing_blow_chance );


	/* skill amplify, Juver, 2020/12/10 */
	if ( pSkill )
	{
		switch ( pSkill->m_sBASIC.emAPPLY )
		{
		case SKILL::EMAPPLY_PHY_SHORT:
			{
				nPercentCri += (int)(sSkillAmplifyMelee.fCriticalRate * 100.0f);
				nCrushingBlow += (int)(sSkillAmplifyMelee.fCriticalRate * 100.0f);
			}break;

		case SKILL::EMAPPLY_PHY_LONG:		
			{
				nPercentCri += (int)(sSkillAmplifyMissile.fCriticalRate * 100.0f);
				nCrushingBlow += (int)(sSkillAmplifyMissile.fCriticalRate * 100.0f);
			}break;

		case SKILL::EMAPPLY_MAGIC:
			{
				nPercentCri += (int)(sSkillAmplifyMagic.fCriticalRate * 100.0f);
				nCrushingBlow += (int)(sSkillAmplifyMagic.fCriticalRate * 100.0f);
			}break;
		};
	}

	if ( nPercentCri > (int)GLCONST_CHAR::dwCRITICAL_MAX )		nPercentCri = GLCONST_CHAR::dwCRITICAL_MAX;
	if ( nPercentCri < 0 )	nPercentCri = 0;

	//	크리티컬 발생 여부 판단.
	if ( nPercentCri > (RANDOM_POS*100) )	bCritical = true;

	//	강한타격 발생 여부 판단
    if ( nCrushingBlow > GLCONST_CHAR::dwCRUSHING_BLOW_MAX )		nCrushingBlow = GLCONST_CHAR::dwCRUSHING_BLOW_MAX;	
	if ( nCrushingBlow > (RANDOM_POS*100) )	bCrushingBlow = true;

	//	대미지 최대 최소값 사이의 랜덤 대미지 결정.
	int  nDAMAGE_NOW = 0;
	nDAMAGE_NOW = int ( gdDamage.wLow + (gdDamage.wHigh-gdDamage.wLow)*RANDOM_POS );
	int nDAMAGE_OLD = ( nDAMAGE_NOW + nExtFORCE );	//	보정치 합산.

	//	최저 수용 대미지 산출.
	int nNetDAMAGE = int ( nDAMAGE_OLD*(1.0f-GLCONST_CHAR::fLOW_SEED_DAMAGE) - nDEFENSE );

	if ( nNetDAMAGE > 0 )	rResultDAMAGE = int ( nDAMAGE_OLD - nDEFENSE );
	else					rResultDAMAGE = int ( (nDAMAGE_OLD*GLCONST_CHAR::fLOW_SEED_DAMAGE)*RANDOM_POS );
	rResultDAMAGE = int(rResultDAMAGE*fSTATE_DAMAGE);

	/*damage reduction limit, Juver, 2018/11/05 */
	if ( RPARAM::damage_reduction_limit )
	{
		//	방어구 대미지 흡수율 반영.
		float damage_reduction_rate = ( nDEFAULT_DEFENSE*nITEM_DEFENSE / GLCONST_CHAR::fDAMAGE_DEC_RATE );
		if ( damage_reduction_rate >= GLCONST_CHAR::fDAMAGE_DEC_MAX_G )
			damage_reduction_rate = GLCONST_CHAR::fDAMAGE_DEC_MAX_G;

		float fRATE = ( 1.0f - damage_reduction_rate );
		if ( fRATE > 1.0f )		fRATE = 1.0f;
		if ( fRATE < 0.0f )		fRATE = 0.0f;
		rResultDAMAGE = int(rResultDAMAGE*fRATE);
	}
	else
	{
		//	방어구 대미지 흡수율 반영.
		float fRATE = ( 1.0f - nDEFAULT_DEFENSE*nITEM_DEFENSE / GLCONST_CHAR::fDAMAGE_DEC_RATE );
		if ( fRATE > 1.0f )		fRATE = 1.0f;
		if ( fRATE < 0.0f )		fRATE = 0.0f;
		rResultDAMAGE = int(rResultDAMAGE*fRATE);
	}

	/*additional RV, Juver, 2018/06/15 */
	int ncritical_hit_damage = int( GLCONST_CHAR::dwCRITICAL_DAMAGE + (int)m_sSUMITEM.fadd_critical_hit_damage );
	int ncrushing_blow_damage = int( GLCONST_CHAR::dwCRUSHING_BLOW_DAMAGE + (int)m_sSUMITEM.fadd_crushing_blow_damage );

	if ( bCritical && bCrushingBlow )
	{
		rResultDAMAGE = int ( rResultDAMAGE* ncrushing_blow_damage / 100 );
	}else{
		if ( bCritical )	rResultDAMAGE = int ( rResultDAMAGE* ncritical_hit_damage / 100 );
		if ( bCrushingBlow ) rResultDAMAGE = int ( rResultDAMAGE* ncrushing_blow_damage / 100 );
	}
	
	/* skill amplify, Juver, 2020/12/10 */
	if ( pSkill )
	{
		float fDamageAmplify = 1.0f;

		switch ( pSkill->m_sBASIC.emAPPLY )
		{
		case SKILL::EMAPPLY_PHY_SHORT:
			{
				fDamageAmplify += sSkillAmplifyMelee.fDamageRate;
			}break;

		case SKILL::EMAPPLY_PHY_LONG:		
			{
				fDamageAmplify += sSkillAmplifyMissile.fDamageRate;
			}break;

		case SKILL::EMAPPLY_MAGIC:
			{
				fDamageAmplify += sSkillAmplifyMagic.fDamageRate;
			}break;
		};

		if ( fDamageAmplify != 1.0f )
		{
			rResultDAMAGE = int(rResultDAMAGE*fDamageAmplify);
		}	
	}


	//	데미지 흡수
	if ( fDamageReduce > 0.0f ) 
	{
		int nDamageReduce = (int) ( ( (rResultDAMAGE * fDamageReduce) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
		rResultDAMAGE -= nDamageReduce;
		
		if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REDUCE;
		else dwDamageFlag += DAMAGE_TYPE_PSY_REDUCE;
	}


	//	데미지 반사 계산
	if ( fDamageReflectionRate > 0.0f )
	{
		if ( fDamageReflectionRate > (RANDOM_POS*1) )
		{
			int nDamageReflection = (int) ( ( (rResultDAMAGE * fDamageReflection) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
			
			if ( nDamageReflection > 0 )  
			{
				if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REFLECTION;
				else dwDamageFlag += DAMAGE_TYPE_MAGIC_REFLECTION;
				
				// 피격자 입장에서 데미지를 다시 보낸다.
				STARGETID sActor(CROW_PC,dwGaeaID);
				pActor->DamageReflectionProc( nDamageReflection, sActor );
			}
		}
	}

	//	발동 스킬
	if ( sDefenseSkill.m_dwSkillID != NATIVEID_NULL() )
	{
		if ( sDefenseSkill.m_fRate > (RANDOM_POS*1) )
		{
			STARGETID sActor(CROW_PC,dwGaeaID);
			pActor->DefenseSkill( sDefenseSkill.m_dwSkillID, 
								  sDefenseSkill.m_wLevel, 
								  sActor );
		}
	}

	if ( rResultDAMAGE <= 1 )
	{
		bCritical = false;
		bCrushingBlow = false;
		rResultDAMAGE = 1;
	}

	if ( dwDivCount>1 )
	{
		rResultDAMAGE /= dwDivCount;
	}

	bShock = GLOGICEX::CHECKSHOCK ( GETLEVEL(), nLEVEL, rResultDAMAGE, bCritical );

	if ( bShock )			dwDamageFlag += DAMAGE_TYPE_SHOCK;
	if ( bCritical )		dwDamageFlag += DAMAGE_TYPE_CRITICAL;
	if ( bCrushingBlow )	dwDamageFlag += DAMAGE_TYPE_CRUSHING_BLOW;

	return dwDamageFlag;
}

// 기존의 데미지 공식
DWORD GLCHARLOGIC_SERVER::CALCDAMAGE_2004(
	int& rResultDAMAGE,
	const DWORD dwGaeaID,
	const STARGETID &cTargetID,
	const GLLandMan* pLandMan,
	const GLSKILL* pSkill,
	const DWORD dwskill_lev,
	const DWORD dwWeatherFlag,
	const int dwDivCount)
{
	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
	if ( !pActor )	return DAMAGE_TYPE_NONE;

	/*ignore damage, Juver, 2017/12/12 */
	if ( pActor->IsIgnoreDamage() )
	{
		rResultDAMAGE = 1;
		return DAMAGE_TYPE_NONE;
	}

	int nDEFENSE = pActor->GetDefense ();
	int nDEFAULT_DEFENSE = pActor->GetBodyDefense ();
	int nITEM_DEFENSE = pActor->GetItemDefense ();

	int nLEVEL = pActor->GetLevel ();
	float fSTATE_DAMAGE = pActor->GETSTATE_DAMAGE ();
	const SRESIST &sRESIST = pActor->GETRESIST ();

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	WORD wGRADE = 0;
	const SITEMCUSTOM &sRHAND = GET_SLOT_ITEM(emRHand);
	if ( sRHAND.sNativeID != SNATIVEID(false) )
		wGRADE = sRHAND.GETGRADE(EMGRINDING_DAMAGE);

	int nCrushingBlow = (int)( m_sSUMITEM.fIncR_CrushingBlow * 100 );
	nCrushingBlow += (int)( m_sSUMITEM.fInc_CrushingBlow );

	DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
	bool bShock = false;
	bool bCrushingBlow = false;
	bool bCritical = false; 
	bool bPsyDamage = true;

	const DAMAGE_SPEC& sDamageSpec = pActor->GetDamageSpec();
	DAMAGE_SPEC* pDamageSpec = pActor->GetDamageSpecPtr();
	float fDamageReduce = sDamageSpec.m_fPsyDamageReduce;
	float fDamageReflection = sDamageSpec.m_fPsyDamageReflection;
	float fDamageReflectionRate = sDamageSpec.m_fPsyDamageReflectionRate;

	DEFENSE_SKILL sDefenseSkill = pActor->GetDefenseSkill();

	/* skill illusion, Juver, 2021/01/17 */
	SKILL_ILLUSION* pIllusion = pActor->GetSkillIllusion();
	if( pIllusion && pIllusion->bActive )
	{
		if( pIllusion->wIllusionRemain > 0 )
		{
			float fCurrentTime = GLGaeaServer::GetInstance().GetCurrentFrameTime();
			float fElapsedTime = fCurrentTime - pIllusion->fIllusionLastTime;
			if( fElapsedTime > GLCONST_CHAR::fIllusionInterval )
			{
				pIllusion->fIllusionLastTime = fCurrentTime;
				pIllusion->wIllusionRemain--;
				rResultDAMAGE = 1;
				return DAMAGE_TYPE_ILLUSION;
			} 
		}
	}

	GLPADATA gdDamage = m_gdDAMAGE_SKILL;
	if ( pSkill )
	{
		const SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[dwskill_lev];
		
		/* immune skill logic, Juver, 2020/12/24 */
		if ( pDamageSpec )
		{
			bool bImmune = false;

			if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_SHORT ) && pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_SHORT )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_PHY_LONG ) && pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_LONG )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneApplyType & SKILL::EMAPPLY_FLAG_MAGIC ) && pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_MAGIC )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_NORMAL ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_NORMAL )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_BUFF ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_BUFF )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_DEBUFF ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_DEBUFF )
				bImmune = true;

			if ( ( pDamageSpec->m_dwImmuneActionType & SKILL::EMACTION_FLAG_LIMIT ) && pSkill->m_sBASIC.emACTION == SKILL::EMACTION_LIMIT )
				bImmune = true;

			if ( bImmune )
			{
				rResultDAMAGE = 0;
				return DAMAGE_TYPE_NONE; /*DAMAGE_TYPE_IMMUNE;*/
			}
		}

		//	Note : 스킬 사용시 부적이 소모되면, 부적의 공격치 가산.
		//
		if ( sSKILL_DATA.wUSE_CHARMNUM > 0 )
		{
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem )
			{
				GLPADATA &sDATA = pItem->sSuitOp.gdDamage;
				gdDamage.wLow += sDATA.wLow;
				gdDamage.wHigh += sDATA.wHigh;
			}
		}

		float fRESIST_G = 0;
		switch ( pSkill->m_sBASIC.emAPPLY )
		{
			//	근접 공격.
		case SKILL::EMAPPLY_PHY_SHORT:
			gdDamage.wLow += m_sSUMITEM.gdDamage.wLow;
			gdDamage.wMax += m_sSUMITEM.gdDamage.wMax;
			
			gdDamage.VAR_PARAM ( m_wSUM_PA );
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			break;

			//	장거리 공격.
		case SKILL::EMAPPLY_PHY_LONG:
			gdDamage.wLow += m_sSUMITEM.gdDamage.wLow;
			gdDamage.wMax += m_sSUMITEM.gdDamage.wMax;

			gdDamage.VAR_PARAM ( m_wSUM_SA );
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			
			fDamageReflection = 0.0f;
			fDamageReflectionRate = 0.0f;

			break;

			//	마법 공격.
    	case SKILL::EMAPPLY_MAGIC:
			//nDEFENSE = 0;						//	마법 공격일때는 일반 방어력 무시됨.
			//nDEFAULT_DEFENSE = 0;				//	마법 공격일때는 일반 방어력 무시됨.
			//nITEM_DEFENSE = 0;					//	마법 공격일때는 일반 방어력 무시됨.

			/*int-def logic, Juver, 2017/09/30 */
			nDEFENSE = int( GLCONST_CHAR::fIntDefRate * nDEFENSE );
			nDEFAULT_DEFENSE = int( GLCONST_CHAR::fIntDefRate * nDEFAULT_DEFENSE );
			nITEM_DEFENSE = int( GLCONST_CHAR::fIntDefRate * nITEM_DEFENSE );

			if ( nDEFENSE < 0 )			nDEFENSE = 0;
			if ( nDEFAULT_DEFENSE < 0 )	nDEFAULT_DEFENSE = 0;
			if ( nITEM_DEFENSE < 0 )	nITEM_DEFENSE = 0;

			gdDamage.VAR_PARAM ( m_wSUM_MA );
			fRESIST_G = GLCONST_CHAR::fRESIST_G;
			
			fDamageReduce = sDamageSpec.m_fMagicDamageReduce;
			fDamageReflection = sDamageSpec.m_fMagicDamageReflection;
			fDamageReflectionRate = sDamageSpec.m_fMagicDamageReflectionRate;
			bPsyDamage = false;
			break;

		default:
			GASSERT(0&&"CALCDAMAGE() 물리 데미지가 산출되지 않는 스킬.");
			break;
		};

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			if ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC == EMSPECA_CRUSHING_BLOW )
			{
				const SKILL::SSPEC &sSKILL_SPEC = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[dwskill_lev];
				nCrushingBlow += (int) ( sSKILL_SPEC.fVAR2 * 100 ) ;
			}
		}

		EMELEMENT emELMT(EMELEMENT_SPIRIT);
		if ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM )
		{
			SITEM *pITEM = GET_ELMT_ITEM ();
			if ( pITEM )	emELMT = STATE_TO_ELEMENT(pITEM->sSuitOp.sBLOW.emTYPE);
		}
		else
		{
			emELMT = pSkill->m_sAPPLY.emELEMENT;
		}

		short nRESIST = sRESIST.GetElement ( emELMT );
		if ( nRESIST>99 )	nRESIST = 99;
		float fPOWER = GLOGICEX::WEATHER_ELEMENT_POW ( emELMT, dwWeatherFlag, pLandMan->IsWeatherActive() );

		//	스킬의 데미지 계산에 영향을 주는 값이 기본인지 특수인지 검사.
		float fSKILL_VAR = sSKILL_DATA.fBASIC_VAR;

		int nVAR = abs ( int(fSKILL_VAR*fPOWER) );
		nVAR = nVAR - (int) ( nVAR*nRESIST*0.01f*fRESIST_G );
		if ( nVAR<0 )	nVAR = 0;

		//	Note : 대미지가 증산된다, 대미지 반영율.
		gdDamage.wLow += WORD ( nVAR + (gdDamage.wLow*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
		gdDamage.wHigh += WORD ( nVAR + (gdDamage.wHigh*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
	}
	else
	{
		gdDamage.wLow += m_sSUMITEM.gdDamage.wLow;
		gdDamage.wMax += m_sSUMITEM.gdDamage.wMax;

		if ( ISLONGRANGE_ARMS() )	gdDamage.VAR_PARAM ( m_wSUM_SA );	//	장거리 공격.
		else						gdDamage.VAR_PARAM ( m_wSUM_PA );	//	근접 공격.
	}

	//	공격력. 변화율 반영.
	gdDamage.wLow = WORD ( gdDamage.wLow * m_fDamageRate );
	gdDamage.wHigh = WORD ( gdDamage.wHigh * m_fDamageRate );

	//	Note : 추가 보정치 산출.
	int nExtFORCE = 0;
	int ndxLvl = nLEVEL - GETLEVEL();
	if ( ndxLvl > 0 )			nExtFORCE = int(RANDOM_POS*ndxLvl*0.1f);

	//	Note : Critical 발생 확율.
	if ( ndxLvl > 5 )		ndxLvl = 5;
	if ( ndxLvl < -5 )		ndxLvl = -5;

	//	Note : Critical 발생 확율.
	int nPerHP = 10;
	if ( GETHP() > 0 && GETMAXHP() > 0 )
		nPerHP = ((GETHP()*100)/GETMAXHP());

	if ( nPerHP <= 10 )	nPerHP = 10;
	int nPercentCri = 1000 / nPerHP - 10 + ndxLvl;
	nPercentCri += (int)( m_sSUMITEM.fIncR_Critical * 100 );
	nPercentCri += (int)( m_sSUMITEM.fInc_Critical );

	/*additional RV, Juver, 2018/06/15 */
	nPercentCri += (int)( m_sSUMITEM.fadd_critical_hit_chance );
	nCrushingBlow += (int)( m_sSUMITEM.fadd_crushing_blow_chance );

	if ( nPercentCri > (int)GLCONST_CHAR::dwCRITICAL_MAX )		nPercentCri = GLCONST_CHAR::dwCRITICAL_MAX;
	if ( nPercentCri < 0 )	nPercentCri = 0;

	//	크리티컬 발생 여부 판단.
	if ( nPercentCri > (RANDOM_POS*100) )	bCritical = true;

	//	강한타격 발생 여부 판단
    if ( nCrushingBlow > GLCONST_CHAR::dwCRUSHING_BLOW_MAX )		nCrushingBlow = GLCONST_CHAR::dwCRUSHING_BLOW_MAX;	
	if ( nCrushingBlow > (RANDOM_POS*100) )	bCrushingBlow = true;

	//	대미지 최대 최소값 사이의 랜덤 대미지 결정.
	int  nDAMAGE_NOW = 0;
	nDAMAGE_NOW = int ( gdDamage.wLow + (gdDamage.wHigh-gdDamage.wLow)*RANDOM_POS );
	int nDAMAGE_OLD = ( nDAMAGE_NOW + nExtFORCE );	//	보정치 합산.

	//	최저 수용 대미지 산출.
	int nNetDAMAGE = int ( nDAMAGE_OLD*(1.0f-GLCONST_CHAR::fLOW_SEED_DAMAGE) - nDEFENSE );

	if ( nNetDAMAGE > 0 )	rResultDAMAGE = int ( nDAMAGE_OLD - nDEFENSE );
	else					rResultDAMAGE = int ( (nDAMAGE_OLD*GLCONST_CHAR::fLOW_SEED_DAMAGE)*RANDOM_POS );
	rResultDAMAGE = int(rResultDAMAGE*fSTATE_DAMAGE);

	/*damage reduction limit, Juver, 2018/11/05 */
	if ( RPARAM::damage_reduction_limit )
	{
		//	방어구 대미지 흡수율 반영.
		float damage_reduction_rate = ( nDEFAULT_DEFENSE*nITEM_DEFENSE / GLCONST_CHAR::fDAMAGE_DEC_RATE );
		if ( damage_reduction_rate >= GLCONST_CHAR::fDAMAGE_DEC_MAX_G )
			damage_reduction_rate = GLCONST_CHAR::fDAMAGE_DEC_MAX_G;

		float fRATE = ( 1.0f - damage_reduction_rate );
		if ( fRATE > 1.0f )		fRATE = 1.0f;
		if ( fRATE < 0.0f )		fRATE = 0.0f;
		rResultDAMAGE = int(rResultDAMAGE*fRATE);
	}
	else
	{
		//	방어구 대미지 흡수율 반영.
		float fRATE = ( 1.0f - nDEFAULT_DEFENSE*nITEM_DEFENSE / GLCONST_CHAR::fDAMAGE_DEC_RATE );
		if ( fRATE > 1.0f )		fRATE = 1.0f;
		if ( fRATE < 0.0f )		fRATE = 0.0f;
		rResultDAMAGE = int(rResultDAMAGE*fRATE);
	}
	

	/*additional RV, Juver, 2018/06/15 */
	int ncritical_hit_damage = int( GLCONST_CHAR::dwCRITICAL_DAMAGE + (int)m_sSUMITEM.fadd_critical_hit_damage );
	int ncrushing_blow_damage = int( GLCONST_CHAR::dwCRUSHING_BLOW_DAMAGE + (int)m_sSUMITEM.fadd_crushing_blow_damage );

	if ( bCritical && bCrushingBlow )
	{
		rResultDAMAGE = int ( rResultDAMAGE* ncrushing_blow_damage / 100 );
	}else{
		if ( bCritical )	rResultDAMAGE = int ( rResultDAMAGE* ncritical_hit_damage / 100 );
		if ( bCrushingBlow ) rResultDAMAGE = int ( rResultDAMAGE* ncrushing_blow_damage / 100 );
	}

	//	데미지 흡수
	if ( fDamageReduce > 0.0f ) 
	{
		int nDamageReduce = (int) ( ( (rResultDAMAGE * fDamageReduce) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
		rResultDAMAGE -= nDamageReduce;
		
		if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REDUCE;
		else dwDamageFlag += DAMAGE_TYPE_MAGIC_REDUCE;
	}


	//	데미지 반사 계산
	if ( fDamageReflectionRate > 0.0f )
	{
		if ( fDamageReflectionRate > (RANDOM_POS*1) )
		{
			int nDamageReflection = (int) ( ( (rResultDAMAGE * fDamageReflection) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
			
			if ( nDamageReflection > 0 ) 
			{
				if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REFLECTION;
				else dwDamageFlag += DAMAGE_TYPE_MAGIC_REFLECTION;		

				// 피격자 입장에서 데미지를 다시 보낸다.
				STARGETID sActor(CROW_PC,dwGaeaID);
				pActor->DamageReflectionProc( nDamageReflection, sActor );
			}
		}
	}

	//	발동 스킬
	if ( sDefenseSkill.m_dwSkillID != NATIVEID_NULL() )
	{
		if ( sDefenseSkill.m_fRate > (RANDOM_POS*1) )
		{
			STARGETID sActor(CROW_PC,dwGaeaID);
			pActor->DefenseSkill( sDefenseSkill.m_dwSkillID, 
								  sDefenseSkill.m_wLevel, 
								  sActor );
		}
	}

	if ( rResultDAMAGE <= 1 )
	{
		bCritical = false;
		bCrushingBlow = false;
		rResultDAMAGE = 1;
	}

	if ( dwDivCount>1 )
	{
		rResultDAMAGE /= dwDivCount;
	}

	bShock = GLOGICEX::CHECKSHOCK ( GETLEVEL(), nLEVEL, rResultDAMAGE, bCritical );

	if ( bShock )			dwDamageFlag += DAMAGE_TYPE_SHOCK;
	if ( bCritical )		dwDamageFlag += DAMAGE_TYPE_CRITICAL;
	if ( bCrushingBlow )	dwDamageFlag += DAMAGE_TYPE_CRUSHING_BLOW;

	return dwDamageFlag;
}

WORD GLCHARLOGIC_SERVER::RECEIVE_DAMAGE ( const WORD wDamage )
{
	WORD wOLD = m_sHP.wNow;
	m_sHP.DECREASE ( wDamage );

	return (wOLD>m_sHP.wNow) ? (wOLD-m_sHP.wNow) : 0;
}

/*qbox check, Juver, 2017/12/05 */
BOOL GLCHARLOGIC_SERVER::RECEIVE_QITEMFACT ( const SNATIVEID &nidITEM, BOOL bMobGen )
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( nidITEM );
	if ( !pITEM )										return FALSE;
	if ( pITEM->sBasicOp.emItemType!=ITEM_QITEM )		return FALSE;
	if ( pITEM->sQuestionItem.emType==QUESTION_NONE )	return FALSE;

	ITEM::SQUESTIONITEM &sQUESTIONITEM = pITEM->sQuestionItem;

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
		m_sQITEMFACT.emType = sQUESTIONITEM.emType;
		m_sQITEMFACT.fTime = sQUESTIONITEM.fTime;
		m_sQITEMFACT.wParam1 = sQUESTIONITEM.wParam1;
		m_sQITEMFACT.wParam2 = sQUESTIONITEM.wParam2;
		m_sQITEMFACT.bMobGen = bMobGen; /*qbox check, Juver, 2017/12/05 */
		break;
	};

	return TRUE;
}

void GLCHARLOGIC_SERVER::RECEIVE_EVENTFACT_BEGIN ( const EMGM_EVENT_TYPE emType, const WORD wValue )
{
	m_sEVENTFACT.SetEVENT( emType, wValue );
}

void GLCHARLOGIC_SERVER::RECEIVE_EVENTFACT_END ( const EMGM_EVENT_TYPE emType )
{
	m_sEVENTFACT.ResetEVENT( emType );
}

//	Note : ? 아이템의 유효 시간을 갱신시킨다.
//			리턴 코드는 종료시점에 false, 그외 사항에 true
//
bool GLCHARLOGIC_SERVER::UPDATE_QITEMFACT ( float fElapsedTime )
{
	if( !m_sQITEMFACT.IsACTIVE() )		return true;

	m_sQITEMFACT.fTime -= fElapsedTime;
	if ( m_sQITEMFACT.fTime <= 0 )
	{
		m_sQITEMFACT.RESET();
		return false;
	}

	return true;
}

void GLCHARLOGIC_SERVER::UPDATE_MAX_POINT ( float fCONFT_POINT_RATE )
{
	EMCHARINDEX emCI = GETCHARINDEX ();										//	케릭터의 종류 인덱스.
	const GLCONST_CHARCLASS &cCHARCONST = GLCONST_CHAR::cCONSTCLASS[emCI];	//	케릭터의 상수들.

	m_sHP.wMax = WORD ( m_sSUMSTATS.wStr*cCHARCONST.fHP_STR + m_sSUMITEM.nHP + m_sSUM_PASSIVE.m_nHP );
	m_sHP.wMax = WORD ( m_sHP.wMax * (1+m_sSUM_PASSIVE.m_fHP_RATE+m_fHP_RATE ) * fCONFT_POINT_RATE );
	m_sHP.wMax += m_dwHPIncrease; /* Codex HP, Archie 02/16/24 */
	if (m_sHP.wMax < m_dwHPIncrease || m_sHP.wMax > USHRT_MAX) m_sHP.wMax = USHRT_MAX;
   // m_sHP.LIMIT();

	m_sMP.wMax = WORD ( m_sSUMSTATS.wSpi*cCHARCONST.fMP_SPI + m_sSUMITEM.nMP + m_sSUM_PASSIVE.m_nMP );
	m_sMP.wMax = WORD ( m_sMP.wMax * (1+m_sSUM_PASSIVE.m_fMP_RATE+m_fMP_RATE ) * fCONFT_POINT_RATE );
	m_sMP.wMax += m_dwMPIncrease; /* Codex HP, Archie 02/16/24 */
	m_sMP.LIMIT();

	m_sSP.wMax = WORD ( m_sSUMSTATS.wSta*cCHARCONST.fSP_STA + m_sSUMITEM.nSP + m_sSUM_PASSIVE.m_nSP );
	m_sSP.wMax = WORD ( m_sSP.wMax * (1+m_sSUM_PASSIVE.m_fSP_RATE+m_fSP_RATE ) * fCONFT_POINT_RATE );
	m_sSP.wMax += m_dwSPIncrease; /* Codex HP, Archie 02/16/24 */
	m_sSP.LIMIT();


	/*combatpoint logic, Juver, 2017/05/28 */
	/*item max CP, Juver, 2018/07/09 */
	m_sCombatPoint.wMax = WORD (GLCONST_CHAR::GETCPMAX(m_dwReborn) + m_sSUMITEM.wadd_max_cp );
	m_sCombatPoint.LIMIT();
}

void GLCHARLOGIC_SERVER::UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient, float fCONFT_POINT_RATE )
{
	//	사용된 스킬 딜래이 갱신.
	UPDATESKILLDELAY ( fElapsedTime );

	m_nDEFENSE_SKILL = m_nDEFENSE;
	m_gdDAMAGE_SKILL = m_gdDAMAGE;

	m_nSUM_HIT = m_nHIT;
	m_nSUM_AVOID = m_nAVOID;

	m_nSUM_PIERCE			= m_sSUM_PASSIVE.m_nPIERCE;
	m_fSUM_TARGET_RANGE		= m_sSUM_PASSIVE.m_fTARGET_RANGE;
	m_fSUM_ATTACK_RANGE		= m_sSUM_PASSIVE.m_fATTACK_RANGE;
	m_fSUM_APPLY_RANGE		= m_sSUM_PASSIVE.m_fAPPLY_RANGE;

	m_bSTATEBLOW = FALSE;
	m_fSTATE_MOVE = 1.0f;

	m_fSTATE_DELAY = 1.0f;

	m_fSTATE_DAMAGE = 1.0f;
	m_fSKILL_MOVE = 0.0f + m_sSUM_PASSIVE.m_fMOVEVELO;

	m_bSTATE_PANT = false;
	m_bSTATE_STUN = false;
	
	m_dwHOLDBLOW = NULL;

	m_fDamageRate = 1.0f + m_sSUM_PASSIVE.m_fDAMAGE_RATE;
	m_fDefenseRate = 1.0f + m_sSUM_PASSIVE.m_fDEFENSE_RATE;

	m_sSUMRESIST_SKILL = m_sSUMRESIST;

	m_sDamageSpec.RESET();
	m_sDamageSpec = m_sSUM_PASSIVE.m_sDamageSpec;
	m_sDefenseSkill.RESET();

	m_fATTVELO = 0.0f;

	m_fSKILLDELAY = 0.0f;

	m_fEXP_RATE = 1;
	m_fGEN_RATE = 1;
	m_fGOLD_RATE = 1.0f;	/*gold drop buff, Juver, 2018/07/01 */

	float fINCR_HP(m_fINCR_HP), fINCR_MP(m_fINCR_MP), fINCR_SP(m_fINCR_SP);

	int nSUM_PA(0), nSUM_SA(0), nSUM_MA(0);

	m_fHP_RATE = 0;
	m_fMP_RATE = 0;
	m_fSP_RATE = 0;

	m_bSafeZone = false;

	/*prohibit potion skill logic, Juver, 2017/06/06 */
	m_bProhibitPotion = false;

	/*prohibit skill logic, Juver, 2017/06/06 */
	m_bProhibitSkill = false; 

	/*curse skill logic, Juver, 2017/06/10 */
	m_bCurse = false;
	m_fCurseDamage = 0.0f;

	/*summon time, Juver, 2017/12/12 */
	m_nSummonTime = 0;

	/*ignore damage, Juver, 2017/12/12 */
	m_bIgnoreDamage = false;

	/*skill vehicle off, Juver, 2018/09/07 */
	m_skill_vehicle_off = false;

	/*skill pet off, Juver, 2018/09/07 */
	m_skill_pet_off = false;

	/*skill transform, Juver, 2018/09/08 */
	m_skill_transform_data.Reset();

	m_fINCR_HP_TOTAL = 0.0f;
	m_fINCR_MP_TOTAL = 0.0f;
	m_fINCR_SP_TOTAL = 0.0f;

	/* skill turn/find invisible, Juver, 2020/04/25 */
	m_bSkillTurnInvisible = false;
	m_wSkillTurnInvisibleLevel = 0;
	m_wSkillTurnInvisibleAnimation = SKILL::EMSPEC_INVISIBLE_ANI_TYPE_NONE;
	m_bSkillTurnInvisibleDisableInCombat = false;
	m_sSkillTurnInvisibleID = NATIVEID_NULL();
	m_bSkillShowInvisible = false;
	m_wSkillShowInvisibleLevel = 0;
	m_wSkillShowInvisibleRange = 0;

	/* skill amplify, Juver, 2020/12/10 */
	m_sSkillAmplifyMelee.RESET();
	m_sSkillAmplifyMissile.RESET();
	m_sSkillAmplifyMagic.RESET();

	/* skill hostile, Juver, 2020/12/16 */
	m_bSkillHostile = false;

	/* skill buff duration change, Juver, 2020/12/17 */
	m_sSkillDurationChange.Reset();

	/* non vend area, Juver, 2021/01/17 */
	m_bNonVendArea = false;

	/* skill illusion, Juver, 2021/01/17 */
	m_sSkillIllusion.bActive = FALSE;

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		SSKILLFACT &sSKEFF = m_sSKILLFACT[i];
		if ( sSKEFF.sNATIVEID==NATIVEID_NULL() )	continue;

		sSKEFF.fAGE -= fElapsedTime;

		//	클라이언트가 아닐 경우 여기서 스킬 이펙트를 비활성화, 클라이언트는 UpdateSkillEffect()에서 함.
		if ( !bClient && sSKEFF.fAGE <= 0.0f )		DISABLESKEFF(i);

		switch ( sSKEFF.emTYPE )
		{
		case SKILL::EMFOR_VARHP:	fINCR_HP += sSKEFF.fMVAR;	break;
		case SKILL::EMFOR_VARMP:	fINCR_MP += sSKEFF.fMVAR;	break;
		case SKILL::EMFOR_VARSP:	fINCR_SP += sSKEFF.fMVAR;	break;
		case SKILL::EMFOR_VARAP:
			fINCR_HP += sSKEFF.fMVAR;
			fINCR_MP += sSKEFF.fMVAR;
			fINCR_SP += sSKEFF.fMVAR;
			break;

		case SKILL::EMFOR_DEFENSE:		m_nDEFENSE_SKILL += (int) sSKEFF.fMVAR;				break;
		case SKILL::EMFOR_HITRATE:		m_nSUM_HIT += int(sSKEFF.fMVAR);					break;
		case SKILL::EMFOR_AVOIDRATE:	m_nSUM_AVOID += int(sSKEFF.fMVAR);					break;
		case SKILL::EMFOR_VARDAMAGE:	m_gdDAMAGE_SKILL.VAR_PARAM ( int(sSKEFF.fMVAR) );	break;
		case SKILL::EMFOR_VARDEFENSE:	m_nDEFENSE_SKILL += int(sSKEFF.fMVAR);				break;
		case SKILL::EMFOR_PA:			nSUM_PA += int(sSKEFF.fMVAR);						break;
		case SKILL::EMFOR_SA:			nSUM_SA += int(sSKEFF.fMVAR);						break;
		case SKILL::EMFOR_MA:			nSUM_MA += int(sSKEFF.fMVAR);						break;
		case SKILL::EMFOR_HP_RATE:		m_fHP_RATE += sSKEFF.fMVAR;							break;
		case SKILL::EMFOR_MP_RATE:		m_fMP_RATE += sSKEFF.fMVAR;							break;
		case SKILL::EMFOR_SP_RATE:		m_fSP_RATE += sSKEFF.fMVAR;							break;
		case SKILL::EMFOR_RESIST:		m_sSUMRESIST_SKILL += int(sSKEFF.fMVAR);			break;
		case SKILL::EMFOR_SUMMONTIME:	m_nSummonTime += (int) sSKEFF.fMVAR;				break;	/*summon time, Juver, 2017/12/12 */
		};

		for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
		{
			switch ( sSKEFF.sImpacts[nImpact].emADDON )
			{
			case EMIMPACTA_HITRATE:			m_nSUM_HIT += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);					break;
			case EMIMPACTA_AVOIDRATE:		m_nSUM_AVOID += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_DAMAGE:			m_gdDAMAGE_SKILL.VAR_PARAM ( int(sSKEFF.sImpacts[nImpact].fADDON_VAR) );	break;
			case EMIMPACTA_DEFENSE:			m_nDEFENSE_SKILL += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);			break;
			case EMIMPACTA_VARHP:			fINCR_HP += sSKEFF.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARMP:			fINCR_MP += sSKEFF.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARSP:			fINCR_SP += sSKEFF.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARAP:
				fINCR_HP += sSKEFF.sImpacts[nImpact].fADDON_VAR;
				fINCR_MP += sSKEFF.sImpacts[nImpact].fADDON_VAR;
				fINCR_SP += sSKEFF.sImpacts[nImpact].fADDON_VAR;
				break;

			case EMIMPACTA_DAMAGE_RATE:		m_fDamageRate += sSKEFF.sImpacts[nImpact].fADDON_VAR;				break;
			case EMIMPACTA_DEFENSE_RATE:	m_fDefenseRate += sSKEFF.sImpacts[nImpact].fADDON_VAR;				break;		

			case EMIMPACTA_PA:				nSUM_PA += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_SA:				nSUM_SA += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_MA:				nSUM_MA += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_HP_RATE:			m_fHP_RATE += sSKEFF.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_MP_RATE:			m_fMP_RATE += sSKEFF.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_SP_RATE:			m_fSP_RATE += sSKEFF.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_RESIST:			m_sSUMRESIST_SKILL += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);		break;
			};
		}

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			switch ( sSKEFF.sSpecs[nSpec].emSPEC )
			{
			case EMSPECA_NONBLOW:		m_dwHOLDBLOW = sSKEFF.sSpecs[nSpec].dwSPECFLAG;			break;
			case EMSPECA_PIERCE:		m_nSUM_PIERCE += int ( sSKEFF.sSpecs[nSpec].fSPECVAR1 );	break;
			case EMSPECA_TARRANGE:		m_fSUM_TARGET_RANGE += sSKEFF.sSpecs[nSpec].fSPECVAR1;		break;
			case EMSPECA_MOVEVELO:		m_fSKILL_MOVE += sSKEFF.sSpecs[nSpec].fSPECVAR1;		break;
			
				/* skill turn/find invisible, Juver, 2020/04/25 */
			case EMSPECA_TURN_INVISIBLE:
				{
					if ( !m_bSkillTurnInvisible )
					{
						m_bSkillTurnInvisible = true;
						m_wSkillTurnInvisibleLevel = WORD(sSKEFF.sSpecs[nSpec].fSPECVAR1);
						m_wSkillTurnInvisibleAnimation = WORD(sSKEFF.sSpecs[nSpec].fSPECVAR2);

						if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMSPEC_INVISIBLE_USE_DEFINE_DISABLE_IN_COMBAT )
							m_bSkillTurnInvisibleDisableInCombat = true;

						m_sSkillTurnInvisibleID = sSKEFF.sNATIVEID;
					}
				}break;

				/* skill turn/find invisible, Juver, 2020/04/25 */
			case EMSPECA_SHOW_INVISIBLE:
				{
					if ( !m_bSkillShowInvisible )
					{
						m_bSkillShowInvisible = true;
						m_wSkillShowInvisibleLevel = WORD(sSKEFF.sSpecs[nSpec].fSPECVAR1);
						m_wSkillShowInvisibleRange = WORD(sSKEFF.sSpecs[nSpec].fSPECVAR2);
					}
				}break;

				//	공격 속도 계념이므로 ( 공격 시간을 -0.1 (-10%) 감소 시키기 위해서는 부호 반전을 하여야함. )
			case EMSPECA_ATTACKVELO:	m_fATTVELO -= sSKEFF.sSpecs[nSpec].fSPECVAR1;			break;

				/* skill delay addition, Juver, 2020/12/08 */
			case EMSPECA_SKILLDELAY:
				{
					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_OLD )
						m_fSKILLDELAY += sSKEFF.sSpecs[nSpec].fSPECVAR1;

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_DURATION )
					{
						if ( !SkillDelayDataExist(sSKEFF.sNATIVEID) )
						{
							SSKILL_DELAY_DATA _sDATA;
							_sDATA.sSkillID = sSKEFF.sNATIVEID;
							_sDATA.fDelay	= sSKEFF.sSpecs[nSpec].fSPECVAR1;
							_sDATA.fChance	= sSKEFF.sSpecs[nSpec].fSPECVAR2;
							SkillDelayDataInsert( _sDATA );
						}
					}
				}break;

				/*prohibit potion skill logic, Juver, 2017/06/06 */
			case EMSPECA_PROHIBIT_POTION:	m_bProhibitPotion = true;			break;

				/*prohibit skill logic, Juver, 2017/06/06 */
			case EMSPECA_PROHIBIT_SKILL:	m_bProhibitSkill = true;			break;

			case EMSPECA_PSY_DAMAGE_REDUCE:
				if ( m_sDamageSpec.m_fPsyDamageReduce < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
					m_sDamageSpec.m_fPsyDamageReduce = sSKEFF.sSpecs[nSpec].fSPECVAR1;
				break;

			case EMSPECA_MAGIC_DAMAGE_REDUCE:	
				if ( m_sDamageSpec.m_fMagicDamageReduce < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
					m_sDamageSpec.m_fMagicDamageReduce = sSKEFF.sSpecs[nSpec].fSPECVAR1;		
				break;

			case EMSPECA_PSY_DAMAGE_REFLECTION:	
				if ( m_sDamageSpec.m_fPsyDamageReflection < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
				{
					m_sDamageSpec.m_fPsyDamageReflection = sSKEFF.sSpecs[nSpec].fSPECVAR1;		
					m_sDamageSpec.m_fPsyDamageReflectionRate = sSKEFF.sSpecs[nSpec].fSPECVAR2;		
				}break;

			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
				if ( m_sDamageSpec.m_fMagicDamageReflection < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
				{
					m_sDamageSpec.m_fMagicDamageReflection = sSKEFF.sSpecs[nSpec].fSPECVAR1;	
					m_sDamageSpec.m_fMagicDamageReflectionRate = sSKEFF.sSpecs[nSpec].fSPECVAR2;
				}break;

			case EMSPECA_DEFENSE_SKILL_ACTIVE:
				{
					m_sDefenseSkill.m_dwSkillID = sSKEFF.sSpecs[nSpec].dwNativeID;
					m_sDefenseSkill.m_wLevel = sSKEFF.sSpecs[nSpec].dwSPECFLAG;
					m_sDefenseSkill.m_fRate = sSKEFF.sSpecs[nSpec].fSPECVAR1;
				}break;

				/*skill range spec logic, Juver, 2017/06/06 */
			case EMSPECA_CHANGE_ATTACK_RANGE:
				{
					m_fSUM_ATTACK_RANGE += sSKEFF.sSpecs[nSpec].fSPECVAR1;		
				}break;
			case EMSPECA_CHANGE_APPLY_RANGE:
				{
					m_fSUM_APPLY_RANGE += sSKEFF.sSpecs[nSpec].fSPECVAR1;		
				}break;

				/*skill stun logic, Juver, 2017/06/06 */
			case EMSPECA_STUN:
				{
					m_fSTATE_MOVE = 0.0f;
					m_bSTATE_STUN = true;
				}break;

				/*continuous damage skill logic, Juver, 2017/06/10 */
			case EMSPECA_CONTINUOUS_DAMAGE:
				{
					if ( !CONTINUOUSDAMAGE_EXIST(sSKEFF.sNATIVEID) )
					{
						SCONTINUOUS_DAMAGE_DATA _sDATA;
						_sDATA.sidSkill = sSKEFF.sNATIVEID;
						_sDATA.fInterval = sSKEFF.sSpecs[nSpec].fSPECVAR1;
						_sDATA.fVar = sSKEFF.sSpecs[nSpec].fSPECVAR2;
						_sDATA.wCrow = sSKEFF._wCasterCrow;
						_sDATA.dwCrowID = sSKEFF._dwCasterID;
						_sDATA.fAge = 0.0f;

						CONTINUOUSDAMAGE_INSERT( _sDATA );
					}
				}break;

				/*curse skill logic, Juver, 2017/06/10 */
			case EMSPECA_CURSE:
				{
					m_bCurse = true;
					m_fCurseDamage += sSKEFF.sSpecs[nSpec].fSPECVAR1;
				}break;

				/*ignore damage, Juver, 2017/12/12 */
			case EMSPECA_IGNORE_DAMAGE:
				{
					m_bIgnoreDamage = true;
				}break;

				/*item drop rate buff, Juver, 2018/07/01 */
			case EMSPECA_ITEMDROP_RATE:
				{
					m_fGEN_RATE += ( sSKEFF.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*gold drop buff, Juver, 2018/07/01 */
			case EMSPECA_MONEYDROP_RATE:
				{
					m_fGOLD_RATE += ( sSKEFF.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*exp rate buff, Juver, 2018/07/01 */
			case EMSPECA_GETEXP_RATE:
				{
					m_fEXP_RATE += ( sSKEFF.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*skill vehicle off, Juver, 2018/09/07 */
			case EMSPECA_VEHICLE_OFF:
				{
					m_skill_vehicle_off = true;			
				}break;

				/*skill pet off, Juver, 2018/09/07 */
			case EMSPECA_PET_OFF:
				{
					m_skill_pet_off = true;
				}break;

				/*skill transform, Juver, 2018/09/08 */
			case EMSPECA_TRANSFORM:
				{
					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMSPEC_TRANSFORM_TYPE_CHANGE_SKIN )
					{
						m_skill_transform_data.use_skin_change = true;
						m_skill_transform_data.skin_change_id = byte(sSKEFF.sSpecs[nSpec].fSPECVAR1);
					}

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMSPEC_TRANSFORM_TYPE_FORCE_ANIM )
					{
						m_skill_transform_data.use_force_anim = true;
						m_skill_transform_data.force_anim_id = byte(sSKEFF.sSpecs[nSpec].fSPECVAR2);
					}
				}break;

				/* skill effect release, Juver, 2020/04/30 */
			case EMSPECA_EFFECT_RELEASE:
				{
					if ( !EffectReleaseExist(sSKEFF.sNATIVEID) )
					{
						SEFFECT_RELEASE_DATA _sDATA;
						_sDATA.sidSkill = sSKEFF.sNATIVEID;
						_sDATA.fRate = sSKEFF.sSpecs[nSpec].fSPECVAR1;
						_sDATA.dwCondition = DWORD(sSKEFF.sSpecs[nSpec].fSPECVAR2);
						EffectReleaseInsert( _sDATA );
					}
				}break;

				/* skill amplify, Juver, 2020/12/10 */
			case EMSPECA_AMPLIFY:
				{
					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMAPPLY_FLAG_PHY_SHORT )
					{
						if ( m_sSkillAmplifyMelee.fCriticalRate < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
							m_sSkillAmplifyMelee.fCriticalRate = sSKEFF.sSpecs[nSpec].fSPECVAR1;	

						if ( m_sSkillAmplifyMelee.fDamageRate < sSKEFF.sSpecs[nSpec].fSPECVAR2 )
							m_sSkillAmplifyMelee.fDamageRate = sSKEFF.sSpecs[nSpec].fSPECVAR2;	
					}

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMAPPLY_FLAG_PHY_LONG )
					{
						if ( m_sSkillAmplifyMissile.fCriticalRate < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
							m_sSkillAmplifyMissile.fCriticalRate = sSKEFF.sSpecs[nSpec].fSPECVAR1;	

						if ( m_sSkillAmplifyMissile.fDamageRate < sSKEFF.sSpecs[nSpec].fSPECVAR2 )
							m_sSkillAmplifyMissile.fDamageRate = sSKEFF.sSpecs[nSpec].fSPECVAR2;	
					}

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMAPPLY_FLAG_MAGIC )
					{
						if ( m_sSkillAmplifyMagic.fCriticalRate < sSKEFF.sSpecs[nSpec].fSPECVAR1 )
							m_sSkillAmplifyMagic.fCriticalRate = sSKEFF.sSpecs[nSpec].fSPECVAR1;	

						if ( m_sSkillAmplifyMagic.fDamageRate < sSKEFF.sSpecs[nSpec].fSPECVAR2 )
							m_sSkillAmplifyMagic.fDamageRate = sSKEFF.sSpecs[nSpec].fSPECVAR2;	
					}

				}break;

				/* skill hostile, Juver, 2020/12/16 */
			case EMSPECA_HOSTILE_ON:
				{
					m_bSkillHostile = true;			
				}break;

				/* skill hostile, Juver, 2020/12/16 */
			case EMSPECA_HOSTILE_OFF:
				{
					m_bSkillHostile = false;			
				}break;

				/* skill buff duration change, Juver, 2020/12/17 */
			case EMSPECA_DURATION_CHANGE:
				{
					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMACTION_FLAG_NORMAL )
						m_sSkillDurationChange.fSecondsNormal += sSKEFF.sSpecs[nSpec].fSPECVAR2;	

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMACTION_FLAG_BUFF )
						m_sSkillDurationChange.fSecondsBuff += sSKEFF.sSpecs[nSpec].fSPECVAR2;	

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMACTION_FLAG_DEBUFF )
						m_sSkillDurationChange.fSecondsDebuff += sSKEFF.sSpecs[nSpec].fSPECVAR2;	

					if ( sSKEFF.sSpecs[nSpec].dwSPECFLAG & SKILL::EMACTION_FLAG_LIMIT )
						m_sSkillDurationChange.fSecondsLimit += sSKEFF.sSpecs[nSpec].fSPECVAR2;	

				}break;

				/* immune skill logic, Juver, 2020/12/24 */
			case EMSPECA_IMMUNE:
				{
					m_sDamageSpec.m_dwImmuneApplyType |= DWORD(sSKEFF.sSpecs[nSpec].fSPECVAR1);
					m_sDamageSpec.m_dwImmuneActionType |= DWORD(sSKEFF.sSpecs[nSpec].fSPECVAR2);
				}break;

				/* skill illusion, Juver, 2021/01/17 */
			case EMSPECA_ILLUSION:
				{
					m_sSkillIllusion.bActive = TRUE;
				}break;

			};
		}
		
	}

	for ( int i=0; i<EMBLOW_MULTI; ++i )
	{
		SSTATEBLOW &sSTATEBLOW = m_sSTATEBLOWS[i];
		if ( sSTATEBLOW.emBLOW == EMBLOW_NONE )		continue;

		m_bSTATEBLOW = TRUE;
		sSTATEBLOW.fAGE -= fElapsedTime;
		if ( !bClient && sSTATEBLOW.fAGE <= 0.0f )	DISABLEBLOW(i);

		switch ( sSTATEBLOW.emBLOW )
		{
		case EMBLOW_NUMB:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	이동속도 감소.
			m_fSTATE_DELAY += sSTATEBLOW.fSTATE_VAR2;			//	딜래이 증가.
			break;

		case EMBLOW_STUN:
			m_fSTATE_MOVE = 0.0f;								//	기절.
			m_bSTATE_PANT = true;
			m_bSTATE_STUN = true;
			break;

		case EMBLOW_STONE:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	이동속도 감소.
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			break;

		case EMBLOW_BURN:
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			break;

		case EMBLOW_FROZEN:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	이동속도 감소.
			m_fSTATE_DAMAGE += sSTATEBLOW.fSTATE_VAR2;			//	대미지 증가.
			break;

		case EMBLOW_MAD:
			m_nSUM_HIT = 30;
			m_nSUM_AVOID = 30;
			m_bSTATE_PANT = true;
			break;

		case EMBLOW_POISON:
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			break;

		case EMBLOW_CURSE:
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			m_fINCR_MP = 0.0f;
			break;
		};
	}

	if ( m_sQITEMFACT.IsACTIVE() )
	{
 		switch ( m_sQITEMFACT.emType )
		{
		case QUESTION_SPEED_UP:		m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_CRAZY:
			m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);
			m_fATTVELO += (m_sQITEMFACT.wParam2/100.0f);
			break;
		case QUESTION_ATTACK_UP:	m_fDamageRate += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_EXP_UP:		m_fEXP_RATE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_LUCKY:		m_fGEN_RATE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_SPEED_UP_M:	m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_MADNESS:
			m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);
			m_fATTVELO += (m_sQITEMFACT.wParam2/100.0f);
			break;
		case QUESTION_ATTACK_UP_M:	m_fDamageRate += (m_sQITEMFACT.wParam1/100.0f);		break;
		};
	}

	//	Memo :	? 아이템 이벤트 적용
	if( m_sEVENTFACT.IsACTIVE( EMGM_EVENT_SPEED ) )		m_fSKILL_MOVE += (m_sEVENTFACT.wSpeed/100.0f);
	if( m_sEVENTFACT.IsACTIVE( EMGM_EVENT_ASPEED ) )	m_fATTVELO += (m_sEVENTFACT.wASpeed/100.0f);
	if( m_sEVENTFACT.IsACTIVE( EMGM_EVENT_ATTACK ) )	m_fDamageRate += (m_sEVENTFACT.wAttack/100.0f);

	// 팻의 보조스킬 처리
	/*dual pet skill, Juver, 2017/12/27 */
	if ( m_sPETSKILLFACT_A.sNATIVEID !=NATIVEID_NULL() )
	{
		switch ( m_sPETSKILLFACT_A.emTYPE )
		{
		case SKILL::EMFOR_PET_HEAL:				//	주인의 HP 회복 속도 상승
			fINCR_HP += m_sPETSKILLFACT_A.fMVAR;
			break;
		case SKILL::EMFOR_PET_SUPPROT:			//	주인의 HP, MP, SP 회복률 상승
			fINCR_HP += m_sPETSKILLFACT_A.fMVAR;
			fINCR_MP += m_sPETSKILLFACT_A.fMVAR;
			fINCR_SP += m_sPETSKILLFACT_A.fMVAR;
			break;
		case SKILL::EMFOR_PET_BACKUP_ATK:		//	주인의 공격력(율) 상승
			m_fDamageRate += m_sPETSKILLFACT_A.fMVAR/100.0f;
			break;
		case SKILL::EMFOR_PET_BACKUP_DEF:		//	주인의 방어력(율) 상승
			m_fDefenseRate += m_sPETSKILLFACT_A.fMVAR/100.0f;
			break;
		};

		// 스킬 라이프타임 갱신
		m_sPETSKILLFACT_A.fAGE -= fElapsedTime;
	}

	// 팻의 보조스킬 처리
	/*dual pet skill, Juver, 2017/12/27 */
	if ( m_sPETSKILLFACT_B.sNATIVEID !=NATIVEID_NULL() )
	{
		switch ( m_sPETSKILLFACT_B.emTYPE )
		{
		case SKILL::EMFOR_PET_HEAL:				//	주인의 HP 회복 속도 상승
			fINCR_HP += m_sPETSKILLFACT_B.fMVAR;
			break;
		case SKILL::EMFOR_PET_SUPPROT:			//	주인의 HP, MP, SP 회복률 상승
			fINCR_HP += m_sPETSKILLFACT_B.fMVAR;
			fINCR_MP += m_sPETSKILLFACT_B.fMVAR;
			fINCR_SP += m_sPETSKILLFACT_B.fMVAR;
			break;
		case SKILL::EMFOR_PET_BACKUP_ATK:		//	주인의 공격력(율) 상승
			m_fDamageRate += m_sPETSKILLFACT_B.fMVAR/100.0f;
			break;
		case SKILL::EMFOR_PET_BACKUP_DEF:		//	주인의 방어력(율) 상승
			m_fDefenseRate += m_sPETSKILLFACT_B.fMVAR/100.0f;
			break;
		};

		// 스킬 라이프타임 갱신
		m_sPETSKILLFACT_B.fAGE -= fElapsedTime;
	}



	// 지형 효과 처리
	for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
	{
		SLANDEFFECT landEffect = m_sLandEffect[i];
		if( !landEffect.IsUse() ) continue;

		switch( landEffect.emLandEffectType ) 
		{
		case EMLANDEFFECT_ATK_SPEED: // 공격속도
			m_fATTVELO	  += landEffect.fValue;
			break;
		case EMLANDEFFECT_MOVE_SPEED: // 이동속도
			m_fSKILL_MOVE += landEffect.fValue;
			break;
		case EMLANDEFFECT_MP_RATE:	  // HP 변화율
			m_fHP_RATE    += landEffect.fValue;
			break;
		case EMLANDEFFECT_HP_RATE:    // MP 변화율
			m_fMP_RATE    += landEffect.fValue;
			break;
		case EMLANDEFFECT_RECOVER_RATE: // 회복율
			fINCR_HP	  += landEffect.fValue;
			fINCR_MP	  += landEffect.fValue;
			fINCR_SP	  += landEffect.fValue;
			break;
		case EMLANDEFFECT_DAMAGE_RATE:	// 공격치 변화율
			m_fDamageRate += landEffect.fValue;
			break;
		case EMLANDEFFECT_DEFENSE_RATE:	// 방어치 변화율
			m_fDefenseRate += landEffect.fValue;
			break;
		case EMLANDEFFECT_RESIST_RATE:	// 저항 수치			
			m_sSUM_PASSIVE.m_sSUMRESIST.nFire += landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nIce += landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nElectric += landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nPoison += landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nSpirit += landEffect.fValue;
			break;
		case EMLANDEFFECT_CANCEL_ALLBUFF: // 모든 버프 취소
//			for ( i=0; i<EMBLOW_MULTI; ++i )		DISABLEBLOW ( i );
//			for ( i=0; i<SKILLFACT_SIZE; ++i )		DISABLESKEFF ( i );
			break;
		case EMLANDEFFECT_SAFE_ZONE:
			m_bSafeZone = true;
			break;

			/* non vend area, Juver, 2021/01/17 */
		case EMLANDEFFECT_NON_VEND:
			m_bNonVendArea = true;
			break;
		}
	}

	/*itemfood system, Juver, 2017/05/24 */
	for ( int i=0; i<FITEMFACT_SIZE; ++i )
	{
		SFITEMFACT &sFITEMEFF = m_sFITEMFACT[i];
		if ( sFITEMEFF.sNATIVEID==NATIVEID_NULL() )	continue;

		sFITEMEFF.fAGE -= fElapsedTime;

		if ( sFITEMEFF.fAGE <= 0.0f )
		{
			 m_sFITEMFACT[i].RESET();
		}

		switch ( sFITEMEFF.emTYPE )
		{
		case SKILL::EMFOR_VARHP:	fINCR_HP += sFITEMEFF.fMVAR;	break;
		case SKILL::EMFOR_VARMP:	fINCR_MP += sFITEMEFF.fMVAR;	break;
		case SKILL::EMFOR_VARSP:	fINCR_SP += sFITEMEFF.fMVAR;	break;
		case SKILL::EMFOR_VARAP:
			fINCR_HP += sFITEMEFF.fMVAR;
			fINCR_MP += sFITEMEFF.fMVAR;
			fINCR_SP += sFITEMEFF.fMVAR;
			break;

		case SKILL::EMFOR_DEFENSE:		m_nDEFENSE_SKILL += (int) sFITEMEFF.fMVAR;			break;
		case SKILL::EMFOR_HITRATE:		m_nSUM_HIT += int(sFITEMEFF.fMVAR);					break;
		case SKILL::EMFOR_AVOIDRATE:	m_nSUM_AVOID += int(sFITEMEFF.fMVAR);				break;
		case SKILL::EMFOR_VARDAMAGE:	m_gdDAMAGE_SKILL.VAR_PARAM ( int(sFITEMEFF.fMVAR) );	break;
		case SKILL::EMFOR_VARDEFENSE:	m_nDEFENSE_SKILL += int(sFITEMEFF.fMVAR);			break;
		case SKILL::EMFOR_PA:			nSUM_PA += int(sFITEMEFF.fMVAR);					break;
		case SKILL::EMFOR_SA:			nSUM_SA += int(sFITEMEFF.fMVAR);					break;
		case SKILL::EMFOR_MA:			nSUM_MA += int(sFITEMEFF.fMVAR);					break;
		case SKILL::EMFOR_HP_RATE:		m_fHP_RATE += sFITEMEFF.fMVAR;						break;
		case SKILL::EMFOR_MP_RATE:		m_fMP_RATE += sFITEMEFF.fMVAR;						break;
		case SKILL::EMFOR_SP_RATE:		m_fSP_RATE += sFITEMEFF.fMVAR;						break;
		case SKILL::EMFOR_RESIST:		m_sSUMRESIST_SKILL += int(sFITEMEFF.fMVAR);			break;	
		case SKILL::EMFOR_SUMMONTIME:	m_nSummonTime += (int) sFITEMEFF.fMVAR;				break;	/*summon time, Juver, 2017/12/12 */
		};

		for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
		{
			switch ( sFITEMEFF.sImpacts[nImpact].emADDON )
			{
			case EMIMPACTA_HITRATE:			m_nSUM_HIT += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);					break;
			case EMIMPACTA_AVOIDRATE:		m_nSUM_AVOID += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_DAMAGE:			m_gdDAMAGE_SKILL.VAR_PARAM ( int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR) );	break;
			case EMIMPACTA_DEFENSE:			m_nDEFENSE_SKILL += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);			break;
			case EMIMPACTA_VARHP:			fINCR_HP += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARMP:			fINCR_MP += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARSP:			fINCR_SP += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARAP:
				fINCR_HP += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;
				fINCR_MP += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;
				fINCR_SP += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;
				break;

			case EMIMPACTA_DAMAGE_RATE:		m_fDamageRate += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;				break;
			case EMIMPACTA_DEFENSE_RATE:	m_fDefenseRate += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;				break;		

			case EMIMPACTA_PA:				nSUM_PA += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_SA:				nSUM_SA += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_MA:				nSUM_MA += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_HP_RATE:			m_fHP_RATE += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_MP_RATE:			m_fMP_RATE += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_SP_RATE:			m_fSP_RATE += sFITEMEFF.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_RESIST:			m_sSUMRESIST_SKILL += int(sFITEMEFF.sImpacts[nImpact].fADDON_VAR);		break;
			};
		}

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			switch ( sFITEMEFF.sSpecs[nSpec].emSPEC )
			{
			case EMSPECA_NONBLOW:		m_dwHOLDBLOW = sFITEMEFF.sSpecs[nSpec].dwSPECFLAG;			break;
			case EMSPECA_PIERCE:		m_nSUM_PIERCE += int ( sFITEMEFF.sSpecs[nSpec].fSPECVAR1 );	break;
			case EMSPECA_TARRANGE:		m_fSUM_TARGET_RANGE += sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		break;
			case EMSPECA_MOVEVELO:		m_fSKILL_MOVE += sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		break;
			
				//	공격 속도 계념이므로 ( 공격 시간을 -0.1 (-10%) 감소 시키기 위해서는 부호 반전을 하여야함. )
			case EMSPECA_ATTACKVELO:	m_fATTVELO -= sFITEMEFF.sSpecs[nSpec].fSPECVAR1;			break;

				/* skill delay addition, Juver, 2020/12/08 */
			case EMSPECA_SKILLDELAY:
				{
					if ( sFITEMEFF.sSpecs[nSpec].dwSPECFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_OLD )
						m_fSKILLDELAY += sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		

					if ( sFITEMEFF.sSpecs[nSpec].dwSPECFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_DURATION )
					{
						if ( !SkillDelayDataExist(sFITEMEFF.sNATIVEID) )
						{
							SSKILL_DELAY_DATA _sDATA;
							_sDATA.sSkillID = sFITEMEFF.sNATIVEID;
							_sDATA.fDelay	= sFITEMEFF.sSpecs[nSpec].fSPECVAR1;
							_sDATA.fChance	= sFITEMEFF.sSpecs[nSpec].fSPECVAR2;
							SkillDelayDataInsert( _sDATA );
						}
					}
				}break;

			case EMSPECA_PSY_DAMAGE_REDUCE:
				if ( m_sDamageSpec.m_fPsyDamageReduce < sFITEMEFF.sSpecs[nSpec].fSPECVAR1 )
					m_sDamageSpec.m_fPsyDamageReduce = sFITEMEFF.sSpecs[nSpec].fSPECVAR1;
				break;

			case EMSPECA_MAGIC_DAMAGE_REDUCE:	
				if ( m_sDamageSpec.m_fMagicDamageReduce < sFITEMEFF.sSpecs[nSpec].fSPECVAR1 )
					m_sDamageSpec.m_fMagicDamageReduce = sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		
				break;

			case EMSPECA_PSY_DAMAGE_REFLECTION:	
				if ( m_sDamageSpec.m_fPsyDamageReflection < sFITEMEFF.sSpecs[nSpec].fSPECVAR1 )
				{
					m_sDamageSpec.m_fPsyDamageReflection = sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		
					m_sDamageSpec.m_fPsyDamageReflectionRate = sFITEMEFF.sSpecs[nSpec].fSPECVAR2;		
				}break;

			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
				if ( m_sDamageSpec.m_fMagicDamageReflection < sFITEMEFF.sSpecs[nSpec].fSPECVAR1 )
				{
					m_sDamageSpec.m_fMagicDamageReflection = sFITEMEFF.sSpecs[nSpec].fSPECVAR1;	
					m_sDamageSpec.m_fMagicDamageReflectionRate = sFITEMEFF.sSpecs[nSpec].fSPECVAR2;
				}break;

			case EMSPECA_DEFENSE_SKILL_ACTIVE:
				{
					m_sDefenseSkill.m_dwSkillID = sFITEMEFF.sSpecs[nSpec].dwNativeID;
					m_sDefenseSkill.m_wLevel = sFITEMEFF.sSpecs[nSpec].dwSPECFLAG;
					m_sDefenseSkill.m_fRate = sFITEMEFF.sSpecs[nSpec].fSPECVAR1;
				}break;

				/*skill range spec logic, Juver, 2017/06/06 */
			case EMSPECA_CHANGE_ATTACK_RANGE:
				{
					m_fSUM_ATTACK_RANGE += sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		
				}break;
			case EMSPECA_CHANGE_APPLY_RANGE:
				{
					m_fSUM_APPLY_RANGE += sFITEMEFF.sSpecs[nSpec].fSPECVAR1;		
				}break;

				/*item drop rate buff, Juver, 2018/07/01 */
			case EMSPECA_ITEMDROP_RATE:
				{
					m_fGEN_RATE += ( sFITEMEFF.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*gold drop buff, Juver, 2018/07/01 */
			case EMSPECA_MONEYDROP_RATE:
				{
					m_fGOLD_RATE += ( sFITEMEFF.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*exp rate buff, Juver, 2018/07/01 */
			case EMSPECA_GETEXP_RATE:
				{
					m_fEXP_RATE += ( sFITEMEFF.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;
			};
		}
	}

	/*system buffs, Juver, 2017/09/04 */
	for ( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
	{
		SSYSTEM_BUFF &ssystem_buff = m_sSYSTEM_BUFF[i];
		if ( ssystem_buff.sNATIVEID==NATIVEID_NULL() )	continue;

		switch ( ssystem_buff.emTYPE )
		{
		case SKILL::EMFOR_VARHP:	fINCR_HP += ssystem_buff.fMVAR;	break;
		case SKILL::EMFOR_VARMP:	fINCR_MP += ssystem_buff.fMVAR;	break;
		case SKILL::EMFOR_VARSP:	fINCR_SP += ssystem_buff.fMVAR;	break;
		case SKILL::EMFOR_VARAP:
			fINCR_HP += ssystem_buff.fMVAR;
			fINCR_MP += ssystem_buff.fMVAR;
			fINCR_SP += ssystem_buff.fMVAR;
			break;

		case SKILL::EMFOR_DEFENSE:		m_nDEFENSE_SKILL += (int) ssystem_buff.fMVAR;			break;
		case SKILL::EMFOR_HITRATE:		m_nSUM_HIT += int(ssystem_buff.fMVAR);					break;
		case SKILL::EMFOR_AVOIDRATE:	m_nSUM_AVOID += int(ssystem_buff.fMVAR);				break;
		case SKILL::EMFOR_VARDAMAGE:	m_gdDAMAGE_SKILL.VAR_PARAM ( int(ssystem_buff.fMVAR) );	break;
		case SKILL::EMFOR_VARDEFENSE:	m_nDEFENSE_SKILL += int(ssystem_buff.fMVAR);			break;
		case SKILL::EMFOR_PA:			nSUM_PA += int(ssystem_buff.fMVAR);						break;
		case SKILL::EMFOR_SA:			nSUM_SA += int(ssystem_buff.fMVAR);						break;
		case SKILL::EMFOR_MA:			nSUM_MA += int(ssystem_buff.fMVAR);						break;
		case SKILL::EMFOR_HP_RATE:		m_fHP_RATE += ssystem_buff.fMVAR;						break;
		case SKILL::EMFOR_MP_RATE:		m_fMP_RATE += ssystem_buff.fMVAR;						break;
		case SKILL::EMFOR_SP_RATE:		m_fSP_RATE += ssystem_buff.fMVAR;						break;
		case SKILL::EMFOR_RESIST:		m_sSUMRESIST_SKILL += int(ssystem_buff.fMVAR);			break;
		case SKILL::EMFOR_SUMMONTIME:	m_nSummonTime += (int) ssystem_buff.fMVAR;				break;	/*summon time, Juver, 2017/12/12 */
		};

		for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
		{
			switch ( ssystem_buff.sImpacts[nImpact].emADDON )
			{
			case EMIMPACTA_HITRATE:			m_nSUM_HIT += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_AVOIDRATE:		m_nSUM_AVOID += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_DAMAGE:			m_gdDAMAGE_SKILL.VAR_PARAM ( int(ssystem_buff.sImpacts[nImpact].fADDON_VAR) );	break;
			case EMIMPACTA_DEFENSE:			m_nDEFENSE_SKILL += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);			break;
			case EMIMPACTA_VARHP:			fINCR_HP += ssystem_buff.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARMP:			fINCR_MP += ssystem_buff.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARSP:			fINCR_SP += ssystem_buff.sImpacts[nImpact].fADDON_VAR;						break;
			case EMIMPACTA_VARAP:
				fINCR_HP += ssystem_buff.sImpacts[nImpact].fADDON_VAR;
				fINCR_MP += ssystem_buff.sImpacts[nImpact].fADDON_VAR;
				fINCR_SP += ssystem_buff.sImpacts[nImpact].fADDON_VAR;
				break;

			case EMIMPACTA_DAMAGE_RATE:		m_fDamageRate += ssystem_buff.sImpacts[nImpact].fADDON_VAR;				break;
			case EMIMPACTA_DEFENSE_RATE:	m_fDefenseRate += ssystem_buff.sImpacts[nImpact].fADDON_VAR;				break;		

			case EMIMPACTA_PA:				nSUM_PA += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_SA:				nSUM_SA += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_MA:				nSUM_MA += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);				break;
			case EMIMPACTA_HP_RATE:			m_fHP_RATE += ssystem_buff.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_MP_RATE:			m_fMP_RATE += ssystem_buff.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_SP_RATE:			m_fSP_RATE += ssystem_buff.sImpacts[nImpact].fADDON_VAR;					break;
			case EMIMPACTA_RESIST:			m_sSUMRESIST_SKILL += int(ssystem_buff.sImpacts[nImpact].fADDON_VAR);		break;
			};
		}

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			switch ( ssystem_buff.sSpecs[nSpec].emSPEC )
			{
			case EMSPECA_NONBLOW:		m_dwHOLDBLOW = ssystem_buff.sSpecs[nSpec].dwSPECFLAG;			break;
			case EMSPECA_PIERCE:		m_nSUM_PIERCE += int ( ssystem_buff.sSpecs[nSpec].fSPECVAR1 );	break;
			case EMSPECA_TARRANGE:		m_fSUM_TARGET_RANGE += ssystem_buff.sSpecs[nSpec].fSPECVAR1;		break;
			case EMSPECA_MOVEVELO:		m_fSKILL_MOVE += ssystem_buff.sSpecs[nSpec].fSPECVAR1;		break;
			
				//	공격 속도 계념이므로 ( 공격 시간을 -0.1 (-10%) 감소 시키기 위해서는 부호 반전을 하여야함. )
			case EMSPECA_ATTACKVELO:	m_fATTVELO -= ssystem_buff.sSpecs[nSpec].fSPECVAR1;			break;

				/* skill delay addition, Juver, 2020/12/08 */
			case EMSPECA_SKILLDELAY:	
				{
					if ( ssystem_buff.sSpecs[nSpec].dwSPECFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_OLD )
						m_fSKILLDELAY += ssystem_buff.sSpecs[nSpec].fSPECVAR1;		

					if ( ssystem_buff.sSpecs[nSpec].dwSPECFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_DURATION )
					{
						if ( !SkillDelayDataExist(ssystem_buff.sNATIVEID) )
						{
							SSKILL_DELAY_DATA _sDATA;
							_sDATA.sSkillID = ssystem_buff.sNATIVEID;
							_sDATA.fDelay	= ssystem_buff.sSpecs[nSpec].fSPECVAR1;
							_sDATA.fChance	= ssystem_buff.sSpecs[nSpec].fSPECVAR2;
							SkillDelayDataInsert( _sDATA );
						}
					}
				}break;

			case EMSPECA_PSY_DAMAGE_REDUCE:
				if ( m_sDamageSpec.m_fPsyDamageReduce < ssystem_buff.sSpecs[nSpec].fSPECVAR1 )
					m_sDamageSpec.m_fPsyDamageReduce = ssystem_buff.sSpecs[nSpec].fSPECVAR1;
				break;

			case EMSPECA_MAGIC_DAMAGE_REDUCE:	
				if ( m_sDamageSpec.m_fMagicDamageReduce < ssystem_buff.sSpecs[nSpec].fSPECVAR1 )
					m_sDamageSpec.m_fMagicDamageReduce = ssystem_buff.sSpecs[nSpec].fSPECVAR1;		
				break;

			case EMSPECA_PSY_DAMAGE_REFLECTION:	
				if ( m_sDamageSpec.m_fPsyDamageReflection < ssystem_buff.sSpecs[nSpec].fSPECVAR1 )
				{
					m_sDamageSpec.m_fPsyDamageReflection = ssystem_buff.sSpecs[nSpec].fSPECVAR1;		
					m_sDamageSpec.m_fPsyDamageReflectionRate = ssystem_buff.sSpecs[nSpec].fSPECVAR2;		
				}break;

			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
				if ( m_sDamageSpec.m_fMagicDamageReflection < ssystem_buff.sSpecs[nSpec].fSPECVAR1 )
				{
					m_sDamageSpec.m_fMagicDamageReflection = ssystem_buff.sSpecs[nSpec].fSPECVAR1;	
					m_sDamageSpec.m_fMagicDamageReflectionRate = ssystem_buff.sSpecs[nSpec].fSPECVAR2;
				}break;

			case EMSPECA_DEFENSE_SKILL_ACTIVE:
				{
					m_sDefenseSkill.m_dwSkillID = ssystem_buff.sSpecs[nSpec].dwNativeID;
					m_sDefenseSkill.m_wLevel = ssystem_buff.sSpecs[nSpec].dwSPECFLAG;
					m_sDefenseSkill.m_fRate = ssystem_buff.sSpecs[nSpec].fSPECVAR1;
				}break;

				/*skill range spec logic, Juver, 2017/06/06 */
			case EMSPECA_CHANGE_ATTACK_RANGE:
				{
					m_fSUM_ATTACK_RANGE += ssystem_buff.sSpecs[nSpec].fSPECVAR1;		
				}break;
			case EMSPECA_CHANGE_APPLY_RANGE:
				{
					m_fSUM_APPLY_RANGE += ssystem_buff.sSpecs[nSpec].fSPECVAR1;		
				}break;

				/*item drop rate buff, Juver, 2018/07/01 */
			case EMSPECA_ITEMDROP_RATE:
				{
					m_fGEN_RATE += ( ssystem_buff.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*gold drop buff, Juver, 2018/07/01 */
			case EMSPECA_MONEYDROP_RATE:
				{
					m_fGOLD_RATE += ( ssystem_buff.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;

				/*exp rate buff, Juver, 2018/07/01 */
			case EMSPECA_GETEXP_RATE:
				{
					m_fEXP_RATE += ( ssystem_buff.sSpecs[nSpec].fSPECVAR1 * 100.0f );
				}break;
			};
		}
	}

	if ( m_fDamageRate < 0.0f )		m_fDamageRate = 0.0f;
	if ( m_fDefenseRate < 0.0f )	m_fDefenseRate = 0.0f;

	if ( m_fHP_RATE < -1.0f )		m_fHP_RATE = -0.9f;
	if ( m_fMP_RATE < -1.0f )		m_fMP_RATE = -0.9f;
	if ( m_fSP_RATE < -1.0f )		m_fSP_RATE = -0.9f;

	/*vehicle booster system, Juver, 2017/08/11 */
	if ( m_bBoosterStart )	m_fSKILL_MOVE += ( GLCONST_CHAR::fVehicleBoosterAddSpeed /100.0f );

	//	Note : HP, MP, SP 총량 확정.
	//
	UPDATE_MAX_POINT ( fCONFT_POINT_RATE );

	//	격투치 사격치 마력치 설정.
	m_wSUM_PA = m_wPA + nSUM_PA;
	m_wSUM_SA = m_wSA + nSUM_SA;
	m_wSUM_MA = m_wMA + nSUM_MA;

	//	방어력. 변화율 반영.
	m_nDEFENSE_SKILL = int ( m_nDEFENSE_SKILL * m_fDefenseRate );
	if ( m_nDEFENSE_SKILL < 0 )			m_nDEFENSE_SKILL = 1;

	m_sSUMRESIST_SKILL.LIMIT();

	//	공격 속도. ( 지속 skill 속성 가산된 부분에 state, passive skill 속성 가산. )
	m_fATTVELO += m_fSTATE_MOVE + m_sSUM_PASSIVE.m_fATTVELO;

	m_fSTATE_DELAY += m_fSKILLDELAY + m_sSUM_PASSIVE.m_fSKILLDELAY;

	//	공격 가능거리.
	m_wSUM_ATTRANGE = m_wATTRANGE;

	//	MP 증가율이 없을때.
	if ( (m_fINCR_MP==0.0f) && (m_fIncMP>0) )		m_fIncMP = 0;





	//	스킬 효과로 변경된 데미지 적용.
	m_gdDAMAGE_PHYSIC = m_gdDAMAGE_SKILL;
	m_gdDAMAGE_PHYSIC.wLow += m_sSUMITEM.gdDamage.wLow;
	m_gdDAMAGE_PHYSIC.wMax += m_sSUMITEM.gdDamage.wMax;

	if ( ISLONGRANGE_ARMS() )	m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_SA );	//	장거리 공격.
	else						m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_PA );	//	근접 공격.

	//	공격력. 변화율 반영.
	m_gdDAMAGE_PHYSIC.wLow = WORD ( m_gdDAMAGE_PHYSIC.wLow * m_fDamageRate );
	m_gdDAMAGE_PHYSIC.wHigh = WORD ( m_gdDAMAGE_PHYSIC.wHigh * m_fDamageRate );

	m_fINCR_HP_TOTAL = fINCR_HP;
	m_fINCR_MP_TOTAL = fINCR_MP;
	m_fINCR_SP_TOTAL = fINCR_SP;

	//	Note : 체력 변화.
	//
	float fElap = (fElapsedTime/GLCONST_CHAR::fUNIT_TIME);
	float fINC_HP = fElap* ( m_sHP.wMax*fINCR_HP + GLCONST_CHAR::fHP_INC + m_sSUMITEM.fInc_HP );
	float fINC_MP = fElap* ( m_sMP.wMax*fINCR_MP + GLCONST_CHAR::fMP_INC + m_sSUMITEM.fInc_MP );
	float fINC_SP = fElap* ( m_sSP.wMax*fINCR_SP + GLCONST_CHAR::fSP_INC + m_sSUMITEM.fInc_SP );

	GLOGICEX::UPDATE_POINT ( m_sHP, m_fIncHP, fINC_HP, 1 );
	GLOGICEX::UPDATE_POINT ( m_sMP, m_fIncMP, fINC_MP, 0 );
	GLOGICEX::UPDATE_POINT ( m_sSP, m_fIncSP, fINC_SP, 0 );

	
	
}

float GLCHARLOGIC_SERVER::GETATTVELO ()
{
	//	return m_fATTVELO<0.0f?0.0f:m_fATTVELO;
	/*additional RV, Juver, 2018/06/15 */
	float fATTVELO = m_fATTVELO + m_sSUMITEM.fIncR_AtkSpeed + m_sSUMITEM.fadd_attack_speed;
	return fATTVELO<0.0f?0.0f:fATTVELO;
}

float GLCHARLOGIC_SERVER::GETMOVEVELO ()
{
	/*additional RV, Juver, 2018/06/15 */
	float fMOVE = m_fSTATE_MOVE + m_fSKILL_MOVE + m_fOPTION_MOVE + m_sSUMITEM.fIncR_MoveSpeed + m_sSUMITEM.fadd_movement_speed;
	return fMOVE<0.0f?0.0f:fMOVE;
}

float GLCHARLOGIC_SERVER::GETATT_ITEM ()
{
	float fATTVELO = m_sSUMITEM.fInc_AtkSpeed / 100;
	return fATTVELO;
}

float GLCHARLOGIC_SERVER::GETMOVE_ITEM ()
{
	float fMOVE = ( m_sSUMITEM.fInc_MoveSpeed / GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fRUNVELO);
	return fMOVE<0.0f?0.0f:fMOVE;
}

float GLCHARLOGIC_SERVER::GETEXP_TOTAL()
{
	float fEXP = m_fEXP_RATE+m_sSUMITEM.fExpRate;
	return fEXP<0.0f?0.0f:fEXP;
}

float GLCHARLOGIC_SERVER::GET_ATT_VELO() const
{
	/*additional RV, Juver, 2018/06/15 */
	float fATTVELO1 = m_fATTVELO + m_sSUMITEM.fIncR_AtkSpeed + m_sSUMITEM.fadd_attack_speed;
	float fATTVELO2 = m_sSUMITEM.fInc_AtkSpeed / 100.0f;
	float fATTVELO = fATTVELO1 + fATTVELO2;
	return fATTVELO<0.0f?0.0f:fATTVELO;
}

float GLCHARLOGIC_SERVER::GET_MOVE_VELO() const
{
	/*additional RV, Juver, 2018/06/15 */
	float fMOVEVELO1 = m_fSTATE_MOVE + m_fSKILL_MOVE + m_fOPTION_MOVE + m_sSUMITEM.fIncR_MoveSpeed + m_sSUMITEM.fadd_movement_speed;
	float fMOVEVELO2 = m_sSUMITEM.fInc_MoveSpeed / 100.0f;
	float fMOVEVELO = fMOVEVELO1 + fMOVEVELO2;
	return fMOVEVELO<0.0f?0.0f:fMOVEVELO;
}

float GLCHARLOGIC_SERVER::GET_EXP_TOTAL() const
{
	float fEXP = m_fEXP_RATE+m_sSUMITEM.fExpRate;
	return fEXP<0.0f?0.0f:fEXP;
}

float GLCHARLOGIC_SERVER::GET_HP_INCR() const
{
	float fVal = 0.0f;
	fVal += ( m_fINCR_HP_TOTAL * 100.f );
	fVal += ( m_sSUMITEM.fInc_HP / 100.0f );
	return fVal<0.0f?0.0f:fVal;
}

float GLCHARLOGIC_SERVER::GET_MP_INCR() const
{
	float fVal = 0.0f;
	fVal += ( m_fINCR_MP_TOTAL * 100.f );
	fVal += ( m_sSUMITEM.fInc_MP / 100.0f );
	return fVal<0.0f?0.0f:fVal;
}

float GLCHARLOGIC_SERVER::GET_SP_INCR() const
{
	float fVal = 0.0f;
	fVal += ( m_fINCR_SP_TOTAL * 100.f );
	fVal += ( m_sSUMITEM.fInc_SP / 100.0f );
	return fVal<0.0f?0.0f:fVal;
}



//	단순히 장착위치 인지만 검사.
BOOL GLCHARLOGIC_SERVER::CHECKSLOT_ITEM ( SNATIVEID sNativeID, EMSLOT emSlot )
{
	if ( sNativeID==NATIVEID_NULL() )		return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )	return FALSE;

	const SITEM &sItem = *pItem;

	if( sItem.sBasicOp.emItemType!=ITEM_SUIT && 
		sItem.sBasicOp.emItemType!=ITEM_ARROW && 
		sItem.sBasicOp.emItemType!=ITEM_CHARM && 
		sItem.sBasicOp.emItemType!=ITEM_BULLET &&  /*gun-bullet logic, Juver, 2017/05/27 */
		sItem.sBasicOp.emItemType!=ITEM_ANTI_DISAPPEAR &&
		sItem.sBasicOp.emItemType!=ITEM_REVIVE && 
		sItem.sBasicOp.emItemType!=ITEM_VEHICLE )
		return FALSE;

	//	해당 슬롯에 장착가능한 SUIT종류.
	EMSUIT emSuit= SLOT_2_SUIT ( emSlot );
	if ( sItem.sSuitOp.emSuit != emSuit )	return FALSE;

	if ( sItem.sBasicOp.emItemType == ITEM_REVIVE ||
		 sItem.sBasicOp.emItemType == ITEM_ANTI_DISAPPEAR )
	{
		/*ep8 puton slots, Juver, 2018/03/30 */
		if ( emSlot != SLOT_LOGIC_NECKLACE )
			return FALSE;	
	}

	if ( sItem.sBasicOp.emItemType == ITEM_VEHICLE )
	{
		if ( emSlot != SLOT_VEHICLE ) return FALSE;
	}

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();
	
	if ( sItem.sBasicOp.emItemType==ITEM_ARROW || 
		sItem.sBasicOp.emItemType==ITEM_CHARM ||

		/*gun-bullet logic, Juver, 2017/05/27 */
		sItem.sBasicOp.emItemType==ITEM_BULLET )
	{
		if ( emSlot!=emLHand )
			return FALSE;
	}

	//	손에 드는 도구중 한손용일때.
	if ( emSuit==SUIT_HANDHELD )
	{
		//	양손 무기류.
		//
		if ( sItem.sSuitOp.IsBOTHHAND() )
		{
			if ( emSlot!=emRHand )											
				return FALSE;
		}
		//	한손 무기류.
		//
		else
		{
			if ( sItem.sSuitOp.emHand!=HAND_BOTH )	//	양손 모두 가능하면 점검 안함.
			{
				if ( sItem.sSuitOp.emHand==HAND_RIGHT && emSlot!=emRHand )	return FALSE;
				if ( sItem.sSuitOp.emHand==HAND_LEFT && emSlot!=emLHand )	return FALSE;
			}

			if ( sItem.sBasicOp.emItemType==ITEM_ARROW )
			{
				//	화살 착용시에는 오른손에 활을 가지고 있지 않다면 착용 불가능.
				if ( !m_pITEMS[emRHand] || m_pITEMS[emRHand]->sSuitOp.emAttack!=ITEMATT_BOW )	
					return FALSE;
			}
			else if ( sItem.sBasicOp.emItemType==ITEM_CHARM )
			{
				//	부적 착용시에는 오른손에 창을 가지고 있지 않다면 착용 불가능.
				if ( !m_pITEMS[emRHand] || m_pITEMS[emRHand]->sSuitOp.emAttack!=ITEMATT_SPEAR )	
					return FALSE;
			}
			/*gun-bullet logic, Juver, 2017/05/27 */
			else if ( sItem.sBasicOp.emItemType==ITEM_BULLET )
			{
				if ( !m_pITEMS[emRHand] || !( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_GUN ||
											m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_RAILGUN ||
											m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_PORTALGUN ) )	
					return FALSE;
			}
		}
	}

	return TRUE;
}

//	넣을려는 슬롯이 비였는지 검사한다.
//	단, 활이 양손 무기이지만 왼손에 화살을 장착할수 있다.
BOOL GLCHARLOGIC_SERVER::ISEMPTY_SLOT ( SNATIVEID sNativeID, EMSLOT emSlot )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )	return FALSE;

	EMSUIT emSuit= SLOT_2_SUIT ( emSlot );

	BOOL bEmptySlot = TRUE;
	if ( pItem->sSuitOp.emSuit==SUIT_HANDHELD )
	{
		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		//	넣을려고 하는 아이탬이 양손도구일 때.
		if ( pItem->sSuitOp.IsBOTHHAND() )
		{
			if ( m_pITEMS[emRHand] || m_pITEMS[emLHand] )
			{
				bEmptySlot = FALSE;
			}

			//	만약 넣을려는게 활이라면. 왼손에 화살이 있어도 됨. 단 오른손은 비어 있어야.
			if ( pItem->sSuitOp.emAttack==ITEMATT_BOW )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_ARROW )
					{
						bEmptySlot = TRUE;
					}
				}
			}

			//	넣으려는 것이 창 종류일때 왼손에 부적이 있어도 됨.
			if ( pItem->sSuitOp.emAttack==ITEMATT_SPEAR )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_CHARM )
					{
						bEmptySlot = TRUE;
					}
				}
			}

			/*gun-bullet logic, Juver, 2017/05/27 */
			if ( pItem->sSuitOp.emAttack==ITEMATT_GUN ||
				pItem->sSuitOp.emAttack==ITEMATT_RAILGUN ||
				 pItem->sSuitOp.emAttack==ITEMATT_PORTALGUN )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_BULLET )
					{
						bEmptySlot = TRUE;
					}
				}
			}
		}
		//	넣을려는 아이탬이 한손도구일 때.
		else
		{
			//	기존 무기가 양손일때.
			if ( m_pITEMS[emRHand] && m_pITEMS[emRHand]->sSuitOp.IsBOTHHAND() )
			{
				bEmptySlot = FALSE;
			}
			//	기존 무기가 한손일때.
			else
			{
				if ( m_pITEMS[emSlot] )		bEmptySlot = FALSE;
			}

			//	넣을려는 아이탬이 화살일때.
			if ( pItem->sBasicOp.emItemType==ITEM_ARROW )
			{
				bEmptySlot = FALSE;
				if ( m_pITEMS[emRHand] && !m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_BOW )			bEmptySlot = TRUE;
				}
			}

			//	넣을려는 아이탬이 부적일때.
			if ( pItem->sBasicOp.emItemType==ITEM_CHARM )
			{
				bEmptySlot = FALSE;
				if ( m_pITEMS[emRHand] && !m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_SPEAR )		bEmptySlot = TRUE;
				}
			}

			/*gun-bullet logic, Juver, 2017/05/27 */
			if ( pItem->sBasicOp.emItemType==ITEM_BULLET )
			{
				bEmptySlot = FALSE;
				if ( m_pITEMS[emRHand] && !m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_GUN ||
						m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_RAILGUN ||
						m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_PORTALGUN )		
						bEmptySlot = TRUE;
				}
			}
		}
	}
	else if ( m_pITEMS[emSlot] )			bEmptySlot = FALSE;

	return bEmptySlot;
}

BOOL GLCHARLOGIC_SERVER::ACCEPT_ITEM ( SNATIVEID sNativeID )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;

	if ( !(sItem.sBasicOp.dwReqCharClass&m_emClass) )			return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( sItem.sBasicOp.emReqBright!=BRIGHT_BOTH )
	{
		if ( GETBRIGHT()!=sItem.sBasicOp.emReqBright )			return FALSE;
	}

	if ( m_wSUM_PA < sItem.sBasicOp.wReqPA )					return FALSE;
	if ( m_wSUM_SA < sItem.sBasicOp.wReqSA )					return FALSE;

	/*UserNum ItemReq, Juver, 2017/06/27 */
	if ( sItem.sBasicOp.dwReqUserNum != 0 )
	{
		if ( GetUserID() != sItem.sBasicOp.dwReqUserNum )		return FALSE;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( sItem.sBasicOp.dwReqContributionPoint != 0 )
	{
		if ( m_dwContributionPoint < sItem.sBasicOp.dwReqContributionPoint )	return FALSE;
	}

	/*activity point, Juver, 2017/08/23 */
	if( sItem.sBasicOp.dwReqActivityPoint != 0 )
	{
		if( m_dwActivityPoint < sItem.sBasicOp.dwReqActivityPoint )				return FALSE;
	}

	/* play time system, Juver, 2021/01/27 */
	if( sItem.sBasicOp.llPlayTimeReq != 0 )
	{
		if( m_llPlayTime < sItem.sBasicOp.llPlayTimeReq )						return FALSE;
	}

	if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )			return FALSE;
	if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )	return FALSE;
	if ( !m_sSUMSTATS.CHECK_REQ ( sItem.sBasicOp.sReqStats, wAPT ) )		return FALSE;
	
	if ( pItem->sSkillBookOp.sSkill_ID!=NATIVEID_NULL() &&
		pItem->sBasicOp.emItemType == ITEM_SKILL )
	{
		if ( CHECKLEARNABLE_SKILL ( pItem->sSkillBookOp.sSkill_ID )!=EMSKILL_LEARN_OK )		return FALSE;
	}

	/*item user requirement, Juver, 2018/05/24 */
	if ( !GLItemUser::GetInstance().CharTest( sNativeID.dwID, GetUserID(), m_dwCharID, m_dwGuild ) )	return FALSE;

	if ( RPARAM::bVIPSystem && sItem.sBasicOp.emVIPLevel != EMVIP_LEVEL_NONE )
	{
		if ( static_cast<int>(m_emVIPLevel) < static_cast<int>(sItem.sBasicOp.emVIPLevel) )				
			return FALSE;
	}

	if ( RPARAM::bRBSystem && sItem.sBasicOp.dwRebornReq != 0 )
	{
		if ( m_dwReborn < sItem.sBasicOp.dwRebornReq )				
			return FALSE;
	}

	return TRUE;
}


BOOL GLCHARLOGIC_SERVER::SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID, bool bLevelCheck )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;    

	if ( !(sItem.sBasicOp.dwReqCharClass&m_emClass) )			return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( bLevelCheck )
	{
		if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )			
			return FALSE;

		if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )	
			return FALSE;	
	}
	
	/*item user requirement, Juver, 2018/05/24 */
	if ( !GLItemUser::GetInstance().CharTest( sNativeID.dwID, GetUserID(), m_dwCharID, m_dwGuild ) )	return FALSE;

	/* play time system, Juver, 2021/01/27 */
	if( sItem.sBasicOp.llPlayTimeReq != 0 )
	{
		if( m_llPlayTime < sItem.sBasicOp.llPlayTimeReq )		return FALSE;
	}

	if ( RPARAM::bVIPSystem && sItem.sBasicOp.emVIPLevel != EMVIP_LEVEL_NONE )
	{
		if ( static_cast<int>(m_emVIPLevel) < static_cast<int>(sItem.sBasicOp.emVIPLevel) )				
			return FALSE;
	}

	if ( RPARAM::bRBSystem && sItem.sBasicOp.dwRebornReq != 0 )
	{
		if ( m_dwReborn < sItem.sBasicOp.dwRebornReq )				
			return FALSE;
	}

	return TRUE;
}

BOOL GLCHARLOGIC_SERVER::SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID, bool bLevelCheck )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;
	if ( sDisguiseID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sDisguiseID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	const SITEM &sItemDisguise = *pItemDisguise;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;

	//	Memo : 코스튬과 방어구의 자격요건을 체크한다.
	//		두 방어구의 교집합이 착용가능 조건이다.
	if( !(sItem.sBasicOp.dwReqCharClass&m_emClass) ||
		!(sItemDisguise.sBasicOp.dwReqCharClass&m_emClass) )	return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( bLevelCheck )
	{
		if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )				
			return FALSE;

		if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )					
			return FALSE;
	}

	

	/*item user requirement, Juver, 2018/05/24 */
	if ( !GLItemUser::GetInstance().CharTest( sNativeID.dwID, GetUserID(), m_dwCharID, m_dwGuild ) )	return FALSE;
	if ( !GLItemUser::GetInstance().CharTest( sDisguiseID.dwID, GetUserID(), m_dwCharID, m_dwGuild ) )	return FALSE;

	/* play time system, Juver, 2021/01/27 */
	if( sItem.sBasicOp.llPlayTimeReq != 0 )
	{
		if( m_llPlayTime < sItem.sBasicOp.llPlayTimeReq )		return FALSE;
	}

	if ( RPARAM::bVIPSystem && sItem.sBasicOp.emVIPLevel != EMVIP_LEVEL_NONE )
	{
		if ( static_cast<int>(m_emVIPLevel) < static_cast<int>(sItem.sBasicOp.emVIPLevel) )				
			return FALSE;
	}

	if ( RPARAM::bRBSystem && sItem.sBasicOp.dwRebornReq != 0 )
	{
		if ( m_dwReborn < sItem.sBasicOp.dwRebornReq )				
			return FALSE;
	}

	return TRUE;
}

BOOL GLCHARLOGIC_SERVER::ACCEPT_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;
	if ( sDisguiseID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sDisguiseID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	const SITEM &sItemDisguise = *pItemDisguise;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;

	//	Memo : 코스튬과 방어구의 자격요건을 체크한다.
	//		두 방어구의 교집합이 착용가능 조건이다.
	if( !(sItem.sBasicOp.dwReqCharClass&m_emClass) ||
		!(sItemDisguise.sBasicOp.dwReqCharClass&m_emClass) )	return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( sItem.sBasicOp.emReqBright!=BRIGHT_BOTH )
	{
		if ( GETBRIGHT()!=sItem.sBasicOp.emReqBright )			return FALSE;
	}

	if ( m_wSUM_PA < sItem.sBasicOp.wReqPA )					return FALSE;
	if ( m_wSUM_SA < sItem.sBasicOp.wReqSA )					return FALSE;

	/*UserNum ItemReq, Juver, 2017/06/27 */
	if ( sItem.sBasicOp.dwReqUserNum != 0 )
	{
		if ( GetUserID() != sItem.sBasicOp.dwReqUserNum )		return FALSE;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( sItem.sBasicOp.dwReqContributionPoint != 0 )
	{
		if ( m_dwContributionPoint < sItem.sBasicOp.dwReqContributionPoint )	return FALSE;
	}

	/*activity point, Juver, 2017/08/23 */
	if( sItem.sBasicOp.dwReqActivityPoint != 0 )
	{
		if( m_dwActivityPoint < sItem.sBasicOp.dwReqActivityPoint )				return FALSE;
	}

	/* play time system, Juver, 2021/01/27 */
	if( sItem.sBasicOp.llPlayTimeReq != 0 )
	{
		if( m_llPlayTime < sItem.sBasicOp.llPlayTimeReq )						return FALSE;
	}

	if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )				return FALSE;
	if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )					return FALSE;
	if ( !m_sSUMSTATS.CHECK_REQ ( sItem.sBasicOp.sReqStats, wAPT ) )		
		return FALSE;

	if ( pItem->sSkillBookOp.sSkill_ID!=NATIVEID_NULL() &&
		pItem->sBasicOp.emItemType == ITEM_SKILL )
	{
		if ( CHECKLEARNABLE_SKILL ( pItem->sSkillBookOp.sSkill_ID )!=EMSKILL_LEARN_OK )		
			return FALSE;
	}

	/*item user requirement, Juver, 2018/05/24 */
	if ( !GLItemUser::GetInstance().CharTest( sNativeID.dwID, GetUserID(), m_dwCharID, m_dwGuild ) )	return FALSE;
	if ( !GLItemUser::GetInstance().CharTest( sDisguiseID.dwID, GetUserID(), m_dwCharID, m_dwGuild ) )	return FALSE;


	if ( RPARAM::bVIPSystem && sItem.sBasicOp.emVIPLevel != EMVIP_LEVEL_NONE )
	{
		if ( static_cast<int>(m_emVIPLevel) < static_cast<int>(sItem.sBasicOp.emVIPLevel) )				
			return FALSE;
	}

	if ( RPARAM::bRBSystem && sItem.sBasicOp.dwRebornReq != 0 )
	{
		if ( m_dwReborn < sItem.sBasicOp.dwRebornReq )				
			return FALSE;
	}

	return TRUE;
}

WORD GLCHARLOGIC_SERVER::CALC_ACCEPTP ( SNATIVEID sNativeID )
{
	WORD wATP = 0;
	if ( sNativeID==NATIVEID_NULL() )							return wATP;
	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pITEM==NULL )											return wATP;

	if ( m_sSUMSTATS.wPow < pITEM->sBasicOp.sReqStats.wPow )		wATP += pITEM->sBasicOp.sReqStats.wPow - m_sSUMSTATS.wPow;
	if ( m_sSUMSTATS.wStr < pITEM->sBasicOp.sReqStats.wStr )		wATP += pITEM->sBasicOp.sReqStats.wStr - m_sSUMSTATS.wStr;
	if ( m_sSUMSTATS.wSpi < pITEM->sBasicOp.sReqStats.wSpi )		wATP += pITEM->sBasicOp.sReqStats.wSpi - m_sSUMSTATS.wSpi;
	if ( m_sSUMSTATS.wDex < pITEM->sBasicOp.sReqStats.wDex )		wATP += pITEM->sBasicOp.sReqStats.wDex - m_sSUMSTATS.wDex;
	if ( m_sSUMSTATS.wInt < pITEM->sBasicOp.sReqStats.wInt )		wATP += pITEM->sBasicOp.sReqStats.wInt - m_sSUMSTATS.wInt;
	if ( m_sSUMSTATS.wSta < pITEM->sBasicOp.sReqStats.wSta )		wATP += pITEM->sBasicOp.sReqStats.wSta - m_sSUMSTATS.wSta;

	if ( m_wLevel < pITEM->sBasicOp.wReqLevelDW )					wATP += pITEM->sBasicOp.wReqLevelDW - m_wLevel;

	return wATP;
}

EMBEGINATTACK_FB GLCHARLOGIC_SERVER::BEGIN_ATTACK ( WORD wStrikeNum )
{
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
	SITEM* pLHAND = GET_SLOT_ITEMDATA ( emLHand );

	if ( pRHAND && pRHAND->sSuitOp.emAttack==ITEMATT_BOW )
	{
		if ( !pLHAND || pLHAND->sBasicOp.emItemType != ITEM_ARROW )			return EMBEGINA_ARROW;

		//	사용가능수 감소 시킴.
		WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
		if ( wTurnNum >= wStrikeNum )	wTurnNum -= wStrikeNum;
		else							wTurnNum = 0;
	}

	/*gun-bullet logic, Juver, 2017/05/27 */
	if ( pRHAND && ( pRHAND->sSuitOp.emAttack==ITEMATT_GUN || 
		pRHAND->sSuitOp.emAttack==ITEMATT_RAILGUN || 
		pRHAND->sSuitOp.emAttack==ITEMATT_PORTALGUN ) )
	{
		if ( !pLHAND || pLHAND->sBasicOp.emItemType != ITEM_BULLET )			return EMBEGINA_BULLET;

		WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
		if ( wTurnNum >= wStrikeNum )	wTurnNum -= wStrikeNum;
		else							wTurnNum = 0;
	}

	WORD wDisSP = GLCONST_CHAR::wBASIC_DIS_SP;
	if ( pRHAND )	wDisSP += pRHAND->sSuitOp.wReqSP;
	if ( pLHAND )	wDisSP += pLHAND->sSuitOp.wReqSP;

	//	SP 여력 채크.
	if ( m_sSP.wNow < (wDisSP*wStrikeNum) )	return EMBEGINA_SP;

	return EMBEGINA_OK;
}

BOOL GLCHARLOGIC_SERVER::VALID_LEVELUP () const
{
	// 극강부는 최대 레벨 체크를 다르게 한다.
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_EXTREME_LEVEL )	return FALSE;
	}else{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_LEVEL )	return FALSE;
	}
	return m_sExperience.lnNow >= GET_LEVELUP_EXP();
}

LONGLONG GLCHARLOGIC_SERVER::GET_LEVELUP_EXP () const
{
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		return GLOGICEX::GLNEEDEXP2(GETLEVEL());
	}
	else
	{
		return GLOGICEX::GLNEEDEXP(GETLEVEL());
	}
}

void GLCHARLOGIC_SERVER::LEVLEUP ( bool bInitNowExp )
{
	bool bMaxLevel = FALSE;
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		if( m_wLevel < GLCONST_CHAR::wMAX_EXTREME_LEVEL ) bMaxLevel = TRUE;
	}else{
		if( m_wLevel < GLCONST_CHAR::wMAX_LEVEL ) bMaxLevel = TRUE;
	}


	if ( /*m_wLevel < GLCONST_CHAR::wMAX_LEVEL*/bMaxLevel )
	{
		//	현제 경험치 감산.
		//GASSERT(m_sExperience.lnNow>=GET_LEVELUP_EXP());
		m_sExperience.lnNow -= GET_LEVELUP_EXP();

		//	랩업됨.
		++m_wLevel;

		//	랩업시 보너스 포인트
		if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
		{
			m_wStatsPoint += GLCONST_CHAR::wLVL_2ndSTATS_P;
			m_dwSkillPoint += GLCONST_CHAR::wLVL_2ndSKILL_P;
		}
		else
		{
			m_wStatsPoint += GLCONST_CHAR::wLVL_STATS_P;
			m_dwSkillPoint += GLCONST_CHAR::wLVL_SKILL_P;
		}

		//	모든 수치를 다시 초기화 한다.
		INIT_DATA(FALSE,FALSE,1.0f,bInitNowExp);

		//	채력을 모두 회복 시켜준다.
		m_sHP.TO_FULL ();
		m_sMP.TO_FULL ();
		m_sSP.TO_FULL ();	
	}
}

void GLCHARLOGIC_SERVER::STATSUP ( EMSTATS emStats )
{
	switch ( emStats )
	{
	case EMPOW:	++m_sStats.wPow; break;
	case EMSTR: ++m_sStats.wStr; break;

	case EMSPI: ++m_sStats.wSpi; break;
	case EMDEX: ++m_sStats.wDex; break;

	case EMINT: ++m_sStats.wInt; break;
	case EMSTA: ++m_sStats.wSta; break;
	};

	//	잉여 STATE를 감소.
	--m_wStatsPoint;

	//	모든 수치를 다시 초기화 한다.
	INIT_DATA(FALSE,FALSE);

}

/*modern character window, Juver, 2018/03/17 */
void GLCHARLOGIC_SERVER::STATSUP_MODERN ( SCHARSTATS	sStats )
{
	m_sStats.wPow += sStats.wPow;
	m_sStats.wStr += sStats.wStr;
	m_sStats.wSpi += sStats.wSpi;
	m_sStats.wDex += sStats.wDex;
	m_sStats.wInt += sStats.wInt;
	m_sStats.wSta += sStats.wSta;

	m_wStatsPoint -= sStats.GetTotal();

	INIT_DATA(FALSE,FALSE);
}

// *****************************************************
// Desc: 스킬정보 리셋
// *****************************************************
bool GLCHARLOGIC_SERVER::RESET_SKILL ()
{
	EMCHARINDEX emINDEX = CharClassToIndex(m_emClass);
	const SCHARDATA2& sCHARDATA = GLCONST_CHAR::GET_CHAR_DATA2(m_wSchool,emINDEX);

	//	Note : 퀘스트에서 획득한 스킬 포인트.
	//
	DWORD dwGiftSkill(0);

	GLQuestPlay::MAPQUEST& sQuestEnd = m_cQuestPlay.GetQuestEnd();
	GLQuestPlay::MAPQUEST_ITER pos = sQuestEnd.begin();
	GLQuestPlay::MAPQUEST_ITER end = sQuestEnd.end();
	for ( ; pos!=end; ++pos )
	{
		GLQUESTPROG *pPROG = (*pos).second;
		GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pPROG->m_sNID.dwID );
		if ( !pQUEST )	continue;

		dwGiftSkill += pQUEST->m_dwGiftSKILLPOINT;
	}

	//	Note : 스킬 포인트 초기화.
	//
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		m_dwSkillPoint = sCHARDATA.m_dwSkillPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_2ndSKILL_P + dwGiftSkill;
	}
	else
	{
		m_dwSkillPoint = sCHARDATA.m_dwSkillPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_SKILL_P + dwGiftSkill;
	}

	//	Note : 스킬과 연결된 사전 정보 리셋.
	//
	m_wSKILLQUICK_ACT = 0;
	for ( int i=0; i<EMSKILLQUICK_SIZE; ++i )
	{
		m_sSKILLQUICK[i] = SNATIVEID(false);
	}


	//	Note : 습득한 스킬 초기화.
	//

	m_ExpSkills.clear();
//	m_ExpSkills = sCHARDATA.m_ExpSkills;

	//	Note : 모든 수치를 다시 초기화 한다.
	//
	INIT_DATA(FALSE,FALSE);

	return true;
}

// *****************************************************
// Desc: 스텟정보 리셋
// *****************************************************
bool GLCHARLOGIC_SERVER::RESET_STATS ( const WORD wDIS )
{
	EMCHARINDEX emINDEX = CharClassToIndex(m_emClass);
	const SCHARDATA2& sCHARDATA = GLCONST_CHAR::GET_CHAR_DATA2(m_wSchool,emINDEX);

	// 전체 스텟 포인트 초기화
	if ( wDIS==USHRT_MAX )
	{
		//	Note : 퀘스트에서 획득한 스텟 포인트.
		//
		WORD wGiftStats(0);

		GLQuestPlay::MAPQUEST& sQuestEnd = m_cQuestPlay.GetQuestEnd();
		GLQuestPlay::MAPQUEST_ITER pos = sQuestEnd.begin();
		GLQuestPlay::MAPQUEST_ITER end = sQuestEnd.end();
		for ( ; pos!=end; ++pos )
		{
			GLQUESTPROG *pPROG = (*pos).second;
			GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pPROG->m_sNID.dwID );
			if ( !pQUEST )	continue;

			wGiftStats += (WORD) pQUEST->m_dwGiftSTATSPOINT;
		}

		if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
		{
			m_wStatsPoint = sCHARDATA.m_wStatsPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_2ndSTATS_P + wGiftStats;
		}
		else
		{
			m_wStatsPoint = sCHARDATA.m_wStatsPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_STATS_P + wGiftStats;
		}

		m_sStats.RESET();

		return true;
	}

	// 망각약 등급에 따른 초기화
	WORD wNOW_DIS(0);
	while(1)
	{
		if ( wNOW_DIS>=wDIS )		break;
		if ( m_sStats.IsZERO() )	break;

		// 각 스텟별로 하나씩 차감
		for ( int i=0; i<EMSIZE; ++i )
		{
			if ( wNOW_DIS>=wDIS )		break;
			if ( m_sStats.IsZERO() )	break;

			WORD &wSTAT = m_sStats.GET((EMSTATS)i);
			if ( wSTAT == 0 )			continue;

			--wSTAT;
			++wNOW_DIS;
		}
	}

	//	Note : 현재 잔여 스텟 포인트에 가산.
	m_wStatsPoint += wNOW_DIS;

	//	Note : 모든 수치를 다시 초기화 한다.
	//
	INIT_DATA(FALSE,FALSE);

	return true;
}

bool GLCHARLOGIC_SERVER::RESET_STATS_SKILL ( const WORD wDIS_STAT )
{
	//	Note : 리셋 호출 순서가 stats, skill순이여야 한다.
	RESET_STATS ( wDIS_STAT );
	RESET_SKILL();

	return true;
}

void GLCHARLOGIC_SERVER::UPDATESKILLDELAY ( float fElapsedTime )
{
	DELAY_MAP_ITER iter_del;

	DELAY_MAP_ITER iter = m_SKILLDELAY.begin ();
	DELAY_MAP_ITER iter_end = m_SKILLDELAY.end ();

	for ( ; iter!=iter_end; )
	{
		float &fDelay = (*iter).second;
		iter_del = iter++;

		fDelay -= fElapsedTime;
		if ( fDelay <= 0.0f )	m_SKILLDELAY.erase ( iter_del );
	}
}

void GLCHARLOGIC_SERVER::LEARN_SKILL ( SNATIVEID skill_id, SNATIVEID item_id, __time64_t _tBornTime )
{
	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return;

	//	Note : 경험치 감소시킴.
	//
	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[0];
	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )			return;

	m_dwSkillPoint -= sSKILL_LEARN_LVL.dwSKP;

	//	Note : 배운 스킬로 등록함.
	//
	m_ExpSkills.insert ( std::make_pair(skill_id.dwID,SCHARSKILL(skill_id,0,item_id,_tBornTime)) );

	//	Note : 페시브 스킬이 변화 할때 초기 수치들을 모두 재 계산한다.
	//
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
}

bool GLCHARLOGIC_SERVER::LEARN_SKILL_QUEST ( SNATIVEID skill_id )
{
	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )	return false;

	//	Note : 배운 스킬로 등록함.
	//
	
	m_ExpSkills.insert ( std::make_pair(skill_id.dwID,SCHARSKILL(skill_id,0,NATIVEID_NULL(),0)) );
	//	Note : 페시브 스킬이 변화 할때 초기 수치들을 모두 재 계산한다.
	//
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
	return true;
}

VOID GLCHARLOGIC_SERVER::LVLUP_SKILL ( SNATIVEID skill_id, WORD wToLevel )
{
	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return;

	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )						return;

	SCHARSKILL &sSkill = (*learniter).second;

	//	Note : 경험치 감소시킴.
	//
	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[wToLevel];
	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )			return;

	m_dwSkillPoint -= sSKILL_LEARN_LVL.dwSKP;

	//	Note : 스킬 레벨 업.
	//
	++sSkill.wLevel;

	//	Note : 페시브 스킬이 변화 할때 초기 수치들을 모두 재 계산한다.
	//
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
}

BOOL GLCHARLOGIC_SERVER::ISLEARNED_SKILL ( SNATIVEID skill_id, WORD wLEVEL )
{
	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return FALSE;

	//	Note : 익힌 스킬인지 검사.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )			return FALSE;

	SCHARSKILL &sSkill = (*learniter).second;
	if ( sSkill.wLevel <  wLEVEL )				return FALSE;

	return TRUE;
}

BOOL GLCHARLOGIC_SERVER::ISMASTER_SKILL ( SNATIVEID skill_id )
{
	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )												return FALSE;

	//	Note : 익힌 스킬인지 검사.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )							return FALSE;

	SCHARSKILL &sSkill = (*learniter).second;

	if ( DWORD(sSkill.wLevel+1)>=SKILL::MAX_SKILL_LEVEL )				return TRUE;
	if ( DWORD(sSkill.wLevel+1)>=pSkill->m_sBASIC.dwMAXLEVEL )	return TRUE;

	return FALSE;
}

SCHARSKILL* GLCHARLOGIC_SERVER::GETLEARNED_SKILL ( SNATIVEID skill_id )
{
	//	Note : 익힌 스킬인지 검사.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )			return NULL;

	return &((*learniter).second);
}

EMSKILL_LEARNCHECK GLCHARLOGIC_SERVER::CHECKLEARNABLE_SKILL ( SNATIVEID skill_id )
{
	WORD wSKILL_LEVEL = 0;

	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )								return EMSKILL_LEARN_UNKNOWN;

	//	Note : 익힌 스킬인지 검사.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter!=m_ExpSkills.end() )
	{
		SCHARSKILL &sSkill = (*learniter).second;
		wSKILL_LEVEL = sSkill.wLevel + 1;			//	현제 스킬 LEVEL

		if ( wSKILL_LEVEL == SKILL::MAX_SKILL_LEVEL )				return EMSKILL_LEARN_MAX;
		if ( wSKILL_LEVEL == pSkill->m_sBASIC.dwMAXLEVEL )	return EMSKILL_LEARN_MAX;
	}
	
	SKILL::SLEARN &sSKILL_LEARN = pSkill->m_sLEARN;
	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[wSKILL_LEVEL];

	//	Note : 스킬 습득조건 검사.
	//
	if ( !(m_emClass&sSKILL_LEARN.dwCLASS) )					return EMSKILL_LEARN_NOTCLASS;
	
	if ( sSKILL_LEARN.emBRIGHT!=BRIGHT_BOTH )
	{
		if ( GETBRIGHT()!=sSKILL_LEARN.emBRIGHT )				return EMSKILL_LEARN_NOTBRIGHT;
	}

	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )				return EMSKILL_LEARN_NOTTERM;
	if ( WORD(GETLEVEL()) < sSKILL_LEARN_LVL.dwLEVEL )			return EMSKILL_LEARN_NOTTERM;
	if ( !m_sSUMSTATS.CHECK_REQ ( sSKILL_LEARN_LVL.sSTATS ) )	return EMSKILL_LEARN_NOTTERM;

	//	요구스킬 조건 검사.
	if ( NATIVEID_NULL()!=sSKILL_LEARN.sSKILL )
	{
		if ( !ISLEARNED_SKILL(sSKILL_LEARN.sSKILL,WORD(sSKILL_LEARN_LVL.dwSKILL_LVL)) )	return EMSKILL_LEARN_NOTTERM;
	}

	if ( RPARAM::bVIPSystem && sSKILL_LEARN.emVIPLevel != EMVIP_LEVEL_NONE )
	{
		if ( static_cast<int>(m_emVIPLevel) < static_cast<int>(sSKILL_LEARN.emVIPLevel) )				
			return EMSKILL_LEARN_NOTTERM;
	}

	if ( RPARAM::bRBSystem && sSKILL_LEARN.dwRebornReq != 0 )
	{
		if ( m_dwReborn < sSKILL_LEARN.dwRebornReq )				
			return EMSKILL_LEARN_NOTTERM;
	}

	return EMSKILL_LEARN_OK;
}

void GLCHARLOGIC_SERVER::SETACTIVESKILL ( SNATIVEID skill_id )
{
	m_idACTIVESKILL = skill_id;
}

/*extreme auto tab, Juver, 2018/09/10 */
EMSKILLCHECK GLCHARLOGIC_SERVER::CHECHSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bNotLearn, EMSLOT emRHand, EMSLOT emLHand )
{
	/*prohibit skill logic, Juver, 2017/06/06 */
	/*pvp capture the flag, Juver, 2018/02/08 */
	if ( m_bProhibitSkill || m_bCaptureTheFlagHoldFlag )	return EMSKILL_PROHIBIT;

	//	Note : 익힌 스킬인지 검사.
	//
	SCHARSKILL sSkill;
	if ( bNotLearn )
	{
		if ( skill_id != m_sDefenseSkill.m_dwSkillID ) return EMSKILL_NOTLEARN;
		sSkill.sNativeID = skill_id;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
		if ( learniter==m_ExpSkills.end() )										return EMSKILL_NOTLEARN; // 배운 스킬이 아닐 경우.
		sSkill = (*learniter).second;

		//	Note : 스킬 딜래이 타임이 지낫는지 점검.
		//
		DELAY_MAP_ITER delayiter = m_SKILLDELAY.find ( skill_id.dwID );
		if ( delayiter!=m_SKILLDELAY.end() )									return EMSKILL_DELAYTIME; // 스킬 딜래이 시간이 지나지 않음.
	}

	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id );
	if ( !pSkill )															return EMSKILL_UNKNOWN;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[sSkill.wLevel];
	

	//	Note : 사용할 수 있는 스킬종류인지 검사.
	if ( pSkill->m_sBASIC.emROLE!=SKILL::EMROLE_NORMAL )					return EMSKILL_UNKNOWN;

	//	Note : 스킬 사용시 착용해야 하는 아이템 조건 검사.
	//
	const bool bHiddenWeapon	= pSkill->m_sLEARN.bHiddenWeapon;
	GLSKILL_ATT emSKILL_LITEM = pSkill->m_sBASIC.emUSE_LITEM;
	GLSKILL_ATT emSKILL_RITEM = pSkill->m_sBASIC.emUSE_RITEM;
	if ( emSKILL_LITEM!=SKILLATT_NOCARE )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTITEM;

		if ( !CHECHSKILL_ITEM(emSKILL_LITEM,pItem->sSuitOp.emAttack,bHiddenWeapon) )		return EMSKILL_NOTITEM;
	}

	if ( emSKILL_RITEM!=SKILLATT_NOCARE )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emRHand);
		if ( !pItem )														return EMSKILL_NOTITEM;
		
		if ( !CHECHSKILL_ITEM(emSKILL_RITEM,pItem->sSuitOp.emAttack,bHiddenWeapon) )		return EMSKILL_NOTITEM;
	}

	//	Note : 화살 소모시 소모량 여유 체크.
	//		--> 소모량이 2 개인대 한개만 남아있는 상황? 에는 남은 숫자만 소진하고 스킬은 발동되게 해줌.
	//
	if ( sSKILL_DATA.wUSE_ARROWNUM!=0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTARROW;

		if ( pItem->sBasicOp.emItemType != ITEM_ARROW )						return EMSKILL_NOTARROW;

		//	갯수는 점검하지 않는다. 모자랄 경우 있는거 한도에서만 소모시킴.
	}

	//	Note : 부적 소모시 소모량 여유 체크.
	//		--> 소모량이 2 개인대 한개만 남아있는 상황? 에는 남은 숫자만 소진하고 스킬은 발동되게 해줌.
	//
	if ( sSKILL_DATA.wUSE_CHARMNUM!=0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTCHARM;

		if ( pItem->sBasicOp.emItemType != ITEM_CHARM )						return EMSKILL_NOTCHARM;

		//	갯수는 점검하지 않는다. 모자랄 경우 있는거 한도에서만 소모시킴.
	}

	/*gun-bullet logic, Juver, 2017/05/27 */
	if ( sSKILL_DATA.wUSE_BULLETNUM!=0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTBULLET;

		if ( pItem->sBasicOp.emItemType != ITEM_BULLET )					return EMSKILL_NOTBULLET;
	}

	//	Note : 기초 체력 소모량 점검.
	if ( m_sHP.wNow <= sSKILL_DATA.wUSE_HP*wStrikeNum )						return EMSKILL_NOTHP;
	if ( m_sMP.wNow < sSKILL_DATA.wUSE_MP*wStrikeNum )						return EMSKILL_NOTMP;

	//if ( m_sExperience.lnNow < sSKILL_DATA.wUSE_EXP*wStrikeNum )			return EMSKILL_NOTEXP;

	//	NEED : 향후 파티스킬이 구성되었을때.
	//
	//sSKILL_DATA.wUSE_HP_PTY;
	//sSKILL_DATA.wUSE_MP_PTY;


	SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
	SITEM* pLHAND = GET_SLOT_ITEMDATA ( emLHand );

	WORD wDisSP = sSKILL_DATA.wUSE_SP;
	if ( pRHAND )	wDisSP += pRHAND->sSuitOp.wReqSP;
	if ( pLHAND )	wDisSP += pLHAND->sSuitOp.wReqSP;

	if ( m_sSP.wNow < wDisSP*wStrikeNum )									return EMSKILL_NOTSP;

	/*combatpoint logic, Juver, 2017/05/28 */
	if ( m_sCombatPoint.wNow < sSKILL_DATA.wUSE_CP*wStrikeNum )				return EMSKILL_NOTCP;

	//	NEED : 향후 파티스킬이 구성되었을때.
	//
	//sSKILL_DATA.wUSE_SP_PTY;

	return EMSKILL_OK;
}

void GLCHARLOGIC_SERVER::ACCOUNTSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bServer )
{
	//	Note : 캐릭터가 배운 스킬 정보 가져옴.
	// 발동스킬은 배운스킬 목록에 없음
	SCHARSKILL sSkill;

	if ( IsDefenseSkill() )
	{
		if ( skill_id != m_sDefenseSkill.m_dwSkillID ) return;
		sSkill.sNativeID = skill_id;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
		if( learniter==m_ExpSkills.end() )										return;	// 배운 스킬이 아닐 경우.
		sSkill = (*learniter).second;
	}

	//	스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )															return;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[sSkill.wLevel];


	//	스킬 딜래이 등록.
	float fDelayTime = GLOGICEX::SKILLDELAY(pSkill->m_sBASIC.dwGRADE,sSkill.wLevel,GETLEVEL(),sSKILL_DATA.fDELAYTIME);
	
	//	Note : 상태이상의 딜래이 감안.
	fDelayTime = fDelayTime * m_fSTATE_DELAY;

	//	Note : 서버에서는 메시지 딜래이 많큼 감소시켜 준다.
	if ( bServer )		APPLY_MSGDELAY ( fDelayTime );

	m_SKILLDELAY.insert ( std::make_pair(skill_id.dwID,fDelayTime) );

	EMSLOT emLHand = GetCurLHand();

	//	화살 소모.
	if ( sSKILL_DATA.wUSE_ARROWNUM!= 0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( pItem )
		{
			//	화살 소모하는 스킬에서는 화살통이 반드시 있어야 하지만 슬롯의 조작으로 없어 질수도 있음.
			//	화살 남은 갯수는 점검하지 않는다.
			WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
			if ( wTurnNum >= sSKILL_DATA.wUSE_ARROWNUM )	wTurnNum -= sSKILL_DATA.wUSE_ARROWNUM;
			else											wTurnNum = 0;
		}
	}

	//	부적 소모.
	if ( sSKILL_DATA.wUSE_CHARMNUM!= 0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( pItem )
		{
			//	부적 소모하는 스킬에서는 화살통이 반드시 있어야 하지만 슬롯의 조작으로 없어 질수도 있음.
			//	부적 남은 갯수는 점검하지 않는다.
			WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
			if ( wTurnNum >= sSKILL_DATA.wUSE_CHARMNUM )	wTurnNum -= sSKILL_DATA.wUSE_CHARMNUM;
			else											wTurnNum = 0;
		}
	}

	/*gun-bullet logic, Juver, 2017/05/27 */
	if ( sSKILL_DATA.wUSE_BULLETNUM!= 0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( pItem )
		{
			WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
			if ( wTurnNum >= sSKILL_DATA.wUSE_BULLETNUM )	wTurnNum -= sSKILL_DATA.wUSE_BULLETNUM;
			else											wTurnNum = 0;
		}
	}

	//	기초 체력 소모.
	m_sHP.DECREASE ( sSKILL_DATA.wUSE_HP*wStrikeNum );
	m_sMP.DECREASE ( sSKILL_DATA.wUSE_MP*wStrikeNum );

	/*combatpoint logic, Juver, 2017/05/28 */
	m_sCombatPoint.DECREASE ( sSKILL_DATA.wUSE_CP*wStrikeNum );

	//m_sExperience.DECREASE ( sSKILL_DATA.wUSE_EXP*wStrikeNum );
}


BOOL GLCHARLOGIC_SERVER::DOGRINDING(	SITEMCUSTOM &sCusItem, 
								const SITEM *pHold, 
								bool &_bRESET, 
								bool &_bTERMINATE, 
								EMANTIDISAPPEAR &emANTIDISAPPEAR )
{
	bool bSUCCEED(false);
	_bRESET = false;
	_bTERMINATE = false;

	const EMGRINDING_TYPE emGRINDING = pHold->sGrindingOp.emTYPE;

	BYTE cGRADE = sCusItem.GETGRADE ( emGRINDING );
	
	if ( emGRINDING == EMGRINDING_DAMAGE || emGRINDING == EMGRINDING_DEFENSE )
	{
		if ( cGRADE >= GLCONST_CHAR::wGRADE_MAX )	return FALSE;
	}
	else
	{
		if ( cGRADE >= GLCONST_CHAR::wGRADE_MAX_REGI )	return FALSE;
	}
	
	
	/*no fail upgrade, Juver, 2017/11/26 */
	if ( pHold->sGrindingOp.bNoFail )
	{
		_bRESET = false;
		_bTERMINATE = false;
		emANTIDISAPPEAR = EMANTIDISAPPEAR_OFF;
	}
	else
	{
		//	4단계 이상부터 실패 확율 존제.
		if ( cGRADE >= GRADE_NORMAL )
		{
			//	성공 확율.
			float fRATE = GLCONST_CHAR::fGRADE_RATE[cGRADE];
			BOOL bOK = RANDOM_GEN ( fRATE );
			if ( !bOK )
			{
				//	리샛 확율.
				float fRESET = GLCONST_CHAR::fGRADE_RESET_RATE[cGRADE];
				bOK = RANDOM_GEN ( fRESET );
				if ( bOK )
				{
					if( emANTIDISAPPEAR != EMANTIDISAPPEAR_ON_NORESET )
					{
						_bRESET = true;
						sCusItem.ResetGrind(emGRINDING);
					}
				
					//	파손 확율.
					bool bTER = GLCONST_CHAR::bENCHANT_TERMINATE_ITEM && ( emGRINDING==EMGRINDING_DAMAGE || emGRINDING==EMGRINDING_DEFENSE );
					if ( bTER )
					{
						float fTERMINATE = GLCONST_CHAR::fGRADE_TERMINATE_RATE[cGRADE];
						bOK = RANDOM_GEN ( fTERMINATE );
						if ( bOK )
						{
							// 소방주를 가지고 있다면
							if( emANTIDISAPPEAR == EMANTIDISAPPEAR_ON || emANTIDISAPPEAR == EMANTIDISAPPEAR_ON_NORESET )
							{
								emANTIDISAPPEAR = EMANTIDISAPPEAR_USE; // 소방주를 사용했다.
							}
							else
							{
								_bTERMINATE = true;		// 아이템이 소멸되었다.
							}
						}
					}
				}

				return FALSE;
			}
		}
	}

	

	//	Note : 연마에 성공.
	//
	switch ( pHold->sGrindingOp.emTYPE )
	{
	case EMGRINDING_DAMAGE:			sCusItem.cDAMAGE++;			break;
	case EMGRINDING_DEFENSE:		sCusItem.cDEFENSE++;		break;
	case EMGRINDING_RESIST_FIRE:	sCusItem.cRESIST_FIRE++;	break;
	case EMGRINDING_RESIST_ICE:		sCusItem.cRESIST_ICE++;		break;
	case EMGRINDING_RESIST_ELEC:	sCusItem.cRESIST_ELEC++;	break;
	case EMGRINDING_RESIST_POISON:	sCusItem.cRESIST_POISON++;	break;
	case EMGRINDING_RESIST_SPIRIT:	sCusItem.cRESIST_SPIRIT++;	break;
	};

	return TRUE;
}

//	Note : 케릭터가 아이템을 소지하고 있는지 검사.
//		착용복장, 인벤토리에서 검색.
//		겹침가능 아이템의 경우 full 로 있을 경우만 소지한 것으로 판단.
//
BOOL GLCHARLOGIC_SERVER::ISHAVEITEM ( SNATIVEID &sNID, DWORD *pNum )
{
	DWORD dwNum = 0;
	for ( int i=0; i<SLOT_TSIZE; ++i )
	{
		const SITEM* pITEM = GET_SLOT_ITEMDATA ( EMSLOT(i) );
		if ( !pITEM )								continue;

		const SITEMCUSTOM &sCUSTOM = GET_SLOT_ITEM ( EMSLOT(i) );
		if ( sCUSTOM.sNativeID==SNATIVEID(false) )	continue;

		if ( pITEM->sBasicOp.sNativeID==sNID )
		{
			if ( pITEM->sDrugOp.wPileNum == 1 )						dwNum++;
			else if ( pITEM->sDrugOp.wPileNum == sCUSTOM.wTurnNum )	dwNum++;
		}
	}

	dwNum += m_cInventory.CountPileItem ( sNID );

	if ( pNum )		*pNum = dwNum;
	return ( dwNum > 0 );
}

BOOL GLCHARLOGIC_SERVER::ISREVIVE ()
{
	/*ep8 puton slots, Juver, 2018/03/30 */
	SITEM* pITEM = GET_SLOT_ITEMDATA(SLOT_LOGIC_NECKLACE);
	if ( !pITEM )											return FALSE;
	if ( pITEM->sDrugOp.emDrug!=ITEM_DRUG_CALL_REVIVE )		return FALSE;

	return TRUE;
}

/*skill range spec logic, Juver, 2017/06/06 */
WORD GLCHARLOGIC_SERVER::GETSKILLRANGE_TAR ( const GLSKILL &sSKILL ) const
{
	int nRANGE = sSKILL.m_sBASIC.wTARRANGE;
	if ( sSKILL.m_sBASIC.emAPPLY==SKILL::EMAPPLY_PHY_LONG )		nRANGE += (int) GETSUM_TARRANGE();

	if ( sSKILL.m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )	
		nRANGE += (int) ( m_fSUM_ATTACK_RANGE );

	if ( nRANGE <= 0 ) nRANGE = 1;

	return (WORD)nRANGE;
}

/*skill range spec logic, Juver, 2017/06/06 */
WORD GLCHARLOGIC_SERVER::GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const
{
	const SKILL::CDATA_LVL &sDATA_LVL = sSKILL.m_sAPPLY.sDATA_LVL[dwLEVEL];

	int nRANGE = sDATA_LVL.wAPPLYRANGE;
	if ( sSKILL.m_sBASIC.emAPPLY==SKILL::EMAPPLY_PHY_LONG )		nRANGE += (int) GETSUM_TARRANGE();

	if ( sSKILL.m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )	
		nRANGE += (int) ( m_fSUM_APPLY_RANGE );

	if ( nRANGE <= 0 ) nRANGE = 1;

	return (WORD)nRANGE;
}

//	Note : 현재 pk 레벨을 알아본다. ( UINT_MAX 일경우 pk 해당사항 없음. )
DWORD GLCHARLOGIC_SERVER::GET_PK_LEVEL ()
{
	if ( m_nBright >= 0 )	return UINT_MAX;

	DWORD dwLEVEL = 0;
	for ( dwLEVEL=0; dwLEVEL<GLCONST_CHAR::EMPK_STATE_LEVEL; ++dwLEVEL )
	{
		if ( GLCONST_CHAR::sPK_STATE[dwLEVEL].nPKPOINT <= m_nBright )		break;
	}

	if ( dwLEVEL>=GLCONST_CHAR::EMPK_STATE_LEVEL )	dwLEVEL = GLCONST_CHAR::EMPK_STATE_LEVEL-1;

	return dwLEVEL;
}


float GLCHARLOGIC_SERVER::GET_PK_DECEXP_RATE ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return 100.0f;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].fPK_EXP_RATE;
}

std::string GLCHARLOGIC_SERVER::GET_PK_NAME ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return GLCONST_CHAR::sPK_STATE[0].strNAME;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].strNAME;
}

DWORD GLCHARLOGIC_SERVER::GET_PK_COLOR ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return GLCONST_CHAR::dwPK_NORMAL_NAME_COLOR;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].dwNAME_COLOR;
}

float GLCHARLOGIC_SERVER::GET_PK_SHOP2BUY ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return 100.0f;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].fSHOP_2BUY_RATE;
}

float GLCHARLOGIC_SERVER::GET_PK_SHOP2SALE ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return 100.0f;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].fSHOP_2SALE_RATE;
}

DWORD GLCHARLOGIC_SERVER::GET_PK_ITEMDROP_NUM ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )
	{
		if ( GLCONST_CHAR::fPK_ITEM_DROP==0 )	return 0;
		return 1;
	}

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].dwITEM_DROP_NUM;
}

float GLCHARLOGIC_SERVER::GET_PK_ITEMDROP_RATE ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	float fRATE = 0;

	if ( dwLEVEL==UINT_MAX )						fRATE = GLCONST_CHAR::fPK_ITEM_DROP;
	else											fRATE = GLCONST_CHAR::sPK_STATE[dwLEVEL].fITEM_DROP_RATE;

	if ( GLSchoolFreePK::GetInstance().IsON() )		fRATE += GLCONST_CHAR::fSCHOOL_FREE_PK_ITEM_DROP;

	return fRATE;
}

bool GLCHARLOGIC_SERVER::ISPLAYKILLING ()
{
	return !m_mapPlayHostile.empty();
}

//	Note : 자신이 범죄자인가?
bool GLCHARLOGIC_SERVER::ISOFFENDER ()
{
	return ( m_nBright < GLCONST_CHAR::sPK_STATE[0].nPKPOINT );
}

bool GLCHARLOGIC_SERVER::IS_HOSTILE_ACTOR ( DWORD dwCHARID )
{
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	if ( pos!=m_mapPlayHostile.end() )
	{
		if( pos->second )
			return ( pos->second->bBAD == TRUE );
		else
			CDebugSet::ToLogFile( "GLCHARLOGIC::IS_HOSTILE_ACTOR, pos->second = NULL" );
	}

	return false;
}

//	Note : 적대자 등록. ( pk 관련 ),	true : 신규 등록시, false : 기존 등록 갱신시.
bool GLCHARLOGIC_SERVER::ADD_PLAYHOSTILE ( DWORD dwCHARID, BOOL bBAD )
{
	if ( m_dwCharID == dwCHARID )
	{
		CDebugSet::ToLogFile( "GLCHARLOGIC::ADD_PLAYHOSTILE, m_dwCharID == dwCHARID" );
		return false;
	}

	bool bResult = true;

	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	// 이미 등록되어 있을때...
	if ( pos!=m_mapPlayHostile.end() )
	{
		bResult = false;

		//	Note : 시간만 갱신. 적대 bBAD는 갱신하지 않음. ( 처음 공격자의 값 유지. )
		if( pos->second )
		{
			pos->second->fTIME = GLCONST_CHAR::fPK_JUSTNESS_TIME;
			return false;
		}
		else
		{
			// 이미 등록되어 있었지만 값이 유효하지 않아서 삭제한다.
			CDebugSet::ToLogFile ("GLCHARLOGIC::ADD_PLAYHOSTILE, pos->second = NULL");
			m_mapPlayHostile.erase (pos);
		}
	}

	// 등록되어 있지 않거나 유효하지 않은 값이었기 때문에
	// 신규로 등록한다.
	SPLAYHOSTILE* pHOSTILE = new SPLAYHOSTILE;
	pHOSTILE->bBAD = bBAD;
	pHOSTILE->fTIME = GLCONST_CHAR::fPK_JUSTNESS_TIME;
	m_mapPlayHostile.insert ( std::make_pair( dwCHARID, pHOSTILE ) );

	return bResult;
}

bool GLCHARLOGIC_SERVER::DEL_PLAYHOSTILE ( DWORD dwCHARID )
{
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	if ( pos!=m_mapPlayHostile.end() )
	{
		SAFE_DELETE( pos->second );
		m_mapPlayHostile.erase ( pos );
		return true;
	}
	else
	{	
		return false;
	}
}

void GLCHARLOGIC_SERVER::DEL_PLAYHOSTILE_ALL ()
{
	for( MAPPLAYHOSTILE_ITER ci = m_mapPlayHostile.begin(); ci != m_mapPlayHostile.end(); ++ci )
		SAFE_DELETE( ci->second );
	m_mapPlayHostile.clear();
}

bool GLCHARLOGIC_SERVER::IS_PLAYHOSTILE ( DWORD dwCHARID )
{
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	return ( pos != m_mapPlayHostile.end() );
}

EMSLOT GLCHARLOGIC_SERVER::GetCurRHand()
{
	if( IsUseArmSub() ) return SLOT_RHAND_S;
	else				return SLOT_RHAND;
}

EMSLOT GLCHARLOGIC_SERVER::GetCurLHand()
{
	if( IsUseArmSub() ) return SLOT_LHAND_S;
	else				return SLOT_LHAND;
}

BOOL GLCHARLOGIC_SERVER::VALID_SLOT_ITEM ( EMSLOT _slot )							
{ 
	if( m_PutOnItems[_slot].sNativeID==NATIVEID_NULL() ) return FALSE;

	if( IsUseArmSub() )
	{
		if( _slot == SLOT_RHAND || _slot == SLOT_LHAND ) return FALSE;
	}
	else
	{
		if( _slot == SLOT_RHAND_S || _slot == SLOT_LHAND_S ) return FALSE;
	}

	return TRUE;
}

BOOL GLCHARLOGIC_SERVER::ISLONGRANGE_ARMS ()							
{
	EMSLOT emRHand = GetCurRHand();
	//return m_pITEMS[emRHand] && ( m_pITEMS[emRHand]->sSuitOp.emAttack>ITEMATT_NEAR ); 

	if ( m_pITEMS[emRHand] )
	{
		return ISLONGRANGE_ARMS ( m_pITEMS[emRHand]->sSuitOp.emAttack );
	}
	else
	{
		return FALSE;
	}
}

BOOL GLCHARLOGIC_SERVER::ISLONGRANGE_ARMS ( GLITEM_ATT emAttack )
{
	switch( emAttack )
	{
	case ITEMATT_NOTHING:
	case ITEMATT_SWORD:
	case ITEMATT_BLADE:
	case ITEMATT_DAGGER:
	case ITEMATT_SPEAR:
	case ITEMATT_STICK:
	case ITEMATT_GAUNT:
	case ITEMATT_SCYTHE:
	case ITEMATT_DUALSPEAR:
	case ITEMATT_SHURIKEN:
		return FALSE;
		break;

	case ITEMATT_BOW:
	case ITEMATT_THROW:
	case ITEMATT_GUN:
	case ITEMATT_RAILGUN:
	case ITEMATT_PORTALGUN:
		return TRUE;
		break;

	default:
		return FALSE;
	}
}

BOOL GLCHARLOGIC_SERVER::IsCurUseArm( EMSLOT emSlot )
{
	if( IsUseArmSub() )
	{
		if( emSlot == SLOT_RHAND || emSlot == SLOT_LHAND ) return FALSE;
	}
	else
	{
		if( emSlot == SLOT_RHAND_S || emSlot == SLOT_LHAND_S ) return FALSE;
	}

	return TRUE;
}


void GLCHARLOGIC_SERVER::DISABLEALLLANDEFF()
{
	for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
	{
		m_sLandEffect[i].Init();
	}
}

void GLCHARLOGIC_SERVER::ADDLANDEFF( SLANDEFFECT landEffect, int iNum )
{
	if( iNum >= EMLANDEFFECT_MULTI ) return;
	m_sLandEffect[iNum] = landEffect;
}

/*continuous damage skill logic, Juver, 2017/06/10 */
BOOL GLCHARLOGIC_SERVER::CONTINUOUSDAMAGE_EXIST( SNATIVEID _sID )
{
	if( m_mapContinuousDamage.empty() )	return FALSE;
	if( _sID == NATIVEID_NULL() )		return FALSE;

	CONTINUOUS_DAMAGE_DATA_MAP_ITER iter = m_mapContinuousDamage.find(_sID.dwID);
	if ( iter != m_mapContinuousDamage.end() )	return TRUE;
	
	return FALSE;
}

/*continuous damage skill logic, Juver, 2017/06/10 */
void GLCHARLOGIC_SERVER::CONTINUOUSDAMAGE_INSERT( SCONTINUOUS_DAMAGE_DATA _sDATA )
{
	if ( !_sDATA.Valid() )	return;

	m_mapContinuousDamage.insert( std::make_pair( _sDATA.sidSkill.dwID, _sDATA ) );
}

/*continuous damage skill logic, Juver, 2017/06/10 */
void GLCHARLOGIC_SERVER::CONTINUOUSDAMAGE_DELETE( SNATIVEID _sID )
{
	if( m_mapContinuousDamage.empty() )	return;
	if( _sID == NATIVEID_NULL() )		return;

	CONTINUOUS_DAMAGE_DATA_MAP_ITER iter = m_mapContinuousDamage.find(_sID.dwID);
	if ( iter != m_mapContinuousDamage.end() )
		m_mapContinuousDamage.erase( iter );
}

/*npc talk buff condition, Juver, 2017/10/03 */
BOOL GLCHARLOGIC_SERVER::ISHAVE_BUFF( SNATIVEID skill_id, WORD wLEVEL/*=1*/ )
{
	//level range 1~9
	if ( wLEVEL == 0 )	return FALSE;
	if ( wLEVEL > SKILL::MAX_SKILL_LEVEL )	return FALSE;

	//bypass check for gm
	if ( m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 )	return TRUE;

	//range in buff data 0~8
	WORD wLevel = wLEVEL -1;

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )	return FALSE;

	//check buffs
	for( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		const SSKILLFACT& skill_fact = m_sSKILLFACT[i];
		if ( skill_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( skill_fact.sNATIVEID == skill_id && skill_fact.wLEVEL >= wLevel )	
			return TRUE;
	}

	//check item food
	for( int i=0; i<FITEMFACT_SIZE; ++i )
	{
		const SFITEMFACT& fitem_fact = m_sFITEMFACT[i];
		if ( fitem_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		/*if ( !fitem_fact.bENABLE ) 
			continue;*/

		if ( fitem_fact.sNATIVEID == skill_id && fitem_fact.wLEVEL >= wLevel )	
			return TRUE;
	}

	//check system buffs
	for( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
	{
		const SSYSTEM_BUFF& system_buff = m_sSYSTEM_BUFF[i];
		if ( system_buff.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( system_buff.sNATIVEID == skill_id && system_buff.wLEVEL >= wLevel )	
			return TRUE;
	}

	return FALSE;
}

/*activity system, Juver, 2017/10/30 */
SACTIVITY_CHAR_DATA* GLCHARLOGIC_SERVER::GetActivityProg( DWORD dwActivityID )
{
	if ( dwActivityID == UINT_MAX )	return NULL;

	SACTIVITY_CHAR_DATA_MAP_ITER iter = m_mapActivityProg.find( dwActivityID );
	if ( iter != m_mapActivityProg.end() )
	{
		return &(*iter).second;
	}

	return NULL;
}

/*activity system, Juver, 2017/10/30 */
SACTIVITY_CHAR_DATA* GLCHARLOGIC_SERVER::GetActivityDone( DWORD dwActivityID )
{
	if ( dwActivityID == UINT_MAX )	return NULL;

	SACTIVITY_CHAR_DATA_MAP_ITER iter = m_mapActivityDone.find( dwActivityID );
	if ( iter != m_mapActivityDone.end() )
	{
		return &(*iter).second;
	}

	return NULL;
}

/*activity system, Juver, 2017/10/30 */
DWORD GLCHARLOGIC_SERVER::GetActivityProgNum()
{
	return DWORD( m_mapActivityProg.size() );
}

/*activity system, Juver, 2017/10/30 */
DWORD GLCHARLOGIC_SERVER::GetActivityDoneNum()
{
	return DWORD( m_mapActivityDone.size() );
}

/* Reset Stats Item */
bool GLCHARLOGIC_SERVER::STATS_RESET ()
{
	const DWORD dwCurrentStats = m_sStats.GetTotal() + m_wStatsPoint;

	m_sStats.RESET();
	m_wStatsPoint = dwCurrentStats;

	INIT_DATA(FALSE,FALSE);

	return true;
}

/*official random value, Juver, 2018/06/12 */
WORD GLCHARLOGIC_SERVER::get_additional_gain_hp()  const
{
	return (WORD)m_sSUMITEM.nadd_gain_hp;
}

/*official random value, Juver, 2018/06/12 */
WORD GLCHARLOGIC_SERVER::get_additional_gain_mp()  const
{
	return (WORD)m_sSUMITEM.nadd_gain_mp;
}

/*official random value, Juver, 2018/06/12 */
WORD GLCHARLOGIC_SERVER::get_additional_gain_sp()  const
{
	return (WORD)m_sSUMITEM.nadd_gain_sp;
}

/*official random value, Juver, 2018/06/12 */
WORD GLCHARLOGIC_SERVER::get_additional_gain_cp()  const
{
	return (WORD)m_sSUMITEM.nadd_gain_cp;
}

/*12-9-14, Battle Pass - CNDev*/
SBATTLEPASS_CHAR_DATA* GLCHARLOGIC_SERVER::GetBattlePassProg( DWORD dwBattlePassID )
{
	if ( dwBattlePassID == UINT_MAX )	return NULL;

	SBATTLEPASS_CHAR_DATA_MAP_ITER iter = m_mapBattlePassProg.find( dwBattlePassID );
	if ( iter != m_mapBattlePassProg.end() )
	{
		return &(*iter).second;
	}

	return NULL;
}

SBATTLEPASS_CHAR_DATA* GLCHARLOGIC_SERVER::GetBattlePassDone( DWORD dwBattlePassID )
{
	if ( dwBattlePassID == UINT_MAX )	return NULL;

	SBATTLEPASS_CHAR_DATA_MAP_ITER iter = m_mapBattlePassDone.find( dwBattlePassID );
	if ( iter != m_mapBattlePassDone.end() )
	{
		return &(*iter).second;
	}

	return NULL;
}

DWORD GLCHARLOGIC_SERVER::GetBattlePassProgNum()
{
	return DWORD( m_mapBattlePassProg.size() );
}

DWORD GLCHARLOGIC_SERVER::GetBattlePassDoneNum()
{
	return DWORD( m_mapBattlePassDone.size() );
}
/*additional RV, Juver, 2018/06/14 */
float GLCHARLOGIC_SERVER::get_critical_hit_chance()  const
{
	int val = (int)( m_sSUMITEM.fadd_critical_hit_chance );

	//non rv value
	val += (int)( m_sSUMITEM.fIncR_Critical * 100 );
	val += (int)( m_sSUMITEM.fInc_Critical );

	if ( val > (int)GLCONST_CHAR::dwCRITICAL_MAX )		val = GLCONST_CHAR::dwCRITICAL_MAX;
	if ( val < 0 )	val = 0;

	return (float)val;
}

/*additional RV, Juver, 2018/06/14 */
float GLCHARLOGIC_SERVER::get_critical_hit_damage()  const
{
	int val = int( GLCONST_CHAR::dwCRITICAL_DAMAGE + (int)m_sSUMITEM.fadd_critical_hit_damage );
	return (float)val;
}

/*additional RV, Juver, 2018/06/14 */
float GLCHARLOGIC_SERVER::get_crushing_blow_chance()  const
{
	int val = (int)( m_sSUMITEM.fadd_crushing_blow_chance );

	//non rv value
	val += (int)( m_sSUMITEM.fIncR_CrushingBlow * 100 );
	val += (int)( m_sSUMITEM.fInc_CrushingBlow );

	if ( val > (int)GLCONST_CHAR::dwCRUSHING_BLOW_MAX )		val = GLCONST_CHAR::dwCRUSHING_BLOW_MAX;
	if ( val < 0 )	val = 0;

	return (float)val;
}

/*additional RV, Juver, 2018/06/14 */
float GLCHARLOGIC_SERVER::get_crushing_blow_damage()  const
{
	int val = int( GLCONST_CHAR::dwCRUSHING_BLOW_DAMAGE + (int)m_sSUMITEM.fadd_crushing_blow_damage );
	return (float)val;
}

EMCHECK_WEAR GLCHARLOGIC_SERVER::CHECK_WEAR_ITEM_SLOT( SNATIVEID sItemID, EMSLOT emPutOnSlot )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemID );
	if ( !pItem )	return EMCHECK_WEAR_INVALID_ITEM;

	const SITEM &sItem = *pItem;

	
	if ( RPARAM::bCheckWearSlot )
	{
		BOOL bOK = CHECKSLOT_ITEM( sItemID, emPutOnSlot );
		if ( !bOK )
			return EMCHECK_WEAR_ERROR_WEAR_SLOT;
	}



	return EMCHECK_WEAR_PASS;
}

/* skill effect release, Juver, 2020/04/30 */
BOOL GLCHARLOGIC_SERVER::EffectReleaseExist( SNATIVEID _sID )
{
	if( m_mapEffectRelease.empty() )	return FALSE;
	if( _sID == NATIVEID_NULL() )		return FALSE;

	SEFFECT_RELEASE_DATA_MAP_ITER iter = m_mapEffectRelease.find(_sID.dwID);
	if ( iter != m_mapEffectRelease.end() )	return TRUE;

	return FALSE;
}

/* skill effect release, Juver, 2020/04/30 */
void GLCHARLOGIC_SERVER::EffectReleaseInsert( const SEFFECT_RELEASE_DATA& _sData )
{
	if ( !_sData.isValid() )	return;

	m_mapEffectRelease.insert( std::make_pair( _sData.sidSkill.dwID, _sData ) );
}

/* skill effect release, Juver, 2020/04/30 */
void GLCHARLOGIC_SERVER::EffectReleaseDelete( SNATIVEID _sID )
{
	if( m_mapEffectRelease.empty() )	return;
	if( _sID == NATIVEID_NULL() )		return;

	SEFFECT_RELEASE_DATA_MAP_ITER iter = m_mapEffectRelease.find(_sID.dwID);
	if ( iter != m_mapEffectRelease.end() )
		m_mapEffectRelease.erase( iter );
}

/* skill delay addition, Juver, 2020/12/09 */
BOOL GLCHARLOGIC_SERVER::SkillDelayDataExist( SNATIVEID _sID )
{
	if( m_mapSkillDelay.empty() )			return FALSE;
	if( _sID == NATIVEID_NULL() )			return FALSE;

	SKILL_DELAY_DATA_MAP_ITER iter = m_mapSkillDelay.find(_sID.dwID);
	if ( iter != m_mapSkillDelay.end() )	return TRUE;

	return FALSE;
}

/* skill delay addition, Juver, 2020/12/09 */
void GLCHARLOGIC_SERVER::SkillDelayDataInsert( const SSKILL_DELAY_DATA& _sData )
{
	if ( !_sData.Valid() )	return;

	m_mapSkillDelay.insert( std::make_pair( _sData.sSkillID.dwID, _sData ) );
}

/* skill delay addition, Juver, 2020/12/09 */
void GLCHARLOGIC_SERVER::SkillDelayDataDelete( SNATIVEID _sID )
{
	if( m_mapSkillDelay.empty() )		return;
	if( _sID == NATIVEID_NULL() )		return;

	SKILL_DELAY_DATA_MAP_ITER iter = m_mapSkillDelay.find(_sID.dwID);
	if ( iter != m_mapSkillDelay.end() )
		m_mapSkillDelay.erase( iter );
}

float GLCHARLOGIC_SERVER::GETSUM_TARRANGE () const	
{ 
	return m_fSUM_TARGET_RANGE; 
}

int GLCHARLOGIC_SERVER::GETSUM_PIERCE () const		
{ 
	return m_nSUM_PIERCE;
}

/* set item option, Juver, 2021/09/09 */
bool GLCHARLOGIC_SERVER::SetOptionIsSuit( EMSUIT emSuit )
{
	return false;
}
/* Codex, Archie 02/16/24 */
SCODEX_CHAR_DATA* GLCHARLOGIC_SERVER::GetCodexProg( DWORD dwCodexID )
{
	if ( dwCodexID == UINT_MAX )	return NULL;

	SCODEX_CHAR_DATA_MAP_ITER iter = m_mapCodexProg.find( dwCodexID );
	if ( iter != m_mapCodexProg.end() )
	{
		return &(*iter).second;
	}

	return NULL;
}

/* Codex, Archie 02/16/24 */
SCODEX_CHAR_DATA* GLCHARLOGIC_SERVER::GetCodexDone( DWORD dwCodexID )
{
	if ( dwCodexID == UINT_MAX )	return NULL;

	SCODEX_CHAR_DATA_MAP_ITER iter = m_mapCodexDone.find( dwCodexID );
	if ( iter != m_mapCodexDone.end() )
	{
		return &(*iter).second;
	}

	return NULL;
}

/* Codex, Archie 02/16/24 */
DWORD GLCHARLOGIC_SERVER::GetCodexProgNum()
{
	return DWORD( m_mapCodexProg.size() );
}

/* Codex, Archie 02/16/24 */
DWORD GLCHARLOGIC_SERVER::GetCodexDoneNum()
{
	return DWORD( m_mapCodexDone.size() );
}
/* Codex HP, Archie 02/16/24 */
void GLCHARLOGIC_SERVER::CODEX_STATS( int nIndex, DWORD dwStatPoint )
{

	WORD m_dwHPIncreaseDummy = 0;
	WORD m_dwMPIncreaseDummy = 0;
	WORD m_dwSPIncreaseDummy = 0;
	WORD m_dwAttackIncreaseDummy = 0;
	WORD m_dwDefenseIncreaseDummy = 0;
	WORD m_dwShootingIncreaseDummy = 0;
	WORD m_dwMeleeIncreaseDummy = 0;
	WORD m_dwEnergyIncreaseDummy = 0;
	WORD m_dwResistanceIncreaseDummy = 0;
	WORD m_dwHitrateIncreaseDummy = 0;
	WORD m_dwAvoidrateIncreaseDummy = 0;
	
	//DWORD dwCodexDoneSize = m_mapCodexDone.size();
	//m_dwCodexDoneSize = m_mapCodexDone.size();
	//if ( m_dwCodexDoneSize != m_mapCodexDone.size() )
	{
		for( SCODEX_CHAR_DATA_MAP_ITER iter_codex_done = m_mapCodexDone.begin();
			iter_codex_done != m_mapCodexDone.end(); ++ iter_codex_done )
		{
			SCODEX_CHAR_DATA& scodex_char_data = (*iter_codex_done).second;
			SCODEX_FILE_DATA* pcodex_char = GLCodex::GetInstance().GetCodex( scodex_char_data.dwCodexID );
			
			//if ( scodex_char_data.dwProgressNow < 1 ) continue;
			
			//dwProgNum++;
			/* Codex HP, Archie 02/16/24 */
			if ( pcodex_char )
			{
				if ( scodex_char_data.emType == EMCODEX_TYPE_REACH_LEVEL )
					m_dwHPIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_KILL_MOB )
					m_dwMPIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_KILL_PLAYER )
					m_dwSPIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_REACH_MAP )
					m_dwAttackIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_TAKE_ITEM )
					m_dwDefenseIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_USE_ITEM )
					m_dwShootingIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_REACH_CODEX )
					m_dwMeleeIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_COMPLETE_QUEST )
					m_dwEnergyIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_CODEX_POINT )
					m_dwResistanceIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_QUESTION_BOX )
					m_dwHitrateIncreaseDummy += pcodex_char->dwRewardPoint;
				if ( scodex_char_data.emType ==EMCODEX_TYPE_ETC )
					m_dwAvoidrateIncreaseDummy += pcodex_char->dwRewardPoint;
			}
		}
		{
			/* Codex HP, Archie 02/16/24 */
			m_dwHPIncrease = m_dwHPIncreaseDummy;
			m_dwMPIncrease = m_dwMPIncreaseDummy;
			m_dwSPIncrease = m_dwSPIncreaseDummy;
			m_dwAttackIncrease = m_dwAttackIncreaseDummy;
			m_dwDefenseIncrease = m_dwDefenseIncreaseDummy;
			m_dwShootingIncrease = m_dwShootingIncreaseDummy;
			m_dwMeleeIncrease = m_dwMeleeIncreaseDummy;
			m_dwEnergyIncrease = m_dwEnergyIncreaseDummy;
			m_dwResistanceIncrease = m_dwResistanceIncreaseDummy;
			m_dwHitrateIncrease = m_dwHitrateIncreaseDummy;
			m_dwAvoidrateIncrease = m_dwAvoidrateIncreaseDummy;
		}
	}
	INIT_DATA(FALSE,FALSE);
}

//MMR Rank
void GLCHARLOGIC_SERVER::ShowRankName( BOOL bShowName ) 
{
	m_bRankName = bShowName;
}
void GLCHARLOGIC_SERVER::ShowRankMark( BOOL bShowMark )
{
	m_bRankMark = bShowMark;
}