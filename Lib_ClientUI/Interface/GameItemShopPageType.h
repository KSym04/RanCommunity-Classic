/*!
 * \file GameItemShopPageType.h
 *
 * \author Juver
 * \date 2021/11/04
 *
 * 
 */

#ifndef GAMEITEMSHOPPAGETYPE_H_INCLUDED__
#define GAMEITEMSHOPPAGETYPE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Client/G-Logic/GLGameItemShopDef.h"
#include "../../Lib_Engine/GUInterface/UIGroup.h"

const DWORD UIMSG_GAME_ITEM_SHOP_SELECT = UIMSG_USER1;

class CGameItemShopPageTypeBase;

class CGameItemShopPageType : public CUIGroup
{
private:
	enum
	{
		GAME_ITEM_SHOP_TYPE_SPECIAL = NO_ID + 1,
		GAME_ITEM_SHOP_TYPE_CONSUMABLE,
		GAME_ITEM_SHOP_TYPE_WEARABLE,
		GAME_ITEM_SHOP_TYPE_ACCESSORY,
		GAME_ITEM_SHOP_TYPE_PET,
		GAME_ITEM_SHOP_TYPE_VEHICLE,
	};

public:
	enum
	{
		GAME_ITEM_SHOP_TYPE_SIZE = 6,
	};

public:
	CGameItemShopPageType ();
	virtual	~CGameItemShopPageType ();

public:
	CGameItemShopPageTypeBase*		m_pIconType[GAME_ITEM_SHOP_TYPE_SIZE];

public:
	int			m_nSelect;

public:
	void	CreateSubControl ();
	int		GetSelect()	{ return m_nSelect; }
	void	DefaultSelect();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

};


#endif // GAMEITEMSHOPPAGETYPE_H_INCLUDED__
