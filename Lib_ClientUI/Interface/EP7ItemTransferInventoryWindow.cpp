#include "StdAfx.h"

#include "./EP7ItemTransferInventoryWindow.h"
#include "./EP7ItemTransferInventoryPage.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./GameTextControl.h"
#include "./ModalCallerID.h"
#include "./ModalWindow.h"
#include "../../Lib_Engine/DxCommon/D3DFont.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./MultiModeButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEP7ItemTransferInvenWindow::CEP7ItemTransferInvenWindow()
	: m_pPage( NULL )
	, m_pMoneyTextBox(NULL)

#if defined( BUILD_EP7 )
	, m_pGamePointsTextBox(NULL)
	, m_pWarChipsTextBox(NULL)
#endif

{
	for( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CEP7ItemTransferInvenWindow::~CEP7ItemTransferInvenWindow()
{
}

CMultiModeButton* CEP7ItemTransferInvenWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "EP7_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "EP7_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "EP7_ITEM_TRANSFER_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}

void CEP7ItemTransferInvenWindow::CreateSubControl()
{
	m_pPage = new CEP7ItemTransferInvenPage;
	m_pPage->CreateSub( this, "EP7_ITEM_TRANSFER_INVENTORY_PAGE", UI_FLAG_YSIZE, ITEM_TRANSFER_INVEN_PAGE );
	m_pPage->CreateSubControl();
	m_pPage->SetOneViewSlot ( EP7_ITEM_TRANSFER_INVEN_DEFAULT_VIEW );
	RegisterControl( m_pPage );

	CUIControl* pControl = CreateControl( "EP7_ITEM_TRANSFER_INVENTORY_MONEY_BACK" );
	pControl->SetAlignFlag( UI_FLAG_BOTTOM );

	CBasicButton* pButton = CreateFlipButton( "EP7_ITEM_TRANSFER_INVENTORY_MONEY_BUTTON", "EP7_ITEM_TRANSFER_INVENTORY_MONEY_BUTTON_F", ITEM_TRANSFER_INVEN_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
	pButton->CreateMouseOver( "EP7_ITEM_TRANSFER_INVENTORY_MONEY_BUTTON_F" );
	pButton->SetUseDynamic( TRUE );
	pButton->SetAlignFlag( UI_FLAG_BOTTOM );

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	m_pMoneyTextBox = CreateStaticControl( "EP7_ITEM_TRANSFER_INVENTORY_MONEYTEXT", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
	m_pMoneyTextBox->SetAlignFlag( UI_FLAG_BOTTOM );

#if defined( BUILD_EP7 )
	{
		CUIControl* pControl = CreateControl ( "EP7_ITEM_TRANSFER_INVENTORY_GAME_POINTS_BACK" );
		pControl->SetAlignFlag ( UI_FLAG_BOTTOM );

		CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
		m_pGamePointsTextBox = CreateStaticControl ( "EP7_ITEM_TRANSFER_INVENTORY_GAME_POINTS_TEXT", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
		m_pGamePointsTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "EP7_ITEM_TRANSFER_INVENTORY_GAME_POINTS_BUTTON", "EP7_ITEM_TRANSFER_INVENTORY_GAME_POINTS_BUTTON_F", ITEM_TRANSFER_INVEN_GAME_POINTS_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "EP7_ITEM_TRANSFER_INVENTORY_GAME_POINTS_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_BOTTOM );
		pButton->SetUseDynamic ( TRUE );
	}

	{
		CUIControl* pControl = CreateControl ( "EP7_ITEM_TRANSFER_INVENTORY_WAR_CHIPS_BACK" );
		pControl->SetAlignFlag ( UI_FLAG_BOTTOM );

		CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
		m_pWarChipsTextBox = CreateStaticControl ( "EP7_ITEM_TRANSFER_INVENTORY_WAR_CHIPS_TEXT", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
		m_pWarChipsTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "EP7_ITEM_TRANSFER_INVENTORY_WAR_CHIPS_BUTTON", "EP7_ITEM_TRANSFER_INVENTORY_WAR_CHIPS_BUTTON_F", ITEM_TRANSFER_INVEN_WAR_CHIPS_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "EP7_ITEM_TRANSFER_INVENTORY_WAR_CHIPS_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_BOTTOM );
		pButton->SetUseDynamic ( TRUE );
	}
#endif

	std::string strRadio[EP7_INVENTORY_PAGE_SIZE] = 
	{
		"EP7_ITEM_TRANSFER_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"EP7_ITEM_TRANSFER_INVENTORY_WINDOW_RADIO_BUTTON_2",
	};

	for ( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), EP7_INVENTORY_RADIO_BUTTON_0 +i );
	}

	OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
}

void CEP7ItemTransferInvenWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

	m_pPage->ResetAllFlipItem();

	if( pCharacter->m_sItemTransferPosOld.VALID() )
		m_pPage->UpdateFlipItem( pCharacter->m_sItemTransferPosOld );	

	if( pCharacter->m_sItemTransferPosNew.VALID() )
		m_pPage->UpdateFlipItem( pCharacter->m_sItemTransferPosNew );	

	{
		CString strTemp1 = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( pCharacter->m_lnMoney, 3, "," );
		m_pMoneyTextBox->SetOneLineText( strTemp1 );

#if defined( BUILD_EP7 )
		CString strTemp2 = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT_UINT( pCharacter->m_dwGamePoints, 3, "," );
		m_pGamePointsTextBox->SetOneLineText( strTemp2 );

		CString strTemp3 = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT_UINT( pCharacter->m_dwWarChips, 3, "," );
		m_pWarChipsTextBox->SetOneLineText( strTemp3 );
#endif

	}

	{
		GLInventory& ref_Inventory = pCharacter->m_cInventory;
		m_pPage->LoadItemPage( ref_Inventory );
	}
}

void CEP7ItemTransferInvenWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ITEM_TRANSFER_INVEN_PAGE:
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

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
				CInnerInterface::GetInstance().CloseItemTransferWindow();
		}
		break;

	case EP7_INVENTORY_RADIO_BUTTON_0:
	case EP7_INVENTORY_RADIO_BUTTON_1:
		{	
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					OpenInvenPage( ControlID - EP7_INVENTORY_RADIO_BUTTON_0 );
				}
			}
		}break;
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

HRESULT CEP7ItemTransferInvenWindow::RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
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

void CEP7ItemTransferInvenWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
	}
}

void CEP7ItemTransferInvenWindow::SetOneViewSlot( const INT& nONE_VIEW_SLOT )
{
}

void CEP7ItemTransferInvenWindow::OpenInvenPage ( int nPage )
{
	if ( nPage < 0 )	return;
	if ( nPage >= EP7_INVENTORY_PAGE_SIZE )	return;

	for ( int i = 0; i < EP7_INVENTORY_PAGE_SIZE; ++i )	
		m_pRadioButton[i]->DoImageFlip ( false );

	m_pRadioButton[nPage]->DoImageFlip ( true );

	switch ( nPage )
	{
	case EP7_INVENTORY_PAGE_INVEN_1:
		{
			m_pPage->ResetItemSlotRender ( 10, 20 );
			m_pPage->SetItemSlotRender ( 0, 10 );
		}break;

	case EP7_INVENTORY_PAGE_INVEN_2:
		{
			m_pPage->ResetItemSlotRender ( 0, 10 );
			m_pPage->SetItemSlotRender ( 10, 20 );
		}break;
	};
}