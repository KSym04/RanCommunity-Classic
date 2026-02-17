/*!
 * \file GLPVPWoe.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma once

#include "./GLPVPWOeData.h"

class DxLandGate;

class	GLPVPWoe
{
public:
	WOE_STATE			m_emState;
	SNATIVEID			m_sMap;
	
	WORD	m_wRegisterTime;
	WORD	m_wBattleTime;
	WORD	m_wRewardTime;

	WORD	m_wLevelReq;
	WORD	m_wPlayerLimit;
	WORD	m_wSchoolGate[100];

	SNATIVEID	m_sOutMap;
	WORD		m_wOutGate;

	WOE_TOWER_DATA	m_sWoeTowerData;
public:
	WOE_SCHED_NEXT	m_sScheduleNext;	
	WOE_SCHED_VEC	m_vecSchedule;

	float			m_fRankingUpdate;

	SNATIVEID	m_sItemRewardGuild;
	SNATIVEID	m_sItemRewardPlayer;
	SNATIVEID	m_sItemRewardResu;
	
public:
	bool Load( std::string strFile );
	bool IsRegister() { return bool(m_emState == WOE_STATE_REGISTER); }
	bool IsEnded()	{ return bool(m_emState == WOE_STATE_ENDED); }
	bool IsBattle()	{ return bool(m_emState == WOE_STATE_BATTLE); }
	bool IsReward()	{ return bool(m_emState == WOE_STATE_REWARD); }
	
public:
	void	FindScheduleFirst();
	bool	FindScheduleNext();

public:
	GLPVPWoe()
		: m_emState(WOE_STATE_ENDED)
		, m_sMap(false)
		, m_wRegisterTime(0)
		, m_wBattleTime(0)
		, m_wRewardTime(0)
		, m_wLevelReq(1)
		, m_wPlayerLimit(1)
		, m_sOutMap(false)
		, m_wOutGate(0)
	{}

	virtual ~GLPVPWoe()
	{

	}
};