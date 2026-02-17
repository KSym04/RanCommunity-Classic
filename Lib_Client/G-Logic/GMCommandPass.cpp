#include "stdafx.h"
#include "./GMCommandPass.h"

#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRPath.h"
#include "../../Lib_Engine/Core/NSRInfo.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGMCommandPass& CGMCommandPass::GetInstance ()
{
	static CGMCommandPass Instance;
	return Instance;
}

CGMCommandPass::CGMCommandPass()
{
	Reset();
}

CGMCommandPass::~CGMCommandPass()
{

}

void CGMCommandPass::Reset()
{
	strPassGetItem = "";
	strPassGetInven = "";
	strPassGetSkill = "";
	strPassGetLevel = "";
	strPassGetStats = "";
	strPassGetStatsP = "";
	strPassGetSkillP = "";
	strPassSendItem = "";
	strPassGetCrowTime = "";
	strPassEventContriTyranny = "";
	strPassEventContriSchoolWars = "";
	strPassEventContriCaptureTheFlag = "";
	strPassKickOut = "";
	strPassPCID = "";

	/* user flag restricted, Juver, 2020/04/21 */
	strPassUserRestrict = "";

	/* gm command inven clear, Juver, 2020/05/09 */
	strPassInvenClear = "";

	/* game notice, Juver, 2021/06/12 */
	strPassGameNoticeReload = "";

	strPassGISReload = "";
}

bool CGMCommandPass::LoadFile()
{
	std::string strFilePath;
	strFilePath = RPATH::getDataGLogicServerPath();
	strFilePath += RINFO::gets(EMRINFO_GM_COMMAND_PASS_FILE_NAME);

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );


	if( !cFILE.open ( strFilePath, true ) )
	{
		RLOG::Log( RINFO::getc(EMRINFO_GM_COMMAND_PASS_ERROR_FILE), strFilePath.c_str() );

		return false;
	}

	Reset();

	cFILE.getflag( "PASSWORD", "GET_ITEM", 0, 1, strPassGetItem );
	cFILE.getflag( "PASSWORD", "GET_INVEN", 0, 1, strPassGetInven );
	cFILE.getflag( "PASSWORD", "GET_SKILL", 0, 1, strPassGetSkill );
	cFILE.getflag( "PASSWORD", "GET_LEVEL", 0, 1, strPassGetLevel );
	cFILE.getflag( "PASSWORD", "GET_STATS", 0, 1, strPassGetStats );
	cFILE.getflag( "PASSWORD", "GET_STATS_P", 0, 1, strPassGetStatsP );
	cFILE.getflag( "PASSWORD", "GET_SKILL_P", 0, 1, strPassGetSkillP );
	cFILE.getflag( "PASSWORD", "SEND_ITEM", 0, 1, strPassSendItem );
	cFILE.getflag( "PASSWORD", "GET_CROW_TIME", 0, 1, strPassGetCrowTime );
	cFILE.getflag( "PASSWORD", "EVENT_CONTRI_TYRANNY", 0, 1, strPassEventContriTyranny );
	cFILE.getflag( "PASSWORD", "EVENT_CONTRI_SCOOLWARS", 0, 1, strPassEventContriSchoolWars );
	cFILE.getflag( "PASSWORD", "EVENT_CONTRI_CAPTURE_THE_FLAG", 0, 1, strPassEventContriCaptureTheFlag );
	cFILE.getflag( "PASSWORD", "KICK_OUT", 0, 1, strPassKickOut );
	cFILE.getflag( "PASSWORD", "PCID", 0, 1, strPassPCID );

	/* user flag restricted, Juver, 2020/04/21 */
	cFILE.getflag( "PASSWORD", "USER_RESTRICT", 0, 1, strPassUserRestrict );

	/* gm command inven clear, Juver, 2020/05/09 */
	cFILE.getflag( "PASSWORD", "INVEN_CLEAR", 0, 1, strPassInvenClear );

	/* game notice, Juver, 2021/06/12 */
	cFILE.getflag( "PASSWORD", "GAME_NOTICE_RELOAD", 0, 1, strPassGameNoticeReload );

	cFILE.getflag( "PASSWORD", "GIS_RELOAD", 0, 1, strPassGISReload );

	return TRUE;
}