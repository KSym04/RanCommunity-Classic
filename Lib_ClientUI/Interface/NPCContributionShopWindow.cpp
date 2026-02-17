#include "StdAfx.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxInputDevice.h"
#include "../../Lib_Engine/Core/NSRGameGlobal.h"

#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLCrowData.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicLineBox.h"
#include "./BasicVarTextBoxEx.h"
#include "./BasicTextBoxEx.h"
#include "./BasicScrollBar.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextButton.h"

#include "./InnerInterface.h"

#include "./NPCContributionShopItemSlot.h"
#include "./NPCContributionShopWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CNPCContributionShopWindow::CNPCContributionShopWindow ()
	: m_pListTextBox(NULL)
	, m_pListScrollBar(NULL)
	, m_pTextPage(NULL)
	, m_pTextStaticContribution(NULL)
	, m_pTextCharContribution(NULL)
	, m_pTextStaticMoney(NULL)
	, m_pTextCharMoney(NULL)
	, m_pButtonPurchase(NULL)
	, m_pButtonPrev(NULL)
	, m_pButtonNext(NULL)
	, m_pBuyConfirm(NULL)
	, m_pBuyConfirmText(NULL)

	, m_nSelectIndexType(-1)
	, m_nCurrentPage(1)
	, m_sSelectItem(NATIVEID_NULL())
	, m_sCrow(NATIVEID_NULL())
	, m_nMaxPage(1)
{
	for( int i=0; i<NPC_CONTRIBUTION_SHOP_MAX_ITEM; ++i )
		m_pItemSlot[i] = NULL;
}

CNPCContributionShopWindow::~CNPCContributionShopWindow ()
{
}

void CNPCContributionShopWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite ( "NPC_CONTRIBUTION_SHOP_WINDOW_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "NPC_CONTRIBUTION_SHOP_SLOT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "NPC_CONTRIBUTION_SHOP_PAGE_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "NPC_CONTRIBUTION_SHOP_CATEGORY_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "NPC_CONTRIBUTION_SHOP_MONEY_LINE" );
	RegisterControl ( pBasicLineBox );

	m_pListTextBox = new CBasicTextBoxEx;
	m_pListTextBox->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_CATEGORY_TEXT", UI_FLAG_DEFAULT, NPC_CONTRIBUTION_SHOP_SELECTION_TEXTBOX );
	m_pListTextBox->SetFont ( pFont9 );		
	m_pListTextBox->SetLineInterval ( 3.0f );
	m_pListTextBox->SetSensitive ( true );
	m_pListTextBox->SetLimitLine ( 10000 );
	RegisterControl ( m_pListTextBox );

	int nTotalLine = m_pListTextBox->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, NPC_CONTRIBUTION_SHOP_SELECTION_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "NPC_CONTRIBUTION_SHOP_CATEGORY_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );

	m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_PAGE_NUM_TEXT" );
	m_pTextPage->SetFont ( pFont9 );
	m_pTextPage->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPage->SetText( "0" );
	RegisterControl ( m_pTextPage );

	m_pTextStaticContribution = new CBasicTextBox;
	m_pTextStaticContribution->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_MONEY_STATIC" );
	m_pTextStaticContribution->SetFont ( pFont9 );
	m_pTextStaticContribution->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextStaticContribution->SetText( ID2GAMEWORD( "NPC_CONTRIBUTION_SHOP_TEXT", 3 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextStaticContribution );

	m_pTextCharContribution = new CBasicTextBox;
	m_pTextCharContribution->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_MONEY_TEXT" );
	m_pTextCharContribution->SetFont ( pFont9 );
	m_pTextCharContribution->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextCharContribution->SetText( "--" );
	RegisterControl ( m_pTextCharContribution );

	m_pTextStaticMoney = new CBasicTextBox;
	m_pTextStaticMoney->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_MONEY2_STATIC" );
	m_pTextStaticMoney->SetFont ( pFont9 );
	m_pTextStaticMoney->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextStaticMoney->SetText( ID2GAMEWORD( "NPC_CONTRIBUTION_SHOP_TEXT", 4 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextStaticMoney );

	m_pTextCharMoney = new CBasicTextBox;
	m_pTextCharMoney->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_MONEY2_TEXT" );
	m_pTextCharMoney->SetFont ( pFont9 );
	m_pTextCharMoney->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextCharMoney->SetText( "--" );
	RegisterControl ( m_pTextCharMoney );


	std::string strSlot[NPC_CONTRIBUTION_SHOP_MAX_ITEM] =
	{
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT00",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT01",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT02",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT03",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT04",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT05",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT06",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT07",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT08",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_LEFT09",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT00",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT01",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT02",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT03",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT04",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT05",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT06",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT07",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT08",
		"NPC_CONTRIBUTION_SHOP_ITEM_LIST_NODE_RIGHT09",
	};

	for( int i=0; i<NPC_CONTRIBUTION_SHOP_MAX_ITEM; ++i )
	{
		m_pItemSlot[i] = new CNPCContributionShopItemSlot;
		m_pItemSlot[i]->CreateSub ( this, strSlot[i].c_str(), UI_FLAG_DEFAULT, NPC_CONTRIBUTION_SHOP_ITEM00 + i );
		m_pItemSlot[i]->CreateSubControl ();
		m_pItemSlot[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pItemSlot[i] );
	}

	m_pButtonPrev = new CBasicButton;
	m_pButtonPrev->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_PRE_PAGE_BUTTON", UI_FLAG_DEFAULT, NPC_CONTRIBUTION_SHOP_PREV_BUTTON );
	m_pButtonPrev->CreateFlip ( "NPC_CONTRIBUTION_SHOP_PRE_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonPrev->CreateMouseOver ( "NPC_CONTRIBUTION_SHOP_PRE_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonPrev );

	m_pButtonNext = new CBasicButton;
	m_pButtonNext->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_NEXT_PAGE_BUTTON", UI_FLAG_DEFAULT, NPC_CONTRIBUTION_SHOP_NEXT_BUTTON );
	m_pButtonNext->CreateFlip ( "NPC_CONTRIBUTION_SHOP_NEXT_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonNext->CreateMouseOver ( "NPC_CONTRIBUTION_SHOP_NEXT_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonNext );

	m_pButtonPurchase = new CBasicTextButton;
	m_pButtonPurchase->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, NPC_CONTRIBUTION_SHOP_PURCHASE_BUTTON );
	m_pButtonPurchase->CreateBaseButton ( "NPC_CONTRIBUTION_SHOP_BUTTON_PURCHASE", CBasicTextButton::SIZE40, CBasicButton::CLICK_FLIP, "0x0BADF00D" );
	RegisterControl ( m_pButtonPurchase );

	m_pBuyConfirm = new CBasicButton;
	m_pBuyConfirm->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON", UI_FLAG_DEFAULT, NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON );
	m_pBuyConfirm->CreateFlip ( "NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON_ON", CBasicButton::RADIO_FLIP );
	m_pBuyConfirm->SetControlNameEx ( "NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON" );
	m_pBuyConfirm->SetFlip(RGAME_GLOBAL::bNPCContributionShopConfirm );
	RegisterControl ( m_pBuyConfirm );

	m_pBuyConfirmText = new CBasicTextBox;
	m_pBuyConfirmText->CreateSub ( this, "NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON_TEXT" );
	m_pBuyConfirmText->SetFont ( pFont9 );
	m_pBuyConfirmText->SetText( ID2GAMEWORD( "NPC_CONTRIBUTION_SHOP_TEXT", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pBuyConfirmText );
}

void CNPCContributionShopWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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

	GLCHARLOGIC_CLIENT& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();

	if ( m_pTextCharContribution )
	{
		m_pTextCharContribution->ClearText();

		CString strMoney;
		strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)ref_CharData.m_dwContributionPoint, 3, "," );
		
		m_pTextCharContribution->SetText( strMoney.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
	}

	if ( m_pTextCharMoney )
	{
		m_pTextCharMoney->ClearText();

		CString strMoney;
		strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)ref_CharData.m_lnMoney, 3, "," );

		m_pTextCharMoney->SetText( strMoney.GetString(), NS_UITEXTCOLOR::GOLD );
	}
}

void CNPCContributionShopWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case NPC_CONTRIBUTION_SHOP_ITEM00:
	case NPC_CONTRIBUTION_SHOP_ITEM01:
	case NPC_CONTRIBUTION_SHOP_ITEM02:
	case NPC_CONTRIBUTION_SHOP_ITEM03:
	case NPC_CONTRIBUTION_SHOP_ITEM04:
	case NPC_CONTRIBUTION_SHOP_ITEM05:
	case NPC_CONTRIBUTION_SHOP_ITEM06:
	case NPC_CONTRIBUTION_SHOP_ITEM07:
	case NPC_CONTRIBUTION_SHOP_ITEM08:
	case NPC_CONTRIBUTION_SHOP_ITEM09:
	case NPC_CONTRIBUTION_SHOP_ITEM10:
	case NPC_CONTRIBUTION_SHOP_ITEM11:
	case NPC_CONTRIBUTION_SHOP_ITEM12:
	case NPC_CONTRIBUTION_SHOP_ITEM13:
	case NPC_CONTRIBUTION_SHOP_ITEM14:
	case NPC_CONTRIBUTION_SHOP_ITEM15:
	case NPC_CONTRIBUTION_SHOP_ITEM16:
	case NPC_CONTRIBUTION_SHOP_ITEM17:
	case NPC_CONTRIBUTION_SHOP_ITEM18:
	case NPC_CONTRIBUTION_SHOP_ITEM19:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - NPC_CONTRIBUTION_SHOP_ITEM00;
				if ( m_pItemSlot[nSelect] )
				{
					SITEMCUSTOM sitemcustom;

					sitemcustom.sNativeID = m_pItemSlot[nSelect]->m_sNPCShopItem.sItemID;
					sitemcustom.cDAMAGE = m_pItemSlot[nSelect]->m_sNPCShopItem.cDAMAGE;
					sitemcustom.cDEFENSE = m_pItemSlot[nSelect]->m_sNPCShopItem.cDEFENSE;
					sitemcustom.cRESIST_FIRE = m_pItemSlot[nSelect]->m_sNPCShopItem.cRESIST_FIRE;
					sitemcustom.cRESIST_ICE = m_pItemSlot[nSelect]->m_sNPCShopItem.cRESIST_ICE;
					sitemcustom.cRESIST_ELEC = m_pItemSlot[nSelect]->m_sNPCShopItem.cRESIST_ELEC;
					sitemcustom.cRESIST_POISON = m_pItemSlot[nSelect]->m_sNPCShopItem.cRESIST_POISON;
					sitemcustom.cRESIST_SPIRIT = m_pItemSlot[nSelect]->m_sNPCShopItem.cRESIST_SPIRIT;

					SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
					if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

					if ( UIMSG_NPC_CONTRIBUTION_SHOP_SLOT_ITEM_IMAGE & dwMsg )
					{
						if ( sitemcustom.sNativeID != NATIVEID_NULL () )
							CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL );
					}

					if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
					{
						SelectItem(nSelect);
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

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
							return;
						}
					}
				}
			}
		}break;

	case NPC_CONTRIBUTION_SHOP_PREV_BUTTON:
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

	case NPC_CONTRIBUTION_SHOP_NEXT_BUTTON:
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

	case NPC_CONTRIBUTION_SHOP_SELECTION_TEXTBOX:
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

	case NPC_CONTRIBUTION_SHOP_CONFIRM_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RGAME_GLOBAL::bNPCContributionShopConfirm = !RGAME_GLOBAL::bNPCContributionShopConfirm;
				m_pBuyConfirm->SetFlip( RGAME_GLOBAL::bNPCContributionShopConfirm );
			}
		}break;

	case NPC_CONTRIBUTION_SHOP_PURCHASE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqNpcContributionShopPurchase( m_sCrow, m_sSelectItem );
			}
		}break;
	};
}

void CNPCContributionShopWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
	else
	{
		ResetShop();
	}
}

void CNPCContributionShopWindow::InitShop( SNATIVEID sidCrow )
{
	ResetShop();

	SCROWDATA* pcrow = GLCrowDataMan::GetInstance().GetCrowData( sidCrow );
	if ( !pcrow )	return;

	SNPC_CONTRIBUTION_SHOP_DATA* pNpcShop = GLCrowDataMan::GetInstance().NPCContributionShopFind( pcrow->m_sAction.m_strContributionShop );
	if ( !pNpcShop )	return;

	if ( pNpcShop->m_strTitle.size() )
		SetTitleName( pNpcShop->m_strTitle.c_str() );

	//copy data
	m_sCrow = pcrow->m_sBasic.sNativeID;

	SNPC_CONTRIBUTION_SHOP_TYPE_MAP_ITER it_b = pNpcShop->m_mapType.begin();
	SNPC_CONTRIBUTION_SHOP_TYPE_MAP_ITER it_e = pNpcShop->m_mapType.end();
	for ( ; it_b != it_e; ++it_b )
	{
		const SNPC_CONTRIBUTION_SHOP_TYPE& sData = *(*it_b).second;

		CString strTemp;
		strTemp.Format( "%s(%d)", sData.strName.c_str(), sData.wItemNum );
		int nIndex = m_pListTextBox->AddText ( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
		m_pListTextBox->SetTextData ( nIndex, sData.wID );
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

void CNPCContributionShopWindow::ResetShop()
{
	m_vecItem.clear();

	m_nCurrentPage = 1;
	m_nSelectIndexType = -1;
	m_sSelectItem = NATIVEID_NULL();
	m_sCrow = NATIVEID_NULL();
	m_nMaxPage = 1;

	for ( int i=0; i<NPC_CONTRIBUTION_SHOP_MAX_ITEM; ++i )
	{
		m_pItemSlot[i]->ItemReset();
		m_pItemSlot[i]->SetVisibleSingle( FALSE );
	}

	m_pListTextBox->ClearText();

	if ( m_pButtonPurchase )
		m_pButtonPurchase->SetOneLineText( ID2GAMEWORD( "NPC_SHOP_TEXT", 1 ) );

	SetTitleName( ID2GAMEWORD( "NPC_CONTRIBUTION_SHOP_TEXT", 0 ) );
}

void CNPCContributionShopWindow::SelectType( DWORD dwType )
{
	m_vecItem.clear();
	m_nCurrentPage = 1;

	SCROWDATA* pcrow = GLCrowDataMan::GetInstance().GetCrowData( m_sCrow );
	if ( pcrow )
	{
		SNPC_CONTRIBUTION_SHOP_DATA* pNpcShop = GLCrowDataMan::GetInstance().NPCContributionShopFind( pcrow->m_sAction.m_strContributionShop );
		if ( pNpcShop )
		{
			SNPC_CONTRIBUTION_SHOP_ITEM_MAP_ITER it_b = pNpcShop->m_mapItem.begin();
			SNPC_CONTRIBUTION_SHOP_ITEM_MAP_ITER it_e = pNpcShop->m_mapItem.end();
			for ( ; it_b != it_e; ++it_b )
			{
				const SNPC_CONTRIBUTION_SHOP_ITEM& sData = *(*it_b).second;
				if ( sData.wType != dwType )	continue;

				m_vecItem.push_back( sData );
			}

			std::sort( m_vecItem.begin(), m_vecItem.end() );
		}
	}

	m_nMaxPage = 1 + ( (int)m_vecItem.size() / NPC_CONTRIBUTION_SHOP_MAX_ITEM );

	ShowItem( m_nCurrentPage );
}

void CNPCContributionShopWindow::ShowItem( int nPage )
{
	for ( int i=0; i< NPC_CONTRIBUTION_SHOP_MAX_ITEM; ++ i )
	{
		m_pItemSlot[i]->ItemReset();
		m_pItemSlot[i]->SetVisibleSingle( FALSE );
	}

	SelectItem( NPC_CONTRIBUTION_SHOP_MAX_ITEM );

	if ( nPage < 1 )	return;

	int nEnd = nPage * NPC_CONTRIBUTION_SHOP_MAX_ITEM;
	int nStart = nEnd - NPC_CONTRIBUTION_SHOP_MAX_ITEM;
	int nSlot = 0;

	for ( int i=nStart; i<nEnd; ++i )
	{
		if ( i >= m_vecItem.size() )	continue;

		if ( m_pItemSlot[nSlot] )
		{
			m_pItemSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pItemSlot[nSlot]->ItemSet( &m_vecItem[i] );
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

void CNPCContributionShopWindow::SelectItem( int nSelect )
{
	for ( int i=0; i< NPC_CONTRIBUTION_SHOP_MAX_ITEM; ++ i )
	{
		if ( m_pItemSlot[i] )
			m_pItemSlot[i]->ItemSelect( FALSE );
	}

	if ( nSelect >= 0 && nSelect < NPC_CONTRIBUTION_SHOP_MAX_ITEM )
	{
		if ( m_pItemSlot[nSelect] )
		{
			if ( m_pItemSlot[nSelect]->IsVisible() )
				m_pItemSlot[nSelect]->ItemSelect( TRUE );

			m_sSelectItem = m_pItemSlot[nSelect]->m_sNPCShopItem.sItemID;

			//juvs.info
			//no longer able to fit this long text
			//no longer showing price in purchase button

			/*DWORD dwContriPrice = m_pItemSlot[nSelect]->m_sNPCShopItem.dwContriPrice;
			DWORD dwGoldPrice = m_pItemSlot[nSelect]->m_sNPCShopItem.dwGoldPrice;

			if ( m_pButtonPurchase )	
			{
				m_pButtonPurchase->ClearText();

				int nIndex = m_pButtonPurchase->AddText( ID2GAMEWORD("NPC_CONTRIBUTION_SHOP_TEXT", 1 ), NS_UITEXTCOLOR::WHITE );

				CString strTemp;
				strTemp.Format( "C: %s", NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)dwContriPrice, 3, "," ) );
				m_pButtonPurchase->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
				m_pButtonPurchase->AddString( nIndex, " / ", NS_UITEXTCOLOR::WHITE );
				strTemp.Format( "G: %s", NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)dwGoldPrice, 3, "," ) );
				m_pButtonPurchase->AddString( nIndex, strTemp.GetString(), NS_UITEXTCOLOR::GOLD );
			}*/

			if ( m_pButtonPurchase )
				m_pButtonPurchase->SetOneLineText( ID2GAMEWORD( "NPC_CONTRIBUTION_SHOP_TEXT", 1 ) );

		}
	}
	else
	{
		m_sSelectItem = NATIVEID_NULL();

		if ( m_pButtonPurchase )
			m_pButtonPurchase->SetOneLineText( ID2GAMEWORD( "NPC_CONTRIBUTION_SHOP_TEXT", 1 ) );
	}

}