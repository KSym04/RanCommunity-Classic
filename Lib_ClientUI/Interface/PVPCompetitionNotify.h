#ifndef PVPCOMPETITIONNOTIFY_H__INCLUDED
#define PVPCOMPETITIONNOTIFY_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"

class CBasicLineBoxEx;
class CBasicTextBox;
class CD3DFontPar;

class	CPVPCompetitionNotify : public CUIGroup
{
private:

public:
	CPVPCompetitionNotify ();
	virtual	~CPVPCompetitionNotify ();

public:
	void	CreateSubControl ();

public:
	CD3DFontPar*		m_pFont;
	CBasicLineBoxEx*	m_pLineBox;
	CBasicTextBox*		m_pTextBox;
	CUIControl*			m_pTail;
	CUIControl*			m_pDummy;

	CUIControl*			m_pControlPos;

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void	SetControlPos( CUIControl* pControl );
	void	SetNotify( std::string strText, DWORD dwColor );
};

#endif // PVPCOMPETITIONNOTIFY_H__INCLUDED


