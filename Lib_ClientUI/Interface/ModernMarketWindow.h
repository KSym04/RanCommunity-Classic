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
#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLCrowData.h"

class	CModernMarketPage;
class	CMultiModeButton;
class	CBasicButton;
class	CBasicTextBox;
class	CD3DFontPar;
class	CItemImage;

struct	SINVENITEM;

class	CModernMarketWindow : public CUIWindowEx
{
private:
static	const	int		nOUTOFRANGE;
protected:
	enum
	{
		nMAXPAGE = SCROWACTION::SALENUM,
		RECOMMEND_ITEM_TOTAL	= 5,
	};

	enum
	{
		MARKET_PAGE0 = ET_CONTROL_NEXT,
		MARKET_PAGE1,
		MARKET_PAGE2,
		MARKET_PAGE_END = MARKET_PAGE0 + nMAXPAGE,
		MARKET_WINDOW_PAGE_BUTTON0,
		MARKET_WINDOW_PAGE_BUTTON1,
		MARKET_WINDOW_PAGE_BUTTON2,
		MARKET_WINDOW_PAGE_BUTTON_END = MARKET_WINDOW_PAGE_BUTTON0 + nMAXPAGE,

		MARKET_WINDOW_RECOMMEND_ITEM_IMAGE0,
		MARKET_WINDOW_RECOMMEND_ITEM_IMAGE1,
		MARKET_WINDOW_RECOMMEND_ITEM_IMAGE2,
		MARKET_WINDOW_RECOMMEND_ITEM_IMAGE3,
		MARKET_WINDOW_RECOMMEND_ITEM_IMAGE4,

		MARKET_WINDOW_SETTING_CHECK_BOX_USABLE,
		MARKET_WINDOW_SETTING_CHECK_BOX_BUYABLE,

	};

public:
	CModernMarketWindow ();
	virtual	~CModernMarketWindow ();

public:
	void	CreateSubControl ();
	void	SetVisiblePage ( int nPage );	

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void		InitMarket ( SNATIVEID sNativeID );

public:
	SNATIVEID	GetMarketID ();

	void	SetItemIndex ( int nPosX, int nPosY );
	void	GetItemIndex ( int* pnPosX, int* pnPosY );	

	int		GetPageIndex ();

	SINVENITEM&	GetItem ( int nPosX, int nPosY );

	BOOL	IsUsingMaketPage( int nPage );

private:
	SNATIVEID	m_sMarketID;

private:
	int		m_nPosX;
	int		m_nPosY;

	int		m_nPageIndex;

	SINVENITEM		m_sItemRecommend[RECOMMEND_ITEM_TOTAL];

protected:
	CModernMarketPage*	CreateMarketPage ( UIGUID ControlID );
	CMultiModeButton*	CreateTextButton ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicButton*		CreateFlipButton ( const char* szButton, const char* szButtonFlip, UIGUID ControlID );
	CBasicTextBox*		CreateTextBox ( const char* szTextBox, DWORD dwTextAllign, CD3DFontPar* pFont );

private:
	CModernMarketPage*		m_pPage[nMAXPAGE];
	CMultiModeButton*		m_pPageButton[nMAXPAGE];

	CBasicTextBox*			m_pRecommendText;
	CItemImage*				m_pRecommendItemImage[RECOMMEND_ITEM_TOTAL];
	CBasicTextBox*			m_pRecommendItemText[RECOMMEND_ITEM_TOTAL];


	CBasicButton*			m_pSettingCheckBoxUsable;
	CBasicButton*			m_pSettingCheckBoxBuyable;
	CBasicTextBox*			m_pSettingTextUsable;
	CBasicTextBox*			m_pSettingTextBuyable;

	CBasicTextBox*			m_pMoneyText;

public:
	void ShowRecommendItem( DWORD dwChannel );
	void SetMoney ( LONGLONG Money );
};