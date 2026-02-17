/*!
 * \file PVPPartyBattleGrounds.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDS_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPPartyBattleGroundsData.h"

namespace PVPPBG
{
	class Manager
	{
	public:
		Manager();
		virtual ~Manager();

	public:
		EVENT_STATE m_emState;

		//config map
		SNATIVEID m_sBattleMap;
		DWORD m_dwBattleGate;

		SNATIVEID m_sLobbyMap;
		DWORD m_dwLobbyGate;

		SNATIVEID m_sOutMap;
		DWORD m_dwOutGate;

		//config durations
		WORD m_wRegisterTime;
		WORD m_wBattleTime;
		WORD m_wRewardTime;

		//entry conditions
		WORD m_wLevelReq;
		DWORD m_dwContriReq;

		WORD m_wPlayerLimit;

		BOOL m_bHashCheck;

		BOOL m_bDisableOtherResu;
		BOOL m_bDisableScoreSelfHeal;
		
		float m_fScoreUpdateTime;

		BOOL m_bForceMoveIn;
		float m_fForceMoveInTime;

		BOOL m_bNoPartyMoveOut;
		float m_fNoPartyMoveOutTime;

		float m_fKillVar;
		float m_fHealVar;
		float m_fResuVar;

		WORD m_wPoint1;
		WORD m_wPoint2;
		WORD m_wPoint3;

	public:
		EVENT_SCHEDEDULE_NEXT	m_sScheduleNext;	
		EVENT_SCHEDEDULE_VEC	m_vecSchedule;

	public:
		bool DataLoad( std::string strFile );
		void DataReset();
		void FindScheduleFirst();
		bool FindScheduleNext();

	public:
		bool IsRegister() { return bool(m_emState == PVPPBG::EVENT_STATE_REGISTER); }
		bool IsEnded()	{ return bool(m_emState == PVPPBG::EVENT_STATE_ENDED); }
		bool IsBattle()	{ return bool(m_emState == PVPPBG::EVENT_STATE_BATTLE); }
		bool IsReward()	{ return bool(m_emState == PVPPBG::EVENT_STATE_REWARD); }

		BOOL isDisableOtherResu()  const { return m_bDisableOtherResu; }
		BOOL isDisableScoreSelfHeal() const	{ return m_bDisableScoreSelfHeal; }
		BOOL isCombatRecord() const { return m_sScheduleNext.sStartSched.bCombatRecord; }
	};

};



#endif // PVPPARTYBATTLEGROUNDS_H_INCLUDED__
