#include "stdafx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxInputDevice.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLGameItemShopClient.h"
#include "../../Lib_Client/G-Logic/GLItem.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#include "./GameItemShopPage.h"

#include "./GameItemShopPageType.h"
#include "./GameItemShopPageItemSlot.h"
#include "./GameItemShopPagePreview.h"

#include "./InnerInterface.h"
#include "./BasicLineBoxEx.h"
#include "./BasicButton.h"
#include "./BasicTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGameItemShopPage::CGameItemShopPage ()
	: m_wPageType(GAME_ITEM_SHOP_TYPE_NONE)
	, m_wCategory(GAME_ITEM_SHOP_CATEGORY_SPECIAL)
	, m_nCurrentPage(1)
	, m_nMaxPage(1)
	, m_sSelected(false)
	, m_pType(NULL)
	, m_pButtonPrev(NULL)
	, m_pButtonNext(NULL)
	, m_pTextPage(NULL)
	, m_pTextPointInfo(NULL)
	, m_pPreview(NULL)
{
	for ( int i=0; i<GAME_ITEM_SHOP_MAX_ITEM; ++i )
	{
		m_pSlot[i] = NULL;
	}
}

CGameItemShopPage::~CGameItemShopPage ()
{
}

void CGameItemShopPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFontPoint = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 16, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "GAME_ITEM_SHOP_PAGE_CATEGORY_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "GAME_ITEM_SHOP_PAGE_ITEM_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "GAME_ITEM_SHOP_PAGE_PREVIEW_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "GAME_ITEM_SHOP_PAGE_POINT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList( "GAME_ITEM_SHOP_PAGE_NUM_TEXT" );
	RegisterControl ( pLineBox );

	m_pType = new CGameItemShopPageType;
	m_pType->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_CATEGORY_LINE", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_TYPE );
	m_pType->CreateSubControl ();
	m_pType->SetVisibleSingle( TRUE );
	RegisterControl ( m_pType );

	std::string str[GAME_ITEM_SHOP_MAX_ITEM] = 
	{
		"GAME_ITEM_SHOP_PAGE_ITEM_00",
		"GAME_ITEM_SHOP_PAGE_ITEM_01",
		"GAME_ITEM_SHOP_PAGE_ITEM_02",
		"GAME_ITEM_SHOP_PAGE_ITEM_03",
		"GAME_ITEM_SHOP_PAGE_ITEM_04",
		"GAME_ITEM_SHOP_PAGE_ITEM_05",
		"GAME_ITEM_SHOP_PAGE_ITEM_06",
		"GAME_ITEM_SHOP_PAGE_ITEM_07",
		"GAME_ITEM_SHOP_PAGE_ITEM_08",
		"GAME_ITEM_SHOP_PAGE_ITEM_09",
		"GAME_ITEM_SHOP_PAGE_ITEM_10",
		"GAME_ITEM_SHOP_PAGE_ITEM_11",
		"GAME_ITEM_SHOP_PAGE_ITEM_12",
		"GAME_ITEM_SHOP_PAGE_ITEM_13",
	};

	for ( int i=0; i<GAME_ITEM_SHOP_MAX_ITEM; ++i )
	{
		m_pSlot[i] = new CGameItemShopPageItemSlot;
		m_pSlot[i]->CreateSub ( this, str[i].c_str(), UI_FLAG_DEFAULT, GAME_ITEM_SHOP_ITEM00 + i );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSlot[i] );
	}
	
	m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_NUM_TEXT" );
	m_pTextPage->SetFont ( pFont9 );
	m_pTextPage->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPage->SetText( "0" );
	RegisterControl ( m_pTextPage );

	m_pButtonPrev = new CBasicButton;
	m_pButtonPrev->CreateSub ( this, "GAME_ITEM_SHOP_PRE_PAGE_BUTTON", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PREV_BUTTON );
	m_pButtonPrev->CreateFlip ( "GAME_ITEM_SHOP_PRE_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonPrev->CreateMouseOver ( "GAME_ITEM_SHOP_PRE_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonPrev );

	m_pButtonNext = new CBasicButton;
	m_pButtonNext->CreateSub ( this, "GAME_ITEM_SHOP_NEXT_PAGE_BUTTON", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_NEXT_BUTTON );
	m_pButtonNext->CreateFlip ( "GAME_ITEM_SHOP_NEXT_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonNext->CreateMouseOver ( "GAME_ITEM_SHOP_NEXT_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonNext );

	m_pTextPointInfo = new CBasicTextBox;
	m_pTextPointInfo->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_POINT_INFO" );
	m_pTextPointInfo->SetFont ( pFontPoint );
	m_pTextPointInfo->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPointInfo->SetText( "1" );
	RegisterControl ( m_pTextPointInfo );


	m_pPreview = new CGameItemShopPagePreview;
	m_pPreview->CreateSub ( this, "GAME_ITEM_SHOP_PAGE_PREVIEW_LINE", UI_FLAG_DEFAULT, GAME_ITEM_SHOP_PAGE_RENDER );
	m_pPreview->CreateSubControl ();
	m_pPreview->SetVisibleSingle( TRUE );
	RegisterControl ( m_pPreview );
}

void CGameItemShopPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{

	case GAME_ITEM_SHOP_ITEM00:
	case GAME_ITEM_SHOP_ITEM01:
	case GAME_ITEM_SHOP_ITEM02:
	case GAME_ITEM_SHOP_ITEM03:
	case GAME_ITEM_SHOP_ITEM04:
	case GAME_ITEM_SHOP_ITEM05:
	case GAME_ITEM_SHOP_ITEM06:
	case GAME_ITEM_SHOP_ITEM07:
	case GAME_ITEM_SHOP_ITEM08:
	case GAME_ITEM_SHOP_ITEM09:
	case GAME_ITEM_SHOP_ITEM10:
	case GAME_ITEM_SHOP_ITEM11:
	case GAME_ITEM_SHOP_ITEM12:
	case GAME_ITEM_SHOP_ITEM13:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - GAME_ITEM_SHOP_ITEM00;

				if ( m_pSlot[nSelect] )
				{
					SITEMCUSTOM sitemcustom;
					sitemcustom.sNativeID = m_pSlot[nSelect]->m_sData.sItemID;	
					SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
					if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

					if ( UIMSG_GAME_ITEM_SHOP_SLOT_ITEM_IMAGE & dwMsg )
					{
						if ( sitemcustom.sNativeID != NATIVEID_NULL () )
							CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL() );
					}

					if ( UIMSG_GAME_ITEM_SHOP_SLOT_BUTTON & dwMsg )
					{
						
					}
					else
					{
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
			}
		}break;

	case GAME_ITEM_SHOP_PAGE_TYPE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_GAME_ITEM_SHOP_SELECT )
				{
					if ( m_pType )
					{
						m_wCategory = (WORD)m_pType->GetSelect();
						SelectType();
					}
				}
			}
		}break;

	case GAME_ITEM_SHOP_PREV_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_nCurrentPage --;
					if ( m_nCurrentPage < 1 )
						m_nCurrentPage = 1;
					ShowItem();
				}
			}
		}break;

	case GAME_ITEM_SHOP_NEXT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					if ( m_nCurrentPage >= m_nMaxPage )	return;

					m_nCurrentPage ++;
					ShowItem();
				}
			}
		}break;
	};
}

void CGameItemShopPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pTextPointInfo )
	{
		m_pTextPointInfo->ClearText();

		DWORD dwPoint = 0;

		if ( m_wPageType == GAME_ITEM_SHOP_TYPE_POINT )
			dwPoint = GLGameItemShopClient::GetInstance().m_dwUserPoint;
		else if ( m_wPageType == GAME_ITEM_SHOP_TYPE_VOTE )
			dwPoint = GLGameItemShopClient::GetInstance().m_dwVotePoint;

		int nIndex = m_pTextPointInfo->AddText( ID2GAMEWORD( "GAME_ITEM_SHOP_TYPES", m_wPageType ), NS_UITEXTCOLOR::BRIGHTGREEN );
		m_pTextPointInfo->AddString( nIndex, " : ", NS_UITEXTCOLOR::WHITE );
		m_pTextPointInfo->AddString( nIndex, NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)dwPoint, 3, "," ), NS_UITEXTCOLOR::YELLOW );
	}
	
}

void CGameItemShopPage::ReloadShop()
{
	if ( m_pType )
	{
		m_pType->DefaultSelect();
		m_wCategory = (WORD)m_pType->GetSelect();
	}

	SelectType();
}

void CGameItemShopPage::DefaultPreview()
{
	if ( m_pPreview )
		m_pPreview->DefaultPreview();
}

void CGameItemShopPage::SelectType()
{
	m_vecShop.clear();
	
	const SGAME_ITEM_SHOP_DATA_MAP& mapShop = GLGameItemShopClient::GetInstance().m_mapItemShop;
	SGAME_ITEM_SHOP_DATA_MAP_CITER it_b = mapShop.begin();
	SGAME_ITEM_SHOP_DATA_MAP_CITER it_e = mapShop.end();

	for ( ; it_b != it_e; ++it_b )
	{
		const SGAME_ITEM_SHOP_DATA& sData = (*it_b).second;

		if ( sData.wType != m_wPageType )
			continue;

		if ( sData.wCategory != m_wCategory )
			continue;

		m_vecShop.push_back( sData );
	}

	m_nCurrentPage = 1;
	m_nMaxPage = 1 + ( (int)m_vecShop.size() / GAME_ITEM_SHOP_MAX_ITEM );

	ShowItem();
}

void CGameItemShopPage::ShowItem()
{
	for ( int i=0; i< GAME_ITEM_SHOP_MAX_ITEM; ++ i )
	{
		m_pSlot[i]->ItemReset();
		m_pSlot[i]->SetVisibleSingle( FALSE );
	}

	SelectItem( GAME_ITEM_SHOP_MAX_ITEM );

	int nPage = m_nCurrentPage;

	if ( nPage < 1 )	return;

	int nEnd = nPage * GAME_ITEM_SHOP_MAX_ITEM;
	int nStart = nEnd - GAME_ITEM_SHOP_MAX_ITEM;
	int nSlot = 0;

	for ( int i=nStart; i<nEnd; ++i )
	{
		if ( i >= m_vecShop.size() )	continue;

		if ( m_pSlot[nSlot] )
		{
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->ItemSet( &m_vecShop[i] );
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

void CGameItemShopPage::SelectItem( int nSelect )
{
	for ( int i=0; i< GAME_ITEM_SHOP_MAX_ITEM; ++ i )
	{
		if ( m_pSlot[i] )
			m_pSlot[i]->ItemSelect( FALSE );
	}

	if ( nSelect >= 0 && nSelect < GAME_ITEM_SHOP_MAX_ITEM )
	{
		if ( m_pSlot[nSelect] )
		{
			if ( m_pSlot[nSelect]->IsVisible() )
				m_pSlot[nSelect]->ItemSelect( TRUE );

			m_sSelected = m_pSlot[nSelect]->m_sData.sItemID;
		}
	}
	else
	{
		m_sSelected = NATIVEID_NULL();
	}

	m_pPreview->DataShow(m_sSelected);
}