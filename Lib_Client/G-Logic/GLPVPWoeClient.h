#pragma once
#include <string>
#include "./GLPVPWoeData.h"



class	GLPVPWoeClient
{
public:
	WOE_STATE		m_emState;
	WOE_SCHED_NEXT	m_sScheduleNext;
	
	WORD	m_wLevelReq;
	WORD	m_wPlayerLimit;

	float	m_fRemain;
	bool	m_bRegistered;

	DWORD	m_dwEmperiumGuildPrev;
	DWORD	m_dwEmperiumMarkVerPrev;

	BOOL	m_bShowStartNotice;

	char strName[64];
	char strLeader[64];


public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

public:
	bool	FrameMove ( float fTime, float fElapsedTime );

public:
	bool	IsRegister() { return bool(m_emState == WOE_STATE_REGISTER); }
	bool	IsEnded()	{ return bool(m_emState == WOE_STATE_ENDED); }
	bool	IsBattle()	{ return bool(m_emState == WOE_STATE_BATTLE); }
	bool	IsReward()	{ return bool(m_emState == WOE_STATE_REWARD); }
	void	SetEventState( WOE_STATE emState );
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward();
	void	DoStateEnd();
	void	NotifyTimeToStart( WORD wTime );
	void	DoRegister();
	void	DoRevive();
	void	RequestEmperiumInfo();
	void	ResetScore();
	void	RequestMapExit();

public:
	GLPVPWoeClient()
		: m_emState(WOE_STATE_ENDED)
		, m_fRemain(0.0f)
		, m_wLevelReq(1)
		, m_wPlayerLimit(1)
		, m_bRegistered(FALSE)
		, m_bShowStartNotice(FALSE)
		, m_dwEmperiumGuildPrev(0)
		, m_dwEmperiumMarkVerPrev(0)
	{
		strName[0] = '\0';
		strLeader[0] = '\0';
	}

	~GLPVPWoeClient()
	{

	}

public:
	static GLPVPWoeClient& GetInstance();

};