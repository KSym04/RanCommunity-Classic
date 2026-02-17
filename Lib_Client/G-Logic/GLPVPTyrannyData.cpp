#include "stdafx.h"

#include "../../Lib_Engine/Common/gltexfile.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"

#include "./GLPVPTyrannyData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL TYRANNY_SCHED_BUFF::LoadBuff()
{
	if( m_strBuffConfig.empty() )	return FALSE;

	std::string strpath;
	strpath = GLOGIC::GetServerPath ();
	strpath += m_strBuffConfig;

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if( !cFILE.open ( strpath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : TYRANNY_SCHED_BUFF::LoadBuff(), File Open %s", m_strBuffConfig.c_str() );
		return FALSE;
	}

	//buff battle
	cFILE.getflag( "CONFIG", "BUFF_BATTLE_USE", 0, 1, m_bBattleBuff );

	m_vecBuffBattle.clear();
	DWORD dwBuffBattleNum = cFILE.GetKeySize( "CONFIG", "BUFF_BATTLE" );
	for( DWORD i=0; i<dwBuffBattleNum; ++i )
	{
		TYRANNY_BUFF_BATTLE sBuffBattle;
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 0, 12, sBuffBattle.sidSkill.wMainID );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 1, 12, sBuffBattle.sidSkill.wSubID );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 2, 12, sBuffBattle.wLevel[0] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 3, 12, sBuffBattle.wLevel[1] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 4, 12, sBuffBattle.wLevel[2] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 5, 12, sBuffBattle.wLevel[3] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 6, 12, sBuffBattle.wLevel[4] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 7, 12, sBuffBattle.wLevel[5] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 8, 12, sBuffBattle.wLevel[6] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 9, 12, sBuffBattle.wLevel[7] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 10, 12, sBuffBattle.wLevel[8] );
		cFILE.getflag( i, "CONFIG", "BUFF_BATTLE", 11, 12, sBuffBattle.wLevel[9] );

		m_vecBuffBattle.push_back( sBuffBattle );
	}

	//buff reward
	cFILE.getflag( "CONFIG", "BUFF_REWARD_USE", 0, 1, m_bRewardBuff );

	cFILE.getflag( "CONFIG", "BUFF_REWARD_MAIN", 0, 3, m_sRewardBuffMain.sidSkill.wMainID );
	cFILE.getflag( "CONFIG", "BUFF_REWARD_MAIN", 1, 3, m_sRewardBuffMain.sidSkill.wSubID );
	cFILE.getflag( "CONFIG", "BUFF_REWARD_MAIN", 2, 3, m_sRewardBuffMain.wLevel );

	m_vecBuffReward.clear();
	DWORD dwBuffRewardNum = cFILE.GetKeySize( "CONFIG", "BUFF_REWARD" );
	for( DWORD i=0; i<dwBuffRewardNum; ++i )
	{
		TYRANNY_BUFF_REWARD sBuffReward;
		cFILE.getflag( i, "CONFIG", "BUFF_REWARD", 0, 3, sBuffReward.sidSkill.wMainID );
		cFILE.getflag( i, "CONFIG", "BUFF_REWARD", 1, 3, sBuffReward.sidSkill.wSubID );
		cFILE.getflag( i, "CONFIG", "BUFF_REWARD", 2, 3, sBuffReward.wLevel );

		m_vecBuffReward.push_back( sBuffReward );
	}

	return TRUE;
}

void TYRANNY_SCHED_NEXT::SetScheduleData( TYRANNY_SCHED& sSchedule )
{
	wStartDay			= sSchedule.wStartDay;
	wStartHour			= sSchedule.wStartHour;
	wStartMinute		= sSchedule.wStartMinute;

	bCombatRecord = sSchedule.bCombatRecord;
}

void TYRANNY_SCHED_NEXT::SetBuffData( TYRANNY_SCHED_BUFF& sBuff )
{
	//reset buff data
	m_bRewardBuff		= FALSE;
	m_bBattleBuff		= FALSE;
	m_sRewardBuffMain	= TYRANNY_BUFF_REWARD();

	for( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
		m_sRewardBuff[i] = TYRANNY_BUFF_REWARD();

	for( int i=0; i<TYRANNY_BATTLE_BUFF_NUM; ++i )
		m_sBattleBuff[i] = TYRANNY_BUFF_BATTLE();


	//CDebugSet::ToFileWithTime( "_pvptyranny_buff.txt", "get buff data from %s", sBuff.m_strBuffConfig.c_str() );

	//copy buff data
	m_bRewardBuff		= sBuff.m_bRewardBuff;
	m_bBattleBuff		= sBuff.m_bBattleBuff;

	m_sRewardBuffMain	= sBuff.m_sRewardBuffMain;

	/*CDebugSet::ToFileWithTime( "_pvptyranny_buff.txt", "main reward buff MID:%u SID:%u LVL:%u", 
		m_sRewardBuffMain.sidSkill.wMainID, 
		m_sRewardBuffMain.sidSkill.wSubID,
		m_sRewardBuffMain.wLevel );*/

	//now randomize buff

	

	if ( sBuff.m_vecBuffReward.size() )
	{
		//randomize buff
		srand(unsigned(time(NULL)));
		std::random_shuffle(sBuff.m_vecBuffReward.begin(), sBuff.m_vecBuffReward.end());

		for( int i=0; i<TYRANNY_REWARD_BUFF_NUM && i<(int)sBuff.m_vecBuffReward.size(); ++i )
		{
			m_sRewardBuff[i] = sBuff.m_vecBuffReward[i];
			/*CDebugSet::ToFileWithTime( "_pvptyranny_buff.txt", "reward buff MID:%u SID:%u LVL:%u", 
				m_sRewardBuff[i].sidSkill.wMainID, 
				m_sRewardBuff[i].sidSkill.wSubID,
				m_sRewardBuff[i].wLevel );*/
		}
	}

	if ( sBuff.m_vecBuffBattle.size() )
	{
		//randomize buff
		srand(unsigned(time(NULL)));
		std::random_shuffle(sBuff.m_vecBuffBattle.begin(), sBuff.m_vecBuffBattle.end());

		for( int i=0; i<TYRANNY_BATTLE_BUFF_NUM && i<(int)sBuff.m_vecBuffBattle.size(); ++i )
		{
			m_sBattleBuff[i] = sBuff.m_vecBuffBattle[i];
			/*CDebugSet::ToFileWithTime( "_pvptyranny_buff.txt", "battle buff MID:%u SID:%u LVL:%u %u %u %u %u %u %u %u %u %u", 
				m_sBattleBuff[i].sidSkill.wMainID, 
				m_sBattleBuff[i].sidSkill.wSubID, 
				m_sBattleBuff[i].wLevel[0], 
				m_sBattleBuff[i].wLevel[1],
				m_sBattleBuff[i].wLevel[2],
				m_sBattleBuff[i].wLevel[3],
				m_sBattleBuff[i].wLevel[4],
				m_sBattleBuff[i].wLevel[5],
				m_sBattleBuff[i].wLevel[6],
				m_sBattleBuff[i].wLevel[7],
				m_sBattleBuff[i].wLevel[8],
				m_sBattleBuff[i].wLevel[9] );*/
		}
	}
}


/*
void TYRANNY_SCHED_NEXT::SetSchedule( TYRANNY_SCHED& sSchedule )
{
	wStartDay			= sSchedule.wStartDay;
	wStartHour			= sSchedule.wStartHour;
	wStartMinute		= sSchedule.wStartMinute;

	
	


	


	
}

*/
void TYRANNY_SCHED_NEXT::CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration )
{
	wPrevStartHour = wBattleStartHour;
	wPrevStartMinute = wBattleStartMinute;
	wPrevEndHour = wBattleEndHour;
	wPrevEndMinute = wBattleEndMinute;

	wBattleStartHour = wStartHour;
	wBattleStartMinute = wStartMinute;
	wBattleEndHour = wStartHour;
	wBattleEndMinute = wStartMinute;

	WORD wtobattleinterval = wRegisterDuration/60;
	WORD woverallinterval = ( wRegisterDuration + wBattleDuration + wRewardDuration )/60;

	wBattleStartMinute += wtobattleinterval; 
	wBattleEndMinute += woverallinterval;

	while ( wBattleStartMinute >= 60 )
	{
		wBattleStartHour += 1;
		wBattleStartMinute -= 60;
	}

	while ( wBattleStartHour >= 24 )
	{
		wBattleStartHour -= 24;
	}

	while ( wBattleEndMinute >= 60 )
	{
		wBattleEndHour += 1;
		wBattleEndMinute -= 60;
	}

	while ( wBattleEndHour >= 24 )
	{
		wBattleEndHour -= 24;
	}
}
