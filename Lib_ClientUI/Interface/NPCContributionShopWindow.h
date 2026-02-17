/*!
 * \file NPCContributionShopWindow.h
 *
 * \author Juver
 * \date 2022/02/24
 *
 * 
 */

#ifndef NPCCONTRIBUTIONSHOPWINDOW_H_INCLUDED__
#define NPCCONTRIBUTIONSHOPWINDOW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Client/G-Logic/GLContributionShop.h"

#include "./UIWindowEx.h"

class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextButton;
class CNPCContributionShopItemSlot;

class CNPCContributionShopWindow : public CUIWindowEx
{
public:
	enum
	{
		NPC_CONTRIBUTION_SHOP_SELECTION_TEXTBOX = ET_CONTROL_NEXT,
		NPC_CONTRIBUTION_SHOP_SELECTION_SCROLLBAR,

		NPC_CONTRIBUTION_SHOP_PURCHASE_BUTTON,
		NPC_CONTRIBUTION_SHOP_PREV_BUTTON,
		NPC_CONTRIBUTION_SHOP_NEXT_BUTTON,
		NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON,

		NPC_CONTRIBUTION_SHOP_ITEM00,
		NPC_CONTRIBUTION_SHOP_ITEM01,
		NPC_CONTRIBUTION_SHOP_ITEM02,
		NPC_CONTRIBUTION_SHOP_ITEM03,
		NPC_CONTRIBUTION_SHOP_ITEM04,
		NPC_CONTRIBUTION_SHOP_ITEM05,
		NPC_CONTRIBUTION_SHOP_ITEM06,
		NPC_CONTRIBUTION_SHOP_ITEM07,
		NPC_CONTRIBUTION_SHOP_ITEM08,
		NPC_CONTRIBUTION_SHOP_ITEM09,
		NPC_CONTRIBUTION_SHOP_ITEM10,
		NPC_CONTRIBUTION_SHOP_ITEM11,
		NPC_CONTRIBUTION_SHOP_ITEM12,
		NPC_CONTRIBUTION_SHOP_ITEM13,
		NPC_CONTRIBUTION_SHOP_ITEM14,
		NPC_CONTRIBUTION_SHOP_ITEM15,
		NPC_CONTRIBUTION_SHOP_ITEM16,
		NPC_CONTRIBUTION_SHOP_ITEM17,
		NPC_CONTRIBUTION_SHOP_ITEM18,
		NPC_CONTRIBUTION_SHOP_ITEM19,
	};

	enum
	{
		NPC_CONTRIBUTION_SHOP_MAX_ITEM = 20,
	};

public:
	CNPCContributionShopWindow ();
	virtual	~CNPCContributionShopWindow ();

public:
	void	CreateSubControl ();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	CBasicTextBoxEx*	m_pListTextBox;
	CBasicScrollBarEx*	m_pListScrollBar;
	CBasicTextBox*		m_pTextPage;
	CBasicTextBox*		m_pTextStaticContribution;
	CBasicTextBox*		m_pTextCharContribution;
	CBasicTextBox*		m_pTextStaticMoney;
	CBasicTextBox*		m_pTextCharMoney;
	CBasicTextButton*	m_pButtonPurchase;
	CBasicButton*		m_pButtonPrev;
	CBasicButton*		m_pButtonNext;
	CBasicButton*		m_pBuyConfirm;
	CBasicTextBox*		m_pBuyConfirmText;
	CNPCContributionShopItemSlot*	m_pItemSlot[NPC_CONTRIBUTION_SHOP_MAX_ITEM];

private:
	int	m_nSelectIndexType;
	int	m_nCurrentPage;
	SNATIVEID	m_sSelectItem;
	SNATIVEID	m_sCrow;
	int m_nMaxPage;

private:
	SNPC_CONTRIBUTION_SHOP_ITEM_VEC	m_vecItem;

public:
	void InitShop( SNATIVEID sidCrow );
	void ResetShop();
	void SelectType( DWORD dwType );
	void ShowItem( int nPage );
	void SelectItem( int nSelect );
};

#endif // NPCCONTRIBUTIONSHOPWINDOW_H_INCLUDED__