/*!
 * \file GLLogicCharClient.h
 *
 * \author Juver
 * \date 2020/02/13
 *
 * 
 */

#ifndef GLLOGICCHARCLIENT_H_INCLUDED__
#define GLLOGICCHARCLIENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <hash_map>

#include "./GLogicCommon.h"
#include "./GLSKILL.h"
#include "./GLCharData.h"
#include "./GLCrowData.h"
#include "./GLogicData.h"
#include "./GLContrlMsg.h"
#include "./GLPVPTyrannyDefine.h" /*pvp tyranny, Juver, 2017/08/25 */

#include "./GLCodexData.h" /* Codex, Archie 02/16/24 */

/*activity system, Juver, 2017/10/30 */
#include "./GLActivityData.h"

/*skill transform, Juver, 2018/09/08 */
#include "./SkillTransformData.h"

#include "./CheckWearDefine.h"

/* post system, Juver, 2021/02/05 */
#include "./GLPostDefine.h"

#include "../../Lib_Helper/cMtSafeVar.h"

#ifndef GLSKILL
struct GLSKILL;
#endif //GLSKILL

//	Note : 변수 추가시에. RESET_DATA() 함수에 변수 초기화 넣어야됨.
//
struct GLCHARLOGIC_CLIENT :public SCHARDATA2
{	
	EMCHARINDEX			m_CHARINDEX;
	DELAY_MAP			m_SKILLDELAY;					//	사용된 스킬의 딜래이.

	SSKILLFACT			m_sSKILLFACT[SKILLFACT_SIZE];	//	스킬효과들.

	/*dual pet skill, Juver, 2017/12/27 */
	SSKILLFACT			m_sPETSKILLFACT_A;				//  팻의 보조스킬효과
	SSKILLFACT			m_sPETSKILLFACT_B;				//  팻의 보조스킬효과
	
	SPASSIVE_SKILL_DATA_CLIENT	m_sSUM_PASSIVE;					//	페시브 가산값.
	
	SLANDEFFECT			m_sLandEffect[EMLANDEFFECT_MULTI];	//  지형 이상 효과들

	DWORD				m_dwTransfromSkill;				//  변신 스킬 넘버


	float				m_fHP_RATE;						//	active 스킬의 HP 증폭율.
	float				m_fMP_RATE;						//	active 스킬의 MP 증폭율.
	float				m_fSP_RATE;						//	active 스킬의 SP 증폭율.

	DWORD				m_dwHOLDBLOW;					//	상태이상이 방지됨.
	BOOL				m_bSTATEBLOW;					//	상태이상이 하나 이상 발생했을때.
	SSTATEBLOW			m_sSTATEBLOWS[EMBLOW_MULTI];	//	상태이상.
	float				m_fSTATE_MOVE;					//	상태 이상 이동속도. ( 1.0f == 100% )
	float				m_fSTATE_DELAY;					//	상태 이상 딜래이. ( 1.0f == 100% ) (+)
	float				m_fSTATE_DAMAGE;				//	상태 이상 대미지 증가.

	SQITEM_FACT			m_sQITEMFACT;					//	? 아이템 적용값.
	SEVENT_FACT			m_sEVENTFACT;					//	이벤트 ? 아이템 적용값.
	//SEventState			m_sEventState;


	bool				m_bSTATE_PANT;					//	상태이상 헐떡임.
	
	
	float				m_fSKILL_MOVE;					//	스킬 이동속도.
	float				m_fOPTION_MOVE;					//	추가적인 이동속도. 월드에디터에서 사용.


	DEFENSE_SKILL		m_sDefenseSkill;				//  발동형 스킬
	SCHARSTATS			m_sSUMSTATS;					//	합산 Stats.

	WORD				m_wSUM_AP;						//	기본 공격력.
	WORD				m_wSUM_DP;						//	기본 방어력.

	WORD				m_wSUM_PA;						//	격투치.
	WORD				m_wSUM_SA;						//	사격치.
	WORD				m_wSUM_MA;						//	마법치.

	SRESIST				m_sSUMRESIST;					//	저항값. ( 아이템 + 패시브 )
	SRESIST				m_sSUMRESIST_SKILL;				//  스킬 저항값 ( 아이템 + 패시브 + 버프 ) 
	SSUM_ITEM			m_sSUMITEM;						//	아이템 가산값.
	
	WORD				m_wACCEPTP;						//	무기 아이템에 능력제한 허용 수치.
	WORD				m_wSUM_DisSP;					//	소모 SP 합산.

	int					m_nHIT;							//	명중율.
	int					m_nSUM_HIT;						//	명중율 + 스킬효과.
	int					m_nAVOID;						//	회피율.
	int					m_nSUM_AVOID;					//	회피율 + 스킬효과.

	int					m_nDEFENSE_BODY;				//	방어력 ( DP + DEX+계수 ).
	int					m_nDEFENSE;						//	방어력 ( DP + DEX*계수 + ITEM_DEF ) * 공간적응 보정.
	int					m_nDEFENSE_SKILL;				//	방어력 ( DP + DEX*계수 + ITEM_DEF ) * 공간적응 보정 + 스킬.

	GLPADATA			m_gdDAMAGE;						//	기본 공격력. ( AP + 페시브 스킬로 가산된 공업 )
	GLPADATA			m_gdDAMAGE_SKILL;				//	기본 공격력 + temp 스킬효과.
	GLPADATA			m_gdDAMAGE_PHYSIC;				//	기본 공격력 + 무기공업 + (격투치or사격치).

	float				m_fDamageRate;					//	공격력 변화율.
	float				m_fDefenseRate;					//	방어력 변화율.

	DAMAGE_SPEC			m_sDamageSpec;					//	데미지 흡수 및 반사

	float				m_fINCR_HP;						//	HP 회복율.
	float				m_fINCR_MP;						//	MP 회복율.
	float				m_fINCR_SP;						//	SP 회복율.

	float				m_fATTVELO;						//	스킬 공격속도 변화.
	WORD				m_wATTRANGE;					//	공격 가능 거리.
	WORD				m_wSUM_ATTRANGE;				//	공격 가능 거리.
	GLITEM_ATT			m_emITEM_ATT;					//	아이탬 공격 속성.

	float				m_fSKILLDELAY;

	float				m_fEXP_RATE;					//	경험치 수신율.
	float				m_fGEN_RATE;					//	아이템 발생율.
	float				m_fGOLD_RATE;					/*gold drop buff, Juver, 2018/07/01 */

	EMANI_SUBTYPE		m_emANISUBTYPE;					//	Item 착용에 따른 에니메이션 종류.

	SNATIVEID			m_idACTIVESKILL;				//	현제 액티브된 스킬.
	EMANI_MAINTYPE		m_emANIMAINSKILL;				//	스킬에 따른 에니메이션.
	EMANI_SUBTYPE		m_emANISUBSKILL;				//	스킬에 따른 에니메이션.

	SITEM*				m_pITEMS[SLOT_TSIZE];

	BOOL				m_bUseArmSub;					// 극강부를 위한 보조 무기 사용여부

	//	Note : 회복되는 수치 산출용 임시 변수.
	float				m_fIncHP;
	float				m_fIncMP;
	float				m_fIncSP;

	MAPPLAYHOSTILE		m_mapPlayHostile;				//	적대 행위자. ( 쌍방 )

//	탈것 추가
	BOOL				m_bVehicle;						//  탈것 활성화 여부
	float				m_fVehicleSpeedRate;
	float				m_fVehicleSpeedVol;
	
	bool				m_bSafeZone;

	/*prohibit potion skill logic, Juver, 2017/06/06 */
	bool				m_bProhibitPotion; 

	/*prohibit skill logic, Juver, 2017/06/06 */
	bool				m_bProhibitSkill; 

	/*continuous damage skill logic, Juver, 2017/06/10 */
	CONTINUOUS_DAMAGE_DATA_MAP	m_mapContinuousDamage;

	/*curse skill logic, Juver, 2017/06/10 */
	bool				m_bCurse;
	float				m_fCurseDamage;

	/*vehicle booster system, Juver, 2017/08/11 */
	bool				m_bBoosterCharge;
	bool				m_bBoosterStart;
	float				m_fBoosterTimer;

	/*summon time, Juver, 2017/12/12 */
	int					m_nSummonTime; 

	/*ignore damage, Juver, 2017/12/12 */
	bool				m_bIgnoreDamage; 

	/*block back damage, Juver, 2017/12/12 */
	float				m_fBlockBackDamageTime;
	bool				m_bBlockBackDamage;

	/*skill vehicle off, Juver, 2018/09/07 */
	bool				m_skill_vehicle_off;

	/*skill pet off, Juver, 2018/09/07 */
	bool				m_skill_pet_off;

	/*skill transform, Juver, 2018/09/08 */
	SKILL_TRANSFORM_DATA m_skill_transform_data;

	float				m_fINCR_HP_TOTAL;	//	HP 회복율.
	float				m_fINCR_MP_TOTAL;	//	MP 회복율.
	float				m_fINCR_SP_TOTAL;	//	SP 회복율.

	/* skill turn/find invisible, Juver, 2020/04/26 */
	bool				m_bSkillTurnInvisible;
	WORD				m_wSkillTurnInvisibleLevel;
	WORD				m_wSkillTurnInvisibleAnimation;
	bool				m_bSkillTurnInvisibleDisableInCombat;
	SNATIVEID			m_sSkillTurnInvisibleID;
	bool				m_bSkillShowInvisible;
	WORD				m_wSkillShowInvisibleLevel;
	WORD				m_wSkillShowInvisibleRange;

	/* costume combine map restriction, Juver, 2020/09/12 */
	bool				m_bCostumeCombineRestrictMoveSpeed;
	bool				m_bCostumeCombineRestrictAttackSpeed;

	/* skill amplify, Juver, 2020/12/10 */
	SSKILL_AMPLIFY		m_sSkillAmplifyMelee;
	SSKILL_AMPLIFY		m_sSkillAmplifyMissile;
	SSKILL_AMPLIFY		m_sSkillAmplifyMagic;

	/* skill position shift, Juver, 2020/12/15 */
	bool				m_bSkillPositionShift;

	/* skill hostile, Juver, 2020/12/16 */
	bool				m_bSkillHostile;

	/* skill buff duration change, Juver, 2020/12/17 */
	SKILL_DURATION_CHANGE		m_sSkillDurationChange;

	/* non vend area, Juver, 2021/01/17 */
	bool				m_bNonVendArea;

	/* post system, Juver, 2021/02/05 */
	EMPOSTBOXID_TYPE	m_emPOSTBOXIDTYPE;				//  사용중인 우편함 ID 타입
	SNATIVEID			m_sPOSTBOXID;					//  사용중인 우편함 ID

	//stun
	bool				m_bSTATE_STUN_ORIG;
	CRITICAL_SECTION	m_STATE_STUN_Lock;
	cMtSafeVar			m_cSTATE_STUN_CHECK;

	//pierce
	int					m_nSUM_PIERCE_ORIG;					//	관통 정도 합산.
	CRITICAL_SECTION	m_SUM_PIERCE_Lock;
	cMtSafeVar			m_cSUM_PIERCE_CHECK;

	//target range
	float				m_fSUM_TARGET_RANGE_ORIG;			//	공격 가능거리 합산.
	CRITICAL_SECTION	m_SUM_TARGET_RANGE_Lock;
	cMtSafeVar			m_cSUM_TARGET_RANGE_CHECK;

	/*skill range spec logic, Juver, 2017/06/06 */
	float				m_fSUM_ATTACK_RANGE_ORIG;
	CRITICAL_SECTION	m_SUM_ATTACK_RANGE_Lock;
	cMtSafeVar			m_cSUM_ATTACK_RANGE_CHECK;

	/*skill range spec logic, Juver, 2017/06/06 */
	float				m_fSUM_APPLY_RANGE_ORIG;
	CRITICAL_SECTION	m_SUM_APPLY_RANGE_Lock;
	cMtSafeVar			m_cSUM_APPLY_RANGE_CHECK;


	GLCHARLOGIC_CLIENT () :
		m_CHARINDEX(GLCI_BRAWLER_M),

		m_dwHOLDBLOW(NULL),
		m_bSTATEBLOW(FALSE),
		m_fSTATE_MOVE(1.0f),
		m_fSTATE_DELAY(1.0f),
		m_fSTATE_DAMAGE(1.0f),
		
		m_bSTATE_PANT(false),
		
		
		m_fHP_RATE(0),
		m_fMP_RATE(0),
		m_fSP_RATE(0),

		m_fSKILL_MOVE(0.0f),
		m_fOPTION_MOVE(0.0f),

		m_wSUM_AP(0),
		m_wSUM_DP(0),
		m_wSUM_PA(0),
		m_wSUM_SA(0),
		m_wSUM_MA(0),

		m_nHIT(0),
		m_nSUM_HIT(0),
		m_nAVOID(0),
		m_nSUM_AVOID(0),

		m_wACCEPTP(0),
		m_wSUM_DisSP(0),

		m_nDEFENSE(0),

		m_fDamageRate(1.0f),
		m_fDefenseRate(1.0f),

		m_fINCR_HP(0.0f),
		m_fINCR_MP(0.0f),
		m_fINCR_SP(0.0f),

		m_fIncHP(0.0f),
		m_fIncMP(0.0f),
		m_fIncSP(0.0f),

		m_fATTVELO(0.0f),
		m_wATTRANGE(4),
		m_wSUM_ATTRANGE(4),

		m_fSKILLDELAY(0.0f),

		m_fEXP_RATE(1),
		m_fGEN_RATE(1),
		m_fGOLD_RATE(1),	/*gold drop buff, Juver, 2018/07/01 */

		m_emITEM_ATT(ITEMATT_NOTHING),

		m_emANISUBTYPE(AN_SUB_NONE),

		m_idACTIVESKILL(NATIVEID_NULL()),

		m_emANIMAINSKILL(AN_NONE),
		m_emANISUBSKILL(AN_SUB_NONE),

		m_bUseArmSub(FALSE),
		m_bVehicle(FALSE),
		m_fVehicleSpeedRate( 0.0f ),
		m_fVehicleSpeedVol( 0.0f ),
		m_bSafeZone(false),

		m_bProhibitPotion(false), /*prohibit potion skill logic, Juver, 2017/06/06 */
		m_bProhibitSkill(false), /*prohibit skill logic, Juver, 2017/06/06 */

		/*curse skill logic, Juver, 2017/06/10 */
		m_bCurse(false),
		m_fCurseDamage(0.0f),

		/*vehicle booster system, Juver, 2017/08/11 */
		m_bBoosterCharge(false),
		m_bBoosterStart(false),
		m_fBoosterTimer(0.0f),

		m_nSummonTime(0), /*summon time, Juver, 2017/12/12 */
		m_bIgnoreDamage(false), /*ignore damage, Juver, 2017/12/12 */

		/*block back damage, Juver, 2017/12/12 */
		m_fBlockBackDamageTime(0.0f),
		m_bBlockBackDamage(false),

		/*skill vehicle off, Juver, 2018/09/07 */
		m_skill_vehicle_off(false),

		/*skill pet off, Juver, 2018/09/07 */
		m_skill_pet_off(false),

		m_fINCR_HP_TOTAL(0.0f),
		m_fINCR_MP_TOTAL(0.0f),
		m_fINCR_SP_TOTAL(0.0f),

		/* skill turn/find invisible, Juver, 2020/04/26 */
		m_bSkillTurnInvisible(false),
		m_wSkillTurnInvisibleLevel(0),
		m_wSkillTurnInvisibleAnimation(SKILL::EMSPEC_INVISIBLE_ANI_TYPE_NONE),
		m_bSkillTurnInvisibleDisableInCombat(false),
		m_sSkillTurnInvisibleID(false),
		m_bSkillShowInvisible(false),
		m_wSkillShowInvisibleLevel(0),
		m_wSkillShowInvisibleRange(0),

		/* costume combine map restriction, Juver, 2020/09/12 */
		m_bCostumeCombineRestrictMoveSpeed(false),
		m_bCostumeCombineRestrictAttackSpeed(false),

		/* skill position shift, Juver, 2020/12/15 */
		m_bSkillPositionShift(false),

		/* skill hostile, Juver, 2020/12/16 */
		m_bSkillHostile(false),

		/* non vend area, Juver, 2021/01/17 */
		m_bNonVendArea(false),

		/* post system, Juver, 2021/02/05 */
		m_sPOSTBOXID(SNATIVEID(false)),
		m_emPOSTBOXIDTYPE(EMPOSTBOXID_TYPE_NONE)
	{
		memset ( m_pITEMS, 0x00, sizeof(SITEM*)*SLOT_TSIZE );
		
		/*skill transform, Juver, 2018/09/08 */
		m_skill_transform_data.Reset();

		//stun
		m_bSTATE_STUN_ORIG = false;
		m_cSTATE_STUN_CHECK.Set( (int)m_bSTATE_STUN_ORIG );
		InitializeCriticalSection(&m_STATE_STUN_Lock);

		//pierce
		m_nSUM_PIERCE_ORIG = 0;
		m_cSUM_PIERCE_CHECK.Set( (int)m_nSUM_PIERCE_ORIG );
		InitializeCriticalSection(&m_SUM_PIERCE_Lock);

		//target range
		m_fSUM_TARGET_RANGE_ORIG = 0.0f;
		m_cSUM_TARGET_RANGE_CHECK.Set( (float)m_fSUM_TARGET_RANGE_ORIG );
		InitializeCriticalSection(&m_SUM_TARGET_RANGE_Lock);

		//attack range
		/*skill range spec logic, Juver, 2017/06/06 */
		m_fSUM_ATTACK_RANGE_ORIG = 0.0f;
		m_cSUM_ATTACK_RANGE_CHECK.Set( (float)m_fSUM_ATTACK_RANGE_ORIG );
		InitializeCriticalSection(&m_SUM_ATTACK_RANGE_Lock);

		//apply range
		m_fSUM_APPLY_RANGE_ORIG = 0.0f;
		m_cSUM_APPLY_RANGE_CHECK.Set( (float)m_fSUM_APPLY_RANGE_ORIG );
		InitializeCriticalSection(&m_SUM_APPLY_RANGE_Lock);

	}

	~GLCHARLOGIC_CLIENT ()
	{
		DEL_PLAYHOSTILE_ALL();

		DeleteCriticalSection(&m_STATE_STUN_Lock);
		DeleteCriticalSection(&m_SUM_PIERCE_Lock);
		DeleteCriticalSection(&m_SUM_TARGET_RANGE_Lock);
		DeleteCriticalSection(&m_SUM_ATTACK_RANGE_Lock);
		DeleteCriticalSection(&m_SUM_APPLY_RANGE_Lock);
	}
public:
	float GetQuestionTime()			{ return m_sQITEMFACT.fTime; }

public:
	EMCROW GETCROW () const			{ return CROW_PC; }
	WORD GETLEVEL () const			{ return m_wLevel; }
	
	DWORD GETHP () const			{ return m_sHP.wNow; }
	DWORD GETMAXHP () const			{ return m_sHP.wMax; }

	int GETMP () const				{ return m_sMP.wNow; }
	int GETMAXMP () const			{ return m_sMP.wMax; }

	int GETSP () const				{ return m_sSP.wNow; }
	int GETMAXSP () const			{ return m_sSP.wMax; }

	/*combatpoint logic, Juver, 2017/05/28 */
	int GETCP () const				{ return m_sCombatPoint.wNow; }
	int GETMAXCP () const			{ return m_sCombatPoint.wMax; }

	int GETHIT () const				{ return m_nSUM_HIT; }
	int GETAVOID () const			{ return m_nSUM_AVOID; }
	EMBRIGHT GETBRIGHT () const		{ if(m_nBright<0) return BRIGHT_DARK; return BRIGHT_LIGHT; }
	int GETBRIGHTPER () const		{ return m_nBright; }
	int GETFORCE_LOW () const		{ return m_gdDAMAGE_PHYSIC.wLow; }
	int GETFORCE_HIGH () const		{ return m_gdDAMAGE_PHYSIC.wHigh; }
	int GETDEFENSE () const			{ return m_nDEFENSE_SKILL; }

	/* Codex, Archie 02/16/24 */
	int GETHPINC() const { return m_dwHPIncrease; }
	int GETMPINC() const { return m_dwMPIncrease; }
	int GETSPINC() const { return m_dwSPIncrease; }
	int GETATTINC() const { return m_dwAttackIncrease; }
	int GETDEFINC() const { return m_dwDefenseIncrease; }
	int GETSHOOTINC() const { return m_dwShootingIncrease; }
	int GETMELEEINC() const { return m_dwMeleeIncrease; }
	int GETENERGYINC() const { return m_dwEnergyIncrease; }
	int GETRESISTINC() const { return m_dwResistanceIncrease; }
	int GETHITRATEINC() const { return m_dwHitrateIncrease; }
	int GETAVOIDINC() const { return m_dwAvoidrateIncrease; }
	
	float GETSUM_TARRANGE ();
	int GETSUM_PIERCE ();

	WORD GETBODYRADIUS () const		{ return GLCONST_CHAR::wBODYRADIUS; }
	WORD GETATTACKRANGE () const	{ return m_wSUM_ATTRANGE; }	

	const SCHARSTATS& GETSTATS_ADD () const;
	const SCHARSTATS& GETSTATS_ITEM () const;

	void OptionMoveUp();
	void OptionMoveDown();

	void SetUseArmSub( BOOL bSub )	{ m_bUseArmSub = bSub; }
	BOOL IsUseArmSub() const		{ return m_bUseArmSub; }

	EMSLOT GetCurRHand();
	EMSLOT GetCurLHand();

	BOOL IsCurUseArm( EMSLOT emSlot );

	bool IsDefenseSkill()				{ return m_sDefenseSkill.m_bActive; }
	void SetDefenseSkill(bool bDefense)	{ m_sDefenseSkill.m_bActive = bDefense; }

public:
	void SLOT_ITEM ( const SITEMCUSTOM &sItemCustom, EMSLOT _slot );
	void RELEASE_SLOT_ITEM ( EMSLOT _slot );

	const SITEMCUSTOM& GET_SLOT_ITEM ( EMSLOT _slot )				{ return m_PutOnItems[_slot]; }
	const SNATIVEID& GET_SLOT_NID ( EMSLOT _slot )					{ return m_PutOnItems[_slot].sNativeID; }
	SITEM* GET_SLOT_ITEMDATA ( EMSLOT _slot )						{ return m_pITEMS[_slot]; }
	BOOL VALID_SLOT_ITEM ( EMSLOT _slot );

	void HOLD_ITEM ( const SITEMCUSTOM &sItemCustom );
	void RELEASE_HOLD_ITEM ();
	const SITEMCUSTOM& GET_HOLD_ITEM ()					{ return m_PutOnItems[SLOT_HOLD]; }
	BOOL VALID_HOLD_ITEM ()								{ return GET_HOLD_ITEM().sNativeID!=NATIVEID_NULL(); }

	BOOL ISLONGRANGE_ARMS ();
	BOOL ISLONGRANGE_ARMS ( GLITEM_ATT emAttack );

	BOOL ISHAVEITEM ( SNATIVEID &sNID, DWORD *pNum=NULL );
	BOOL ISREVIVE ();

	SITEM* GET_ELMT_ITEM ();	//	왼손, 오른손 아이템중에서 'BLOW' 속성이 있는 아이템을 반환.

	WORD GETSKILLRANGE_TAR ( const GLSKILL &sSKILL );
	WORD GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL );

public:
	EMBEGINATTACK_FB BEGIN_ATTACK ( WORD wStrikeNum );
	BOOL VALID_LEVELUP () const;
	LONGLONG GET_LEVELUP_EXP () const;
	void LEVLEUP ( bool bInitNowExp = TRUE );
	void STATSUP ( EMSTATS emStats );

	/*modern character window, Juver, 2018/03/17 */
	void STATSUP_MODERN ( SCHARSTATS sStats );

	//MMR Rank
	void ShowRankName (BOOL bShowName);
	void ShowRankMark (BOOL bShowMark);

	void CODEX_STATS( int nIndex, DWORD dwStatPoint ); /* Codex, Archie 02/16/24 */

	bool RESET_STATS_SKILL ( const WORD wDIS_STAT=USHRT_MAX );
	bool RESET_SKILL ();
	bool RESET_STATS ( const WORD wDIS=USHRT_MAX );

	/* Reset Stats Item */
	bool STATS_RESET();

protected:
	void SUM_ITEM ();
	void SUM_PASSIVE ();
	void SUM_ADDITION ( float fCONFT_POINT_RATE );

public:
	void INIT_NEW_CHAR ( const EMCHARINDEX _emCIndex, const DWORD dwUserID, const DWORD dwServerID, const char* szCharName,
		const WORD _wSchool=0, const WORD wHair=0, const WORD wFace=0, const WORD _wHairColor=0, const WORD _wSex=0, const float _fScaleRange=1.0f );

	BOOL INIT_DATA ( BOOL bNEW, BOOL bReGen, float fCONFT_POINT_RATE = 1.0f, bool bInitNowExp = TRUE );

	void INIT_RECOVER ( int nRECOVER=30 );

	void RESET_DATA ();

public:
	void UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient=FALSE, float fCONFT_POINT_RATE=1.0f );
	void UPDATE_MAX_POINT ( float fCONFT_POINT_RATE );
	bool UPDATE_QITEMFACT ( float fElapsedTime );

public:
	BOOL CHECKSLOT_ITEM ( SNATIVEID sNativeID, EMSLOT emSlot );
	BOOL ACCEPT_ITEM ( SNATIVEID sNativeID );
	BOOL ACCEPT_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID );
	BOOL ISEMPTY_SLOT ( SNATIVEID sNativeID, EMSLOT emSlot );
	WORD CALC_ACCEPTP ( SNATIVEID sNativeID );
	BOOL SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID );
	BOOL SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID );

	EMCHECK_WEAR CHECK_WEAR_ITEM_SLOT( SNATIVEID sItemID, EMSLOT emPutOnSlot );

public:
	WORD RECEIVE_DAMAGE ( const WORD wDamage );

public:
	/*qbox check, Juver, 2017/12/05 */
	BOOL RECEIVE_QITEMFACT ( const SNATIVEID &nidITEM, BOOL bMobGen );
	//	Memo :	이벤트로 인해 받는 ?아이템 효과를 설정한다.
	void RECEIVE_EVENTFACT_BEGIN ( const EMGM_EVENT_TYPE emType, const WORD wValue );
	//	Memo :	이벤트로 인해 받는 ?아이템 효과를 해제한다.
	void RECEIVE_EVENTFACT_END ( const EMGM_EVENT_TYPE emType );

public:
	BOOL ISLEARNED_SKILL ( SNATIVEID skill_id, WORD wLEVEL=0 );
	BOOL ISMASTER_SKILL ( SNATIVEID skill_id );
	SCHARSKILL* GETLEARNED_SKILL ( SNATIVEID skill_id );

	EMSKILL_LEARNCHECK CHECKLEARNABLE_SKILL ( SNATIVEID skill_id );
	void LEARN_SKILL ( SNATIVEID skill_id, SNATIVEID item_id,__time64_t _tBornTime );
	VOID LVLUP_SKILL ( SNATIVEID skill_id, WORD wToLevel );

	bool LEARN_SKILL_QUEST ( SNATIVEID skill_id );

public:
	float GETATTVELO ();
	float GETMOVEVELO ();
	float GETATT_ITEM (); // 절대치 증가
	float GETMOVE_ITEM (); // 절대치 증가 % 
	float GETEXP_TOTAL ();

	//const 
	float GET_ATT_VELO() const;
	float GET_MOVE_VELO() const;
	float GET_EXP_TOTAL() const;
	float GET_HP_INCR() const;
	float GET_MP_INCR() const;
	float GET_SP_INCR() const;

public:
	//	Note : 적대자 등록. ( pk 관련 ),	true : 신규 등록시, false : 기존 등록 갱신시.
	bool ADD_PLAYHOSTILE ( DWORD dwCHARID, BOOL bBAD );
	bool DEL_PLAYHOSTILE ( DWORD dwCHARID );
	void DEL_PLAYHOSTILE_ALL ();

public:
	//	Note : 적대 리스트에 등록 되어 있는가?
	bool IS_PLAYHOSTILE ( DWORD dwCHARID );
	//	Note : 적대행위자?
	bool IS_HOSTILE_ACTOR ( DWORD dwCHARID );
	//	Note : 자신이 범죄자인가?
	bool ISOFFENDER ();

	bool ISPLAYKILLING ();

public:
	//	Note : 현제 pk 레벨을 알아본다. ( UINT_MAX 일경우 pk 해당사항 없음. )
	DWORD GET_PK_LEVEL ();

	//	Note : 죽을때 경험치 감소 pk 배수.
	float GET_PK_DECEXP_RATE ();
	std::string GET_PK_NAME ();
	DWORD GET_PK_COLOR ();
	float GET_PK_SHOP2BUY ();
	float GET_PK_SHOP2SALE ();

	DWORD GET_PK_ITEMDROP_NUM ();
	float GET_PK_ITEMDROP_RATE ();

public:
	BOOL DOGRINDING(SITEMCUSTOM &sCusItem, 
					const SITEM *pHold, 
					bool &bRESET, 
					bool &bTERMINATE, 
					EMANTIDISAPPEAR &emANTIDISAPPEAR );

public:
	BOOL ISSTATEBLOW ()							{ return m_bSTATEBLOW; }
	void DISABLESKEFF ( int i )					
	{ 
		CONTINUOUSDAMAGE_DELETE(m_sSKILLFACT[i].sNATIVEID);	/*continuous damage skill logic, Juver, 2017/06/10 */		
		m_sSKILLFACT[i].RESET(); 
	}
	void DISABLEBLOW ( int i )					{ m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE; }
	void RESETAGESKEFF ( int i )				{ m_sSKILLFACT[i].fAGE = 0.0f; }
	void RESETAGEBLOW ( int i )					{ m_sSTATEBLOWS[i].fAGE = 0.0f; }

	void DISABLEALLLANDEFF();
	void ADDLANDEFF( SLANDEFFECT landEffect, int iNum );


	void UPDATESKILLDELAY ( float fElapsedTime );

	void SETACTIVESKILL ( SNATIVEID skill_id );
	SNATIVEID GETACTIVESKILL ()					{ return m_idACTIVESKILL; }

	/*extreme auto tab, Juver, 2018/09/10 */
	EMSKILLCHECK CHECHSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bNotLearn, EMSLOT emRHand, EMSLOT emLHand );

	void ACCOUNTSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bServer=false );
	
	/*continuous damage skill logic, Juver, 2017/06/10 */
	BOOL CONTINUOUSDAMAGE_EXIST( SNATIVEID _sID );
	void CONTINUOUSDAMAGE_INSERT( SCONTINUOUS_DAMAGE_DATA _sDATA );
	void CONTINUOUSDAMAGE_DELETE( SNATIVEID _sID );

	/*npc talk buff condition, Juver, 2017/10/03 */
	BOOL ISHAVE_BUFF( SNATIVEID skill_id, WORD wLEVEL=1 );

	/*activity system, Juver, 2017/10/30 */
	SACTIVITY_CHAR_DATA*	GetActivityProg( DWORD dwActivityID );
	SACTIVITY_CHAR_DATA*	GetActivityDone( DWORD dwActivityID );
	DWORD	GetActivityProgNum();
	DWORD	GetActivityDoneNum();

	/* Codex, Archie 02/16/24 */
	SCODEX_CHAR_DATA*	GetCodexProg( DWORD dwCodexID );
	SCODEX_CHAR_DATA*	GetCodexDone( DWORD dwCodexID );
	DWORD	GetCodexProgNum();
	DWORD	GetCodexDoneNum();
	
		/*12-9-14, Battle Pass - CNDev*/
	SBATTLEPASS_CHAR_DATA*	GetBattlePassProg( DWORD dwBattlePassID );
	SBATTLEPASS_CHAR_DATA*	GetBattlePassDone( DWORD dwBattlePassID );
	DWORD	GetBattlePassProgNum();
	DWORD	GetBattlePassDoneNum();

	/*official random value, Juver, 2018/06/12 */
	WORD	get_additional_gain_hp() const;
	WORD	get_additional_gain_mp() const;
	WORD	get_additional_gain_sp() const;
	WORD	get_additional_gain_cp() const;

	/*additional RV, Juver, 2018/06/14 */
	float get_critical_hit_chance() const;
	float get_critical_hit_damage() const;
	float get_crushing_blow_chance() const;
	float get_crushing_blow_damage() const;

public:
	virtual void ReSelectAnimation ()	{}


public:
	void StateStunSet( bool bSet );
	void StateStunSet2( bool bSet );
	bool StateStunGet();

	void SumPierceSet( int nVal );
	void SumPierceSet2( int nVal );
	int SumPierceGet();

	void SumTargetRangeSet( float fVal );
	void SumTargetRangeSet2( float fVal );
	float SumTargetRangeGet();

	void SumAttackRangeSet( float fVal );
	void SumAttackRangeSet2( float fVal );
	float SumAttackRangeGet();

	void SumApplyRangeSet( float fVal );
	void SumApplyRangeSet2( float fVal );
	float SumApplyRangeGet();

	/* set item option, Juver, 2021/09/09 */
	bool SetOptionIsSuit( EMSUIT emSuit );
};


#endif // GLLOGICCHARCLIENT_H_INCLUDED__
