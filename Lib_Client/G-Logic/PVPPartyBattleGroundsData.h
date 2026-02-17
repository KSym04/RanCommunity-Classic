/*!
 * \file PVPPartyBattleGroundsData.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSDATA_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSDATA_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPPartyBattleGroundsDefine.h"

namespace PVPPBG
{
	struct EVENT_SCHEDEDULE
	{  
		WORD	wStartDay;		//schedule day
		WORD	wStartHour;		//schedule hour (24 hour format)
		WORD	wStartMinute;	//schedule minute

		BOOL	bCombatRecord;

		EVENT_SCHEDEDULE()
			: wStartDay(0)
			, wStartHour(0)
			, wStartMinute(0)
			, bCombatRecord(FALSE)
		{
		}

		bool operator < ( const EVENT_SCHEDEDULE& sData )
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

		void Reset();
	};

	typedef std::vector<EVENT_SCHEDEDULE> EVENT_SCHEDEDULE_VEC;

	struct EVENT_SCHEDEDULE_NEXT
	{
		DWORD		dwIndex;

		EVENT_SCHEDEDULE	sStartSched;

		WORD		wPrevStartHour;
		WORD		wPrevStartMinute;
		WORD		wPrevEndHour;
		WORD		wPrevEndMinute;

		WORD		wBattleStartHour;
		WORD		wBattleStartMinute;
		WORD		wBattleEndHour;
		WORD		wBattleEndMinute;

		EVENT_SCHEDEDULE_NEXT()
			: dwIndex(PVP_PBG_SCHEDULE_NONE)
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

		void Reset();
		void CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration );
	};

	struct REGISTER_DATA
	{  
		DWORD	dwInfoCharID;
		char	szInfoCharName[PVP_PBG_CHAR_NAME_SIZE];
		WORD	wInfoSchool;
		WORD	wInfoClass;

		float	fForceMoveLobbyAreaTime;
		float	fForceMoveNoPartyTime;

		REGISTER_DATA()
			: dwInfoCharID(PVP_PBG_CHAR_ID_NULL)
			, wInfoSchool(0)
			, wInfoClass(0)
			, fForceMoveLobbyAreaTime(0.0f)
			, fForceMoveNoPartyTime(0.0f)
		{
			memset( szInfoCharName, 0, sizeof(char) * (PVP_PBG_CHAR_NAME_SIZE) );
		}

		REGISTER_DATA ( const REGISTER_DATA &value )
		{
			operator=(value);
		}

		REGISTER_DATA& operator = ( const REGISTER_DATA& rvalue )
		{
			dwInfoCharID	= rvalue.dwInfoCharID;
			wInfoSchool		= rvalue.wInfoSchool;
			wInfoClass		= rvalue.wInfoClass;

			StringCchCopy( szInfoCharName, PVP_PBG_CHAR_NAME_SIZE, rvalue.szInfoCharName );

			fForceMoveLobbyAreaTime = rvalue.fForceMoveLobbyAreaTime;
			fForceMoveNoPartyTime = rvalue.fForceMoveNoPartyTime;

			return *this;
		}
	};

	struct REGISTER_DATA_QUEUE
	{ 
		REGISTER_DATA sPlayerData;
		WORD	wQueueNum;

		REGISTER_DATA_QUEUE()
			: sPlayerData()
			, wQueueNum(0)
		{
		}
	};

	struct PLAYER_DATA
	{  
		//info
		DWORD	dwInfoCharID;
		char	szInfoCharName[PVP_PBG_CHAR_NAME_SIZE];
		WORD	wInfoSchool;
		WORD	wInfoClass;

		//rank
		WORD	wRank;

		//score
		WORD	wScoreKill;
		WORD	wScoreDeath;
		DWORD	dwScoreHeal;
		WORD	wScoreResu;

		DWORD	dwScoreTotal;
		WORD	wRewardPoint;

		PLAYER_DATA()
			: dwInfoCharID(PVP_PBG_CHAR_ID_NULL)
			, wInfoSchool(0)
			, wInfoClass(0)

			, wRank(0)

			, wScoreKill(0)
			, wScoreDeath(0)
			, dwScoreHeal(0)
			, wScoreResu(0)
			, dwScoreTotal(0)
			, wRewardPoint(0)
		{
			memset( szInfoCharName, 0, sizeof(char) * (PVP_PBG_CHAR_NAME_SIZE) );
		}

		bool operator < ( const PLAYER_DATA& sData )
		{			
			if ( dwScoreTotal > sData.dwScoreTotal )
				return true;
			else if ( dwScoreTotal == sData.dwScoreTotal && wScoreDeath < sData.wScoreDeath )	
				return true;

			return  false;
		}

		PLAYER_DATA ( const PLAYER_DATA &value )
		{
			operator=(value);
		}

		PLAYER_DATA& operator = ( const PLAYER_DATA& rvalue )
		{
			dwInfoCharID	= rvalue.dwInfoCharID;
			wInfoSchool		= rvalue.wInfoSchool;
			wInfoClass		= rvalue.wInfoClass;

			wRank			= rvalue.wRank;

			wScoreKill		= rvalue.wScoreKill;
			wScoreDeath		= rvalue.wScoreDeath;
			dwScoreHeal		= rvalue.dwScoreHeal;
			wScoreResu		= rvalue.wScoreResu;
			dwScoreTotal	= rvalue.dwScoreTotal;
			wRewardPoint	= rvalue.wRewardPoint;

			StringCchCopy( szInfoCharName, PVP_PBG_CHAR_NAME_SIZE, rvalue.szInfoCharName );

			return *this;
		}
	};


	typedef std::map<DWORD,REGISTER_DATA>		REGISTER_DATA_MAP;
	typedef REGISTER_DATA_MAP::iterator			REGISTER_DATA_MAP_ITER;
	typedef std::vector<REGISTER_DATA>			REGISTER_DATA_VEC;
	typedef std::vector<REGISTER_DATA_QUEUE>	REGISTER_DATA_QUEUE_VEC;

	typedef std::map<DWORD,PLAYER_DATA>			PLAYER_DATA_MAP;
	typedef PLAYER_DATA_MAP::iterator			PLAYER_DATA_MAP_ITER;
	typedef std::vector<PLAYER_DATA>			PLAYER_DATA_VEC;

	struct PLAYER_HASH_DATA
	{
		DWORD dwCharID;
		std::string strHash;

		PLAYER_HASH_DATA()
			: dwCharID(PVP_PBG_CHAR_ID_NULL)
			, strHash("")
		{
		}
	};

	typedef std::vector<PLAYER_HASH_DATA>			PLAYER_HASH_DATA_VEC;
	typedef std::map<std::string, PLAYER_HASH_DATA>	PLAYER_HASH_DATA_MAP;
	typedef PLAYER_HASH_DATA_MAP::iterator			PLAYER_HASH_DATA_MAP_ITER;



};


#endif // PVPPARTYBATTLEGROUNDSDATA_H_INCLUDED__
