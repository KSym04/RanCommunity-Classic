#include "StdAfx.h"
#include "./GLPVPTyranny.h"

#include "../../Lib_Engine/Common/gltexfile.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"
#include "../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool GLPVPTyranny::Load( std::string strFile )
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
		CDebugSet::ToLogFile ( "ERROR : GLPVPTyranny::Load(), File Open %s", strFile.c_str() );
		return false;
	}


	//event map
	m_vecMapRandom.clear();
	DWORD dwMapNum = cFILE.GetKeySize( "EVENT", "EVENT_MAP" );
	for( DWORD i=0; i<dwMapNum; ++i )
	{
		//random map
		SNATIVEID sMapID;
		cFILE.getflag( i, "EVENT", "EVENT_MAP", 0, 2, sMapID.wMainID );
		cFILE.getflag( i, "EVENT", "EVENT_MAP", 1, 2, sMapID.wSubID );

		m_vecMapRandom.push_back( sMapID );

		if ( i == 0 )
		{
			//use the very first map as the fixed map
			m_sMapFixed = sMapID;
		}
	}

	//base gate
	for( DWORD i=0; i<TYRANNY_SCHOOL_SIZE; ++i ){
		cFILE.getflag( "EVENT", "EVENT_GATE", i, TYRANNY_SCHOOL_SIZE, m_wSchoolGate[i] );
	}

	//out map/gate
	cFILE.getflag( "EVENT", "EXIT_MAP", 0, 2, m_sOutMap.wMainID );
	cFILE.getflag( "EVENT", "EXIT_MAP", 1, 2, m_sOutMap.wSubID );
	cFILE.getflag( "EVENT", "EXIT_GATE", 0, 1, m_wOutGate );

	//reg settings
	cFILE.getflag( "EVENT", "PLAYER_LIMIT", 0, 1, m_wPlayerLimit );
	cFILE.getflag( "EVENT", "LEVEL_REQ", 0, 1, m_wLevelReq );

	//durations
	cFILE.getflag( "EVENT", "DURATION_REGISTER", 0, 1, m_wRegisterTime );
	cFILE.getflag( "EVENT", "DURATION_BATTLE", 0, 1, m_wBattleTime );
	cFILE.getflag( "EVENT", "DURATION_REWARD", 0, 1, m_wRewardTime );

	//tower
	cFILE.getflag( "EVENT", "TOWER_DAMAGE_CAPTURE", 0, 1, m_dwDamageCapture );
	cFILE.getflag( "EVENT", "TOWER_DAMAGE_BASE", 0, 1, m_dwDamageBase );

	//score logic
	cFILE.getflag( "EVENT", "VAR_DAMAGE", 0, 1, m_fDamageVar );
	cFILE.getflag( "EVENT", "VAR_KILL", 0, 1, m_fKillVar );
	cFILE.getflag( "EVENT", "VAR_HEAL", 0, 1, m_fHealVar );
	cFILE.getflag( "EVENT", "VAR_RESU", 0, 1, m_fResuVar );

	//point logic
	cFILE.getflag( "EVENT", "VAR_POINT", 0, 1, m_fPointVar );
	cFILE.getflag( "EVENT", "POINT_BASE_WIN", 0, 1, m_wBasePointWin );
	cFILE.getflag( "EVENT", "POINT_BASE_LOSS", 0, 1, m_wBasePointLoss );
	cFILE.getflag( "EVENT", "POINT_MAX", 0, 1, m_wMaxRewardPoint );

	//reward item
	cFILE.getflag( "EVENT", "REWARD_ITEM_WIN", 0, 2, m_sItemRewardWin.wMainID );
	cFILE.getflag( "EVENT", "REWARD_ITEM_WIN", 1, 2, m_sItemRewardWin.wSubID );
	cFILE.getflag( "EVENT", "REWARD_SCORE_WIN_LIMIT", 0, 1, m_wRewardLimitScoreWin );

	cFILE.getflag( "EVENT", "REWARD_ITEM_LOST", 0, 2, m_sItemRewardLost.wMainID );
	cFILE.getflag( "EVENT", "REWARD_ITEM_LOST", 1, 2, m_sItemRewardLost.wSubID );
	cFILE.getflag( "EVENT", "REWARD_SCORE_LOST_LIMIT", 0, 1, m_wRewardLimitScoreLost );

	//respawn time
	cFILE.getflag( "EVENT", "REBIRTH_TIME_BASE", 0, 1, m_wRebirthTimeBase );
	cFILE.getflag( "EVENT", "PERCHAR_REBIRTH_TIME_ADD", 0, 1, m_fPerCharRebirthTimeAdd );

	/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
	cFILE.getflag( "EVENT", "DISABLE_OTHER_SCHOOL_RESU", 0, 1, m_bDisableOtherSchoolResu );

	cFILE.getflag( "EVENT", "DISABLE_OTHER_GUILD_RESU", 0, 1, m_bDisableOtherGuildResu );

	/*PVPTyranny disable score self heal, EJCode, 2018/11/28 */
	cFILE.getflag( "EVENT", "DISABLE_SCORE_SELF_HEAL", 0, 1, m_bDisableScoreSelfHeal );

	/* PVPTyranny Hash Check, Juver, 2020/07/28 */
	cFILE.getflag( "EVENT", "HASH_CHECK", 0, 1, m_bHashCheck );

	cFILE.getflag( "EVENT", "NON_TOWER_END", 0, 1, m_bNonTowerEnd );

	for ( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
	{
		CString strtowerdata;
		strtowerdata.Format ( "TOWER_%02d", i );
		cFILE.getflag( "EVENT", strtowerdata.GetString(), 0, 2, m_sTowerData[i].sidCrow.wMainID );
		cFILE.getflag( "EVENT", strtowerdata.GetString(), 1, 2, m_sTowerData[i].sidCrow.wSubID );
	}

	for ( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
	{
		for ( int ii=0; ii<TYRANNY_TOWER_SIZE; ++ii )
		{
			if ( i == ii )	continue;

			if ( m_sTowerData[i].sidCrow == m_sTowerData[ii].sidCrow )
			{
				CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load duplicate id in index %d~%d ID[%d~%d]", i, ii, m_sTowerData[i].sidCrow.wMainID, m_sTowerData[i].sidCrow.wSubID );
			}
		}
	}

	if ( m_wPlayerLimit == 0 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid PlayerLimit:%u limit must be atleast 1", m_wPlayerLimit );
		m_wPlayerLimit = 1;
	}

	if ( m_wLevelReq == 0 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid LevelReq:%u req must be atleast 1", m_wLevelReq );
		m_wLevelReq = 1;
	}

	if ( m_wRegisterTime < 300 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid BattleTime time must be more than 300 sec:%u", m_wRegisterTime );
		m_wRegisterTime = 300;
	}

	if ( m_wBattleTime < 300 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid BattleTime time must be more than 300 sec:%u", m_wBattleTime );
		m_wBattleTime = 300;
	}

	if ( m_wRewardTime < 180 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid RewardTime time must be more than 300 sec:%u", m_wRewardTime );
		m_wRewardTime = 180;
	}

	if ( m_dwDamageCapture <= 0 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid m_dwDamageCapture must be more than 0 val:%u", m_dwDamageCapture );
		m_dwDamageCapture = 1;
	}

	if ( m_dwDamageBase <= 0 )
	{
		CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid m_dwDamageBase must be more than 0 val:%u", m_dwDamageBase );
		m_dwDamageBase = 1;
	}

	//schedule day-hour-minute
	m_vecSchedule.clear();
	m_mapBuffData.clear();
	DWORD dwnum = cFILE.GetKeySize( "EVENT", "BATTLE_TIME" );
	for( DWORD i=0; i<dwnum; ++i )
	{
		TYRANNY_SCHED sched;
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 0, 6, sched.wStartDay );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 1, 6, sched.wStartHour );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 2, 6, sched.wStartMinute );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 3, 6, sched.m_strBuffConfig );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 4, 6, sched.bUseRandomMap );
		cFILE.getflag( i, "EVENT", "BATTLE_TIME", 5, 6, sched.bCombatRecord );

		if ( sched.wStartDay < 1 || sched.wStartDay > 7 )
		{
			CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid startday:%u index:%d", sched.wStartDay, i );
			continue;
		}

		if ( sched.wStartHour >= 24 )
		{
			CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid StartHour:%u index:%d", sched.wStartHour, i );
			continue;
		}


		if ( sched.wStartMinute >= 60 )
		{
			CDebugSet::MsgBoxAfx( "GLPVPTyranny::Load invalid StartMinute:%u index:%d", sched.wStartMinute, i );
			continue;
		}

		//there should be no problem with this exist check right? ... right?
		TYRANNY_SCHED_BUFF_MAP_ITER it = m_mapBuffData.find( sched.m_strBuffConfig );
		if ( it == m_mapBuffData.end() )
		{
			TYRANNY_SCHED_BUFF sBuff;
			sBuff.m_strBuffConfig = sched.m_strBuffConfig;
			BOOL bLoad = sBuff.LoadBuff();
			if ( bLoad )
			{
				m_mapBuffData.insert( std::make_pair( sBuff.m_strBuffConfig, sBuff ) );
			}
		}
		
		m_vecSchedule.push_back( sched );
	}

	std::sort( m_vecSchedule.begin(), m_vecSchedule.end() );

	if ( m_vecSchedule.empty() )
	{
		CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyranny::Load no schedules loaded" );
	}

	if ( m_vecMapRandom.empty() )
	{
		if ( RPARAM::bTyrannyRandomMapLogs == 1 )
		{
			CDebugSet::ToFileWithTime( "_pvptyranny_random_map.txt", "no maps loaded!" );
		}
	}
	else
	{
		for ( size_t i=0; i<m_vecMapRandom.size(); ++i )
		{
			if ( RPARAM::bTyrannyRandomMapLogs == 1 )
			{
				CDebugSet::ToFileWithTime( "_pvptyranny_random_map.txt", "map:%u/%u", 
				m_vecMapRandom[i].wMainID, 
				m_vecMapRandom[i].wSubID );
			}
		}
	}


	return true;
}

void GLPVPTyranny::FindScheduleFirst()
{
	if ( m_vecSchedule.empty() )	return;

	m_sScheduleNext.dwIndex = TYRANNY_SCHEDULE_NONE;

	if ( m_vecSchedule.size() == 1 )
	{
		m_sScheduleNext.dwIndex = 0;

		TYRANNY_SCHED& sSchedule = m_vecSchedule[0];
		m_sScheduleNext.SetScheduleData( sSchedule );
		m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

		/*TYRANNY_SCHED_BUFF_MAP_ITER it = m_mapBuffData.find( sSchedule.m_strBuffConfig );
		if ( it != m_mapBuffData.end() )
		{
			TYRANNY_SCHED_BUFF& sBuff = (*it).second;
			m_sScheduleNext.SetBuffData( sBuff );
		}*/
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
				TYRANNY_SCHED& sSchedule = m_vecSchedule[i];
				m_sScheduleNext.SetScheduleData( sSchedule );
				m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

				/*TYRANNY_SCHED_BUFF_MAP_ITER it = m_mapBuffData.find( sSchedule.m_strBuffConfig );
				if ( it != m_mapBuffData.end() )
				{
					TYRANNY_SCHED_BUFF& sBuff = (*it).second;
					m_sScheduleNext.SetBuffData( sBuff );
				}*/
			}

			if ( m_sScheduleNext.dwIndex != TYRANNY_SCHEDULE_NONE )
				break;

			//week limit
			if ( nminutes >= 11000/*10080*/ )
				break;

			CTimeSpan span( 0, 0, 1, 0 );
			ctimecur += span;
			nminutes ++;
			Sleep(0);
		}

		if ( m_sScheduleNext.dwIndex == TYRANNY_SCHEDULE_NONE )
		{
			CDebugSet::ToFileWithTime( "_pvptyranny.txt", "FindScheduleFirst failed! no events will start" );
		}
		else
		{
			CDebugSet::ToFileWithTime( "_pvptyranny.txt", "FindScheduleFirst Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
				m_sScheduleNext.wStartDay, m_sScheduleNext.wStartHour, m_sScheduleNext.wStartMinute,
				m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );
		}
	}
};

bool GLPVPTyranny::FindScheduleNext()
{
	if ( m_sScheduleNext.dwIndex == TYRANNY_SCHEDULE_NONE )	return false;

	DWORD dwnewidx = m_sScheduleNext.dwIndex + 1;
	if ( dwnewidx >= (DWORD)m_vecSchedule.size() )	dwnewidx = 0;

	m_sScheduleNext.dwIndex = dwnewidx;

	TYRANNY_SCHED& sSchedule = m_vecSchedule[dwnewidx];

	m_sScheduleNext.SetScheduleData( sSchedule );
	m_sScheduleNext.CalculateEndTime( m_wRegisterTime, m_wBattleTime, m_wRewardTime );

	/*TYRANNY_SCHED_BUFF_MAP_ITER it = m_mapBuffData.find( sSchedule.m_strBuffConfig );
	if ( it != m_mapBuffData.end() )
	{
		TYRANNY_SCHED_BUFF& sBuff = (*it).second;
		m_sScheduleNext.SetBuffData( sBuff );
	}*/

	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "FindScheduleNext Index:%u Start(D:%u/H:%u/M:%u) BattleStart(H:%u/M:%u) BattleEnd(H:%u/M:%u)", m_sScheduleNext.dwIndex, 
		m_sScheduleNext.wStartDay, m_sScheduleNext.wStartHour, m_sScheduleNext.wStartMinute,
		m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute );

	return true;
};

BOOL GLPVPTyranny::SetContributionPointMultiplier( float fMulti )
{
	m_fContributionPointMultiplier = fMulti;
	return TRUE;
}

float GLPVPTyranny::GetContributionPointMultiplier()
{
	return m_fContributionPointMultiplier;
}

void GLPVPTyranny::BuffSelect()
{
	if ( m_sScheduleNext.dwIndex == TYRANNY_SCHEDULE_NONE )	return;
	if ( m_sScheduleNext.dwIndex >= m_vecSchedule.size() )	return;

	TYRANNY_SCHED& sSchedule = m_vecSchedule[m_sScheduleNext.dwIndex];

	TYRANNY_SCHED_BUFF_MAP_ITER it = m_mapBuffData.find( sSchedule.m_strBuffConfig );
	if ( it != m_mapBuffData.end() )
	{
		TYRANNY_SCHED_BUFF& sBuff = (*it).second;
		m_sScheduleNext.SetBuffData( sBuff );
	}
}

void GLPVPTyranny::BattleMapSelect()
{
	TYRANNY_SCHED& sSchedule = m_vecSchedule[m_sScheduleNext.dwIndex];
	if ( sSchedule.bUseRandomMap == FALSE )
	{
		//schedule is not random
		//just select the fixed map
		m_sScheduleNext.setBattleMap( m_sMapFixed );
		if ( RPARAM::bTyrannyRandomMapLogs == 1 )
		{
			CDebugSet::ToFileWithTime( "_pvptyranny_random_map.txt", "map use fixed Index:%u map:%u/%u", 
				m_sScheduleNext.dwIndex, 
				m_sScheduleNext.getBatteMap().wMainID, 
				m_sScheduleNext.getBatteMap().wSubID );
		}
	}
	else
	{
		//local copy if vector
		TYRANNY_MAP_VEC vecMap = m_vecMapRandom;
		if ( vecMap.empty() == false )
		{
			//random shuffle the maps
			std::random_shuffle( vecMap.begin(), vecMap.end() );

			const SNATIVEID& mapID = vecMap[0];

			//select the first data in vector
			m_sScheduleNext.setBattleMap( mapID );

			CDebugSet::ToFileWithTime( "_pvptyranny_random_map.txt", "map random Index:%u map:%u/%u", 
				m_sScheduleNext.dwIndex, 
				m_sScheduleNext.getBatteMap().wMainID, 
				m_sScheduleNext.getBatteMap().wSubID );
		}
		else
		{
			//random maps are empty!
			//this should not happen 
			//we handle this error by selecting the fixed map
			m_sScheduleNext.setBattleMap( m_sMapFixed );

			CDebugSet::ToFileWithTime( "_pvptyranny_random_map.txt", "random map error use fixed Index:%u map:%u/%u", 
				m_sScheduleNext.dwIndex, 
				m_sScheduleNext.getBatteMap().wMainID, 
				m_sScheduleNext.getBatteMap().wSubID );
		}
	}
}