/*!
 * \file PVPClubDeathMatchData.h
 *
 * \author Juver
 * \date 2019/09/15
 *
 * 
 */

#ifndef PVPCLUBDEATHMATCHDATA_H_INCLUDED__
#define PVPCLUBDEATHMATCHDATA_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./PVPClubDeathMatchDefine.h"

struct PVP_CLUB_DEATH_MATCH_EVENT_SCHED
{  
	WORD	wStartDay;		//schedule day
	WORD	wStartHour;		//schedule hour (24 hour format)
	WORD	wStartMinute;	//schedule minute

	BOOL	bCombatRecord;

	PVP_CLUB_DEATH_MATCH_EVENT_SCHED()
		: wStartDay(0)
		, wStartHour(0)
		, wStartMinute(0)
		, bCombatRecord(FALSE)
	{
	}

	bool operator < ( const PVP_CLUB_DEATH_MATCH_EVENT_SCHED& sData )
	{			
		if ( wStartDay < sData.wStartDay ) 
			return true;

		else if ( wStartDay == sData.wStartDay && 
			wStartHour < sData.wStartHour ) 
			return true;

		else if ( wStartDay == sData.wStartDay && 
			wStartHour == sData.wStartHour &&
			wStartMinute < sData.wStartMinute ) 
			return true;

		return  false;
	}

	void Reset();
};

typedef std::vector<PVP_CLUB_DEATH_MATCH_EVENT_SCHED>				PVP_CLUB_DEATH_MATCH_EVENT_SCHED_VEC;

struct PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT
{
	DWORD		dwIndex;

	PVP_CLUB_DEATH_MATCH_EVENT_SCHED	sStartSched;

	WORD		wPrevStartHour;
	WORD		wPrevStartMinute;
	WORD		wPrevEndHour;
	WORD		wPrevEndMinute;

	WORD		wBattleStartHour;
	WORD		wBattleStartMinute;
	WORD		wBattleEndHour;
	WORD		wBattleEndMinute;

	PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT()
		: dwIndex(PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE)
		, wPrevStartHour(0)
		, wPrevStartMinute(0)
		, wPrevEndHour(0)
		, wPrevEndMinute(0)
		, wBattleStartHour(0)
		, wBattleStartMinute(0)
		, wBattleEndHour(0)
		, wBattleEndMinute(0)
	{
	};

	void Reset();
	void CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration );
};

struct PVP_CLUB_DEATH_MATCH_PLAYER_DATA
{
	DWORD dwCharID;
	TCHAR szCharName[PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE];
	WORD wCharSchool;
	WORD wCharClass;
	float fForceMoveLobbyAreaTime;
	float fForceMoveBattleAreaTime;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA()
		: dwCharID(PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL)
		, wCharSchool(0)
		, wCharClass(0)
		, fForceMoveLobbyAreaTime(0.0f)
		, fForceMoveBattleAreaTime(0.0f)
	{
		memset( szCharName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE) );
	};
};

typedef std::map<DWORD,PVP_CLUB_DEATH_MATCH_PLAYER_DATA>	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP;
typedef PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP::iterator		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER;
typedef std::vector<PVP_CLUB_DEATH_MATCH_PLAYER_DATA>		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_VEC;

struct PVP_CLUB_DEATH_MATCH_REGISTER_DATA
{  
	DWORD dwCharID;
	TCHAR szCharName[PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE];
	WORD wCharSchool;
	WORD wCharClass;

	PVP_CLUB_DEATH_MATCH_REGISTER_DATA()
		: dwCharID(PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL)
		, wCharSchool(0)
		, wCharClass(0)
	{
		memset( szCharName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE) );
	}

	PVP_CLUB_DEATH_MATCH_REGISTER_DATA ( const PVP_CLUB_DEATH_MATCH_PLAYER_DATA &value )
	{
		operator=(value);
	}

	PVP_CLUB_DEATH_MATCH_REGISTER_DATA& operator = ( const PVP_CLUB_DEATH_MATCH_PLAYER_DATA& rvalue )
	{
		dwCharID		= rvalue.dwCharID;
		wCharSchool		= rvalue.wCharSchool;
		wCharClass		= rvalue.wCharClass;

		StringCchCopy( szCharName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, rvalue.szCharName );

		return *this;
	}
};

struct PVP_CLUB_DEATH_MATCH_CLUB_DATA
{
	DWORD	dwClubID;
	TCHAR	szClubName[PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE];
	TCHAR	szMasterName[PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE];
	DWORD	dwCLUB_MARK_VER;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP		mapPlayerData;

	WORD	wRank;
	WORD	wPoints;
	WORD	wKills;
	WORD	wDeaths;
	WORD	wResu;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA()
		: dwClubID(PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL)
		, dwCLUB_MARK_VER(0)
		, wPoints(0)
		, wRank(PVP_CLUB_DEATH_MATCH_RANK_NONE)
		, wKills(0)
		, wDeaths(0)
		, wResu(0)
	{
		memset( szClubName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE) );
		memset( szMasterName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE) );
	};

	void CleanUp();

	BOOL isPlayerReg( DWORD dwCharID );
	BOOL isPlayerFull( WORD wMax );
	BOOL isPlayerEnough( WORD wMin, WORD wMax );
	
	BOOL InsertRegister( DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool );
	BOOL DeleteRegister( DWORD dwCharID );

	WORD GetPlayerNum();

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA*			PlayerDataGet( DWORD dwCharID );
};

typedef std::map<DWORD, PVP_CLUB_DEATH_MATCH_CLUB_DATA>		PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP;
typedef PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP::iterator		PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER;
typedef std::vector<PVP_CLUB_DEATH_MATCH_CLUB_DATA>			PVP_CLUB_DEATH_MATCH_CLUB_DATA_VEC;

struct PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK
{
	DWORD	dwClubID;
	TCHAR	szClubName[PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE];
	TCHAR	szMasterName[PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE];
	DWORD	dwCLUB_MARK_VER;

	WORD	wRank;
	WORD	wPoints;
	WORD	wKills;
	WORD	wDeaths;
	WORD	wResu;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK()
		: dwClubID(PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL)
		, dwCLUB_MARK_VER(0)
		, wPoints(0)
		, wRank(PVP_CLUB_DEATH_MATCH_RANK_NONE)
		, wKills(0)
		, wDeaths(0)
		, wResu(0)
	{
		memset( szClubName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE) );
		memset( szMasterName, 0, sizeof(TCHAR) * (PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE) );
	};

	bool operator < ( const PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& sData )
	{			
		if ( wPoints > sData.wPoints )
			return true;
		else if ( wPoints == sData.wPoints && wKills > sData.wKills )	
			return true;
		else if ( wPoints == sData.wPoints && wKills == sData.wKills && wDeaths < sData.wDeaths )	
			return true;

		return  false;
	}

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK ( const PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK &value )
	{
		operator=(value);
	}

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK ( const PVP_CLUB_DEATH_MATCH_CLUB_DATA &value )
	{
		operator=(value);
	}

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& operator = ( const PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& rvalue )
	{
		dwClubID	= rvalue.dwClubID;

		StringCchCopy( szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, rvalue.szClubName );
		StringCchCopy( szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, rvalue.szMasterName );
		dwCLUB_MARK_VER	= rvalue.dwCLUB_MARK_VER;

		wRank		= rvalue.wRank;
		wPoints		= rvalue.wPoints;
		wKills		= rvalue.wKills;
		wDeaths		= rvalue.wDeaths;
		wResu		= rvalue.wResu;

		return *this;
	}

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& operator = ( const PVP_CLUB_DEATH_MATCH_CLUB_DATA& rvalue )
	{
		dwClubID	= rvalue.dwClubID;

		StringCchCopy( szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, rvalue.szClubName );
		StringCchCopy( szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, rvalue.szMasterName );
		dwCLUB_MARK_VER	= rvalue.dwCLUB_MARK_VER;

		wRank		= rvalue.wRank;
		wPoints		= rvalue.wPoints;
		wKills		= rvalue.wKills;
		wDeaths		= rvalue.wDeaths;
		wResu		= rvalue.wResu;

		return *this;
	}
};

typedef std::vector<PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK>			PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK_VEC;


struct PVP_CLUB_DEATH_MATCH_REWARD_DATA
{
	SNATIVEID sItemID;
	WORD wContributionPoint;

	PVP_CLUB_DEATH_MATCH_REWARD_DATA()
		: sItemID(false)
		, wContributionPoint(0)
	{

	}
};

/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
struct PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA
{
	DWORD dwCharID;
	std::string strHash;

	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA()
		: dwCharID(PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL)
		, strHash("")
	{
	}
};

typedef std::vector<PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA>				PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_VEC;
typedef std::map<std::string, PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA>	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP;
typedef PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP::iterator				PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP_ITER;



#endif // PVPCLUBDEATHMATCHDATA_H_INCLUDED__
