#pragma once

#include "GLPVPWoe.h"
#include "GLPVPWoeDefine.h"

class	GLPVPWoeField : public GLPVPWoe
{

public:
	void	LoadEvent( std::string strFile );
	bool	FrameMove ( float fElaps );
	bool	SetMapState ();

	void	ResetData();
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward();
	void	DoStateEnd();

	void	RequestInfoPC( DWORD dwCharID, BOOL bWoeMap );

	WOE_PLAYER_DATA*		PlayerDataGet( DWORD dwCharID );

private:
	void	InsertPlayerData( WOE_REGISTER_DATA* pPlayerData );
	void	PlayerRevive( DWORD dwGaeaID, DWORD dwCharID );

public:
	void EmperiumCapture(SNATIVEID sidMob, WORD wClubID);
	DWORD m_dwClubID;
	float m_fUpdateScore;

public:
	void DeclareEmperiumWinner(const TCHAR* szName);
	void PointSend();
	void WoeGuildReward();
	void WoePlayerReward();
	void WoeResuReward();

	void HitScore(DWORD dwCharID, WORD wHitScore);
	void UpdateWoePlayerInfo(DWORD dwCharID, bool isHit, bool isKill, bool isDeath);

public:
	void	AddWoeGuildScore(DWORD dwKillClub, DWORD dwDeathClub);
	void	UpdateGuildScore();
	WOE_GUILD_RANK_INFO_MAP	m_mapWoeScore;
	SWOE_GUILD_RANK_INFO* GetWoeGuildRankInfo(DWORD dwClubID);

	void AddWoePlayerScore(DWORD dwKillScore, DWORD dwDeathScore);
	void UpdatePlayerScore();
	WOE_PLAYER_RANK_INFO_MAP m_mapWoePlayerScore;
	SWOE_PLAYER_RANK_INFO* GetWoePlayerRankInfo(DWORD dwCharID);
	SWOE_PLAYER_RANK_INFO sRankInfo;

	void AddWoeResuScore(DWORD dwResuScore);
	void UpdateResuScore();
	WOE_RESU_PLAYER_INFO_MAP m_mapWoeResuScore;
	SWOE_RESU_PLAYER_INFO* GetWoeResuRankInfo(DWORD dwCharID);

	WOE_AWARD_CHAR		m_vecWoeAwardChar;

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

private:
	WOE_PLAYER_DATA_MAP			m_mapGuildData;
	BOOL						m_bValidInstance;

public:
	GLPVPWoeField()
		: m_bValidInstance(FALSE)
	{
		
	}

	~GLPVPWoeField()
	{

	}


public:
	static GLPVPWoeField& GetInstance();
};
