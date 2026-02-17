/*!
 * \file GLPVPCaptureTheFlag.h
 *
 * \author Juver
 * \date January 2018
 *
 * 
 */

#pragma once

#include "./GLPVPCaptureTheFlagData.h"

class DxLandGate;

class	GLPVPCaptureTheFlag
{
public:
	CAPTURE_THE_FLAG_STATE	m_emState;
	SNATIVEID			m_sMap;
	
	WORD	m_wRegisterTime;
	WORD	m_wBattleTime;
	WORD	m_wRewardTime;

	WORD	m_wLevelReq;
	WORD	m_wPlayerLimit;
	DWORD	m_dwContributionReq;

	WORD	m_wMaxScore;
	WORD	m_wBalancer;
	WORD	m_wTeamGate[CAPTURE_THE_FLAG_TEAM_SIZE];

	SNATIVEID	m_sOutMap;
	WORD		m_wOutGate;

	float		m_fFlagVar;
	float		m_fKillVar;
	float		m_fHealVar;
	float		m_fResuVar;

	float		m_fPointVar;
	WORD		m_wBasePointWin;
	WORD		m_wBasePointLoss;
	WORD		m_wMaxRewardPoint;

	SNATIVEID	m_sItemRewardWin;
	WORD		m_wRewardLimitScoreWin;

	SNATIVEID	m_sItemRewardLost;
	WORD		m_wRewardLimitScoreLost;

public:
	CAPTURE_THE_FLAG_SCHED_NEXT	m_sScheduleNext;	
	CAPTURE_THE_FLAG_SCHED_VEC	m_vecSchedule;

	SNATIVEID	m_sFlagCrow[CAPTURE_THE_FLAG_TEAM_SIZE];

	/*PVPCaptureTheFlag enemy resu, EJCode, 2018/10/23 */
	BOOL		m_bDisableOtherTeamResu;

	/*PVPCaptureTheFlag disable score self heal, EJCode, 2018/11/28 */
	BOOL		m_bDisableScoreSelfHeal;

	float		m_fContributionPointMultiplier;

	/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
	BOOL		m_bHashCheck;

public:
	bool Load( std::string strFile );
	bool IsRegister() { return bool(m_emState == CAPTURE_THE_FLAG_STATE_REGISTER); }
	bool IsEnded()	{ return bool(m_emState == CAPTURE_THE_FLAG_STATE_ENDED); }
	bool IsBattle()	{ return bool(m_emState == CAPTURE_THE_FLAG_STATE_BATTLE); }
	bool IsReward()	{ return bool(m_emState == CAPTURE_THE_FLAG_STATE_REWARD); }
	
public:
	//scheduler function
	void	FindScheduleFirst();
	bool	FindScheduleNext();

	/*PVPCaptureTheFlag enemy resu, EJCode, 2018/10/23 */
	BOOL	isDisableOtherTeamResu()	{ return m_bDisableOtherTeamResu; }

	/*PVPCaptureTheFlag disable score self heal, EJCode, 2018/11/28 */
	BOOL	isDisableScoreSelfHeal()	{ return m_bDisableScoreSelfHeal; }

	BOOL	SetContributionPointMultiplier( float fMulti );
	float	GetContributionPointMultiplier();

	BOOL	isCombatRecord() { return m_sScheduleNext.sStartSched.bCombatRecord; }

public:
	GLPVPCaptureTheFlag()
		: m_emState(CAPTURE_THE_FLAG_STATE_ENDED)
		, m_sMap(false)
		, m_wRegisterTime(0)
		, m_wBattleTime(0)
		, m_wRewardTime(0)
		, m_wLevelReq(1)
		, m_wPlayerLimit(1)
		, m_dwContributionReq(0)
		, m_wMaxScore(0)
		, m_wBalancer(0)
		, m_sOutMap(false)
		, m_wOutGate(0)
		, m_fFlagVar(1.0f)
		, m_fKillVar(1.0f)
		, m_fHealVar(0.1f)
		, m_fResuVar(1.0f)
		, m_sItemRewardWin(false)
		, m_wRewardLimitScoreWin(10)
		, m_sItemRewardLost(false)
		, m_wRewardLimitScoreLost(10)
		, m_fPointVar(0.0f)
		, m_wBasePointWin(0)
		, m_wBasePointLoss(0)
		, m_wMaxRewardPoint(0)
		, m_bDisableOtherTeamResu(FALSE)		/*PVPCaptureTheFlag enemy resu, EJCode, 2018/10/23 */
		, m_bDisableScoreSelfHeal(FALSE)		/*PVPCaptureTheFlag disable score self heal, EJCode, 2018/11/28 */
		, m_fContributionPointMultiplier(1.0f)
		, m_bHashCheck(FALSE)					/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
	{
		for( int i=0; i<CAPTURE_THE_FLAG_TEAM_SIZE; ++i )
		{
			m_wTeamGate[i] = 0;
			m_sFlagCrow[i] = NATIVEID_NULL();
		}
	}

	virtual ~GLPVPCaptureTheFlag()
	{

	}
};