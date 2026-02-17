#include "stdafx.h"

#include "../../Lib_Engine/Common/gltexfile.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#include "./GLogicData.h"

#include "./GLReborn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLReborn::GLReborn(void)
	: dwMaxReborn(0)
{
}

GLReborn::~GLReborn(void)
{
}

GLReborn& GLReborn::GetInstance ()
{
	static GLReborn _instance;
	return _instance;
}

BOOL GLReborn::LOAD ( std::string strFILE )
{
	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += strFILE;

	gltexfile cFILE;
	cFILE.reg_sep ( '\t' );
	cFILE.reg_sep ( ' ' );
	cFILE.reg_sep ( ',' );
	cFILE.reg_sep ( '{' );
	cFILE.reg_sep ( '}' );
	cFILE.reg_sep ( '[' );
	cFILE.reg_sep ( ']' );
	cFILE.reg_sep ( '(' );
	cFILE.reg_sep ( ')' );
	cFILE.reg_sep ( '|' );
	cFILE.reg_sep ( '\"' );

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if( !cFILE.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLReborn::LOAD, File Open %s", strFILE.c_str());
		return FALSE;
	}

	Reset();

	cFILE.getflag( "dwMaxReborn", 1, 1, dwMaxReborn );

	cFILE.getflag( "Reborn_Max", 1, 11, sRebornMax.wLevel );	
	cFILE.getflag( "Reborn_Max", 2, 11, sRebornMax.dwPrice );	
	cFILE.getflag( "Reborn_Max", 3, 11, sRebornMax.wRemainStats );
	cFILE.getflag( "Reborn_Max", 4, 11, sRebornMax.sStatsReplace.wPow );
	cFILE.getflag( "Reborn_Max", 5, 11, sRebornMax.sStatsReplace.wStr );
	cFILE.getflag( "Reborn_Max", 6, 11, sRebornMax.sStatsReplace.wSpi );
	cFILE.getflag( "Reborn_Max", 7, 11, sRebornMax.sStatsReplace.wDex );
	cFILE.getflag( "Reborn_Max", 8, 11, sRebornMax.sStatsReplace.wInt );
	cFILE.getflag( "Reborn_Max", 9, 11, sRebornMax.sStatsReplace.wSta );
	cFILE.getflag( "Reborn_Max", 10, 11, sRebornMax.wLevelAfter );
	cFILE.getflag( "Reborn_Max", 11, 11, sRebornMax.wRebornIncrease );

	for ( int i=0; i<MAX_ARRAY; ++i )
	{
		CString strTemp;
		strTemp.Format( "Reborn_%d", i+1 );
		cFILE.getflag( strTemp.GetString(), 1, 11, sReborn[i].wLevel );	
		cFILE.getflag( strTemp.GetString(), 2, 11, sReborn[i].dwPrice );	
		cFILE.getflag( strTemp.GetString(), 3, 11, sReborn[i].wRemainStats );
		cFILE.getflag( strTemp.GetString(), 4, 11, sReborn[i].sStatsReplace.wPow );
		cFILE.getflag( strTemp.GetString(), 5, 11, sReborn[i].sStatsReplace.wStr );
		cFILE.getflag( strTemp.GetString(), 6, 11, sReborn[i].sStatsReplace.wSpi );
		cFILE.getflag( strTemp.GetString(), 7, 11, sReborn[i].sStatsReplace.wDex );
		cFILE.getflag( strTemp.GetString(), 8, 11, sReborn[i].sStatsReplace.wInt );
		cFILE.getflag( strTemp.GetString(), 9, 11, sReborn[i].sStatsReplace.wSta );
		cFILE.getflag( strTemp.GetString(), 10, 11, sReborn[i].wLevelAfter );
		cFILE.getflag( strTemp.GetString(), 11, 11, sReborn[i].wRebornIncrease );
	}

	return TRUE;
};

void GLReborn::Reset()
{
	for ( int i=0; i<MAX_ARRAY; ++i )
	{
		sReborn[i].Reset();
	}

	sRebornMax.Reset();
	dwMaxReborn = 0;
}

SREBORN* GLReborn::GetRebornData( int nReborn )
{
	if ( nReborn >= MAX_ARRAY )	return NULL;
	return &sReborn[nReborn];
}