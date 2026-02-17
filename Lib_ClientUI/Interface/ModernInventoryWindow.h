/*!
 * \file ModernInventoryWindow.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */

#pragma once

#include "./UIWindowEx.h"

class CModernInventoryWindowInven;
class CBasicTextBox;
class CMultiModeButton;
class CBasicButton;

class CModernInventoryWindow : public CUIWindowEx
{
	enum
	{
		MODERN_INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,

		MODERN_INVENTORY_PAGE_INVEN,
		
		MODERN_INVENTORY_RADIO_BUTTON_0,
		MODERN_INVENTORY_RADIO_BUTTON_1,
		MODERN_INVENTORY_RADIO_BUTTON_2,

		MODERN_INVENTORY_HELP_BUTTON,

		MODERN_INVENTORY_LOCKER_BUTTON,
		MODERN_INVENTORY_EXTEND_BUTTON,
		MODERN_INVENTORY_TRASH_BUTTON,
		MODERN_INVENTORY_SORT_BUTTON,
	};

	enum
	{
		MODERN_INVENTORY_PAGE_SIZE = 3,
	};

public:
	CModernInventoryWindow();
	virtual ~CModernInventoryWindow();

public:
	CMultiModeButton* CreateRadioButton( char* szControl, char* szText, const UIGUID& cID = NO_ID );
	CMultiModeButton* CreateBlueButton( char* szControl, const UIGUID& cID = NO_ID );
	void CreateSubControl();

private:
	CUIControl*			m_pBackGround;
	CBasicTextBox*		m_pMoneyTextBox;
	CMultiModeButton*	m_pRadioButton[MODERN_INVENTORY_PAGE_SIZE];
	CBasicButton*		m_pHelpButton;
	CModernInventoryWindowInven*	m_pInven;

private:
	int		m_nONE_VIEW_SLOT;
	WORD	m_wSplitItemPosX;
	WORD	m_wSplitItemPosY;
	BOOL	m_bSplitItem;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	GetSplitPos ( WORD* pwPosX, WORD* pwPosY );
	void	SetSplitPos ( WORD wPosX, WORD wPosY );
	void	SetOneViewSlot ( const int& nONE_VIEW_SLOT );
	const int& GetOneViewSlot () const						{ return m_nONE_VIEW_SLOT; }

private:
	void	SetMoney ( LONGLONG Money );
	void	OpenPage( int nPage );

};