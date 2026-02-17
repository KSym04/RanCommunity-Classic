#include "stdafx.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/GUInterface/UITextUtil.h"

#include "../../Lib_Client/G-Logic/GLItemDef.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLGameItemShopClient.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"
#include "./BasicTextButton.h"

#include "./GameItemShopPageItemSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGameItemShopPageItemSlot::CGameItemShopPageItemSlot ()
	: m_pSelectImage(NULL)
	, m_pTextName(NULL)
	, m_pTextPrice(NULL)
	, m_pTextStock(NULL)
	, m_pItemImage(NULL)
	, m_pBuyButton(NULL)
	, m_sData( SGAME_ITEM_SHOP_DATA() )
	, m_fUpdateTime(0.0f)
{
}

CGameItemShopPageItemSlot::~CGameItemShopPageItemSlot ()
{
}

void CGameItemShopPageItemSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "GAME_ITEM_SHOP_ITEM_LIST_NODE_BACK" );
	RegisterControl ( pBasicLineBox );

	m_pSelectImage = new CUIControl;
	m_pSelectImage->CreateSub ( this, "GAME_ITEM_SHOP_ITEM_LIST_NODE_SELECT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pSelectImage->SetVisibleSingle( FALSE );
	RegisterControl ( m_pSelectImage );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "GAME_ITEM_SHOP_ITEM_LIST_NODE_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextName );

	m_pTextPrice = new CBasicTextBox;
	m_pTextPrice->CreateSub ( this, "GAME_ITEM_SHOP_ITEM_LIST_NODE_PRICE" );
	m_pTextPrice->SetFont ( pFont8 );
	m_pTextPrice->SetTextAlign( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextPrice );

	m_pTextStock = new CBasicTextBox;
	m_pTextStock->CreateSub ( this, "GAME_ITEM_SHOP_ITEM_LIST_NODE_STOCK" );
	m_pTextStock->SetFont ( pFont8 );
	m_pTextStock->SetTextAlign( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextStock );

	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub ( this, "GAME_ITEM_SHOP_ITEM_LIST_NODE_IMAGE", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_SLOT_ITEM_IMAGE );
	m_pItemImage->CreateSubControl ();
	RegisterControl ( m_pItemImage );

	m_pBuyButton = new CBasicTextButton;
	m_pBuyButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, GAME_ITEM_SHOP_SLOT_BUTTON );
	m_pBuyButton->CreateBaseButton ( "GAME_ITEM_SHOP_ITEM_LIST_NODE_BUY_BUTTON", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("GAME_ITEM_SHOP_WINDOW", 1) );
	m_pBuyButton->SetVisibleSingle( FALSE );
	RegisterControl ( m_pBuyButton );
}

void CGameItemShopPageItemSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsVisible() )
	{
		m_fUpdateTime += fElapsedTime;

		//updates only every .5 seconds
		if ( m_fUpdateTime >= 0.5f )
		{
			const SGAME_ITEM_SHOP_DATA* pShopItem = GLGameItemShopClient::GetInstance().GetItem( m_sData.wIndex );
			if ( pShopItem )
			{
				//we only needed to update stock count for now
				//later we also needed to update price when discount system is added
				if ( m_pTextStock )
				{
					m_pTextStock->ClearText();

					int nIndex = m_pTextStock->AddText( ID2GAMEWORD( "GAME_ITEM_SHOP_WINDOW", 2 ), NS_UITEXTCOLOR::WHITE );
					m_pTextStock->AddString( nIndex, " : ", NS_UITEXTCOLOR::WHITE );
					m_pTextStock->AddString( nIndex, NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->wStock, 3, "," ), NS_UITEXTCOLOR::WHITE );
				}
			}


			m_fUpdateTime = 0.0f;
		}
		
		

	}
	
}


void CGameItemShopPageItemSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case GAME_ITEM_SHOP_SLOT_ITEM_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_GAME_ITEM_SHOP_SLOT_ITEM_IMAGE );
			}
		}break;

	case GAME_ITEM_SHOP_SLOT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_GAME_ITEM_SHOP_SLOT_BUTTON );

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					GLGameItemShopClient::GetInstance().RequestPurchase( m_sData.wIndex );
				}
			}
		}break;
	};
}

void CGameItemShopPageItemSlot::ItemSet( const SGAME_ITEM_SHOP_DATA* pShopItem )
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
			int nIndex = m_pTextPrice->AddText( ID2GAMEWORD( "GAME_ITEM_SHOP_TYPES", pShopItem->wType ), NS_UITEXTCOLOR::BRIGHTGREEN );
			m_pTextPrice->AddString( nIndex, " : ", NS_UITEXTCOLOR::WHITE );
			m_pTextPrice->AddString( nIndex, NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->wPrice, 3, "," ), NS_UITEXTCOLOR::YELLOW );
		}

		if ( m_pTextStock )		
		{
			int nIndex = m_pTextStock->AddText( ID2GAMEWORD( "GAME_ITEM_SHOP_WINDOW", 2 ), NS_UITEXTCOLOR::WHITE );
			m_pTextStock->AddString( nIndex, " : ", NS_UITEXTCOLOR::WHITE );
			m_pTextStock->AddString( nIndex, NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->wStock, 3, "," ), NS_UITEXTCOLOR::WHITE );
		}


		if ( m_pItemImage )		
		{
			m_pItemImage->SetItem( pITEM->sBasicOp.sICONID, pITEM->GetInventoryFile() );
		}

		m_sData = *pShopItem;
	}
}

void CGameItemShopPageItemSlot::ItemReset()
{
	if ( m_pTextName )	m_pTextName->ClearText();
	if ( m_pTextPrice )	m_pTextPrice->ClearText();
	if ( m_pTextStock )	m_pTextStock->ClearText();
	if ( m_pItemImage )	m_pItemImage->ResetItem();
	if ( m_pSelectImage )	m_pSelectImage->SetVisibleSingle( FALSE );
	if ( m_pBuyButton )		m_pBuyButton->SetVisibleSingle( FALSE );

	m_sData = SGAME_ITEM_SHOP_DATA();
}

void CGameItemShopPageItemSlot::ItemSelect( BOOL bSelect )
{
	if ( m_pSelectImage )
		m_pSelectImage->SetVisibleSingle( bSelect );

	if ( m_pBuyButton )		
		m_pBuyButton->SetVisibleSingle( bSelect );
}