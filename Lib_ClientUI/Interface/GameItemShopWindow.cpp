#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "./GameItemShopWindow.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./BasicLineBoxEx.h"
#include "./MultiModeButton.h"
#include "./BasicTextBox.h"
#include "./InnerInterface.h"

#include "./GameItemShopPage.h"
#include "./GameItemShopPageItemSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGameItemShopWindow::CGameItemShopWindow () 
	: m_nPage(-1)
	, m_bFirstOpen(TRUE)
	, m_pUserPointText(NULL)
	, m_pVotePointText(NULL)
{
	for ( int i=0; i<GAME_ITEM_SHOP_WINDOW_SIZE; ++i )
	{
		m_pButton[i] = NULL;
		m_pPage[i] = NULL;
	}
}

CGameItemShopWindow::~CGameItemShopWindow ()
{
}

void CGameItemShopWindow::CreateSubControl ()
{
	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_COMPETITION_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxCompetitionWindow( "GAME_ITEM_SHOP_PAGE" );
	RegisterControl ( pLineBox );

	std::string strEventButtons[GAME_ITEM_SHOP_WINDOW_SIZE] = 
	{
		"GAME_ITEM_SHOP_OBJECT_TAP_0",
		"GAME_ITEM_SHOP_OBJECT_TAP_1",
	};

	for ( int i=0; i<GAME_ITEM_SHOP_WINDOW_SIZE; ++i )
	{
		m_pButton[i] = CreateTabButton( strEventButtons[i].c_str(), ID2GAMEWORD( "GAME_ITEM_SHOP_WINDOW_PAGE", i ), GAME_ITEM_SHOP_WINDOW_BUTTON_0 + i );

		m_pPage[i] = new CGameItemShopPage;
		m_pPage[i]->CreateSub( this, "GAME_ITEM_SHOP_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, GAME_ITEM_SHOP_WINDOW_PAGE_0 + i );
		m_pPage[i]->CreateSubControl ();
		
		if ( i == 0 )
			m_pPage[i]->m_wPageType = GAME_ITEM_SHOP_TYPE_POINT;
		else if ( i == 1 )
			m_pPage[i]->m_wPageType = GAME_ITEM_SHOP_TYPE_VOTE;

		RegisterControl ( m_pPage[i] );
	}

	OpenPage( GAME_ITEM_SHOP_WINDOW_POINT_SHOP );
}

CMultiModeButton* CGameItemShopWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID );
	pButton->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
	pButton->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
	pButton->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pButton->AlignToControl( szButtonBox );
	pButton->SetOneLineText( szButtonText, NS_UITEXTCOLOR::WHITE );
	pButton->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pButton );
	return pButton;
}

void CGameItemShopWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CGameItemShopWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;

	case GAME_ITEM_SHOP_WINDOW_BUTTON_0:
	case GAME_ITEM_SHOP_WINDOW_BUTTON_1:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = ControlID - GAME_ITEM_SHOP_WINDOW_BUTTON_0;
				OpenPage( nSelected );
			}
		}break;

	};
}

void CGameItemShopWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if ( bVisible )
	{
		if ( m_bFirstOpen )
		{
			for ( int i=0; i<GAME_ITEM_SHOP_WINDOW_SIZE; ++i )
			{
				if ( m_pPage[i] )
					m_pPage[i]->DefaultPreview();
			}

			m_bFirstOpen = FALSE;
		}
	}
	
}

void CGameItemShopWindow::ReloadShop()
{
	for ( int i=0; i<GAME_ITEM_SHOP_WINDOW_SIZE; ++i )
	{
		if ( m_pPage[i] )
			m_pPage[i]->ReloadShop();
	}
}

void CGameItemShopWindow::OpenPage( int nPage )
{
	if ( nPage == m_nPage )					
		return;

	if ( nPage >= GAME_ITEM_SHOP_WINDOW_SIZE )
		return;

	for ( int i=0; i<GAME_ITEM_SHOP_WINDOW_SIZE; ++i )
	{
		if ( m_pButton[i] )
			m_pButton[i]->DoImageFlip( FALSE );

		if ( m_pPage[i] )
			m_pPage[i]->SetVisibleSingle( FALSE );
	}

	m_nPage = nPage;

	if ( m_pButton[m_nPage] )
		m_pButton[m_nPage]->DoImageFlip( TRUE );


	if ( m_pPage[m_nPage] )
		m_pPage[m_nPage]->SetVisibleSingle( TRUE );


}