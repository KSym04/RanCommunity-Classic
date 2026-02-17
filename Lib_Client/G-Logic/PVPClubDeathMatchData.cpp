#include "stdafx.h"
#include "./PVPClubDeathMatchData.h"

#include "./GLCharDefine.h"

#include "../../Lib_Helper/CommonContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void PVP_CLUB_DEATH_MATCH_EVENT_SCHED::Reset()
{
	wStartDay		= 0;
	wStartHour		= 0;
	wStartMinute	= 0;
}

void PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT::Reset()
{
	dwIndex = PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE;

	sStartSched.Reset();

	wPrevStartHour		= 0;
	wPrevStartMinute	= 0;
	wPrevEndHour		= 0;
	wPrevEndMinute		= 0;

	wBattleStartHour	= 0;
	wBattleStartMinute	= 0;
	wBattleEndHour		= 0;
	wBattleEndMinute	= 0;
}

void PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT::CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration )
{
	wPrevStartHour = wBattleStartHour;
	wPrevStartMinute = wBattleStartMinute;
	wPrevEndHour = wBattleEndHour;
	wPrevEndMinute = wBattleEndMinute;

	wBattleStartHour = sStartSched.wStartHour;
	wBattleStartMinute = sStartSched.wStartMinute;
	wBattleEndHour = sStartSched.wStartHour;
	wBattleEndMinute = sStartSched.wStartMinute;

	WORD wToBattleInterval = wRegisterDuration/60;
	WORD wOverAllInterval = ( wRegisterDuration + wBattleDuration + wRewardDuration )/60;

	wBattleStartMinute += wToBattleInterval; 
	wBattleEndMinute += wOverAllInterval;

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

void PVP_CLUB_DEATH_MATCH_CLUB_DATA::CleanUp()
{
	dwClubID = PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL;

	StringCchCopy( szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, "" );
	StringCchCopy( szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, "" );

	mapPlayerData.clear();

	wRank = PVP_CLUB_DEATH_MATCH_RANK_NONE;
	wPoints = 0;
	wKills = 0;
	wDeaths = 0;
	wResu = 0;
}

BOOL PVP_CLUB_DEATH_MATCH_CLUB_DATA::isPlayerReg( DWORD dwCharID )
{
	if ( dwCharID == PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL )	return FALSE;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER iterdata = mapPlayerData.find( dwCharID );
	if( iterdata != mapPlayerData.end() )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL PVP_CLUB_DEATH_MATCH_CLUB_DATA::isPlayerFull( WORD wMax )
{
	//unlimited team members
	if ( wMax == 0 )	return FALSE;

	if ( mapPlayerData.size() >= wMax )
		return TRUE;

	return FALSE;
}

BOOL PVP_CLUB_DEATH_MATCH_CLUB_DATA::isPlayerEnough( WORD wMin, WORD wMax )
{
	if ( GetPlayerNum() < wMin )	return FALSE;

	return TRUE;
}


BOOL PVP_CLUB_DEATH_MATCH_CLUB_DATA::InsertRegister( DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool )
{
	if ( !szName )											return FALSE;
	if ( dwCharID == PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL )	return FALSE;
	if ( wClass >= GLCI_NUM_8CLASS )						return FALSE;
	if ( wSchool >= PVP_CLUB_DEATH_MATCH_SCHOOL_SIZE )		return FALSE;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA sdata;
	sdata.dwCharID = dwCharID;
	sdata.wCharClass = wClass;
	sdata.wCharSchool = wSchool;
	StringCchCopy( sdata.szCharName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, szName );
	mapPlayerData.insert( std::make_pair( sdata.dwCharID, sdata ) );

	return TRUE;
}

BOOL PVP_CLUB_DEATH_MATCH_CLUB_DATA::DeleteRegister( DWORD dwCharID )
{
	if ( dwCharID == PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL )	return FALSE;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER iterdata = mapPlayerData.find( dwCharID );
	if( iterdata != mapPlayerData.end() )
	{
		mapPlayerData.erase( iterdata );
		return TRUE;
	}

	return FALSE;
}

WORD PVP_CLUB_DEATH_MATCH_CLUB_DATA::GetPlayerNum()
{
	return mapPlayerData.size();
}

PVP_CLUB_DEATH_MATCH_PLAYER_DATA* PVP_CLUB_DEATH_MATCH_CLUB_DATA::PlayerDataGet( DWORD dwCharID )
{
	if ( dwCharID == PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL ) return NULL;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it = mapPlayerData.find( dwCharID );
	if( it != mapPlayerData.end() )
	{
		return &(*it).second;
	}

	return NULL;
};
