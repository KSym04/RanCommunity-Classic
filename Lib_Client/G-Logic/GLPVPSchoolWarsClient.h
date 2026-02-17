/*!
 * \file GLPVPSchoolWarsClient.h
 *
 * \author Juver
 * \date January 2018
 *
 * 
 */

#pragma once

#include "./GLPVPSchoolWarsData.h"


class	GLPVPSchoolWarsClient
{
public:
	SCHOOLWARS_STATE		m_emState;
	SCHOOLWARS_SCHED_NEXT	m_sScheduleNext;
	
	WORD	m_wLevelReq;
	DWORD	m_dwContriReq;
	WORD	m_wPlayerLimit;
	WORD	m_wMaxScore;
	WORD	m_wBattleTime;

	float	m_fRemain;
	bool	m_bRegistered;
	bool	m_bQueued;
	WORD	m_wQueueNum;

	WORD	m_wLastWinner;
	SCHOOLWARS_SCORE_DATA	m_sScore[SCHOOLWARS_SCHOOL_SIZE];
	
	SCHOOLWARS_PLAYER_DATA_VEC	m_vecRankingAll;
	SCHOOLWARS_PLAYER_DATA_VEC	m_vecRankingSG;
	SCHOOLWARS_PLAYER_DATA_VEC	m_vecRankingMP;
	SCHOOLWARS_PLAYER_DATA_VEC	m_vecRankingPHX;

	SCHOOLWARS_PLAYER_DATA		m_sLastTop[SCHOOLWARS_MINI_RANKING_NUM];

	BOOL	m_bShowStartNotice;

	float		m_fDoublePoint;
	bool		m_bDoublePoint;

	SNATIVEID	m_sItemRewardWin;
	SNATIVEID	m_sItemRewardLoss;
	WORD		m_wRewardLimitScoreWin;
	WORD		m_wRewardLimitScoreLost;

	float		m_fContributionPointMultiplier;

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

public:
	bool	FrameMove ( float fTime, float fElapsedTime );

public:
	bool	IsRegister() { return bool(m_emState == SCHOOLWARS_STATE_REGISTER); }
	bool	IsEnded()	{ return bool(m_emState == SCHOOLWARS_STATE_ENDED); }
	bool	IsBattle()	{ return bool(m_emState == SCHOOLWARS_STATE_BATTLE); }
	bool	IsReward()	{ return bool(m_emState == SCHOOLWARS_STATE_REWARD); }
	void	SetEventState( SCHOOLWARS_STATE emState );
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward();
	void	DoStateEnd();
	void	NotifyTimeToStart( WORD wTime );
	void	DoRegister();
	void	DoRevive();
	void	RequestScoreInfo();
	void	ResetScore();

	SCHOOLWARS_PLAYER_DATA*	GetOwnRanking();
	int		GetRankingSize( int nType );
	SCHOOLWARS_PLAYER_DATA_VEC&	GetRankingVector( int nType );
	void	RequestMapExit();
	
	BOOL	SetContributionPointMultiplier( float fMulti );
	float	GetContributionPointMultiplier();

public:
	GLPVPSchoolWarsClient()
		: m_emState(SCHOOLWARS_STATE_ENDED)
		, m_fRemain(0.0f)
		, m_wLevelReq(1)
		, m_dwContriReq(0)
		, m_wPlayerLimit(1)
		, m_wMaxScore(1)
		, m_wBattleTime(0)
		, m_bRegistered(FALSE)
		, m_bQueued(FALSE)
		, m_wQueueNum(0)
		, m_wLastWinner(SCHOOLWARS_SCHOOL_SIZE)
		, m_bShowStartNotice(FALSE)
		, m_fDoublePoint(0.0f)
		, m_bDoublePoint(false)

		, m_sItemRewardWin(false)
		, m_sItemRewardLoss(false)
		, m_wRewardLimitScoreWin(0)
		, m_wRewardLimitScoreLost(0)

		, m_fContributionPointMultiplier(1.0f)
	{
		for( int i=0; i<SCHOOLWARS_SCHOOL_SIZE; ++i )
		{
			m_sScore[i].wSchool = i;
			m_sScore[i].Reset();
		}
	}

	~GLPVPSchoolWarsClient()
	{

	}

public:
	static GLPVPSchoolWarsClient& GetInstance();

};