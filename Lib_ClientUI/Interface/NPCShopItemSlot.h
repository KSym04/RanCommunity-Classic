

#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-logic/GLdefine.h"

#include "../../Lib_Client/G-Logic/GLNPCShop.h"

class CBasicLineBox;
class CBasicTextBox;
class CItemImage;

const DWORD UIMSG_NPC_SHOP_SLOT_ITEM_IMAGE = UIMSG_USER1;

class	CNPCShopItemSlot : public CUIGroup
{
	enum
	{
		NPC_SHOP_SLOT_ITEM_IMAGE = NO_ID + 1,
	};


public:
	CNPCShopItemSlot ();
	virtual	~CNPCShopItemSlot ();

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
	SNPC_SHOP_ITEM  m_sNPCShopItem;

public:
	void	ItemSet( const SNPC_SHOP_ITEM* pShopItem );
	void	ItemReset();
	void	ItemSelect( BOOL bSelect );
};