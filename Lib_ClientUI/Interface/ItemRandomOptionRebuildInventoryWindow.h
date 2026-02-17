/*!
 * \file ItemRandomOptionRebuildInventoryWindow.h
 *
 * \author Juver
 * \date July 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

class CItemRandomOptionRebuildInventoryPage;

class CItemRandomOptionRebuildInventoryWindow : public CUIWindowEx	
{
protected:
	enum
	{
		ITEM_RANDOM_OPTION_REBUILD_INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,
		ITEM_RANDOM_OPTION_REBUILD_INVENTORY_GAME_POINTS_BUTTON,
		ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WAR_CHIPS_BUTTON,
		ITEM_RANDOM_OPTION_REBUILD_INVENTORY_PAGE
	};

private:
	CItemRandomOptionRebuildInventoryPage*	m_pPage;
	CBasicTextBox*		m_pMoneyTextBox;
	CBasicTextBox*		m_pGamePointsTextBox;
	CBasicTextBox*		m_pWarChipsTextBox;
	INT					m_nONE_VIEW_SLOT;

public:
	CItemRandomOptionRebuildInventoryWindow();
	virtual	~CItemRandomOptionRebuildInventoryWindow();

public:
	void CreateSubControl();

public:
	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void SetOneViewSlot( const INT& nONE_VIEW_SLOT );
};