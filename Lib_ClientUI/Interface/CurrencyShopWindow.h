#ifndef CURRENCYSHOPWINDOW_H__INCLUDED
#define CURRENCYSHOPWINDOW_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/GLCurrencyShop.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

class	CBasicTextBox;
class	CBasicButton;
class	CCurrencyShopSlot;

class CCurrencyShopWindow : public CUIWindowEx
{
public:
	enum
	{
		CURRENCY_SHOP_PREV_BUTTON = ET_CONTROL_NEXT,
		CURRENCY_SHOP_NEXT_BUTTON,
		CURRENCY_SHOP_ITEM00,
		CURRENCY_SHOP_ITEM01,
		CURRENCY_SHOP_ITEM02,
		CURRENCY_SHOP_ITEM03,
		CURRENCY_SHOP_ITEM04,
		CURRENCY_SHOP_ITEM05,
		CURRENCY_SHOP_ITEM06,
		CURRENCY_SHOP_ITEM07,
		CURRENCY_SHOP_ITEM08,
		CURRENCY_SHOP_ITEM09,
	};

	enum
	{
		CURRENCY_SHOP_MAX_SLOT = 10,
	};

public:
	CCurrencyShopWindow ();
	virtual	~CCurrencyShopWindow ();

public:
	void	CreateSubControl ();

public:
	CCurrencyShopSlot*	m_pSlot[CURRENCY_SHOP_MAX_SLOT];
	CBasicTextBox*		m_pTextPage;
	CBasicButton*		m_pButtonPrev;
	CBasicButton*		m_pButtonNext;

private:
	SCURRENCY_SHOP_ITEM_VEC m_vecList;
	SNATIVEID	m_sNpcID;
	int	m_nCurrentPage;
	int m_nMaxPage;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	LoadList( SNATIVEID sNpcID );
	void	ResetList();
	void	ShowPage( int nPage );
	void	UpdatePrice();
};

#endif // CURRENCYSHOPWINDOW_H__INCLUDED