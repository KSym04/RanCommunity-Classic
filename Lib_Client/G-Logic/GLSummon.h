#pragma once

#include "./GLSummonDefine.h"
#include "./GLItem.h"
#include "./GLCharData.h"
#include "./GLCrowData.h"

#include "../../Lib_Engine/G-Logic/GLDefine.h"

/*skill transform, Juver, 2018/09/08 */
#include "./SkillTransformData.h"

struct GLSUMMON
{
	SUMMON_TYPE		m_emTYPE;
	float			m_fRunArea; 
	float			m_fWalkArea; 
	float			m_fOwnerDistance;

	GLSUMMON ()
		: m_emTYPE(SUMMON_TYPE_NONE)
		, m_fRunArea(60.0f)
		, m_fWalkArea(40.0f)
		, m_fOwnerDistance(20.0f)
	{
	}
	GLSUMMON ( const GLSUMMON& value ) { operator=( value ); }
	GLSUMMON& operator= ( const GLSUMMON& rvalue );

	BOOL LoadFile ( const char* szFileName, SUMMON_TYPE emType );
};

typedef GLSUMMON* PGLSUMMON;


struct SUMMON_DATA_SKILL
{
	BOOL		bSummon;
	STARID		sTarget;		//target

	SNATIVEID	sidSkill;		//caster skill
	SNATIVEID	sidCrow;		//summon crow
	float		fDelay;			//time to summon
	float		fLife;			//summon lifetime
	WORD		wSkillLevel;	//summon skill level
	WORD		wMaxSummon;		//maximum summon for this skill

	SUMMON_DATA_SKILL() 
		: bSummon(FALSE)		
		, sidSkill(NATIVEID_NULL())
		, sidCrow(NATIVEID_NULL())
		, fDelay(0.0f)
		, fLife(1.0f)
		, wSkillLevel(0)
		, wMaxSummon(1)
	{
	}

	SUMMON_DATA_SKILL ( const SUMMON_DATA_SKILL &value )
	{
		operator=(value);
	}

	SUMMON_DATA_SKILL& operator = ( const SUMMON_DATA_SKILL& rvalue )
	{
		bSummon			= rvalue.bSummon;
		sTarget			= rvalue.sTarget;

		sidSkill		= rvalue.sidSkill;
		sidCrow			= rvalue.sidCrow;
		fDelay			= rvalue.fDelay;
		fLife			= rvalue.fLife;
		wSkillLevel		= rvalue.wSkillLevel;
		wMaxSummon		= rvalue.wMaxSummon;

		return *this;
	}

	void Assign ( SKILL::SSKILL_SUMMON_DATA &sdata );

	void Reset()
	{
		bSummon			= FALSE;
		sTarget			= STARID();

		sidSkill		= NATIVEID_NULL();
		sidCrow			= NATIVEID_NULL();
		fDelay			= 0.0f;
		fLife			= 1.0f;
		wSkillLevel		= 0;
		wMaxSummon		= 1;
	}
};


struct SDROPSUMMON
{
	DWORD			m_dwGUID;
	SNATIVEID		m_sSummonID;
	SUMMON_TYPE		m_emTYPE;
	DWORD			m_dwOwner;					// 주인 GaeaID
	SNATIVEID		m_sMapID;
	DWORD			m_dwCellID;
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vTarPos;
	DWORD			m_dwActionFlag;
	EMSUMMONACTIONTYPE m_emSUMMONACTYPE;
	WORD			m_wAniSub;					 // 애니메이션 관련
	DWORD			m_dwNowHP;						//	생명량.
	WORD			m_wNowMP;						//	정신량.

	/*skill summon, Juver, 2017/10/09 */
	WORD			m_wArrayIndex;
	SUMMON_DATA_SKILL	m_Summon;

	SDROPSUMMON () :
		m_dwGUID(UINT_MAX),
		m_sSummonID(NATIVEID_NULL()),
		m_dwOwner(0),
		m_sMapID(NATIVEID_NULL()),
		m_dwCellID(0),
		m_vPos (0,0,0),
		m_vDir (0,0,-1),
		m_vTarPos(0,0,0),
		m_dwActionFlag(0),
		m_emSUMMONACTYPE(SUMMONAT_IDLE),
		m_emTYPE(SUMMON_TYPE_NONE),
		m_wAniSub(1),
		m_dwNowHP(0),
		m_wNowMP(0),

		/*skill summon, Juver, 2017/10/09 */
		m_wArrayIndex(0)
	{
	}

	SDROPSUMMON ( const SDROPSUMMON& value ) { operator=( value ); }
	SDROPSUMMON& operator= ( const SDROPSUMMON& rvalue );

	void RESET ();
};

typedef SDROPSUMMON* PSDROPSUMMON;

