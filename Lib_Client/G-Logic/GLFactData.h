/*!
 * \file GLFactData.h
 *
 * \author Juver
 * \date May 2017
 *
 * 
 */

#ifndef GLFACTDATA_H_
#define GLFACTDATA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLSkill.h"

#define SKILL_DELAY_MULTI_UPDATE_MAX_ARRAY 40

struct SSKILLFACT_IMPACTS
{
	EMIMPACT_ADDON	emADDON;		//	부가 종류.
	float			fADDON_VAR;		//	부가 수치.

	SSKILLFACT_IMPACTS()
		: emADDON(EMIMPACTA_NONE)
		, fADDON_VAR(0)
	{
	}
};

struct SSKILLFACT_SPECS
{
	EMSPEC_ADDON	emSPEC;
	float			fSPECVAR1;
	float			fSPECVAR2;
	DWORD			dwSPECFLAG;
	SNATIVEID		dwNativeID;

	SSKILLFACT_SPECS()
		: emSPEC(EMSPECA_NULL)
		, fSPECVAR1(0)
		, fSPECVAR2(0)
		, dwSPECFLAG(NULL)
		, dwNativeID( NATIVEID_NULL() )
	{
	}
};

struct SSKILLFACT
{
	SNATIVEID		sNATIVEID;		//	스킬 ID.
	WORD			wLEVEL;			//	스킬 레벨.
	float			fAGE;			//	생성후 남은 시간.

	SKILL::EMTYPES	emTYPE;			//	기본 종류.
	float			fMVAR;			//	기본 수치.

	DWORD			dwSpecialSkill;		  // 특수스킬
	bool			bRanderSpecialEffect; // 특수 스킬 사용시 이펙트가 발동되었는지 여부

	SSKILLFACT_IMPACTS	sImpacts[SKILL::MAX_IMPACT];
	SSKILLFACT_SPECS	sSpecs[SKILL::MAX_SPEC];

	WORD			_wCasterCrow;
	DWORD			_dwCasterID;

	SSKILLFACT () :
		sNATIVEID(NATIVEID_NULL()),
		wLEVEL(0),
		fAGE(0),
		emTYPE(SKILL::EMFOR_VARHP),
		fMVAR(0),
		dwSpecialSkill(0),
		bRanderSpecialEffect(FALSE),
		_wCasterCrow(0),
		_dwCasterID(NATIVEID_NULL().dwID)
	{
	}

	BOOL IsSpecialSkill ( DWORD dwState )		 		{ return dwSpecialSkill == dwState ? TRUE : FALSE ; }
	void SetSpecialSkill ( DWORD dwState )				{ dwSpecialSkill = dwState; }


	void RESET ();
};

struct SDROP_SKILLFACT
{
	SNATIVEID		sNATIVEID;		//	스킬 ID.
	WORD			wSLOT;			//	슬롯 위치.
	WORD			wLEVEL;			//	스킬 LEVEL.
	float			fAGE;			//	생성후 남은 시간.

	WORD			_wCasterCrow;
	DWORD			_dwCasterID;

	SDROP_SKILLFACT () 
		: sNATIVEID(NATIVEID_NULL())
		, wSLOT(0)
		, wLEVEL(0)
		, fAGE(0)
		, _wCasterCrow(0)
		, _dwCasterID(NATIVEID_NULL().dwID)
	{
	}

	void Assign ( SSKILLFACT &sfact, WORD _wSLOT )
	{
		wSLOT = _wSLOT;
		sNATIVEID = sfact.sNATIVEID;
		wLEVEL = sfact.wLEVEL;
		fAGE = sfact.fAGE;

		_wCasterCrow = sfact._wCasterCrow;
		_dwCasterID = sfact._dwCasterID;
	}
};

struct SQITEM_FACT
{
	EMITEM_QUESTION	emType;
	float			fTime;
	WORD			wParam1;
	WORD			wParam2;
	BOOL			bMobGen; /*qbox check, Juver, 2017/12/05 */

	SQITEM_FACT () :
	emType(QUESTION_NONE),
		fTime(0),
		wParam1(0),
		wParam2(0),
		bMobGen(FALSE) /*qbox check, Juver, 2017/12/05 */
	{
	}

	bool IsACTIVE ()
	{
		return emType!=QUESTION_NONE;
	}

	void RESET ()
	{
		emType = QUESTION_NONE;
		fTime = 0;
		wParam1 = 0;
		wParam2 = 0;
		bMobGen = FALSE; /*qbox check, Juver, 2017/12/05 */
	};
};


/*itemfood system, Juver, 2017/05/24 */
struct SFITEMFACT
{
	enum { VERSION = 0x0001 };

	BOOL			bENABLE;
	SNATIVEID		sNATIVEID;
	WORD			wLEVEL;
	float			fAGE;

	SKILL::EMTYPES	emTYPE;
	float			fMVAR;

	SSKILLFACT_IMPACTS	sImpacts[SKILL::MAX_IMPACT];
	SSKILLFACT_SPECS	sSpecs[SKILL::MAX_SPEC];

	SFITEMFACT () :
		bENABLE(FALSE),
		sNATIVEID(NATIVEID_NULL()),
		wLEVEL(0),
		fAGE(0),
		emTYPE(SKILL::EMFOR_VARHP),
		fMVAR(0)
	{
	}

	void RESET ()
	{
		sNATIVEID  = NATIVEID_NULL();
		wLEVEL     = 0;
		fAGE       = 0.0f;
		emTYPE	   = SKILL::EMFOR_VARHP;
		fMVAR	   = 0.0f;

		for( int i=0; i<SKILL::MAX_IMPACT; ++i )
		{
			sImpacts[i].emADDON    = EMIMPACTA_NONE;
			sImpacts[i].fADDON_VAR = 0.0f;
		}

		for( int i=0; i<SKILL::MAX_SPEC; ++i )
		{
			sSpecs[i].emSPEC	 = EMSPECA_NULL;
			sSpecs[i].fSPECVAR1  = 0;
			sSpecs[i].fSPECVAR2  = 0;
			sSpecs[i].dwSPECFLAG = NULL;
			sSpecs[i].dwNativeID = NATIVEID_NULL();
		}
	}
};

/*itemfood system, Juver, 2017/05/24 */
struct SDROP_FITEMFACT
{
	SNATIVEID		sNATIVEID;
	WORD			wSLOT;
	WORD			wLEVEL;	
	float			fAGE;

	SDROP_FITEMFACT () 
		: sNATIVEID(NATIVEID_NULL())
		, wSLOT(0)
		, wLEVEL(0)
		, fAGE(0)
	{
	}

	void Assign ( SFITEMFACT &sfact, WORD _wSLOT )
	{
		wSLOT = _wSLOT;
		sNATIVEID = sfact.sNATIVEID;
		wLEVEL = sfact.wLEVEL;
		fAGE = sfact.fAGE;
	}
};

/*continuous damage skill logic, Juver, 2017/06/10 */
struct SCONTINUOUS_DAMAGE_DATA
{
	SNATIVEID	sidSkill;
	float		fAge;
	float		fInterval;
	float		fVar;
	WORD		wCrow;
	DWORD		dwCrowID;

	SCONTINUOUS_DAMAGE_DATA()
		: sidSkill(NATIVEID_NULL())
		, fAge(0.0f)
		, fInterval(0.0f)
		, fVar(0.0f)
		, wCrow(CROW_NUM)
		, dwCrowID(NATIVEID_NULL().dwID)
	{

	}

	BOOL Valid()
	{
		if ( sidSkill == NATIVEID_NULL() )	return FALSE;
		if ( fInterval == 0.0f )	return FALSE;
		if ( fVar == 0.0f )			return FALSE;
		if ( wCrow == CROW_NUM )	return FALSE;
		if ( dwCrowID == NATIVEID_NULL().dwID )	return FALSE;

		return TRUE;
	};
};

typedef std::map<DWORD,SCONTINUOUS_DAMAGE_DATA>	CONTINUOUS_DAMAGE_DATA_MAP;
typedef CONTINUOUS_DAMAGE_DATA_MAP::iterator	CONTINUOUS_DAMAGE_DATA_MAP_ITER;


/*system buffs, Juver, 2017/09/04 */
struct SSYSTEM_BUFF
{
	SNATIVEID		sNATIVEID;
	WORD			wLEVEL;

	SKILL::EMTYPES	emTYPE;
	float			fMVAR;

	SSKILLFACT_IMPACTS	sImpacts[SKILL::MAX_IMPACT];
	SSKILLFACT_SPECS	sSpecs[SKILL::MAX_SPEC];

	SSYSTEM_BUFF () :
	sNATIVEID(NATIVEID_NULL()),
		wLEVEL(0),
		emTYPE(SKILL::EMFOR_VARHP),
		fMVAR(0)
	{
	}

	void RESET ()
	{
		sNATIVEID  = NATIVEID_NULL();
		wLEVEL     = 0;
		emTYPE	   = SKILL::EMFOR_VARHP;
		fMVAR	   = 0.0f;

		for( int i=0; i<SKILL::MAX_IMPACT; ++i )
		{
			sImpacts[i].emADDON    = EMIMPACTA_NONE;
			sImpacts[i].fADDON_VAR = 0.0f;
		}

		for( int i=0; i<SKILL::MAX_SPEC; ++i )
		{
			sSpecs[i].emSPEC	 = EMSPECA_NULL;
			sSpecs[i].fSPECVAR1  = 0;
			sSpecs[i].fSPECVAR2  = 0;
			sSpecs[i].dwSPECFLAG = NULL;
			sSpecs[i].dwNativeID = NATIVEID_NULL();
		}
	}
};

/*system buffs, Juver, 2017/09/04 */
struct SDROP_SYSTEM_BUFF
{
	SNATIVEID		sNATIVEID;
	WORD			wSLOT;
	WORD			wLEVEL;	

	SDROP_SYSTEM_BUFF () 
		: sNATIVEID(NATIVEID_NULL())
		, wSLOT(0)
		, wLEVEL(0)
	{
	}

	void Assign ( SSYSTEM_BUFF &sfact, WORD _wSLOT )
	{
		wSLOT = _wSLOT;
		sNATIVEID = sfact.sNATIVEID;
		wLEVEL = sfact.wLEVEL;
	}
};

/* skill effect release, Juver, 2020/04/30 */
struct SEFFECT_RELEASE_DATA
{
	SNATIVEID	sidSkill;
	float		fRate;
	DWORD		dwCondition;		//actual usage unknown

	SEFFECT_RELEASE_DATA()
		: sidSkill(NATIVEID_NULL())
		, fRate(0.0f)
		, dwCondition(0)
	{

	}

	BOOL isValid() const
	{
		if ( sidSkill == NATIVEID_NULL() )	return FALSE;
		if ( fRate == 0.0f )				return FALSE;

		return TRUE;
	};
};

typedef std::map<DWORD,SEFFECT_RELEASE_DATA>	SEFFECT_RELEASE_DATA_MAP;
typedef SEFFECT_RELEASE_DATA_MAP::iterator		SEFFECT_RELEASE_DATA_MAP_ITER;


/* skill delay addition, Juver, 2020/12/09 */
struct SSKILL_DELAY_DATA
{
	SNATIVEID	sSkillID;
	float		fDelay;
	float		fChance;

	SSKILL_DELAY_DATA()
		: sSkillID(NATIVEID_NULL())
		, fDelay(0.0f)
		, fChance(0.0f)
	{

	}

	BOOL Valid() const
	{
		if ( sSkillID == NATIVEID_NULL() )	return FALSE;
		if ( fDelay == 0.0f )				return FALSE;
		if ( fChance == 0.0f )				return FALSE;

		return TRUE;
	};
};

typedef std::map<DWORD,SSKILL_DELAY_DATA>		SKILL_DELAY_DATA_MAP;
typedef SKILL_DELAY_DATA_MAP::iterator			SKILL_DELAY_DATA_MAP_ITER;

/* skill delay addition, Juver, 2020/12/09 */
struct SKILL_DELAY_ARRAY_DATA
{
	SNATIVEID sSkillID;
	float fAdjust;

	SKILL_DELAY_ARRAY_DATA()
		: sSkillID(false)
		, fAdjust(0.0f)
	{

	}
};

/* skill stigma, Juver, 2020/12/12 */
struct SKILL_STIGMA_DATA
{
	SNATIVEID sSkillID;
	float fDistance;
	float fDamage;
	DWORD dwAttackFlag;
	SNATIVEID sMapID;

	TARID_SET setTargetID;

	SKILL_STIGMA_DATA()
		: sSkillID(false)
		, fDistance(0.0f)
		, fDamage(0.0f)
		, dwAttackFlag(0)
	{

	}
	

	BOOL isValid()
	{
		if ( sSkillID == NATIVEID_NULL() )	return FALSE;
		if ( fDistance == 0.0f )			return FALSE;
		if ( fDamage == 0.0f )				return FALSE;
		if ( sMapID == NATIVEID_NULL() )	return FALSE;
		if ( dwAttackFlag == 0 )			return FALSE;

		return TRUE;
	}
};

typedef std::map<DWORD,SKILL_STIGMA_DATA>		SKILL_STIGMA_DATA_MAP;
typedef SKILL_STIGMA_DATA_MAP::iterator			SKILL_STIGMA_DATA_MAP_ITER;

/* skill buff duration change, Juver, 2020/12/17 */
struct SKILL_DURATION_CHANGE
{
	float fSecondsNormal;
	float fSecondsBuff;
	float fSecondsDebuff;
	float fSecondsLimit;

	SKILL_DURATION_CHANGE()
		: fSecondsNormal(0.0f)
		, fSecondsBuff(0.0f)
		, fSecondsDebuff(0.0f)
		, fSecondsLimit(0.0f)
	{

	}

	void Reset()
	{
		fSecondsNormal = 0.0f;
		fSecondsBuff = 0.0f;
		fSecondsDebuff = 0.0f;
		fSecondsLimit = 0.0f;
	}
};

/* skill illusion, Juver, 2021/01/17 */
struct SKILL_ILLUSION
{
	BOOL	bActive;
	SNATIVEID sSkillID;

	float	fIllusionJumpBound;
	float	fIllusionLastTime;
	WORD	wIllusionRemain;

	SKILL_ILLUSION()
		: bActive(FALSE)
		, sSkillID(false)
		, fIllusionJumpBound(0.0f)
		, fIllusionLastTime(0.0f)
		, wIllusionRemain(0)
	{

	}

	void Reset()
	{
		bActive = FALSE;
		sSkillID.ResetNativeID();
		fIllusionJumpBound = 0.0f;
		fIllusionLastTime = 0.0f;
		wIllusionRemain = 0;
	}
};

#endif // GLFACTDATA_H_