/*!
 * \file ModernChaosMachineInventoryWindow.h
 *
 * \author Juver
 * \date 2021/07/08
 *
 * 
 */

#ifndef MODERNCHAOSMACHINEINVENTORYWINDOW_H_INCLUDED__
#define MODERNCHAOSMACHINEINVENTORYWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./UIWindowEx.h"

class CModernChaosMachineInvenPage;
class CBasicTextBox;
class CMultiModeButton;
class CBasicButton;

class CModernChaosMachineInvenWindow : public CUIWindowEx	
{
protected:
	enum
	{
		MODERN_CHAOS_MACHINE_INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,
		MODERN_CHAOS_MACHINE_INVENTORY_PAGE_INVEN,
		MODERN_CHAOS_MACHINE_INVENTORY_RADIO_BUTTON_0,
		MODERN_CHAOS_MACHINE_INVENTORY_RADIO_BUTTON_1,
		MODERN_CHAOS_MACHINE_INVENTORY_RADIO_BUTTON_2,
		MODERN_CHAOS_MACHINE_INVENTORY_HELP_BUTTON,
	};

	enum
	{
		MODERN_CHAOS_MACHINE_INVENTORY_PAGE_SIZE = 3,
	};

private:
	CModernChaosMachineInvenPage*	m_pPage;
	CUIControl*					m_pBackGround;
	CBasicTextBox*				m_pMoneyTextBox;
	CMultiModeButton*			m_pRadioButton[MODERN_CHAOS_MACHINE_INVENTORY_PAGE_SIZE];
	CBasicButton*				m_pHelpButton;
	int							m_nONE_VIEW_SLOT;

public:
	CModernChaosMachineInvenWindow();
	virtual	~CModernChaosMachineInvenWindow();

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


#endif // MODERNCHAOSMACHINEINVENTORYWINDOW_H_INCLUDED__