#pragma	once

#include "./UIWindowEx.h"

class	CEP7InventoryPage;
class	CInventoryPageWear;
class	CInventoryPageWearEx;
class	CBasicTextButton;
class	CMultiModeButton;

#define EP7_INVEN_DEFAULT_VIEW 10

class	CEP7InventoryWindow : public CUIWindowEx
{
protected:
	enum
	{
		INVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,


		INVENTORY_PAGE,
		INVENTORY_PAGEWEAR,
		INVENTORY_PAGEWEAR_EX,
		INVENTORY_SORT_BUTTON,			/*inventory sort, Juver, 2018/07/07 */

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
	CBasicTextButton*			m_pButtonPageMain;
	CBasicTextButton*			m_pButtonPageSub;
	CInventoryPageWear*			m_pPageWear;
	CInventoryPageWearEx*		m_pPageWearEx;
	CMultiModeButton*			m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];
	CEP7InventoryPage*			m_pPage;
	CBasicTextBox*				m_pMoneyTextBox;


	CBasicTextButton*			m_pSortButton;	/*inventory sort, Juver, 2018/07/07 */
	

	WORD	m_wSplitItemPosX;
	WORD	m_wSplitItemPosY;
	BOOL	m_bSplitItem;

public:
	CEP7InventoryWindow ();
	virtual	~CEP7InventoryWindow ();

public:
	CMultiModeButton* CreateRadioButton( char* szControl, char* szText, const UIGUID& cID = NO_ID );

	void	CreateSubControl ();

public:
	void	GetSplitPos ( WORD* pwPosX, WORD* pwPosY );
	void	SetSplitPos ( WORD wPosX, WORD wPosY );

	void	SetArmSwapTabButton( BOOL bArmSub );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

private:
	void	SetMoney ( LONGLONG Money );


public:
	void	SetOneViewSlot ( const int& nONE_VIEW_SLOT );
	int		GetOneViewSlot () { return EP7_INVEN_DEFAULT_VIEW; }

	void	OpenInvenPage( int nPage );
};