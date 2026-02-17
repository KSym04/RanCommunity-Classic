#pragma	once

#include "./UIWindowEx.h"

class CEP7GarbageInventoryPage;
class CMultiModeButton;


#define EP7_GARBAGE_INVEN_DEFAULT_VIEW 10

//--------------------------------------------------------------------
// CEP7GarbageInventoryWindow : 
//--------------------------------------------------------------------
class CEP7GarbageInventoryWindow : public CUIWindowEx	// »ﬁ¡ˆ≈Î
{
protected:
	enum
	{
		GARBAGEINVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,


		GARBAGEINVENTORY_PAGE,

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
	CEP7GarbageInventoryPage*	m_pPage;
	CBasicTextBox*			m_pMoneyTextBox;


	CMultiModeButton*		m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];

public:
	CEP7GarbageInventoryWindow();
	virtual	~CEP7GarbageInventoryWindow();

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