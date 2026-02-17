/*!
 * \file GLPVPTyranny.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma once

#include "./GLPVPTyrannyData.h"

class DxLandGate;

class	GLPVPTyranny
{
public:
	TYRANNY_STATE		m_emState;
	
	TYRANNY_MAP_VEC		m_vecMapRandom;
	SNATIVEID			m_sMapFixed;

	WORD	m_wRegisterTime;
	WORD	m_wBattleTime;
	WORD	m_wRewardTime;

	WORD	m_wLevelReq;
	WORD	m_wPlayerLimit;
	WORD	m_wSchoolGate[TYRANNY_SCHOOL_SIZE];

	DWORD	m_dwDamageCapture;
	DWORD	m_dwDamageBase;

	SNATIVEID	m_sOutMap;
	WORD		m_wOutGate;

	float		m_fDamageVar;
	float		m_fKillVar;
	float		m_fHealVar;
	float		m_fResuVar;

	float		m_fPointVar;
	float		m_fRemainTimeToNextBattle; // PVP Next Battle UI - Jxyyy
	WORD		m_wBasePointWin;
	WORD		m_wBasePointLoss;
	WORD		m_wMaxRewardPoint;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	float		m_fTyrannyRankingDisplayUpdate;

	SNATIVEID	m_sItemRewardWin;
	WORD		m_wRewardLimitScoreWin;

	SNATIVEID	m_sItemRewardLost;
	WORD		m_wRewardLimitScoreLost;

	WORD		m_wRebirthTimeBase;
	float		m_fPerCharRebirthTimeAdd;

	/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
	BOOL		m_bDisableOtherSchoolResu;

	/*PVPTyranny disable score self heal, EJCode, 2018/11/28 */
	BOOL		m_bDisableScoreSelfHeal;

	/* PVPTyranny Hash Check, Juver, 2020/07/28 */
	BOOL		m_bHashCheck;

	TYRANNY_TOWER_DATA	m_sTowerData[TYRANNY_TOWER_SIZE];

	BOOL		m_bNonTowerEnd;

	BOOL		m_bDisableOtherGuildResu;

	DWORD		m_dwGuiID[TYRANNY_TOWER_SIZE];
	DWORD		m_dwMarkVer[TYRANNY_TOWER_SIZE];

public:
	TYRANNY_SCHED_NEXT	m_sScheduleNext;	
	TYRANNY_SCHED_VEC	m_vecSchedule;
	TYRANNY_SCHED_BUFF_MAP m_mapBuffData;

	float		m_fContributionPointMultiplier;

public:
	bool Load( std::string strFile );
	bool IsRegister() { return bool(m_emState == TYRANNY_STATE_REGISTER); }
	bool IsEnded()	{ return bool(m_emState == TYRANNY_STATE_ENDED); }
	bool IsBattle()	{ return bool(m_emState == TYRANNY_STATE_BATTLE); }
	bool IsReward()	{ return bool(m_emState == TYRANNY_STATE_REWARD); }
	
public:
	//scheduler function
	void	FindScheduleFirst();
	bool	FindScheduleNext();

	/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
	BOOL	isDisableOtherSchoolResu()	{ return m_bDisableOtherSchoolResu; }

	BOOL	isDisableOtherGuildResu()	{ return m_bDisableOtherGuildResu; }
	

	/*PVPTyranny disable score self heal, EJCode, 2018/11/28 */
	BOOL	isDisableScoreSelfHeal()	{ return m_bDisableScoreSelfHeal; }

	BOOL	SetContributionPointMultiplier( float fMulti );
	float	GetContributionPointMultiplier();

	void	BuffSelect();
	void	BattleMapSelect();	//agent server, only agent server should select a map

	BOOL	isCombatRecord() { return m_sScheduleNext.bCombatRecord; }

public:
	GLPVPTyranny()
		: m_emState(TYRANNY_STATE_ENDED)
		, m_sMapFixed(false)
		, m_wRegisterTime(0)
		, m_wBattleTime(0)
		, m_wRewardTime(0)
		, m_wLevelReq(1)
		, m_wPlayerLimit(1)
		, m_sOutMap(false)
		, m_wOutGate(0)
		, m_dwDamageCapture(2000)
		, m_dwDamageBase(1000)
		, m_fDamageVar(0.1f)
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
		
		, m_wRebirthTimeBase(20)
		, m_fPerCharRebirthTimeAdd(0.0f)
		, m_bDisableOtherSchoolResu(FALSE)		/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
		, m_bDisableScoreSelfHeal(FALSE)		/*PVPTyranny disable score self heal, EJCode, 2018/11/28 */
		, m_fContributionPointMultiplier(1.0f)
		, m_bHashCheck(FALSE)					/* PVPTyranny Hash Check, Juver, 2020/07/28 */

		, m_bNonTowerEnd(FALSE)
		, m_bDisableOtherGuildResu(FALSE)
		, m_fTyrannyRankingDisplayUpdate(0)	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	{
		for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
		{
			m_wSchoolGate[i] = 0;
			m_dwGuiID[i] = CLUB_NULL;
			m_dwMarkVer[i] = 0;
		}
		
		for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
			m_sTowerData[i] = TYRANNY_TOWER_DATA();
	}

	virtual ~GLPVPTyranny()
	{

	}
};