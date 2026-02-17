#pragma	once

#include "./UIWindowEx.h"

class CModernGarbageInventoryPage;
class CBasicTextBox;
class CMultiModeButton;
class CBasicButton;

//--------------------------------------------------------------------
// CModernGarbageInventoryWindow : 
//--------------------------------------------------------------------
class CModernGarbageInventoryWindow : public CUIWindowEx	// »ﬁ¡ˆ≈Î
{
protected:
	enum
	{
		MODERN_GARBAGE_INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,
		MODERN_GARBAGE_INVENTORY_PAGE_INVEN,
		MODERN_GARBAGE_INVENTORY_RADIO_BUTTON_0,
		MODERN_GARBAGE_INVENTORY_RADIO_BUTTON_1,
		MODERN_GARBAGE_INVENTORY_RADIO_BUTTON_2,
		MODERN_GARBAGE_INVENTORY_HELP_BUTTON,
	};

	enum
	{
		MODERN_GARBAGE_INVENTORY_PAGE_SIZE = 3,
	};

private:
	CModernGarbageInventoryPage*	m_pPage;
	CUIControl*					m_pBackGround;
	CBasicTextBox*				m_pMoneyTextBox;
	CMultiModeButton*			m_pRadioButton[MODERN_GARBAGE_INVENTORY_PAGE_SIZE];
	CBasicButton*				m_pHelpButton;
	int							m_nONE_VIEW_SLOT;

public:
	CModernGarbageInventoryWindow();
	virtual	~CModernGarbageInventoryWindow();

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