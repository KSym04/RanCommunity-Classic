#include "stdafx.h"
#include "./PVPClubWarRankingWindow.h"

#include "./PVPClubWarRankingPage.h"

#include "../../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../../Lib_ClientUI/Interface/BasicLineBoxEx.h"
#include "../../Lib_ClientUI/Interface/MultiModeButton.h"
#include "../../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../../Lib_ClientUI/Interface/BasicTextButton.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubWarRankingWindow::CPVPClubWarRankingWindow ()
	: m_nPage(-1)
{
	for ( int i=0; i<PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE; ++i )
	{
		m_pButton[i] = NULL;
		m_pPage[i] = NULL;
	}
}

CPVPClubWarRankingWindow::~CPVPClubWarRankingWindow ()
{

}

void CPVPClubWarRankingWindow::CreateSubControl ()
{
	std::string strEventButtons[PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE] = 
	{
		"PVP_CLUB_WAR_RANKING_OBJECT_TAP_0",
		"PVP_CLUB_WAR_RANKING_OBJECT_TAP_1",
		"PVP_CLUB_WAR_RANKING_OBJECT_TAP_2",
		"PVP_CLUB_WAR_RANKING_OBJECT_TAP_3",
	};

	for ( int i=0; i<PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE; ++i )
	{
		m_pButton[i] = new CMultiModeButton;
		m_pButton[i]->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, PVPCLUB_WAR_RANKING_WINDOW_BUTTON_0 + i );
		m_pButton[i]->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
		m_pButton[i]->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
		m_pButton[i]->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
		m_pButton[i]->AlignToControl( strEventButtons[i].c_str() );
		m_pButton[i]->SetOneLineText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_BUTTON", i ), NS_UITEXTCOLOR::WHITE );
		m_pButton[i]->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
		m_pButton[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pButton[i] );

		m_pPage[i] = new CPVPClubWarRankingPage;
		m_pPage[i]->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PVPCLUB_WAR_RANKING_WINDOW_PAGE_0 + i );
		m_pPage[i]->CreateSubControl();
		m_pPage[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pPage[i] );
	}
}

void CPVPClubWarRankingWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
	}
	else
	{
	}
}

void CPVPClubWarRankingWindow::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;


	case PVPCLUB_WAR_RANKING_WINDOW_BUTTON_0:
	case PVPCLUB_WAR_RANKING_WINDOW_BUTTON_1:
	case PVPCLUB_WAR_RANKING_WINDOW_BUTTON_2:
	case PVPCLUB_WAR_RANKING_WINDOW_BUTTON_3:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				int nPage = cID - PVPCLUB_WAR_RANKING_WINDOW_BUTTON_0;
				SetVisiblePage( nPage );
			}
		}break;

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
				
			}
		}break;

	};
}

void CPVPClubWarRankingWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{		
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );


}

void CPVPClubWarRankingWindow::SetVisiblePage( int nPage )
{
	if ( nPage == m_nPage )	return;
	if ( nPage == -1 )	return;

	for( int i=0; i<PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE; ++i )
	{
		if ( m_pButton[i] )
			m_pButton[i]->DoImageFlip( FALSE );

		if ( m_pPage[i] )
			m_pPage[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pButton[nPage] )
	{
		m_pButton[nPage]->DoImageFlip( TRUE );
	}

	if ( m_pPage[nPage] )
	{
		m_pPage[nPage]->SetVisibleSingle( TRUE );
		m_pPage[nPage]->RefreshRanking();
	}

	m_nPage = nPage;
}

void CPVPClubWarRankingWindow::SetData()
{
	for( int i=0; i<PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE; ++i )
	{
		if ( m_pButton[i] )
		{
			m_pButton[i]->DoImageFlip( FALSE );
			m_pButton[i]->SetVisibleSingle( FALSE );
		}

		if ( m_pPage[i] )
		{
			m_pPage[i]->SetVisibleSingle( FALSE );
			m_pPage[i]->SetData( NULL );
		}
	}

	SPVP_CLUB_WAR_CLIENT_DATA_MAP& mapData = GLGaeaClient::GetInstance().m_mapClubWarData;

	SPVP_CLUB_WAR_CLIENT_DATA_MAP_ITER it_b = mapData.begin();
	SPVP_CLUB_WAR_CLIENT_DATA_MAP_ITER it_e = mapData.end();
	for ( int nSlot = 0; it_b != it_e; ++it_b, nSlot++ )
	{
		SPVP_CLUB_WAR_CLIENT_DATA& sData = (*it_b).second;
		if ( nSlot < PVPCLUB_WAR_RANKING_WINDOW_MAX_PAGE )
		{
			if ( m_pButton[nSlot] )
			{
				m_pButton[nSlot]->SetVisibleSingle( TRUE );
				m_pButton[nSlot]->SetOneLineText( sData.szClubWarName, NS_UITEXTCOLOR::WHITE );
				m_pButton[nSlot]->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
				m_pButton[nSlot]->DoImageFlip( FALSE );
			}

			if ( m_pPage[nSlot] )
			{
				m_pPage[nSlot]->SetData( &sData );
			}
		}
	}

	m_nPage = -1;
	SetVisiblePage(0);
}
