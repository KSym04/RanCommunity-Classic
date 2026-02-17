//	마켓 윈도우
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.12.6]
//			@ 작성
//

#pragma	once

#include "./UIWindowEx.h"

struct	SITEMCUSTOM;
class CEP7TradeInventoryPage;
class CMultiModeButton;


#define EP7_REBUILD_INVEN_DEFAULT_VIEW 10


class	CEP7TradeInventoryWindow : public CUIWindowEx
{
protected:
	enum
	{
		TRADEINVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,


		TRADEINVENTORY_PAGE,

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

public:
	CEP7TradeInventoryWindow ();
	virtual	~CEP7TradeInventoryWindow ();

public:
	void	CreateSubControl ();
	CMultiModeButton* CreateRadioButton( char* szControl, char* szText, const UIGUID& cID = NO_ID );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	SetOPENER ( bool bOPENER );

private:
	void	SetMoney ( LONGLONG Money );


	bool	IsTurnITEM ( SITEMCUSTOM& sItemCustom );

public:
	void	SetOneViewSlot ( const int& nONE_VIEW_SLOT );
	void	OpenInvenPage( int nPage );

private:
	CEP7TradeInventoryPage*	m_pPage;

private:
	CBasicTextBox*			m_pMoneyTextBox;


	CMultiModeButton*		m_pRadioButton[EP7_INVENTORY_PAGE_SIZE];

	bool	m_bTRADE_MODE;
	bool	m_bOPENER;
};