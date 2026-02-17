/*!
 * \file ModernExchangeItemInventoryWindow.h
 *
 * \author Juver
 * \date 2022/04/27
 *
 * 
 */

#ifndef MODERNEXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__
#define MODERNEXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./UIWindowEx.h"

class CModernExchangeItemInventoryPage;
class CBasicTextBox;
class CMultiModeButton;
class CBasicButton;

class CModernExchangeItemInventoryWindow : public CUIWindowEx	
{
protected:
	enum
	{
		MODERN_EXCHANGE_ITEM_INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,
		MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_INVEN,
		MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_0,
		MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_1,
		MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_2,
		MODERN_EXCHANGE_ITEM_INVENTORY_HELP_BUTTON,
	};

	enum
	{
		MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE = 3,
	};

private:
	CModernExchangeItemInventoryPage*	m_pPage;
	CUIControl*					m_pBackGround;
	CBasicTextBox*				m_pMoneyTextBox;
	CMultiModeButton*			m_pRadioButton[MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE];
	CBasicButton*				m_pHelpButton;
	int							m_nONE_VIEW_SLOT;

public:
	CModernExchangeItemInventoryWindow();
	virtual	~CModernExchangeItemInventoryWindow();

public:
	CMultiModeButton* CreateRadioButton( char* szControl, char* szText, const UIGUID& cID = NO_ID );
	void CreateSubControl();

public:
	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void SetOneViewSlot( const INT& nONE_VIEW_SLOT );

private:
	void	SetMoney ( LONGLONG Money );
	void	OpenPage( int nPage );
};


#endif // MODERNEXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__