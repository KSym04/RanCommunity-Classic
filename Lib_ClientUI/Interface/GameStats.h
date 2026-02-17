#pragma once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;

class CGameStats : public CUIGroup
{
protected:
	
public:
	CGameStats();
	virtual ~CGameStats(void);

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	float			m_fPingAgent;
	float			m_fPingField;
	float			m_fFPS;
	float			m_fPingAgentBack;
	float			m_fPingFieldBack;
	float			m_fFPSBack;
    CBasicTextBox*	m_pPingTextAgent;
	CBasicTextBox*	m_pPingTextField;
	CBasicTextBox*	m_pFpsText;

	void UpdateInfo();

public:
	void UpdatePingAgent( float fPing ) { m_fPingAgent = fPing; }
	void UpdatePingField( float fPing ) { m_fPingField =  fPing; }
	void UpdateFPS( float fFPS ) { m_fFPS = fFPS; }
};