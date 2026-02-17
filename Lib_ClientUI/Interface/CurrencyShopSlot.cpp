#include "stdafx.h"
#include "./CurrencyShopSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"
#include "./BasicTextButton.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLCrowData.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLCurrencyShop.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCurrencyShopSlot::CCurrencyShopSlot ()
	: m_sNpcID(false)
	, m_sItemID(false)

	, m_pTextName(NULL)
	, m_pItemImageResult(NULL)

	, m_pButtonCostMoney(NULL)
	, m_pButtonCostGamePoints(NULL)
	, m_pButtonCostWarChips(NULL)

	, m_pTextCostMoney(NULL)
	, m_pTextCostGamePoints(NULL)
	, m_pTextCostWarChips(NULL)

	, m_pButtonPurchase(NULL)
{

}

CCurrencyShopSlot::~CCurrencyShopSlot ()
{
}

void CCurrencyShopSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "CURRENCY_SHOP_NODE_BACK" );
	RegisterControl ( pBasicLineBox );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "CURRENCY_SHOP_NODE_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText( "1" );
	RegisterControl ( m_pTextName );

	m_pItemImageResult = new CItemImage;
	m_pItemImageResult->CreateSub ( this, "CURRENCY_SHOP_NODE_ITEM_IMAGE_RES", UI_FLAG_DEFAULT, CURRENCY_SHOP_SLOT_ITEM_IMAGE_RES );
	m_pItemImageResult->CreateSubControl ();
	m_pItemImageResult->CreateTextBoxDownRight();
	RegisterControl ( m_pItemImageResult );


	m_pTextCostMoney = new CBasicTextBox;
	m_pTextCostMoney->CreateSub ( this, "CURRENCY_SHOP_NODE_COST_MONEY_TEXT" );
	m_pTextCostMoney->SetFont ( pFont8 );
	m_pTextCostMoney->SetTextAlign( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextCostMoney->SetText( "2" );
	RegisterControl ( m_pTextCostMoney );

	m_pTextCostGamePoints = new CBasicTextBox;
	m_pTextCostGamePoints->CreateSub ( this, "CURRENCY_SHOP_NODE_COST_GAME_POINTS_TEXT" );
	m_pTextCostGamePoints->SetFont ( pFont8 );
	m_pTextCostGamePoints->SetTextAlign( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextCostGamePoints->SetText( "3" );
	RegisterControl ( m_pTextCostGamePoints );

	m_pTextCostWarChips = new CBasicTextBox;
	m_pTextCostWarChips->CreateSub ( this, "CURRENCY_SHOP_NODE_COST_WAR_CHIPS_TEXT" );
	m_pTextCostWarChips->SetFont ( pFont8 );
	m_pTextCostWarChips->SetTextAlign( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextCostWarChips->SetText( "4" );
	RegisterControl ( m_pTextCostWarChips );

	m_pButtonPurchase = new CBasicTextButton;
	m_pButtonPurchase->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CURRENCY_SHOP_SLOT_BUTTON_PURCHASE );
	m_pButtonPurchase->CreateBaseButton ( "CURRENCY_SHOP_NODE_BUTTON", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("CURRENCY_SHOP_WINDOW", 1) );
	RegisterControl ( m_pButtonPurchase );


	m_pButtonCostMoney = new CBasicButton;
	m_pButtonCostMoney->CreateSub ( this, "CURRENCY_SHOP_NODE_MONEY_BUTTON", UI_FLAG_DEFAULT, CURRENCY_SHOP_SLOT_BUTTON_COST_MONEY );
	m_pButtonCostMoney->CreateFlip ( "CURRENCY_SHOP_NODE_MONEY_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonCostMoney->CreateMouseOver ( "CURRENCY_SHOP_NODE_MONEY_BUTTON_F" );
	m_pButtonCostMoney->SetAlignFlag ( UI_FLAG_BOTTOM );
	m_pButtonCostMoney->SetUseDynamic ( TRUE );
	RegisterControl ( m_pButtonCostMoney );

	m_pButtonCostGamePoints = new CBasicButton;
	m_pButtonCostGamePoints->CreateSub ( this, "CURRENCY_SHOP_NODE_POINT_BUTTON", UI_FLAG_DEFAULT, CURRENCY_SHOP_SLOT_BUTTON_COST_GAME_POINTS );
	m_pButtonCostGamePoints->CreateFlip ( "CURRENCY_SHOP_NODE_POINT_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonCostGamePoints->CreateMouseOver ( "CURRENCY_SHOP_NODE_POINT_BUTTON_F" );
	m_pButtonCostGamePoints->SetAlignFlag ( UI_FLAG_BOTTOM );
	m_pButtonCostGamePoints->SetUseDynamic ( TRUE );
	RegisterControl ( m_pButtonCostGamePoints );

	m_pButtonCostWarChips = new CBasicButton;
	m_pButtonCostWarChips->CreateSub ( this, "CURRENCY_SHOP_WAR_CHIPS_BUTTON", UI_FLAG_DEFAULT, CURRENCY_SHOP_SLOT_BUTTON_COST_WAR_CHIPS );
	m_pButtonCostWarChips->CreateFlip ( "CURRENCY_SHOP_WAR_CHIPS_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonCostWarChips->CreateMouseOver ( "CURRENCY_SHOP_WAR_CHIPS_BUTTON_F" );
	m_pButtonCostWarChips->SetAlignFlag ( UI_FLAG_BOTTOM );
	m_pButtonCostWarChips->SetUseDynamic ( TRUE );
	RegisterControl ( m_pButtonCostWarChips );


}

void CCurrencyShopSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case CURRENCY_SHOP_SLOT_ITEM_IMAGE_RES:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				PCROWDATA pCrow = GLCrowDataMan::GetInstance().GetCrowData( m_sNpcID );
				if ( pCrow )
				{
					SCURRENCY_SHOP_DATA* pShop = GLCrowDataMan::GetInstance().CurrencyShopFind( pCrow->m_sAction.m_strCurrencyShop );
					if ( pShop )
					{
						SCURRENCY_SHOP_ITEM* pShopItem = pShop->FindData( m_sItemID );
						if ( pShopItem )
						{
							ShowItemInfo( pShopItem, dwMsg );
						}
					}
				}
			}
		}break;

	case CURRENCY_SHOP_SLOT_BUTTON_PURCHASE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				DoPurchase();
			}
		}break;


	};
}

void CCurrencyShopSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCurrencyShopSlot::DataSet( SNATIVEID sNpcID, SCURRENCY_SHOP_ITEM* pData )
{
	DataReset();

	if ( pData )
	{
		m_sNpcID = sNpcID;
		m_sItemID = pData->sItemID;

		SITEM* pItemData = GLItemMan::GetInstance().GetItem( m_sItemID );
		if ( pItemData )
		{
			if ( m_pTextName )		
				m_pTextName->SetText( pItemData->GetName(), COMMENT::ITEMCOLOR[pItemData->sBasicOp.emLevel] );

			if ( m_pItemImageResult )
			{
				m_pItemImageResult->SetItem( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );

				if ( pItemData->ISPILE() )
				{
					CString strNum;
					strNum.Format( "%d", pItemData->GETAPPLYNUM() );
					m_pItemImageResult->SetTextDownRight( strNum.GetString(), NS_UITEXTCOLOR::GOLD );
				}
			}

			UpdatePrice();
		}
	}
}

void CCurrencyShopSlot::DataReset()
{
	m_sNpcID = SNATIVEID(false);
	m_sItemID = SNATIVEID(false);

	if ( m_pTextName )	
		m_pTextName->ClearText();

	if ( m_pItemImageResult )	
	{
		m_pItemImageResult->ResetItem();
		m_pItemImageResult->SetTextDownRight( "", NS_UITEXTCOLOR::GOLD );
	}

	if ( m_pButtonCostMoney )
		m_pButtonCostMoney->SetVisibleSingle( FALSE );

	if ( m_pButtonCostGamePoints )
		m_pButtonCostGamePoints->SetVisibleSingle( FALSE );

	if ( m_pButtonCostWarChips )
		m_pButtonCostWarChips->SetVisibleSingle( FALSE );

	if ( m_pTextCostMoney )
		m_pTextCostMoney->ClearText();

	if ( m_pTextCostGamePoints )
		m_pTextCostGamePoints->ClearText();

	if ( m_pTextCostWarChips )
		m_pTextCostWarChips->ClearText();
}

void CCurrencyShopSlot::DoPurchase()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )	return;

	pCharacter->ReqCurrencyShopPurchase( m_sNpcID, m_sItemID );
	
}

void CCurrencyShopSlot::ShowItemInfo( SCURRENCY_SHOP_ITEM* pData, DWORD dwUIMsg )
{
	if ( !pData )	return;

	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = pData->sItemID;
	sItemCustom.wTurnNum = 0;

	sItemCustom.cDAMAGE = pData->cDAMAGE;
	sItemCustom.cDEFENSE = pData->cDEFENSE;
	sItemCustom.cRESIST_FIRE = pData->cRESIST_FIRE;
	sItemCustom.cRESIST_ICE = pData->cRESIST_ICE;
	sItemCustom.cRESIST_ELEC = pData->cRESIST_ELEC;
	sItemCustom.cRESIST_POISON = pData->cRESIST_POISON;
	sItemCustom.cRESIST_SPIRIT = pData->cRESIST_SPIRIT;

	if( sItemCustom.sNativeID == NATIVEID_NULL() ) return;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
	if ( pItemData && pItemData->ISPILE() )	sItemCustom.wTurnNum = pItemData->GETAPPLYNUM();

	CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0, FALSE, FALSE, FALSE, TRUE, m_sNpcID, m_sItemID );

	/*item preview, Juver, 2017/07/27 */
	if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
	{
		if ( dwUIMsg & UIMSG_LB_UP )
		{
			CInnerInterface::GetInstance().PreviewItem( sItemCustom );
			return;
		}
	}

	/*box contents, Juver, 2017/08/30 */
	if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
	{
		if ( dwUIMsg & UIMSG_RB_UP )
		{
			CInnerInterface::GetInstance().ShowBoxContents( sItemCustom.sNativeID );
			return;
		}
	}

	if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
	{
		if ( dwUIMsg & UIMSG_LB_UP )
		{
			CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
			return;
		}
	}
}

void CCurrencyShopSlot::UpdatePrice()
{
	PCROWDATA pCrow = GLCrowDataMan::GetInstance().GetCrowData( m_sNpcID );
	if ( pCrow )
	{
		SCURRENCY_SHOP_DATA* pShop = GLCrowDataMan::GetInstance().CurrencyShopFind( pCrow->m_sAction.m_strCurrencyShop );
		if ( pShop )
		{
			SCURRENCY_SHOP_ITEM* pShopItem = pShop->FindData( m_sItemID );
			if ( pShopItem )
			{
				if ( pShopItem->llPriceMoney )
				{
					if ( m_pButtonCostMoney )
						m_pButtonCostMoney->SetVisibleSingle( TRUE );

					if ( m_pTextCostMoney )
					{
						CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( pShopItem->llPriceMoney, 3, "," );

						D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
						if( pShopItem->llPriceMoney <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
						{
							dwColor = NS_UITEXTCOLOR::WHITE;
						}

						m_pTextCostMoney->ClearText();
						m_pTextCostMoney->SetText( strTemp.GetString(), dwColor );
					}
				}

				if ( pShopItem->dwPriceGamePoints )
				{
					if ( m_pButtonCostGamePoints )
						m_pButtonCostGamePoints->SetVisibleSingle( TRUE );

					if ( m_pTextCostGamePoints )
					{
						CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->dwPriceGamePoints, 3, "," );

						D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
						if( pShopItem->dwPriceGamePoints <= GLGaeaClient::GetInstance().GetCharacterLogic().m_dwGamePoints )
						{
							dwColor = NS_UITEXTCOLOR::WHITE;
						}

						m_pTextCostGamePoints->ClearText();
						m_pTextCostGamePoints->SetText( strTemp.GetString(), dwColor );
					}
				}

				if ( pShopItem->dwPriceWarChips )
				{
					if ( m_pButtonCostWarChips )
						m_pButtonCostWarChips->SetVisibleSingle( TRUE );

					if ( m_pTextCostWarChips )
					{
						CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pShopItem->dwPriceWarChips, 3, "," );

						D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
						if( pShopItem->dwPriceWarChips <= GLGaeaClient::GetInstance().GetCharacterLogic().m_dwWarChips )
						{
							dwColor = NS_UITEXTCOLOR::WHITE;
						}

						m_pTextCostWarChips->ClearText();
						m_pTextCostWarChips->SetText( strTemp.GetString(), dwColor );
					}
				}
			}
		}
	}
}