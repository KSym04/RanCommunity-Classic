#include "StdAfx.h"
#include "./PVPClubDeathMatchRankings.h"
#include "./PVPClubDeathMatchRankingPage.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./MultiModeButton.h"
#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchRankings::CPVPClubDeathMatchRankings ()
	: m_pButtonClose(NULL)
	, m_pInfoExit(NULL)
	, m_nPage(-1)
	, m_fUpdateTime(0.0f)
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_SIZE; ++i )
	{
		m_pButtonRank[i] = NULL;
		m_pPageRanking[i] = NULL;
	}
}

CPVPClubDeathMatchRankings::~CPVPClubDeathMatchRankings ()
{
}

void CPVPClubDeathMatchRankings::CreateSubControl ()
{
	CD3DFontPar * pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	std::string strbutton[PVP_CLUB_DEATH_MATCH_RANK_SIZE] = 
	{
		"RNCDM_RANK_TAB_BUTTON",
	};

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_SIZE; ++i )
	{
		m_pButtonRank[i] = CreateTabButton( strbutton[i].c_str(), ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_RANKING_PAGE", i ), PVP_CLUB_DEATH_MATCH_RANK_BUTTON_ALL +i );

		m_pPageRanking[i] = new CPVPClubDeathMatchRankingPage;
		m_pPageRanking[i]->CreateSub ( this, "RNCDM_RANK_RANKING", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PVP_CLUB_DEATH_MATCH_RANK_PAGE_ALL + i );
		m_pPageRanking[i]->CreateSubControl( (i==0)? PVP_CLUB_DEATH_MATCH_RANK_SIZE: i-1 );
		m_pPageRanking[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pPageRanking[i] );
	}

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, PVP_CLUB_DEATH_MATCH_RANK_BUTTON_CLOSE );
	m_pButtonClose->CreateBaseButton ( "CTF_RESULT_BUTTON_CLOSE", CBasicTextButton::SIZE19, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_RANKING", 0 ) );
	m_pButtonClose->SetFlip ( TRUE );
	RegisterControl ( m_pButtonClose );

	m_pInfoExit = new CBasicTextBox;
	m_pInfoExit->CreateSub ( this, "CTF_RESULT_TEXT_TIME", UI_FLAG_DEFAULT );
	m_pInfoExit->SetFont ( pFont );
	m_pInfoExit->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );	
	RegisterControl ( m_pInfoExit );
}

CMultiModeButton* CPVPClubDeathMatchRankings::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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

void CPVPClubDeathMatchRankings::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		SetVisiblePage( PVP_CLUB_DEATH_MATCH_RANK_ALL );
	}
	else
	{
	}
}

void CPVPClubDeathMatchRankings::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;


	case PVP_CLUB_DEATH_MATCH_RANK_ALL:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) ){
				int nPage = cID - PVP_CLUB_DEATH_MATCH_RANK_ALL;
				SetVisiblePage( nPage );
			}
		}break;

	case ET_CONTROL_BUTTON:
	case PVP_CLUB_DEATH_MATCH_RANK_BUTTON_CLOSE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
				PVPClubDeathMatchClient::GetInstance().RequestMapExit();
			}
		}break;

	};
}

void CPVPClubDeathMatchRankings::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{		
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( !IsVisible () ){
		return;
	}

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		m_fUpdateTime = 0.0f;

		if ( m_pInfoExit )
		{
			m_pInfoExit->ClearText();

			float fremain_time = PVPClubDeathMatchClient::GetInstance().m_fRemain;
			if ( fremain_time > 0.0f )
			{
				CString strCombine;
				strCombine.Format ( "< %s %02d:%02d >", ID2GAMEWORD( "PVP_CLUB_DEATH_MATCH_RESULT_TEXT", 0 ), (DWORD)fremain_time/60, (DWORD)fremain_time%60 );
				m_pInfoExit->AddText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE );
			}
		}	
	}
}

void CPVPClubDeathMatchRankings::SetVisiblePage( int nPage )
{
	if ( nPage == m_nPage )	return;
	if ( nPage == -1 )	return;

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_SIZE; ++i )
	{
		if ( m_pButtonRank[i] )
			m_pButtonRank[i]->DoImageFlip( FALSE );

		if ( m_pPageRanking[i] )
			m_pPageRanking[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pButtonRank[nPage] )
		m_pButtonRank[nPage]->DoImageFlip( TRUE );

	if ( m_pPageRanking[nPage] )
		m_pPageRanking[nPage]->SetVisibleSingle( TRUE );

	m_nPage = nPage;
}

void CPVPClubDeathMatchRankings::UpdateRankings()
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_SIZE; ++i )
	{
		if ( m_pPageRanking[i] )
			m_pPageRanking[i]->RefreshRanking();
	}
}