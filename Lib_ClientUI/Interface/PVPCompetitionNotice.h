#ifndef PVPCOMPETITIONNOTICE_H__INCLUDED
#define PVPCOMPETITIONNOTICE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class	CBasicTextBox;

class	CPVPCompetitionNotice : public CUIGroup
{
public:
	CPVPCompetitionNotice ();
	virtual	~CPVPCompetitionNotice ();

public:
	void	CreateSubControl ();

public:
	void	AddText ( CString strMessage, D3DCOLOR dwMessageColor );

public:
	virtual void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicTextBox*	m_pTextBox;	

private:
	float	m_fLifeTime;
};

#endif // PVPCOMPETITIONNOTICE_H__INCLUDED
