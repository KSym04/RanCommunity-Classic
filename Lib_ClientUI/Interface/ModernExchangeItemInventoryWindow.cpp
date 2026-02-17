#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"
#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./MultiModeButton.h"
#include "./BasicButton.h"

#include "./ModernExchangeItemInventoryWindow.h"
#include "./ModernExchangeItemInventoryPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernExchangeItemInventoryWindow::CModernExchangeItemInventoryWindow()
	: m_pPage( NULL )
	, m_pBackGround(NULL)
	, m_pMoneyTextBox(NULL)
	, m_pHelpButton(NULL)
	, m_nONE_VIEW_SLOT( 4 )
{
	for( int i=0; i<MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CModernExchangeItemInventoryWindow::~CModernExchangeItemInventoryWindow()
{
}

CMultiModeButton* CModernExchangeItemInventoryWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::BLACK );
	pControl->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}

void CModernExchangeItemInventoryWindow::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_REGION" );
	RegisterControl ( pBasicLineBox );

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );
	pUIControl->AlignToControl( pBasicLineBox, TRUE );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_ITEM_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_MONEY_REGION" );
	RegisterControl ( pBasicLineBox );

	std::string strRadio[MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE] = 
	{
		"MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_RADIO_BUTTON_2",
		"MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_RADIO_BUTTON_3",
	};

	for ( int i=0; i<MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_0 +i );
	}

	m_pHelpButton = new CBasicButton;
	m_pHelpButton->CreateSub ( this, "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_HELP_BUTTON" , UI_FLAG_DEFAULT, MODERN_EXCHANGE_ITEM_INVENTORY_HELP_BUTTON );
	m_pHelpButton->CreateFlip ( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_HELP_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pHelpButton->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelpButton );

	m_pPage = new CModernExchangeItemInventoryPage;
	m_pPage->CreateSub( this, "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW_ITEM_REGION", UI_FLAG_YSIZE, MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_INVEN );
	m_pPage->CreateSubControl();
	RegisterControl( m_pPage );

	CBasicButton* pMoneyButton = NULL;
	pMoneyButton = CreateFlipButton ( "MODERN_EXCHANGE_ITEM_INVENTORY_MONEY_BUTTON_RENEW", "MODERN_EXCHANGE_ITEM_INVENTORY_MONEY_BUTTON_RENEW_F", MODERN_EXCHANGE_ITEM_INVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
	pMoneyButton->CreateMouseOver ( "MODERN_EXCHANGE_ITEM_INVENTORY_MONEY_BUTTON_RENEW_F" );
	pMoneyButton->SetAlignFlag ( UI_FLAG_BOTTOM );
	pMoneyButton->SetUseDynamic ( TRUE );

	m_pMoneyTextBox = CreateStaticControl ( "MODERN_EXCHANGE_ITEM_INVENTORY_MONEY_TEXT_RENEW", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
	m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

	OpenPage(0);
}

void CModernExchangeItemInventoryWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

	m_pPage->ResetAllFlipItem();

	if( pCharacter->m_sExchangeItemInvenPos.VALID() )
		m_pPage->UpdateFlipItem( pCharacter->m_sExchangeItemInvenPos );	
	
	SetMoney ( pCharacter->m_lnMoney );

	GLInventory& ref_Inventory = pCharacter->m_cInventory;
	m_pPage->LoadItemPage( ref_Inventory );
}

void CModernExchangeItemInventoryWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
				CInnerInterface::GetInstance().CloseExchangeItemWindow();
		}break;

	case MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_INVEN:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				INT nPosX, nPosY;
				m_pPage->GetItemIndex( &nPosX, &nPosY );

				if( nPosX < 0 || nPosY < 0 )
					return;

				SINVENITEM& sInvenItem = m_pPage->GetItem( nPosX, nPosY );
				if( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL() )
					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sInvenItem.sItemCustom, FALSE, FALSE, FALSE, sInvenItem.wPosX, sInvenItem.wPosY );

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

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sInvenItem.sItemCustom );
						return;
					}
				}

				if( dwMsg & UIMSG_LB_UP )
					GLGaeaClient::GetInstance().GetCharacter()->ReqInvenTo( nPosX, nPosY );
			}
		}break;	

	case MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_0:
	case MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_1:
	case MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_2:
		{	
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					OpenPage( ControlID - MODERN_EXCHANGE_ITEM_INVENTORY_RADIO_BUTTON_0 );
				}
			}
		}break;

	case MODERN_EXCHANGE_ITEM_INVENTORY_HELP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_INVENTORY_HELP_INVEN" ), NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;
	
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

HRESULT CModernExchangeItemInventoryWindow::RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = CUIWindowEx::RestoreDeviceObjects( pd3dDevice );
	if( FAILED( hr ) )
		return hr;

	const LONG lResolution = CInnerInterface::GetInstance().GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );

	const UIRECT& rcWindow = GetGlobalPos();

	D3DXVECTOR2 vPos;
	vPos.x = ( (X_RES) / 2.0f );
	vPos.y = ( Y_RES - rcWindow.sizeY ) / 2.0f;
	SetGlobalPos( vPos );

	return S_OK;
}

void CModernExchangeItemInventoryWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage(0);
	}
}

void CModernExchangeItemInventoryWindow::SetOneViewSlot( const INT& nONE_VIEW_SLOT )
{
	m_pPage->SetOneViewSlot( nONE_VIEW_SLOT );
	m_nONE_VIEW_SLOT = nONE_VIEW_SLOT;
}

void CModernExchangeItemInventoryWindow::SetMoney ( LONGLONG Money )
{
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}

void CModernExchangeItemInventoryWindow::OpenPage ( int nPage )
{
	if ( nPage < 0 )	return;
	if ( nPage >= MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE )	return;

	for ( int i = 0; i < MODERN_EXCHANGE_ITEM_INVENTORY_PAGE_SIZE; ++i )	
		m_pRadioButton[i]->DoImageFlip ( false );

	m_pRadioButton[nPage]->DoImageFlip ( true );

	switch ( nPage )
	{
	case 0:
		{
			m_pPage->ResetItemSlotRender ( 10, 30 );
			m_pPage->SetItemSlotRender ( 0, 10 );
		}break;
	case 1:
		{
			m_pPage->ResetItemSlotRender ( 0, 10 );
			m_pPage->ResetItemSlotRender ( 20, 30 );
			m_pPage->SetItemSlotRender ( 10, 20 );
		}break;
	case 2:
		{
			m_pPage->ResetItemSlotRender ( 0, 20 );
			m_pPage->SetItemSlotRender ( 20, 30 );
		}break;
	};
}