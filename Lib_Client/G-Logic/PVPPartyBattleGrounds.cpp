#include "stdafx.h"
#include "./PVPPartyBattleGrounds.h"

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

namespace PVPPBG
{
	Manager::Manager()
		: m_emState(EVENT_STATE_ENDED)

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
		, m_dwContriReq(0)
		, m_wPlayerLimit(1)

		, m_bHashCheck(FALSE)
		, m_bDisableOtherResu(FALSE)
		, m_bDisableScoreSelfHeal(FALSE)

		, m_fScoreUpdateTime(1.0f)

		, m_bForceMoveIn(FALSE)
		, m_fForceMoveInTime(10.0f)

		, m_bNoPartyMoveOut(FALSE)
		, m_fNoPartyMoveOutTime(10.0f)

		, m_fKillVar(1.0f)
		, m_fHealVar(1.0f)
		, m_fResuVar(1.0f)

		, m_wPoint1(1)
		, m_wPoint2(1)
		, m_wPoint3(1)
	{
		
	}

	Manager::~Manager()
	{

	}

	bool Manager::DataLoad( std::string strFile )
	{
		if( strFile.empty() )	
			return FALSE;

		std::string strpath;
		strpath = RPATH::getDataGLogicServerPath();
		strpath += strFile;

		CIniLoader cFILE;

		if( GLOGIC::bGLOGIC_ZIPFILE )
			cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

		if( !cFILE.open ( strpath, true ) )
		{
			RLOG::LogFile( "_PVPPBG.txt", "DataLoad failed %s", strFile.c_str() );

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

		//entry conditions
		cFILE.getflag( "EVENT", "LEVEL_REQ", 0, 1, m_wLevelReq );
		cFILE.getflag( "EVENT", "CONTRI_REQ", 0, 1, m_dwContriReq );
		cFILE.getflag( "EVENT", "PLAYER_LIMIT", 0, 1, m_wPlayerLimit );
		
		cFILE.getflag( "EVENT", "PCID_CHECK", 0, 1, m_bHashCheck );

		cFILE.getflag( "EVENT", "DISABLE_OTHER_RESU", 0, 1, m_bDisableOtherResu );
		cFILE.getflag( "EVENT", "DISABLE_SCORE_SELF_HEAL", 0, 1, m_bDisableScoreSelfHeal );

		cFILE.getflag( "EVENT", "SCORE_UPDATE_TIME", 0, 1, m_fScoreUpdateTime );
		
		cFILE.getflag( "EVENT", "FORCE_MOVE_IN", 0, 1, m_bForceMoveIn );
		cFILE.getflag( "EVENT", "FORCE_MOVE_IN_TIME", 0, 1, m_fForceMoveInTime );

		cFILE.getflag( "EVENT", "FORCE_MOVE_NOPARTY", 0, 1, m_bNoPartyMoveOut );
		cFILE.getflag( "EVENT", "FORCE_MOVE_NOPARTY_TIME", 0, 1, m_fNoPartyMoveOutTime );

		//score logic
		cFILE.getflag( "EVENT", "VAR_KILL", 0, 1, m_fKillVar );
		cFILE.getflag( "EVENT", "VAR_HEAL", 0, 1, m_fHealVar );
		cFILE.getflag( "EVENT", "VAR_RESU", 0, 1, m_fResuVar );

		cFILE.getflag( "EVENT", "POINT_1", 0, 1, m_wPoint1 );
		cFILE.getflag( "EVENT", "POINT_2", 0, 1, m_wPoint2 );
		cFILE.getflag( "EVENT", "POINT_3", 0, 1, m_wPoint3 );

		//schedule day-hour-minute

		DWORD dwNum = cFILE.GetKeySize( "EVENT", "BATTLE_TIME" );
		for( DWORD i=0; i<dwNum; ++i )
		{
			EVENT_SCHEDEDULE sSchedule;
			cFILE.getflag( i, "EVENT", "BATTLE_TIME", 0, 4, sSchedule.wStartDay );
			cFILE.getflag( i, "EVENT", "BATTLE_TIME", 1, 4, sSchedule.wStartHour );
			cFILE.getflag( i, "EVENT", "BATTLE_TIME", 2, 4, sSchedule.wStartMinute );
			cFILE.getflag( i, "EVENT", "BATTLE_TIME", 3, 4, sSchedule.bCombatRecord );

			if ( sSchedule.wStartDay < 1 || sSchedule.wStartDay > 7 )
			{
				CDebugSet::MsgBoxAfx( "PVPPBG::Load invalid startday:%u index:%d", sSchedule.wStartDay, i );
				continue;
			}

			if ( sSchedule.wStartHour >= 24 )
			{
				CDebugSet::MsgBoxAfx( "PVPPBG::Load invalid StartHour:%u index:%d", sSchedule.wStartHour, i );
				continue;
			}

			if ( sSchedule.wStartMinute >= 60 )
			{
				CDebugSet::MsgBoxAfx( "PVPPBG::Load invalid StartMinute:%u index:%d", sSchedule.wStartMinute, i );
				continue;
			}

			m_vecSchedule.push_back( sSchedule );
		}

		std::sort( m_vecSchedule.begin(), m_vecSchedule.end() );

		RLOG::LogFile( "_PVPPBG.txt", "DataLoad %u schedules loaded", m_vecSchedule.size() );

		return true;
	}

	void Manager::DataReset()
	{
		m_emState = PVPPBG::EVENT_STATE_ENDED;

		m_sBattleMap.ResetNativeID();
		m_dwBattleGate	= 0;

		m_sLobbyMap.ResetNativeID();
		m_dwLobbyGate	= 0;

		m_sOutMap.ResetNativeID();
		m_dwOutGate		= 0;

		m_wRegisterTime = 0;
		m_wBattleTime	= 0;
		m_wRewardTime	= 0;

		m_wLevelReq = 1;
		m_dwContriReq = 0;
		m_wPlayerLimit = 1;

		m_bHashCheck = FALSE;
		m_bDisableOtherResu = FALSE;
		m_bDisableScoreSelfHeal = FALSE;

		m_fScoreUpdateTime = 1.0f;

		m_bForceMoveIn = FALSE;
		m_fForceMoveInTime = 10.0f;

		m_bNoPartyMoveOut = FALSE;
		m_fNoPartyMoveOutTime = 10.f;

		m_fKillVar = 1.0f;
		m_fHealVar = 1.0f;
		m_fResuVar = 1.0f;

		m_wPoint1 = 1;
		m_wPoint2 = 1;
		m_wPoint3 = 1;

		m_sScheduleNext.Reset();

		common_vector_free( m_vecSchedule );
	}

	void Manager::FindScheduleFirst()
	{
		if ( m_vecSchedule.empty() )	
			return;

		m_sScheduleNext.dwIndex = PVP_PBG_SCHEDULE_NONE;

		if ( m_vecSchedule.size() == 1 )
		{
			m_sScheduleNext.dwIndex = 0;
			m_sScheduleNext.sStartSched = m_vecSchedule[0];
			m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

			RLOG::LogFile( "_PVPPBG.txt","FindScheduleFirst Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
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
					if ( m_vecSchedule[i].wStartDay != (WORD)ctimecur.GetDayOfWeek() )	
						continue;

					if ( m_vecSchedule[i].wStartHour != (WORD)ctimecur.GetHour() )	
						continue;

					if ( m_vecSchedule[i].wStartMinute != (WORD)ctimecur.GetMinute() )	
						continue;

					m_sScheduleNext.dwIndex = i;
					m_sScheduleNext.sStartSched = m_vecSchedule[i];
					m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );
				}

				if ( m_sScheduleNext.dwIndex != PVP_PBG_SCHEDULE_NONE )
					break;

				//week limit
				if ( nminutes >= 11000/*10080*/ )
					break;

				CTimeSpan span( 0, 0, 1, 0 );
				ctimecur += span;
				nminutes ++;
				Sleep(0);
			}

			if ( m_sScheduleNext.dwIndex == PVP_PBG_SCHEDULE_NONE )
			{
				RLOG::LogFile( "_PVPPBG.txt", "FindScheduleFirst %u failed! no events will start" );
			}
			else
			{
				RLOG::LogFile( "_PVPPBG.txt","FindScheduleFirst Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
					m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
					m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );
			}
		}
	};

	bool Manager::FindScheduleNext()
	{
		if ( m_sScheduleNext.dwIndex == PVP_PBG_SCHEDULE_NONE )	
			return false;

		DWORD dwnewidx = m_sScheduleNext.dwIndex + 1;
		if ( dwnewidx >= (DWORD)m_vecSchedule.size() )	
			dwnewidx = 0;

		m_sScheduleNext.dwIndex = dwnewidx;
		m_sScheduleNext.sStartSched = m_vecSchedule[dwnewidx];
		m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

		RLOG::LogFile( "_PVPPBG.txt", "FindScheduleNext Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
			m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
			m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );

		return true;
	};
}