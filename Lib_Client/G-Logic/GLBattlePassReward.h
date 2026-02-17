#ifndef GLBATTLEPASSREWARD_H_INCLUDED__
#define GLBATTLEPASSREWARD_H_INCLUDED__

/*12-9-14, Battle Pass - CNDev*/

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/G-Logic/GLDefine.h"

struct SBATTLEPASS_LEVEL_REWARD
{
	DWORD		dwLevel;
	SNATIVEID	sNormalRewardID;
	SNATIVEID	sPremiumRewardID;

	SBATTLEPASS_LEVEL_REWARD()
		: dwLevel(0)
		, sNormalRewardID(false)
		, sPremiumRewardID(false)
	{
	}

	bool operator < ( const SBATTLEPASS_LEVEL_REWARD& sData )
	{			
		if ( dwLevel < sData.dwLevel )
			return true;

		return  false;
	}
};

typedef std::map<DWORD, SBATTLEPASS_LEVEL_REWARD>	BATTLEPASS_LEVEL_REWARD_MAP;
typedef BATTLEPASS_LEVEL_REWARD_MAP::iterator		BATTLEPASS_LEVEL_REWARD_MAP_ITER;
typedef std::vector<SBATTLEPASS_LEVEL_REWARD>		BATTLEPASS_LEVEL_REWARD_VEC;


class GLBattlePassReward
{
public:
	GLBattlePassReward();
	~GLBattlePassReward();

	static GLBattlePassReward& GetInstance ();

public:
	BATTLEPASS_LEVEL_REWARD_MAP	m_mapBattlePassLevelRewardMap;

public:
	void CleanUp();
	void LoadFile( std::string strFileName );
};


#endif // GLBATTLEPASSREWARD_H_INCLUDED__
