/*!
 * \file PVPClubDeathMatchClient.h
 *
 * \author Juver
 * \date 2019/09/16
 *
 * 
 */

#ifndef PVPCLUBDEATHMATCHCLIENT_H_INCLUDED__
#define PVPCLUBDEATHMATCHCLIENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPClubDeathMatchData.h"
#include "./PVPClubDeathMatchDefine.h"

struct NET_MSG_GENERIC;

class	PVPClubDeathMatchClient
{
public:
	PVP_CLUB_DEATH_MATCH_EVENT_STATE m_emState;
	PVP_CLUB_DEATH_MATCH_EVENT_SCHED_NEXT m_sScheduleNext;

	float m_fRemain;

public:
	WORD m_wLevelReq;
	WORD m_wClubLevelLimit;
	WORD m_wClubPlayerMin;
	WORD m_wClubPlayerMax;
	bool m_bClubPlayerReqCDMRights;

	WORD m_wBattleDuration;

	bool m_bRegistered;
	WORD m_wClubParticipants;

	BOOL m_bShowStartNotice;

	BOOL m_bDoublePoint;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK			m_sClubTop[PVP_CLUB_DEATH_MATCH_RANK_TOP];
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK			m_sFirstClubRank;
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK			m_sMyClubRank;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP			m_mapClubData;
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK_VEC		m_vecRanking;

	PVP_CLUB_DEATH_MATCH_REWARD_DATA			m_sReward[PVP_CLUB_DEATH_MATCH_RANK_TOP];

	void ProcessClubCleanUp();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA* GetClubData( DWORD dwClubID );
	PVP_CLUB_DEATH_MATCH_CLUB_DATA* CreateClubData( DWORD dwClubID, const TCHAR* szClubName, const TCHAR* szMasterName, DWORD dwClubMarkVer );

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

public:
	bool	FrameMove ( float fTime, float fElapsedTime );

public:
	bool	IsRegister() { return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER); }
	bool	IsEnded()	{ return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED); }
	bool	IsBattle()	{ return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE); }
	bool	IsReward()	{ return bool(m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD); }

	void	SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE emState );
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward();
	void	DoStateEnd();
	void	NotifyTimeToStart( WORD wTime );

	void	DoRegister();

	void	DoRevive();
	void	RequestScoreInfo();
	void	RequestMapExit();

public:
	PVPClubDeathMatchClient();
	~PVPClubDeathMatchClient();

public:
	static PVPClubDeathMatchClient& GetInstance();
};

#endif // PVPCLUBDEATHMATCHCLIENT_H_INCLUDED__
