#include "StdAfx.h"

#include "./EP7GarbageInventoryWindow.h"
#include "./EP7GarbageInventoryPage.h"
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

//--------------------------------------------------------------------
// CEP7GarbageInventoryWindow
//--------------------------------------------------------------------
CEP7GarbageInventoryWindow::CEP7GarbageInventoryWindow()	// »ﬁ¡ˆ≈Î
	: m_pPage( NULL )
	, m_pMoneyTextBox(NULL)

{
	for( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CEP7GarbageInventoryWindow::~CEP7GarbageInventoryWindow()
{
}

CMultiModeButton* CEP7GarbageInventoryWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "EP7_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "EP7_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "EP7_GARBAGE_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}

VOID CEP7GarbageInventoryWindow::CreateSubControl()
{
	m_pPage = new CEP7GarbageInventoryPage;
	m_pPage->CreateSub( this, "EP7_GARBAGE_INVENTORY_PAGE", UI_FLAG_YSIZE, GARBAGEINVENTORY_PAGE );
	m_pPage->CreateSubControl();
	m_pPage->SetOneViewSlot ( EP7_GARBAGE_INVEN_DEFAULT_VIEW );
	RegisterControl( m_pPage );

	CUIControl* pControl = CreateControl( "EP7_GARBAGE_INVENTORY_MONEY_BACK" );
	pControl->SetAlignFlag( UI_FLAG_BOTTOM );

	CBasicButton* pButton = CreateFlipButton( "EP7_GARBAGE_INVENTORY_MONEY_BUTTON", "EP7_GARBAGE_INVENTORY_MONEY_BUTTON_F", GARBAGEINVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
	pButton->CreateMouseOver( "EP7_GARBAGE_INVENTORY_MONEY_BUTTON_F" );
	pButton->SetUseDynamic( TRUE );
	pButton->SetAlignFlag( UI_FLAG_BOTTOM );

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	m_pMoneyTextBox = CreateStaticControl( "EP7_GARBAGE_INVENTORY_MONEYTEXT", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
	m_pMoneyTextBox->SetAlignFlag( UI_FLAG_BOTTOM );


	std::string strRadio[EP7_INVENTORY_PAGE_SIZE] = 
	{
		"EP7_GARBAGE_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"EP7_GARBAGE_INVENTORY_WINDOW_RADIO_BUTTON_2",
	};

	for ( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), EP7_INVENTORY_RADIO_BUTTON_0 +i );
	}

	OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
}

VOID CEP7GarbageInventoryWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	
	m_pPage->ResetAllFlipItem();

	for ( int i=0; i<ITEM_GARBAGE_MAX_SLOT; ++i ) 
	{
		if( pCharacter->m_sGarbageItem[i].VALID() )
			m_pPage->UpdateFlipItem( pCharacter->m_sGarbageItem[i] );		
	}

	{
		CString strTemp1 = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( pCharacter->m_lnMoney, 3, "," );
		m_pMoneyTextBox->SetOneLineText( strTemp1 );

	}

	{
		GLInventory& ref_Inventory = pCharacter->m_cInventory;
		m_pPage->LoadItemPage( ref_Inventory );
	}
}

VOID CEP7GarbageInventoryWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case GARBAGEINVENTORY_PAGE:
		if( CHECK_MOUSE_IN( dwMsg ) )
		{
			INT nPosX, nPosY;
			m_pPage->GetItemIndex( &nPosX, &nPosY );

			CDebugSet::ToView( 1, 1, "CEP7GarbageInventoryWindow::TranslateUIMessage: %d %d", nPosX, nPosY );

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
		break;	

	case ET_CONTROL_BUTTON:
		if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseItemGarbageWindow();
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

void CEP7GarbageInventoryWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
	}
}

HRESULT CEP7GarbageInventoryWindow::RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
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

VOID CEP7GarbageInventoryWindow::SetOneViewSlot( const INT& nONE_VIEW_SLOT )
{
}

void CEP7GarbageInventoryWindow::OpenInvenPage ( int nPage )
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