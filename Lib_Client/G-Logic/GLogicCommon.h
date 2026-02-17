/*!
 * \file GLLogicCommon.h
 *
 * \author Juver
 * \date 2020/02/13
 *
 * 
 */

#ifndef GLLOGICCOMMON_H_INCLUDED__
#define GLLOGICCOMMON_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <hash_map>

#include "./GLCharData.h"
#include "./GLCharDefine.h"

typedef stdext::hash_map<DWORD,float>	DELAY_MAP;
typedef DELAY_MAP::iterator				DELAY_MAP_ITER;

class STARDIST
{
public:
	float		m_fDIST;
	STARGETID	m_sTARGETID;

public:
	STARDIST () {}

	STARDIST ( const float fDIST, const STARGETID &sTARID )
	{
		m_fDIST = fDIST;
		m_sTARGETID = sTARID;
	}

	bool operator < ( const STARDIST &rvalue )
	{
		return m_fDIST < rvalue.m_fDIST;
	}
};

typedef std::vector<STARDIST>			TARGETMAP;
typedef TARGETMAP::iterator				TARGETMAP_ITER;

typedef std::vector<STARGETID>						DETECTMAP;
typedef DETECTMAP::iterator							DETECTMAP_ITER;
typedef std::pair<DETECTMAP_ITER,DETECTMAP_ITER>	DETECTMAP_RANGE;

class STAR_ORDER
{
public:
	bool operator() ( const STARGETID &lvalue, const STARGETID &rvalue )
	{
		return lvalue.emCrow < rvalue.emCrow;
	}

	bool operator() ( const STARGETID &lvalue, const EMCROW emCROW )
	{
		return lvalue.emCrow < emCROW;
	}

	bool operator() ( const EMCROW emCROW, const STARGETID &rvalue )
	{
		return emCROW < rvalue.emCrow;
	}
};

//	적대자.
struct SPLAYHOSTILE
{
	BOOL	bBAD;
	float	fTIME;

	SPLAYHOSTILE ()
	{
		bBAD     = FALSE;
		fTIME    = 0;
	}
};

typedef stdext::hash_map<DWORD,SPLAYHOSTILE*>	MAPPLAYHOSTILE;
typedef MAPPLAYHOSTILE::iterator				MAPPLAYHOSTILE_ITER;

struct SSUM_ITEM
{
	GLPADATA	gdDamage;
	int			nDefense;

	int			nAvoidRate;
	int			nHitRate;

	int			nHP;
	int			nMP;
	int			nSP;

	float		fIncR_HP;
	float		fIncR_MP;
	float		fIncR_SP;

	float		fInc_HP;
	float		fInc_MP;
	float		fInc_SP;

	SCHARSTATS	sStats;
	SRESIST		sResist;

	int			nPA;
	int			nSA;
	int			nMA;

	float		fIncR_MoveSpeed;
	float		fIncR_AtkSpeed;
	float		fIncR_Critical;
	float		fIncR_CrushingBlow;

	float		fInc_MoveSpeed;
	float		fInc_AtkSpeed;
	float		fInc_Critical;
	float		fInc_CrushingBlow;

	float		fExpRate;

	/*official random value, Juver, 2018/06/12 */
	int			nadd_gain_hp;
	int			nadd_gain_mp;
	int			nadd_gain_sp;
	int			nadd_gain_cp;

	/*additional RV, Juver, 2018/06/15 */
	float		fadd_movement_speed;
	float		fadd_attack_speed;

	float		fadd_critical_hit_chance;
	float		fadd_critical_hit_damage;
	float		fadd_crushing_blow_chance;
	float		fadd_crushing_blow_damage;

	/*item max CP, Juver, 2018/07/09 */
	WORD		wadd_max_cp;

	SSUM_ITEM () :
	nDefense(0),

		nAvoidRate(0),
		nHitRate(0),
		nHP(0),
		nMP(0),
		nSP(0),
		fIncR_HP(0.0f),
		fIncR_MP(0.0f),
		fIncR_SP(0.0f),

		fInc_HP(0.0f),
		fInc_MP(0.0f),
		fInc_SP(0.0f),

		nPA(0),
		nSA(0),
		nMA(0),
		fIncR_MoveSpeed(0.0f),
		fIncR_AtkSpeed(0.0f),
		fIncR_Critical(0.0f),
		fIncR_CrushingBlow(0.0f),
		fInc_MoveSpeed(0.0f),
		fInc_AtkSpeed(0.0f),
		fInc_Critical(0.0f),
		fInc_CrushingBlow(0.0f),

		fExpRate(0.0f),

		/*official random value, Juver, 2018/06/12 */
		nadd_gain_hp(0),
		nadd_gain_mp(0),
		nadd_gain_sp(0),
		nadd_gain_cp(0),

		/*additional RV, Juver, 2018/06/15 */
		fadd_movement_speed(0.0f),
		fadd_attack_speed(0.0f),

		fadd_critical_hit_chance(0.0f),
		fadd_critical_hit_damage(0.0f),
		fadd_crushing_blow_chance(0.0f),
		fadd_crushing_blow_damage(0.0f),

		/*item max CP, Juver, 2018/07/09 */
		wadd_max_cp(0)
	{
	}

	void RESET ()
	{
		*this = SSUM_ITEM();
	}
};

namespace GLOGICEX
{
	EM_BRIGHT_FB GLSPACEGAP ( const EMBRIGHT emACTOR, const EMBRIGHT emRECEP, const EMBRIGHT emSPACE );

	//	공격 성공율 계산.
	int GLHITRATE ( const int nHit, const int nAvoid, const EM_BRIGHT_FB bFB );

	//	완전 쇼크 확율.
	BOOL CHECKSHOCK ( const int AttackerLev, const int DefenserLev, const int nDamage, const bool bCritical );

	//	방어력 계산.
	int GLDEFENSE ( const int nDEFENSE, const EMBRIGHT emACTOR, const EMBRIGHT emRecep, const EMBRIGHT emSPACE );

	//	경험치 획득 ( 타격시 )
	int GLATTACKEXP ( int AttackerLev, int DefenserLev, DWORD dwDamage, DWORD wMaxHP, DWORD dwBonusExp=0 );

	//	경험치 획득 ( 제거시 )
	int	GLKILLEXP ( int AttackerLev, int DefenserLev, DWORD dwBonusExp=0 );

	//	경험치 획득 ( 타격시 )
	int CALCATTACKEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan, DWORD dwDamage );

	//	경험치 획득 ( 제거시 )
	int CALCKILLEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan );

	//	죽었을때 경험치 감소.
	float GLDIE_DECEXP ( WORD wACTLEV );

	//	경험치 복구 비율
	float GLDIE_RECOVERYEXP ( WORD wACTLEV );

	//  경험치1당 돈
	float GLDIE_EXPMONEY ( WORD wACTLEV );

	//	LevelUp을 하기 위한 경험치
	LONGLONG GLNEEDEXP ( WORD wLev );
	LONGLONG GLNEEDEXP2 ( WORD wLev );

	//	상태이상 발생할 수 있는 확율 산출.
	BOOL CHECKSTATEBLOW ( float fACTRATE, WORD wACTLEVEL, WORD wLEVEL, WORD wRESIST );

	//	스킬 딜레이 산출.
	float SKILLDELAY ( DWORD dwSKILL_GRADE, WORD wSKILL_LEV, WORD wCHAR_LEVEL, float fDelay );

	//	Note : 파라메타 변경.
	//		계산시에는 int 형으로 계산을 수행하여 연산시 손실을 막는다.
	//		값 적용시에는 WORD 형 값의 영역 이내로 제한.
	inline WORD VARIATION ( WORD &wNow, const WORD wMax, const int nValue )
	{
		int nOld = int(wNow);

		int nNew = nOld + nValue;

		//	WORD 형 값의 영역 이내로 제한.
		if ( nNew < 0 )					nNew = 0;
		if ( nNew > USHRT_MAX )			nNew = USHRT_MAX;

		wNow = nNew;
		if ( wNow >= wMax )				wNow = wMax;

		WORD wDX = (WORD) abs(nOld-int(wNow));
		return wDX;
	}

	//	Note : 파라메타 변경.
	//		계산시에는 int 형으로 계산을 수행하여 연산시 손실을 막는다.
	//		값 적용시에는 WORD 형 값의 영역 이내로 제한.
	inline DWORD VARIATION ( DWORD &dwNow, const DWORD dwMax, const int nValue )
	{
		int nOld = int(dwNow);

		int nNew = nOld + nValue;

		//	WORD 형 값의 영역 이내로 제한.
		if ( nNew < 0 )					nNew = 0;

		dwNow = nNew;
		if ( dwNow >= dwMax )			dwNow = dwMax;

		DWORD dwDX = (DWORD) abs(nOld-int(dwNow));
		return dwDX;
	}

	//	날씨와 속성간 증폭율.
	float WEATHER_ELEMENT_POW ( EMELEMENT emElement, DWORD dwWeather, BOOL bWeatherActive );

	float WEATHER_BLOW_POW ( EMSTATE_BLOW emBlow, DWORD dwWeather, BOOL bWeatherActive );

	//	Note : 체력 같은 수치들을 변화율에 따라 갱신해준다.
	//		계산시에는 int 형으로 계산을 수행하여 연산시 손실을 막는다.
	//		값 적용시에는 WORD 형 값의 영역 이내로 제한.
	inline void UPDATE_POINT ( GLPADATA &sPOINT, float &fELP_VAR, const float fVAR, const WORD wLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(sPOINT.wNow) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD 형 값의 영역 이내로 제한.
		if ( nNEWP < 0 )			nNEWP = 0;
		if ( nNEWP > USHRT_MAX )	nNEWP = USHRT_MAX;

		if ( nNEWP < wLOW_LMT )		nNEWP = wLOW_LMT;

		sPOINT.wNow = nNEWP;
		sPOINT.LIMIT ();
	}

	//	Note : 체력 같은 수치들을 변화율에 따라 갱신해준다.
	//		계산시에는 int 형으로 계산을 수행하여 연산시 손실을 막는다.
	//		값 적용시에는 WORD 형 값의 영역 이내로 제한.
	inline void UPDATE_POINT ( DWORD &dwNowP, float &fELP_VAR, const DWORD MaxP, const float fVAR, const DWORD dwLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(dwNowP) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD 형 값의 영역 이내로 제한.
		if ( nNEWP < 0 )				nNEWP = 0;
		if ( nNEWP < (int)dwLOW_LMT )	nNEWP = dwLOW_LMT;

		dwNowP = nNEWP;
		if ( dwNowP > MaxP )		dwNowP = MaxP;
	}

	inline void UPDATE_POINT ( WORD &wNowP, float &fELP_VAR, const WORD MaxP, const float fVAR, const WORD wLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(wNowP) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD 형 값의 영역 이내로 제한.
		if ( nNEWP < 0 )			nNEWP = 0;
		if ( nNEWP > USHRT_MAX )	nNEWP = USHRT_MAX;

		if ( nNEWP < wLOW_LMT )		nNEWP = wLOW_LMT;

		wNowP = nNEWP;
		if ( wNowP > MaxP )			wNowP = MaxP;
	}
};

enum EMBEGINATTACK_FB
{
	EMBEGINA_OK		= 0,
	EMBEGINA_SP		= 1,
	EMBEGINA_ARROW	= 2,
	EMBEGINA_BULLET	= 3, /*gun-bullet logic, Juver, 2017/05/27 */
};


EMANI_SUBTYPE CHECK_ANISUB ( SITEM* pRHAND, SITEM* pLHAND, bool transform_skin );
EMANI_SUBTYPE CHECK_ATTACK_ANISUB ( SITEM* pRHAND, SITEM* pLHAND, bool transform_skin );

//! 아이템 획득 권한을 주기위한...
//! 데미지 로그를 기록하기 위한 구조체
struct SDAMAGELOG
{
	DWORD dwUserID; ///< User ID
	DWORD dwGaeaID; ///< 가이아 ID
	DWORD dwDamage; ///< 준 데미지	

	SDAMAGELOG () 
		: dwUserID(0)
		, dwGaeaID(0)
		, dwDamage(0)
	{
	}

	SDAMAGELOG ( DWORD dwuserid, DWORD dwgaeaid, DWORD dwdamage ) 
		: dwUserID(dwuserid)
		, dwGaeaID(dwgaeaid)
		, dwDamage(dwdamage)
	{
	}
};

typedef stdext::hash_map<DWORD, SDAMAGELOG>	DAMAGELOG;
typedef DAMAGELOG::iterator					DAMAGELOG_ITER;

inline void AddDamageLog ( DAMAGELOG &cDamageLog, DWORD dwGaeaID, DWORD dwUserID, DWORD dwDamage )
{
	DAMAGELOG_ITER iter = cDamageLog.find ( dwUserID );
	DAMAGELOG_ITER iter_end = cDamageLog.end();

	if ( iter != iter_end )
	{
		SDAMAGELOG &sDamageLog = (*iter).second;
		if ( sDamageLog.dwUserID == dwUserID )	dwDamage += sDamageLog.dwDamage;
	}

	cDamageLog[dwUserID] = SDAMAGELOG(dwUserID,dwGaeaID,dwDamage);
}

//! 추가:2006-04-12 Jgkim
//! 파티 데미지 로그기록을 위한 구조체
struct SDAMAGELOGPARTY
{
	DWORD m_dwPartyID; ///< 파티 고유번호
	DWORD m_dwDamage; ///< 파티가 준 데미지

	SDAMAGELOGPARTY()
		: m_dwPartyID(0)
		, m_dwDamage(0)
	{
	}

	SDAMAGELOGPARTY(DWORD dwPartyID, DWORD dwDamage)
		: m_dwPartyID(dwPartyID)
		, m_dwDamage(dwDamage)
	{
	}
};
typedef stdext::hash_map<DWORD, SDAMAGELOGPARTY>	DAMAGELOGPARTY;
typedef DAMAGELOGPARTY::iterator					DAMAGELOGPARTY_ITER;

inline void AddDamageLogParty(DAMAGELOGPARTY &cDamageLogParty, DWORD dwPartyID, DWORD dwDamage)
{
	if (dwPartyID == PARTY_NULL) return;

	DAMAGELOGPARTY_ITER iter     = cDamageLogParty.find(dwPartyID);
	DAMAGELOGPARTY_ITER iter_end = cDamageLogParty.end();

	if (iter != iter_end)
	{
		SDAMAGELOGPARTY &sDamageLogParty = (*iter).second;
		if (sDamageLogParty.m_dwPartyID == dwPartyID)
		{
			dwDamage += sDamageLogParty.m_dwDamage;
		}
	}	
	cDamageLogParty[dwPartyID] = SDAMAGELOGPARTY(dwPartyID, dwDamage);
}

inline bool CHECHSKILL_ITEM ( GLSKILL_ATT emSKILL, GLITEM_ATT emITEM, bool bHiddenWeapon )
{
	if ( emITEM == ITEMATT_EXTREME_GLOVE && bHiddenWeapon )
	{
		return true;
	}

	switch ( emSKILL )
	{
	case SKILLATT_NOTHING:			
		return ( emITEM == ITEMATT_NOTHING );
	case SKILLATT_SWORD:			
		return ( emITEM == ITEMATT_SWORD );
	case SKILLATT_BLADE:			
		return ( emITEM == ITEMATT_BLADE );
	case SKILLATT_SWORDBLADE:		
		return ( emITEM == ITEMATT_SWORD || emITEM == ITEMATT_BLADE );
	case SKILLATT_DAGGER:			
		return ( emITEM == ITEMATT_DAGGER );
	case SKILLATT_SPEAR:			
		return ( emITEM == ITEMATT_SPEAR );
	case SKILLATT_STICK:			
		return ( emITEM == ITEMATT_STICK );
	case SKILLATT_GAUNT:			
		return ( emITEM == ITEMATT_GAUNT );
	case SKILLATT_BOW:				
		return ( emITEM == ITEMATT_BOW );
	case SKILLATT_THROW:			
		return ( emITEM == ITEMATT_THROW );

		/*gun-bullet logic, Juver, 2017/05/27 */
	case SKILLATT_GUN:			
		return ( emITEM == ITEMATT_GUN);
	case SKILLATT_RAILGUN:			
		return ( emITEM == ITEMATT_RAILGUN);
	case SKILLATT_PORTALGUN:			
		return ( emITEM == ITEMATT_PORTALGUN);
	case SKILLATT_SHOTGUN:			
		return ( emITEM == ITEMATT_GUN || emITEM == ITEMATT_RAILGUN || emITEM == ITEMATT_PORTALGUN );
	case SKILLATT_RAILPORTALGUN:			
		return ( emITEM == ITEMATT_RAILGUN || emITEM == ITEMATT_PORTALGUN );

	case SKILLATT_SCYTHE:			
		return ( emITEM == ITEMATT_SCYTHE );

	case SKILLATT_DUALSPEAR:			
		return ( emITEM == ITEMATT_DUALSPEAR );

	case SKILLATT_SHURIKEN:			
		return ( emITEM == ITEMATT_SHURIKEN );

	case SKILLATT_EXTREME_FIST:	
		return ( emITEM == ITEMATT_EXTREME_GLOVE );

	case SKILLATT_NOCARE:			
		return ( emITEM == ITEMATT_NOCARE );
	};

	return	false;
}

/*vehicle system, Juver, 2017/08/07 */
EMANI_MAINTYPE	CHECK_VEHICLE_MAIN( SNATIVEID sVehicleID );
EMANI_SUBTYPE   CHECK_VEHICLE_SUB( EMANI_MAINTYPE emCharMainAni, bool bPassenger = FALSE );


D3DXVECTOR3 UpdateSkillDirection ( const D3DXVECTOR3 &vMyPos, const D3DXVECTOR3 vMyDir, const SNATIVEID sSKILLID, const D3DXVECTOR3 vTARPOS, const STARID *pTARID );




#endif // GLLOGICCOMMON_H_INCLUDED__
