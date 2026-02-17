

#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-logic/GLdefine.h"

#include "../../Lib_Client/G-Logic/GLItemExchange.h"

class CBasicLineBox;
class CItemImage;
class CBasicTextBox;
class CBasicTextButton;

class	CNPCItemExchangeSlot : public CUIGroup
{
	enum
	{
		NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_RES = NO_ID + 1,
		NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_0,
		NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_1,
		NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_2,
		NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_3,
		NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_4,
		NPC_ITEM_EXCHANGE_SLOT_BUTTON,
		NPCEX_INVENTORY_MONEY_BUTTON,
		NPCEX_INVENTORY_CONTRI_BUTTON,
	};


public:
	CNPCItemExchangeSlot ();
	virtual	~CNPCItemExchangeSlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CBasicTextBox*	m_pTextName;
	CBasicTextBox*	m_pTextMoneyContri; /* NpcExchange Contri Requirement | Jxyy | 11-21-24*/

	CBasicTextBox*	m_pTextExReq;
	CItemImage*		m_pItemImageResult;
	CItemImage*		m_pItemImageRequire[NPC_ITEM_EXCHANGE_MAX_REQ];
	CBasicTextButton*	m_pButton;

	CBasicTextBox*	m_pTextMoney; /* NpcExchange Gold Requirement | JX | 01-11-24*/

private:
	SNATIVEID					m_sNPCID;
	SNPC_ITEM_EXCHANGE_DATA		m_sDATA;

public:
	void	DataReset();
	void	DataSet( SNATIVEID snpc_id, SNPC_ITEM_EXCHANGE_DATA sdata );
	void	DoExchange();
};