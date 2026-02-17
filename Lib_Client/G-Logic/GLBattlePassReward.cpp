#include "stdafx.h"

#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/IniLoader.h"

#include "./GLBattlePassReward.h"

/*12-9-14, Battle Pass - CNDev*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLBattlePassReward::GLBattlePassReward()
{

}

GLBattlePassReward::~GLBattlePassReward()
{

}

GLBattlePassReward& GLBattlePassReward::GetInstance ()
{
	static GLBattlePassReward Instance;
	return Instance;
}

void GLBattlePassReward::CleanUp()
{
	m_mapBattlePassLevelRewardMap.clear();
}

void GLBattlePassReward::LoadFile( std::string strFileName )
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
		CDebugSet::ToLogFile ( "ERROR : GLBattlePassReward::LoadFile(), File Open %s", strPath.c_str() );
		return;
	}

	CleanUp();

	DWORD dwNUM = cFILE.GetKeySize( "BATTLEPASS_LEVEL_REWARD", "LEVEL_REWARD" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SBATTLEPASS_LEVEL_REWARD sReward;
		cFILE.getflag( i, "BATTLEPASS_LEVEL_REWARD", "LEVEL_REWARD", 0, 5, sReward.dwLevel );
		cFILE.getflag( i, "BATTLEPASS_LEVEL_REWARD", "LEVEL_REWARD", 1, 5, sReward.sNormalRewardID.wMainID );
		cFILE.getflag( i, "BATTLEPASS_LEVEL_REWARD", "LEVEL_REWARD", 2, 5, sReward.sNormalRewardID.wSubID );
		cFILE.getflag( i, "BATTLEPASS_LEVEL_REWARD", "LEVEL_REWARD", 3, 5, sReward.sPremiumRewardID.wMainID );
		cFILE.getflag( i, "BATTLEPASS_LEVEL_REWARD", "LEVEL_REWARD", 4, 5, sReward.sPremiumRewardID.wSubID );

		m_mapBattlePassLevelRewardMap.insert( std::make_pair( sReward.dwLevel, sReward ) );
	}
}