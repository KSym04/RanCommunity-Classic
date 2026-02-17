#include "StdAfx.h"
#include "./EP7InventoryWindow.h"
#include "./EP7InventoryPage.h"
#include "./InventoryPageWear.h"
#include "./InventoryPageWearEx.h"
#include "./UITextControl.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./ModalWindow.h"
#include "./ModalCallerID.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "./MarketWindow.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/DxGlobalStage.h"

#include "./BasicButton.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./MultiModeButton.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEP7InventoryWindow::CEP7InventoryWindow ()
	: m_pPage( NULL )
	, m_pPageWear( NULL )
	, m_pPageWearEx( NULL )
	, m_pMoneyTextBox( NULL )


	, m_pSortButton(NULL)		/*inventory sort, Juver, 2018/07/07 */
	, m_pButtonPageMain(NULL)
	, m_pButtonPageSub(NULL)
{
	for( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
		m_pRadioButton[i] = NULL;
}

CEP7InventoryWindow::~CEP7InventoryWindow ()
{
}

CMultiModeButton* CEP7InventoryWindow::CreateRadioButton( char* szControl, char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateImageBase( "EP7_INVENTORY_WINDOW_RADIO_OFF_TEXTUREINFO" );
	pControl->CreateImageFlip( "EP7_INVENTORY_WINDOW_RADIO_ON_TEXTUREINFO" );
	pControl->CreateTextBox( "EP7_INVENTORY_WINDOW_RADIO_BUTTON_TEXT", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->DoImageFlip( TRUE );
	RegisterControl ( pControl );

	return pControl;
}

void CEP7InventoryWindow::CreateSubControl ()
{
	const EMCHARCLASS emCharClass = DxGlobalStage::GetInstance().GetGameStage()->GetCharJoinData().m_CharData2.m_emClass;
	const int nClassType = CharClassToIndex ( emCharClass );

	// 극강부와 일반 캐릭터를 구분해서 인벤토리 장착 윈도우를 생성한다.
	if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
	{
		m_pPageWearEx = new CInventoryPageWearEx;
		m_pPageWearEx->CreateSub ( this, "EP7_INVENTORY_PAGE_WEAR", UI_FLAG_DEFAULT, INVENTORY_PAGEWEAR_EX );
		m_pPageWearEx->CreateSubControl ( nClassType );
		RegisterControl ( m_pPageWearEx );
	}
	else
	{
		m_pPageWear = new CInventoryPageWear;
		m_pPageWear->CreateSub ( this, "EP7_INVENTORY_PAGE_WEAR", UI_FLAG_DEFAULT, INVENTORY_PAGEWEAR );
		m_pPageWear->CreateSubControl ( nClassType );
		RegisterControl ( m_pPageWear );
	}

	std::string strRadio[EP7_INVENTORY_PAGE_SIZE] = 
	{
		"EP7_INVENTORY_WINDOW_RADIO_BUTTON_1",
		"EP7_INVENTORY_WINDOW_RADIO_BUTTON_2",
	};

	for ( int i=0; i<EP7_INVENTORY_PAGE_SIZE; ++i )
	{
		CString strtext;
		strtext.Format( "%d", i+1 );
		m_pRadioButton[i] = CreateRadioButton( (char*)strRadio[i].c_str(), (char*)strtext.GetString(), EP7_INVENTORY_RADIO_BUTTON_0 +i );
	}

	m_pPage = new CEP7InventoryPage;
	m_pPage->CreateSub ( this, "EP7_INVENTORY_PAGE", UI_FLAG_YSIZE, INVENTORY_PAGE );
	m_pPage->CreateSubControl ();
	m_pPage->SetOneViewSlot ( EP7_INVEN_DEFAULT_VIEW );
	RegisterControl ( m_pPage );

	/*inventory sort, Juver, 2018/07/07 */
	if ( RPARAM::use_inven_sort )
	{
		CUIControl* pControl = CreateControl ( "EP7_INVENTORY_MONEY_BACK2" );
		pControl->SetAlignFlag ( UI_FLAG_BOTTOM );

		CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
		m_pMoneyTextBox = CreateStaticControl ( "EP7_INVENTORY_MONEY_TEXT2", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
		m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "EP7_INVENTORY_MONEY_BUTTON", "EP7_INVENTORY_MONEY_BUTTON_F", INVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "EP7_INVENTORY_MONEY_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_BOTTOM );
		pButton->SetUseDynamic ( TRUE );
	}
	else
	{
		CUIControl* pControl = CreateControl ( "EP7_INVENTORY_MONEY_BACK" );
		pControl->SetAlignFlag ( UI_FLAG_BOTTOM );

		CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );
		m_pMoneyTextBox = CreateStaticControl ( "EP7_INVENTORY_MONEY_TEXT", pFont8, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
		m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_BOTTOM );

		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "EP7_INVENTORY_MONEY_BUTTON", "EP7_INVENTORY_MONEY_BUTTON_F", INVENTORY_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "EP7_INVENTORY_MONEY_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_BOTTOM );
		pButton->SetUseDynamic ( TRUE );
	}

	/*inventory sort, Juver, 2018/07/07 */
	if ( RPARAM::use_inven_sort )
	{
		m_pSortButton = new CBasicTextButton;
		m_pSortButton->CreateSub( this, "BASIC_TEXT_BUTTON18", UI_FLAG_BOTTOM | UI_FLAG_XSIZE, INVENTORY_SORT_BUTTON );
		m_pSortButton->CreateBaseButton( "EP7_INVENTORY_ITEM_SORT_BUTTON", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD( "INVENTORY_ITEM_SORT_BUTTON", 0 ) );
		RegisterControl( m_pSortButton );
	}

	OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
}

void CEP7InventoryWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	SetMoney ( ref_CharData.m_lnMoney );


	GLInventory& ref_Inventory = ref_CharData.m_cInventory;	
	m_pPage->LoadItemPage ( ref_Inventory );
}

void CEP7InventoryWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( INVENTORY_WINDOW );
			}
		}break;

	case INVENTORY_PAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nPosX, nPosY;
				m_pPage->GetItemIndex ( &nPosX, &nPosY );

				if ( nPosX < 0 || nPosY < 0 ) return;

				BOOL bMarketOpen = CInnerInterface::GetInstance().IsMarketWindowOpen ();

				{
					SINVENITEM sInvenItem = m_pPage->GetItem ( nPosX, nPosY );
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
		}break;

	case INVENTORY_PAGEWEAR:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pPageWear->GetItemIndex ();
				if ( nIndex < 0 ) return ;				

				{
					SITEMCUSTOM sItemCustom = m_pPageWear->GetItem ( nIndex );
					if ( sItemCustom.sNativeID != NATIVEID_NULL () )
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
					}

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sItemCustom );
							return;
						}
					}

					/*item link, Juver, 2017/07/31 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().LinkItem( &sItemCustom );
							return;
						}
					}

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
							return;
						}
					}
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					EMSLOT emSlot = m_pPageWear->GetItemSlot ();
					GLGaeaClient::GetInstance().GetCharacter()->ReqSlotTo ( emSlot );
				}

				/*rightclick wear/unwear, Juver, 2017/06/24 */
				if ( dwMsg & UIMSG_RB_UP )
				{
					EMSLOT emSlot = m_pPageWear->GetItemSlot ();

					SNATIVEID sid_item_hold = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
					if ( sid_item_hold != NATIVEID_NULL () )
					{
						BOOL bUse = GLGaeaClient::GetInstance().GetCharacter()->InvenUseToPutOn ( emSlot );
						if ( bUse )	return;
					}

					GLGaeaClient::GetInstance().GetCharacter()->ReqWearToInven ( emSlot );
					return;
				}
			}
		}break;

	case INVENTORY_PAGEWEAR_EX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pPageWearEx->GetItemIndex ();
				if ( nIndex < 0 ) return ;				

				//if ( CUIMan::GetFocusControl () == this )
				{
					SITEMCUSTOM sItemCustom = m_pPageWearEx->GetItem ( nIndex );
					if ( sItemCustom.sNativeID != NATIVEID_NULL () )
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
					}

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sItemCustom );
							return;
						}
					}

					/*item link, Juver, 2017/07/31 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().LinkItem( &sItemCustom );
							return;
						}
					}

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
							return;
						}
					}
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					EMSLOT emSlot = m_pPageWearEx->GetItemSlot ();
					GLGaeaClient::GetInstance().GetCharacter()->ReqSlotTo ( emSlot );
				}

				/*rightclick wear/unwear, Juver, 2017/06/24 */
				if ( dwMsg & UIMSG_RB_UP )
				{
					EMSLOT emSlot = m_pPageWearEx->GetItemSlot ();

					SNATIVEID sid_item_hold = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
					if ( sid_item_hold != NATIVEID_NULL () )
					{
						BOOL bUse = GLGaeaClient::GetInstance().GetCharacter()->InvenUseToPutOn ( emSlot );
						if ( bUse )	return;
					}

					GLGaeaClient::GetInstance().GetCharacter()->ReqWearToInven ( emSlot );
					return;
				}
			}
		}break;

	case INVENTORY_MONEY_BUTTON:
		{
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

	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( MARKET_WINDOW );
				CInnerInterface::GetInstance().HideGroup ( STORAGE_WINDOW );
				CInnerInterface::GetInstance().HideGroup ( CLUB_STORAGE_WINDOW );
			}
		}break;

		/*inventory sort, Juver, 2018/07/07 */
	case INVENTORY_SORT_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
				GLGaeaClient::GetInstance().GetCharacter()->InventorySort();	
		}break;

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

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}

void CEP7InventoryWindow::SetMoney ( LONGLONG Money )
{
	//	돈자리수
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}


void CEP7InventoryWindow::GetSplitPos ( WORD* pwPosX, WORD* pwPosY )
{
	*pwPosX = m_wSplitItemPosX;
	*pwPosY = m_wSplitItemPosY;
}

void CEP7InventoryWindow::SetSplitPos ( WORD wPosX, WORD wPosY )
{
	m_wSplitItemPosX = wPosX;
	m_wSplitItemPosY = wPosY;
}

void CEP7InventoryWindow::SetOneViewSlot ( const int& nONE_VIEW_SLOT )
{
}

void CEP7InventoryWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenInvenPage(EP7_INVENTORY_PAGE_INVEN_1);
		
		if( m_pPageWearEx )
		{
			if( GLGaeaClient::GetInstance().GetCharacter()->IsUseArmSub() )
			{
				m_pPageWearEx->SetTabButton( FALSE );
			}
			else
			{
				m_pPageWearEx->SetTabButton( TRUE );
			}

		}
	}
}

void CEP7InventoryWindow::SetArmSwapTabButton( BOOL bArmSub )
{
	if( m_pPageWearEx )
	{
		m_pPageWearEx->SetTabButton( bArmSub );
	}
}

void CEP7InventoryWindow::OpenInvenPage ( int nPage )
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
