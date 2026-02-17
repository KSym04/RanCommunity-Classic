/*!
 * \file ItemShopWindow.h
 *
 * \author Juver
 * \date July 2017
 *
 * 
 */

#pragma	once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLCharData.h"
#include "../Lib_ClientUI/Interface/GameItemShopPagePreview.h"  // Include the preview class

class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CBasicTextButton;
class	CBasicButton;

class	CItemShopItemSlot;
class	CGameItemShopPagePreview;

class CItemShopWindow : public CUIWindowEx
{
public:
	enum
	{
		ITEM_SHOP_SELECTION_TEXTBOX = ET_CONTROL_NEXT,
		ITEM_SHOP_SELECTION_SCROLLBAR,
		ITEM_SHOP_PURCHASE_BUTTON,
		ITEM_SHOP_PREV_BUTTON,
		ITEM_SHOP_NEXT_BUTTON,
		ITEM_SHOP_CONFIRM_BUTTON,
		ITEM_SHOP_ITEM00,
		ITEM_SHOP_ITEM01,
		ITEM_SHOP_ITEM02,
		ITEM_SHOP_ITEM03,
		ITEM_SHOP_ITEM04,
		ITEM_SHOP_ITEM05,
		ITEM_SHOP_ITEM06,
		ITEM_SHOP_ITEM07,
		ITEM_SHOP_ITEM08,
		ITEM_SHOP_ITEM09,
		ITEM_SHOP_ITEM10,
		ITEM_SHOP_ITEM11,
		ITEM_SHOP_PAGE_RENDER,
		
	};

	enum
	{
		ITEM_SHOP_MAX_ITEM = 12,
		ITEM_SHOP_MAX_CATEGORY = 13
	};

public:
	CItemShopWindow ();
	virtual	~CItemShopWindow ();

public:
	void	CreateSubControl ();

private:
	CBasicTextBoxEx*	m_pListTextBox;
	CBasicScrollBarEx*	m_pListScrollBar;
	CBasicTextButton*	m_pButtonPurchase;
	CBasicButton*		m_pButtonPrev;
	CBasicButton*		m_pButtonNext;
	CBasicTextBox*		m_pTextPage;
	CBasicTextBox*		m_pTextStaticBattlePoint;
	CBasicTextBox*		m_pTextCharBattlePoint;
	CBasicTextBox*		m_pTextStaticEPoint;
	CBasicTextBox*		m_pTextCharEPoint;
	CItemShopItemSlot*	m_pItemSlot[ITEM_SHOP_MAX_ITEM];

	CGameItemShopPagePreview* m_pPreview;

	CBasicButton*		m_pBuyConfirm;
	CBasicTextBox*		m_pBuyConfirmText;
	CBasicTextBox*		m_pMidTitleText;
	CBasicTextBox*		m_pLeftTitleText;
	CBasicTextBox*		m_pLeftCharTitleText;
	bool				bItemShopConfirm;


private:
	int	m_nSelectIndexType;
	int	m_nCurrentPage;
	SNATIVEID	m_sSelectItem;
	CString		m_strItemNum;
	SNATIVEID	m_sCrow;
	int m_nMaxPage;

private:
	//SITEM_SHOP_TYPE_MAP	m_mapType;
	ITEMMAPSHOP	m_mapItem;
	VECITEMSHOP	m_vecItem;
	WORD				m_wShopType;

public:
	void InitShop();
	void ResetShop();
	void SelectType( DWORD dwType );
	void ShowItem( int nPage );
	void SelectItem( int nSelect );
	const CString& GetPurKey() { return m_strItemNum; }

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
};