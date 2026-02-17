/*!
 * \file ModernItemRandomOptionRebuildInventoryWindow.h
 *
 * \author Juver
 * \date July 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

class CModernItemRandomOptionRebuildInventoryPage;
class CBasicTextBox;
class CMultiModeButton;
class CBasicButton;

class CModernItemRandomOptionRebuildInventoryWindow : public CUIWindowEx	
{
protected:
	enum
	{
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_PAGE_INVEN,
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_RADIO_BUTTON_0,
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_RADIO_BUTTON_1,
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_RADIO_BUTTON_2,
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_HELP_BUTTON,
	};

	enum
	{
		MODERN_RANDOM_OPTION_REBUILD_INVENTORY_PAGE_SIZE = 3,
	};

private:
	CModernItemRandomOptionRebuildInventoryPage*	m_pPage;
	CUIControl*					m_pBackGround;
	CBasicTextBox*				m_pMoneyTextBox;
	CMultiModeButton*			m_pRadioButton[MODERN_RANDOM_OPTION_REBUILD_INVENTORY_PAGE_SIZE];
	CBasicButton*				m_pHelpButton;
	int							m_nONE_VIEW_SLOT;

public:
	CModernItemRandomOptionRebuildInventoryWindow();
	virtual	~CModernItemRandomOptionRebuildInventoryWindow();

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