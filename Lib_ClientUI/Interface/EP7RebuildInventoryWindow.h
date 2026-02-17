#pragma	once

#include "./UIWindowEx.h"

class CEP7RebuildInventoryPage;
class CMultiModeButton;


#define EP7_REBUILD_INVEN_DEFAULT_VIEW 10
//--------------------------------------------------------------------
// EP7RebuildInventoryWindow : √÷¡ÿ«ı
//--------------------------------------------------------------------
class CEP7RebuildInventoryWindow : public CUIWindowEx	// ITEMREBUILD_MARK
{
protected:
	enum
	{
		REBUILDINVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,

#if defined( BUILD_EP7 )
		REBUILDINVENTORY_GAME_POINTS_BUTTON,
		REBUILDINVENTORY_WAR_CHIPS_BUTTON,
#endif

		REBUILDINVENTORY_PAGE,

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
	CEP7RebuildInventoryPage*	m_pPage;
	CBasicTextBox*			m_pMoneyTextBox;

#if defined( BUILD_EP7 )
	CBasicTextBox*			m_pGamePointsTextBox;
	CBasicTextBox*			m_pWarChipsTextBox;
#endif

	CMultiModeButton*		m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];

public:
	CEP7RebuildInventoryWindow();
	virtual	~CEP7RebuildInventoryWindow();

public:
	VOID CreateSubControl();
	CMultiModeButton* CreateRadioButton( char* szControl, char* szText, const UIGUID& cID = NO_ID );

public:
	virtual	VOID Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	VOID TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	VOID SetOneViewSlot( const INT& nONE_VIEW_SLOT );
	void OpenInvenPage( int nPage );
};