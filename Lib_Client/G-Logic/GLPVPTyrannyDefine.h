/*!
 * \file GLPVPTyrannyDefine.h
 *
 * \author Juver
 * \date July 2017
 *
 * 
 */

#include "./GLCharDefine.h" /* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */

#pragma once

#define TYRANNY_SCHEDULE_NONE			UINT_MAX
#define TYRANNY_CHARNAME_STRING_SIZE	33			//similar to CHAR_SZNAME
#define TYRANNY_PLAYER_NULL				0xFFFFFFFF	//similar to GAEAID_NULL
#define TYRANNY_MINI_RANKING_NUM		10			//top rankings size
#define TYRANNY_PACKET_PLAYER_DATA_NUM	30			//agent to field player data
#define TYRANNY_PACKET_RANK_NUM			26			//field to client ranking data
#define TYRANNY_CLUB_NAME_STRING_SIZE	10

#define TYRANNY_REWARD_BUFF_NUM	4
#define TYRANNY_BATTLE_BUFF_NUM 4
#define TYRANNY_BATTLE_BUFF_LEVEL_NUM 10

/* PVPTyranny Hash Check, Juver, 2020/07/28 */
#define TYRANNY_PLAYER_HASH_SIZE 128

#define TYRANNY_GUILD_RANKING_NUM 10

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
#define TYRANNY_CLUB_RANKING_NUM		10
#define TYRANNY_CLUB_NAME				10
#define TYRANNY_RANKING_NUM				10
#define TYRANNY_CHAR_NAME				20

enum TYRANNY_STATE 
{
	TYRANNY_STATE_ENDED		= 0,
	TYRANNY_STATE_REGISTER	= 1,
	TYRANNY_STATE_BATTLE	= 2,
	TYRANNY_STATE_REWARD	= 3,
	TYRANNY_STATE_SIZE		= 4,
};

enum 
{
	TYRANNY_SCHOOL_SG	= 0,
	TYRANNY_SCHOOL_MP	= 1,
	TYRANNY_SCHOOL_PHX	= 2,
	TYRANNY_SCHOOL_SIZE	= 3,
};

enum 
{
	TYRANNY_TOWER_0	= 0,
	TYRANNY_TOWER_1	= 1,
	TYRANNY_TOWER_2	= 2,
	TYRANNY_TOWER_SIZE	= 3,
};

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
////////////////////////////////////////////////////////////////////////////////////
struct STYRANNY_CLUB_RANK_EX
{
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szClubName[TYRANNY_CLUB_NAME+1];	

	STYRANNY_CLUB_RANK_EX()
		: dwClubID(0)
		, wClubRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
	{
		memset( szClubName, 0, sizeof(char) * (TYRANNY_CLUB_NAME+1) );
	}

	void Init()
	{
		dwClubID		= 0;
		wClubRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;
		wResuNum = 0;
		wGuNum = 0;
		wGuMarkVer = 0;

		StringCchCopy( szClubName, TYRANNY_CLUB_NAME+1, "" );
	}

	STYRANNY_CLUB_RANK_EX& operator = ( const STYRANNY_CLUB_RANK_EX& rvalue )
	{
		dwClubID		= rvalue.dwClubID;
		wClubRanking	= rvalue.wClubRanking;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		= rvalue.wDeathNum;
		wResuNum		= rvalue.wResuNum;
		wGuNum			= rvalue.wGuNum;
		wGuMarkVer		= rvalue.wGuMarkVer;

		StringCchCopy( szClubName, TYRANNY_CLUB_NAME+1, rvalue.szClubName );

		return *this;
	}

};

struct STYRANNY_CLUB_RANK_INFO
{
	int		nIndex;   
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szClubName[TYRANNY_CLUB_NAME+1];	

	STYRANNY_CLUB_RANK_INFO()
		: nIndex ( -1 )
		, dwClubID( CLUB_NULL )
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
	{
		memset( szClubName, 0, sizeof(char) * (TYRANNY_CLUB_NAME+1) );
	}

	STYRANNY_CLUB_RANK_INFO ( const STYRANNY_CLUB_RANK_INFO &value )
	{
		operator=(value);
	}

	STYRANNY_CLUB_RANK_INFO& operator = ( const STYRANNY_CLUB_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer	= rvalue.wGuMarkVer;

		StringCchCopy( szClubName, TYRANNY_CLUB_NAME+1, rvalue.szClubName );

		return *this;
	}

	bool operator < ( const STYRANNY_CLUB_RANK_INFO& sTyrannyClubRank )
	{			
		if ( wKillNum > sTyrannyClubRank.wKillNum ) return true;
		else if ( wKillNum == sTyrannyClubRank.wKillNum && wDeathNum < sTyrannyClubRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const STYRANNY_CLUB_RANK_INFO& sTyrannyClubRank ) 
	{
		if ( wKillNum == sTyrannyClubRank.wKillNum && wDeathNum == sTyrannyClubRank.wDeathNum ) return true;

		return false;

	}
};

struct STYRANNY_CLUB_RANK
{
	int nIndex;
	DWORD dwClubID;
	WORD wClubRanking;
	char szClubName[TYRANNY_CLUB_NAME+1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wResuNum;
	WORD wGuNum;
	WORD wGuMarkVer;

	STYRANNY_CLUB_RANK() 
		: nIndex(-1)
		, dwClubID( CLUB_NULL )
		, wClubRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
	{
		memset( szClubName, 0, sizeof(char) * (TYRANNY_CLUB_NAME+1) );
	}

	STYRANNY_CLUB_RANK ( const STYRANNY_CLUB_RANK &value )
	{
		operator=(value);
	}

	STYRANNY_CLUB_RANK& operator = ( const STYRANNY_CLUB_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID	= rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, TYRANNY_CLUB_NAME+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	STYRANNY_CLUB_RANK ( const STYRANNY_CLUB_RANK_INFO &value )
	{
		operator=(value);
	}

	STYRANNY_CLUB_RANK& operator = ( const STYRANNY_CLUB_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID	= rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, TYRANNY_CLUB_NAME+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer= rvalue.wGuMarkVer;

		return *this;
	}
};

/*===========================*/
/* 7-13-23 Tyranny War Ranking System - Montage */
struct STYRANNY_RANK_EX
{
	DWORD	dwCharID;
	WORD	wCharRanking;
	BYTE	nChaClass;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szName[TYRANNY_CHAR_NAME+1];
	WORD	wSchoolNum;

	STYRANNY_RANK_EX()
		: dwCharID(0)
		, wCharRanking(0)
		, nChaClass (0)
		, wKillNum (0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
		, wSchoolNum(0)
	{
		memset( szName, 0, sizeof(char) * (TYRANNY_CHAR_NAME+1) );
	}

	void Init()
	{
		dwCharID = 0;
		wCharRanking = 0;
		nChaClass = 0;
		wKillNum = 0;
		wDeathNum = 0;
		wResuNum = 0;
		wGuNum = 0;
		wGuMarkVer = 0;
		wSchoolNum = 0;

		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, "" );
	}

	STYRANNY_RANK_EX& operator = ( const STYRANNY_RANK_EX& rvalue )
	{
		dwCharID		= rvalue.dwCharID;
		wCharRanking	= rvalue.wCharRanking;
		nChaClass		= rvalue.nChaClass;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		= rvalue.wDeathNum;
		wResuNum		= rvalue.wResuNum;
		wGuNum			= rvalue.wGuNum;
		wGuMarkVer		= rvalue.wGuMarkVer;
		wSchoolNum		= rvalue.wSchoolNum;

		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

};

struct STYRANNY_RANK_INFO
{
	int		nIndex;   
	DWORD	dwCharID;
	WORD	wCharRanking;
	BYTE	nChaClass;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szName[TYRANNY_CHAR_NAME+1];
	WORD	wSchoolNum;

	STYRANNY_RANK_INFO()
		: nIndex ( -1 )
		, dwCharID( GAEAID_NULL )
		, wCharRanking(0)
		, nChaClass(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
		, wSchoolNum(0)
	{
		memset( szName, 0, sizeof(char) * (TYRANNY_CHAR_NAME+1) );
	}

	STYRANNY_RANK_INFO ( const STYRANNY_RANK_INFO &value )
	{
		operator=(value);
	}

	STYRANNY_RANK_INFO& operator = ( const STYRANNY_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nChaClass = rvalue.nChaClass;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer	= rvalue.wGuMarkVer;
		wSchoolNum = rvalue.wSchoolNum;

		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

	bool operator < ( const STYRANNY_RANK_INFO& sTyrannyRank )
	{			
		if ( wKillNum > sTyrannyRank.wKillNum ) return true;
		else if ( wKillNum == sTyrannyRank.wKillNum && wDeathNum < sTyrannyRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const STYRANNY_RANK_INFO& sTyrannyRank ) 
	{
		if ( wKillNum == sTyrannyRank.wKillNum && wDeathNum == sTyrannyRank.wDeathNum ) return true;

		return false;

	}
};

struct STYRANNY_RANK
{
	int nIndex;
	DWORD dwCharID;
	WORD wCharRanking;
	BYTE nCharClass;
	char szName[TYRANNY_CHAR_NAME+1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wResuNum;
	WORD wGuNum;
	WORD wGuMarkVer;
	WORD wSchoolNum;

	STYRANNY_RANK() 
		: nIndex(-1)
		, dwCharID( CLUB_NULL )
		, wCharRanking (0)
		, nCharClass(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
		, wSchoolNum(0)
	{
		memset( szName, 0, sizeof(char) * (TYRANNY_CHAR_NAME+1) );
	}

	STYRANNY_RANK ( const STYRANNY_RANK &value )
	{
		operator=(value);
	}

	STYRANNY_RANK& operator = ( const STYRANNY_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nCharClass;
		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}

	STYRANNY_RANK ( const STYRANNY_RANK_INFO &value )
	{
		operator=(value);
	}

	STYRANNY_RANK& operator = ( const STYRANNY_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nChaClass;
		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}
};

/************/
/* 7-13-23 Tyranny War Ranking System - Montage */
struct STYRANNY_RANK_RESU_EX
{
	DWORD	dwCharID;
	WORD	wCharRanking;
	BYTE	nChaClass;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szName[TYRANNY_CHAR_NAME+1];
	WORD	wSchoolNum;

	STYRANNY_RANK_RESU_EX()
		: dwCharID(0)
		, wCharRanking(0)
		, nChaClass (0)
		, wKillNum (0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
		, wSchoolNum(0)
	{
		memset( szName, 0, sizeof(char) * (TYRANNY_CHAR_NAME+1) );
	}

	void Init()
	{
		dwCharID = 0;
		wCharRanking = 0;
		nChaClass = 0;
		wKillNum = 0;
		wDeathNum = 0;
		wResuNum = 0;
		wGuNum = 0;
		wGuMarkVer = 0;
		wSchoolNum = 0;

		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, "" );
	}

	STYRANNY_RANK_RESU_EX& operator = ( const STYRANNY_RANK_RESU_EX& rvalue )
	{
		dwCharID		= rvalue.dwCharID;
		wCharRanking	= rvalue.wCharRanking;
		nChaClass		= rvalue.nChaClass;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		= rvalue.wDeathNum;
		wResuNum		= rvalue.wResuNum;
		wGuNum			= rvalue.wGuNum;
		wGuMarkVer		= rvalue.wGuMarkVer;
		wSchoolNum		= rvalue.wSchoolNum;

		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

};

struct STYRANNY_RANK_RESU_INFO
{
	int		nIndex;   
	DWORD	dwCharID;
	WORD	wCharRanking;
	BYTE	nChaClass;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szName[TYRANNY_CHAR_NAME+1];
	WORD	wSchoolNum;

	STYRANNY_RANK_RESU_INFO()
		: nIndex ( -1 )
		, dwCharID( GAEAID_NULL )
		, wCharRanking(0)
		, nChaClass(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
		, wSchoolNum(0)
	{
		memset( szName, 0, sizeof(char) * (TYRANNY_CHAR_NAME+1) );
	}

	STYRANNY_RANK_RESU_INFO ( const STYRANNY_RANK_RESU_INFO &value )
	{
		operator=(value);
	}

	STYRANNY_RANK_RESU_INFO& operator = ( const STYRANNY_RANK_RESU_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nChaClass = rvalue.nChaClass;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer	= rvalue.wGuMarkVer;
		wSchoolNum = rvalue.wSchoolNum;

		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

	bool operator < ( const STYRANNY_RANK_RESU_INFO& sTyrannyRank )
	{			
		if ( wResuNum > sTyrannyRank.wResuNum ) return true;
		else if ( wResuNum == sTyrannyRank.wResuNum && wDeathNum < sTyrannyRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const STYRANNY_RANK_RESU_INFO& sTyrannyRank ) 
	{
		if ( wResuNum == sTyrannyRank.wResuNum && wDeathNum == sTyrannyRank.wDeathNum ) return true;

		return false;

	}
};

struct STYRANNY_RANK_RESU
{
	int nIndex;
	DWORD dwCharID;
	WORD wCharRanking;
	BYTE nCharClass;
	char szName[TYRANNY_CHAR_NAME+1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wResuNum;
	WORD wGuNum;
	WORD wGuMarkVer;
	WORD wSchoolNum;

	STYRANNY_RANK_RESU() 
		: nIndex(-1)
		, dwCharID( CLUB_NULL )
		, wCharRanking (0)
		, nCharClass(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
		, wSchoolNum(0)
	{
		memset( szName, 0, sizeof(char) * (TYRANNY_CHAR_NAME+1) );
	}

	STYRANNY_RANK_RESU ( const STYRANNY_RANK_RESU &value )
	{
		operator=(value);
	}

	STYRANNY_RANK_RESU& operator = ( const STYRANNY_RANK_RESU& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nCharClass;
		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}

	STYRANNY_RANK_RESU ( const STYRANNY_RANK_RESU_INFO &value )
	{
		operator=(value);
	}

	STYRANNY_RANK_RESU& operator = ( const STYRANNY_RANK_RESU_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nChaClass;
		StringCchCopy( szName, TYRANNY_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}
};

typedef std::vector<STYRANNY_RANK_INFO>				TYRANNY_RANK_INFO_VEC;
typedef TYRANNY_RANK_INFO_VEC::iterator				TYRANNY_RANK_INFO_VEC_ITER;

typedef std::vector<STYRANNY_RANK>					TYRANNY_RANK_VEC;
typedef TYRANNY_RANK_VEC::iterator					TYRANNY_RANK_VEC_ITER;

typedef std::map<DWORD,STYRANNY_RANK_INFO>			TYRANNY_RANK_INFO_MAP;
typedef TYRANNY_RANK_INFO_MAP::iterator				TYRANNY_RANK_INFO_MAP_ITER;


typedef std::vector<STYRANNY_RANK_RESU_INFO>			TYRANNY_RANK_RESU_INFO_VEC;
typedef TYRANNY_RANK_RESU_INFO_VEC::iterator			TYRANNY_RANK_RESU_INFO_VEC_ITER;

typedef std::vector<STYRANNY_RANK_RESU>					TYRANNY_RANK_RESU_VEC;
typedef TYRANNY_RANK_RESU_VEC::iterator					TYRANNY_RANK_RESU_VEC_ITER;

typedef std::map<DWORD,STYRANNY_RANK_RESU_INFO>			TYRANNY_RANK_RESU_INFO_MAP;
typedef TYRANNY_RANK_RESU_INFO_MAP::iterator			TYRANNY_RANK_RESU_INFO_MAP_ITER;


typedef std::vector<STYRANNY_CLUB_RANK_INFO>		TYRANNY_CLUB_RANK_INFO_VEC;
typedef TYRANNY_CLUB_RANK_INFO_VEC::iterator		TYRANNY_CLUB_RANK_INFO_VEC_ITER;

typedef std::vector<STYRANNY_CLUB_RANK>				TYRANNY_CLUB_RANK_VEC;
typedef TYRANNY_CLUB_RANK_VEC::iterator				TYRANNY_CLUB_RANK_VEC_ITER;

typedef std::map<DWORD,STYRANNY_CLUB_RANK_INFO>		TYRANNY_CLUB_RANK_INFO_MAP;
typedef TYRANNY_CLUB_RANK_INFO_MAP::iterator		TYRANNY_CLUB_RANK_INFO_MAP_ITER;
//////////////////////////////////////////////////////////////////////////////////////////////////
/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */