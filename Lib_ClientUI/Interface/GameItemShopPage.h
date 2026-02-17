/*!
 * \file GameItemShopPage.h
 *
 * \author Juver
 * \date 2021/11/04
 *
 * 
 */

#ifndef GAMEITEMSHOPPAGE_H_INCLUDED__
#define GAMEITEMSHOPPAGE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLGameItemShopDef.h"

class CGameItemShopPageType;
class CGameItemShopPageItemSlot;
class CGameItemShopPagePreview;

class CBasicButton;
class CBasicTextBox;



class CGameItemShopPage : public CUIGroup
{
private:
	enum
	{
		GAME_ITEM_SHOP_PAGE_TYPE = NO_ID + 1,
		GAME_ITEM_SHOP_ITEM00,
		GAME_ITEM_SHOP_ITEM01,
		GAME_ITEM_SHOP_ITEM02,
		GAME_ITEM_SHOP_ITEM03,
		GAME_ITEM_SHOP_ITEM04,
		GAME_ITEM_SHOP_ITEM05,
		GAME_ITEM_SHOP_ITEM06,
		GAME_ITEM_SHOP_ITEM07,
		GAME_ITEM_SHOP_ITEM08,
		GAME_ITEM_SHOP_ITEM09,
		GAME_ITEM_SHOP_ITEM10,
		GAME_ITEM_SHOP_ITEM11,
		GAME_ITEM_SHOP_ITEM12,
		GAME_ITEM_SHOP_ITEM13,
		GAME_ITEM_SHOP_PREV_BUTTON,
		GAME_ITEM_SHOP_NEXT_BUTTON,
		GAME_ITEM_SHOP_PAGE_RENDER,
	};

	enum
	{
		GAME_ITEM_SHOP_MAX_ITEM = 14,
	};

public:
	CGameItemShopPage ();
	virtual	~CGameItemShopPage ();

public:
	void	CreateSubControl ();

public:
	WORD	m_wPageType;
	WORD	m_wCategory;
	int		m_nCurrentPage;
	int		m_nMaxPage;
	SNATIVEID m_sSelected;

	CGameItemShopPageType*		m_pType;
	CBasicButton*		m_pButtonPrev;
	CBasicButton*		m_pButtonNext;
	CBasicTextBox*		m_pTextPage;

	CBasicTextBox*		m_pTextPointInfo;

	CGameItemShopPagePreview*	m_pPreview;


	SGAME_ITEM_SHOP_DATA_VEC	m_vecShop;

	CGameItemShopPageItemSlot* m_pSlot[GAME_ITEM_SHOP_MAX_ITEM];

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void ReloadShop();
	void DefaultPreview();
	void SelectType();
	void ShowItem();
	void SelectItem( int nSelect );
};


#endif // GAMEITEMSHOPPAGE_H_INCLUDED__
