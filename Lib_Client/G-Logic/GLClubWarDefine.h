/*!
 * \file GLClubWarDefine.h
 *
 * \author Montage
 * \date August 2023
 *
 * 
 */

#include "GLCharDefine.h"

#pragma once

/* 7-13-23 ClubWar War Ranking System - Montage */
#define CLUBWAR_CLUB_RANKING_NUM		10
#define CLUBWAR_CLUB_NAME				10
#define CLUBWAR_RANKING_NUM				10
#define CLUBWAR_CHAR_NAME				20

/* 7-13-23 ClubWar War Ranking System - Montage */
struct SCLUBWAR_CLUB_RANK_EX
{
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szClubName[CLUBWAR_CLUB_NAME+1];	

	SCLUBWAR_CLUB_RANK_EX()
		: dwClubID(0)
		, wClubRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
	{
		memset( szClubName, 0, sizeof(char) * (CLUBWAR_CLUB_NAME+1) );
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

		StringCchCopy( szClubName, CLUBWAR_CLUB_NAME+1, "" );
	}

	SCLUBWAR_CLUB_RANK_EX& operator = ( const SCLUBWAR_CLUB_RANK_EX& rvalue )
	{
		dwClubID		= rvalue.dwClubID;
		wClubRanking	= rvalue.wClubRanking;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		= rvalue.wDeathNum;
		wResuNum		= rvalue.wResuNum;
		wGuNum			= rvalue.wGuNum;
		wGuMarkVer		= rvalue.wGuMarkVer;

		StringCchCopy( szClubName, CLUBWAR_CLUB_NAME+1, rvalue.szClubName );

		return *this;
	}

};

struct SCLUBWAR_CLUB_RANK_INFO
{
	int		nIndex;   
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szClubName[CLUBWAR_CLUB_NAME+1];	

	SCLUBWAR_CLUB_RANK_INFO()
		: nIndex ( -1 )
		, dwClubID( CLUB_NULL )
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
	{
		memset( szClubName, 0, sizeof(char) * (CLUBWAR_CLUB_NAME+1) );
	}

	SCLUBWAR_CLUB_RANK_INFO ( const SCLUBWAR_CLUB_RANK_INFO &value )
	{
		operator=(value);
	}

	SCLUBWAR_CLUB_RANK_INFO& operator = ( const SCLUBWAR_CLUB_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer	= rvalue.wGuMarkVer;

		StringCchCopy( szClubName, CLUBWAR_CLUB_NAME+1, rvalue.szClubName );

		return *this;
	}

	bool operator < ( const SCLUBWAR_CLUB_RANK_INFO& sClubWarClubRank )
	{			
		if ( wKillNum > sClubWarClubRank.wKillNum ) return true;
		else if ( wKillNum == sClubWarClubRank.wKillNum && wDeathNum < sClubWarClubRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const SCLUBWAR_CLUB_RANK_INFO& sClubWarClubRank ) 
	{
		if ( wKillNum == sClubWarClubRank.wKillNum && wDeathNum == sClubWarClubRank.wDeathNum ) return true;

		return false;

	}
};

struct SCLUBWAR_CLUB_RANK
{
	int nIndex;
	DWORD dwClubID;
	WORD wClubRanking;
	char szClubName[CLUBWAR_CLUB_NAME+1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wResuNum;
	WORD wGuNum;
	WORD wGuMarkVer;

	SCLUBWAR_CLUB_RANK() 
		: nIndex(-1)
		, dwClubID( CLUB_NULL )
		, wClubRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, wGuNum(0)
		, wGuMarkVer(0)
	{
		memset( szClubName, 0, sizeof(char) * (CLUBWAR_CLUB_NAME+1) );
	}

	SCLUBWAR_CLUB_RANK ( const SCLUBWAR_CLUB_RANK &value )
	{
		operator=(value);
	}

	SCLUBWAR_CLUB_RANK& operator = ( const SCLUBWAR_CLUB_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID	= rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, CLUBWAR_CLUB_NAME+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	SCLUBWAR_CLUB_RANK ( const SCLUBWAR_CLUB_RANK_INFO &value )
	{
		operator=(value);
	}

	SCLUBWAR_CLUB_RANK& operator = ( const SCLUBWAR_CLUB_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID	= rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, CLUBWAR_CLUB_NAME+1, rvalue.szClubName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer= rvalue.wGuMarkVer;

		return *this;
	}
};

/*===========================*/
/* 7-13-23 ClubWar War Ranking System - Montage */
struct SCLUBWAR_RANK_EX
{
	DWORD	dwCharID;
	WORD	wCharRanking;
	BYTE	nChaClass;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szName[CLUBWAR_CHAR_NAME+1];
	WORD	wSchoolNum;

	SCLUBWAR_RANK_EX()
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
		memset( szName, 0, sizeof(char) * (CLUBWAR_CHAR_NAME+1) );
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

		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, "" );
	}

	SCLUBWAR_RANK_EX& operator = ( const SCLUBWAR_RANK_EX& rvalue )
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

		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

};

struct SCLUBWAR_RANK_INFO
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
	char	szName[CLUBWAR_CHAR_NAME+1];
	WORD	wSchoolNum;

	SCLUBWAR_RANK_INFO()
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
		memset( szName, 0, sizeof(char) * (CLUBWAR_CHAR_NAME+1) );
	}

	SCLUBWAR_RANK_INFO ( const SCLUBWAR_RANK_INFO &value )
	{
		operator=(value);
	}

	SCLUBWAR_RANK_INFO& operator = ( const SCLUBWAR_RANK_INFO& rvalue )
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

		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

	bool operator < ( const SCLUBWAR_RANK_INFO& sClubWarRank )
	{			
		if ( wKillNum > sClubWarRank.wKillNum ) return true;
		else if ( wKillNum == sClubWarRank.wKillNum && wDeathNum < sClubWarRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const SCLUBWAR_RANK_INFO& sClubWarRank ) 
	{
		if ( wKillNum == sClubWarRank.wKillNum && wDeathNum == sClubWarRank.wDeathNum ) return true;

		return false;

	}
};

struct SCLUBWAR_RANK
{
	int nIndex;
	DWORD dwCharID;
	WORD wCharRanking;
	BYTE nCharClass;
	char szName[CLUBWAR_CHAR_NAME+1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wResuNum;
	WORD wGuNum;
	WORD wGuMarkVer;
	WORD wSchoolNum;

	SCLUBWAR_RANK() 
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
		memset( szName, 0, sizeof(char) * (CLUBWAR_CHAR_NAME+1) );
	}

	SCLUBWAR_RANK ( const SCLUBWAR_RANK &value )
	{
		operator=(value);
	}

	SCLUBWAR_RANK& operator = ( const SCLUBWAR_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nCharClass;
		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}

	SCLUBWAR_RANK ( const SCLUBWAR_RANK_INFO &value )
	{
		operator=(value);
	}

	SCLUBWAR_RANK& operator = ( const SCLUBWAR_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nChaClass;
		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

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
/* 7-13-23 ClubWar War Ranking System - Montage */
struct SCLUBWAR_RANK_RESU_EX
{
	DWORD	dwCharID;
	WORD	wCharRanking;
	BYTE	nChaClass;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szName[CLUBWAR_CHAR_NAME+1];
	WORD	wSchoolNum;

	SCLUBWAR_RANK_RESU_EX()
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
		memset( szName, 0, sizeof(char) * (CLUBWAR_CHAR_NAME+1) );
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

		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, "" );
	}

	SCLUBWAR_RANK_RESU_EX& operator = ( const SCLUBWAR_RANK_RESU_EX& rvalue )
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

		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

};

struct SCLUBWAR_RANK_RESU_INFO
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
	char	szName[CLUBWAR_CHAR_NAME+1];
	WORD	wSchoolNum;

	SCLUBWAR_RANK_RESU_INFO()
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
		memset( szName, 0, sizeof(char) * (CLUBWAR_CHAR_NAME+1) );
	}

	SCLUBWAR_RANK_RESU_INFO ( const SCLUBWAR_RANK_RESU_INFO &value )
	{
		operator=(value);
	}

	SCLUBWAR_RANK_RESU_INFO& operator = ( const SCLUBWAR_RANK_RESU_INFO& rvalue )
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

		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		return *this;
	}

	bool operator < ( const SCLUBWAR_RANK_RESU_INFO& sClubWarRank )
	{			
		if ( wResuNum > sClubWarRank.wResuNum ) return true;
		else if ( wResuNum == sClubWarRank.wResuNum && wDeathNum < sClubWarRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const SCLUBWAR_RANK_RESU_INFO& sClubWarRank ) 
	{
		if ( wResuNum == sClubWarRank.wResuNum && wDeathNum == sClubWarRank.wDeathNum ) return true;

		return false;

	}
};

struct SCLUBWAR_RANK_RESU
{
	int nIndex;
	DWORD dwCharID;
	WORD wCharRanking;
	BYTE nCharClass;
	char szName[CLUBWAR_CHAR_NAME+1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wResuNum;
	WORD wGuNum;
	WORD wGuMarkVer;
	WORD wSchoolNum;

	SCLUBWAR_RANK_RESU() 
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
		memset( szName, 0, sizeof(char) * (CLUBWAR_CHAR_NAME+1) );
	}

	SCLUBWAR_RANK_RESU ( const SCLUBWAR_RANK_RESU &value )
	{
		operator=(value);
	}

	SCLUBWAR_RANK_RESU& operator = ( const SCLUBWAR_RANK_RESU& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nCharClass;
		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}

	SCLUBWAR_RANK_RESU ( const SCLUBWAR_RANK_RESU_INFO &value )
	{
		operator=(value);
	}

	SCLUBWAR_RANK_RESU& operator = ( const SCLUBWAR_RANK_RESU_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID	= rvalue.dwCharID;
		wCharRanking = rvalue.wCharRanking;
		nCharClass = rvalue.nChaClass;
		StringCchCopy( szName, CLUBWAR_CHAR_NAME+1, rvalue.szName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;

		wResuNum = rvalue.wResuNum;

		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		wSchoolNum = rvalue.wSchoolNum;

		return *this;
	}
};

struct SCW_RANKING_REWARD1
{
	SNATIVEID sReward_[10];
	SNATIVEID sReward_Other;
	WORD	  wOther_Required_Kill;

	SCW_RANKING_REWARD1() : wOther_Required_Kill(0)
	{
		for (int i = 0; i < 10; i++)
			sReward_[i] = NATIVEID_NULL();
			sReward_Other = NATIVEID_NULL();
	}
};

struct SCW_RANKING_RESU_REWARD
{
	SNATIVEID sRewardResu_[10];
	SNATIVEID sRewardResu_Other;
	WORD	  wOther_Required_Resu;

	SCW_RANKING_RESU_REWARD() : wOther_Required_Resu(0)
	{
		for (int i = 0; i < 10; i++)
			sRewardResu_[i] = NATIVEID_NULL();
			sRewardResu_Other = NATIVEID_NULL();
	}
};

typedef std::vector<SCLUBWAR_RANK_INFO>				CLUBWAR_RANK_INFO_VEC;
typedef CLUBWAR_RANK_INFO_VEC::iterator				CLUBWAR_RANK_INFO_VEC_ITER;

typedef std::vector<SCLUBWAR_RANK>					CLUBWAR_RANK_VEC;
typedef CLUBWAR_RANK_VEC::iterator					CLUBWAR_RANK_VEC_ITER;

typedef std::map<DWORD,SCLUBWAR_RANK_INFO>			CLUBWAR_RANK_INFO_MAP;
typedef CLUBWAR_RANK_INFO_MAP::iterator				CLUBWAR_RANK_INFO_MAP_ITER;


typedef std::vector<SCLUBWAR_RANK_RESU_INFO>			CLUBWAR_RANK_RESU_INFO_VEC;
typedef CLUBWAR_RANK_RESU_INFO_VEC::iterator			CLUBWAR_RANK_RESU_INFO_VEC_ITER;

typedef std::vector<SCLUBWAR_RANK_RESU>					CLUBWAR_RANK_RESU_VEC;
typedef CLUBWAR_RANK_RESU_VEC::iterator					CLUBWAR_RANK_RESU_VEC_ITER;

typedef std::map<DWORD,SCLUBWAR_RANK_RESU_INFO>			CLUBWAR_RANK_RESU_INFO_MAP;
typedef CLUBWAR_RANK_RESU_INFO_MAP::iterator			CLUBWAR_RANK_RESU_INFO_MAP_ITER;


typedef std::vector<SCLUBWAR_CLUB_RANK_INFO>		CLUBWAR_CLUB_RANK_INFO_VEC;
typedef CLUBWAR_CLUB_RANK_INFO_VEC::iterator		CLUBWAR_CLUB_RANK_INFO_VEC_ITER;

typedef std::vector<SCLUBWAR_CLUB_RANK>				CLUBWAR_CLUB_RANK_VEC;
typedef CLUBWAR_CLUB_RANK_VEC::iterator				CLUBWAR_CLUB_RANK_VEC_ITER;

typedef std::map<DWORD,SCLUBWAR_CLUB_RANK_INFO>		CLUBWAR_CLUB_RANK_INFO_MAP;
typedef CLUBWAR_CLUB_RANK_INFO_MAP::iterator		CLUBWAR_CLUB_RANK_INFO_MAP_ITER;