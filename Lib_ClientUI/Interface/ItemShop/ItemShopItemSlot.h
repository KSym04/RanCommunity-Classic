

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Engine/G-logic/GLdefine.h"
#include "../Lib_Client/G-Logic/GLCharData.h"

class CBasicLineBox;
class CBasicTextBox;
class CItemImage;

const DWORD UIMSG_ITEM_SHOP_SLOT_ITEM_IMAGE = UIMSG_USER1;

class	CItemShopItemSlot : public CUIGroup
{
	enum
	{
		ITEM_SHOP_SLOT_ITEM_IMAGE = NO_ID + 1,
	};


public:
	CItemShopItemSlot ();
	virtual	~CItemShopItemSlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CUIControl*		m_pSelectImage;
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextPrice;
	CBasicTextBox*	m_pTextStock;
	CItemImage*		m_pItemImage;

private:
	ITEMSHOP  m_sItemShopItem;

public:
	void	ItemSet( ITEMSHOP sShopItem );
	void	ItemReset();
	void	ItemSelect( BOOL bSelect );
	ITEMSHOP	ItemGet()	{	return m_sItemShopItem;	}

};