
/*!
 * \file NPCContributionShopItemSlot.h
 *
 * \author Juver
 * \date 2022/02/24
 *
 * 
 */

#ifndef NPCCONTRIBUTIONSHOPITEMSLOT_H_INCLUDED__
#define NPCCONTRIBUTIONSHOPITEMSLOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-logic/GLDefine.h"

#include "../../Lib_Client/G-Logic/GLContributionShop.h"

class CBasicLineBox;
class CBasicTextBox;
class CItemImage;

const DWORD UIMSG_NPC_CONTRIBUTION_SHOP_SLOT_ITEM_IMAGE = UIMSG_USER1;

class	CNPCContributionShopItemSlot : public CUIGroup
{
	enum
	{
		NPC_CONTRIBUTION_SHOP_SLOT_ITEM_IMAGE = NO_ID + 1,
	};


public:
	CNPCContributionShopItemSlot ();
	virtual	~CNPCContributionShopItemSlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CUIControl*		m_pSelectImage;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextPrice;
	CItemImage*		m_pItemImage;

public:
	SNPC_CONTRIBUTION_SHOP_ITEM  m_sNPCShopItem;

public:
	void	ItemSet( const SNPC_CONTRIBUTION_SHOP_ITEM* pShopItem );
	void	ItemReset();
	void	ItemSelect( BOOL bSelect );
};

#endif // NPCCONTRIBUTIONSHOPITEMSLOT_H_INCLUDED__