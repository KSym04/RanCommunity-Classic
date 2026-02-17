#include "stdafx.h"
#include "./GLCombatRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace nscombat_record
{
	SDATA::SDATA()
		: dwCharID(0)

		, dwPVPKills(0)
		, dwPVPDeaths(0)
		, dwPVPResu(0)

		, dwTyrannyKills(0)
		, dwTyrannyDeaths(0)
		, dwTyrannyResu(0)

		, dwClubWarKills(0)
		, dwClubWarDeaths(0)
		, dwClubWarResu(0)

		, dwCaptureTheFlagKills(0)
		, dwCaptureTheFlagDeaths(0)
		, dwCaptureTheFlagResu(0)

		, dwSchoolWarsKills(0)
		, dwSchoolWarsDeaths(0)
		, dwSchoolWarsResu(0)

		, dwClubDeathMatchKills(0)
		, dwClubDeathMatchDeaths(0)
		, dwClubDeathMatchResu(0)

		//, dwConfrontWin(0)
		//, dwConfrontLoss(0)
		//, dwConfrontDraw(0)

		, dwPKMapKills(0)
		, dwPKMapDeaths(0)
		, dwPKMapResu(0)

		, dwPVEKills(0)
		, dwPVEDeaths(0)
		, llPVEMoneyGain(0)

		, bUpdateCombatRecord(false)
		, fCombatRecordUpdateTime(COMBAT_RECORD_UPDATE_TIME)
	{

	}

	void SDATA::resetUpdate()
	{
		bUpdateCombatRecord = false;
		fCombatRecordUpdateTime = COMBAT_RECORD_UPDATE_TIME;
	}

	//returns true if update to client side is needed
	bool SDATA::updateTimer(float fElapsed)
	{
		fCombatRecordUpdateTime += fElapsed;

		if (fCombatRecordUpdateTime >= COMBAT_RECORD_UPDATE_TIME)
		{
			if ( bUpdateCombatRecord )
			{
				bUpdateCombatRecord = false;
				return true;
			}

			fCombatRecordUpdateTime = 0.0f;
		}

		return false;
	}

	


	void SDATA::increasePVEKills()
	{
		dwPVEKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increasePVEDeaths()
	{
		dwPVEDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increasePVEMoneyGain(LONGLONG llVal)
	{
		llPVEMoneyGain += llVal;
		bUpdateCombatRecord = true;
	}


	void SDATA::increasePVPKills()
	{
		dwPVPKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increasePVPDeaths()
	{
		dwPVPDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increasePVPResu()
	{
		dwPVPResu ++;
		bUpdateCombatRecord = true;
	}


	void SDATA::increaseTyrannyKills()
	{
		dwTyrannyKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseTyrannyDeaths()
	{
		dwTyrannyDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseTyrannyResu()
	{
		dwTyrannyResu ++;
		bUpdateCombatRecord = true;
	}


	void SDATA::increaseClubWarKills()
	{
		dwClubWarKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseClubWarDeaths()
	{
		dwClubWarDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseClubWarResu()
	{
		dwClubWarResu ++;
		bUpdateCombatRecord = true;
	}


	void SDATA::increaseCaptureTheFlagKills()
	{
		dwCaptureTheFlagKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseCaptureTheFlagDeaths()
	{
		dwCaptureTheFlagDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseCaptureTheFlagResu()
	{
		dwCaptureTheFlagResu ++;
		bUpdateCombatRecord = true;
	}


	void SDATA::increaseClubDeathMatchKills()
	{
		dwClubDeathMatchKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseClubDeathMatchDeaths()
	{
		dwClubDeathMatchDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseClubDeathMatchResu()
	{
		dwClubDeathMatchResu ++;
		bUpdateCombatRecord = true;
	}


	void SDATA::increaseSchoolWarsKills()
	{
		dwSchoolWarsKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseSchoolWarsDeaths()
	{
		dwSchoolWarsDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increaseSchoolWarsResu()
	{
		dwSchoolWarsResu ++;
		bUpdateCombatRecord = true;
	}


	void SDATA::increasePKMapKills()
	{
		dwPKMapKills ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increasePKMapDeaths()
	{
		dwPKMapDeaths ++;
		bUpdateCombatRecord = true;
	}

	void SDATA::increasePKMapResu()
	{
		dwPKMapResu ++;
		bUpdateCombatRecord = true;
	}
}