#pragma once

#include "GLCharDefine.h"

#define RANKING_NUM_CW		10
#define CLUB_NAME			10

struct SCWGUILD_RANK_EX
{
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szClubName[CLUB_NAME+1];	
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWGUILD_RANK_EX()
		: wClubRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
		, wGuNum( 0 )
		, wGuMarkVer( CLUB_NULL )
	{
		memset( szClubName, 0, sizeof(char) * (CLUB_NAME+1) );
	}

	void Init()
	{
		wClubRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;
		wGuNum = 0;
		wGuMarkVer = 0;

		StringCchCopy( szClubName, CLUB_NAME+1, "" );
	}

	SCWGUILD_RANK_EX& operator = ( const SCWGUILD_RANK_EX& rvalue )
	{
		wClubRanking	= rvalue.wClubRanking;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		 = rvalue.wDeathNum;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		StringCchCopy( szClubName, CLUB_NAME+1, rvalue.szClubName );

		return *this;
	}

};

struct SCWGUILD_RANK_INFO
{
	int		nIndex;
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szClubName[CLUB_NAME+1];	
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWGUILD_RANK_INFO()
        : nIndex ( -1 )
		, dwClubID( CLUB_NULL )
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wGuNum( 0 )
		, wGuMarkVer( CLUB_NULL )
	{
		memset( szClubName, 0, sizeof(char) * (CLUB_NAME+1) );
	}

	SCWGUILD_RANK_INFO ( const SCWGUILD_RANK_INFO &value )
	{
		operator=(value);
	}

	SCWGUILD_RANK_INFO& operator = ( const SCWGUILD_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, CLUB_NAME+1, rvalue.szClubName );
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	bool operator < ( const SCWGUILD_RANK_INFO& sCWGuildRank )
	{			
		if ( wKillNum > sCWGuildRank.wKillNum ) return true;
		else if ( wKillNum == sCWGuildRank.wKillNum && wDeathNum < sCWGuildRank.wDeathNum ) return true;
		
		return  false;
	}

	bool operator == ( const SCWGUILD_RANK_INFO& sCWGuildRank ) 
	{
		if ( wKillNum == sCWGuildRank.wKillNum && wDeathNum == sCWGuildRank.wDeathNum ) return true;

		return false;

	}
};

struct SCWGUILD_RANK
{
	int		nIndex;
	WORD	wClubRanking;
	char	szClubName[CLUB_NAME+1];
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWGUILD_RANK() 
		: nIndex(-1)
		, wClubRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
		, wGuNum( 0 )
		, wGuMarkVer( CLUB_NULL )
	{
		memset( szClubName, 0, sizeof(char) * (CLUB_NAME+1) );
	}

	SCWGUILD_RANK ( const SCWGUILD_RANK &value )
	{
		operator=(value);
	}

	SCWGUILD_RANK& operator = ( const SCWGUILD_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, CLUB_NAME+1, rvalue.szClubName );
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	SCWGUILD_RANK ( const SCWGUILD_RANK_INFO &value )
	{
		operator=(value);
	}

	SCWGUILD_RANK& operator = ( const SCWGUILD_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy( szClubName, CLUB_NAME+1, rvalue.szClubName );
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}
};

typedef std::vector<SCWGUILD_RANK_INFO>		CWGUILD_RANK_INFO_VEC;
typedef CWGUILD_RANK_INFO_VEC::iterator		CWGUILD_RANK_INFO_VEC_ITER;

typedef std::vector<SCWGUILD_RANK>			CWGUILD_RANK_VEC;
typedef CWGUILD_RANK_VEC::iterator			CWGUILD_RANK_VEC_ITER;

typedef std::map<DWORD,SCWGUILD_RANK_INFO>	CWGUILD_RANK_INFO_MAP;
typedef CWGUILD_RANK_INFO_MAP::iterator		CWGUILD_RANK_INFO_MAP_ITER;

typedef std::set<DWORD>						CWGUILD_AWARD_CHAR;			//	보상을 받은 캐릭터
typedef CWGUILD_AWARD_CHAR::iterator		CWGUILD_AWARD_CHAR_ITER;

struct SCWGUILD_AWARD_ITEM
{
	SNATIVEID	nAwardItem[4];		// 1,2,3,기타
	DWORD		dwAwardLimit;

	SCWGUILD_AWARD_ITEM()
		: dwAwardLimit(4)
	{	
		memset( nAwardItem, -1, sizeof( SNATIVEID ) * 4 );
	}
};
struct SCWPLAYER_RANK_EX
{
	WORD	wPlayerRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szCharName[CHAR_SZNAME+1];	

    int		nChaClass;
    WORD	wSchool;
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWPLAYER_RANK_EX()
		: wPlayerRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
        , nChaClass(0)
        , wSchool(0)
		, wGuNum ( 0 )
		, wGuMarkVer ( CLUB_NULL )
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_SZNAME+1) );
	}

	void Init()
	{
		wPlayerRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;
        nChaClass = 0;
        wSchool = 0;
		wGuNum = 0;
		wGuMarkVer = 0;
		StringCchCopy( szCharName, CHAR_SZNAME+1, "" );
	}

	SCWPLAYER_RANK_EX& operator = ( const SCWPLAYER_RANK_EX& rvalue )
	{
		wPlayerRanking	= rvalue.wPlayerRanking;
		wKillNum		= rvalue.wKillNum;
		wDeathNum		 = rvalue.wDeathNum;
        nChaClass = rvalue.nChaClass;
        wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );

		return *this;
	}

};

struct SCWPLAYER_RANK_INFO
{
	int		nIndex;   
	DWORD	dwCharID;
	WORD	wPlayerRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	char	szCharName[CHAR_SZNAME+1];	

    int		nChaClass;
    WORD	wSchool;
	WORD	wGuNum;
	WORD	wGuMarkVer;


	SCWPLAYER_RANK_INFO()
		: nIndex ( -1 )
		, dwCharID( CLUB_NULL )
		, wPlayerRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
        , nChaClass(0)
        , wSchool(0)
		, wGuNum ( 0 )
		, wGuMarkVer ( CLUB_NULL )
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_SZNAME+1) );
	}

	SCWPLAYER_RANK_INFO ( const SCWPLAYER_RANK_INFO &value )
	{
		operator=(value);
	}

	SCWPLAYER_RANK_INFO& operator = ( const SCWPLAYER_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wPlayerRanking = rvalue.wPlayerRanking;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
        nChaClass = rvalue.nChaClass;
        wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		return *this;
	}

	bool operator < ( const SCWPLAYER_RANK_INFO& sCWPlayerRank )
	{			
		if ( wKillNum > sCWPlayerRank.wKillNum ) return true;
		else if ( wKillNum == sCWPlayerRank.wKillNum && wDeathNum < sCWPlayerRank.wDeathNum ) return true;

		return  false;
	}

	bool operator == ( const SCWPLAYER_RANK_INFO& sCWPlayerRank ) 
	{
		if ( wKillNum == sCWPlayerRank.wKillNum && wDeathNum == sCWPlayerRank.wDeathNum ) return true;

		return false;

	}
};

struct SCWPLAYER_RANK
{
	int 	nIndex;
	WORD	wPlayerRanking;
	char	szCharName[CHAR_SZNAME+1];
	WORD	wKillNum;
	WORD	wDeathNum;

    int		nChaClass;
    WORD	wSchool;
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWPLAYER_RANK() 
		: nIndex(-1)
		, wPlayerRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
        , nChaClass(0)
        , wSchool(0)
		, wGuNum(0)
		, wGuMarkVer ( CLUB_NULL ) 
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_SZNAME+1) );
	}

	SCWPLAYER_RANK ( const SCWPLAYER_RANK &value )
	{
		operator=(value);
	}

	SCWPLAYER_RANK& operator = ( const SCWPLAYER_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		wPlayerRanking = rvalue.wPlayerRanking;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		nChaClass = rvalue.nChaClass;
		wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	SCWPLAYER_RANK ( const SCWPLAYER_RANK_INFO &value )
	{
		operator=(value);
	}

	SCWPLAYER_RANK& operator = ( const SCWPLAYER_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		wPlayerRanking = rvalue.wPlayerRanking;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		nChaClass = rvalue.nChaClass;
		wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;
		return *this;
	}
};

typedef std::vector<SCWPLAYER_RANK_INFO>		CWPLAYER_RANK_INFO_VEC;
typedef CWPLAYER_RANK_INFO_VEC::iterator		CWPLAYER_RANK_INFO_VEC_ITER;

typedef std::vector<SCWPLAYER_RANK>				CWPLAYER_RANK_VEC;
typedef CWPLAYER_RANK_VEC::iterator				CWPLAYER_RANK_VEC_ITER;

typedef std::map<DWORD,SCWPLAYER_RANK_INFO>		CWPLAYER_RANK_INFO_MAP;
typedef CWPLAYER_RANK_INFO_MAP::iterator		CWPLAYER_RANK_INFO_MAP_ITER;

typedef std::set<DWORD>							CWPLAYER_AWARD_CHAR;			//	보상을 받은 캐릭터
typedef CWPLAYER_AWARD_CHAR::iterator			CWPLAYER_AWARD_CHAR_ITER;

struct SCWPLAYER_AWARD_ITEM
{
	SNATIVEID	nAwardItem[4];		// 1,2,3,기타
	DWORD		dwAwardLimit;

	SCWPLAYER_AWARD_ITEM()
		: dwAwardLimit(4)
	{	
		memset( nAwardItem, -1, sizeof( SNATIVEID ) * 4 );
	}
};

///////////////////////////////////RESU DEFINE//////////////////////////////////////////
/* rsilent, club wars live ranking, resu 1/30/2024 11:22PM */
struct SCWPLAYER_RANK_RESU_EX
{
	WORD	wPlayerRankingResu;
	WORD	wResuNum;
    int		nChaClass;
    WORD	wSchool;
	WORD	wGuNum;
	WORD	wGuMarkVer;
	char	szCharName[CHAR_SZNAME+1];	

	SCWPLAYER_RANK_RESU_EX()
		: wPlayerRankingResu(0)
		, wResuNum (0)
        , nChaClass(0)
        , wSchool(0)
		, wGuNum(0)
		, wGuMarkVer ( CLUB_NULL ) 
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_SZNAME+1) );
	}

	void Init()
	{
		wPlayerRankingResu = 0;
		wResuNum = 0;
        nChaClass = 0;
        wSchool = 0;
		wGuNum = 0;
		wGuMarkVer = 0;

		StringCchCopy( szCharName, CHAR_SZNAME+1, "" );
	}

	SCWPLAYER_RANK_RESU_EX& operator = ( const SCWPLAYER_RANK_RESU_EX& rvalue )
	{
		wPlayerRankingResu	= rvalue.wPlayerRankingResu;
		wResuNum		= rvalue.wResuNum;
		nChaClass = rvalue.nChaClass;
		wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );

		return *this;
	}

};

struct SCWPLAYER_RANK_INFO_RESU_SCORE
{
	int		nIndex;   
	DWORD	dwCharID;
	WORD	wPlayerRankingResu;
	WORD	wResuNum;
	char	szCharName[CHAR_SZNAME+1];
    int		nChaClass;
    WORD	wSchool;
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWPLAYER_RANK_INFO_RESU_SCORE()
		: nIndex ( -1 )
		, dwCharID( CLUB_NULL )
		, wPlayerRankingResu(0)
		, wResuNum(0)
        , nChaClass(0)
        , wSchool(0)
		, wGuNum(0)
		, wGuMarkVer ( CLUB_NULL ) 
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_SZNAME+1) );
	}

	SCWPLAYER_RANK_INFO_RESU_SCORE ( const SCWPLAYER_RANK_INFO_RESU_SCORE &value )
	{
		operator=(value);
	}

	SCWPLAYER_RANK_INFO_RESU_SCORE& operator = ( const SCWPLAYER_RANK_INFO_RESU_SCORE& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wPlayerRankingResu = rvalue.wPlayerRankingResu;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );
		wResuNum = rvalue.wResuNum;
		nChaClass = rvalue.nChaClass;
		wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	bool operator < ( const SCWPLAYER_RANK_INFO_RESU_SCORE& sCWPlayerRankResuScore )
	{			
		if ( wResuNum > sCWPlayerRankResuScore.wResuNum ) return true;
		else if ( wResuNum == sCWPlayerRankResuScore.wResuNum ) return true;

		return  false;
	}

	bool operator == ( const SCWPLAYER_RANK_INFO_RESU_SCORE& sCWPlayerRankResuScore ) 
	{
		if ( wResuNum == sCWPlayerRankResuScore.wResuNum ) return true;

		return false;

	}
};

struct SCWPLAYER_RANK_RESU
{
	int 	nIndex;
	WORD	wPlayerRankingResu;
	char	szCharName[CHAR_SZNAME+1];
	WORD	wResuNum;
    int		nChaClass;
    WORD	wSchool;
	WORD	wGuNum;
	WORD	wGuMarkVer;

	SCWPLAYER_RANK_RESU() 
		: nIndex(-1)
		, wPlayerRankingResu (0)
		, wResuNum(0)
        , nChaClass(0)
        , wSchool(0)
		, wGuNum(0)
		, wGuMarkVer ( CLUB_NULL ) 
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_SZNAME+1) );
	}

	SCWPLAYER_RANK_RESU ( const SCWPLAYER_RANK_RESU &value )
	{
		operator=(value);
	}

	SCWPLAYER_RANK_RESU& operator = ( const SCWPLAYER_RANK_RESU& rvalue )
	{
		nIndex = rvalue.nIndex;
		wPlayerRankingResu = rvalue.wPlayerRankingResu;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );
		wResuNum = rvalue.wResuNum;
		nChaClass = rvalue.nChaClass;
		wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}

	SCWPLAYER_RANK_RESU ( const SCWPLAYER_RANK_INFO_RESU_SCORE &value )
	{
		operator=(value);
	}

	SCWPLAYER_RANK_RESU& operator = ( const SCWPLAYER_RANK_INFO_RESU_SCORE& rvalue )
	{
		nIndex = rvalue.nIndex;
		wPlayerRankingResu = rvalue.wPlayerRankingResu;
		StringCchCopy( szCharName, CHAR_SZNAME+1, rvalue.szCharName );
		wResuNum = rvalue.wResuNum;
		nChaClass = rvalue.nChaClass;
		wSchool = rvalue.wSchool;
		wGuNum = rvalue.wGuNum;
		wGuMarkVer = rvalue.wGuMarkVer;

		return *this;
	}
};

typedef std::vector<SCWPLAYER_RANK_INFO_RESU_SCORE>			CWPLAYER_RANK_INFO_RESU_SCORE_VEC;
typedef CWPLAYER_RANK_INFO_RESU_SCORE_VEC::iterator			CWPLAYER_RANK_INFO_VEC_RESU_SCORE_ITER;

typedef std::vector<SCWPLAYER_RANK_RESU>					CWPLAYER_RANK_RESU_VEC;
typedef CWPLAYER_RANK_RESU_VEC::iterator					CWPLAYER_RANK_VEC_RESU_ITER;

typedef std::map<DWORD,SCWPLAYER_RANK_INFO_RESU_SCORE>		CWPLAYER_RANK_INFO_RESU_SCORE_MAP;
typedef CWPLAYER_RANK_INFO_RESU_SCORE_MAP::iterator			CWPLAYER_RANK_INFO_RESU_SCORE_MAP_ITER;

typedef std::set<DWORD>										CWPLAYER_RESU_SCORE_AWARD_CHAR;			//	보상을 받은 캐릭터
typedef CWPLAYER_RESU_SCORE_AWARD_CHAR::iterator			CWPLAYER_RESU_SCORE_AWARD_CHAR_ITER;

struct SCWPLAYER_RESU_SCORE_AWARD_ITEM
{
	SNATIVEID	nAwardItem[4];		// 1,2,3,기타
	DWORD		dwAwardLimit;

	SCWPLAYER_RESU_SCORE_AWARD_ITEM()
		: dwAwardLimit(4)
	{	
		memset( nAwardItem, -1, sizeof( SNATIVEID ) * 4 );
	}
};
///////////////////////////////////PLAYER DEFINE//////////////////////////////////////////