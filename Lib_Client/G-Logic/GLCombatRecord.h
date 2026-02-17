/*!
 * \file GLCombatRecord.h
 *
 * \author Juver
 * \date 2022/05/10
 *
 * 
 */

#ifndef GLCOMBATRECORD_H_INCLUDED__
#define GLCOMBATRECORD_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COMBAT_RECORD_UPDATE_TIME 1.0f

namespace nscombat_record
{
	//
	struct SDATA
	{
		DWORD dwCharID;

		DWORD dwPVPKills;
		DWORD dwPVPDeaths;
		DWORD dwPVPResu;

		DWORD dwTyrannyKills;
		DWORD dwTyrannyDeaths;
		DWORD dwTyrannyResu;

		DWORD dwClubWarKills;
		DWORD dwClubWarDeaths;
		DWORD dwClubWarResu;

		DWORD dwCaptureTheFlagKills;
		DWORD dwCaptureTheFlagDeaths;
		DWORD dwCaptureTheFlagResu;

		DWORD dwSchoolWarsKills;
		DWORD dwSchoolWarsDeaths;
		DWORD dwSchoolWarsResu;

		DWORD dwClubDeathMatchKills;
		DWORD dwClubDeathMatchDeaths;
		DWORD dwClubDeathMatchResu;

		//DWORD dwConfrontWin;
		//DWORD dwConfrontLoss;
		//DWORD dwConfrontDraw;

		DWORD dwPKMapKills;
		DWORD dwPKMapDeaths;
		DWORD dwPKMapResu;

		DWORD dwPVEKills;
		DWORD dwPVEDeaths;
		LONGLONG llPVEMoneyGain;

		bool	bUpdateCombatRecord;
		float	fCombatRecordUpdateTime;


		SDATA();

		void resetUpdate();
		bool updateTimer(float fElapsed);


		void increasePVEKills();
		void increasePVEDeaths();
		void increasePVEMoneyGain(LONGLONG llVal);

		void increasePVPKills();
		void increasePVPDeaths();
		void increasePVPResu();

		void increaseTyrannyKills();
		void increaseTyrannyDeaths();
		void increaseTyrannyResu();

		void increaseClubWarKills();
		void increaseClubWarDeaths();
		void increaseClubWarResu();

		void increaseCaptureTheFlagKills();
		void increaseCaptureTheFlagDeaths();
		void increaseCaptureTheFlagResu();

		void increaseClubDeathMatchKills();
		void increaseClubDeathMatchDeaths();
		void increaseClubDeathMatchResu();

		void increaseSchoolWarsKills();
		void increaseSchoolWarsDeaths();
		void increaseSchoolWarsResu();

		void increasePKMapKills();
		void increasePKMapDeaths();
		void increasePKMapResu();
	};
};

#endif // GLCOMBATRECORD_H_INCLUDED__
