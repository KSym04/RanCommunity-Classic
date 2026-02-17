/*!
 * \file InfoWindow.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma once

#include "./UIWindowEx.h"

class CBasicTextBox;
class CBasicScrollBarEx;

class CInfoWindow : public CUIWindowEx
{
	enum
	{
		INFO_WINDOW_TEXT = ET_CONTROL_NEXT,
		INFO_WINDOW_SCROLLBAR,
	};

public:
	CInfoWindow();
	virtual ~CInfoWindow();

public:
	void CreateSubControl();

public:
	CBasicTextBox*		m_pTextInfo;
	CBasicScrollBarEx*	m_pListScrollBar;
	UIGUID				m_CallerID;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

	CBasicTextBox*		GetTextBox()	{ return m_pTextInfo; }
	void	SetCallerID( UIGUID uid )	{ m_CallerID = uid; }
};