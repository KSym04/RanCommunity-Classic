/*
~ RRSECURTY DEVELOPMENTS 22/05/22 5:44 PM
*/

#pragma once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class	CBasicTextBox;

class	CPKStreakKillNotice : public CUIGroup
{
public:
	CPKStreakKillNotice ();
	virtual	~CPKStreakKillNotice ();

public:
	void	CreateSubControl ();

public:
	int	AddText ( CString strMessage, D3DCOLOR dwMessageColor );
	int		AddString(int Index, CString strMessage, D3DCOLOR dwMessageColor);

public:
	virtual void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicTextBox*	m_pTextBox;	

private:
	float	m_fLifeTime;
};