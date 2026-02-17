
#pragma	once

#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/GLGameItemShopDef.h"

class CUIControl;
class CBasicLineBox;
class CBasicTextBoxEx;
class CBasicButton;

class CBasicTextButton;
class CMultiModeButton;

class CGameItemShopPage;

class CGameItemShopWindow : public CUIWindowEx
{
public:
	enum
	{
		GAME_ITEM_SHOP_WINDOW_BUTTON_0	= ET_CONTROL_NEXT,
		GAME_ITEM_SHOP_WINDOW_BUTTON_1,
		GAME_ITEM_SHOP_WINDOW_PAGE_0,
		GAME_ITEM_SHOP_WINDOW_PAGE_1,
	};

	enum 
	{
		GAME_ITEM_SHOP_WINDOW_POINT_SHOP	= 0,
		GAME_ITEM_SHOP_WINDOW_VOTE_SHOP		= 1,
		GAME_ITEM_SHOP_WINDOW_SIZE			= 2,
	};

public:
	CGameItemShopWindow ();
	virtual	~CGameItemShopWindow ();

public:
	void	CreateSubControl ();

	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );

public:
	CMultiModeButton*		m_pButton[GAME_ITEM_SHOP_WINDOW_SIZE];
	CGameItemShopPage*		m_pPage[GAME_ITEM_SHOP_WINDOW_SIZE];

	CBasicTextBox*	m_pUserPointText;
	CBasicTextBox*	m_pVotePointText;

	int		m_nPage;
	BOOL	m_bFirstOpen;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void ReloadShop();
	void OpenPage( int nPage );
};