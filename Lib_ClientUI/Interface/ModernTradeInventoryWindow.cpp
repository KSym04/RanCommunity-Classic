#include "StdAfx.h"
#include "./ModernTradeInventoryWindow.h"
#include "./ModernTradeInventoryPage.h"

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
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernTradeInventoryWindow::CModernTradeInventoryWindow ()
	: m_pPage ( NULL )
	, m_pBackGround(NULL)
	, m_pMoneyTextBox(NULL)
	, m_pHelpButton(NULL)
	, m_nONE_VIEW_SLOT( 4 )
{
	for( int i=0; i<MODERN_TRADE_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CModernTradeInventoryWindow::~CModernTradeInventoryWindow ()
{
}

CMultiModeButton* CModernTradeInventoryWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "MODERN_TRADE_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "MODERN_TRADE_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "MODERN_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::BLACK );
	pControl->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::BLACK );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}

void CModernTradeInventoryWindow::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "MODERN_TRADE_INVENTORY_WINDOW_REGION" );
	RegisterControl ( pBasicLineBox );

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, "MODERN_TRADE_INVENTORY_WINDOW_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );
	pUIControl->AlignToControl( pBasicLineBox, TRUE );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_TRADE_INVENTORY_WINDOW_ITEM_REGION" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_TRADE_INVENTORY_WINDOW_MONEY_REGION" );
	RegisterControl ( pBasicLineBox );

	std::string strRadio[MODERN_TRADE_INVENTORY_PAGE_SIZE] = 
	{
		"MODERN_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"MODERN_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_2",
		"MODERN_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_3",
	};

	for ( int i=0; i<MODERN_TRADE_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), MODERN_TRADE_INVENTORY_RADIO_BUTTON_0 +i );
	}

	m_pHelpButton = new CBasicButton;
	m_pHelpButton->CreateSub ( this, "MODERN_TRADE_INVENTORY_WINDOW_HELP_BUTTON" , UI_FLAG_DEFAULT, MODERN_TRADE_INVENTORY_HELP_BUTTON );
	m_pHelpButton->CreateFlip ( "MODERN_TRADE_INVENTORY_WINDOW_HELP_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pHelpButton->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelpButton );

	CModernTradeInventoryPage* pTradeInventoryPage = new CModernTradeInventoryPage;
	pTradeInventoryPage->CreateSub ( this, "MODERN_TRADE_INVENTORY_WINDOW_ITEM_REGION", UI_FLAG_YSIZE, MODERN_TRADE_INVENTORY_PAGE_INVEN );
	pTradeInventoryPage->CreateSubControl ();
	RegisterControl ( pTradeInventoryPage );
	m_pPage = pTradeInventoryPage;

	CBasicButton* pMoneyButton = NULL;
	pMoneyButton = CreateFlipButton ( "MODERN_TRADE_INVENTORY_MONEY_BUTTON_RENEW", "MODERN_TRADE_INVENTORY_MONEY_BUTTON_RENEW_F", MODERN_TRADE_INVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
	pMoneyButton->CreateMouseOver ( "MODERN_TRADE_INVENTORY_MONEY_BUTTON_RENEW_F" );
	pMoneyButton->SetAlignFlag ( UI_FLAG_BOTTOM );
	pMoneyButton->SetUseDynamic ( TRUE );

	m_pMoneyTextBox = CreateStaticControl ( "MODERN_TRADE_INVENTORY_MONEY_TEXT_RENEW", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
	m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

	OpenPage(0);
}

void CModernTradeInventoryWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_bTRADE_MODE = (CInnerInterface::GetInstance().IsVisibleGroup(TRADE_WINDOW))?true:false;

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_bTRADE_MODE )
	{
		GLInventory& ref_ItemData = GLTradeClient::GetInstance().GetMyTradeBox();
		m_pPage->UpdateFlipItem ( ref_ItemData );
	}
	else
	{
		m_pPage->ResetAllFlipItem ();
//		GLInventory& ref_ItemData = GLGaeaClient::GetInstance().GetCharacter ()->m_sPMarket.GetInven();
//		m_pPage->UpdateFlipItem ( ref_ItemData );
	}

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	SetMoney ( ref_CharData.m_lnMoney );

	GLInventory& ref_Inventory = ref_CharData.m_cInventory;	
	m_pPage->LoadItemPage ( ref_Inventory );
}

void CModernTradeInventoryWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	//	의도적으로 윈도우 이동을 막음
//	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{				
				if ( m_bTRADE_MODE )	CInnerInterface::GetInstance().SetTradeWindowCloseReq ();
				else
				{
					bool bOPENER;
					DWORD dwGaeaID;
					CInnerInterface::GetInstance().GetPrivateMarketInfo ( bOPENER, dwGaeaID );
					if ( bOPENER )
					{
						DoModal ( ID2GAMEINTEXT("PRIVATE_MARKET_QUIT"), MODAL_QUESTION, YESNO, MODAL_PRIVATE_MARKET_QUIT );
					}
					else
					{
						CInnerInterface::GetInstance().SetPrivateMarketClose ();
					}
				}
			}
		}break;

	case MODERN_TRADE_INVENTORY_PAGE_INVEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nPosX, nPosY;
				m_pPage->GetItemIndex ( &nPosX, &nPosY );

				CDebugSet::ToView ( 1, 1, "인벤 : %d %d", nPosX, nPosY );				

				if ( nPosX < 0 || nPosY < 0 ) return;

//				if ( CUIMan::GetFocusControl () == this )
//				{
					SINVENITEM& sInvenItem = m_pPage->GetItem ( nPosX, nPosY );
					if ( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL () )
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sInvenItem.sItemCustom, FALSE, FALSE, FALSE, sInvenItem.wPosX, sInvenItem.wPosY );
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

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sInvenItem.sItemCustom );
							return;
						}
					}
//				}

				if ( dwMsg & UIMSG_LB_UP )
				{   
					if ( m_bTRADE_MODE )	//	거래모드
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqInvenTo ( nPosX, nPosY );
					}
					else					//	상점모드
					{
						if ( m_bOPENER )
						{
							GLPrivateMarket &sPMarket = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket;
							if ( sPMarket.IsOpen () ) return ;

							if ( sInvenItem.sItemCustom.sNativeID == NATIVEID_NULL () ) return ;
							const bool bCOUNTABLE = IsTurnITEM ( sInvenItem.sItemCustom );
							WORD wPosX = static_cast<WORD>(nPosX);
							WORD wPosY = static_cast<WORD>(nPosY);

							CInnerInterface::GetInstance().MODAL_PRIVATE_MARKET_SELLITEM ( bCOUNTABLE, wPosX, wPosY );
						}
						else
						{
//							GLGaeaClient::GetInstance().GetCharacter()->ReqInvenTo ( nPosX, nPosY );
						}
					}
				}
				if ( dwMsg & UIMSG_RB_UP )
				{
//					GLGaeaClient::GetInstance().GetCharacter()->ReqInvenDrug ( nPosX, nPosY );
				}
			}
		}
		break;

	case MODERN_TRADE_INVENTORY_MONEY_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//	거래(TRADE)일 경우에만 모달이 뜬다.
				if ( m_bTRADE_MODE ) DoModal ( ID2GAMEINTEXT("TRADE_MONEY"), MODAL_INPUT, EDITBOX_MONEY, MODAL_TRADEMONEY );
			}
		}break;

	case MODERN_TRADE_INVENTORY_RADIO_BUTTON_0:
	case MODERN_TRADE_INVENTORY_RADIO_BUTTON_1:
	case MODERN_TRADE_INVENTORY_RADIO_BUTTON_2:
		{	
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					OpenPage( ControlID - MODERN_TRADE_INVENTORY_RADIO_BUTTON_0 );
				}
			}
		}break;

	case MODERN_TRADE_INVENTORY_HELP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
				CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_INVENTORY_HELP_INVEN" ), NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;
	}
}


void CModernTradeInventoryWindow::SetMoney ( LONGLONG Money )
{
	//	돈자리수
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}

HRESULT CModernTradeInventoryWindow::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	hr = CUIWindowEx::RestoreDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	const long lResolution = CInnerInterface::GetInstance().GetResolution ();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	const UIRECT& rcWindow = GetGlobalPos ();

	D3DXVECTOR2 vPos;
	vPos.x = ((X_RES) / 2.0f);
	vPos.y = (Y_RES - rcWindow.sizeY) / 2.0f;
	SetGlobalPos ( vPos );

	return S_OK;
}

void CModernTradeInventoryWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage(0);
	}
}

bool CModernTradeInventoryWindow::IsTurnITEM ( SITEMCUSTOM& sItemCustom )
{
	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
	if ( !pItemData )
	{
		GASSERT ( 0 && "아이템 정보를 찾을 수 없습니다." );
		return false;
	}

	const WORD& wPileNum = pItemData->sDrugOp.wPileNum;
	if ( wPileNum > 1 )
	{
		const WORD& wTurnNum = sItemCustom.wTurnNum;        
		if ( wTurnNum > 1 ) return true;
	}

	return false;
}

void	CModernTradeInventoryWindow::SetOPENER ( bool bOPENER )
{
	m_bOPENER = bOPENER;
}

void	CModernTradeInventoryWindow::SetOneViewSlot ( const int& nONE_VIEW_SLOT )
{
	m_pPage->SetOneViewSlot ( nONE_VIEW_SLOT );
	m_nONE_VIEW_SLOT = nONE_VIEW_SLOT;
}

const int& CModernTradeInventoryWindow::GetOneViewSlot () const
{
	return m_nONE_VIEW_SLOT;
}

void CModernTradeInventoryWindow::OpenPage ( int nPage )
{
	if ( nPage < 0 )	return;
	if ( nPage >= MODERN_TRADE_INVENTORY_PAGE_SIZE )	return;

	for ( int i = 0; i < MODERN_TRADE_INVENTORY_PAGE_SIZE; ++i )	
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