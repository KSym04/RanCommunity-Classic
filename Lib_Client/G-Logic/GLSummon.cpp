#include "stdafx.h"
#include "./GLItemMan.h"
#include "./GlItemLMT.h"
#include "./GLGaeaServer.h"
#include "./GLChar.h"

#include "../../Lib_Engine/Common/GLTexFile.h"
#include "../../Lib_Engine/G-Logic/Glogic.h"

#include "./GLSummon.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLSUMMON& GLSUMMON::operator= ( const GLSUMMON& rvalue )
{
	m_emTYPE			= rvalue.m_emTYPE;
	m_fRunArea			= rvalue.m_fRunArea;
	m_fWalkArea			= rvalue.m_fWalkArea;
	m_fOwnerDistance	= rvalue.m_fOwnerDistance;

	return *this;
}

BOOL GLSUMMON::LoadFile ( const char* szFileName, SUMMON_TYPE emType )
{
	if ( !szFileName )				  return FALSE;
	if ( strlen ( szFileName ) == 0 ) return FALSE;

	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += szFileName;	

	gltexfile cFILE;
	cFILE.reg_sep( '\t' );
	cFILE.reg_sep( ' ' );
	cFILE.reg_sep( ',' );
	cFILE.reg_sep( '{' );
	cFILE.reg_sep( '}' );
	cFILE.reg_sep( '[' );
	cFILE.reg_sep( ']' );
	cFILE.reg_sep( '(' );
	cFILE.reg_sep( ')' );
	cFILE.reg_sep( '|' );

	if( GLOGIC::bGLOGIC_ZIPFILE ) // by 경대
		cFILE.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if( !cFILE.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ToLogFile( "ERROR : GLSUMMON::LoadData (), %s", szFileName );
		return FALSE;
	}

	WORD dwVERSION = 0;
	cFILE.getflag( "VERSION", 1, 1, dwVERSION );
	cFILE.getflag( "emTYPE",	 1, 1, m_emTYPE );


	cFILE.getflag( "fRunArea", 1, 1, m_fRunArea ); // 달려야만 하는 거리
	cFILE.getflag( "fWalkArea", 1, 1, m_fWalkArea ); // 걸어야만 하는 거리
	cFILE.getflag( "fOwnerDistance", 1, 1, m_fOwnerDistance ); // 주인과의 유지 거리

	GLCONST_SUMMON::pGLSUMMON[emType] = this;



	DWORD dwMaxSummon = 0;
	cFILE.getflag( "dwMaxSummon", 1, 1, dwMaxSummon );

	GLCONST_SUMMON::m_dwMaxSummon[emType] = dwMaxSummon;

	return TRUE;
}

void SUMMON_DATA_SKILL::Assign ( SKILL::SSKILL_SUMMON_DATA &sdata )
{
	sidCrow			= sdata.sidCrow;
	fDelay			= sdata.fDelay;
	fLife			= sdata.fLife;
	wSkillLevel		= sdata.wSkillLevel;
	wMaxSummon		= (WORD)sdata.dwMaxNum;
}

SDROPSUMMON& SDROPSUMMON::operator= ( const SDROPSUMMON& rvalue )
{
	m_dwGUID		 = rvalue.m_dwGUID;
	m_sSummonID		 = rvalue.m_sSummonID;
	m_dwOwner		 = rvalue.m_dwOwner;
	m_sMapID		 = rvalue.m_sMapID;
	m_dwCellID		 = rvalue.m_dwCellID;
	m_emTYPE		 = rvalue.m_emTYPE;
	m_vPos		 	 = rvalue.m_vPos;
	m_vDir			 = rvalue.m_vDir;
	m_vTarPos		 = rvalue.m_vTarPos;
	m_dwActionFlag	 = rvalue.m_dwActionFlag;
	m_emSUMMONACTYPE = rvalue.m_emSUMMONACTYPE;
	m_wAniSub		 = rvalue.m_wAniSub;
	m_dwNowHP		 = rvalue.m_dwNowHP;
	m_wNowMP		 = rvalue.m_wNowMP;

	/*skill summon, Juver, 2017/10/09 */
	m_wArrayIndex    = rvalue.m_wArrayIndex;
	m_Summon         = rvalue.m_Summon;

	return *this;
}

void SDROPSUMMON::RESET ()
{
	m_dwGUID		 = UINT_MAX;
	m_sSummonID		 = NATIVEID_NULL();
	m_dwOwner		 = 0;
	m_sMapID		 = NATIVEID_NULL();
	m_dwCellID		 = 0;
	m_emTYPE		 = SUMMON_TYPE_NONE;
	m_vPos			 = D3DXVECTOR3(0,0,0);
	m_vDir			 = D3DXVECTOR3(0,0,-1);
	m_vTarPos		 = D3DXVECTOR3(0,0,0);
	m_dwActionFlag	 = 0;
	m_emSUMMONACTYPE = SUMMONAT_IDLE;
	m_wAniSub		 = 1;
	m_dwNowHP		 = 0;
	m_wNowMP		 = 0;

	/*skill summon, Juver, 2017/10/09 */
	m_wArrayIndex = 0;
	m_Summon.Reset();
}