#ifndef GLBATTLEPASS_DATA_H_
#define GLBATTLEPASS_DATA_H_


/*12-9-14, Battle Pass - CNDev*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <hash_map>

#include "../../Lib_Engine/Common/ByteStream.h"
#include "../../Lib_Engine/Common/SerialFile.h"

#include "./GLBattlePassDefine.h"
#include "../Lib_Engine/G-Logic/GLDefine.h"

struct SBATTLEPASS_FILE_DATA
{
	enum { VERSION = 0x0001 };

	DWORD					dwBattlePassID;
	std::string				strBattlePassTitle;
	std::string				strBattlePassDescription;

	EMBATTLEPASS_TYPE		emType;

	DWORD					dwRewardPoint;	

	SNATIVEID				sidMobKill;
	WORD					wProgressMobKill;

	SNATIVEID				sidMapKill;
	WORD					wProgressMapKill;

	SNATIVEID				sidMapReach;
	WORD					wProgressMapReach;

	SNATIVEID				sidItemGet;
	WORD					wProgressItemGet;

	SNATIVEID				sidItemUse;
	WORD					wProgressItemUse;

	WORD					wProgressTrade;

	WORD					wProgressWhisper;

	WORD					wProgressPrivateMarket;

	WORD					wProgressGainPlayTime;
	WORD					wProgressGainGold;
	WORD					wProgressGainContriPoint;
	
	SBATTLEPASS_FILE_DATA()
		: dwBattlePassID(UINT_MAX)
		, emType(EMBATTLEPASS_TYPE_KILL_MOB)
		, dwRewardPoint(0)
		, sidMobKill(NATIVEID_NULL())
		, wProgressMobKill(0)
		, sidMapKill(NATIVEID_NULL())
		, wProgressMapKill(0)
		, sidMapReach(NATIVEID_NULL())
		, wProgressMapReach(0)
		, sidItemGet(NATIVEID_NULL())
		, wProgressItemGet(0)
		, sidItemUse(NATIVEID_NULL())
		, wProgressItemUse(0)
		, wProgressTrade(0)
		, wProgressWhisper(0)
		, wProgressPrivateMarket(0)
		, wProgressGainPlayTime(0)
		, wProgressGainGold(0)
		, wProgressGainContriPoint(0)
		
	{
		strBattlePassTitle = "";
		strBattlePassDescription = "";
	};	

	bool operator < ( const SBATTLEPASS_FILE_DATA& sData )
	{			
		if ( emType < sData.emType )
			return true;
		else if ( emType == sData.emType && dwBattlePassID < sData.dwBattlePassID )
			return true;

		return  false;
	}

	SBATTLEPASS_FILE_DATA& operator = ( const SBATTLEPASS_FILE_DATA& rvalue )
	{
		dwBattlePassID				= rvalue.dwBattlePassID;
		strBattlePassDescription	= rvalue.strBattlePassDescription;
		strBattlePassTitle          = rvalue.strBattlePassTitle;

		emType						= rvalue.emType;

		dwRewardPoint				= rvalue.dwRewardPoint;

		sidMobKill					= rvalue.sidMobKill;
		wProgressMobKill			= rvalue.wProgressMobKill;

		sidMapKill					= rvalue.sidMapKill;
		wProgressMapKill			= rvalue.wProgressMapKill;

		sidMapReach					= rvalue.sidMapReach;
		wProgressMapReach			= rvalue.wProgressMapReach;

		sidItemGet					= rvalue.sidItemGet;
		wProgressItemGet			= rvalue.wProgressItemGet;

		sidItemUse					= rvalue.sidItemUse;
		wProgressItemUse			= rvalue.wProgressItemUse;

		wProgressTrade				= rvalue.wProgressTrade;
		wProgressWhisper			= rvalue.wProgressWhisper;
		wProgressPrivateMarket		= rvalue.wProgressPrivateMarket;

		wProgressGainPlayTime		= rvalue.wProgressGainPlayTime;
		wProgressGainGold			= rvalue.wProgressGainGold;
		wProgressGainContriPoint	= rvalue.wProgressGainContriPoint;

		return *this;
	}

	BOOL LOAD ( basestream &SFile, BOOL bServer );
	BOOL SAVE ( CSerialFile &SFile );
};

typedef std::map<DWORD,SBATTLEPASS_FILE_DATA>	SBATTLEPASS_FILE_DATA_MAP;
typedef SBATTLEPASS_FILE_DATA_MAP::iterator		SBATTLEPASS_FILE_DATA_MAP_ITER;
typedef std::vector<SBATTLEPASS_FILE_DATA>		SBATTLEPASS_FILE_DATA_VEC;


struct SBATTLEPASS_REWARD_CHAR_DATA 
{
	DWORD dwLevel;
	bool bNormalClaimed;
	bool bPremiumClaimed;

	SBATTLEPASS_REWARD_CHAR_DATA()
		: dwLevel(0)
		, bNormalClaimed(false)
		, bPremiumClaimed(false) 

	{
	}
};

typedef stdext::hash_map<DWORD,SBATTLEPASS_REWARD_CHAR_DATA>		SBATTLEPASS_REWARD_CHAR_DATA_MAP;
typedef SBATTLEPASS_REWARD_CHAR_DATA_MAP::iterator					SBATTLEPASS_REWARD_CHAR_DATA_MAP_ITER;
typedef SBATTLEPASS_REWARD_CHAR_DATA_MAP::const_iterator			SBATTLEPASS_REWARD_CHAR_DATA_MAP_CITER;

struct SBATTLEPASS_CHAR_DATA
{
	enum { VERSION = 0x0001 };

	DWORD				dwBattlePassID;
	EMBATTLEPASS_TYPE	emType;
	SNATIVEID			sidProgress;
	DWORD				dwProgressNow;
	DWORD				dwProgressMax;

	SBATTLEPASS_CHAR_DATA()
		: dwBattlePassID(UINT_MAX)
		, emType(EMBATTLEPASS_TYPE_KILL_MOB)
		, dwProgressNow(0)
		, dwProgressMax(0)
	{
	};

	void Assign( SBATTLEPASS_FILE_DATA& sbattlepass_file_data );
	void Correction( SBATTLEPASS_FILE_DATA& sbattlepass_file_data );
};

typedef stdext::hash_map<DWORD,SBATTLEPASS_CHAR_DATA>	SBATTLEPASS_CHAR_DATA_MAP;
typedef SBATTLEPASS_CHAR_DATA_MAP::iterator				SBATTLEPASS_CHAR_DATA_MAP_ITER;
typedef SBATTLEPASS_CHAR_DATA_MAP::const_iterator		SBATTLEPASS_CHAR_DATA_MAP_CITER;

#endif // GLBATTLEPASS_DATA_H_
