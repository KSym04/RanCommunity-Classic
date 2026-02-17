/*!
 * \file PVPClubDeathMatchField.h
 *
 * \author Juver
 * \date 2019/09/16
 *
 * 
 */

#ifndef PVPCLUBDEATHMATCHFIELD_H_INCLUDED__
#define PVPCLUBDEATHMATCHFIELD_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPClubDeathMatch.h"

struct NET_MSG_GENERIC;

class	PVPClubDeathMatchField : public PVPClubDeathMatch
{
public:
	bool m_bValidInstance;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP		m_mapClubData;
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK		m_sFirstClubRank;

	float m_fRankUpdateTimer;

	BOOL m_bDoublePoint;
	float m_fDoublePointTime;

public:
	void LoadEvent( std::string strFile );
	void ResetEvent();
	bool SetMapState ();

	bool FrameMove ( float fElaps );

	void DoStateRegister();
	void DoStateBattle();
	void DoStateReward();
	void DoStateEnd();

	void RequestInfoPC( DWORD dwCharID, BOOL bLobbyMap, BOOL bBattleMap );
	void ProcessClubCleanUp();

	PVP_CLUB_DEATH_MATCH_CLUB_DATA* GetClubData( DWORD dwClubID );
	PVP_CLUB_DEATH_MATCH_CLUB_DATA* CreateClubData( DWORD dwClubID, const TCHAR* szClubName, const TCHAR* szMasterName, DWORD dwClubMarkVer );

public:
	void ScoreKill( DWORD dwCharKilled, DWORD dwCharKiller );
	void ScoreResu( DWORD dwChar );
	void RankingUpdate();
	void RankingSend( BOOL bFinal = FALSE );
	void RequestScoreInfo( DWORD dwCharID );
	void BroadCastDoublePoint();

	void SendRewards();
public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

public:
	PVPClubDeathMatchField();
	~PVPClubDeathMatchField();

public:
	static PVPClubDeathMatchField& GetInstance();
};


#endif // PVPCLUBDEATHMATCHFIELD_H_INCLUDED__
