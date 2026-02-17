#pragma once

#include <map>
#include <set>

#include "./GLCharDefine.h"
#include "../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLPVPWoeDefine.h"

struct WOE_SCHED
{  
	WORD	wStartDay;
	WORD	wStartHour;
	WORD	wStartMinute;

	WOE_SCHED()
		: wStartDay(0)
		, wStartHour(0)
		, wStartMinute(0)
	{
	}

	bool operator < ( const WOE_SCHED& sData )
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
};

struct WOE_SCHED_NEXT
{
	DWORD	dwIndex;
	WOE_SCHED	sStartSched;
	
	WORD		wPrevStartHour;
	WORD		wPrevStartMinute;
	WORD		wPrevEndHour;
	WORD		wPrevEndMinute;

	WORD		wBattleStartHour;
	WORD		wBattleStartMinute;
	WORD		wBattleEndHour;
	WORD		wBattleEndMinute;
	
	WOE_SCHED_NEXT()
		: dwIndex(UINT_MAX)
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

	void CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration )
	{
		wPrevStartHour = wBattleStartHour;
		wPrevStartMinute = wBattleStartMinute;
		wPrevEndHour = wBattleEndHour;
		wPrevEndMinute = wBattleEndMinute;

		wBattleStartHour = sStartSched.wStartHour;
		wBattleStartMinute = sStartSched.wStartMinute;
		wBattleEndHour = sStartSched.wStartHour;
		wBattleEndMinute = sStartSched.wStartMinute;

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
};

struct WOE_REGISTER_DATA
{  
	DWORD	dwInfoCharID;
	char	szInfoCharName[WOE_CHARNAME_STRING_SIZE];
	WORD	wInfoSchool;
	WORD	wInfoClass;

	WOE_REGISTER_DATA()
		: dwInfoCharID(WOE_PLAYER_NULL)
		, wInfoSchool(0)
		, wInfoClass(0)
	{
		memset( szInfoCharName, 0, sizeof(char) * (WOE_CHARNAME_STRING_SIZE) );
	}

	WOE_REGISTER_DATA ( const WOE_REGISTER_DATA &value )
	{
		operator=(value);
	}

	WOE_REGISTER_DATA& operator = ( const WOE_REGISTER_DATA& rvalue )
	{
		dwInfoCharID	= rvalue.dwInfoCharID;
		wInfoSchool		= rvalue.wInfoSchool;
		wInfoClass		= rvalue.wInfoClass;

		StringCchCopy( szInfoCharName, WOE_CHARNAME_STRING_SIZE, rvalue.szInfoCharName );

		return *this;
	}
};

struct WOE_PLAYER_DATA
{  
	DWORD	dwInfoCharID;
	char	szInfoCharName[WOE_CHARNAME_STRING_SIZE];
	WORD	wInfoSchool;
	WORD	wInfoClass;
	WORD	wRewardPoint;

	WOE_PLAYER_DATA()
		: dwInfoCharID(WOE_PLAYER_NULL)
		, wInfoSchool(0)
		, wInfoClass(0)
		, wRewardPoint(0)
	{
		memset( szInfoCharName, 0, sizeof(char) * (WOE_CHARNAME_STRING_SIZE) );
	}

	bool operator < ( const WOE_PLAYER_DATA& sData )
	{			
		return  false;
	}

	WOE_PLAYER_DATA ( const WOE_PLAYER_DATA &value )
	{
		operator=(value);
	}

	WOE_PLAYER_DATA& operator = ( const WOE_PLAYER_DATA& rvalue )
	{
		dwInfoCharID	= rvalue.dwInfoCharID;
		wInfoSchool		= rvalue.wInfoSchool;
		wInfoClass		= rvalue.wInfoClass;
		wRewardPoint	= rvalue.wRewardPoint;
		
		StringCchCopy( szInfoCharName, WOE_CHARNAME_STRING_SIZE, rvalue.szInfoCharName );

		return *this;
	}
};

struct WOE_TOWER_DATA
{ 
	SNATIVEID	sidCrow;
	DWORD		dwGuildOwner;

	WOE_TOWER_DATA()
		: sidCrow( NATIVEID_NULL().dwID)
		, dwGuildOwner(CLUB_NULL)
	{
	}
};

struct WOE_SCORE_DATA
{ 
	WORD	wSchool;
	WORD	wCapture;
	WORD	wKills;
	WORD	wDeaths;

	WOE_SCORE_DATA()
		: wSchool(0)
		, wCapture(0)
		, wKills(0)
		, wDeaths(0)
	{
	}

	bool operator < ( const WOE_SCORE_DATA& sData )
	{			
		if ( wCapture > sData.wCapture )
			return true;
		else if ( wCapture == sData.wCapture && wKills > sData.wKills )	
			return true;
		else if ( wCapture == sData.wCapture && wKills == sData.wKills && wDeaths < sData.wDeaths )	
			return true;

		return  false;
	}

	void Reset()
	{
		wCapture = 0;
		wKills = 0;
		wDeaths = 0;
	}
};

struct WOE_REWARD_DATA
{ 
	DWORD		dwCharID;
	SNATIVEID	sItemReward;
	bool		bSent;

	WOE_REWARD_DATA()
		: dwCharID(WOE_PLAYER_NULL)
		, bSent( FALSE )
	{
	};
};

struct SWOE_RANK_EX
{
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szClubName[CLUB_NAME + 1];
	char	szLeaderName[CHAR_SZNAME + 1];

	SWOE_RANK_EX()
		: dwClubID(0)
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
	{
		memset(szClubName, 0, sizeof(char) * (CLUB_NAME + 1));
		memset(szLeaderName, 0, sizeof(char) * (CHAR_SZNAME + 1));
	}

	void Init()
	{
		dwClubID = 0;
		wClubRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;

		StringCchCopy(szClubName, CLUB_NAME + 1, "");
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, "");
	}

	SWOE_RANK_EX& operator = (const SWOE_RANK_EX& rvalue)
	{
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		return *this;
	}

};

struct SWOE_RANK_INFO
{
	int		nIndex;
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szClubName[CLUB_NAME + 1];
	char	szLeaderName[CHAR_SZNAME + 1];

	SWOE_RANK_INFO()
		: nIndex(-1)
		, dwClubID(CLUB_NULL)
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
	{
		memset(szClubName, 0, sizeof(char) * (CLUB_NAME + 1));
		memset(szLeaderName, 0, sizeof(char) * (CHAR_SZNAME + 1));
	}

	SWOE_RANK_INFO(const SWOE_RANK_INFO& value)
	{
		operator=(value);
	}

	SWOE_RANK_INFO& operator = (const SWOE_RANK_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		return *this;
	}

	bool operator < (const SWOE_RANK_INFO& sCWRank)
	{
		if (wKillNum > sCWRank.wKillNum) return true;
		else if (wKillNum == sCWRank.wKillNum && wDeathNum < sCWRank.wDeathNum) return true;

		return  false;
	}

	bool operator == (const SWOE_RANK_INFO& sCWRank)
	{
		if (wKillNum == sCWRank.wKillNum && wDeathNum == sCWRank.wDeathNum) return true;

		return false;

	}
};

struct SWOE_RANK
{
	int		nIndex;
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;

	char	szClubName[CLUB_NAME + 1];
	char	szLeaderName[CHAR_SZNAME + 1];

	SWOE_RANK()
		: nIndex(-1)
		, dwClubID(CLUB_NULL)
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
	{
		memset(szClubName, 0, sizeof(char) * (CLUB_NAME + 1));
		memset(szLeaderName, 0, sizeof(char) * (CHAR_SZNAME + 1));
	}

	SWOE_RANK(const SWOE_RANK& value)
	{
		operator=(value);
	}

	SWOE_RANK& operator = (const SWOE_RANK& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		return *this;
	}

	SWOE_RANK(const SWOE_RANK_INFO& value)
	{
		operator=(value);
	}

	SWOE_RANK& operator = (const SWOE_RANK_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		
		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		return *this;
	}
};

typedef std::vector<WOE_SCHED>				WOE_SCHED_VEC;

typedef std::map<DWORD,WOE_REGISTER_DATA>		WOE_REGISTER_DATA_MAP;
typedef WOE_REGISTER_DATA_MAP::iterator			WOE_REGISTER_DATA_MAP_ITER;
typedef std::vector<WOE_REGISTER_DATA>			WOE_REGISTER_DATA_VEC;

typedef std::map<DWORD,WOE_PLAYER_DATA>		WOE_PLAYER_DATA_MAP;
typedef WOE_PLAYER_DATA_MAP::iterator		WOE_PLAYER_DATA_MAP_ITER;
typedef std::vector<WOE_PLAYER_DATA>		WOE_PLAYER_DATA_VEC;

typedef std::vector<WOE_SCORE_DATA>			WOE_SCORE_DATA_VEC;
typedef std::map<DWORD,WOE_REWARD_DATA>		WOE_REWARD_MAP;
typedef WOE_REWARD_MAP::iterator			WOE_REWARD_MAP_ITER;