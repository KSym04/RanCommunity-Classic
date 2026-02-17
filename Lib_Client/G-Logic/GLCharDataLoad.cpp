#include "stdafx.h"
#include "./GLCharData.h"
#include "./GLogicData.h"
#include "./GLQuest.h"
#include "./GLQuestMan.h"

#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Common/GLTexFile.h"

#include "../../Lib_Engine/Core/NSRParam.h"

/*activity system, Juver, 2017/11/01 */
#include "./GLActivity.h"

#include "./GLCodex.h" /* Codex, Archie 02/16/24 */
/*12-9-14, Battle Pass - CNDev*/
#include "./GLBattlePass.h"

#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL SCHARDATA2::LOADFILE ( const char* szFileName )
{
	if( !szFileName )				return FALSE;
	if( strlen(szFileName) == 0 )	return FALSE;

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
		CDebugSet::ToLogFile( "ERROR : SCHARDATA2::LOADFILE(), %s", szFileName );
		return FALSE;
	}

	WORD dwVERSION = 0;
	cFILE.getflag( "VERSION", 1, 1, dwVERSION );

	cFILE.getflag( "emTribe", 1, 1, m_emTribe );
	cFILE.getflag( "emClass", 1, 1, m_emClass );
	cFILE.getflag( "wSchool", 1, 1, m_wSchool );
	cFILE.getflag( "wSex", 1, 1, m_wSex );
	cFILE.getflag( "wHair", 1, 1, m_wHair );
	cFILE.getflag( "wHairColor", 1, 1, m_wHairColor );
	cFILE.getflag( "wFace", 1, 1, m_wFace );
	
	cFILE.getflag( "nBright", 1, 1, m_nBright );
	cFILE.getflag( "wLevel" , 1, 1, m_wLevel );
	cFILE.getflag( "wGuild" , 1, 1, m_dwGuild );
	cFILE.getflag( "lnMoney", 1, 1, m_lnMoney );

	cFILE.getflag( "sStats", 1, 6, m_sStats.wPow );
	cFILE.getflag( "sStats", 2, 6, m_sStats.wStr );
	cFILE.getflag( "sStats", 3, 6, m_sStats.wSpi );
	cFILE.getflag( "sStats", 4, 6, m_sStats.wDex );
	cFILE.getflag( "sStats", 5, 6, m_sStats.wInt );
	cFILE.getflag( "sStats", 6, 6, m_sStats.wSta );
	
	cFILE.getflag( "wStatsPoint", 1, 1, m_wStatsPoint );
	cFILE.getflag( "wAP", 1, 1, m_wAP );
	cFILE.getflag( "wDP", 1, 1, m_wDP );
	cFILE.getflag( "wPA", 1, 1, m_wPA );
	cFILE.getflag( "wSA", 1, 1, m_wSA );

	cFILE.getflag( "sExperience", 1, 2, m_sExperience.lnNow );
	cFILE.getflag( "sExperience", 2, 2, m_sExperience.lnMax );

	cFILE.getflag( "dwSkillPoint", 1, 1, m_dwSkillPoint );
	
	cFILE.getflag( "sHP", 1, 2, m_sHP.wNow );
	cFILE.getflag( "sHP", 2, 2, m_sHP.wMax );

	cFILE.getflag( "sMP", 1, 2, m_sMP.wNow );
	cFILE.getflag( "sMP", 2, 2, m_sMP.wMax );

	cFILE.getflag( "sSP", 1, 2, m_sSP.wNow );
	cFILE.getflag( "sSP", 2, 2, m_sSP.wMax );

	/*combatpoint logic, Juver, 2017/05/27 */
	cFILE.getflag( "sCP", 1, 2, m_sCombatPoint.wNow );
	cFILE.getflag( "sCP", 2, 2, m_sCombatPoint.wMax );

	cFILE.getflag( "wPK", 1, 1, m_wPK );

	cFILE.getflag( "sStartMapID", 1, 2, m_sStartMapID.wMainID );
	cFILE.getflag( "sStartMapID", 2, 2, m_sStartMapID.wSubID );

	cFILE.getflag( "dwStartGate", 1, 1, m_dwStartGate );

	cFILE.getflag( "vStartPos", 1, 3, m_vStartPos.x );
	cFILE.getflag( "vStartPos", 2, 3, m_vStartPos.y );
	cFILE.getflag( "vStartPos", 3, 3, m_vStartPos.z );

	cFILE.getflag( "fScaleRange", 1, 1, m_fScaleRange );

	/*contribution point, Juver, 2017/08/23 */
	cFILE.getflag( "dwContributionPoint", 1, 1, m_dwContributionPoint );

	/*activity point, Juver, 2017/08/23 */
	cFILE.getflag( "dwActivityPoint", 1, 1, m_dwActivityPoint );

	cFILE.getflag( "dwWarChips", 1, 1, m_dwWarChips );
	cFILE.getflag( "dwGamePoints", 1, 1, m_dwGamePoints );

	/* play time system, Juver, 2021/01/26 */
	cFILE.getflag( "llPlayTime", 1, 1, m_llPlayTime );
	cFILE.getflag( "llPlayPoint", 1, 1, m_llPlayPoint );

	m_ExpSkills.clear();
	DWORD dwNUM = cFILE.getflagnum ( "ExpSkills" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SCHARSKILL CharSkill;
		cFILE.getflag( i, "ExpSkills", 1, 3, CharSkill.sNativeID.wMainID );
		cFILE.getflag( i, "ExpSkills", 2, 3, CharSkill.sNativeID.wSubID );
		cFILE.getflag( i, "ExpSkills", 3, 3, CharSkill.wLevel );

		m_ExpSkills[CharSkill.sNativeID.dwID] = CharSkill;
	}

	for ( int i=0; i<EMSKILLQUICK_SIZE; ++i)	m_sSKILLQUICK[i] = NATIVEID_NULL();

	dwNUM = cFILE.getflagnum ( "QuickSkill" );
	if ( dwNUM > EMSKILLQUICK_SIZE ) dwNUM = EMSKILLQUICK_SIZE;

	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SNATIVEID skillID;
		cFILE.getflag( i, "QuickSkill", 1, 2, skillID.wMainID );
		cFILE.getflag( i, "QuickSkill", 2, 2, skillID.wSubID );

		m_sSKILLQUICK[i] = skillID;
	}

	m_cQuestPlay.DeleteAll();
	dwNUM = cFILE.getflagnum ( "Quest" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		DWORD dwNID;
		cFILE.getflag( i, "Quest", 1, 1, dwNID );

		//	Note : 퀘스트 시작을 위한 검사.
		//
		GLQUEST* pQUEST = GLQuestMan::GetInstance().Find ( dwNID );
		if ( !pQUEST )	continue;

		//	Note : 새로이 시작되는 퀘스트 설정.
		//
		GLQUESTPROG sQUEST_PROG_NEW;
		sQUEST_PROG_NEW.START ( pQUEST, NULL );

		//	리스트에 등록.
		m_cQuestPlay.InsertProc ( sQUEST_PROG_NEW );
	}

	char* strBodyPart[16] =
	{
		"HEAD",	
		"UPPER", 
		"LOWER", 
		"HAND", 
		"FOOT", 
		"RHAND", 
		"LHAND", 
		"NECK", 
		"WRIST", 
		"RFINGER", 
		"LFINGER",
		"RHAND_S", 
		"LHAND_S",
		"VEHICLE",
	};
	
	for ( int i=0; i<SLOT_TSIZE; ++i )
		m_PutOnItems[i].sNativeID = NATIVEID_NULL();
	
	dwNUM = cFILE.getflagnum ( "PutOnItems" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SITEMCUSTOM ItemCustom;

		std::string strArg;
		cFILE.getflag( i, "PutOnItems", 1, 11, strArg );
		int BodyPartNum = -1;

		for ( int j = 0; j < SLOT_NSIZE_S_2; j++ )
		{
			if ( strArg == strBodyPart[j] )
			{		
				BodyPartNum = j;
				break;
			}
		}

		if ( BodyPartNum != -1 )
		{
			cFILE.getflag( i, "PutOnItems", 2, 11, ItemCustom.sNativeID.wMainID );
			cFILE.getflag( i, "PutOnItems", 3, 11, ItemCustom.sNativeID.wSubID );
			cFILE.getflag( i, "PutOnItems", 4, 11, ItemCustom.wTurnNum );
			cFILE.getflag( i, "PutOnItems", 5, 11, ItemCustom.cDAMAGE );
			cFILE.getflag( i, "PutOnItems", 6, 11, ItemCustom.cDEFENSE );
			cFILE.getflag( i, "PutOnItems", 7, 11, ItemCustom.cRESIST_FIRE	);
			cFILE.getflag( i, "PutOnItems", 8, 11, ItemCustom.cRESIST_ICE	);	
			cFILE.getflag( i, "PutOnItems", 9, 11, ItemCustom.cRESIST_ELEC );
			cFILE.getflag( i, "PutOnItems", 10, 11, ItemCustom.cRESIST_POISON );
			cFILE.getflag( i, "PutOnItems", 11, 11, ItemCustom.cRESIST_SPIRIT );

			//	Note : Set item creation TYPE to initial grant.
			//
			ItemCustom.cGenType = EMGEN_INIT;

			m_PutOnItems[BodyPartNum] = ItemCustom;
		}
	}

	m_cInventory.DeleteItemAll ();
	dwNUM = cFILE.getflagnum ( "cInventory" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SINVENITEM rInvenItem;

		cFILE.getflag( i, "cInventory", 1, 10, rInvenItem.sItemCustom.sNativeID.wMainID );
		cFILE.getflag( i, "cInventory", 2, 10, rInvenItem.sItemCustom.sNativeID.wSubID );

		cFILE.getflag( i, "cInventory", 3, 10, rInvenItem.sItemCustom.wTurnNum );

		cFILE.getflag( i, "cInventory", 4, 10, rInvenItem.sItemCustom.cDAMAGE );
		cFILE.getflag( i, "cInventory", 5, 10, rInvenItem.sItemCustom.cDEFENSE );

		cFILE.getflag( i, "cInventory", 6, 10, rInvenItem.sItemCustom.cRESIST_FIRE );
		cFILE.getflag( i, "cInventory", 7, 10, rInvenItem.sItemCustom.cRESIST_ICE	);
		cFILE.getflag( i, "cInventory", 8, 10, rInvenItem.sItemCustom.cRESIST_ELEC );
		cFILE.getflag( i, "cInventory", 9, 10, rInvenItem.sItemCustom.cRESIST_POISON );
		cFILE.getflag( i, "cInventory", 10, 10, rInvenItem.sItemCustom.cRESIST_SPIRIT );
											   
		//	Note : Set item creation TYPE to initial grant.
		//									   
		rInvenItem.sItemCustom.cGenType = EMGEN_INIT;

		m_cInventory.InsertItem ( rInvenItem.sItemCustom );
	}

	for ( int i=0; i<EMACTIONQUICK_SIZE; ++i)	m_sACTIONQUICK[i].sNID = NATIVEID_NULL();

	dwNUM = cFILE.getflagnum ( "QuickAction" );
	if ( dwNUM > EMACTIONQUICK_SIZE ) dwNUM = EMACTIONQUICK_SIZE;

	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SACTION_SLOT sItemSlot;
		sItemSlot.wACT = EMACT_SLOT_DRUG;
		cFILE.getflag( i, "QuickAction", 1, 2, sItemSlot.sNID.wMainID );
		cFILE.getflag( i, "QuickAction", 2, 2, sItemSlot.sNID.wSubID );

		m_sACTIONQUICK[i] = sItemSlot;
	}


	/*activity system, Juver, 2017/11/01 */
	SACTIVITY_FILE_DATA_MAP &map_activity_file_data = GLActivity::GetInstance().m_mapActivity;
	for( SACTIVITY_FILE_DATA_MAP_ITER pos = map_activity_file_data.begin();
		pos != map_activity_file_data.end(); ++pos )
	{
		SACTIVITY_FILE_DATA &sactivity_data = (*pos).second; 

		SACTIVITY_CHAR_DATA_MAP_ITER iter_done = m_mapActivityDone.find( sactivity_data.dwActivityID );
		if ( iter_done != m_mapActivityDone.end() )	continue;

		SACTIVITY_CHAR_DATA_MAP_ITER iter_prog = m_mapActivityProg.find( sactivity_data.dwActivityID );
		if ( iter_prog != m_mapActivityProg.end() )	continue;

		SACTIVITY_CHAR_DATA sactivity_char_data;
		sactivity_char_data.Assign( sactivity_data );
		m_mapActivityProg.insert( std::make_pair( sactivity_char_data.dwActivityID, sactivity_char_data ) );
	}

	/*12-9-14, Battle Pass - CNDev*/
	CTime cCurrentTime = CTime::GetCurrentTime();
	if (m_cBattlePassResetTimer.GetDay() != cCurrentTime.GetDay())
	{
		m_cBattlePassResetTimer = cCurrentTime;

		for (SBATTLEPASS_CHAR_DATA_MAP_ITER iter = m_mapBattlePassProg.begin(); iter != m_mapBattlePassProg.end();)
		{
			SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = iter->second;

			iter = m_mapBattlePassProg.erase(iter);
		}

		m_mapBattlePassDone.clear();

		std::vector<SBATTLEPASS_FILE_DATA*> vecBattlePass;

		SBATTLEPASS_FILE_DATA_MAP& sBattlePassMission = GLBattlePass::GetInstance().m_mapBattlePass;
		for (SBATTLEPASS_FILE_DATA_MAP_ITER iter = sBattlePassMission.begin(); iter != sBattlePassMission.end(); ++iter)
		{
			SBATTLEPASS_FILE_DATA& sBattlePass = iter->second;
			vecBattlePass.push_back(&sBattlePass);
		}

		typedef boost::mt19937 RNGType;
		RNGType rng(static_cast<unsigned int>(time(0)));
		boost::uniform_int<> dist(0, vecBattlePass.size() - 1);
		boost::variate_generator<RNGType&, boost::uniform_int<> > gen(rng, dist);

		for (size_t i = 0; i < vecBattlePass.size(); ++i)
		{
			int nRandomIndex = dist(rng);
			std::swap(vecBattlePass[i], vecBattlePass[nRandomIndex]);
		}


		int nMissionCount = 0;
		for (size_t i = 0; i < vecBattlePass.size(); ++i)
		{
			if (nMissionCount >= BATTLEPASS_DAILY_MISSION_MAX)
				break;

			SBATTLEPASS_FILE_DATA* pBattlePassData = vecBattlePass[i];
			SBATTLEPASS_CHAR_DATA sNewBattlePassData;
			sNewBattlePassData.Assign(*pBattlePassData);
			m_mapBattlePassProg[pBattlePassData->dwBattlePassID] = sNewBattlePassData;

			nMissionCount++;
		}


	}
	/*12-16-24, daily login - CNDev*/
	//CTime cCurrentTime = CTime::GetCurrentTime(); //Montage Daily
	CTimeSpan cTimeDifference = cCurrentTime - m_cDailyLoginTimer;

	if (m_cDailyLoginTimer.GetDay() != cCurrentTime.GetDay())
	{
		if (cTimeDifference.GetDays() > 1)
		{
			m_dwDayStreakCounter = 0; // Reset streak
		}

		m_bDailyLogin = true;
	}
	else
	{
		m_bDailyLogin = false;
	}
	
	int nBeginIndex = 0;
    int nEndIndex = 0;
	
    switch ( m_emClass )
    {
    case GLCC_BRAWLER_M:
    case GLCC_BRAWLER_W:
        {
            nBeginIndex = EMSKILL_BRAWLER_01;
            nEndIndex = EMSKILL_BRAWLER_04;
        }
        break;

    case GLCC_SWORDSMAN_M:
    case GLCC_SWORDSMAN_W:
        {
            nBeginIndex = EMSKILL_SWORDSMAN_01;
            nEndIndex = EMSKILL_SWORDSMAN_04;
        }
        break;

    case GLCC_ARCHER_M:
    case GLCC_ARCHER_W:
        {
            nBeginIndex = EMSKILL_ARCHER_01;
            nEndIndex = EMSKILL_ARCHER_04;
        }
        break;

    case GLCC_SHAMAN_M:
    case GLCC_SHAMAN_W:
        {
            nBeginIndex = EMSKILL_SHAMAN_01;
            nEndIndex = EMSKILL_SHAMAN_04;
        }
        break;    

    case GLCC_EXTREME_M:
    case GLCC_EXTREME_W:
        {
            nBeginIndex = EMSKILL_EXTREME_01;
            nEndIndex = EMSKILL_EXTREME_04;
        }
        break;
        //add class
    case GLCC_GUNNER_M:
    case GLCC_GUNNER_W:
        {
            nBeginIndex = EMSKILL_GUNNER_01;
            nEndIndex = EMSKILL_GUNNER_04;
        }
        break;
		
	case GLCC_ASSASSIN_M:
	case GLCC_ASSASSIN_W:
		{
			nBeginIndex = EMSKILL_ASSASSIN_01;
			nEndIndex = EMSKILL_ASSASSIN_04;
		}
		break;
    };

    WORD wClass, wIndex;
    GLSkillMan::GetInstance().GetMaxSkill ( wClass, wIndex );

	/* Codex, Archie 02/16/24 */
	SCODEX_FILE_DATA_MAP &map_codex_file_data = GLCodex::GetInstance().m_mapCodex;
	for( SCODEX_FILE_DATA_MAP_ITER pos = map_codex_file_data.begin();
		pos != map_codex_file_data.end(); ++pos )
	{
		SCODEX_FILE_DATA &scodex_data = (*pos).second; 

		SCODEX_CHAR_DATA_MAP_ITER iter_done = m_mapCodexDone.find( scodex_data.dwCodexID );
		if ( iter_done != m_mapCodexDone.end() )	continue;

		SCODEX_CHAR_DATA_MAP_ITER iter_prog = m_mapCodexProg.find( scodex_data.dwCodexID );
		if ( iter_prog != m_mapCodexProg.end() )	continue;

		SCODEX_CHAR_DATA scodex_char_data;
		scodex_char_data.Assign( scodex_data );
		m_mapCodexProg.insert( std::make_pair( scodex_char_data.dwCodexID, scodex_char_data ) );
	}

	return TRUE;
}