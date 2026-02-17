#include "StdAfx.h"
#include "./ModernMarketWindow.h"
#include "./ModernMarketPage.h"

#include "./BasicTextButton.h"
#include "./BasicLineBox.h"
#include "./MultiModeButton.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./ItemImage.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"
#include "./ModalWindow.h"
#include "./ModalCallerID.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/Core/NSRProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	int	CModernMarketWindow::nOUTOFRANGE = -1;

CModernMarketWindow::CModernMarketWindow ()
	: m_nPageIndex ( nOUTOFRANGE )

	, m_pRecommendText( NULL )

	, m_pSettingCheckBoxUsable(NULL)
	, m_pSettingCheckBoxBuyable(NULL)
	, m_pSettingTextUsable(NULL)
	, m_pSettingTextBuyable(NULL)

	, m_pMoneyText(NULL)
{
	memset ( m_pPage, 0, sizeof ( CModernMarketPage* ) * nMAXPAGE );
	memset ( m_pPageButton, 0, sizeof ( CBasicTextButton* ) * nMAXPAGE );	

	for ( int i=0; i<RECOMMEND_ITEM_TOTAL; ++ i )
	{
		m_pRecommendItemText[i] = NULL;
		m_pRecommendItemImage[i] = NULL;
		m_sItemRecommend[i] = SINVENITEM();
	}
}

CModernMarketWindow::~CModernMarketWindow ()
{
}

void CModernMarketWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "MODERN_MARKET_WINDOW_BACKGROUND" );
	RegisterControl ( pControl );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhiteBlankBody ( "MODERN_MARKET_WINDOW_BACKGROUND_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_SELL_LIST_BACK" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_LIST_BACK" );
	RegisterControl ( pBasicLineBox );
	
	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_BACK_0" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_BACK_1" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_BACK_2" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_BACK_3" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_BACK_4" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_MARKET_CHECK_ITEM_TYPE_BACK" );
	RegisterControl ( pBasicLineBox );

	//pBasicLineBox = new CBasicLineBox;
	//pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_MARKET_WINDOW_CART_LIST_BACK" );
	//RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MARKET_WINDOW_USABLE_ITEM_BACK" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MARKET_WINDOW_BUYABLE_ITEM_BACK" );
	RegisterControl ( pBasicLineBox );

	//pBasicLineBox = new CBasicLineBox;
	//pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MARKET_SELL_RIGHT_BUTTON_BACK" );
	//RegisterControl ( pBasicLineBox );

	//pBasicLineBox = new CBasicLineBox;
	//pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MARKET_MARKET_COFIRM_SELL_ITEM_BACK" );
	//RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_MARKET_WINDOW_MARKET_MONEY_BACK" );
	RegisterControl ( pBasicLineBox );

	CString strButtonKeyword[nMAXPAGE] = 
	{
		"MODERN_MARKET_WINDOW_ACTIVE_TAB0",
		"MODERN_MARKET_WINDOW_ACTIVE_TAB1",
		"MODERN_MARKET_WINDOW_ACTIVE_TAB2",
	};

	for ( int i = 0; i < nMAXPAGE; i++ )
	{
		m_pPage[i] = CreateMarketPage ( MARKET_PAGE0 + i );	
		m_pPageButton[i] = CreateTextButton ( strButtonKeyword[i].GetString (), MARKET_WINDOW_PAGE_BUTTON0 + i, (char*)ID2GAMEWORD ( "MARKET_WINDOW_PAGE_BUTTON", i ) );
	}

	m_pRecommendText = CreateTextBox( "MODERN_MARKET_WINDOW_MARKET_RECOMMEND_TITLE", TEXT_ALIGN_CENTER_X, pFont9 );
	m_pRecommendText->SetText( ID2GAMEINTEXT( "MODERN_MARKET_WINDOW_RECOMMENT_ITEM_TITLE" ), NS_UITEXTCOLOR::WHITE );

	std::string strRecommentItemText[RECOMMEND_ITEM_TOTAL] = 
	{
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_TEXT_0",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_TEXT_1",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_TEXT_2",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_TEXT_3",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_TEXT_4",
	};

	std::string strRecommentItemImage[RECOMMEND_ITEM_TOTAL] = 
	{
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_ITEM_0",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_ITEM_1",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_ITEM_2",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_ITEM_3",
		"MODERN_MARKET_WINDOW_MARKET_RECOMMEND_ITEM_4",
	};

	for( int i=0; i<RECOMMEND_ITEM_TOTAL; ++i )
	{
		m_pRecommendItemText[i] = new CBasicTextBox;
		m_pRecommendItemText[i]->CreateSub ( this, strRecommentItemText[i].c_str() );
		m_pRecommendItemText[i]->SetFont ( pFont9 );
		m_pRecommendItemText[i]->SetTextAlign( TEXT_ALIGN_LEFT );
		RegisterControl ( m_pRecommendItemText[i] );

		m_pRecommendItemImage[i] = new CItemImage;
		m_pRecommendItemImage[i]->CreateSub ( this, strRecommentItemImage[i].c_str(), UI_FLAG_DEFAULT, MARKET_WINDOW_RECOMMEND_ITEM_IMAGE0 +i );
		m_pRecommendItemImage[i]->CreateSubControl ();
		RegisterControl ( m_pRecommendItemImage[i] );
	}



	m_pSettingCheckBoxUsable = CreateFlipButton ( "MODERN_MARKET_WINDOW_USABLE_ITEM_CHECK", "MODERN_MARKET_WINDOW_USABLE_ITEM_UNCHECK", MARKET_WINDOW_SETTING_CHECK_BOX_USABLE );
	m_pSettingCheckBoxBuyable = CreateFlipButton ( "MODERN_MARKET_WINDOW_BUYABLE_ITEM_CHECK", "MODERN_MARKET_WINDOW_BUYABLE_ITEM_UNCHECK", MARKET_WINDOW_SETTING_CHECK_BOX_BUYABLE );

	m_pSettingTextUsable = CreateTextBox( "MODERN_MARKET_WINDOW_USABLE_ITEM_TEXT", TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, pFont9 );
	m_pSettingTextUsable->SetText( ID2GAMEINTEXT( "MODERN_MARKET_WINDOW_SETTING_USABLE_TEXT" ) );

	m_pSettingTextBuyable = CreateTextBox( "MODERN_MARKET_WINDOW_BUYABLE_ITEM_TEXT", TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y, pFont9 );
	m_pSettingTextBuyable->SetText( ID2GAMEINTEXT( "MODERN_MARKET_WINDOW_BUYABLE_TEXT" ) );

	m_pMoneyText = CreateTextBox( "MODERN_MARKET_WINDOW_MARKET_MONEY_TEXT", TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y, pFont9 );



	SetVisiblePage(MARKET_PAGE0);
}

CModernMarketPage* CModernMarketWindow::CreateMarketPage ( UIGUID ControlID )
{
	CModernMarketPage* pPage = new CModernMarketPage;
	pPage->CreateSub ( this, "MODERN_MARKET_WINDOW_MARKET_SELL_LIST", UI_FLAG_DEFAULT, ControlID );
	pPage->CreateSubControl ();
	RegisterControl ( pPage );
	return pPage;
}

CMultiModeButton*  CModernMarketWindow::CreateTextButton ( const char* szButton, UIGUID ControlID, const char* szText )
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID );
	pButton->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
	pButton->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
	pButton->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pButton->AlignToControl( szButton );
	pButton->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	pButton->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pButton );
	return pButton;
}

CBasicButton* CModernMarketWindow::CreateFlipButton ( const char* szButton, const char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );
	return pButton;
}

CBasicTextBox* CModernMarketWindow::CreateTextBox ( const char* szTextBox, DWORD dwTextAllign, CD3DFontPar* pFont)
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szTextBox );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign( dwTextAllign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

BOOL CModernMarketWindow::IsUsingMaketPage( int nPage )
{
	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( m_sMarketID );
	if ( !pCrowData )			return false;

	std::string strCrowSale = pCrowData->GetCrowSale( (DWORD)nPage );
	if ( strCrowSale.empty() )	return false;

	SCROWSALE_DATA* pCrowSale = GLCrowDataMan::GetInstance().CrowSaleFind( strCrowSale );
	if ( !pCrowSale )			return false;

	GLInventory& ref_Inventory = *pCrowSale->GetSaleInven();

	if( ref_Inventory.GetNumItems() > 0 )
		return true;
	else
		return false;
}

void CModernMarketWindow::SetVisiblePage ( int nPage )
{
	if ( nPage < MARKET_PAGE0 || MARKET_PAGE2 < nPage )
	{
		GASSERT ( 0 && "영역을 넘어서는 페이지입니다." );
		return ;
	}

	{	//	리셋
		for ( int i = 0; i < nMAXPAGE; i++ )
		{
			m_pPage[i]->SetVisibleSingle ( FALSE );
			m_pPageButton[i]->DoImageFlip ( FALSE );
		}
	}

	m_nPageIndex = nPage - MARKET_PAGE0;
	m_pPage[m_nPageIndex]->SetVisibleSingle ( TRUE );
	m_pPageButton[m_nPageIndex]->DoImageFlip ( TRUE );

	ShowRecommendItem( DWORD(m_nPageIndex) );
}

void CModernMarketWindow::InitMarket ( SNATIVEID sNativeID )
{
	m_sMarketID = sNativeID;

	//reset
	for ( int i = 0; i < nMAXPAGE; i++ )
	{
		m_pPage[i]->UnLoadItemPage ( (DWORD)i );
		m_pPageButton[i]->SetOneLineText ( "", NS_UITEXTCOLOR::WHITE );
	}

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( m_sMarketID );
	if ( pCrowData )
	{
		for ( int i = 0; i < nMAXPAGE; i++ )
		{
			std::string strCrowSale = pCrowData->GetCrowSale( i );
			if ( strCrowSale.empty() )	continue;

			SCROWSALE_DATA* pCrowSale = GLCrowDataMan::GetInstance().CrowSaleFind( strCrowSale );
			if ( !pCrowSale )			continue;

			GLInventory& ref_Inventory = *pCrowSale->GetSaleInven();
			m_pPage[i]->UnLoadItemPage ( (DWORD)i );
			m_pPage[i]->LoadItemPage ( ref_Inventory, m_sMarketID, (DWORD)i );

			//	추후에 수정
			CString strTabText = pCrowSale->GetSaleType().c_str();
			m_pPageButton[i]->SetOneLineText ( strTabText, NS_UITEXTCOLOR::WHITE );
		}
	}	

	if ( m_pSettingCheckBoxUsable )
		m_pSettingCheckBoxUsable->SetFlip( RPROFILE::bModernMarketWindowSettingUsable );

	if ( m_pSettingCheckBoxBuyable )
		m_pSettingCheckBoxBuyable->SetFlip( RPROFILE::bModernMarketWindowSettingBuyable );


	for( int i=0; i<RECOMMEND_ITEM_TOTAL; ++i )
	{
		if ( m_pRecommendItemText[i] )
		{
			m_pRecommendItemText[i]->ClearText();
		}

		if ( m_pRecommendItemImage[i] )
		{
			m_pRecommendItemImage[i]->ResetItem();
			m_pRecommendItemImage[i]->SetVisibleSingle( FALSE );
		}

		m_sItemRecommend[i] = SINVENITEM();
	}

	SetVisiblePage ( MARKET_PAGE0 );
}

void CModernMarketWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );


    if( ControlID == ET_CONTROL_TITLE || ET_CONTROL_TITLE_F == ControlID )
	{
		if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
		{
			CInnerInterface::GetInstance().SetDefaultPosInterface( MARKET_WINDOW );
			return ;
		}
	}	
	if ( ControlID == ET_CONTROL_BUTTON )
	{
		if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
		{
			CInnerInterface::GetInstance().SetMarketWindowClose ();
			return ;
		}
	}

	if ( MARKET_WINDOW_PAGE_BUTTON0 <= ControlID && ControlID < MARKET_WINDOW_PAGE_BUTTON_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			int nIndex = ControlID - MARKET_WINDOW_PAGE_BUTTON0;

			if ( dwMsg & UIMSG_LB_DOWN )
			{
				if ( IsUsingMaketPage( nIndex ) )
				{
					SetVisiblePage ( MARKET_PAGE0 + nIndex );
				}
				else
				{	
					return;
				}

			}
		}
	}
	else if ( MARKET_PAGE0 <= ControlID && ControlID < MARKET_PAGE_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			int nPageIndex = ControlID - MARKET_PAGE0;

			//	NOTE
			//		손에 들고 있는 아이템 처리
			if ( dwMsg & UIMSG_LB_UP || dwMsg & UIMSG_RB_UP )
			{
				SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
				if ( sNativeID != NATIVEID_NULL () )
				{
					//	NOTE
					//		물건 팔기
					SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
					if ( pItemData )
					{
						const std::string& strItemName = pItemData->GetName();
						CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("SELL_ITEM_CONFIRM"), strItemName.c_str() );
						DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_SELL_ITEM );
					}
					else
					{
						GASSERT ( 0 && "아이템을 찾을 수 없습니다." );
					}

					return ;
				}
			}

			int nPosX, nPosY;
			m_pPage[nPageIndex]->GetItemIndex ( &nPosX, &nPosY );			
			SetItemIndex ( nPosX, nPosY );
			if ( nPosX < 0 || nPosY < 0 ) return ;

//			if ( CUIMan::GetFocusControl () == this )
			SINVENITEM sInvenItem = m_pPage[nPageIndex]->GetItem ( nPosX, nPosY );
			{				
				if ( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sInvenItem.sItemCustom, TRUE, TRUE, FALSE, sInvenItem.wPosX, sInvenItem.wPosY, m_sMarketID,
						FALSE, 0, FALSE, FALSE, FALSE, FALSE, SNATIVEID(false), SNATIVEID(false), m_nPageIndex );
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
			}


			//	NOTE
			//		아이템 바로 사기
			if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL ) & DXKEY_PRESSED )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
					if ( sNativeID != NATIVEID_NULL () ) return ;

					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcTo ( m_sMarketID, m_nPageIndex, nPosX, nPosY );
					return ;
				}
			}

			if ( dwMsg & UIMSG_LB_UP )
			{
				SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM().sNativeID;
				if ( sNativeID != NATIVEID_NULL () ) return ;

				SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
				if ( pITEM )
				{
					// Note : 겹침이 가능하고 한개씩 파는 아이템의 경우 구입 수량 입력. ( 화살묶음, 부적묶음. )
					if( pITEM->ISPILE() && (pITEM->GETAPPLYNUM()==1) )
					{
						DoModal ( ID2GAMEINTEXT ( "BUY_PILE_ITEM" ), MODAL_INPUT, EDITBOX_NUMBER, MODAL_PILE_ITEM );
						return ;
					}
					else
					{
						//	NOTE
						//		물건 사기							
						const std::string& strItemName = pITEM->GetName();
						CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("BUY_ITEM_CONFIRM"), strItemName.c_str() );
						DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_BUY_ITEM );							
						return ;
					}
				}
			}
		}
	}

	switch( ControlID )
	{
	case MARKET_WINDOW_SETTING_CHECK_BOX_BUYABLE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RPROFILE::bModernMarketWindowSettingBuyable = !RPROFILE::bModernMarketWindowSettingBuyable;

				if ( m_pSettingCheckBoxBuyable )
					m_pSettingCheckBoxBuyable->SetFlip( RPROFILE::bModernMarketWindowSettingBuyable );
			}
		}break;

	case MARKET_WINDOW_SETTING_CHECK_BOX_USABLE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RPROFILE::bModernMarketWindowSettingUsable = !RPROFILE::bModernMarketWindowSettingUsable;

				if ( m_pSettingCheckBoxUsable )
					m_pSettingCheckBoxUsable->SetFlip( RPROFILE::bModernMarketWindowSettingUsable );
			}
		}break;

	case MARKET_WINDOW_RECOMMEND_ITEM_IMAGE0:
	case MARKET_WINDOW_RECOMMEND_ITEM_IMAGE1:
	case MARKET_WINDOW_RECOMMEND_ITEM_IMAGE2:
	case MARKET_WINDOW_RECOMMEND_ITEM_IMAGE3:
	case MARKET_WINDOW_RECOMMEND_ITEM_IMAGE4:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - MARKET_WINDOW_RECOMMEND_ITEM_IMAGE0;
				if ( nSelect < 0 || nSelect >= RECOMMEND_ITEM_TOTAL )	return;

				SINVENITEM sInvenItem = m_sItemRecommend[nSelect];
				if ( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sInvenItem.sItemCustom, TRUE, TRUE, FALSE, sInvenItem.wPosX, sInvenItem.wPosY, m_sMarketID,
						FALSE, 0, FALSE, FALSE, FALSE, FALSE, SNATIVEID(false), SNATIVEID(false), m_nPageIndex );

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sInvenItem.sItemCustom );
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
			}
		}break;
	}
}

void CModernMarketWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	SetMoney ( ref_CharData.m_lnMoney );
}

void CModernMarketWindow::GetItemIndex ( int* pnPosX, int* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CModernMarketWindow::SetItemIndex ( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

int	 CModernMarketWindow::GetPageIndex ()
{
	return m_nPageIndex;
}

SNATIVEID	CModernMarketWindow::GetMarketID ()
{
	return m_sMarketID;
}

SINVENITEM&	CModernMarketWindow::GetItem ( int nPosX, int nPosY )
{
	return m_pPage[m_nPageIndex]->GetItem ( nPosX, nPosY );
}

void CModernMarketWindow::ShowRecommendItem( DWORD dwChannel )
{
	for( int i=0; i<RECOMMEND_ITEM_TOTAL; ++i )
	{
		if ( m_pRecommendItemText[i] )
		{
			m_pRecommendItemText[i]->ClearText();
		}

		if ( m_pRecommendItemImage[i] )
		{
			m_pRecommendItemImage[i]->ResetItem();
			m_pRecommendItemImage[i]->SetVisibleSingle( FALSE );
		}

		m_sItemRecommend[i] = SINVENITEM();
	}

	WORD dwHP = GLGaeaClient::GetInstance().GetCharacter()->m_sHP.wNow;
	WORD dwHPMAX = GLGaeaClient::GetInstance().GetCharacter()->m_sHP.wMax;
	WORD dwMP = GLGaeaClient::GetInstance().GetCharacter()->m_sMP.wNow;
	WORD dwMPMAX = GLGaeaClient::GetInstance().GetCharacter()->m_sMP.wMax;
	WORD dwSP = GLGaeaClient::GetInstance().GetCharacter()->m_sSP.wNow;
	WORD dwSPMAX = GLGaeaClient::GetInstance().GetCharacter()->m_sSP.wMax;

	BOOL bFULHP = !( dwHP < dwHPMAX );
	BOOL bFULMP = !( dwMP < dwMPMAX );
	BOOL bFULSP = !( dwSP < dwSPMAX );

	EMSLOT emRHand = GLGaeaClient::GetInstance().GetCharacter()->GetCurRHand();
	SNATIVEID sRHANDID;
	if ( GLGaeaClient::GetInstance().GetCharacter()->VALID_SLOT_ITEM(emRHand) )	
	{
		sRHANDID = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM(emRHand).sNativeID;
	}

	LONGLONG llCharacterMoney = GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney;

	int nRecommentIndex = 0;

	for ( int y = 0; y < SALE_INVEN_Y; y++ )
	{
		for ( int x = 0; x < SALE_INVEN_X; x++ )
		{
			if ( nRecommentIndex >=RECOMMEND_ITEM_TOTAL )	continue;

			SINVENITEM sInvenItem = m_pPage[m_nPageIndex]->GetItem ( x, y );

			SITEM* pItem = GLItemMan::GetInstance().GetItem( sInvenItem.sItemCustom.sNativeID );

			if ( pItem )
			{
				if ( !GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM( pItem->sBasicOp.sNativeID ) )	continue;

				if ( pItem->sBasicOp.emItemType == ITEM_CURE )
				{
					if ( bFULHP && bFULMP && bFULSP )	continue;

					switch ( pItem->sDrugOp.emDrug )
					{
					case ITEM_DRUG_HP:
						{
							if ( !bFULHP )
							{
								m_sItemRecommend[nRecommentIndex] = sInvenItem;
								nRecommentIndex ++;
							}
						}break;

					case ITEM_DRUG_MP:
						{
							if ( !bFULMP )
							{
								m_sItemRecommend[nRecommentIndex] = sInvenItem;
								nRecommentIndex ++;
							}
						}break;

					case ITEM_DRUG_SP:
						{
							if ( !bFULSP )
							{
								m_sItemRecommend[nRecommentIndex] = sInvenItem;
								nRecommentIndex ++;
							}
						}break;
					};
				}
				else if ( pItem->sBasicOp.emItemType == ITEM_ARROW )
				{
					SITEM* pItemRHAND = GLItemMan::GetInstance().GetItem( sRHANDID );
					if ( pItemRHAND )
					{
						if ( pItemRHAND->sSuitOp.emAttack==ITEMATT_BOW )
						{
							m_sItemRecommend[nRecommentIndex] = sInvenItem;
							nRecommentIndex ++;
						}
					}
				}
				else if ( pItem->sBasicOp.emItemType == ITEM_CHARM )
				{
					SITEM* pItemRHAND = GLItemMan::GetInstance().GetItem( sRHANDID );
					if ( pItemRHAND )
					{
						if ( pItemRHAND->sSuitOp.emAttack==ITEMATT_SPEAR )
						{
							m_sItemRecommend[nRecommentIndex] = sInvenItem;
							nRecommentIndex ++;
						}
					}
				}
				else if ( pItem->sBasicOp.emItemType == ITEM_BULLET )
				{
					SITEM* pItemRHAND = GLItemMan::GetInstance().GetItem( sRHANDID );
					if ( pItemRHAND )
					{
						if ( pItemRHAND->sSuitOp.emAttack==ITEMATT_GUN ||
							pItemRHAND->sSuitOp.emAttack==ITEMATT_RAILGUN ||
							pItemRHAND->sSuitOp.emAttack==ITEMATT_PORTALGUN )	
						{
							m_sItemRecommend[nRecommentIndex] = sInvenItem;
							nRecommentIndex ++;
						}
					}
				}else{
					m_sItemRecommend[nRecommentIndex] = sInvenItem;
					nRecommentIndex ++;
				}
			}
		}
	}

	for ( int i=0; i<RECOMMEND_ITEM_TOTAL; i++ )
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sItemRecommend[i].sItemCustom.sNativeID );
		if ( pItem )
		{
			m_pRecommendItemImage[i]->SetVisibleSingle( TRUE );
			m_pRecommendItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
			m_pRecommendItemText[i]->AddText( pItem->GetName() );

			LONGLONG llTotalPrice = 0;
			LONGLONG dwNpcSellPrice = 0;

			WORD wNum = 1;

			if( pItem->ISPILE() && (pItem->GETAPPLYNUM()==1) )
			{
				wNum = m_sItemRecommend[i].sItemCustom.wTurnNum;
				if ( wNum == 0 )	wNum = 1;
			}

			volatile LONGLONG dwPrice = 0;
			volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
			volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;

			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( m_sMarketID );
			if( pCrowData != NULL )
			{
				std::string strCrowSale = pCrowData->GetCrowSale( dwChannel );
				if ( strCrowSale.size() )
				{
					SCROWSALE_DATA* pCrowSale = GLCrowDataMan::GetInstance().CrowSaleFind( strCrowSale );
					if ( pCrowSale )
					{
						LONGLONG dwNpcPrice = pCrowSale->GetNpcSellPrice( pItem->sBasicOp.sNativeID.dwID );
						if( dwNpcPrice == 0 )
						{								
							dwNpcSellPrice = pItem->sBasicOp.dwBuyPrice;
							dwPrice = DWORD ( (float)dwNpcSellPrice * fSHOP_RATE_C );
						}else{
							dwNpcSellPrice = dwNpcPrice;
							dwPrice = dwNpcSellPrice;								
						}
					}
				}

				llTotalPrice += dwPrice * wNum;
			}

			BOOL bCanBuy = llCharacterMoney >= llTotalPrice;
			CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( llTotalPrice, 3, "," );
			m_pRecommendItemText[i]->AddText( strMoney.GetString(), bCanBuy? NS_UITEXTCOLOR::ENABLE : NS_UITEXTCOLOR::RED );
		}	
	}
}

void CModernMarketWindow::SetMoney ( LONGLONG Money )
{
	if ( m_pMoneyText && m_pMoneyText->IsVisible() )
	{
		CString strTemp;
		strTemp.Format( "%s %s",ID2GAMEINTEXT( "MODERN_MARKET_WINDOW_CUR_MONEY" ),  NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," ) );
		m_pMoneyText->SetOneLineText ( strTemp );
	}
}