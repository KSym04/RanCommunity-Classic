#include "stdafx.h"
#include "./PVPClubDeathMatch.h"

#include "../../Lib_Engine/Common/gltexfile.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRPath.h"

#include "../../Lib_Helper/CommonContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PVPClubDeathMatch::PVPClubDeathMatch()
	: m_emState(PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED)

	, m_sBattleMap(false)
	, m_dwBattleGate(0)

	, m_sLobbyMap(false)
	, m_dwLobbyGate(0)
	
	, m_sOutMap(false)
	, m_dwOutGate(0)

	, m_wRegisterTime(0)
	, m_wBattleTime(0)
	, m_wRewardTime(0)

	, m_wLevelReq(1)
	, m_wClubLevelLimit(1)
	, m_wClubPlayerMin(1)
	, m_wClubPlayerMax(1)
	, m_bClubPlayerReqCDMRights(true)

	, m_fMoveLobbyAreaTime(30.0f)
	, m_fMoveBattleAreaTime(30.0f)

	, m_bDisableOtherGuildResu(false)

	, m_wDefaultPoint(100)
	, m_wPointKillGain(1)
	, m_wPointDeathLoss(1)
	, m_wDoublePointKillGain(1)
	, m_wDoublePointDeathLoss(1)
	, m_wPointResu(0)
	, m_fRankUpdateTime(10.0f)

	, m_fDoublePointRechargeTime(300.0f)
	, m_fDoublePointConsumeTime(60.0f)
	, m_bHashCheck(FALSE)					/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		m_sReward[i] = PVP_CLUB_DEATH_MATCH_REWARD_DATA();
	}
}

PVPClubDeathMatch::~PVPClubDeathMatch()
{

}

bool PVPClubDeathMatch::DataLoad( std::string strFile )
{
	if( strFile.empty() )	return FALSE;

	std::string strpath;
	strpath = RPATH::getDataGLogicServerPath();
	strpath += strFile;

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if( !cFILE.open ( strpath, true ) )
	{
		RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatch::Load failed %s", strFile.c_str() );

		return false;
	}

	DataReset();

	//config map
	cFILE.getflag( "EVENT", "BATTLE_MAP", 0, 2, m_sBattleMap.wMainID );
	cFILE.getflag( "EVENT", "BATTLE_MAP", 1, 2, m_sBattleMap.wSubID );
	cFILE.getflag( "EVENT", "BATTLE_GATE", 0, 1, m_dwBattleGate );

	cFILE.getflag( "EVENT", "LOBBY_MAP", 0, 2, m_sLobbyMap.wMainID );
	cFILE.getflag( "EVENT", "LOBBY_MAP", 1, 2, m_sLobbyMap.wSubID );
	cFILE.getflag( "EVENT", "LOBBY_GATE", 0, 1, m_dwLobbyGate );

	cFILE.getflag( "EVENT", "OUT_MAP", 0, 2, m_sOutMap.wMainID );
	cFILE.getflag( "EVENT", "OUT_MAP", 1, 2, m_sOutMap.wSubID );
	cFILE.getflag( "EVENT", "OUT_GATE", 0, 1, m_dwOutGate );

	//config durations
	cFILE.getflag( "EVENT", "DURATION_REGISTER", 0, 1, m_wRegisterTime );
	cFILE.getflag( "EVENT", "DURATION_BATTLE", 0, 1, m_wBattleTime );
	cFILE.getflag( "EVENT", "DURATION_REWARD", 0, 1, m_wRewardTime );

	//config conditions
	cFILE.getflag( "EVENT", "REQ_PLAYER_LEVEL", 0, 1, m_wLevelReq );
	cFILE.getflag( "EVENT", "REQ_CLUB_LEVEL", 0, 1, m_wClubLevelLimit );
	cFILE.getflag( "EVENT", "CLUB_PARTICIPANT_MIN", 0, 1, m_wClubPlayerMin );
	cFILE.getflag( "EVENT", "CLUB_PARTICIPANT_MAX", 0, 1, m_wClubPlayerMax );
	cFILE.getflag( "EVENT", "CLUB_PARTICIPANT_REQ_RIGHTS", 0, 1, m_bClubPlayerReqCDMRights );

	cFILE.getflag( "EVENT", "MOVE_LOBBY_AREA_TIME", 0, 1, m_fMoveLobbyAreaTime );
	cFILE.getflag( "EVENT", "MOVE_BATTLE_AREA_TIME", 0, 1, m_fMoveBattleAreaTime );

	cFILE.getflag( "EVENT", "DISABLE_OTHER_GUILD_RESU", 0, 1, m_bDisableOtherGuildResu );


	cFILE.getflag( "EVENT", "POINT_DEFAULT", 0, 1, m_wDefaultPoint );
	cFILE.getflag( "EVENT", "POINT_KILL_GAIN", 0, 1, m_wPointKillGain );
	cFILE.getflag( "EVENT", "DOUBLE_POINT_DEATH_LOSS", 0, 1, m_wDoublePointDeathLoss );
	cFILE.getflag( "EVENT", "DOUBLE_POINT_KILL_GAIN", 0, 1, m_wDoublePointKillGain );
	cFILE.getflag( "EVENT", "POINT_DEATH_LOSS", 0, 1, m_wPointDeathLoss );
	cFILE.getflag( "EVENT", "POINT_RESU", 0, 1, m_wPointResu );
	cFILE.getflag( "EVENT", "RANK_UPDATE_TIME", 0, 1, m_fRankUpdateTime );

	cFILE.getflag( "EVENT", "DOUBLE_POINT_CHARGE_TIME", 0, 1, m_fDoublePointRechargeTime );
	cFILE.getflag( "EVENT", "DOUBLE_POINT_CONSUME_TIME", 0, 1, m_fDoublePointConsumeTime );

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	cFILE.getflag( "EVENT", "HASH_CHECK", 0, 1, m_bHashCheck );

	//reward
	cFILE.getflag( "EVENT", "REWARD_TOP_1", 0, 3, m_sReward[0].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_1", 1, 3, m_sReward[0].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_1", 2, 3, m_sReward[0].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_2", 0, 3, m_sReward[1].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_2", 1, 3, m_sReward[1].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_2", 2, 3, m_sReward[1].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_3", 0, 3, m_sReward[2].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_3", 1, 3, m_sReward[2].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_3", 2, 3, m_sReward[2].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_4", 0, 3, m_sReward[3].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_4", 1, 3, m_sReward[3].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_4", 2, 3, m_sReward[3].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_5", 0, 3, m_sReward[4].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_5", 1, 3, m_sReward[4].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_5", 2, 3, m_sReward[4].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_6", 0, 3, m_sReward[5].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_6", 1, 3, m_sReward[5].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_6", 2, 3, m_sReward[5].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_7", 0, 3, m_sReward[6].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_7", 1, 3, m_sReward[6].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_7", 2, 3, m_sReward[6].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_8", 0, 3, m_sReward[7].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_8", 1, 3, m_sReward[7].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_8", 2, 3, m_sReward[7].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_9", 0, 3, m_sReward[8].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_9", 1, 3, m_sReward[8].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_9", 2, 3, m_sReward[8].wContributionPoint );

	cFILE.getflag( "EVENT", "REWARD_TOP_10", 0, 3, m_sReward[9].sItemID.wMainID );
	cFILE.getflag( "EVENT", "REWARD_TOP_10", 1, 3, m_sReward[9].sItemID.wSubID );
	cFILE.getflag( "EVENT", "REWARD_TOP_10", 2, 3, m_sReward[9].wContributionPoint );


	//schedule day-hour-minute

	DWORD dwNum = cFILE.GetKeySize( "EVENT", "BATTLE_TIME" );
	for( DWORD i=0; i<dwNum; ++i )
	{
		PVP_CLUB_DEATH_MATCH_EVENT_SCHED sSchedule;
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 0, 4, sSchedule.wStartDay );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 1, 4, sSchedule.wStartHour );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 2, 4, sSchedule.wStartMinute );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 3, 4, sSchedule.bCombatRecord );

		if ( sSchedule.wStartDay < 1 || sSchedule.wStartDay > 7 )
		{
			CDebugSet::MsgBoxAfx( "PVPClubDeathMatch::Load invalid startday:%u index:%d", sSchedule.wStartDay, i );
			continue;
		}

		if ( sSchedule.wStartHour >= 24 )
		{
			CDebugSet::MsgBoxAfx( "PVPClubDeathMatch::Load invalid StartHour:%u index:%d", sSchedule.wStartHour, i );
			continue;
		}

		if ( sSchedule.wStartMinute >= 60 )
		{
			CDebugSet::MsgBoxAfx( "PVPClubDeathMatch::Load invalid StartMinute:%u index:%d", sSchedule.wStartMinute, i );
			continue;
		}

		m_vecSchedule.push_back( sSchedule );
	}

	std::sort( m_vecSchedule.begin(), m_vecSchedule.end() );

	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatch::Load %u schedules loaded", m_vecSchedule.size() );

	return true;
}

void PVPClubDeathMatch::DataReset()
{
	m_emState = PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED;

	m_sBattleMap.ResetNativeID();
	m_dwBattleGate	= 0;

	m_sLobbyMap.ResetNativeID();
	m_dwLobbyGate	= 0;
	
	m_sOutMap.ResetNativeID();
	m_dwOutGate		= 0;

	m_wRegisterTime = 0;
	m_wBattleTime	= 0;
	m_wRewardTime	= 0;

	m_wLevelReq			= 1;
	m_wClubLevelLimit	= 1;
	m_wClubPlayerMin	= 1;
	m_wClubPlayerMax	= 1;
	m_bClubPlayerReqCDMRights = true;

	m_fMoveLobbyAreaTime = 30.0f;
	m_fMoveBattleAreaTime = 30.0f;

	m_bDisableOtherGuildResu = false;

	m_wDefaultPoint = 100;
	m_wPointKillGain = 1;
	m_wPointDeathLoss = 1;
	m_wDoublePointKillGain = 1;
	m_wDoublePointDeathLoss = 1;
	m_wPointResu = 0;
	m_fRankUpdateTime = 10.0f;

	m_fDoublePointRechargeTime = 300.0f;
	m_fDoublePointConsumeTime = 60.0f;

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		m_sReward[i] = PVP_CLUB_DEATH_MATCH_REWARD_DATA();
	}


	m_sScheduleNext.Reset();

	common_vector_free( m_vecSchedule );
}

void PVPClubDeathMatch::FindScheduleFirst()
{
	if ( m_vecSchedule.empty() )	return;

	m_sScheduleNext.dwIndex = PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE;

	if ( m_vecSchedule.size() == 1 )
	{
		m_sScheduleNext.dwIndex = 0;
		m_sScheduleNext.sStartSched = m_vecSchedule[0];
		m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

		RLOG::LogFile( "_PVPClubDeathMatch.txt","PVPClubDeathMatch::FindScheduleFirst Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
			m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
			m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );
	}
	else
	{
		CTime ctimecur = CTime::GetCurrentTime();

		//expensive loop
		int nminutes = 0;
		while(1)
		{
			for ( size_t i=0; i<m_vecSchedule.size(); ++i )
			{
				if ( m_vecSchedule[i].wStartDay != (WORD)ctimecur.GetDayOfWeek() )	continue;
				if ( m_vecSchedule[i].wStartHour != (WORD)ctimecur.GetHour() )		continue;
				if ( m_vecSchedule[i].wStartMinute != (WORD)ctimecur.GetMinute() )	continue;

				m_sScheduleNext.dwIndex = i;
				m_sScheduleNext.sStartSched = m_vecSchedule[i];
				m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );
			}

			if ( m_sScheduleNext.dwIndex != PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE )
				break;

			//week limit
			if ( nminutes >= 11000/*10080*/ )
				break;

			CTimeSpan span( 0, 0, 1, 0 );
			ctimecur += span;
			nminutes ++;
			Sleep(0);
		}

		if ( m_sScheduleNext.dwIndex == PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE )
		{
			RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatch::FindScheduleFirst %u failed! no events will start" );
		}
		else
		{
			RLOG::LogFile( "_PVPClubDeathMatch.txt","PVPClubDeathMatch::FindScheduleFirst Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
				m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
				m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );
		}
	}
};

bool PVPClubDeathMatch::FindScheduleNext()
{
	if ( m_sScheduleNext.dwIndex == PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE )	return false;

	DWORD dwnewidx = m_sScheduleNext.dwIndex + 1;
	if ( dwnewidx >= (DWORD)m_vecSchedule.size() )	dwnewidx = 0;

	m_sScheduleNext.dwIndex = dwnewidx;
	m_sScheduleNext.sStartSched = m_vecSchedule[dwnewidx];
	m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatch::FindScheduleNext Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
		m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
		m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );

	return true;
};