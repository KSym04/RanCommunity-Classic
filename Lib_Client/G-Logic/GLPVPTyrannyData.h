/*!
 * \file GLPVPTyrannyData.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma once

#include <map>
#include <set>

#include "./GLCharDefine.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLPVPTyrannyDefine.h"

struct TYRANNY_SCHED
{  
	WORD	wStartDay;		//schedule day
	WORD	wStartHour;		//schedule hour (24 hour format)
	WORD	wStartMinute;	//schedule minute

	std::string m_strBuffConfig;
	BOOL bUseRandomMap;
	BOOL bCombatRecord;

	TYRANNY_SCHED()
		: wStartDay(0)
		, wStartHour(0)
		, wStartMinute(0)

		, m_strBuffConfig("")
		, bUseRandomMap(FALSE)
		, bCombatRecord(FALSE)
		
	{
	}

	bool operator < ( const TYRANNY_SCHED& sData )
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

	TYRANNY_SCHED& operator = ( const TYRANNY_SCHED& rvalue )
	{
		wStartDay				= rvalue.wStartDay;
		wStartHour				= rvalue.wStartHour;
		wStartMinute			= rvalue.wStartMinute;

		m_strBuffConfig			= rvalue.m_strBuffConfig;

		bUseRandomMap			= rvalue.bUseRandomMap;

		return *this;
	}
};

struct TYRANNY_BUFF_REWARD
{ 
	SNATIVEID	sidSkill;
	WORD		wLevel;

	TYRANNY_BUFF_REWARD()
		: sidSkill(NATIVEID_NULL())
		, wLevel(0)
	{
	};
};

struct TYRANNY_BUFF_BATTLE
{ 
	SNATIVEID	sidSkill;
	WORD		wLevel[TYRANNY_BATTLE_BUFF_LEVEL_NUM];

	TYRANNY_BUFF_BATTLE()
		: sidSkill(NATIVEID_NULL())
	{
		for( int i=0; i<TYRANNY_BATTLE_BUFF_LEVEL_NUM; ++i )
			wLevel[i] = 0;
	};
};

typedef std::vector<TYRANNY_BUFF_REWARD>			TYRANNY_BUFF_REWARD_VEC;
typedef std::vector<TYRANNY_BUFF_BATTLE>			TYRANNY_BUFF_BATTLE_VEC;


struct TYRANNY_SCHED_BUFF
{  
	std::string				m_strBuffConfig;

	BOOL					m_bBattleBuff;
	TYRANNY_BUFF_BATTLE_VEC m_vecBuffBattle;

	BOOL					m_bRewardBuff;
	TYRANNY_BUFF_REWARD_VEC m_vecBuffReward;
	TYRANNY_BUFF_REWARD		m_sRewardBuffMain;

	TYRANNY_SCHED_BUFF()
		: m_strBuffConfig("")
		, m_bBattleBuff(FALSE)
		, m_bRewardBuff(FALSE)
	{

	}

	TYRANNY_SCHED_BUFF& operator = ( const TYRANNY_SCHED_BUFF& rvalue )
	{
		m_strBuffConfig			= rvalue.m_strBuffConfig;

		m_bBattleBuff			= rvalue.m_bBattleBuff;
		m_vecBuffBattle			= rvalue.m_vecBuffBattle;

		m_bRewardBuff			= rvalue.m_bRewardBuff;
		m_vecBuffReward			= rvalue.m_vecBuffReward;

		m_sRewardBuffMain		= rvalue.m_sRewardBuffMain;

		return *this;
	}

	BOOL LoadBuff();
};

typedef std::map< std::string, TYRANNY_SCHED_BUFF>			TYRANNY_SCHED_BUFF_MAP;
typedef TYRANNY_SCHED_BUFF_MAP::iterator					TYRANNY_SCHED_BUFF_MAP_ITER;

struct TYRANNY_SCHED_NEXT
{
	DWORD		dwIndex;
	
	WORD		wStartDay;		//schedule day
	WORD		wStartHour;		//schedule hour (24 hour format)
	WORD		wStartMinute;	//schedule minute
	
	WORD		wPrevStartHour;
	WORD		wPrevStartMinute;
	WORD		wPrevEndHour;
	WORD		wPrevEndMinute;

	WORD		wBattleStartHour;
	WORD		wBattleStartMinute;
	WORD		wBattleEndHour;
	WORD		wBattleEndMinute;

	BOOL					m_bRewardBuff;
	BOOL					m_bBattleBuff;
	TYRANNY_BUFF_REWARD		m_sRewardBuffMain;
	TYRANNY_BUFF_REWARD		m_sRewardBuff[TYRANNY_REWARD_BUFF_NUM];
	TYRANNY_BUFF_BATTLE		m_sBattleBuff[TYRANNY_BATTLE_BUFF_NUM];
	SNATIVEID	m_sBattleMap;

	BOOL	bCombatRecord;

	TYRANNY_SCHED_NEXT()
		: dwIndex(UINT_MAX)

		, wStartDay(0)
		, wStartHour(0)
		, wStartMinute(0)

		, wPrevStartHour(0)
		, wPrevStartMinute(0)
		, wPrevEndHour(0)
		, wPrevEndMinute(0)
		, wBattleStartHour(0)
		, wBattleStartMinute(0)
		, wBattleEndHour(0)
		, wBattleEndMinute(0)

		//buff data
		, m_bRewardBuff(FALSE)
		, m_bBattleBuff(FALSE)

		, m_sBattleMap(false)
		, bCombatRecord(FALSE)
	{
		m_sRewardBuffMain = TYRANNY_BUFF_REWARD();

		for( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
			m_sRewardBuff[i] = TYRANNY_BUFF_REWARD();

		for( int i=0; i<TYRANNY_BATTLE_BUFF_NUM; ++i )
			m_sBattleBuff[i] = TYRANNY_BUFF_BATTLE();
	};

	void SetScheduleData( TYRANNY_SCHED& sSchedule );
	void SetBuffData( TYRANNY_SCHED_BUFF& sBuff );
	void CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration );
	void setBattleMap( const SNATIVEID& sMapID ) { m_sBattleMap = sMapID; }
	SNATIVEID& getBatteMap() { return m_sBattleMap; }
	
};

struct TYRANNY_REGISTER_DATA
{  
	DWORD	dwInfoCharID;
	char	szInfoCharName[TYRANNY_CHARNAME_STRING_SIZE];
	WORD	wInfoSchool;
	WORD	wInfoClass;
	BOOL	bLateJoin;

	TYRANNY_REGISTER_DATA()
		: dwInfoCharID(TYRANNY_PLAYER_NULL)
		, wInfoSchool(0)
		, wInfoClass(0)
		, bLateJoin(FALSE)
	{
		memset( szInfoCharName, 0, sizeof(char) * (TYRANNY_CHARNAME_STRING_SIZE) );
	}

	TYRANNY_REGISTER_DATA ( const TYRANNY_REGISTER_DATA &value )
	{
		operator=(value);
	}

	TYRANNY_REGISTER_DATA& operator = ( const TYRANNY_REGISTER_DATA& rvalue )
	{
		dwInfoCharID	= rvalue.dwInfoCharID;
		wInfoSchool		= rvalue.wInfoSchool;
		wInfoClass		= rvalue.wInfoClass;
		bLateJoin		= rvalue.bLateJoin;

		StringCchCopy( szInfoCharName, TYRANNY_CHARNAME_STRING_SIZE, rvalue.szInfoCharName );

		return *this;
	}
};

struct TYRANNY_REGISTER_DATA_QUEUE
{ 
	TYRANNY_REGISTER_DATA sPlayerData;
	WORD	wQueueNum;

	TYRANNY_REGISTER_DATA_QUEUE()
		: sPlayerData()
		, wQueueNum(0)
	{
	}
};

struct TYRANNY_PLAYER_DATA
{  
	//info
	DWORD	dwInfoCharID;
	char	szInfoCharName[TYRANNY_CHARNAME_STRING_SIZE];
	WORD	wInfoSchool;
	WORD	wInfoClass;

	//rank
	WORD	wRankAll;
	WORD	wRankSchool;

	//score
	WORD	wScoreKill;
	WORD	wScoreDeath;
	DWORD	dwScoreDamage;
	DWORD	dwScoreHeal;
	WORD	wScoreResu;
	DWORD	dwScoreTotal;
	WORD	wRewardPoint;

	BOOL	bLateJoin;

	TYRANNY_PLAYER_DATA()
		: dwInfoCharID(TYRANNY_PLAYER_NULL)
		, wInfoSchool(0)
		, wInfoClass(0)

		, wRankAll(0)
		, wRankSchool(0)

		, wScoreKill(0)
		, wScoreDeath(0)
		, dwScoreDamage(0)
		, dwScoreHeal(0)
		, wScoreResu(0)
		, dwScoreTotal(0)
		, wRewardPoint(0)

		, bLateJoin(FALSE)
	{
		memset( szInfoCharName, 0, sizeof(char) * (TYRANNY_CHARNAME_STRING_SIZE) );
	}

	bool operator < ( const TYRANNY_PLAYER_DATA& sData )
	{			
		if ( dwScoreTotal > sData.dwScoreTotal )
			return true;
		else if ( dwScoreTotal == sData.dwScoreTotal && wScoreDeath < sData.wScoreDeath )	
			return true;

		return  false;
	}

	TYRANNY_PLAYER_DATA ( const TYRANNY_PLAYER_DATA &value )
	{
		operator=(value);
	}

	TYRANNY_PLAYER_DATA& operator = ( const TYRANNY_PLAYER_DATA& rvalue )
	{
		dwInfoCharID	= rvalue.dwInfoCharID;
		wInfoSchool		= rvalue.wInfoSchool;
		wInfoClass		= rvalue.wInfoClass;

		wRankAll		= rvalue.wRankAll;
		wRankSchool		= rvalue.wRankSchool;

		wScoreKill		= rvalue.wScoreKill;
		wScoreDeath		= rvalue.wScoreDeath;
		dwScoreDamage	= rvalue.dwScoreDamage;
		dwScoreHeal		= rvalue.dwScoreHeal;
		wScoreResu		= rvalue.wScoreResu;
		dwScoreTotal	= rvalue.dwScoreTotal;
		wRewardPoint	= rvalue.wRewardPoint;
		
		StringCchCopy( szInfoCharName, TYRANNY_CHARNAME_STRING_SIZE, rvalue.szInfoCharName );

		bLateJoin	= rvalue.bLateJoin;

		return *this;
	}
};

struct TYRANNY_TOWER_DATA
{ 
	SNATIVEID	sidCrow;
	WORD		wOwner;

	TYRANNY_TOWER_DATA()
		: sidCrow( NATIVEID_NULL().dwID)
		, wOwner(TYRANNY_SCHOOL_SIZE)
	{
	}
};

struct TYRANNY_SCORE_DATA
{ 
	WORD	wSchool;
	WORD	wCapture;
	WORD	wKills;
	WORD	wDeaths;

	TYRANNY_SCORE_DATA()
		: wSchool(TYRANNY_SCHOOL_SIZE)
		, wCapture(0)
		, wKills(0)
		, wDeaths(0)
	{
	}

	bool operator < ( const TYRANNY_SCORE_DATA& sData )
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

struct TYRANNY_REWARD_DATA
{ 
	DWORD		dwCharID;
	SNATIVEID	sItemReward;
	bool		bSent;

	TYRANNY_REWARD_DATA()
		: dwCharID(TYRANNY_PLAYER_NULL)
		, bSent( FALSE )
	{
	};
};

struct TYRANNY_BUFF_DATA
{ 
	SNATIVEID	sidSkill;
	WORD		wLevel;

	TYRANNY_BUFF_DATA()
		: sidSkill(NATIVEID_NULL())
		, wLevel(0)
	{
	};
};

struct TYRANNY_BUFF_DATA2
{ 
	SNATIVEID	sidSkill;
	WORD		wLevel[TYRANNY_BATTLE_BUFF_LEVEL_NUM];

	TYRANNY_BUFF_DATA2()
		: sidSkill(NATIVEID_NULL())
	{
		for( int i=0; i<TYRANNY_BATTLE_BUFF_LEVEL_NUM; ++i )
			wLevel[i] = 0;
	};
};

/* PVPTyranny Hash Check, Juver, 2020/07/28 */
struct TYRANNY_PLAYER_HASH_DATA
{
	DWORD dwCharID;
	std::string strHash;

	TYRANNY_PLAYER_HASH_DATA()
		: dwCharID(TYRANNY_PLAYER_NULL)
		, strHash("")
	{
	}
};


typedef std::vector<TYRANNY_SCHED>				TYRANNY_SCHED_VEC;

typedef std::map<DWORD,TYRANNY_REGISTER_DATA>		TYRANNY_REGISTER_DATA_MAP;
typedef TYRANNY_REGISTER_DATA_MAP::iterator			TYRANNY_REGISTER_DATA_MAP_ITER;
typedef std::vector<TYRANNY_REGISTER_DATA>			TYRANNY_REGISTER_DATA_VEC;
typedef std::vector<TYRANNY_REGISTER_DATA_QUEUE>	TYRANNY_REGISTER_DATA_QUEUE_VEC;

typedef std::map<DWORD,TYRANNY_PLAYER_DATA>		TYRANNY_PLAYER_DATA_MAP;
typedef TYRANNY_PLAYER_DATA_MAP::iterator		TYRANNY_PLAYER_DATA_MAP_ITER;
typedef std::vector<TYRANNY_PLAYER_DATA>		TYRANNY_PLAYER_DATA_VEC;

typedef std::vector<TYRANNY_SCORE_DATA>			TYRANNY_SCORE_DATA_VEC;
typedef std::map<DWORD,TYRANNY_REWARD_DATA>		TYRANNY_REWARD_MAP;
typedef TYRANNY_REWARD_MAP::iterator			TYRANNY_REWARD_MAP_ITER;

typedef std::vector<TYRANNY_BUFF_DATA>			TYRANNY_BUFF_DATA_VEC;

/* PVPTyranny Hash Check, Juver, 2020/07/28 */
typedef std::vector<TYRANNY_PLAYER_HASH_DATA>			TYRANNY_PLAYER_HASH_VEC;
typedef std::map<std::string, TYRANNY_PLAYER_HASH_DATA> TYRANNY_PLAYER_HASH_DATA_MAP;
typedef TYRANNY_PLAYER_HASH_DATA_MAP::iterator			TYRANNY_PLAYER_HASH_DATA_MAP_ITER;

typedef std::vector<SNATIVEID>					TYRANNY_MAP_VEC;


struct STG_RANK_EX
{
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szClubName[TYRANNY_CLUB_NAME_STRING_SIZE+1];	

	STG_RANK_EX()
		: wClubRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
	{
		memset( szClubName, 0, sizeof(char) * (TYRANNY_CLUB_NAME_STRING_SIZE+1) );
	}

	void Init()
	{
		wClubRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;

		StringCchCopy( szClubName, TYRANNY_CLUB_NAME_STRING_SIZE+1, "" );
	}

	STG_RANK_EX& operator = ( const STG_RANK_EX& rvalue )
	{
		wClubRanking	= rvalue.wClubRanking;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		 = rvalue.wDeathNum;
		StringCchCopy( szClubName, TYRANNY_CLUB_NAME_STRING_SIZE+1, rvalue.szClubName );

		return *this;
	}

};

struct STG_RANK_INFO
{
	int		nIndex;   
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szClubName[TYRANNY_CLUB_NAME_STRING_SIZE+1];	

	STG_RANK_INFO()
		: nIndex ( -1 )
		, dwClubID( CLUB_NULL )
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
	{
		memset( szClubName, 0, sizeof(char) * (TYRANNY_CLUB_NAME_STRING_SIZE+1) );
	}

	STG_RANK_INFO ( const STG_RANK_INFO &value )
	{
		operator=(value);
	}

	STG_RANK_INFO& operator = ( const STG_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, TYRANNY_CLUB_NAME_STRING_SIZE+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		return *this;
	}

	bool operator < ( const STG_RANK_INFO& sCWRank )
	{			
		if ( wKillNum > sCWRank.wKillNum ) return true;
		else if ( wKillNum == sCWRank.wKillNum && wDeathNum < sCWRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const STG_RANK_INFO& sCWRank ) 
	{
		if ( wKillNum == sCWRank.wKillNum && wDeathNum == sCWRank.wDeathNum ) return true;

		return false;

	}
};

struct STG_RANK
{
	int nIndex;
	WORD wClubRanking;
	char szClubName[TYRANNY_CLUB_NAME_STRING_SIZE+1];

	//	디버그용
	WORD wKillNum;
	WORD wDeathNum;

	STG_RANK() 
		: nIndex(-1)
		, wClubRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
	{
		memset( szClubName, 0, sizeof(char) * (TYRANNY_CLUB_NAME_STRING_SIZE+1) );
	}

	STG_RANK ( const STG_RANK &value )
	{
		operator=(value);
	}

	STG_RANK& operator = ( const STG_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, TYRANNY_CLUB_NAME_STRING_SIZE+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		return *this;
	}

	STG_RANK ( const STG_RANK_INFO &value )
	{
		operator=(value);
	}

	STG_RANK& operator = ( const STG_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, TYRANNY_CLUB_NAME_STRING_SIZE+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		return *this;
	}
};

typedef std::vector<STG_RANK_INFO>		TG_RANK_INFO_VEC;
typedef TG_RANK_INFO_VEC::iterator		TG_RANK_INFO_VEC_ITER;

typedef std::vector<STG_RANK>			TG_RANK_VEC;
typedef TG_RANK_VEC::iterator			TG_RANK_VEC_ITER;

typedef std::map<DWORD,STG_RANK_INFO>	TG_RANK_INFO_MAP;
typedef TG_RANK_INFO_MAP::iterator		TG_RANK_INFO_MAP_ITER;

//  ________                  .____                         ._______  ___                                       
//  \______ \    ____  ___  __|    |      ____  _______   __| _/\   \/  /                                       
//   |    |  \ _/ __ \ \  \/ /|    |     /  _ \ \_  __ \ / __ |  \     /                                        
//   |    `   \\  ___/  \   / |    |___ (  <_> ) |  | \// /_/ |  /     \                                        
//  /_______  / \___  >  \_/  |_______ \ \____/  |__|   \____ | /___/\  \                                       
//          \/      \/                \/                     \/       \_/                                       
//  ________  _______    ____ _________                                                                         
//  \_____  \ \   _  \  /_   |\______  \                                                                        
//   /  ____/ /  /_\  \  |   |    /    /                                                                        
//  /       \ \  \_/   \ |   |   /    /                                                                         
//  \_______ \ \_____  / |___|  /____/                                                                          
//          \/ 