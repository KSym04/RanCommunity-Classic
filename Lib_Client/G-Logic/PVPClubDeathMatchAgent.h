/*!
 * \file PVPClubDeathMatchAgent.h
 *
 * \author Juver
 * \date 2019/09/16
 *
 * 
 */

#ifndef PVPCLUBDEATHMATCHAGENT_H_INCLUDED__
#define PVPCLUBDEATHMATCHAGENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPClubDeathMatch.h"
#include "./PVPClubDeathMatchData.h"

struct NET_MSG_GENERIC;

class	PVPClubDeathMatchAgent : public PVPClubDeathMatch
{
public:
	float	m_fTimer;
	float	m_fRemain;
	float	m_fBrdTimer;
	WORD	m_wLastNotifyTime;

	bool	m_bMoveIn;
	bool	m_bMoveOut;
	float	m_fMoveTimer;
	float	m_fMoveTimerTotal;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP		m_mapClubData;
	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_VEC	m_vecPlayerMove;
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK		m_sClubTop[PVP_CLUB_DEATH_MATCH_RANK_TOP];
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK		m_sFirstClubRank;

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP	m_mapPlayerHash;

public:
	void LoadEvent( std::string strFile );
	void ResetEvent();

	bool FrameMove ( float fElapsedTime );
	bool SetMapState ();

	void SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE emState );

	void DoStateRegister();
	void DoStateBattle();
	void DoStateReward();
	void DoStateEnd();

	void NotifyTimeToStart( float fTime );
	void BroadCastPlayerNum();

	void RequestInfoPC( DWORD dwCharID, BOOL bLobbyMap, BOOL bBattleMap );
	void RequestPlayerRegister( DWORD dwCharID, WORD wLevel, BOOL bRegister, std::string strHash );
	void RequestPlayerCancelRegister( DWORD dwCharID, DWORD dwGuildID );

	void PlayerRegister( PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData, DWORD dwCharID, std::string strHash );
	void CancelRegister( PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData, DWORD dwCharID );

	void ProcessClubCleanUp();
	void ProcessClubQualify();

	void QueueMoveIn();
	void QueueMoveOut();
	void MapMoveInPlayer();
	void MapMoveOutPlayer();

	void PlayerRejoinBattle( DWORD dwCharID );

	void CheckForceMove( float fElapsedTime );
	void SendClubDataAll();
	void SendClubData( PVP_CLUB_DEATH_MATCH_CLUB_DATA& sClubData );

	PVP_CLUB_DEATH_MATCH_CLUB_DATA* GetClubData( DWORD dwClubID );
	PVP_CLUB_DEATH_MATCH_CLUB_DATA* CreateClubData( DWORD dwClubID, const TCHAR* szClubName, const TCHAR* szMasterName, DWORD dwClubMarkVer );

	void SendTop10Info();
	
	void MapExit( DWORD dwCharID );

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	BOOL	PCHashCheck( std::string strHash );
	void	PCHashInsert( std::string strHash, DWORD dwCharID );
	void	PCHashDelete( DWORD dwCharID );

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

public:
	PVPClubDeathMatchAgent();
	~PVPClubDeathMatchAgent();

public:
	static PVPClubDeathMatchAgent& GetInstance();
};

#endif // PVPCLUBDEATHMATCHAGENT_H_INCLUDED__
