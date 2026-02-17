#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLExchangeItem.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./ItemImage.h"
#include "./ItemMove.h"
#include "./BasicLineBox.h"
#include "./BasicLineBoxEx.h"
#include "./BasicTextButton.h"
#include "./BasicTextBox.h"
#include "./BasicTextBoxEx.h"
#include "./BasicScrollBar.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"

#include "./ExchangeItemWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CExchangeItemWindow::CExchangeItemWindow ()
	: m_pItemImage_In( NULL )
	, m_pItemImage_Out( NULL )
	, m_pButtonOk ( NULL )
	, m_pButtonCancel ( NULL )
	, m_pListText(NULL)
	, m_pListScrollBar(NULL)
	, m_pTextBefore( NULL )
	, m_pTextAfter( NULL )
	, m_nX(0)
	, m_nY(0)
	, m_nSelectIndex ( -1 )
	, m_pTextCIP( NULL )
{
	m_sIDOut = NATIVEID_NULL();
}

CExchangeItemWindow::~CExchangeItemWindow ()
{
}

void CExchangeItemWindow::CreateSubControl ()
{
	CD3DFontPar * pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8 ,D3DFONT_SHADOW | D3DFONT_ASCII);

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "EXCHANGE_ITEM_WINDOW_BACKGROUND" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "EXCHANGE_ITEM_WINDOW_ITEM_LIST_TEXTBOX" );
	RegisterControl ( pBasicLineBox );	

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "EXCHANGE_ITEM_WINDOW_BEFORE_BACKGROUND" );
	RegisterControl ( pBasicLineBox );	

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "EXCHANGE_ITEM_WINDOW_AFTER_BACKGROUND" );
	RegisterControl ( pBasicLineBox );	

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "EXCHANGE_ITEM_WINDOW_CIP" );
	RegisterControl ( pBasicLineBox );	

	m_pButtonOk = new CBasicTextButton;
	m_pButtonOk->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, BUTTON_OK );
	m_pButtonOk->CreateBaseButton ( "EXCHANGE_ITEM_WINDOW_BUTTON_OK", 
		CBasicTextButton::SIZE19, 
		CBasicButton::CLICK_FLIP, 
		ID2GAMEWORD ( "EXCHANGE_ITEM_WINDOW", 1 ),
		_DEFAULT_FONT_SHADOW_EX_FLAG );
	m_pButtonOk->SetFlip ( TRUE );
	RegisterControl ( m_pButtonOk );

	m_pButtonCancel = new CBasicTextButton;
	m_pButtonCancel->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, BUTTON_CANCEL );
	m_pButtonCancel->CreateBaseButton ( "EXCHANGE_ITEM_WINDOW_BUTTON_CANCEL", 
		CBasicTextButton::SIZE19, 
		CBasicButton::CLICK_FLIP, 
		ID2GAMEWORD ( "EXCHANGE_ITEM_WINDOW", 2 ),
		_DEFAULT_FONT_SHADOW_EX_FLAG );
	m_pButtonCancel->SetFlip ( TRUE );
	RegisterControl ( m_pButtonCancel );

	CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
	pTextBox->CreateSub ( this, "EXCHANGE_ITEM_WINDOW_ITEM_LIST_TEXTBOX", UI_FLAG_DEFAULT, ITEM_LIST_TEXTBOX );
	pTextBox->SetFont ( pFont );		
	pTextBox->SetLineInterval ( 3.0f );
	pTextBox->SetSensitive ( true );
	pTextBox->SetLimitLine ( 10000 );
	RegisterControl ( pTextBox );
	m_pListText = pTextBox;

	int nTotalLine = pTextBox->GetVisibleLine ();

	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, ITEM_LIST_SCROLLBAR );
	pScrollBar->CreateBaseScrollBar ( "EXCHANGE_ITEM_WINDOW_ITEM_LIST_SCROLLBAR" );
	pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( pScrollBar );
	m_pListScrollBar = pScrollBar;

	m_pItemImage_In = new CItemImage;
	m_pItemImage_In->CreateSub( this, "EXCHANGE_ITEM_WINDOW_ITEM_SLOT_IN", UI_FLAG_DEFAULT, ITEM_SLOT_IN );
	m_pItemImage_In->CreateSubControl();
	RegisterControl( m_pItemImage_In );

	m_pItemImage_Out = new CItemImage;
	m_pItemImage_Out->CreateSub( this, "EXCHANGE_ITEM_WINDOW_ITEM_SLOT_OUT", UI_FLAG_DEFAULT, ITEM_SLOT_OUT );
	m_pItemImage_Out->CreateSubControl();
	RegisterControl( m_pItemImage_Out );

	m_pTextBefore = new CBasicTextBox;
	m_pTextBefore->CreateSub ( this, "EXCHANGE_ITEM_WINDOW_BEFORE_BACKGROUND" );
	m_pTextBefore->SetFont ( pFont );
	m_pTextBefore->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextBefore );

	m_pTextAfter = new CBasicTextBox;
	m_pTextAfter->CreateSub ( this, "EXCHANGE_ITEM_WINDOW_AFTER_BACKGROUND" );
	m_pTextAfter->SetFont ( pFont );
	m_pTextAfter->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextAfter );

	m_pTextCIP = new CBasicTextBox;
	m_pTextCIP->CreateSub ( this, "EXCHANGE_ITEM_WINDOW_CIP" );
	m_pTextCIP->SetFont ( pFont );
	m_pTextCIP->SetTextAlign ( TEXT_ALIGN_LEFT );	
	RegisterControl ( m_pTextCIP );

}

void CExchangeItemWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::ORANGE );
	}

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}

	SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GetExchangeItemItem();
	if( m_pItemImage_In )
	{
		if( sItemCustom.sNativeID == NATIVEID_NULL() )
		{
			m_pItemImage_In->ResetItem();
		}
		else
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
			if( pItem )
			{
				m_pItemImage_In->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
			}
			else
			{
				m_pItemImage_In->ResetItem();
			}
		}
	}

	if( m_pItemImage_Out )
	{
		if( m_sIDOut == NATIVEID_NULL() )
		{
			m_pItemImage_Out->ResetItem();
		}
		else
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sIDOut );
			if( pItem )
			{
				m_pItemImage_Out->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
			}
			else
			{
				m_pItemImage_Out->ResetItem();
			}
		}
	}

	CString strExchangeItemPoints;
	strExchangeItemPoints.Format( "CIP: %d", GLGaeaClient::GetInstance().GetCharacter()->m_wExchangeItemPoints );
	m_pTextCIP->SetOneLineText( strExchangeItemPoints.GetString() );

	CheckCard();
}

void CExchangeItemWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ITEM_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKORANGE );

				if( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;

					SNATIVEID	sSelectedID; 
					sSelectedID.dwID = m_pListText->GetTextData ( m_nSelectIndex );
					SITEM* pItem = GLItemMan::GetInstance().GetItem( sSelectedID );
					if ( pItem )	
						m_sIDOut = pItem->sBasicOp.sNativeID;	
				}
			}
		}
		break;

	case ITEM_SLOT_IN:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )
				{
					GASSERT( 0 && "CItemGarbage::TranslateUIMessage, pItemMove == NULL" );
					break;
				}

				SNATIVEID sItem = pItemMove->GetItem();
				if( sItem != NATIVEID_NULL() )
				{
					const UIRECT& rcSlotPos = m_pItemImage_In->GetGlobalPos();
					pItemMove->SetGlobalPos( rcSlotPos );
					CInnerInterface::GetInstance().SetSnapItem();

					if( dwMsg & UIMSG_LB_UP )
					{
						if ( CheckValidItem( sItem ) )
							GLGaeaClient::GetInstance().GetCharacter()->ReqExchangeItemMoveItem();
						else
							CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_INVALID_ITEM") );	
					}
				}

				if( dwMsg & UIMSG_RB_UP )
					GLGaeaClient::GetInstance().GetCharacter()->ReqExchangeItemResetItem();

				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GetExchangeItemItem();
				if( sItemCustom.sNativeID != NATIVEID_NULL() )
					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, 0, 0 );
			}
		}
		break;

	case ITEM_SLOT_OUT:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				if ( m_sIDOut != NATIVEID_NULL() )
				{
					SITEMCUSTOM sItemCustom;
					sItemCustom.sNativeID = m_sIDOut;
					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, 0, 0 );

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sItemCustom );
							return;
						}
					}

					/*if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_PRESSED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
							if ( pITEM )
							{
								GLGaeaClient::GetInstance().GetCharacter()->PreviewItem( sItemCustom.sNativeID, sItemCustom.GETGRADE_EFFECT() );
								return;
							}	
						}
					}*/

				}
			}
		}
		break;

	case BUTTON_OK:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( m_sIDOut != NATIVEID_NULL() )
					{
						if ( CheckValidCIP( m_sIDOut ) )
							GLGaeaClient::GetInstance().GetCharacter()->ReqExchangeItem( m_sIDOut, m_nX, m_nY );
						else
							CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_ERROR_NOCIP") );	
					}
				}
			}
		}
		break;

	case BUTTON_CANCEL:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )	
					CInnerInterface::GetInstance().CloseExchangeItemWindow();
			}
		}
		break;

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
				CInnerInterface::GetInstance().CloseExchangeItemWindow();
		}
		break;
	};
}

void CExchangeItemWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		Refresh();
	}
	else
	{
	}
}

void CExchangeItemWindow::CheckCard()
{
	//check if player change card position
	if ( ! IsVisible() )	
		return;

	SINVENITEM* pInvenItem = GLGaeaClient::GetInstance().GetCharacterLogic().m_cInventory.FindPosItem ( m_nX, m_nY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_POS_ERROR") );	
		CInnerInterface::GetInstance().CloseExchangeItemWindow();
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_ITEM_ERROR") );	
		CInnerInterface::GetInstance().CloseExchangeItemWindow();
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_EXCHANGE_ITEM)
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_ITEMTYPE_ERROR") );	
		CInnerInterface::GetInstance().CloseExchangeItemWindow();
		return;
	}
}

void CExchangeItemWindow::SetCard ( int nPosX, int nPosY )
{
	m_nX = nPosX;
	m_nY = nPosY;
}

void CExchangeItemWindow::Refresh()
{
	SINVENITEM* pInvenItem = GLGaeaClient::GetInstance().GetCharacterLogic().m_cInventory.FindPosItem ( m_nX, m_nY );
	if ( pInvenItem )
	{
		SITEM* pItemCard = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
		if ( pItemCard )
		{
			nsexchange_item::Data* pData = nsexchange_item::DataManager::GetInstance().findData( pItemCard->getExchangeItem() );
			if ( pData )
			{
				m_pListText->ClearText();

				nsexchange_item::MAP_RESULT_ITER it_b = pData->mapResult.begin();
				nsexchange_item::MAP_RESULT_ITER it_e = pData->mapResult.end();
				for ( ; it_b != it_e; ++ it_b )
				{
					nsexchange_item::Result& sResult = (*it_b).second;

					SITEM* pItemResult = GLItemMan::GetInstance().GetItem ( sResult.sItemID );
					if ( pItemResult )
					{
						CString strList;
						if ( sResult.wPointCost > 0 )
						{
							CString strCombine;
							CString strName( pItemResult->GetName() );
							int nLength = strName.GetLength();
							if ( nLength < 40 )
							{
								std::string strBlank( 40 - nLength , ' ' ); 		
								strList.Format( "%s%s%s:%d", strName, strBlank.c_str(), "Required CIP", sResult.wPointCost );
							}else{
								strList.Format( "%s%s%s:%d", strName, " ", "Required CIP", sResult.wPointCost );
							}
						}
						else
						{
							strList.Format( "%s", pItemResult->GetName() );	
						}

						int nIndex = m_pListText->AddText ( strList.GetString() );	
						m_pListText->SetTextData ( nIndex, pItemResult->sBasicOp.sNativeID.dwID );
					}

				}
			}
		}
	}	

	m_pTextBefore->ClearText();
	m_pTextAfter->ClearText();

	m_pTextBefore->AddText( ID2GAMEWORD ( "EXCHANGE_ITEM_WINDOW", 3 ) );
	m_pTextAfter->AddText( ID2GAMEWORD ( "EXCHANGE_ITEM_WINDOW", 4 ) );

	m_pItemImage_In->ResetItem();
	m_pItemImage_Out->ResetItem();

	m_sIDOut = NATIVEID_NULL();
	m_nSelectIndex =-1;
}

BOOL CExchangeItemWindow::CheckValidItem( SNATIVEID sID )
{
	SINVENITEM* pInvenItem = GLGaeaClient::GetInstance().GetCharacterLogic().m_cInventory.FindPosItem ( m_nX, m_nY );
	if ( pInvenItem )
	{
		SITEM* pItemCard = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
		if ( pItemCard )
		{
			nsexchange_item::Data* pData = nsexchange_item::DataManager::GetInstance().findData( pItemCard->getExchangeItem() );
			if ( pData )
			{
				nsexchange_item::Require* pRequire = pData->findRequire( sID.dwID );
				if ( pRequire )
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( pRequire->sItemID );
					if ( pItem )	
						return TRUE;
				}
			}
		}
	}	

	return FALSE;
}

BOOL CExchangeItemWindow::CheckValidCIP( SNATIVEID sID )
{
	WORD wExchangeItemPoints = GLGaeaClient::GetInstance().GetCharacter()->m_wExchangeItemPoints;

	SINVENITEM* pInvenItem = GLGaeaClient::GetInstance().GetCharacterLogic().m_cInventory.FindPosItem ( m_nX, m_nY );
	if ( pInvenItem )
	{
		SITEM* pItemCard = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
		if ( pItemCard )
		{
			nsexchange_item::Data* pData = nsexchange_item::DataManager::GetInstance().findData( pItemCard->getExchangeItem() );
			if ( pData )
			{
				nsexchange_item::Result* pRequire = pData->findResult( sID.dwID );
				if ( pRequire )
				{
					if ( wExchangeItemPoints >= pRequire->wPointCost )
						return TRUE;
				}
			}
		}
	}	

	return FALSE;
}
