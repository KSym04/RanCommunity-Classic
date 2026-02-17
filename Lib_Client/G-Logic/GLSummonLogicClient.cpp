#include "stdafx.h"
#include "./GLItemMan.h"
#include "./GlItemLMT.h"

#include "../../Lib_Engine/Common/GLTexFile.h"
#include "../../Lib_Engine/G-Logic/Glogic.h"

#include "./GLSummonLogicClient.h"
#include "./GLogicCommon.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLSUMMONLOGIC_CLIENT& GLSUMMONLOGIC_CLIENT::operator= ( const GLSUMMONLOGIC_CLIENT& rvalue )
{
	m_emTYPE			= rvalue.m_emTYPE;
	m_dwAType			= rvalue.m_dwAType;
	m_fRunArea			= rvalue.m_fRunArea;
	m_fWalkArea			= rvalue.m_fWalkArea;
	m_fOwnerDistance	= rvalue.m_fOwnerDistance;

	m_dwGUID	  = rvalue.m_dwGUID;
	m_sSummonID   = rvalue.m_sSummonID;
	m_dwOwner	  = rvalue.m_dwOwner;
	m_sMapID	  = rvalue.m_sMapID;
	m_dwCellID	  = rvalue.m_dwCellID;
	m_dwNowHP	  = rvalue.m_dwNowHP;
	m_wNowMP	  = rvalue.m_wNowMP;

	/*skill summon, Juver, 2017/10/09 */
	m_wArrayIndex = rvalue.m_wArrayIndex;	
	m_Summon      = rvalue.m_Summon;

	return *this;
}

void GLSUMMONLOGIC_CLIENT::ASSIGN ( const GLSUMMONLOGIC_CLIENT& sSummonData )
{
	m_emTYPE			= sSummonData.m_emTYPE;
	m_dwAType			= sSummonData.m_dwAType;
	m_fRunArea			= sSummonData.m_fRunArea;
	m_fWalkArea			= sSummonData.m_fWalkArea;
	m_fOwnerDistance	= sSummonData.m_fOwnerDistance;

	m_dwGUID	  = sSummonData.m_dwGUID;
	m_sSummonID   = sSummonData.m_sSummonID;
	m_dwOwner	  = sSummonData.m_dwOwner;
	m_sMapID	  = sSummonData.m_sMapID;
	m_dwCellID	  = sSummonData.m_dwCellID;
	m_dwNowHP	  = sSummonData.m_dwNowHP;
	m_wNowMP	  = sSummonData.m_wNowMP;

	/*skill summon, Juver, 2017/10/09 */
	m_wArrayIndex = sSummonData.m_wArrayIndex;	
	m_Summon      = sSummonData.m_Summon;
}

D3DXVECTOR3  GLSUMMONLOGIC_CLIENT::GetRandomPostision ()
{
	srand(unsigned int(time(NULL)));
	WORD idx = (WORD)(rand() % 8);
	return m_RandPos[idx];
}

void GLSUMMONLOGIC_CLIENT::RESET ()
{
	m_dwGUID	 = UINT_MAX;
	m_sSummonID  = NATIVEID_NULL();
	m_dwOwner 	 = 0;
	m_sMapID	 = NATIVEID_NULL();
	m_dwCellID	 = 0;
	m_sDamageSpec.RESET();
	m_sSUMRESIST.RESET();
	m_sDefenseSkill.RESET();

	m_wArrayIndex = 0; /*skill summon, Juver, 2017/10/09 */
	m_Summon.Reset();
}

void GLSUMMONLOGIC_CLIENT::RESET_DATA ()
{
	m_sSummonID = SNATIVEID(0,0);
	m_pSummonCrowData = NULL;

	for ( int i=0; i< SCROWDATA::EMMAXATTACK; ++i )	
		m_fACTIONDELAY[i] = 0.0f;

	for ( int i=0; i< SKILLFACT_SIZE; ++i )
		m_sSKILLFACT[i] = SSKILLFACT();

	m_dwHOLDBLOW = NULL;

	for ( int i=0; i< EMBLOW_MULTI; ++i )
		m_sSTATEBLOWS[i] = SSTATEBLOW();

	m_fSTATE_MOVE = 1.0f;
	m_fSTATE_DELAY = 1.0f;
	m_fSTATE_DAMAGE = 1.0f;

	m_bSTATE_PANT = false;
	m_bSTATE_STUN = false;

	m_fATTVELO = 0.0f;
	m_fSKILL_MOVE = 0.0f;

	m_nSUM_HIT = 0;
	m_nSUM_AVOID = 0;

	m_nSUM_DEFENSE = 0;
	m_nDX_DAMAGE = 0;

	m_nSUM_PIERCE = 0;
	m_fSUM_TARRANGE = 0;

	m_fDamageRate = 1.0f;
	m_fDefenseRate = 1.0f;

	m_dwAType = 0;

	m_dwNowHP = 0;
	m_wNowMP = 0;
	m_wNowSP = 0;

	m_idACTIVESKILL = false;
	m_wACTIVESKILL_LVL = 0;

	m_fIncHP = 0.0f;
	m_fIncMP = 0.0f;
	m_fIncSP = 0.0f;

	/*prohibit potion skill logic, Juver, 2017/06/06 */
	m_bProhibitPotion = false; 

	/*prohibit skill logic, Juver, 2017/06/06 */
	m_bProhibitSkill = false; 

	/*skill range spec logic, Juver, 2017/06/06 */
	m_fSUM_SKILL_ATTACKRANGE = 0.0f;
	m_fSUM_SKILL_APPLYRANGE = 0.0f;

	/*continuous damage skill logic, Juver, 2017/06/10 */
	m_mapContinuousDamage.clear();

	/*curse skill logic, Juver, 2017/06/10 */
	m_bCurse = false;
	m_fCurseDamage = 0.0f;

	/*skill summon, Juver, 2017/10/09 */
	m_wArrayIndex = 0;
	m_Summon.Reset();

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

	/* skill amplify, Juver, 2020/12/10 */
	m_sSkillAmplifyMelee.RESET();
	m_sSkillAmplifyMissile.RESET();
	m_sSkillAmplifyMagic.RESET();

	/* skill position shift, Juver, 2020/12/15 */
	m_bSkillPositionShift = false;

	/* skill buff duration change, Juver, 2020/12/17 */
	m_sSkillDurationChange.Reset();
}

const char* GLSUMMONLOGIC_CLIENT::GETNAME ()
{
	GASSERT(m_pSummonCrowData);
	return m_pSummonCrowData->GetName();
}

int GLSUMMONLOGIC_CLIENT::GETFORCE_LOW () const
{
	int nDamage = m_pSummonCrowData->m_sCrowAttack[m_dwAType].sDamage.wLow;

	if ( (nDamage+m_nDX_DAMAGE) < 0 )	return 0;
	return nDamage + m_nDX_DAMAGE;
}

int GLSUMMONLOGIC_CLIENT::GETFORCE_HIGH () const
{
	int nDamage = m_pSummonCrowData->m_sCrowAttack[m_dwAType].sDamage.wHigh;

	if ( (nDamage+m_nDX_DAMAGE) < 0 )	return 0;
	return nDamage + m_nDX_DAMAGE;
}

WORD GLSUMMONLOGIC_CLIENT::GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const
{
	const SKILL::CDATA_LVL &sDATA_LVL = sSKILL.m_sAPPLY.sDATA_LVL[dwLEVEL];

	WORD dwRANGE = sDATA_LVL.wAPPLYRANGE;
	if ( sSKILL.m_sBASIC.emAPPLY==SKILL::EMAPPLY_PHY_LONG )		dwRANGE += (WORD) GETSUM_TARRANGE();

	/*skill range spec logic, Juver, 2017/06/06 */
	if ( sSKILL.m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )	dwRANGE += (WORD) m_fSUM_SKILL_APPLYRANGE;

	return dwRANGE;
}

BOOL GLSUMMONLOGIC_CLIENT::INIT_DATA ()
{
	m_sSummonID = m_pSummonCrowData->m_sBasic.sNativeID;
	m_dwNowHP = m_pSummonCrowData->m_sBasic.m_dwHP;
	m_wNowMP = m_pSummonCrowData->m_sBasic.m_wMP;

	return TRUE;
}

EMSKILLCHECK GLSUMMONLOGIC_CLIENT::CHECHSKILL ( DWORD dwAType,  bool bNotLearn )
{
	GASSERT(dwAType<SCROWDATA::EMMAXATTACK);

	/*prohibit skill logic, Juver, 2017/06/06 */
	if ( m_bProhibitSkill )	return EMSKILL_PROHIBIT;

	SNATIVEID skill_id;
	WORD wskill_lev = 0;
	WORD wStrikeNum = 0;
	if ( bNotLearn )
	{
		skill_id = m_sDefenseSkill.m_dwSkillID;
		wskill_lev = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		const SCROWATTACK &sATTACK = m_pSummonCrowData->m_sCrowAttack[dwAType];
		skill_id = sATTACK.skill_id;
		wskill_lev = sATTACK.skill_lev;

		//	Note : 스킬 딜래이 타임이 지낫는지 점검.
		//
		if ( m_fACTIONDELAY[dwAType] > 0.0f )	
			return EMSKILL_DELAYTIME; // 스킬 딜래이 시간이 지나지 않음.
	}




	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )															return EMSKILL_UNKNOWN;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wskill_lev];

	if ( GETHP() <= sSKILL_DATA.wUSE_HP*wStrikeNum )						return EMSKILL_NOTHP;
	if ( GETMP() < sSKILL_DATA.wUSE_MP*wStrikeNum )							return EMSKILL_NOTMP;
	//if ( GETSP() < sSKILL_DATA.wUSE_SP*wStrikeNum )							return EMSKILL_NOTSP;


	return EMSKILL_OK;
}

DWORD GLSUMMONLOGIC_CLIENT::RECEIVE_DAMAGE ( const WORD wDamage )
{
	DWORD dwOLD = m_dwNowHP;

	DECREASE(m_dwNowHP,(DWORD)wDamage);

	return (dwOLD>m_dwNowHP) ? (dwOLD-m_dwNowHP) : 0;
}

void GLSUMMONLOGIC_CLIENT::UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient )
{
	const float fUNIT_TIME = GLCONST_CHAR::fUNIT_TIME_CROW;
	float m_fINCR_HP = GLCONST_CHAR::fHP_INC_PER_CROW;
	float m_fINCR_MP = GLCONST_CHAR::fMP_INC_PER_CROW;
	float m_fINCR_SP = GLCONST_CHAR::fSP_INC_PER_CROW;

	m_nSUM_HIT = static_cast<int>(m_pSummonCrowData->m_sBasic.m_wHitRate);
	m_nSUM_AVOID = static_cast<int>(m_pSummonCrowData->m_sBasic.m_wAvoidRate);

	m_nSUM_DEFENSE = static_cast<int>(m_pSummonCrowData->m_sBasic.m_wDefense);

	m_fSTATE_MOVE = 1.0f;
	m_fSTATE_DELAY = 1.0f;

	m_fSTATE_DAMAGE = 1.0f;

	m_bSTATE_PANT = false;
	m_bSTATE_STUN = false;
	
	m_fATTVELO = 0.0f;
	m_fSKILL_MOVE = 0.0f;

	m_nDX_DAMAGE = 0;

	m_fSUM_TARRANGE = 0;
	m_nSUM_PIERCE = 0;

	m_fDamageRate = 1.0f;
	m_fDefenseRate = 1.0f;

	m_sDamageSpec.RESET();
	m_sDefenseSkill.RESET();

	m_sSUMRESIST = m_pSummonCrowData->m_sBasic.m_sResist;

	/*prohibit potion skill logic, Juver, 2017/06/06 */
	m_bProhibitPotion = false; 

	/*prohibit skill logic, Juver, 2017/06/06 */
	m_bProhibitSkill = false; 

	/*skill range spec logic, Juver, 2017/06/06 */
	m_fSUM_SKILL_ATTACKRANGE = 0.0f;
	m_fSUM_SKILL_APPLYRANGE = 0.0f;

	/*curse skill logic, Juver, 2017/06/10 */
	m_bCurse = false;
	m_fCurseDamage = 0.0f;

	/*ignore damage, Juver, 2017/12/12 */
	m_bIgnoreDamage = false;

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

	/* skill amplify, Juver, 2020/12/10 */
	m_sSkillAmplifyMelee.RESET();
	m_sSkillAmplifyMissile.RESET();
	m_sSkillAmplifyMagic.RESET();

	/* skill buff duration change, Juver, 2020/12/17 */
	m_sSkillDurationChange.Reset();

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		SSKILLFACT &sSKEFF = m_sSKILLFACT[i];
		if ( sSKEFF.sNATIVEID==NATIVEID_NULL() )	continue;

		sSKEFF.fAGE -= fElapsedTime;

		//	클라이언트가 아닐 경우 여기서 스킬 이펙트를 비활성화, 클라이언트는 UpdateSkillEffect()에서 함.
		if ( !bClient && sSKEFF.fAGE <= 0.0f )		DISABLESKEFF(i);

		switch ( sSKEFF.emTYPE )
		{
		case SKILL::EMFOR_VARHP:
			m_fIncHP += (fElapsedTime/fUNIT_TIME) * (sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_VARMP:
			m_fIncMP += (fElapsedTime/fUNIT_TIME) * (sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_VARSP:
			m_fIncSP += (fElapsedTime/fUNIT_TIME) * (sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_VARAP:
			m_fIncHP += (fElapsedTime/fUNIT_TIME) * (sSKEFF.fMVAR);
			m_fIncMP += (fElapsedTime/fUNIT_TIME) * (sSKEFF.fMVAR);
			m_fIncSP += (fElapsedTime/fUNIT_TIME) * (sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_DEFENSE:
			m_nSUM_DEFENSE += (int) sSKEFF.fMVAR;
			break;

		case SKILL::EMFOR_HITRATE:
			m_nSUM_HIT += int(sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_AVOIDRATE:
			m_nSUM_AVOID += int(sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_VARDAMAGE:
			m_nDX_DAMAGE += int(sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_VARDEFENSE:
			m_nSUM_DEFENSE += int(sSKEFF.fMVAR);
			break;

		case SKILL::EMFOR_RESIST:
			m_sSUMRESIST += int(sSKEFF.fMVAR);
			break;
		};

		for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
		{
			switch ( sSKEFF.sImpacts[nImpact].emADDON )
			{
			case EMIMPACTA_HITRATE:
				m_nSUM_HIT += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);
				break;

			case EMIMPACTA_AVOIDRATE:
				m_nSUM_AVOID += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);
				break;

			case EMIMPACTA_DAMAGE:
				m_nDX_DAMAGE += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);
				break;

			case EMIMPACTA_DEFENSE:
				m_nSUM_DEFENSE += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);
				break;

			case EMIMPACTA_VARHP:
				m_fIncHP += sSKEFF.sImpacts[nImpact].fADDON_VAR*fElapsedTime;
				break;

			case EMIMPACTA_VARMP:
				m_fIncMP += sSKEFF.sImpacts[nImpact].fADDON_VAR*fElapsedTime;
				break;

			case EMIMPACTA_VARSP:
				m_fIncSP += sSKEFF.sImpacts[nImpact].fADDON_VAR*fElapsedTime;
				break;

			case EMIMPACTA_VARAP:
				m_fIncHP += sSKEFF.sImpacts[nImpact].fADDON_VAR*fElapsedTime;
				m_fIncMP += sSKEFF.sImpacts[nImpact].fADDON_VAR*fElapsedTime;
				m_fIncSP += sSKEFF.sImpacts[nImpact].fADDON_VAR*fElapsedTime;
				break;

			case EMIMPACTA_DAMAGE_RATE:
				m_fDamageRate += sSKEFF.sImpacts[nImpact].fADDON_VAR;
				break;

			case EMIMPACTA_DEFENSE_RATE:
				m_fDefenseRate += sSKEFF.sImpacts[nImpact].fADDON_VAR;
				break;

			case EMIMPACTA_RESIST:
				m_sSUMRESIST += int(sSKEFF.sImpacts[nImpact].fADDON_VAR);
				break;
			};
		}
		

		m_dwHOLDBLOW = NULL;

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			switch ( sSKEFF.sSpecs[nSpec].emSPEC )
			{
			case EMSPECA_NONBLOW:
				m_dwHOLDBLOW = sSKEFF.sSpecs[nSpec].dwSPECFLAG;
				break;

			case EMSPECA_MOVEVELO:
				m_fSKILL_MOVE += sSKEFF.sSpecs[nSpec].fSPECVAR1;
				break;

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

			case EMSPECA_ATTACKVELO:
				//	공격 속도 계념이므로 ( 공격 시간을 -0.1 (-10%) 감소 시키기 위해서는 부호 반전을 하여야함. )
				m_fATTVELO -= sSKEFF.sSpecs[nSpec].fSPECVAR1;
				break;

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

				/*prohibit potion skill logic, Juver, 2017/06/06 */
			case EMSPECA_PROHIBIT_POTION:	m_bProhibitPotion = true;			break;

				/*prohibit skill logic, Juver, 2017/06/06 */
			case EMSPECA_PROHIBIT_SKILL:	m_bProhibitSkill = true;			break;

				/*skill range spec logic, Juver, 2017/06/06 */
			case EMSPECA_CHANGE_ATTACK_RANGE:
				{
					m_fSUM_SKILL_ATTACKRANGE += sSKEFF.sSpecs[nSpec].fSPECVAR1;		
				}break;
			case EMSPECA_CHANGE_APPLY_RANGE:
				{
					m_fSUM_SKILL_APPLYRANGE += sSKEFF.sSpecs[nSpec].fSPECVAR1;		
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

				/*skill transform, Juver, 2018/09/08 */
			case EMSPECA_TRANSFORM:
				{
					if ( m_pSummonCrowData && m_pSummonCrowData->m_sBasic.IsNoTransform() == false )
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
			};
		}
		
	}

	for ( int i=0; i<EMBLOW_MULTI; ++i )
	{
		SSTATEBLOW &sSTATEBLOW = m_sSTATEBLOWS[i];
		if ( sSTATEBLOW.emBLOW == EMBLOW_NONE )		continue;

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
			m_fIncHP += (sSTATEBLOW.fSTATE_VAR2*fElapsedTime);
			break;

		case EMBLOW_BURN:
			m_fIncHP += (sSTATEBLOW.fSTATE_VAR2*fElapsedTime);	//	지속타격
			break;

		case EMBLOW_FROZEN:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	이동속도 감소.
			m_fSTATE_DAMAGE += sSTATEBLOW.fSTATE_VAR2;			//	타격증폭.
			break;

		case EMBLOW_MAD:
			m_nSUM_HIT = 30;
			m_nSUM_AVOID = 30;
			m_bSTATE_PANT = true;
			break;

		case EMBLOW_POISON:
			m_fIncHP += (sSTATEBLOW.fSTATE_VAR2*fElapsedTime);	//	지속타격
			break;

		case EMBLOW_CURSE:
			m_fIncHP += (sSTATEBLOW.fSTATE_VAR2*fElapsedTime);	//	지속타격
			m_fINCR_MP = 0.0f;
			break;
		};
	}

	if ( m_fDamageRate < 0.0f )		m_fDamageRate = 0.0f;
	if ( m_fDefenseRate < 0.0f )	m_fDefenseRate = 0.0f;

	m_fATTVELO += m_fSTATE_MOVE;

	if ( m_pSummonCrowData->IsInVisible() )
	{
		/* skill turn/find invisible, Juver, 2020/04/25 */
		if ( !m_bSkillTurnInvisible )
		{
			m_bSkillTurnInvisible = true;
			m_wSkillTurnInvisibleLevel = CROW_DEFAULT_INVISIBLE_LEVEL;
			m_wSkillTurnInvisibleAnimation = CROW_DEFAULT_INVISIBLE_ANIMATION;
			m_bSkillTurnInvisibleDisableInCombat = CROW_DEFAULT_INVISIBLE_DISABLE_IN_COMBAT;
		}
	}

	if ( m_pSummonCrowData->IsRecVisible() )
	{
		/* skill turn/find invisible, Juver, 2020/04/25 */
		if ( !m_bSkillShowInvisible )
		{
			m_bSkillShowInvisible = true;
			m_wSkillShowInvisibleLevel = CROW_DEFAULT_SHOW_INVISIBLE_LEVEL;
			m_wSkillShowInvisibleRange = CROW_DEFAULT_SHOW_INVISIBLE_RANGE;
		}
	}

	//	방어력. 변화율 반영.
	m_nSUM_DEFENSE = int ( m_nSUM_DEFENSE * m_fDefenseRate );
	//	방어력이 - 값을 가지지 않게.
	if ( m_nSUM_DEFENSE < 0 )			m_nSUM_DEFENSE = 1;

	m_sSUMRESIST.LIMIT();

	//	MP 증가율이 없을때.
	if ( (m_fINCR_MP==0.0f) && (m_fIncMP>0) )		m_fIncMP = 0;

	//	Note : 체력 변화.	
	//
	float fElap = (fElapsedTime/fUNIT_TIME);
	GLOGICEX::UPDATE_POINT ( m_dwNowHP, m_fIncHP, GETMAXHP(), fElap*(GETMAXHP()*m_fINCR_HP), 1 );
	GLOGICEX::UPDATE_POINT ( m_wNowMP, m_fIncMP, GETMAXMP(), fElap*(GETMAXMP()*m_fINCR_MP), 0 );
	GLOGICEX::UPDATE_POINT ( m_wNowSP, m_fIncSP, GETMAXSP(), fElap*(GETMAXSP()*m_fINCR_SP), 0 );
}

float GLSUMMONLOGIC_CLIENT::GETATTVELO ()
{
	return m_fATTVELO<0.0f?0.0f:m_fATTVELO;
}

float GLSUMMONLOGIC_CLIENT::GETMOVEVELO ()
{
	float fMOVE = m_fSTATE_MOVE + m_fSKILL_MOVE;
	return fMOVE<0.0f?0.0f:fMOVE;
}

/*continuous damage skill logic, Juver, 2017/06/10 */
BOOL GLSUMMONLOGIC_CLIENT::CONTINUOUSDAMAGE_EXIST( SNATIVEID _sID )
{
	if( m_mapContinuousDamage.empty() )	return FALSE;
	if( _sID == NATIVEID_NULL() )		return FALSE;

	CONTINUOUS_DAMAGE_DATA_MAP_ITER iter = m_mapContinuousDamage.find(_sID.dwID);
	if ( iter != m_mapContinuousDamage.end() )	return TRUE;

	return FALSE;
}

/*continuous damage skill logic, Juver, 2017/06/10 */
void GLSUMMONLOGIC_CLIENT::CONTINUOUSDAMAGE_INSERT( SCONTINUOUS_DAMAGE_DATA _sDATA )
{
	if ( !_sDATA.Valid() )	return;

	m_mapContinuousDamage.insert( std::make_pair( _sDATA.sidSkill.dwID, _sDATA ) );
}

/*continuous damage skill logic, Juver, 2017/06/10 */
void GLSUMMONLOGIC_CLIENT::CONTINUOUSDAMAGE_DELETE( SNATIVEID _sID )
{
	if( m_mapContinuousDamage.empty() )	return;
	if( _sID == NATIVEID_NULL() )		return;

	CONTINUOUS_DAMAGE_DATA_MAP_ITER iter = m_mapContinuousDamage.find(_sID.dwID);
	if ( iter != m_mapContinuousDamage.end() )
		m_mapContinuousDamage.erase( iter );
}

void GLSUMMONLOGIC_CLIENT::ACCOUNTSKILL ( WORD wStrikeNum )
{
	//	스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL.wMainID, m_idACTIVESKILL.wSubID );
	if ( !pSkill )	return;

	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[m_wACTIVESKILL_LVL];

	//	스킬 딜래이 등록.
	float fDelayTime = GLOGICEX::SKILLDELAY(pSkill->m_sBASIC.dwGRADE,m_wACTIVESKILL_LVL,GETLEVEL(),sSKILL_DATA.fDELAYTIME);

	//	Note : 상태이상의 딜래이 감안.
	m_fACTIONDELAY[m_dwAType] = fDelayTime * m_fSTATE_DELAY;

	//	기초 체력 소모.
	DECREASE ( m_dwNowHP, DWORD(sSKILL_DATA.wUSE_HP*wStrikeNum) );
	DECREASE ( m_wNowMP, WORD(sSKILL_DATA.wUSE_MP*wStrikeNum) );
}