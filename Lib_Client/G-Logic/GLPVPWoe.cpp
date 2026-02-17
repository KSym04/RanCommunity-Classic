#include "stdafx.h"
#include "GLPVPWoe.h"

#include "../Lib_Engine/Common/GLTexFile.h"
#include "../Lib_Engine/Common/IniLoader.h"
#include "../Lib_Engine/G-Logic/GLogic.h"
#include "./GLGaeaServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool GLPVPWoe::Load( std::string strFile )
{
	if( strFile.empty() )	return FALSE;

	std::string strpath;
	strpath = GLOGIC::GetServerPath ();
	strpath += strFile;

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if( !cFILE.open ( strpath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLPVPWoe::Load(), File Open %s", strFile.c_str() );
		return false;
	}

	cFILE.getflag( "EVENT", "EVENT_MAP", 0, 2, m_sMap.wMainID );
	cFILE.getflag( "EVENT", "EVENT_MAP", 1, 2, m_sMap.wSubID );

	for( DWORD i=0; i<3; ++i ){
		cFILE.getflag( "EVENT", "EVENT_GATE", i, 3, m_wSchoolGate[i] );
	}

	cFILE.getflag( "EVENT", "EXIT_MAP", 0, 2, m_sOutMap.wMainID );
	cFILE.getflag( "EVENT", "EXIT_MAP", 1, 2, m_sOutMap.wSubID );
	cFILE.getflag( "EVENT", "EXIT_GATE", 0, 1, m_wOutGate );

	cFILE.getflag( "EVENT", "PLAYER_LIMIT", 0, 1, m_wPlayerLimit );
	cFILE.getflag( "EVENT", "LEVEL_REQ", 0, 1, m_wLevelReq );

	cFILE.getflag( "EVENT", "DURATION_REGISTER", 0, 1, m_wRegisterTime );
	cFILE.getflag( "EVENT", "DURATION_BATTLE", 0, 1, m_wBattleTime );
	cFILE.getflag( "EVENT", "DURATION_REWARD", 0, 1, m_wRewardTime );

	cFILE.getflag( "EVENT", "EMPERIUM_TOWER", 0, 2, m_sWoeTowerData.sidCrow.wMainID );
	cFILE.getflag( "EVENT", "EMPERIUM_TOWER", 1, 2, m_sWoeTowerData.sidCrow.wSubID );

	cFILE.getflag("EVENT", "GUILD_ITEM_REWARD", 0, 2, m_sItemRewardGuild.wMainID);
	cFILE.getflag("EVENT", "GUILD_ITEM_REWARD", 1, 2, m_sItemRewardGuild.wSubID);

	cFILE.getflag("EVENT", "PLAYER_ITEM_REWARD", 0, 2, m_sItemRewardPlayer.wMainID);
	cFILE.getflag("EVENT", "PLAYER_ITEM_REWARD", 1, 2, m_sItemRewardPlayer.wSubID);

	cFILE.getflag("EVENT", "RESU_ITEM_REWARD", 0, 2, m_sItemRewardResu.wMainID);
	cFILE.getflag("EVENT", "RESU_ITEM_REWARD", 1, 2, m_sItemRewardResu.wSubID);
	
	if ( m_wPlayerLimit == 0 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPWoe::Load invalid PlayerLimit:%u limit must be atleast 1", m_wPlayerLimit );
		m_wPlayerLimit = 1;
	}

	if ( m_wLevelReq == 0 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPWoe::Load invalid LevelReq:%u req must be atleast 1", m_wLevelReq );
		m_wLevelReq = 1;
	}

	m_vecSchedule.clear();
	DWORD dwnum = cFILE.GetKeySize( "EVENT", "BATTLE_TIME" );
	for( DWORD i=0; i<dwnum; ++i )
	{
		WOE_SCHED sched;
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 0, 3, sched.wStartDay );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 1, 3, sched.wStartHour );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 2, 3, sched.wStartMinute );

		if ( sched.wStartDay < 1 || sched.wStartDay > 7 )
		{
			CDebugSet::MsgBoxAfx( "GLPVPWoe::Load invalid startday:%u index:%d", sched.wStartDay, i );
			continue;
		}

		if ( sched.wStartHour >= 24 )
		{
			CDebugSet::MsgBoxAfx( "GLPVPWoe::Load invalid StartHour:%u index:%d", sched.wStartHour, i );
			continue;
		}


		if ( sched.wStartMinute >= 60 )
		{
			CDebugSet::MsgBoxAfx( "GLPVPWoe::Load invalid StartMinute:%u index:%d", sched.wStartMinute, i );
			continue;
		}

		m_vecSchedule.push_back( sched );
	}

	std::sort( m_vecSchedule.begin(), m_vecSchedule.end() );

	if ( m_vecSchedule.empty() )
	{
		CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPWoe::Load no schedules loaded" );
	}

	return true;
}

void GLPVPWoe::FindScheduleFirst()
{
	if ( m_vecSchedule.empty() )	return;

	m_sScheduleNext.dwIndex = WOE_SCHEDULE_NONE;

	if ( m_vecSchedule.size() == 1 )
	{
		m_sScheduleNext.dwIndex = 0;
		m_sScheduleNext.sStartSched = m_vecSchedule[0];
		m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );
	}
	else
	{
		CTime ctimecur = CTime::GetCurrentTime();

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

			if ( m_sScheduleNext.dwIndex != WOE_SCHEDULE_NONE )
				break;

			if ( nminutes >= 11000)
				break;

			CTimeSpan span( 0, 0, 1, 0 );
			ctimecur += span;
			nminutes ++;
			Sleep(0);
		}
	}
};

bool GLPVPWoe::FindScheduleNext()
{
	if ( m_sScheduleNext.dwIndex == WOE_SCHEDULE_NONE )	return false;

	DWORD dwnewidx = m_sScheduleNext.dwIndex + 1;
	if ( dwnewidx >= (DWORD)m_vecSchedule.size() )	dwnewidx = 0;

	m_sScheduleNext.dwIndex = dwnewidx;
	m_sScheduleNext.sStartSched = m_vecSchedule[dwnewidx];
	m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

	return true;
};

