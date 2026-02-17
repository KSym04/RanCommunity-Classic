/*!
 * \file ItemTransferInventoryWindow.h
 *
 * \author Juver
 * \date January 2018
 *
 * 
 */

#pragma	once

#include "./UIWindowEx.h"

class CEP7ItemTransferInvenPage;
class CMultiModeButton;

#define EP7_ITEM_TRANSFER_INVEN_DEFAULT_VIEW 10

class CEP7ItemTransferInvenWindow : public CUIWindowEx	
{
protected:
	enum
	{
		ITEM_TRANSFER_INVEN_MONEY_BUTTON = ET_CONTROL_NEXT,

#if defined( BUILD_EP7 )
		ITEM_TRANSFER_INVEN_GAME_POINTS_BUTTON,
		ITEM_TRANSFER_INVEN_WAR_CHIPS_BUTTON,
#endif

		ITEM_TRANSFER_INVEN_PAGE,

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
	CEP7ItemTransferInvenPage*	m_pPage;
	CBasicTextBox*			m_pMoneyTextBox;

#if defined( BUILD_EP7 )
	CBasicTextBox*			m_pGamePointsTextBox;
	CBasicTextBox*			m_pWarChipsTextBox;
#endif

	CMultiModeButton*		m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];

public:
	CEP7ItemTransferInvenWindow();
	virtual	~CEP7ItemTransferInvenWindow();

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