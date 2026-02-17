/*!
 * \file GLPVPSchoolWars.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma once

#include "./GLPVPSchoolWarsData.h"

class DxLandGate;

class	GLPVPSchoolWars
{
public:
	SCHOOLWARS_STATE	m_emState;
	SNATIVEID			m_sMap;
	
	WORD	m_wRegisterTime;
	WORD	m_wBattleTime;
	WORD	m_wRewardTime;

	WORD	m_wLevelReq;
	DWORD	m_dwContriReq;
	WORD	m_wPlayerLimit;
	WORD	m_wMaxScore;
	WORD	m_wSchoolGate[SCHOOLWARS_SCHOOL_SIZE];

	SNATIVEID	m_sOutMap;
	WORD		m_wOutGate;

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

	/*PPVPSchoolWars enemy resu, EJCode, 2018/10/22 */
	BOOL		m_bDisableOtherSchoolResu;

	/*PVPSchoolWars disable score self heal, EJCode, 2018/11/28 */
	BOOL		m_bDisableScoreSelfHeal;

	/* PVPSchoolWars hash check, Juver, 2021/07/17 */
	BOOL		m_bHashCheck;

public:
	SCHOOLWARS_SCHED_NEXT	m_sScheduleNext;	
	SCHOOLWARS_SCHED_VEC	m_vecSchedule;

	SCHOOLWARS_CROW_SCORE_MAP m_mapCrowScore;

	float		m_fContributionPointMultiplier;

public:
	bool Load( std::string strFile );
	bool IsRegister() { return bool(m_emState == SCHOOLWARS_STATE_REGISTER); }
	bool IsEnded()	{ return bool(m_emState == SCHOOLWARS_STATE_ENDED); }
	bool IsBattle()	{ return bool(m_emState == SCHOOLWARS_STATE_BATTLE); }
	bool IsReward()	{ return bool(m_emState == SCHOOLWARS_STATE_REWARD); }
	
public:
	//scheduler function
	void	FindScheduleFirst();
	bool	FindScheduleNext();

	SCHOOLWARS_CROW_SCORE* GetCrowScore( SNATIVEID idcrow );

	/*PPVPSchoolWars enemy resu, EJCode, 2018/10/22 */
	BOOL	isDisableOtherSchoolResu()	{ return m_bDisableOtherSchoolResu; }

	/*PVPSchoolWars disable score self heal, EJCode, 2018/11/28 */
	BOOL	isDisableScoreSelfHeal()	{ return m_bDisableScoreSelfHeal; }

	BOOL	SetContributionPointMultiplier( float fMulti );
	float	GetContributionPointMultiplier();

	BOOL	isCombatRecord() { return m_sScheduleNext.sStartSched.bCombatRecord; }

public:
	GLPVPSchoolWars()
		: m_emState(SCHOOLWARS_STATE_ENDED)
		, m_sMap(false)
		, m_wRegisterTime(0)
		, m_wBattleTime(0)
		, m_wRewardTime(0)
		, m_wLevelReq(1)
		, m_dwContriReq(0)
		, m_wPlayerLimit(1)
		, m_wMaxScore(0)
		, m_sOutMap(false)
		, m_wOutGate(0)
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
		, m_bDisableOtherSchoolResu(FALSE)		/*PPVPSchoolWars enemy resu, EJCode, 2018/10/22 */
		, m_bDisableScoreSelfHeal(FALSE)		/*PVPSchoolWars disable score self heal, EJCode, 2018/11/28 */
		, m_fContributionPointMultiplier(1.0f)
		, m_bHashCheck(FALSE)					/* PVPSchoolWars hash check, Juver, 2021/07/17 */
	{
		for( int i=0; i<SCHOOLWARS_SCHOOL_SIZE; ++i )
			m_wSchoolGate[i] = 0;
	}

	virtual ~GLPVPSchoolWars()
	{

	}
};