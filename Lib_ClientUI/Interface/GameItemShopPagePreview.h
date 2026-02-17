/*!
 * \file GameItemShopPagePreview.h
 *
 * \author Juver
 * \date 2021/12/22
 *
 * 
 */

#ifndef GAMEITEMSHOPPAGEPREVIEW_H_INCLUDED__
#define GAMEITEMSHOPPAGEPREVIEW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../../Lib_Client/G-Logic/GLItem.h"
#include "../../Lib_Client/G-Logic/GLItemBox.h"

class CGameItemShopPageRender;
class CItemImage;
class CBasicButton;
class CBasicTextBox;

class	CGameItemShopPagePreview : public CUIGroup
{
	enum
	{
		GAME_ITEM_SHOP_PAGE_RENDER_PREVIEW = NO_ID + 1,
		GAME_ITEM_SHOP_PAGE_RENDER_ROTATE_LEFT,
		GAME_ITEM_SHOP_PAGE_RENDER_ROTATE_RIGHT,
		GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_MALE,
		GAME_ITEM_SHOP_PAGE_RENDER_BUTTON_FEMALE,

		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_0,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_1,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_2,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_3,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_4,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_5,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_6,

		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_7,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_8,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_9,
		/*GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_10,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_11,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_12,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_13,

		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_14,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_15,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_16,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_17,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_18,
		GAME_ITEM_SHOP_PAGE_RENDER_ITEM_IMAGE_19,*/
	};

private:

public:
	CGameItemShopPagePreview ();
	virtual	~CGameItemShopPagePreview ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CGameItemShopPageRender*	m_pRender;

	CItemImage*		m_pItemImageBox[ITEM::SBOX::ITEM_SIZE];
	CUIControl*		m_pMouseOver;

	CBasicButton*		m_pButtonLeft;
	CBasicButton*		m_pButtonRight;

	CBasicTextBox*		m_pTextMale;
	CBasicButton*		m_pButtonMale;

	CBasicTextBox*		m_pTextFemale;
	CBasicButton*		m_pButtonFemale;

	SNATIVEID		m_sItemIDBox[ITEM::SBOX::ITEM_SIZE];

public:
	void DefaultPreview();
	void DataReset();
	void DataShow( SNATIVEID sItemID );
};

#endif // GAMEITEMSHOPPAGEPREVIEW_H_INCLUDED__