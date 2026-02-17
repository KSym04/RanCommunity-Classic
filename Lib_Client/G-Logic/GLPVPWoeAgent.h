#pragma once

#include "./GLPVPWoe.h"

class	GLPVPWoeAgent : public GLPVPWoe
{

public:
	void	LoadEvent( std::string strFile );
	bool	FrameMove ( float fElaps );
	bool	SetMapState ();
	void	SetEventState( WOE_STATE emState );
	void	ResetData();
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward();
	void	DoStateEnd();
	void	NotifyTimeToStart( float fTime );
	void	ResetPreviousWinner();

	void	RequestInfoPC( DWORD dwCharID, BOOL bWoeMap);

public:
	WOE_REGISTER_DATA* PlayerDataGet(DWORD dwCharID);

private:
	BOOL	IsPlayerFull( WOE_REGISTER_DATA_MAP& mapData );
	BOOL	IsPlayerReg( WOE_REGISTER_DATA_MAP& mapData, DWORD dwCharID );
	BOOL	InsertRegister( WOE_REGISTER_DATA_MAP& mapData, DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool );
	BOOL	DeleteRegister( DWORD dwCharID, WOE_REGISTER_DATA_MAP& mapData );

public:
	void	RequestPlayerRegister( DWORD dwCharID, WORD wLevel, BOOL bRegister );
	void	RequestPlayerCancelRegister( DWORD dwCharID );
	void	PlayerRegister( DWORD dwCharID, WOE_REGISTER_DATA_MAP& mapData);
	void	CancelRegister( DWORD dwCharID, WOE_REGISTER_DATA_MAP& mapData);
	void	PlayerRejoinBattle( DWORD dwCharID );
	void	MapExit( DWORD dwCharID );

private:
	void	MapMoveInPlayer();
	void	MapMoveOutPlayer();
	void	QueueMoveIn();
	void	QueueMoveOut();
	void	SendPlayerData();

private:
	void	EmperiumCapture(SNATIVEID sidMob, WORD wClubID);
	void	DeclareEmperiumWinner();
	void	RequestEmperiumInfo( DWORD dwCharID );
	DWORD	ResetTowerID;
	DWORD	ResetTowerVer;

	DWORD	dwClubID;
	DWORD	dwMarkVer;
	char	szName[CLUB_NAME + 1];

	DWORD	m_dwEmperiumGuild;
	DWORD	m_dwEmperiumMarkVer;

	int		m_nCountCaptured;
	std::string m_strName;
	std::string m_strLeader;

public:
	float	m_fTimer;
	float	m_fRemain;
	WORD	m_wLastNotifyTime;
	WORD	m_wLastWinner;

	WOE_REGISTER_DATA_VEC		m_vecPlayerMove;
	WOE_REGISTER_DATA_MAP		m_mapGuildData;

	bool		m_bMoveIn;
	bool		m_bMoveOut;
	float		m_fMoveTimer;
	float		m_fMoveTimerTotal;

	WORD m_wClubID;

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

public:
	GLPVPWoeAgent()
		: m_fTimer(0.0f)
		, m_fRemain(0.0f)
		, m_wLastNotifyTime(0)
		, m_bMoveIn(false)
		, m_bMoveOut(false)
		, m_fMoveTimer(0.0f)
		, m_fMoveTimerTotal(0.0f)
		, m_wClubID(CLUB_NULL)
		, m_nCountCaptured(0)
	{
		memset(szName, 0, CLUB_NAME + 1);
	}

	~GLPVPWoeAgent()
	{

	}
public:
	static GLPVPWoeAgent& GetInstance();

};