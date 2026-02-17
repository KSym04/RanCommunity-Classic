#ifndef PVPCOMPETITIONBUTTON_H__INCLUDED
#define PVPCOMPETITIONBUTTON_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CPVPCompetitionButton : public CUIGroup
{
private:
	enum
	{
		COMPETITION_BUTTON_IMAGE = NO_ID + 1,
	};

public:
	CPVPCompetitionButton ();
	virtual	~CPVPCompetitionButton ();

public:
	void	CreateSubControl ();

public:
	CUIControl*							m_pButtonImage;
	CUIControl*							m_pButtonBlink;
	CUIControl*							m_pButtonLock;


public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	float	m_fBLINK_TIME;
	BOOL	m_bAlarm;
	BOOL	m_bLocked;

public:
	void	SetButtonAlarm( BOOL bSet );
	void	SetButtonLock( BOOL bSet );
};

#endif // PVPCOMPETITIONBUTTON_H__INCLUDED

