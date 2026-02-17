#pragma once
#include "GLCharDefine.h"

class GLFreePK
{
public:
	/*dmk14 freepk*/
	SNATIVEID						m_sMapID;
	float	m_fTIMER;
	float	m_fPKTIME;
	float	m_fUPDATETIMER;

public:
	GLFreePK() : m_fTIMER(0.0f), m_fPKTIME(0.0f), m_fUPDATETIMER(300.0f) {}
	virtual ~GLFreePK();

	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

	bool IsFREEPKMODE() { return m_fPKTIME > 0.0f; }

	void RESET() { m_fTIMER = 0.0f; m_fPKTIME = 0.0f; }
	/*dmk14 freepk*/
	void StartFREEPKMODE( DWORD dwTIME, SNATIVEID sMapID, SNATIVEID sItemReward );
	void EndFREEPKMODE();

	bool FrameMove( float fElapsedAppTime );

public:
	void	RequestInfoPC( DWORD dwCharID );

	static GLFreePK& GetInstance ();
}; // end of decl

class GLFreePKField : public GLFreePK
{
public:
	GLFreePKField();
	~GLFreePKField();

public:
	float							m_fPKTIMEField;
	SNATIVEID						m_sItemReward;
	int								m_nState;

	MAP_BATTLE_RANKING				m_mapBattleRankingP;
	VEC_BATTLE_RANKING				m_vecBattleRankingP;
	MAP_BATTLE_RANKING				m_mapBattleRankingC;
	VEC_BATTLE_RANKING				m_vecBattleRankingC;

public:
	void FrameMove( float fElapsedAppTime );
	bool IsFREEPKMODE() { return m_fPKTIMEField > 0.0f; }

public:
	void	StartFREEPKMODE( DWORD dwTIME, SNATIVEID sMapID, SNATIVEID sItemReward );
	void	EndFREEPKMODE();

	void	AddBattleRanking(DWORD dwKillID, DWORD dwDeathID);
	void	UpdateBattleRanking();
	void	ReqBattleRanking(DWORD dwClientID, int nPage);
	void	ReqMyBattleRanking(DWORD dwClientID, DWORD dwCharID, int nPage);
	void	SendBattleReward();

	void	RequestInfoPC( DWORD dwCharID );
	void	SetEventState( int nState )	{ m_nState = nState; }

public:
	static GLFreePKField& GetInstance();
};