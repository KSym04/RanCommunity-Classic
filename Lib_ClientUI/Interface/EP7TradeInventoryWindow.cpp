#include "StdAfx.h"
#include "./EP7TradeInventoryWindow.h"
#include "./EP7TradeInventoryPage.h"
#include "./BasicButton.h"
#include "./ModalCallerID.h"
#include "./ModalWindow.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./BasicTextBox.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "./MultiModeButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEP7TradeInventoryWindow::CEP7TradeInventoryWindow ()
	: m_pPage ( NULL )
	, m_pMoneyTextBox ( NULL )

{
	for( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CEP7TradeInventoryWindow::~CEP7TradeInventoryWindow ()
{
}

CMultiModeButton* CEP7TradeInventoryWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "EP7_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "EP7_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "EP7_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}

void CEP7TradeInventoryWindow::CreateSubControl ()
{
	m_pPage = new CEP7TradeInventoryPage;
	m_pPage->CreateSub ( this, "EP7_TRADE_INVENTORY_PAGE", UI_FLAG_YSIZE, TRADEINVENTORY_PAGE );
	m_pPage->CreateSubControl ();
	m_pPage->SetOneViewSlot ( EP7_REBUILD_INVEN_DEFAULT_VIEW );
	RegisterControl ( m_pPage );

	CUIControl* pControl = CreateControl ( "EP7_TRADE_INVENTORY_MONEY_BACK" );
	pControl->SetAlignFlag ( UI_FLAG_BOTTOM );

	CBasicButton* pButton = CreateFlipButton ( "EP7_TRADE_INVENTORY_MONEY_BUTTON", "EP7_TRADE_INVENTORY_MONEY_BUTTON_F", TRADEINVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
	pButton->CreateMouseOver ( "EP7_TRADE_INVENTORY_MONEY_BUTTON_F" );
	pButton->SetUseDynamic ( TRUE );
	pButton->SetAlignFlag ( UI_FLAG_BOTTOM );

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
	m_pMoneyTextBox = CreateStaticControl ( "EP7_TRADE_INVENTORY_MONEYTEXT", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
	m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );


	std::string strRadio[EP7_INVENTORY_PAGE_SIZE] = 
	{
		"EP7_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"EP7_TRADE_INVENTORY_WINDOW_RADIO_BUTTON_2",
	};

	for ( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), EP7_INVENTORY_RADIO_BUTTON_0 +i );
	}

	OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
}

void CEP7TradeInventoryWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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
	}

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	SetMoney ( ref_CharData.m_lnMoney );

	GLInventory& ref_Inventory = ref_CharData.m_cInventory;	
	m_pPage->LoadItemPage ( ref_Inventory );
}

void CEP7TradeInventoryWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	//	의도적으로 윈도우 이동을 막음
//	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case TRADEINVENTORY_PAGE:
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

	case TRADEINVENTORY_MONEY_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//	거래(TRADE)일 경우에만 모달이 뜬다.
				if ( m_bTRADE_MODE ) DoModal ( ID2GAMEINTEXT("TRADE_MONEY"), MODAL_INPUT, EDITBOX_MONEY, MODAL_TRADEMONEY );
			}
		}
		break;

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
}

void CEP7TradeInventoryWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
	}
}

void CEP7TradeInventoryWindow::SetMoney ( LONGLONG Money )
{
	//	돈자리수
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}

HRESULT CEP7TradeInventoryWindow::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
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

bool CEP7TradeInventoryWindow::IsTurnITEM ( SITEMCUSTOM& sItemCustom )
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

void	CEP7TradeInventoryWindow::SetOPENER ( bool bOPENER )
{
	m_bOPENER = bOPENER;
}

void	CEP7TradeInventoryWindow::SetOneViewSlot ( const int& nONE_VIEW_SLOT )
{
}

void CEP7TradeInventoryWindow::OpenInvenPage ( int nPage )
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