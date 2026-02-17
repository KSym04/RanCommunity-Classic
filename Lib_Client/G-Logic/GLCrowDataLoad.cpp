#include "stdafx.h"
#include "./GLCrowData.h"
#include "./GLItemMan.h"
#include "./GLogicData.h"

#include "../../Lib_Engine/Common/GLTexFile.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL SGENITEM::LOADFILE ( const char* szFileName )
{
	if( !szFileName )					return FALSE;
	if( strlen( szFileName ) == 0 )		return FALSE;

	std::string strPath;
	strPath = GLOGIC::GetServerPath();
	strPath += szFileName;	

	gltexfile glTextFile;
	glTextFile.reg_sep ( '\t' );
	glTextFile.reg_sep ( ' ' );
	glTextFile.reg_sep ( ',' );
	glTextFile.reg_sep ( '{' );
	glTextFile.reg_sep ( '}' );
	glTextFile.reg_sep ( '[' );
	glTextFile.reg_sep ( ']' );
	glTextFile.reg_sep ( '(' );
	glTextFile.reg_sep ( ')' );
	glTextFile.reg_sep ( '|' );

	if( GLOGIC::bGLOGIC_ZIPFILE ) // by 경대
		glTextFile.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if ( !glTextFile.open ( strPath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : SGENITEM::LOADFILE(), %s", szFileName );
		return FALSE;
	}
	
	WORD dwVERSION = 0;
	glTextFile.getflag ( "VERSION", 1, 1, dwVERSION );

	glTextFile.getflag ( "GENRATE", 1, 1, m_fGenRate );

	WORD wNum=0;
	float fRate=0, fRateL=0;

	DWORD dwNUM = glTextFile.getflagnum ( "GENNUM" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		glTextFile.getflag ( i, "GENNUM", 1, 2, wNum );
		glTextFile.getflag ( i, "GENNUM", 2, 2, fRate );

		m_sGenNum[i].wNum = wNum;
		m_sGenNum[i].fRateL = fRateL;
		m_sGenNum[i].fRateH = fRateL+fRate;
		fRateL = m_sGenNum[i].fRateH;
	}

	DWORD dwSpecID=0;
	fRate=0, fRateL=0;

	typedef std::multimap<float,SGITEMSPEC> MAP_ITEMGEN;
	typedef MAP_ITEMGEN::iterator			MAP_ITEMGEN_ITER;

	MAP_ITEMGEN mapGENITEMS;

	DWORD dwSpecNum = glTextFile.getflagnum ( "ITEMSPEC" );

	if ( dwSpecNum > SGENITEM::EMGSPEC )
	{
		CString strErr;
		strErr.Format ( "GenItem : %s Overflow.",
			szFileName );
		MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );
		return FALSE;
	}

	for ( DWORD i=0; i<dwSpecNum; ++i )
	{
		glTextFile.getflag ( i, "ITEMSPEC", 1, 2, dwSpecID );
		glTextFile.getflag ( i, "ITEMSPEC", 2, 2, fRate );

		if ( dwSpecID >= GLItemMan::SPECID_NUM )
		{
			CString strErr;
			strErr.Format ( "ITEMSPEC : %s Overflow. %d >= %d",
				szFileName, dwSpecID, GLItemMan::SPECID_NUM );
			MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );

			dwSpecID = 0;
		}

		//	Note : 
		SGITEMSPEC sSPEC;
		sSPEC.dwSPECID = dwSpecID;
		sSPEC.fRateL = fRate;
		sSPEC.fRateH = 0.0f;
		sSPEC.bSPECID = true;

		mapGENITEMS.insert ( std::make_pair(fRate,sSPEC) );
	}

	DWORD dwIDNUM = glTextFile.getflagnum ( "ITEMID" );
	
	if ( dwSpecNum+dwIDNUM > SGENITEM::EMGSPEC )
	{
		CString strErr;
		strErr.Format ( "GenItem : %s Overflow.",
			szFileName );
		MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );
		return FALSE;
	}


	for ( DWORD i=0; i<dwIDNUM; ++i )
	{
		SNATIVEID sNativeID;
		glTextFile.getflag ( i, "ITEMID", 1, 3, sNativeID.wMainID );
		glTextFile.getflag ( i, "ITEMID", 2, 3, sNativeID.wSubID );
		dwSpecID = sNativeID.dwID;

		glTextFile.getflag ( i, "ITEMID", 3, 3, fRate );

		if ( sNativeID == NATIVEID_NULL() )
		{
			CString strErr;
			strErr.Format ( "ITEMID : %s NativeID_NULL. %d",
				szFileName, dwNUM );
			MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );

			dwSpecID = 0;
		}

		//	Note : 
		SGITEMSPEC sSPEC;
		sSPEC.dwSPECID = dwSpecID;
		sSPEC.fRateL = fRate;
		sSPEC.fRateH = 0.0f;
		sSPEC.bSPECID = false;

		mapGENITEMS.insert ( std::make_pair(fRate,sSPEC) );
	}


	//	Note : 희귀성 순서대로 입력.
	//
	int i = 0;
	fRate=0, fRateL=0;
	MAP_ITEMGEN_ITER pos = mapGENITEMS.begin();
	MAP_ITEMGEN_ITER end = mapGENITEMS.end();
	for ( ; pos!=end; ++pos )
	{
		SGITEMSPEC sSPEC = (*pos).second;

		float fRate = sSPEC.fRateL;
		sSPEC.fRateL = fRateL;
		sSPEC.fRateH = fRateL + fRate;
		fRateL = sSPEC.fRateH;

		m_sItemSpec[i++] = sSPEC;
	}




	/*genitem skill spec, Juver, 2018/07/01 */
	typedef std::multimap<float,SGITEMSPEC_SKILL>	MAP_ITEMGEN_SKILL;
	typedef MAP_ITEMGEN_SKILL::iterator				MAP_ITEMGEN_SKILL_ITER;

	MAP_ITEMGEN_SKILL map_gen_item_skill;

	DWORD specnum_skill = glTextFile.getflagnum ( "ITEMSPECSKILLFACT" );
	if ( specnum_skill > SGENITEM::EMGSPEC )
	{
		CString strErr;
		strErr.Format ( "ITEMSPECSKILLFACT GenItem : %s Overflow.",szFileName );
		MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );
		return FALSE;
	}

	for ( DWORD i=0; i<specnum_skill; ++i )
	{
		SGITEMSPEC_SKILL sspec_skill;
		glTextFile.getflag ( i, "ITEMSPECSKILLFACT", 1, 4, sspec_skill.sidSkill.wMainID );
		glTextFile.getflag ( i, "ITEMSPECSKILLFACT", 2, 4, sspec_skill.sidSkill.wSubID );
		glTextFile.getflag ( i, "ITEMSPECSKILLFACT", 3, 4, sspec_skill.dwSPECID );
		glTextFile.getflag ( i, "ITEMSPECSKILLFACT", 4, 4, sspec_skill.fRateL );

		sspec_skill.fRateH = 0.0f;
		sspec_skill.bSPECID = true;

		if ( sspec_skill.dwSPECID >= GLItemMan::SPECID_NUM )
		{
			CString strErr;
			strErr.Format ( "ITEMSPECSKILLFACT : %s Overflow. %d >= %d",szFileName, sspec_skill.dwSPECID, GLItemMan::SPECID_NUM );
			MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );

			continue;
		}

		map_gen_item_skill.insert ( std::make_pair(sspec_skill.fRateL,sspec_skill) );
	}

	i = 0;
	fRate=0, fRateL=0;
	for ( MAP_ITEMGEN_SKILL_ITER it = map_gen_item_skill.begin(); it!=map_gen_item_skill.end(); ++it )
	{
		SGITEMSPEC_SKILL sspec_skill = (*it).second;

		float fRate = sspec_skill.fRateL;
		sspec_skill.fRateL = fRateL;
		sspec_skill.fRateH = fRateL + fRate;
		fRateL = sspec_skill.fRateH;

		m_sItemSpecSkill[i++] = sspec_skill;
	}


	
	return TRUE;
}

BOOL SQTGENITEM::LOADFILE ( const char* szFileName ) // by 경대
{
	if( !szFileName )					return FALSE;
	if( strlen( szFileName ) == 0 )		return FALSE;

	std::string strPath;
	strPath = GLOGIC::GetServerPath();
	strPath += szFileName;	

	gltexfile glTextFile;
	glTextFile.reg_sep ( '\t' );
	glTextFile.reg_sep ( ' ' );
	glTextFile.reg_sep ( ',' );
	glTextFile.reg_sep ( '{' );
	glTextFile.reg_sep ( '}' );
	glTextFile.reg_sep ( '[' );
	glTextFile.reg_sep ( ']' );
	glTextFile.reg_sep ( '(' );
	glTextFile.reg_sep ( ')' );
	glTextFile.reg_sep ( '|' );

	if( GLOGIC::bGLOGIC_ZIPFILE ) // by 경대
		glTextFile.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if ( !glTextFile.open ( strPath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : SQTGENITEM::LOADFILE(), %s", szFileName );
		return FALSE;
	}
	
	//WORD dwVERSION = 0;
	//glTextFile.getflag ( "VERSION", 1, 1, dwVERSION );
	
	float fRate(0);

	DWORD dwNUM = glTextFile.getflagnum ( "QTITEM" );
	if ( dwNUM > EMQTMAX )	dwNUM = EMQTMAX;

	for ( DWORD i=0; i<dwNUM; ++i )
	{
		glTextFile.getflag ( i, "QTITEM", 1, 3, m_sItem[i].m_sGenItemID.wMainID );
		glTextFile.getflag ( i, "QTITEM", 2, 3, m_sItem[i].m_sGenItemID.wSubID );
		glTextFile.getflag ( i, "QTITEM", 3, 3, m_sItem[i].m_fGenRate );
		fRate += m_sItem[i].m_fGenRate;
	}

	if( fRate > 100.0 )
	{
		CDebugSet::ToLogFile ( "ERROR : SQTGENITEM::LOADFILE(), %s", szFileName );
		return FALSE;
	}

	m_nItemNum = dwNUM; // 아이템 등록 개수 저장
	
	return TRUE;
}