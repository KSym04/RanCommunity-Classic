#include "StdAfx.h"
#include "GachaWindow.h"

#include "../MultiModeButton.h"
#include "../BasicTextBox.h"

#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"

#include "../InnerInterface.h"

#include "GachaPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGachaWindow::CGachaWindow ()
	: m_nPage(-1)
	, m_pButtonPage1(NULL)
	, m_pPageGacha(NULL)
{
}

CGachaWindow::~CGachaWindow ()
{
}

void CGachaWindow::CreateSubControl ()
{
	m_pButtonPage1 = CreateTabButton( "GACHA_OBJECT_TAP_0", "Gacha", STUDENT_RECORD_WINDOW_BUTTON_PAGE1 );

	m_pPageGacha = new CGachaPage;
	m_pPageGacha->CreateSub( this, "GACHA_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, STUDENT_RECORD_WINDOW_PAGE1 );
	m_pPageGacha->CreateSubControl ();
	m_pPageGacha->SetVisibleSingle( FALSE );
	RegisterControl ( m_pPageGacha );
}

CMultiModeButton* CGachaWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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

void CGachaWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CGachaWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
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

	case STUDENT_RECORD_WINDOW_BUTTON_PAGE1:
	case STUDENT_RECORD_WINDOW_BUTTON_PAGE2:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = ControlID - STUDENT_RECORD_WINDOW_BUTTON_PAGE1;
				OpenPage( nSelected );
			}
		}break;
	case STUDENT_RECORD_WINDOW_PAGE1:
		{

		}break;
	};
}

void CGachaWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage( STUDENT_RECORD_WINDOW_PAGE_TEMP_1 );
	}
}

void CGachaWindow::OpenPage( int nPage )
{
	if ( nPage == m_nPage )	return;

	if ( m_pButtonPage1 )	m_pButtonPage1->DoImageFlip( FALSE );
	if ( m_pPageGacha )	m_pPageGacha->SetVisibleSingle( FALSE );


	m_nPage = nPage;

	switch( nPage )
	{
	case STUDENT_RECORD_WINDOW_PAGE_TEMP_1:
		{
			if ( m_pButtonPage1 )	m_pButtonPage1->DoImageFlip( TRUE );
			
			m_pPageGacha->SetVisibleSingle(TRUE);
		}break;

	case STUDENT_RECORD_WINDOW_PAGE_TEMP_2:
		{
		}break;
	};
}
void CGachaWindow::LoadData()
{
	m_pPageGacha->LoadData();
}
void CGachaWindow::SetItem(WORD nSelected)
{
	m_pPageGacha->SetItem(nSelected);
}
void CGachaWindow::SetDraw()
{
	m_pPageGacha->m_bDraw = true;
}