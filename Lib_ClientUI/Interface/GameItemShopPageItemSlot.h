/*!
 * \file GameItemShopPageItemSlot.h
 *
 * \author Juver
 * \date 2021/11/10
 *
 * 
 */

#ifndef GAMEITEMSHOPPAGEITEMSLOT_H_INCLUDED__
#define GAMEITEMSHOPPAGEITEMSLOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Client/G-Logic/GLGameItemShopDef.h"

class CBasicLineBox;
class CBasicTextBox;
class CItemImage;
class CBasicTextButton;

const DWORD UIMSG_GAME_ITEM_SHOP_SLOT_ITEM_IMAGE = UIMSG_USER1;
const DWORD UIMSG_GAME_ITEM_SHOP_SLOT_BUTTON = UIMSG_USER2;

class	CGameItemShopPageItemSlot : public CUIGroup
{
	enum
	{
		GAME_ITEM_SHOP_SLOT_ITEM_IMAGE = NO_ID + 1,
		GAME_ITEM_SHOP_SLOT_BUTTON,
	};


public:
	CGameItemShopPageItemSlot ();
	virtual	~CGameItemShopPageItemSlot ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CUIControl*		m_pSelectImage;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextPrice;
	CBasicTextBox*	m_pTextStock;
	CItemImage*		m_pItemImage;
	CBasicTextButton*	m_pBuyButton;

	float m_fUpdateTime;

public:
	SGAME_ITEM_SHOP_DATA  m_sData;

public:
	void	ItemSet( const SGAME_ITEM_SHOP_DATA* pShopItem );
	void	ItemReset();
	void	ItemSelect( BOOL bSelect );
};

#endif // GAMEITEMSHOPPAGEITEMSLOT_H_INCLUDED__
