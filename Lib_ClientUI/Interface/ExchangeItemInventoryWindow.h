/*!
 * \file ExchangeItemInventoryWindow.h
 *
 * \author Juver
 * \date 2022/04/28
 *
 * 
 */

#ifndef EXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__
#define EXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./UIWindowEx.h"

class CExchangeItemInventoryPage;

class CExchangeItemInventoryWindow : public CUIWindowEx	
{
protected:
	enum
	{
		EXCHANGE_ITEM_INVEN_MONEY_BUTTON = ET_CONTROL_NEXT,
		EXCHANGE_ITEM_INVEN_GAME_POINTS_BUTTON,
		EXCHANGE_ITEM_INVEN_WAR_CHIPS_BUTTON,
		EXCHANGE_ITEM_INVEN_PAGE
	};

private:
	CExchangeItemInventoryPage*	m_pPage;
	CBasicTextBox*		m_pMoneyTextBox;
	CBasicTextBox*		m_pGamePointsTextBox;
	CBasicTextBox*		m_pWarChipsTextBox;
	INT					m_nONE_VIEW_SLOT;

public:
	CExchangeItemInventoryWindow();
	virtual	~CExchangeItemInventoryWindow();

public:
	void CreateSubControl();

public:
	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void SetOneViewSlot( const INT& nONE_VIEW_SLOT );
};

#endif // EXCHANGEITEMINVENTORYWINDOW_H_INCLUDED__