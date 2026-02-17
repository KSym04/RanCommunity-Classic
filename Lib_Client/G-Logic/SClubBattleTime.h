/*!
 * \file SClubBattleTime.h
 *
 * \author Juver
 * \date 2022/02/06
 *
 * 
 */

#ifndef SCLUBBATTLETIME_H_INCLUDED__
#define SCLUBBATTLETIME_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SCLUBWAR_BATTLE_TIMER
{
	enum { TEXT_LEN = 33, };

	DWORD dwID;
	char szName[TEXT_LEN];
	BOOL bBattle;

	float fTime;

	SCLUBWAR_BATTLE_TIMER()
		: dwID(0)
		, fTime(0.0f)
		, bBattle(FALSE)
	{
		memset(szName, 0, sizeof(char) * TEXT_LEN);
	}
};

typedef std::map<DWORD, SCLUBWAR_BATTLE_TIMER>	CLUB_BATTLE_TIMER_MAP;
typedef CLUB_BATTLE_TIMER_MAP::iterator			CLUB_BATTLE_TIMER_MAP_ITER;

#endif // SCLUBBATTLETIME_H_INCLUDED__
