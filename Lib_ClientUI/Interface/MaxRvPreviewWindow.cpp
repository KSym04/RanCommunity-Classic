#include "StdAfx.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "./BasicLineBox.h"
#include "./BasicLineBoxEx.h"
#include "./MultiModeButton.h"
#include "./BasicTextBoxEx.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "./MaxRvPreviewPage.h"
#include "./MaxRvPreviewWindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CMaxRvPreviewWindow::CMaxRvPreviewWindow(void)
	: m_nPage(-1)
	, m_pPageMobGen(NULL)
	, m_pPageRebuild(NULL)
{
	for ( int i=0; i<MAXRV_PREVIEW_MAX_PAGE; ++i )
	{
		m_pButtonPage[i] = NULL;
		m_pPage[i] = NULL;
	}
}

CMaxRvPreviewWindow::~CMaxRvPreviewWindow(void)
{
}

void CMaxRvPreviewWindow::CreateSubControl()
{
	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite ( "MAXRV_PREVIEW_OBJECT_PAGE" );
	RegisterControl ( pBasicLineBox );

	std::string strEventButtons[MAXRV_PREVIEW_MAX_PAGE] = 
	{
		"MAXRV_PREVIEW_OBJECT_TAP_0",
		"MAXRV_PREVIEW_OBJECT_TAP_1",
	};

	int nIndex = 0;
	if ( RPARAM::bMaxRvPreviewMobGen )
	{
		m_pButtonPage[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "MAXRV_PREVIEW_PAGE", 0 ), MAXRV_PREVIEW_BUTTON_0 + nIndex );

		m_pPageMobGen = new CMaxRvPreviewPage;
		m_pPageMobGen->CreateSub( this, "MAXRV_PREVIEW_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, MAXRV_PREVIEW_PAGE_MOBGEN );
		m_pPageMobGen->CreateSubControl ();
		m_pPageMobGen->SetPreviewType(EMMAXRV_PREVIEW_MOBGEN);
		RegisterControl ( m_pPageMobGen );

		m_pPage[nIndex] = m_pPageMobGen;

		nIndex ++;
	}

	if ( RPARAM::bMaxRvPreviewRebuild )
	{
		m_pButtonPage[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "MAXRV_PREVIEW_PAGE", 1 ), MAXRV_PREVIEW_BUTTON_0 + nIndex );

		m_pPageRebuild = new CMaxRvPreviewPage;
		m_pPageRebuild->CreateSub( this, "MAXRV_PREVIEW_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, MAXRV_PREVIEW_PAGE_REBUILD );
		m_pPageRebuild->CreateSubControl ();
		m_pPageRebuild->SetPreviewType(EMMAXRV_PREVIEW_REBUILD);
		RegisterControl ( m_pPageRebuild );

		m_pPage[nIndex] = m_pPageRebuild;

		nIndex ++;
	}

	OpenPage( 0 );
}

CMultiModeButton* CMaxRvPreviewWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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

void CMaxRvPreviewWindow::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;


	case MAXRV_PREVIEW_BUTTON_0:
	case MAXRV_PREVIEW_BUTTON_1:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = cID - MAXRV_PREVIEW_BUTTON_0;
				OpenPage( nSelected );
			}
		}break;
	};
}

void CMaxRvPreviewWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
	else
	{
	}
}

void CMaxRvPreviewWindow::OpenPage( int nPage )
{
	if ( nPage == m_nPage )					return;
	if ( nPage >= MAXRV_PREVIEW_MAX_PAGE )	return;

	for ( int i=0; i<MAXRV_PREVIEW_MAX_PAGE; ++i )
	{
		if ( m_pButtonPage[i] )
			m_pButtonPage[i]->DoImageFlip( FALSE );

		if ( m_pPage[i] )
			m_pPage[i]->SetVisibleSingle( FALSE );
	}

	m_nPage = nPage;

	if ( m_pButtonPage[m_nPage] )
		m_pButtonPage[m_nPage]->DoImageFlip( TRUE );

	if ( m_pPage[m_nPage] )
		m_pPage[m_nPage]->SetVisibleSingle( TRUE );
}


void CMaxRvPreviewWindow::ShowItemData()
{
	if ( m_pPageMobGen )
		m_pPageMobGen->ShowItemData();

	if ( m_pPageRebuild )
		m_pPageRebuild->ShowItemData();
}