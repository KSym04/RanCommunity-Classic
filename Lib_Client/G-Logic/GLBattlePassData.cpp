#include "StdAfx.h"
#include "./GLBattlePassData.h"

#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

BOOL SBATTLEPASS_FILE_DATA::LOAD ( basestream &SFile, BOOL bServer )
{
	DWORD dwVer(0);
	SFile >> dwVer;

	if ( dwVer == VERSION )
	{
		SFile >> dwBattlePassID;
		SFile >> strBattlePassTitle;
		SFile >> strBattlePassDescription;

		DWORD dwData(0);
		SFile >> dwData;	emType  = (EMBATTLEPASS_TYPE)dwData;

		SFile >> dwRewardPoint;	

		SFile >> sidMobKill.dwID;
		SFile >> wProgressMobKill;

		SFile >> sidMapKill.dwID;
		SFile >> wProgressMapKill;

		SFile >> sidMapReach.dwID;
		SFile >> wProgressMapReach;

		SFile >> sidItemGet.dwID;
		SFile >> wProgressItemGet;

		SFile >> sidItemUse.dwID;
		SFile >> wProgressItemUse;

		SFile >> wProgressTrade;
		SFile >> wProgressWhisper;
		SFile >> wProgressPrivateMarket;

		SFile >> wProgressGainPlayTime;
		SFile >> wProgressGainGold;
		SFile >> wProgressGainContriPoint;
	}
	else
	{
		CDebugSet::ErrorVersion( "SBATTLEPASS_FILE_DATA::LOAD", dwVer );
	}

	return TRUE;
}

BOOL SBATTLEPASS_FILE_DATA::SAVE ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;

	SFile << dwBattlePassID;
	SFile << strBattlePassTitle;
	SFile << strBattlePassDescription;

	SFile << (DWORD)emType;

	SFile << dwRewardPoint;	

	SFile << sidMobKill.dwID;
	SFile << wProgressMobKill;

	SFile << sidMapKill.dwID;
	SFile << wProgressMapKill;

	SFile << sidMapReach.dwID;
	SFile << wProgressMapReach;

	SFile << sidItemGet.dwID;
	SFile << wProgressItemGet;

	SFile << sidItemUse.dwID;
	SFile << wProgressItemUse;

	SFile << wProgressTrade;
	SFile << wProgressWhisper;
	SFile << wProgressPrivateMarket;

	SFile << wProgressGainPlayTime;
	SFile << wProgressGainGold;
	SFile << wProgressGainContriPoint;


	return TRUE;
}

void SBATTLEPASS_CHAR_DATA::Assign( SBATTLEPASS_FILE_DATA& sbattlepass_file_data )
{
	dwBattlePassID = sbattlepass_file_data.dwBattlePassID;
	emType = sbattlepass_file_data.emType;

	switch ( emType )
	{
	case EMBATTLEPASS_TYPE_KILL_MOB:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressMobKill;
			sidProgress	 = sbattlepass_file_data.sidMobKill;
		}break;
	case EMBATTLEPASS_TYPE_KILL_PLAYER:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressMapKill;
			sidProgress	 = sbattlepass_file_data.sidMapKill;
		}break;
	case EMBATTLEPASS_TYPE_REACH_MAP:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressMapReach;
			sidProgress	 = sbattlepass_file_data.sidMapReach;
		}break;
	case EMBATTLEPASS_TYPE_TAKE_ITEM:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressItemGet;
			sidProgress	 = sbattlepass_file_data.sidItemGet;
		}break;
	case EMBATTLEPASS_TYPE_USE_ITEM:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressItemUse;
			sidProgress	 = sbattlepass_file_data.sidItemUse;
		}break;

	case EMBATTLEPASS_TYPE_TRADE_PLAYER:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressTrade;
		}break;

	case EMBATTLEPASS_TYPE_WHISPER_PLAYER:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressWhisper;
		}break;

	case EMBATTLEPASS_TYPE_PRIVATE_MARKET:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressPrivateMarket;
		}break;

	case EMBATTLEPASS_TYPE_GAIN_PLAYTIME:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressGainPlayTime;
		}break;

	case EMBATTLEPASS_TYPE_GAIN_GOLD:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressGainGold;
		}break;

	case EMBATTLEPASS_TYPE_GAIN_CONTRIPOINT:
		{
			dwProgressNow = 0;
			dwProgressMax = sbattlepass_file_data.wProgressGainContriPoint;
		}break;
	}
}

void SBATTLEPASS_CHAR_DATA::Correction( SBATTLEPASS_FILE_DATA& sbattlepass_file_data )
{
	if ( emType != sbattlepass_file_data.emType )
	{
		sidProgress = NATIVEID_NULL();
		dwProgressNow = 0;
		dwProgressMax = 0;

		Assign( sbattlepass_file_data );
		return;
	}

	switch ( emType )
	{
	case EMBATTLEPASS_TYPE_KILL_MOB:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressMobKill )	
				dwProgressMax = sbattlepass_file_data.wProgressMobKill;

			if ( sidProgress != sbattlepass_file_data.sidMobKill )	
				sidProgress	= sbattlepass_file_data.sidMobKill;
		}break;
	case EMBATTLEPASS_TYPE_KILL_PLAYER:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressMapKill )	
				dwProgressMax = sbattlepass_file_data.wProgressMapKill;

			if ( sidProgress != sbattlepass_file_data.sidMapKill )	
				sidProgress	 = sbattlepass_file_data.sidMapKill;
		}break;
	case EMBATTLEPASS_TYPE_REACH_MAP:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressMapReach )
				dwProgressMax = sbattlepass_file_data.wProgressMapReach;

			if ( sidProgress != sbattlepass_file_data.sidMapReach )
				sidProgress	= sbattlepass_file_data.sidMapReach;
		}break;
	case EMBATTLEPASS_TYPE_TAKE_ITEM:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressItemGet )
				dwProgressMax = sbattlepass_file_data.wProgressItemGet;

			if ( sidProgress != sbattlepass_file_data.sidItemGet )
				sidProgress	= sbattlepass_file_data.sidItemGet;
		}break;
	case EMBATTLEPASS_TYPE_USE_ITEM:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressItemUse )
				dwProgressMax = sbattlepass_file_data.wProgressItemUse;

			if ( sidProgress != sbattlepass_file_data.sidItemUse )
				sidProgress	 = sbattlepass_file_data.sidItemUse;
		}break;
	case EMBATTLEPASS_TYPE_TRADE_PLAYER:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressTrade )	
				dwProgressMax = sbattlepass_file_data.wProgressTrade;
		}break;
	case EMBATTLEPASS_TYPE_WHISPER_PLAYER:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressWhisper )	
				dwProgressMax = sbattlepass_file_data.wProgressWhisper;
		}break;
	case EMBATTLEPASS_TYPE_PRIVATE_MARKET:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressPrivateMarket )	
				dwProgressMax = sbattlepass_file_data.wProgressPrivateMarket;
		}break;
	case EMBATTLEPASS_TYPE_GAIN_PLAYTIME:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressGainPlayTime )	
				dwProgressMax = sbattlepass_file_data.wProgressGainPlayTime;
		}break;
	case EMBATTLEPASS_TYPE_GAIN_GOLD:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressGainGold )	
				dwProgressMax = sbattlepass_file_data.wProgressGainGold;
		}break;
	case EMBATTLEPASS_TYPE_GAIN_CONTRIPOINT:
		{
			if ( dwProgressMax != sbattlepass_file_data.wProgressGainContriPoint )	
				dwProgressMax = sbattlepass_file_data.wProgressGainContriPoint;
		}break;
	}
}