/*!
 * \file EP7ExchangeItemInventoryWindow.h
 *
 * \author Juver
 * \date 2022/04/28
 *
 * 
 */

#ifndef EP7EXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__
#define EP7EXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./UIWindowEx.h"

class CEP7ExchangeItenInventoryPage;
class CMultiModeButton;

#define EP7_EXCHANGE_ITEM_INVEN_DEFAULT_VIEW 10

class CEP7ExchangeItemInventoryWindow : public CUIWindowEx	
{
protected:
	enum
	{
		EXCHANGE_ITEM_INVEN_MONEY_BUTTON = ET_CONTROL_NEXT,

#if defined( BUILD_EP7 )
		EXCHANGE_ITEM_INVEN_GAME_POINTS_BUTTON,
		EXCHANGE_ITEM_INVEN_WAR_CHIPS_BUTTON,
#endif

		EXCHANGE_ITEM_INVEN_PAGE,

		EP7_INVENTORY_RADIO_BUTTON_0,
		EP7_INVENTORY_RADIO_BUTTON_1,
	};

	enum
	{
		EP7_INVENTORY_PAGE_SIZE = 2,
	};

	enum
	{
		EP7_INVENTORY_PAGE_INVEN_1 = 0,
		EP7_INVENTORY_PAGE_INVEN_2 = 1,
	};

private:
	CEP7ExchangeItenInventoryPage*	m_pPage;
	CBasicTextBox*			m_pMoneyTextBox;

#if defined( BUILD_EP7 )
	CBasicTextBox*			m_pGamePointsTextBox;
	CBasicTextBox*			m_pWarChipsTextBox;
#endif

	CMultiModeButton*		m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];

public:
	CEP7ExchangeItemInventoryWindow();
	virtual	~CEP7ExchangeItemInventoryWindow();

public:
	void CreateSubControl();
	CMultiModeButton* CreateRadioButton( char* szControl, char* szText, const UIGUID& cID = NO_ID );

public:
	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void SetOneViewSlot( const INT& nONE_VIEW_SLOT );
	void OpenInvenPage( int nPage );
};

#endif // EP7EXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__