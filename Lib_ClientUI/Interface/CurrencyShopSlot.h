#ifndef CURRENCYSHOPSLOT_H__INCLUDED
#define CURRENCYSHOPSLOT_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-logic/GLdefine.h"

#include "../../Lib_Client/G-Logic/GLCurrencyShop.h"

class CBasicLineBox;
class CItemImage;
class CBasicButton;
class CBasicTextBox;
class CBasicTextButton;

class	CCurrencyShopSlot : public CUIGroup
{
	enum
	{
		CURRENCY_SHOP_SLOT_ITEM_IMAGE_RES = NO_ID + 1,
		CURRENCY_SHOP_SLOT_BUTTON_COST_MONEY,
		CURRENCY_SHOP_SLOT_BUTTON_COST_GAME_POINTS,
		CURRENCY_SHOP_SLOT_BUTTON_COST_WAR_CHIPS,
		CURRENCY_SHOP_SLOT_BUTTON_PURCHASE,
	};


public:
	CCurrencyShopSlot ();
	virtual	~CCurrencyShopSlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicTextBox*	m_pTextName;
	CItemImage*		m_pItemImageResult;

	CBasicButton*	m_pButtonCostMoney;
	CBasicButton*	m_pButtonCostGamePoints;
	CBasicButton*	m_pButtonCostWarChips;

	CBasicTextBox*	m_pTextCostMoney;
	CBasicTextBox*	m_pTextCostGamePoints;
	CBasicTextBox*	m_pTextCostWarChips;

	CBasicTextButton*	m_pButtonPurchase;

private:
	SNATIVEID		m_sNpcID;
	SNATIVEID		m_sItemID;

public:
	void	DataReset();
	void	DataSet( SNATIVEID sNpcID, SCURRENCY_SHOP_ITEM* pData );
	void	DoPurchase();
	void	ShowItemInfo( SCURRENCY_SHOP_ITEM* pData, DWORD dwUIMsg );
	void	UpdatePrice();
};

#endif // CURRENCYSHOPSLOT_H__INCLUDED
