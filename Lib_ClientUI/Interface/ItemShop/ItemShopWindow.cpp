#include "StdAfx.h"
#include "ItemShopWindow.h"
#include "ItemShopItemSlot.h"

#include "../Lib_Engine/Core/NSRGameGlobal.h"

#include "../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../Lib_ClientUI/Interface/BasicTextBoxEx.h"
#include "../Lib_ClientUI/Interface/BasicScrollBarEx.h"
#include "../Lib_ClientUI/Interface/BasicScrollThumbFrame.h"
#include "../Lib_ClientUI/Interface/BasicButton.h"
#include "../Lib_ClientUI/Interface/BasicTextButton.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"

#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLCrowData.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/ModalWindow.h"
#include "../Lib_ClientUI/Interface/ModalCallerID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CItemShopWindow::CItemShopWindow ()
	: m_pListTextBox(NULL)
	, m_pListScrollBar(NULL)
	, m_pButtonPurchase(NULL)
	, m_pButtonPrev(NULL)
	, m_pButtonNext(NULL)
	, m_pTextPage(NULL)
	, m_pTextStaticBattlePoint(NULL)
	, m_pTextStaticEPoint(NULL)
	, m_pTextCharBattlePoint(NULL)
	, m_pTextCharEPoint(NULL)
	, m_pBuyConfirm(NULL)
	, m_pBuyConfirmText(NULL)
	, m_nSelectIndexType(-1)
	, m_nCurrentPage(1)
	, m_sSelectItem(NATIVEID_NULL())
	, m_sCrow(NATIVEID_NULL())
	, m_wShopType(0)
	, m_nMaxPage(1)
	, bItemShopConfirm(true)
	, m_strItemNum ("")
	, m_pMidTitleText(NULL)
	, m_pLeftTitleText(NULL)
	, m_pLeftCharTitleText(NULL)

{
	for( int i=0; i<ITEM_SHOP_MAX_ITEM; ++i )
		m_pItemSlot[i] = NULL;
}

CItemShopWindow::~CItemShopWindow ()
{
}

void CItemShopWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite ( "NPCSHOP_WINDOW_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_SLOT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_PAGE_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_CATEGORY_LINE" );
	RegisterControl ( pBasicLineBox );

	/*pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_MONEY_LINE" );
	RegisterControl ( pBasicLineBox );*/

	m_pListTextBox = new CBasicTextBoxEx;
	m_pListTextBox->CreateSub ( this, "ITEM_SHOP_CATEGORY_TEXT", UI_FLAG_DEFAULT, ITEM_SHOP_SELECTION_TEXTBOX );
	m_pListTextBox->SetFont ( pFont9 );		
	m_pListTextBox->SetLineInterval ( 3.0f );
	m_pListTextBox->SetSensitive ( true );
	m_pListTextBox->SetLimitLine ( 10000 );
	RegisterControl ( m_pListTextBox );

	int nTotalLine = m_pListTextBox->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, ITEM_SHOP_SELECTION_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "ITEM_SHOP_CATEGORY_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );

	m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "ITEM_SHOP_PAGE_NUM_TEXT" );
	m_pTextPage->SetFont ( pFont9 );
	m_pTextPage->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPage->SetText( "0" );
	RegisterControl ( m_pTextPage );

	std::string strSlot[ITEM_SHOP_MAX_ITEM] =
	{
		"ITEM_SHOP_ITEM_LIST_NODE_LEFT00",
		"ITEM_SHOP_ITEM_LIST_NODE_LEFT01",
		"ITEM_SHOP_ITEM_LIST_NODE_LEFT02",
		"ITEM_SHOP_ITEM_LIST_NODE_LEFT03",
		"ITEM_SHOP_ITEM_LIST_NODE_LEFT04",
		"ITEM_SHOP_ITEM_LIST_NODE_LEFT05",
		"ITEM_SHOP_ITEM_LIST_NODE_RIGHT00",
		"ITEM_SHOP_ITEM_LIST_NODE_RIGHT01",
		"ITEM_SHOP_ITEM_LIST_NODE_RIGHT02",
		"ITEM_SHOP_ITEM_LIST_NODE_RIGHT03",
		"ITEM_SHOP_ITEM_LIST_NODE_RIGHT04",
		"ITEM_SHOP_ITEM_LIST_NODE_RIGHT05",
	};

	for( int i=0; i<ITEM_SHOP_MAX_ITEM; ++i )
	{
		m_pItemSlot[i] = new CItemShopItemSlot;
		m_pItemSlot[i]->CreateSub ( this, strSlot[i].c_str(), UI_FLAG_DEFAULT, ITEM_SHOP_ITEM00 + i );
		m_pItemSlot[i]->CreateSubControl ();
		m_pItemSlot[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pItemSlot[i] );
	}

	m_pButtonPrev = new CBasicButton;
	m_pButtonPrev->CreateSub ( this, "ITEM_SHOP_PRE_PAGE_BUTTON", UI_FLAG_DEFAULT, ITEM_SHOP_PREV_BUTTON );
	m_pButtonPrev->CreateFlip ( "ITEM_SHOP_PRE_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonPrev->CreateMouseOver ( "ITEM_SHOP_PRE_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonPrev );

	m_pButtonNext = new CBasicButton;
	m_pButtonNext->CreateSub ( this, "ITEM_SHOP_NEXT_PAGE_BUTTON", UI_FLAG_DEFAULT, ITEM_SHOP_NEXT_BUTTON );
	m_pButtonNext->CreateFlip ( "ITEM_SHOP_NEXT_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonNext->CreateMouseOver ( "ITEM_SHOP_NEXT_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonNext );
	
	m_pButtonPurchase = new CBasicTextButton;
	m_pButtonPurchase->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, ITEM_SHOP_PURCHASE_BUTTON );
	m_pButtonPurchase->CreateBaseButton ( "ITEM_SHOP_BUTTON_PURCHASE", CBasicTextButton::SIZE40, CBasicButton::CLICK_FLIP, "aw" );
	RegisterControl ( m_pButtonPurchase );

	m_pBuyConfirm = new CBasicButton;
	m_pBuyConfirm->CreateSub ( this, "ITEM_SHOP_CONFIRM_BUTTON", UI_FLAG_DEFAULT, ITEM_SHOP_CONFIRM_BUTTON );
	m_pBuyConfirm->CreateFlip ( "ITEM_SHOP_CONFIRM_BUTTON_ON", CBasicButton::RADIO_FLIP );
	m_pBuyConfirm->SetControlNameEx ( "ITEM_SHOP_CONFIRM_BUTTON" );
	m_pBuyConfirm->SetFlip( bItemShopConfirm );
	RegisterControl ( m_pBuyConfirm );
	
	m_pBuyConfirmText = new CBasicTextBox;
	m_pBuyConfirmText->CreateSub ( this, "ITEM_SHOP_CONFIRM_BUTTON_TEXT" );
	m_pBuyConfirmText->SetFont ( pFont9 );
	m_pBuyConfirmText->SetText( ID2GAMEWORD( "ITEM_SHOP_TEXT", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pBuyConfirmText );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_CATEGORY_LINE_TITLE" );
	RegisterControl ( pBasicLineBox );

	m_pMidTitleText = new CBasicTextBox;
	m_pMidTitleText->CreateSub ( this, "ITEM_SHOP_MID_TITLE_TEXT" );
	m_pMidTitleText->SetFont ( pFont9 );
	m_pMidTitleText->SetText( ID2GAMEWORD( "ITEM_SHOP_TEXT", 8 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pMidTitleText );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_ACCCOUNT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_ACCCOUNT_LINE_TITLE" );
	RegisterControl ( pBasicLineBox );

	m_pLeftTitleText = new CBasicTextBox;
	m_pLeftTitleText->CreateSub ( this, "ITEM_SHOP_ACCOUNT_TITLE_TEXT" );
	m_pLeftTitleText->SetFont ( pFont9 );
	m_pLeftTitleText->SetText( ID2GAMEWORD( "ITEM_SHOP_TEXT", 9 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pLeftTitleText );

	m_pTextStaticBattlePoint = new CBasicTextBox;
	m_pTextStaticBattlePoint->CreateSub ( this, "ITEM_SHOP_BP_STATIC" );
	m_pTextStaticBattlePoint->SetFont ( pFont9 );
	m_pTextStaticBattlePoint->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextStaticBattlePoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextStaticBattlePoint );

	m_pTextStaticEPoint = new CBasicTextBox;
	m_pTextStaticEPoint->CreateSub ( this, "ITEM_SHOP_EP_STATIC" );
	m_pTextStaticEPoint->SetFont ( pFont9 );
	m_pTextStaticEPoint->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextStaticEPoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextStaticEPoint );

	m_pTextCharBattlePoint = new CBasicTextBox;
	m_pTextCharBattlePoint->CreateSub ( this, "ITEM_SHOP_BP_TEXT" );
	m_pTextCharBattlePoint->SetFont ( pFont9 );
	m_pTextCharBattlePoint->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	//m_pTextCharBattlePoint->SetText( "--" );
	RegisterControl ( m_pTextCharBattlePoint );

	m_pTextCharEPoint = new CBasicTextBox;
	m_pTextCharEPoint->CreateSub ( this, "ITEM_SHOP_EP_TEXT" );
	m_pTextCharEPoint->SetFont ( pFont9 );
	m_pTextCharEPoint->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextCharEPoint->SetText( "--" );
	RegisterControl ( m_pTextCharEPoint );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_CHAR_RENDER_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "ITEM_SHOP_CHAR_RENDER_LINE_TITLE" );
	RegisterControl ( pBasicLineBox );

	m_pLeftCharTitleText = new CBasicTextBox;
	m_pLeftCharTitleText->CreateSub ( this, "ITEM_SHOP_CHAR_RENDER_TITLE_TEXT" );
	m_pLeftCharTitleText->SetFont ( pFont9 );
	m_pLeftCharTitleText->SetText( ID2GAMEWORD( "ITEM_SHOP_TEXT", 10 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pLeftCharTitleText );

	m_pPreview = new CGameItemShopPagePreview;
	m_pPreview->CreateSub(this, "ITEM_SHOP_PAGE_PREVIEW", UI_FLAG_DEFAULT, ITEM_SHOP_PAGE_RENDER);
	m_pPreview->CreateSubControl();
	m_pPreview->SetVisibleSingle(TRUE); 
	RegisterControl(m_pPreview);
	
}

void CItemShopWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListTextBox->SetUseOverColor ( FALSE );
	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndexType )
		m_pListTextBox->SetUseTextColor ( m_nSelectIndexType, FALSE );

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndexType )
	{
		m_pListTextBox->SetUseTextColor ( m_nSelectIndexType, TRUE );
		m_pListTextBox->SetTextColor ( m_nSelectIndexType, NS_UITEXTCOLOR::ORANGE );
	}

	CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();
	const int nTotalLine = m_pListTextBox->GetTotalLine ();
	const int nLinePerOneView = m_pListTextBox->GetVisibleLine ();
	pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
	if ( nLinePerOneView < nTotalLine )
	{
		const int nMovableLine = nTotalLine - nLinePerOneView;
		float fPercent = pThumbFrame->GetPercent ();
		int nPos = (int)floor(fPercent * nMovableLine);
		m_pListTextBox->SetCurLine ( nPos );			
	}
	if ( m_pTextCharBattlePoint )
	{
		GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();

		m_pTextCharBattlePoint->ClearText();
		CString strMoney;
		strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( ref_CharData.m_dwVPoints, 3, "," );
		m_pTextCharBattlePoint->SetText( strMoney.GetString(), NS_UITEXTCOLOR::DARKORANGE );
	}

	if ( m_pTextCharEPoint )
	{
		GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();

		m_pTextCharEPoint->ClearText();
		CString strMoney;
		strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( ref_CharData.m_dwPPoints, 3, "," );
		m_pTextCharEPoint->SetText( strMoney.GetString(), NS_UITEXTCOLOR::DARKORANGE );
	}
}

void CItemShopWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( ITEMSHOP_WINDOW );
			}
		}break;
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqItemShopOpen( false );
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}
		break;
	
	case ITEM_SHOP_ITEM00:
	case ITEM_SHOP_ITEM01:
	case ITEM_SHOP_ITEM02:
	case ITEM_SHOP_ITEM03:
	case ITEM_SHOP_ITEM04:
	case ITEM_SHOP_ITEM05:
	case ITEM_SHOP_ITEM06:
	case ITEM_SHOP_ITEM07:
	case ITEM_SHOP_ITEM08:
	case ITEM_SHOP_ITEM09:
	case ITEM_SHOP_ITEM10:
	case ITEM_SHOP_ITEM11:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - ITEM_SHOP_ITEM00;
				if ( m_pItemSlot[nSelect] )
				{
					SITEMCUSTOM sitemcustom;
					ITEMSHOP sshop_item = m_pItemSlot[nSelect]->ItemGet();
					sitemcustom.sNativeID = sshop_item.sID;
					SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
					if ( pitem_data )
					{
						if ( pitem_data->ISPILE() )
						sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

						sitemcustom.cDAMAGE = (BYTE)pitem_data->sBasicOp.wGradeAttack;
						sitemcustom.cDEFENSE = (BYTE)pitem_data->sBasicOp.wGradeDefense;

						if ( UIMSG_ITEM_SHOP_SLOT_ITEM_IMAGE & dwMsg )
						{
							CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL() );
						}
					}

					if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
					{
						SelectItem(nSelect);
						m_pPreview->DataShow(sitemcustom.sNativeID); // Add this for preview
					}
					

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
				}
			}
		}break;

	case ITEM_SHOP_PREV_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_nCurrentPage --;
					if ( m_nCurrentPage < 1 )
						m_nCurrentPage = 1;
					ShowItem( m_nCurrentPage );
				}
			}
		}break;

	case ITEM_SHOP_NEXT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					if ( m_nCurrentPage >= m_nMaxPage )	return;

					m_nCurrentPage ++;
					ShowItem( m_nCurrentPage );
				}
			}
		}break;

	case ITEM_SHOP_SELECTION_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pListTextBox->GetSelectPos ();
				if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0  ) return ;

				if ( UIMSG_LB_UP & dwMsg )
				{
					DWORD dwType = m_pListTextBox->GetTextData ( nIndex );
					m_nSelectIndexType = nIndex;
					SelectType( dwType );
				}

				m_pListTextBox->SetUseOverColor ( TRUE );
				m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
			}
		}
		break;

	case ITEM_SHOP_CONFIRM_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bItemShopConfirm = !RANPARAM::bItemShopConfirm;
				bItemShopConfirm = !bItemShopConfirm;
				m_pBuyConfirm->SetFlip( bItemShopConfirm );
			}
		}break;

	case ITEM_SHOP_PURCHASE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( bItemShopConfirm )
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sSelectItem );
					if ( pItem )
					{
						CString strQuestion = CInnerInterface::GetInstance().MakeString( ID2GAMEINTEXT("NPCSHOP_PURCHASE_QUESTION"), pItem->GetName() );
						DoModal( strQuestion.GetString(),  MODAL_INFOMATION, YESNO, MODAL_BUYITEM );	
						return;
					}
				}
				else
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sSelectItem );
					if ( pItem )
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqBuyItem( m_strItemNum );
						return;
					}
				}
			}
		}break;
	};
}

void CItemShopWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
	else
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqItemShopOpen( false );
	}
}

void CItemShopWindow::InitShop()
{
	ResetShop();

	/*SCROWDATA* pcrow = GLCrowDataMan::GetInstance().GetCrowData( sidCrow );
	if ( !pcrow )	return;

	SITEM_SHOP& npc_shop = pcrow->m_sItemShop;

	if ( npc_shop.strTitle.size() )
		SetTitleName( npc_shop.strTitle.c_str() );*/

	if ( m_pTextCharBattlePoint )
	{
		m_pTextCharBattlePoint->ClearText();
		m_pTextCharBattlePoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 0 ), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextCharEPoint )
	{
		m_pTextCharEPoint->ClearText();
		m_pTextCharEPoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 1 ), NS_UITEXTCOLOR::WHITE );
	}

	//copy data
	//m_mapType = npc_shop.mapType;
	//m_mapItem = npc_shop.mapItem;
	//m_wShopType = npc_shop.wShopType;
	//m_sCrow = pcrow->sNativeID;

	/*for( SITEM_SHOP_TYPE_MAP_ITER iter = m_mapType.begin();
		iter != m_mapType.end(); ++iter )
	{
		SITEM_SHOP_TYPE& shop_type = (*iter).second;

		CString strTemp;
		strTemp.Format( "%s(%d)", shop_type.strName.c_str(), shop_type.wItemNum );
		int nIndex = m_pListTextBox->AddText ( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
		m_pListTextBox->SetTextData ( nIndex, shop_type.wID );
	}*/
	m_mapItem = GLGaeaClient::GetInstance().GetCharacter()->m_mapItemShop;
	DWORD dwCount = m_mapItem.size();
	
	for( int i = 0; i < ITEM_SHOP_MAX_CATEGORY; ++i )
	{
		CString strTemp;

		if (i == 0)
		{
			strTemp.Format("%s(%d)", ID2GAMEWORD("ITEMSHOP_MENU_BUTTON", i), dwCount);
		}
		else
		{
			strTemp.Format("%s(%d)", ID2GAMEWORD("ITEMSHOP_MENU_BUTTON", i), GLGaeaClient::GetInstance().GetCharacter()->m_cInvenItemShop[i].GetNumItems());
		}
		
		int nIndex = m_pListTextBox->AddText ( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
		m_pListTextBox->SetTextData ( nIndex, i );
	}

	//select first line 
	if ( m_pListTextBox->GetCount () > 0 )
	{
		m_pListTextBox->SetSelectPos( 0 );
		int nIndex = m_pListTextBox->GetSelectPos ();
		if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0  ) return;
		DWORD dwType = m_pListTextBox->GetTextData ( nIndex );
		m_nSelectIndexType = nIndex;
		SelectType( dwType );
		m_pListTextBox->SetUseOverColor ( TRUE );
		m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
	}
}

void CItemShopWindow::ResetShop()
{
	//m_mapType.clear();
	m_mapItem.clear();
	m_vecItem.clear();
	m_wShopType = 0;
	m_strItemNum = "";

	m_nCurrentPage = 1;
	m_nSelectIndexType = -1;
	m_sSelectItem = NATIVEID_NULL();
	m_sCrow = NATIVEID_NULL();
	m_nMaxPage = 1;

	for ( int i=0; i<ITEM_SHOP_MAX_ITEM; ++i )
	{
		m_pItemSlot[i]->ItemReset();
		m_pItemSlot[i]->SetVisibleSingle( FALSE );
	}

	m_pListTextBox->ClearText();

	if ( m_pButtonPurchase )
		m_pButtonPurchase->SetOneLineText( ID2GAMEWORD( "ITEM_SHOP_TEXT", 1 ) );

	if ( m_pTextCharBattlePoint )
	{
		m_pTextCharBattlePoint->ClearText();
		m_pTextCharBattlePoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 0 ), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextCharEPoint )
	{
		m_pTextCharEPoint->ClearText();
		m_pTextCharEPoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 1 ), NS_UITEXTCOLOR::WHITE );
	}

	if (m_pPreview)
        m_pPreview->DataReset();  // Clear preview data on reset

	SetTitleName( ID2GAMEWORD( "ITEM_SHOP_TEXT", 0 ) );
}

void CItemShopWindow::SelectType( DWORD dwType )
{
	m_vecItem.clear();
	m_nCurrentPage = 1;

	for( ITEMMAPSHOP_ITER iter = m_mapItem.begin();
		iter != m_mapItem.end(); ++iter )
	{
		ITEMSHOP& shop_item = (*iter).second;
		if ( dwType != 0 )	
		{
			if ( shop_item.wItemCtg != dwType )	continue;
			if ( shop_item.wItemCurrency <= 0 || shop_item.wItemCurrency > 2 )	continue;
		}
		m_vecItem.push_back( shop_item );
	}

	std::sort( m_vecItem.begin(), m_vecItem.end() );

	m_nMaxPage = 1 + ( (int)m_vecItem.size() / ITEM_SHOP_MAX_ITEM );

	ShowItem( m_nCurrentPage );
}

void CItemShopWindow::ShowItem( int nPage )
{
	for ( int i=0; i< ITEM_SHOP_MAX_ITEM; ++ i )
	{
		m_pItemSlot[i]->ItemReset();
		m_pItemSlot[i]->SetVisibleSingle( FALSE );
	}

	SelectItem( ITEM_SHOP_MAX_ITEM );

	if ( nPage < 1 )	return;

	int nEnd = nPage * ITEM_SHOP_MAX_ITEM;
	int nStart = nEnd - ITEM_SHOP_MAX_ITEM;
	int nSlot = 0;

	for ( int i=nStart; i<nEnd; ++i )
	{
		if ( i >= m_vecItem.size() )	continue;

		if ( m_pItemSlot[nSlot] )
		{
			m_pItemSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pItemSlot[nSlot]->ItemSet( m_vecItem[i] );
			nSlot ++;
		}
	}

	if ( m_pTextPage )
	{
		CString strTemp;
		strTemp.Format( "%d", nPage );
		m_pTextPage->ClearText();
		m_pTextPage->SetText( strTemp.GetString() );
	}
}

void CItemShopWindow::SelectItem( int nSelect )
{
	for ( int i=0; i< ITEM_SHOP_MAX_ITEM; ++ i )
	{
		if ( m_pItemSlot[i] )
			m_pItemSlot[i]->ItemSelect( FALSE );
	}

	if ( nSelect >= 0 && nSelect < ITEM_SHOP_MAX_ITEM )
	{
		if ( m_pItemSlot[nSelect] )
		{
			if ( m_pItemSlot[nSelect]->IsVisible() )
				m_pItemSlot[nSelect]->ItemSelect( TRUE );

			m_sSelectItem = m_pItemSlot[nSelect]->ItemGet().sID;

			if (m_pPreview)
                m_pPreview->DataShow(m_sSelectItem);

			m_strItemNum = m_pItemSlot[nSelect]->ItemGet().strItemNum;

			m_wShopType = m_pItemSlot[nSelect]->ItemGet().wItemCurrency - 1;

	if ( m_pTextCharBattlePoint )
			{
				m_pTextCharBattlePoint->ClearText();
				m_pTextCharBattlePoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 0 ), NS_UITEXTCOLOR::WHITE );
			}

			if ( m_pTextCharEPoint )
			{
				m_pTextCharEPoint->ClearText();
				m_pTextCharEPoint->SetText( ID2GAMEWORD( "ITEM_SHOP_TYPE", 1 ), NS_UITEXTCOLOR::WHITE );
			}

			DWORD dwPrice = 0;
			SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sSelectItem );
			if ( pItem )
			{
				CString strTemp;
				strTemp.Format( "%s %s", ID2GAMEWORD( "ITEM_SHOP_TEXT", 1 ), NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)m_pItemSlot[nSelect]->ItemGet().wItemPrice, 3, "," ).GetString() );
				strTemp += ID2GAMEWORD("ITEM_SHOP_TYPE",m_pItemSlot[nSelect]->ItemGet().wItemCurrency-1);

				if ( m_pButtonPurchase )	
					m_pButtonPurchase->SetOneLineText( strTemp.GetString() );	
			}
		}
	}
	else
	{
		m_sSelectItem = NATIVEID_NULL();

		if ( m_pButtonPurchase )
			m_pButtonPurchase->SetOneLineText( ID2GAMEWORD( "ITEM_SHOP_TEXT", 1 ) );

		 if (m_pPreview)
            m_pPreview->DataReset(); // Clear preview if nothing selected
	}

}