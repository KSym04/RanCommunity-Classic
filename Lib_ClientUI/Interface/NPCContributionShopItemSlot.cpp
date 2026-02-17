#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLItem.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./NPCContributionShopItemSlot.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNPCContributionShopItemSlot::CNPCContributionShopItemSlot ()
	: m_pSelectImage(NULL)
	, m_pTextName(NULL)
	, m_pTextPrice(NULL)
	, m_pItemImage(NULL)
	, m_sNPCShopItem( SNPC_CONTRIBUTION_SHOP_ITEM() )
{
}

CNPCContributionShopItemSlot::~CNPCContributionShopItemSlot ()
{
}

void CNPCContributionShopItemSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_BACK" );
	RegisterControl ( pBasicLineBox );

	m_pSelectImage = new CUIControl;
	m_pSelectImage->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_SELECT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pSelectImage->SetVisibleSingle( FALSE );
	RegisterControl ( m_pSelectImage );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextName );

	m_pTextPrice = new CBasicTextBox;
	m_pTextPrice->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_PRICE" );
	m_pTextPrice->SetFont ( pFont8 );
	m_pTextPrice->SetTextAlign( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextPrice );

	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_IMAGE", UI_FLAG_DEFAULT, NPC_CONTRIBUTION_SHOP_SLOT_ITEM_IMAGE );
	m_pItemImage->CreateSubControl ();
	RegisterControl ( m_pItemImage );

}

void CNPCContributionShopItemSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case NPC_CONTRIBUTION_SHOP_SLOT_ITEM_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_NPC_CONTRIBUTION_SHOP_SLOT_ITEM_IMAGE );
			}
		}break;
	};
}

void CNPCContributionShopItemSlot::ItemSet( const SNPC_CONTRIBUTION_SHOP_ITEM* pShopItem )
{
	ItemReset();

	if ( !pShopItem )	return;

	SITEM* pITEM = GLItemMan::GetInstance().GetItem( pShopItem->sItemID );
	if ( pITEM )
	{
		if ( m_pTextName )		
		{
			m_pTextName->SetText( pITEM->GetName(), COMMENT::ITEMCOLOR[pITEM->sBasicOp.emLevel] );
		}

		if ( m_pTextPrice )		
		{
			CString strTemp;
			strTemp.Format( "C: %s", NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->dwContriPrice, 3, "," ) );
			int nIndex = m_pTextPrice->AddText( strTemp.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
			m_pTextPrice->AddString( nIndex, " / ", NS_UITEXTCOLOR::WHITE );
			strTemp.Format( "G: %s", NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->dwGoldPrice, 3, "," ) );
			m_pTextPrice->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::GOLD );
		}

		if ( m_pItemImage )		
		{
			m_pItemImage->SetItem( pITEM->sBasicOp.sICONID, pITEM->GetInventoryFile() );
		}

		m_sNPCShopItem = *pShopItem;
	}
}

void CNPCContributionShopItemSlot::ItemReset()
{
	if ( m_pTextName )	m_pTextName->ClearText();
	if ( m_pTextPrice )	m_pTextPrice->ClearText();
	if ( m_pItemImage )	m_pItemImage->ResetItem();
	if ( m_pSelectImage )	m_pSelectImage->SetVisibleSingle( FALSE );
	m_sNPCShopItem = SNPC_CONTRIBUTION_SHOP_ITEM();
}

void CNPCContributionShopItemSlot::ItemSelect( BOOL bSelect )
{
	if ( m_pSelectImage )
		m_pSelectImage->SetVisibleSingle( bSelect );
}