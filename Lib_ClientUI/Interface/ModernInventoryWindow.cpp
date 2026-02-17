#include "StdAfx.h"
#include "./ModernInventoryWindow.h"
#include "./ModernInventoryWindowInven.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./MultiModeButton.h"
#include "./BasicButton.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./InnerInterface.h"
#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CModernInventoryWindow::CModernInventoryWindow()
	: m_pBackGround(NULL)
	, m_pMoneyTextBox(NULL)
	, m_pHelpButton(NULL)
	, m_pInven(NULL)
	, m_nONE_VIEW_SLOT( 4 )
{
	for( int i=0; i<MODERN_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CModernInventoryWindow::~CModernInventoryWindow()
{
}

CMultiModeButton* CModernInventoryWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "MODERN_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "MODERN_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "MODERN_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::BLACK );
	pControl->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}


CMultiModeButton* CModernInventoryWindow::CreateBlueButton( char* szControl, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, "MODERN_BUTTON_BLUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pControl->CreateImageBaseMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_BASE_IMAGE_LEFT", "MODERN_BUTTON_BLUE_BASE_IMAGE_CENTER", "MODERN_BUTTON_BLUE_BASE_IMAGE_RIGHT" );
	pControl->CreateImageOverMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_OVER_IMAGE_LEFT", "MODERN_BUTTON_BLUE_OVER_IMAGE_CENTER", "MODERN_BUTTON_BLUE_OVER_IMAGE_RIGHT" );
	pControl->CreateImageHoldMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_CLICK_IMAGE_LEFT", "MODERN_BUTTON_BLUE_CLICK_IMAGE_CENTER", "MODERN_BUTTON_BLUE_CLICK_IMAGE_RIGHT" );
	pControl->AlignToControl( szControl );
	RegisterControl ( pControl );

	return pControl;
}

void CModernInventoryWindow::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "MODERN_INVENTORY_WINDOW_REGION" );
	RegisterControl ( pBasicLineBox );

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, "MODERN_INVENTORY_WINDOW_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );
	pUIControl->AlignToControl( pBasicLineBox, TRUE );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_INVENTORY_WINDOW_ITEM_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_INVENTORY_WINDOW_MONEY_REGION" );
	RegisterControl ( pBasicLineBox );

	std::string strRadio[MODERN_INVENTORY_PAGE_SIZE] = 
	{
		"MODERN_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"MODERN_INVENTORY_WINDOW_RADIO_BUTTON_2",
		"MODERN_INVENTORY_WINDOW_RADIO_BUTTON_3",
	};
	
	for ( int i=0; i<MODERN_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), MODERN_INVENTORY_RADIO_BUTTON_0 +i );
	}
	
	m_pHelpButton = new CBasicButton;
	m_pHelpButton->CreateSub ( this, "MODERN_INVENTORY_WINDOW_HELP_BUTTON" , UI_FLAG_DEFAULT, MODERN_INVENTORY_HELP_BUTTON );
	m_pHelpButton->CreateFlip ( "MODERN_INVENTORY_WINDOW_HELP_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pHelpButton->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelpButton );

	m_pInven = new CModernInventoryWindowInven;
	m_pInven->CreateSub ( this, "MODERN_INVENTORY_WINDOW_ITEM_REGION", UI_FLAG_XSIZE | UI_FLAG_YSIZE, MODERN_INVENTORY_PAGE_INVEN );
	m_pInven->CreateSubControl ();
	RegisterControl ( m_pInven );

	CBasicButton* pMoneyButton = NULL;
	pMoneyButton = CreateFlipButton ( "MODERN_INVENTORY_MONEY_BUTTON_RENEW", "MODERN_INVENTORY_MONEY_BUTTON_RENEW_F", MODERN_INVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
	pMoneyButton->CreateMouseOver ( "MODERN_INVENTORY_MONEY_BUTTON_RENEW_F" );
	pMoneyButton->SetAlignFlag ( UI_FLAG_BOTTOM );
	pMoneyButton->SetUseDynamic ( TRUE );

	m_pMoneyTextBox = CreateStaticControl ( "MODERN_INVENTORY_MONEY_TEXT_RENEW", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
	m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

	CreateBlueButton( "MODERN_INVENTORY_WINDOW_LOCKER_BUTTON", MODERN_INVENTORY_LOCKER_BUTTON );
	CreateBlueButton( "MODERN_INVENTORY_WINDOW_EXTEND_BUTTON", MODERN_INVENTORY_EXTEND_BUTTON );
	CreateBlueButton( "MODERN_INVENTORY_WINDOW_TRASH_BUTTON", MODERN_INVENTORY_TRASH_BUTTON );
	CreateBlueButton( "MODERN_INVENTORY_WINDOW_SORT_BUTTON", MODERN_INVENTORY_SORT_BUTTON );

	CreateControl ( "MODERN_INVENTORY_WINDOW_LOCKER_IMAGE" );
	CreateControl ( "MODERN_INVENTORY_WINDOW_EXTEND_IMAGE" );
	CreateControl ( "MODERN_INVENTORY_WINDOW_TRASH_IMAGE" );
	CreateControl ( "MODERN_INVENTORY_WINDOW_SORT_IMAGE" );

	OpenPage(0);
}

void CModernInventoryWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	SetMoney ( ref_CharData.m_lnMoney );

	GLInventory& ref_Inventory = ref_CharData.m_cInventory;	
	m_pInven->LoadItemPage ( ref_Inventory );
}

void CModernInventoryWindow::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}
		break;

	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( MARKET_WINDOW );
				CInnerInterface::GetInstance().HideGroup ( STORAGE_WINDOW );
				CInnerInterface::GetInstance().HideGroup ( CLUB_STORAGE_WINDOW );
			}
		}break;

	case MODERN_INVENTORY_PAGE_INVEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nPosX, nPosY;
				m_pInven->GetItemIndex ( &nPosX, &nPosY );

				if ( nPosX < 0 || nPosY < 0 ) return;

				BOOL bMarketOpen = CInnerInterface::GetInstance().IsMarketWindowOpen ();

				SINVENITEM sInvenItem = m_pInven->GetItem ( nPosX, nPosY );
				if ( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL () )
				{						
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sInvenItem.sItemCustom, bMarketOpen, FALSE, FALSE, sInvenItem.wPosX, sInvenItem.wPosY, NATIVEID_NULL(), FALSE, 0, TRUE );
				}

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sInvenItem.sItemCustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sInvenItem.sItemCustom.sNativeID );
						return;
					}
				}

				/*item link, Juver, 2017/07/31 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().LinkItem( &sInvenItem.sItemCustom );
						return;
					}
				}

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sInvenItem.sItemCustom );
						return;
					}
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( m_bSplitItem )
					{
						m_bSplitItem = FALSE;

						WORD wSplitPosX, wSplitPosY;
						GetSplitPos ( &wSplitPosX, &wSplitPosY );
						BOOL bDiffPos = !( nPosX == wSplitPosX && nPosY == wSplitPosY );
						BOOL bSplitable = GLGaeaClient::GetInstance().GetCharacter()->IsInvenSplitItem ( wSplitPosX, wSplitPosY );
						if ( bDiffPos && bSplitable )
						{
							DoModal ( ID2GAMEINTEXT ( "SPLIT_ITEM" ), MODAL_QUESTION, EDITBOX_NUMBER, MODAL_SPLIT_ITEM );
						}
						else
						{
							GLGaeaClient::GetInstance().GetCharacter()->ReqInvenTo ( nPosX, nPosY );
						}
					}
					return ;
				}

				if ( dwMsg & UIMSG_RB_UP )
				{
					/*rightclick wear/unwear, Juver, 2017/06/24 */
					HRESULT hrWear = GLGaeaClient::GetInstance().GetCharacter()->ReqInvenToWear( nPosX, nPosY );
					if ( hrWear != S_OK )
						GLGaeaClient::GetInstance().GetCharacter()->ReqInvenDrug ( nPosX, nPosY, TRUE );

					return ;
				}

				if ( dwMsg & UIMSG_LB_DOWN )
				{
					SetSplitPos ( nPosX, nPosY );
					m_bSplitItem = TRUE;
					return ;
				}
			}
		}
		break;

	case MODERN_INVENTORY_MONEY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )	
			{
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEINTEXT( "MODERN_INVENTORY_MONEY_TEXT" ) , NS_UITEXTCOLOR::GOLD );
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( CInnerInterface::GetInstance().IsStorageWindowOpen () )
				{
					DoModal ( ID2GAMEINTEXT("PLAYINTERFACE_31"), MODAL_INPUT, EDITBOX_MONEY, MODAL_SAVEMONEY );
				}
				else if ( CInnerInterface::GetInstance().IsClubStorageWindowOpen () )
				{
					DoModal ( ID2GAMEINTEXT("PLAYINTERFACE_31"), MODAL_INPUT, EDITBOX_MONEY, MODAL_CLUB_SAVEMONEY );
				}
				else
				{
					if ( GLCONST_CHAR::bMONEY_DROP2FIELD )
					{
						DoModal ( ID2GAMEINTEXT("MONEYTO_FIELD"), MODAL_INPUT, EDITBOX_MONEY, MODAL_INVENTORY_MONEY_TO_FIELD );
					}
				}
			}
		}break;

	case MODERN_INVENTORY_LOCKER_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )	
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "MODERN_INVENTORY_BUTTON_TEXT", 0 ) , NS_UITEXTCOLOR::DARKLBUE );

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
				GLGaeaClient::GetInstance().GetCharacter()->InventoryOpenLocker();	
		}break;

	case MODERN_INVENTORY_EXTEND_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )	
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "MODERN_INVENTORY_BUTTON_TEXT", 1 ) , NS_UITEXTCOLOR::DARKLBUE );

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
				GLGaeaClient::GetInstance().GetCharacter()->InventoryExpand();	
		}break;

	case MODERN_INVENTORY_TRASH_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )	
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "MODERN_INVENTORY_BUTTON_TEXT", 2 ) , NS_UITEXTCOLOR::DARKLBUE );

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
				GLGaeaClient::GetInstance().GetCharacter()->InventoryOpenTrashCan();	
		}break;

	case MODERN_INVENTORY_SORT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )	
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "MODERN_INVENTORY_BUTTON_TEXT", 3 ) , NS_UITEXTCOLOR::DARKLBUE );

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
				GLGaeaClient::GetInstance().GetCharacter()->InventorySort();	
		}break;

	case MODERN_INVENTORY_RADIO_BUTTON_0:
	case MODERN_INVENTORY_RADIO_BUTTON_1:
	case MODERN_INVENTORY_RADIO_BUTTON_2:
		{	
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					OpenPage( cID - MODERN_INVENTORY_RADIO_BUTTON_0 );
				}
			}
		}break;

	case MODERN_INVENTORY_HELP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_INVENTORY_HELP_INVEN" ), NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;
	}

	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );
}

void CModernInventoryWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage(0);
	}
}

void CModernInventoryWindow::SetMoney ( LONGLONG Money )
{
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}

void CModernInventoryWindow::GetSplitPos ( WORD* pwPosX, WORD* pwPosY )
{
	*pwPosX = m_wSplitItemPosX;
	*pwPosY = m_wSplitItemPosY;
}

void CModernInventoryWindow::SetSplitPos ( WORD wPosX, WORD wPosY )
{
	m_wSplitItemPosX = wPosX;
	m_wSplitItemPosY = wPosY;
}

void	CModernInventoryWindow::SetOneViewSlot ( const int& nONE_VIEW_SLOT )
{
	m_pInven->SetOneViewSlot ( nONE_VIEW_SLOT );
	m_nONE_VIEW_SLOT = nONE_VIEW_SLOT;
}

void CModernInventoryWindow::OpenPage ( int nPage )
{
	if ( nPage < 0 )	return;
	if ( nPage >= MODERN_INVENTORY_PAGE_SIZE )	return;

	for ( int i = 0; i < MODERN_INVENTORY_PAGE_SIZE; ++i )	
		m_pRadioButton[i]->DoImageFlip ( false );

	m_pRadioButton[nPage]->DoImageFlip ( true );

	switch ( nPage )
	{
	case 0:
		{
			m_pInven->ResetItemSlotRender ( 10, 30 );
			m_pInven->SetItemSlotRender ( 0, 10 );
		}break;
	case 1:
		{
			m_pInven->ResetItemSlotRender ( 0, 10 );
			m_pInven->ResetItemSlotRender ( 20, 30 );
			m_pInven->SetItemSlotRender ( 10, 20 );
		}break;
	case 2:
		{
			m_pInven->ResetItemSlotRender ( 0, 20 );
			m_pInven->SetItemSlotRender ( 20, 30 );
		}break;
	};
}