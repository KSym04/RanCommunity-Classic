/*!
 * \file ChangeSchoolWindow.h
 *
 * \author CNDev
 * \date May 2023
 *
 * 
 */

/* 5-15-23 Change Color Name Card - CNDev */

#pragma once

#include "UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;
class CBasicButton;
class CBasicTextButton;

class CChangeColorNameWindow : public CUIWindowEx
{
	enum
	{
		CHANGE_COLOR_NAME_WINDOW_BUTTON_SAVE = ET_CONTROL_NEXT,
		CHANGE_COLOR_NAME_WINDOW_BUTTON_CANCEL,
		CHANGE_COLOR_NAME_WINDOW_SELECT_ONE,
		CHANGE_COLOR_NAME_WINDOW_SELECT_TWO,
		CHANGE_COLOR_NAME_WINDOW_SELECT_THREE,
		CHANGE_COLOR_NAME_WINDOW_SELECT_FOUR,
		CHANGE_COLOR_NAME_WINDOW_SELECT_FIVE,
		CHANGE_COLOR_NAME_WINDOW_SELECT_SIX,

		CHANGE_COLOR_NAME_WINDOW_SIZE = 6,
	};

public:
	CChangeColorNameWindow(void);
	virtual ~CChangeColorNameWindow(void);

public:
	void CreateSubControl();

private:
	CUIControl*		m_pTypeColor_Off[CHANGE_COLOR_NAME_WINDOW_SIZE];
	CUIControl*		m_pTypeColor_On[CHANGE_COLOR_NAME_WINDOW_SIZE];
	CUIControl*		m_pTypeColor_Box[CHANGE_COLOR_NAME_WINDOW_SIZE];
	CUIControl*		m_pTypeColor_Focus[CHANGE_COLOR_NAME_WINDOW_SIZE];

	CBasicTextBox*	m_pInfoTitle;
	CBasicTextBox*	m_pInfoDesc;

	CBasicTextButton*	m_pButtonClose;
	CBasicTextButton*	m_pButtonSave;

public:
	int		m_nSelectedLastColor;
	int		m_nSelectedColor;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

	void	ResetAll();
	void	UpdateUI();
	void	ColorSelect( int nSelect );
	void	ColorMouseIn( int nSelect );


};