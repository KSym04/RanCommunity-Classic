/*!
 * \file GLPVPTyrannyClient.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma once

#include "./GLPVPTyrannyData.h"


class	GLPVPTyrannyClient
{
public:
	TYRANNY_STATE		m_emState;
	TYRANNY_SCHED_NEXT	m_sScheduleNext;
	float	m_fRemainTimeToNextBattle;	// PVP Next Battle UI - Jxyyy
	
	WORD	m_wLevelReq;
	WORD	m_wPlayerLimit;

	float	m_fRemain;
	bool	m_bRegistered;
	bool	m_bQueued;
	WORD	m_wQueueNum;

	WORD	m_wTowerOwner[TYRANNY_TOWER_SIZE];
	WORD	m_wLastWinner;

	DWORD	m_dwGuiID[TYRANNY_TOWER_SIZE];
	DWORD	m_dwMarkVer[TYRANNY_TOWER_SIZE];

	TYRANNY_PLAYER_DATA_VEC	m_vecRankingAll;
	TYRANNY_PLAYER_DATA_VEC	m_vecRankingSG;
	TYRANNY_PLAYER_DATA_VEC	m_vecRankingMP;
	TYRANNY_PLAYER_DATA_VEC	m_vecRankingPHX;

	TYRANNY_PLAYER_DATA		m_sLastTop[TYRANNY_MINI_RANKING_NUM];

	//SNATIVEID	m_sRewardBuff[TYRANNY_REWARD_BUFF_NUM];
	BOOL		m_bShowStartNotice;

	SNATIVEID	m_sItemRewardWin;
	SNATIVEID	m_sItemRewardLoss;
	WORD		m_wRewardLimitScoreWin;
	WORD		m_wRewardLimitScoreLost;

	WORD		m_wRebirthTime[TYRANNY_SCHOOL_SIZE];

	float		m_fContributionPointMultiplier;

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

public:
	bool	FrameMove ( float fTime, float fElapsedTime );

public:
	bool	IsRegister() { return bool(m_emState == TYRANNY_STATE_REGISTER); }
	bool	IsEnded()	{ return bool(m_emState == TYRANNY_STATE_ENDED); }
	bool	IsBattle()	{ return bool(m_emState == TYRANNY_STATE_BATTLE); }
	bool	IsReward()	{ return bool(m_emState == TYRANNY_STATE_REWARD); }
	void	SetEventState( TYRANNY_STATE emState );
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward();
	void	DoStateEnd();
	void	NotifyTimeToStart( WORD wTime );
	void	DoRegister();
	void	DoRevive();
	void	RequestTowerInfo();
	void	ResetScore();

	TYRANNY_PLAYER_DATA*	GetOwnRanking();
	int		GetRankingSize( int nType );
	TYRANNY_PLAYER_DATA_VEC&	GetRankingVector( int nType );
	void	RequestMapExit();

	WORD	GetRebirthTime();

	BOOL	SetContributionPointMultiplier( float fMulti );
	float	GetContributionPointMultiplier();

public:
	GLPVPTyrannyClient()
		: m_emState(TYRANNY_STATE_ENDED)
		, m_fRemain(0.0f)
		, m_wLevelReq(1)
		, m_wPlayerLimit(1)
		, m_bRegistered(FALSE)
		, m_bQueued(FALSE)
		, m_wQueueNum(0)
		, m_wLastWinner(TYRANNY_SCHOOL_SIZE)
		, m_bShowStartNotice(FALSE)
		, m_sItemRewardWin(false)
		, m_sItemRewardLoss(false)
		, m_wRewardLimitScoreWin(0)
		, m_wRewardLimitScoreLost(0)
		, m_fContributionPointMultiplier(1.0f)
	{
		for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
		{
			m_wTowerOwner[i] = TYRANNY_SCHOOL_SIZE;
			m_dwGuiID[i] = CLUB_NULL;
			m_dwMarkVer[i] = 0;
		}

		//for( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
		//	m_sRewardBuff[i] = NATIVEID_NULL();

		for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
			m_wRebirthTime[i] = 20;
	}

	~GLPVPTyrannyClient()
	{

	}

public:
	static GLPVPTyrannyClient& GetInstance();

};