/**MMR Rank Stats, Mhundz 01/02/2025**/
#include "StdAfx.h"
#include "MMRWindow.h"
#include "MMRPage.h"

#include "../MultiModeButton.h"
#include "../BasicTextBox.h"

#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../InnerInterface.h"

#include "../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMMRWindow::CMMRWindow ()
	: m_nPage(-1)
	, m_pButtonPage1(NULL)
	, m_pButtonPage2(NULL)
	, m_pPageCodex(NULL)
{
}

CMMRWindow::~CMMRWindow ()
{
}

void CMMRWindow::CreateSubControl ()
{
	m_pButtonPage1 = CreateTabButton( "RAN_MMR_OBJECT_TAP_0", "MMR Stats", MMR_WINDOW_BUTTON_PAGE1 );
	m_pButtonPage2 = CreateTabButton( "RAN_MMR_OBJECT_TAP_1", "MMR Info", MMR_WINDOW_BUTTON_PAGE2 );
	m_pButtonPage2->SetVisibleSingle(FALSE);

	m_pPageCodex = new CMMRPage;
	m_pPageCodex->CreateSub( this, "RAN_MMR_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, MMR_WINDOW_PAGE2 );
	m_pPageCodex->CreateSubControl ();
	m_pPageCodex->SetVisibleSingle( FALSE );
	RegisterControl ( m_pPageCodex );
}

CMultiModeButton* CMMRWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID );
	pButton->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
	pButton->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
	pButton->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pButton->AlignToControl( szButtonBox );
	pButton->SetOneLineText( szButtonText, NS_UITEXTCOLOR::WHITE);
	pButton->SetTextColor( NS_UITEXTCOLOR::BLUE_WOW, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::BLUE_WOW );
	RegisterControl ( pButton );
	return pButton;
}

void CMMRWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CMMRWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
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

	case MMR_WINDOW_BUTTON_PAGE1:
	case MMR_WINDOW_BUTTON_PAGE2:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = ControlID - MMR_WINDOW_BUTTON_PAGE1;
				OpenPage( nSelected );
			}
		}break;
	};
}

void CMMRWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage( MMR_WINDOW_PAGE_TEMP_1 );
	}
}

void CMMRWindow::OpenPage( int nPage )
{
	if ( nPage == m_nPage )	return;

	if ( m_pButtonPage1 )	m_pButtonPage1->DoImageFlip( FALSE );
	if ( m_pButtonPage2 )	m_pButtonPage2->DoImageFlip( FALSE );

	if ( m_pPageCodex )	m_pPageCodex->SetVisibleSingle( FALSE );

	m_nPage = nPage;

	switch( nPage )
	{
	case MMR_WINDOW_PAGE_TEMP_1:
		{
			if ( m_pButtonPage1 )	m_pButtonPage1->DoImageFlip( TRUE );
			
			m_pPageCodex->SetVisibleSingle( TRUE );
		}break;

	case MMR_WINDOW_PAGE_TEMP_2:
		{
			if ( m_pButtonPage2 )	m_pButtonPage2->DoImageFlip( TRUE );
		}break;
	};
}
