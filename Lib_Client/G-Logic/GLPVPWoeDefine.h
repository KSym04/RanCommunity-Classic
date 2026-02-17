#include "GLCharDefine.h"
#define RANKING_NUM		10
#define CLUB_NAME		10
#define CHAR_NAME		20


#pragma once

#define WOE_SCHEDULE_NONE			UINT_MAX
#define WOE_CHARNAME_STRING_SIZE	33
#define WOE_PLAYER_NULL				0xFFFFFFFF
#define WOE_REVIVE_TIME				10.0f
#define WOE_PACKET_PLAYER_DATA_NUM	32
#define WOE_PACKET_RANK_NUM			26

enum WOE_STATE 
{
	WOE_STATE_ENDED		= 0,
	WOE_STATE_REGISTER	= 1,
	WOE_STATE_BATTLE	= 2,
	WOE_STATE_REWARD	= 3,
	WOE_STATE_SIZE		= 4,
};

//enum 
//{
//	WOE_SCHOOL_SG	= 0,
//	WOE_SCHOOL_MP	= 1,
//	WOE_SCHOOL_PHX	= 2,
//	WOE_SCHOOL_SIZE	= 3,
//};
//
//enum 
//{
//	WOE_TOWER_0	= 0,
//	WOE_TOWER_1	= 1,
//	WOE_TOWER_2	= 2,
//	WOE_TOWER_3 = 3,
//	WOE_TOWER_SIZE	= 4,
//};

struct SWOE_GUILD_RANK_EX
{
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	DWORD	m_dwMarkVER;
	char	szClubName[CLUB_NAME + 1];
	char	szLeaderName[CHAR_SZNAME + 1];

	SWOE_GUILD_RANK_EX()
		: dwClubID(0)
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, m_dwMarkVER(0)
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
		m_dwMarkVER = 0;

		StringCchCopy(szClubName, CLUB_NAME + 1, "");
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, "");
	}

	SWOE_GUILD_RANK_EX& operator = (const SWOE_GUILD_RANK_EX& rvalue)
	{
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		m_dwMarkVER = rvalue.m_dwMarkVER;
		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		return *this;
	}

};

struct SWOE_GUILD_RANK_INFO
{
	int		nIndex;
	DWORD	dwClubID;
	WORD	wClubRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	DWORD	m_dwMarkVER;
	char	szClubName[CLUB_NAME + 1];
	char	szLeaderName[CHAR_SZNAME + 1];

	SWOE_GUILD_RANK_INFO()
		: nIndex(-1)
		, dwClubID(CLUB_NULL)
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, m_dwMarkVER(0)
	{
		memset(szClubName, 0, sizeof(char) * (CLUB_NAME + 1));
		memset(szLeaderName, 0, sizeof(char) * (CHAR_SZNAME + 1));
	}

	SWOE_GUILD_RANK_INFO(const SWOE_GUILD_RANK_INFO& value)
	{
		operator=(value);
	}

	SWOE_GUILD_RANK_INFO& operator = (const SWOE_GUILD_RANK_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		m_dwMarkVER = rvalue.m_dwMarkVER;

		return *this;
	}

	bool operator < (const SWOE_GUILD_RANK_INFO& sCWRank)
	{
		if (wKillNum > sCWRank.wKillNum) return true;
		else if (wKillNum == sCWRank.wKillNum && wDeathNum < sCWRank.wDeathNum) return true;

		return  false;
	}

	bool operator == (const SWOE_GUILD_RANK_INFO& sCWRank)
	{
		if (wKillNum == sCWRank.wKillNum && wDeathNum == sCWRank.wDeathNum) return true;

		return false;

	}
};

struct SWOE_GUILD_RANK
{
	int nIndex;
	DWORD dwClubID;
	WORD wClubRanking;
	char szClubName[CLUB_NAME + 1];
	char szLeaderName[CHAR_SZNAME + 1];

	WORD wKillNum;
	WORD wDeathNum;
	DWORD m_dwMarkVER;

	SWOE_GUILD_RANK()
		: nIndex(-1)
		, dwClubID(CLUB_NULL)
		, wClubRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, m_dwMarkVER(0)
	{
		memset(szClubName, 0, sizeof(char) * (CLUB_NAME + 1));
		memset(szLeaderName, 0, sizeof(char) * (CHAR_SZNAME + 1));
	}

	SWOE_GUILD_RANK(const SWOE_GUILD_RANK& value)
	{
		operator=(value);
	}

	SWOE_GUILD_RANK& operator = (const SWOE_GUILD_RANK& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		m_dwMarkVER = rvalue.m_dwMarkVER;

		return *this;
	}

	SWOE_GUILD_RANK(const SWOE_GUILD_RANK_INFO& value)
	{
		operator=(value);
	}

	SWOE_GUILD_RANK& operator = (const SWOE_GUILD_RANK_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwClubID = rvalue.dwClubID;
		wClubRanking = rvalue.wClubRanking;
		StringCchCopy(szClubName, CLUB_NAME + 1, rvalue.szClubName);
		StringCchCopy(szLeaderName, CHAR_SZNAME + 1, rvalue.szLeaderName);

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		m_dwMarkVER = rvalue.m_dwMarkVER;

		return *this;
	}
};

struct SWOE_PLAYER_RANK_EX
{
	WORD	wRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wSchool;
	WORD	wClass;
	char	szName[CHAR_NAME + 1];
	DWORD	dwHitCount;

	SWOE_PLAYER_RANK_EX()
		: wRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wSchool(0)
		, wClass(GLCC_ALL_6CLASS)
		, dwHitCount(0)
	{
		memset(szName, 0, sizeof(char) * (CHAR_NAME + 1));
	}

	void Init()
	{
		wRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;
		wSchool = 0;
		wClass = GLCC_ALL_6CLASS;
		dwHitCount = 0;

		StringCchCopy(szName, CHAR_NAME + 1, "");
	}

	SWOE_PLAYER_RANK_EX& operator = (const SWOE_PLAYER_RANK_EX& rvalue)
	{
		wRanking = rvalue.wRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wSchool = rvalue.wSchool;
		wClass = rvalue.wClass;
		dwHitCount = rvalue.dwHitCount;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		return *this;
	}

};

struct SWOE_PLAYER_RANK_INFO
{
	int		nIndex;
	DWORD	dwCharID;
	WORD	wRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wSchool;
	WORD	wClass;
	DWORD	dwCountHit;
	char	szName[CHAR_NAME + 1];

	SWOE_PLAYER_RANK_INFO()
		: nIndex(-1)
		, dwCharID(GAEAID_NULL)
		, wRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wSchool(0)
		, wClass(GLCC_ALL_6CLASS)
		, dwCountHit(0)
	{
		memset(szName, 0, sizeof(char) * (CHAR_NAME + 1));
	}

	SWOE_PLAYER_RANK_INFO(const SWOE_PLAYER_RANK_INFO& value)
	{
		operator=(value);
	}

	SWOE_PLAYER_RANK_INFO& operator = (const SWOE_PLAYER_RANK_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wRanking = rvalue.wRanking;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wSchool = rvalue.wSchool;
		wClass = rvalue.wClass;
		dwCountHit = rvalue.dwCountHit;

		return *this;
	}

	bool operator < (const SWOE_PLAYER_RANK_INFO& sTWPRank)
	{
		if (dwCountHit > sTWPRank.dwCountHit) return true;
		else if (dwCountHit == sTWPRank.dwCountHit && wDeathNum < sTWPRank.wDeathNum) return true;

		return  false;
	}

	bool operator == (const SWOE_PLAYER_RANK_INFO& sTWPRank)
	{
		if (dwCountHit == sTWPRank.dwCountHit && wDeathNum == sTWPRank.wDeathNum) return true;

		return false;

	}
};
struct SWOE_PLAYER_RANK
{
	int		nIndex;
	DWORD	dwCharID;
	WORD	wRanking;
	char	szName[CHAR_NAME + 1];

	WORD wKillNum;
	WORD wDeathNum;
	WORD wSchool;
	WORD wClass;
	DWORD dwCountHit;

	SWOE_PLAYER_RANK()
		: nIndex(-1)
		, dwCharID(GAEAID_NULL)
		, wRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wSchool(0)
		, wClass(GLCC_ALL_6CLASS)
		, dwCountHit(0)
	{
		memset(szName, 0, sizeof(char) * (CHAR_NAME + 1));
	}

	SWOE_PLAYER_RANK(const SWOE_PLAYER_RANK& value)
	{
		operator=(value);
	}

	SWOE_PLAYER_RANK& operator = (const SWOE_PLAYER_RANK& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wRanking = rvalue.wRanking;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wSchool = rvalue.wSchool;
		wClass = rvalue.wClass;
		dwCountHit = rvalue.dwCountHit;

		return *this;
	}

	SWOE_PLAYER_RANK(const SWOE_PLAYER_RANK_INFO& value)
	{
		operator=(value);
	}

	SWOE_PLAYER_RANK& operator = (const SWOE_PLAYER_RANK_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wRanking = rvalue.wRanking;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wSchool = rvalue.wSchool;
		wClass = rvalue.wClass;
		dwCountHit = rvalue.dwCountHit;

		return *this;
	}
};
struct SWOE_RESU_RANK_EX
{
	WORD	wRanking;
	WORD	wResuNum;
	WORD	wClass;
	WORD	wSchool;
	char	szName[CHAR_NAME + 1];

	SWOE_RESU_RANK_EX()
		: wRanking(0)
		, wResuNum(0)
		, wClass(0)
		, wSchool(0)
	{
		memset(szName, 0, sizeof(char) * (CHAR_NAME + 1));
	}

	void Init()
	{
		wRanking = 0;
		wResuNum = 0;
		wClass = 0;
		wSchool = 0;

		StringCchCopy(szName, CHAR_NAME + 1, "");
	}

	SWOE_RESU_RANK_EX& operator = (const SWOE_RESU_RANK_EX& rvalue)
	{
		wRanking = rvalue.wRanking;
		wResuNum = rvalue.wResuNum;
		wClass = rvalue.wClass;
		wSchool = rvalue.wSchool;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		return *this;
	}
};

struct SWOE_RESU_PLAYER_INFO
{
	int		nIndex;
	DWORD	dwCharID;
	WORD	wRanking;
	WORD	wResuNum;
	WORD	wClass;
	WORD	wSchool;
	char	szName[CHAR_NAME + 1];

	SWOE_RESU_PLAYER_INFO()
		: nIndex(-1)
		, dwCharID(GAEAID_NULL)
		, wRanking(0)
		, wResuNum(0)
		, wClass(0)
		, wSchool(0)
	{
		memset(szName, 0, sizeof(char) * (CHAR_NAME + 1));
	}

	SWOE_RESU_PLAYER_INFO(const SWOE_RESU_PLAYER_INFO& value)
	{
		operator=(value);
	}

	SWOE_RESU_PLAYER_INFO& operator = (const SWOE_RESU_PLAYER_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wRanking = rvalue.wRanking;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		wResuNum = rvalue.wResuNum;
		wClass = rvalue.wClass;
		wSchool = rvalue.wSchool;

		return *this;
	}

	bool operator < (const SWOE_RESU_PLAYER_INFO& sTWPRank)
	{
		if (wResuNum > sTWPRank.wResuNum) return true;
		else if (wResuNum == sTWPRank.wResuNum) return true;

		return  false;
	}

	bool operator == (const SWOE_RESU_PLAYER_INFO& sTWPRank)
	{
		if (wResuNum == sTWPRank.wResuNum) return true;

		return false;

	}
};

struct SWOE_RANK_RESU
{
	int		nIndex;
	DWORD	dwCharID;
	WORD	wRanking;
	char	szName[CHAR_NAME + 1];

	WORD wResuNum;
	WORD wClass;
	WORD wSchool;

	SWOE_RANK_RESU()
		: nIndex(-1)
		, dwCharID(GAEAID_NULL)
		, wRanking(0)
		, wResuNum(0)
		, wClass(0)
		, wSchool(0)
	{
		memset(szName, 0, sizeof(char) * (CHAR_NAME + 1));
	}

	SWOE_RANK_RESU(const SWOE_RANK_RESU& value)
	{
		operator=(value);
	}

	SWOE_RANK_RESU& operator = (const SWOE_RANK_RESU& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wRanking = rvalue.wRanking;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		wResuNum = rvalue.wResuNum;
		wClass = rvalue.wClass;
		wSchool = rvalue.wSchool;

		return *this;
	}

	SWOE_RANK_RESU(const SWOE_RESU_PLAYER_INFO& value)
	{
		operator=(value);
	}

	SWOE_RANK_RESU& operator = (const SWOE_RESU_PLAYER_INFO& rvalue)
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		wRanking = rvalue.wRanking;
		StringCchCopy(szName, CHAR_NAME + 1, rvalue.szName);

		wResuNum = rvalue.wResuNum;
		wClass = rvalue.wClass;
		wSchool = rvalue.wSchool;

		return *this;
	}
};

typedef std::vector<SWOE_GUILD_RANK_INFO>		WOE_GUILD_RANK_INFO_VEC;
typedef WOE_GUILD_RANK_INFO_VEC::iterator		WOE_GUILD_RANK_INFO_VEC_ITER;

typedef std::map<DWORD, SWOE_GUILD_RANK_INFO>	WOE_GUILD_RANK_INFO_MAP;
typedef WOE_GUILD_RANK_INFO_MAP::iterator		WOE_GUILD_RANK_INFO_MAP_ITER;

typedef std::vector<SWOE_GUILD_RANK>			WOE_GUILD_RANK_VEC;
typedef WOE_GUILD_RANK_VEC::iterator			WOE_GUILD_RANK_VEC_ITER;

typedef std::vector<SWOE_PLAYER_RANK_INFO>		WOE_PLAYER_RANK_INFO_VEC;
typedef WOE_PLAYER_RANK_INFO_VEC::iterator		WOE_PLAYER_RANK_INFO_VEC_ITER;

typedef std::map<DWORD, SWOE_PLAYER_RANK_INFO>	WOE_PLAYER_RANK_INFO_MAP;
typedef WOE_PLAYER_RANK_INFO_MAP::iterator		WOE_PLAYER_RANK_INFO_MAP_ITER;

typedef std::vector<SWOE_PLAYER_RANK>			WOE_PLAYER_RANK_VEC;
typedef WOE_PLAYER_RANK_VEC::iterator			WOE_PLAYER_RANK_VEC_ITER;

typedef std::vector<SWOE_RESU_PLAYER_INFO>		WOE_RESU_PLAYER_INFO_VEC;
typedef WOE_RESU_PLAYER_INFO_VEC::iterator		WOE_RESU_PLAYER_INFO_VEC_ITER;

typedef std::map<DWORD, SWOE_RESU_PLAYER_INFO>	WOE_RESU_PLAYER_INFO_MAP;
typedef WOE_RESU_PLAYER_INFO_MAP::iterator		WOE_RESU_PLAYER_INFO_MAP_ITER;

typedef std::vector<SWOE_RANK_RESU>				WOE_RANK_RESU_VEC;
typedef WOE_RANK_RESU_VEC::iterator				WOE_RANK_RESU_VEC_ITER;

typedef std::set<DWORD>							WOE_AWARD_CHAR;
typedef WOE_AWARD_CHAR::iterator				WOE_AWARD_CHAR_ITER;