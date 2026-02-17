#ifndef GLDAILYLOGIN_H_INCLUDED__
#define GLDAILYLOGIN_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif

#include <map>
#include <hash_map>

#include "../../Lib_Engine/G-Logic/GLDefine.h"

#define DAILYLOGIN_MAX_DAY	28

/*12-16-24, daily login - CNDev*/

struct SDAILYLOGIN_CHAR_DATA
{
	enum { VERSION = 0x0001 };

	DWORD			dwDay;
	bool			bRewardClaimed;
	CTime			cDailyLoginTime;

	SDAILYLOGIN_CHAR_DATA()
		: dwDay(0)
		, bRewardClaimed(false)
		, cDailyLoginTime(0)
	{
	};
};

typedef stdext::hash_map<DWORD,SDAILYLOGIN_CHAR_DATA>	SDAILYLOGIN_CHAR_DATA_MAP;
typedef SDAILYLOGIN_CHAR_DATA_MAP::iterator				SDAILYLOGIN_CHAR_DATA_MAP_ITER;
typedef SDAILYLOGIN_CHAR_DATA_MAP::const_iterator		SDAILYLOGIN_CHAR_DATA_MAP_CITER;

struct SDAILYLOGIN_REWARD
{
	DWORD		dwDay;
	SNATIVEID	sRewardID;

	SDAILYLOGIN_REWARD()
		: dwDay(0)
		, sRewardID(false)
	{
	}

	bool operator < ( const SDAILYLOGIN_REWARD& sData )
	{			
		if ( dwDay < sData.dwDay )
			return true;

		return  false;
	}
};

typedef std::map<DWORD, SDAILYLOGIN_REWARD>			DAILYLOGIN_REWARD_MAP;
typedef DAILYLOGIN_REWARD_MAP::iterator				DAILYLOGIN_REWARD_MAP_ITER;
typedef std::vector<SDAILYLOGIN_REWARD>				DAILYLOGIN_REWARD_VEC;


class GLDailyLogin
{
public:
	GLDailyLogin();
	~GLDailyLogin();

	static GLDailyLogin& GetInstance ();

public:
	DAILYLOGIN_REWARD_MAP	m_mapDailyLoginReward;

public:
	void CleanUp();
	void LoadFile( std::string strFileName );
};


#endif // GLDAILYLOGIN_H_INCLUDED__
