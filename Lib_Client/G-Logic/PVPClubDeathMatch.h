/*!
 * \file PVPClubDeathMatch.h
 *
 * \author Juver
 * \date 2019/09/15
 *
 * 
 */

#ifndef PVPCLUBDEATHMATCH_H_INCLUDED__
#define PVPCLUBDEATHMATCH_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPClubDeathMatchDefine.h"
#include "./PVPClubDeathMatchData.h"

#include "../../Lib_Engine/G-Logic/GLDefine.h"

class PVPClubDeathMatch
{
public:
	PVP_CLUB_DEATH_MATCH_EVENT_STATE m_emState;

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

	//config condition
	WORD m_wLevelReq;
	WORD m_wClubLevelLimit;
	WORD m_wClubPlayerMin;
	WORD m_wClubPlayerMax;
	bool m_bClubPlayerReqCDMRights;

	float m_fMoveLobbyAreaTime;
	float m_fMoveBattleAreaTime;

	bool m_bDisableOtherGuildResu;

	WORD m_wDefaultPoint;
	WORD m_wPointKillGain;
	WORD m_wPointDeathLoss;
	WORD m_wDoublePointKillGain;
	WORD m_wDoublePointDeathLoss;
	WORD m_wPointResu;
	float m_fRankUpdateTime;

	float m_fDoublePointRechargeTime;
	float m_fDoublePointConsumeTime;
	
	PVP_CLUB_DEATH_MATCH_REWARD_DATA m_sReward[PVP_CLUB_DEATH_MATCH_RANK_TOP];

	BOOL		m_bHashCheck;				/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */

public:
	PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT	m_sScheduleNext;	
	PVP_CLUB_DEATH_MATCH_EVENT_SCHED_VEC	m_vecSchedule;

public:
	bool DataLoad( std::string strFile );
	void DataReset();
	void FindScheduleFirst();
	bool FindScheduleNext();

public:
	bool IsRegister() { return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER); }
	bool IsEnded()	{ return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED); }
	bool IsBattle()	{ return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE); }
	bool IsReward()	{ return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD); }

public:
	bool isDisableOtherGuildResu()	{ return m_bDisableOtherGuildResu; }

	BOOL isCombatRecord() { return m_sScheduleNext.sStartSched.bCombatRecord; }

public:
	PVPClubDeathMatch();
	virtual ~PVPClubDeathMatch();
};

#endif // PVPCLUBDEATHMATCH_H_INCLUDED__
