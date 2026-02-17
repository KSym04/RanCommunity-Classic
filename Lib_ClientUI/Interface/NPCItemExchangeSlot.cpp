#include "StdAfx.h"
#include "./NPCItemExchangeSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"
#include "./BasicTextButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLItem.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

/* NpcExchange Gold Requirement | JX | 01-11-24*/
#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNPCItemExchangeSlot::CNPCItemExchangeSlot ()
	: m_pTextName(NULL)
	, m_pTextMoney(NULL)
	, m_pItemImageResult(NULL)
	, m_sNPCID( NATIVEID_NULL() )
	, m_sDATA( SNPC_ITEM_EXCHANGE_DATA() )
	, m_pButton(NULL)
{
	for ( int i=0; i<NPC_ITEM_EXCHANGE_MAX_REQ; ++i )
		m_pItemImageRequire[i] = NULL;
}

CNPCItemExchangeSlot::~CNPCItemExchangeSlot ()
{
}

void CNPCItemExchangeSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "NPC_ITEM_EXCHANGE_NODE_BACK" );
	RegisterControl ( pBasicLineBox );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "NPC_ITEM_EXCHANGE_NODE_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextName );

	/* NpcExchange Gold Requirement | JX | 01-11-24*/
	m_pTextMoney = new CBasicTextBox;
	m_pTextMoney->CreateSub ( this, "NPC_ITEM_EXCHANGE_NODE_MONEY" );
	m_pTextMoney->SetFont ( pFont8 );
	m_pTextMoney->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextMoney->SetText( "--" );
	RegisterControl ( m_pTextMoney );
	/* NpcExchange Contri Requirement | Jxyy | 11-21-24*/
	m_pTextMoneyContri = new CBasicTextBox;
	m_pTextMoneyContri->CreateSub ( this, "NPC_ITEM_EXCHANGE_NODE_CONTRI" );
	m_pTextMoneyContri->SetFont ( pFont8 );
	m_pTextMoneyContri->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextMoneyContri->SetText( "--" );
	RegisterControl ( m_pTextMoneyContri );
	m_pTextExReq = new CBasicTextBox;
	m_pTextExReq->CreateSub ( this, "NPC_ITEM_EXCHANGE_NODE_REQTEXT" );
	m_pTextExReq->SetFont ( pFont8 );
	m_pTextExReq->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextExReq->SetText( "--" );
	RegisterControl ( m_pTextExReq );
	std::string stritem_req[NPC_ITEM_EXCHANGE_MAX_REQ] = 
	{
		"NPC_ITEM_EXCHANGE_NODE_ITEM_IMAGE_REQ_0",
		"NPC_ITEM_EXCHANGE_NODE_ITEM_IMAGE_REQ_1",
		"NPC_ITEM_EXCHANGE_NODE_ITEM_IMAGE_REQ_2",
		"NPC_ITEM_EXCHANGE_NODE_ITEM_IMAGE_REQ_3",
		"NPC_ITEM_EXCHANGE_NODE_ITEM_IMAGE_REQ_4",
	};

	for( int i=0; i<NPC_ITEM_EXCHANGE_MAX_REQ; ++i )
	{
		m_pItemImageRequire[i] = new CItemImage;
		m_pItemImageRequire[i]->CreateSub ( this, stritem_req[i].c_str(), UI_FLAG_DEFAULT, NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_0 + i );
		m_pItemImageRequire[i]->CreateSubControl ();
		m_pItemImageRequire[i]->CreateTextBoxDownRight();
		RegisterControl( m_pItemImageRequire[i] );
	}

	m_pItemImageResult = new CItemImage;
	m_pItemImageResult->CreateSub ( this, "NPC_ITEM_EXCHANGE_NODE_ITEM_IMAGE_RES", UI_FLAG_DEFAULT, NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_RES );
	m_pItemImageResult->CreateSubControl ();
	m_pItemImageResult->CreateTextBoxDownRight();
	RegisterControl ( m_pItemImageResult );

	m_pButton = new CBasicTextButton;
	m_pButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, NPC_ITEM_EXCHANGE_SLOT_BUTTON );
	m_pButton->CreateBaseButton ( "NPC_ITEM_EXCHANGE_NODE_BUTTON", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("NPC_ITEM_EXCHANGE_TEXT", 3) );
	RegisterControl ( m_pButton );
	CBasicButton* pButton = new CBasicButton; // Gold Icon
	pButton->CreateSub ( this, "NPCEX_INVENTORY_MONEY_BUTTON", UI_FLAG_DEFAULT, NPCEX_INVENTORY_MONEY_BUTTON );
	pButton->CreateFlip ( "NPCEX_INVENTORY_MONEY_BUTTON_F", CBasicButton::CLICK_FLIP );
	pButton->SetControlNameEx ( "NPCEX_INVENTORY_MONEY_BUTTON" );
	RegisterControl ( pButton );

	CBasicButton* pButtonContri = new CBasicButton; // Contri Icon
	pButtonContri->CreateSub ( this, "NPCEX_INVENTORY_CONTRI_BUTTON", UI_FLAG_DEFAULT, NPCEX_INVENTORY_CONTRI_BUTTON );
	pButtonContri->CreateFlip ( "NPCEX_INVENTORY_CONTRI_BUTTON_F", CBasicButton::CLICK_FLIP );
	pButtonContri->SetControlNameEx ( "NPCEX_INVENTORY_CONTRI_BUTTON" );
	RegisterControl ( pButtonContri );
}

void CNPCItemExchangeSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_RES:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sDATA.sidItemResult.sidItem;
				sitemcustom.wTurnNum = m_sDATA.sidItemResult.wItemNum;

				sitemcustom.cDAMAGE = m_sDATA.cDAMAGE;
				sitemcustom.cDEFENSE = m_sDATA.cDEFENSE;
				sitemcustom.cRESIST_FIRE = m_sDATA.cRESIST_FIRE;
				sitemcustom.cRESIST_ICE = m_sDATA.cRESIST_ICE;
				sitemcustom.cRESIST_ELEC = m_sDATA.cRESIST_ELEC;
				sitemcustom.cRESIST_POISON = m_sDATA.cRESIST_POISON;
				sitemcustom.cRESIST_SPIRIT = m_sDATA.cRESIST_SPIRIT;

				if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

				SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
				if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM() * m_sDATA.sidItemResult.wItemNum;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0 );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sitemcustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						return;
					}
				}

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
						return;
					}
				}
			}
		}break;
	case NPCEX_INVENTORY_MONEY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine ( ID2GAMEWORD("INVENTORY_BUTTON_TEXT",4), NS_UITEXTCOLOR::WHITE );
			}
		}
		break;

	case NPCEX_INVENTORY_CONTRI_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine ( ID2GAMEWORD("INVENTORY_BUTTON_TEXT",5), NS_UITEXTCOLOR::WHITE );
			}
		}
		break;
	case NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_0:
	case NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_1:
	case NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_2:
	case NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_3:
	case NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_4:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - NPC_ITEM_EXCHANGE_SLOT_ITEM_IMAGE_REQ_0;

				if ( nSelect >= NPC_ITEM_EXCHANGE_MAX_REQ )	return;

				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sDATA.sidItemRequire[nSelect].sidItem;
				sitemcustom.wTurnNum = m_sDATA.sidItemRequire[nSelect].wItemNum;

				if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0 );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sitemcustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						return;
					}
				}

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
						return;
					}
				}
			}
		}break;

	case NPC_ITEM_EXCHANGE_SLOT_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				DoExchange();
			}
		}break;
	};
}

void CNPCItemExchangeSlot::DataSet( SNATIVEID snpc_id, SNPC_ITEM_EXCHANGE_DATA sdata )
{
	DataReset();

	m_sNPCID = snpc_id;
	m_sDATA = sdata;

	for ( int i=0; i<NPC_ITEM_EXCHANGE_MAX_REQ; ++i )
	{
		SNPC_ITEM_EXCHANGE_ITEM& sitem_exchange_data_req = m_sDATA.sidItemRequire[i];
		SITEM* pitem_data_req = GLItemMan::GetInstance().GetItem( sitem_exchange_data_req.sidItem );
		if ( pitem_data_req )
		{
			if ( m_pItemImageRequire[i] )
			{
				m_pItemImageRequire[i]->SetItem( pitem_data_req->sBasicOp.sICONID, pitem_data_req->GetInventoryFile() );

				if ( pitem_data_req->ISPILE() )
				{
					CString strNum;
					strNum.Format( "%d", sitem_exchange_data_req.wItemNum );
					m_pItemImageRequire[i]->SetTextDownRight( strNum.GetString(), NS_UITEXTCOLOR::GOLD );
				}
			}
		}	
	}

	SNPC_ITEM_EXCHANGE_ITEM& sitem_exchange_data_result = m_sDATA.sidItemResult;
	SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( sitem_exchange_data_result.sidItem );
	if ( pitem_data_result )
	{
		if ( m_pTextName )		
			m_pTextName->SetText( pitem_data_result->GetName(), COMMENT::ITEMCOLOR[pitem_data_result->sBasicOp.emLevel] );

		/* NpcExchange Gold Requirement | Johenez Tanga| 01-11-24*/
		if ( m_pTextMoney )		
		{
			CString strPrice = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pitem_data_result->sBasicOp.dwBuyPrice, 3, "," );
			CString strCombine;
			strCombine.Format( ID2GAMEWORD( "NPC_ITEM_EXCHANGE_TEXT_REQUIRED", 0 ), strPrice.GetString() );	
			m_pTextMoney->SetText( strCombine.GetString(), NS_UITEXTCOLOR::GOLD );
		}
		/* NpcExchange Contri Requirement | Jxyy | 11-21-24*/
		if ( m_pTextMoneyContri )		
		{
			CString strPrice = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pitem_data_result->sBasicOp.dwReqContributionPoint, 3, "," );
			CString strCombine;
			strCombine.Format( ID2GAMEWORD( "NPC_ITEM_EXCHANGE_TEXT_REQUIRED_CONTRI", 0 ), strPrice.GetString() );	
			m_pTextMoneyContri->SetText( strCombine.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
		}

		if (m_pTextExReq)
		{
			// Directly use the string "Exchange Requirements: "
			CString strExchangeRequirements = ID2GAMEWORD("EXCHANGE_REQUIREMENTS_LABEL", 0);  // This will get the localized string, if available.

			// Set the text of m_pTextExReq without the price or any extra formatting
			m_pTextExReq->SetText(strExchangeRequirements.GetString(), NS_UITEXTCOLOR::ENABLE);
		}
		if ( m_pItemImageResult )
		{
			m_pItemImageResult->SetItem( pitem_data_result->sBasicOp.sICONID, pitem_data_result->GetInventoryFile() );

			if ( pitem_data_result->ISPILE() )
			{
				CString strNum;
				strNum.Format( "%d", pitem_data_result->GETAPPLYNUM() * sitem_exchange_data_result.wItemNum );
				m_pItemImageResult->SetTextDownRight( strNum.GetString(), NS_UITEXTCOLOR::GOLD );
			}
		}
	}	
}

void CNPCItemExchangeSlot::DataReset()
{
	for ( int i=0; i<NPC_ITEM_EXCHANGE_MAX_REQ; ++i )
	{
		if ( m_pItemImageRequire[i] )
		{
			m_pItemImageRequire[i]->ResetItem();
			m_pItemImageRequire[i]->SetTextDownRight( "", NS_UITEXTCOLOR::GOLD );
		}
	}

	if ( m_pItemImageResult )	
	{
		m_pItemImageResult->ResetItem();
		m_pItemImageResult->SetTextDownRight( "", NS_UITEXTCOLOR::GOLD );
	}

	if ( m_pTextName )	
		m_pTextName->ClearText();

	/* NpcExchange Gold Requirement | Johenez Tanga| 01-11-24*/
	if ( m_pTextMoney )		m_pTextMoney->ClearText();
	/* NpcExchange Contri Requirement | Jxyy | 11-21-24*/
	if ( m_pTextMoneyContri )		m_pTextMoneyContri->ClearText();
}

void CNPCItemExchangeSlot::DoExchange()
{
	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pcharacter )	return;

	pcharacter->ReqNPCItemExchange( m_sNPCID, m_sDATA.wID );
}