#include "stdafx.h"
#include "./PVPPartyBattleGroundsData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace PVPPBG
{
	void EVENT_SCHEDEDULE::Reset()
	{
		wStartDay		= 0;
		wStartHour		= 0;
		wStartMinute	= 0;
	}


	void EVENT_SCHEDEDULE_NEXT::Reset()
	{
		dwIndex = PVP_PBG_SCHEDULE_NONE;

		sStartSched.Reset();

		wPrevStartHour		= 0;
		wPrevStartMinute	= 0;
		wPrevEndHour		= 0;
		wPrevEndMinute		= 0;

		wBattleStartHour	= 0;
		wBattleStartMinute	= 0;
		wBattleEndHour		= 0;
		wBattleEndMinute	= 0;
	}

	void EVENT_SCHEDEDULE_NEXT::CalculateEndTime( WORD wRegisterDuration, WORD wBattleDuration, WORD wRewardDuration )
	{
		wPrevStartHour = wBattleStartHour;
		wPrevStartMinute = wBattleStartMinute;
		wPrevEndHour = wBattleEndHour;
		wPrevEndMinute = wBattleEndMinute;

		wBattleStartHour = sStartSched.wStartHour;
		wBattleStartMinute = sStartSched.wStartMinute;
		wBattleEndHour = sStartSched.wStartHour;
		wBattleEndMinute = sStartSched.wStartMinute;

		WORD wToBattleInterval = wRegisterDuration/60;
		WORD wOverAllInterval = ( wRegisterDuration + wBattleDuration + wRewardDuration )/60;

		wBattleStartMinute += wToBattleInterval; 
		wBattleEndMinute += wOverAllInterval;

		while ( wBattleStartMinute >= 60 )
		{
			wBattleStartHour += 1;
			wBattleStartMinute -= 60;
		}

		while ( wBattleStartHour >= 24 )
		{
			wBattleStartHour -= 24;
		}

		while ( wBattleEndMinute >= 60 )
		{
			wBattleEndHour += 1;
			wBattleEndMinute -= 60;
		}

		while ( wBattleEndHour >= 24 )
		{
			wBattleEndHour -= 24;
		}
	}



};