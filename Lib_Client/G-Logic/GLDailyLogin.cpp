#include "stdafx.h"

#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/IniLoader.h"

#include "./GLDailyLogin.h"

/*12-16-24, daily login - CNDev*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLDailyLogin::GLDailyLogin()
{

}

GLDailyLogin::~GLDailyLogin()
{

}

GLDailyLogin& GLDailyLogin::GetInstance ()
{
	static GLDailyLogin Instance;
	return Instance;
}

void GLDailyLogin::CleanUp()
{
	m_mapDailyLoginReward.clear();
}

void GLDailyLogin::LoadFile( std::string strFileName )
{
	if ( strFileName.empty() )
		return;

	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += strFileName;

	CIniLoader cFILE;
	std::string strSep( ",[]\t" );
	cFILE.reg_sep( strSep );

	if( !cFILE.open( strPath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLDailyLoginReward::LoadFile(), File Open %s", strPath.c_str() );
		return;
	}

	CleanUp();

	DWORD dwNUM = cFILE.GetKeySize( "DAILYLOGIN_REWARD", "DAY_REWARD" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SDAILYLOGIN_REWARD sReward;
		cFILE.getflag( i, "DAILYLOGIN_REWARD", "DAY_REWARD", 0, 3, sReward.dwDay );
		cFILE.getflag( i, "DAILYLOGIN_REWARD", "DAY_REWARD", 1, 3, sReward.sRewardID.wMainID );
		cFILE.getflag( i, "DAILYLOGIN_REWARD", "DAY_REWARD", 2, 3, sReward.sRewardID.wSubID );

		m_mapDailyLoginReward.insert( std::make_pair( sReward.dwDay, sReward ) );
	}
}