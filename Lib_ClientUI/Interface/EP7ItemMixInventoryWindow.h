#pragma	once

#include "./UIWindowEx.h"

class CEP7ItemMixInvenPage;
class CMultiModeButton;


#define EP7_ITEMMIX_INVEN_DEFAULT_VIEW 10

//--------------------------------------------------------------------
// CEP7ItemMixInvenWindow : 
//--------------------------------------------------------------------
class CEP7ItemMixInvenWindow : public CUIWindowEx	// »ﬁ¡ˆ≈Î
{
protected:
	enum
	{
		ITEMMIXINVEN_MONEY_BUTTON = ET_CONTROL_NEXT,

#if defined( BUILD_EP7 )
		ITEMMIXINVEN_GAME_POINTS_BUTTON,
		ITEMMIXINVEN_WAR_CHIPS_BUTTON,
#endif

		ITEMMIXINVEN_PAGE,

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
	CEP7ItemMixInvenPage*	m_pPage;
	CBasicTextBox*			m_pMoneyTextBox;

#if defined( BUILD_EP7 )
	CBasicTextBox*			m_pGamePointsTextBox;
	CBasicTextBox*			m_pWarChipsTextBox;
#endif

	CMultiModeButton*		m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];

public:
	CEP7ItemMixInvenWindow();
	virtual	~CEP7ItemMixInvenWindow();

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