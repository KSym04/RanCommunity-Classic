#include "StdAfx.h"
#include "ClubWarRankingsDisplayMain.h"

#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../BasicTextButton.h"

#include "../MultiModeButton.h"

#include "../GameTextControl.h"
#include "../InnerInterface.h"
#include "../UITextControl.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

#include "./ClubWarRankingsDisplay.h"
#include "./ClubWarRankingsDisplayResu.h"
#include "./ClubWarRankingsDisplayGuild.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClubWarRankingsDisplayMain::CClubWarRankingsDisplayMain ()
: m_nPage(-1)
, m_pButtonRankingPlayer(NULL)
, m_pButtonRankingPlayerResu(NULL)
, m_pButtonRankingGuild(NULL)
, m_pPageRankingPlayer(NULL)
, m_pPageRankingPlayerResu(NULL)
, m_pPageRankingGuild(NULL)
{
}

CClubWarRankingsDisplayMain::~CClubWarRankingsDisplayMain ()
{
}

void CClubWarRankingsDisplayMain::CreateSubControl ()
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_TYRANNY_RANKING_DISPLAY_BG_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );	

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_TYRANNY_RANKING_DISPLAY_LINEBOX_LOWER" );
	//RegisterControl ( pLineBox );

	m_pWhiteBG = new CUIControl;
	m_pWhiteBG->CreateSub(this, "WAR_RANKING_WINDOW_WHITE_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	RegisterControl(m_pWhiteBG);

	m_pWhiteBG1 = new CUIControl;
	m_pWhiteBG1->CreateSub(this, "WAR_RANKING_WINDOW_WHITE_BG1", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	RegisterControl(m_pWhiteBG1);

	//m_pButtonRankingPlayer = CreateTextButton19 ( "PVP_TYRANNY_RANKING_DISPLAY_BUTTON_PLAYER", BUTTON_RANKING_PLAYER, (char*)ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 1 ) );
	//m_pButtonRankingPlayerResu = CreateTextButton19 ( "PVP_TYRANNY_RANKING_DISPLAY_BUTTON_PLAYER_RESU", BUTTON_RANKING_PLAYER_RESU, (char*)ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 2 ) );
	//m_pButtonRankingGuild = CreateTextButton19 ( "PVP_TYRANNY_RANKING_DISPLAY_BUTTON_GUILD", BUTTON_RANKING_GUILD, (char*)ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 3 ) );

	m_pButtonRankingPlayer = CreateTabButton( "PVP_TYRANNY_RANKING_DISPLAY_BUTTON_PLAYER", ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 1 ), BUTTON_RANKING_PLAYER );
	m_pButtonRankingPlayerResu = CreateTabButton( "PVP_TYRANNY_RANKING_DISPLAY_BUTTON_PLAYER_RESU", ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 2 ), BUTTON_RANKING_PLAYER_RESU );
	m_pButtonRankingGuild = CreateTabButton( "PVP_TYRANNY_RANKING_DISPLAY_BUTTON_GUILD", ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 3 ), BUTTON_RANKING_GUILD );



	m_pPageRankingPlayer = new CClubWarRankingsDisplay;
	m_pPageRankingPlayer->CreateSub( this, "PVP_TYRANNY_RANKING_DISPLAY_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PAGE_RANKING_PLAYER );
	m_pPageRankingPlayer->CreateSubControl ();
	RegisterControl ( m_pPageRankingPlayer );

	m_pPageRankingPlayerResu = new CClubWarRankingsDisplayResu;
	m_pPageRankingPlayerResu->CreateSub( this, "PVP_TYRANNY_RANKING_DISPLAY_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PAGE_RANKING_PLAYER_RESU );
	m_pPageRankingPlayerResu->CreateSubControl ();
	RegisterControl ( m_pPageRankingPlayerResu );

	m_pPageRankingGuild = new CClubWarRankingsDisplayGuild;
	m_pPageRankingGuild->CreateSub( this, "PVP_TYRANNY_RANKING_DISPLAY_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, PAGE_RANKING_GUILD );
	m_pPageRankingGuild->CreateSubControl ();
	RegisterControl ( m_pPageRankingGuild );

	m_pButtonRankingPlayer->DoImageFlip( TRUE );

	


}

CMultiModeButton* CClubWarRankingsDisplayMain::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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
CBasicTextButton* CClubWarRankingsDisplayMain::CreateTextButton19 ( char* szButton, UIGUID ControlID, char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText );
	pTextButton->SetFlip ( FALSE );
	RegisterControl ( pTextButton );
	return pTextButton;
}

void CClubWarRankingsDisplayMain::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		OpenPage( WINDOW_RANKING_PLAYER );
	}
	else
	{
	}
}

void CClubWarRankingsDisplayMain::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
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

	case BUTTON_RANKING_PLAYER:
	case BUTTON_RANKING_PLAYER_RESU:
	case BUTTON_RANKING_GUILD:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = cID - BUTTON_RANKING_PLAYER;
				OpenPage( nSelected );
			}
		}break;
	};

}

void CClubWarRankingsDisplayMain::OpenPage( int nPage )
{
	if ( nPage == m_nPage )	return;

	if ( m_pButtonRankingPlayer )		m_pButtonRankingPlayer->DoImageFlip( FALSE );
	if ( m_pButtonRankingPlayerResu )		m_pButtonRankingPlayerResu->DoImageFlip( FALSE );
	if ( m_pButtonRankingGuild )		m_pButtonRankingGuild->DoImageFlip( FALSE );

	if ( m_pPageRankingPlayer )	m_pPageRankingPlayer->SetVisibleSingle( FALSE );
	if ( m_pPageRankingPlayerResu )	m_pPageRankingPlayerResu->SetVisibleSingle( FALSE );
	if ( m_pPageRankingGuild )	m_pPageRankingGuild->SetVisibleSingle( FALSE );

	m_nPage = nPage;

	switch( nPage )
	{
	case WINDOW_RANKING_PLAYER:
		{
			if ( m_pButtonRankingPlayer )	m_pButtonRankingPlayer->DoImageFlip( TRUE );
			if ( m_pPageRankingPlayer )		m_pPageRankingPlayer->SetVisibleSingle( TRUE );
			GLGaeaClient::GetInstance().ReqClubWarRankInfo();
		}break;

	case WINDOW_RANKING_PLAYER_RESU:
		{
			if ( m_pButtonRankingPlayerResu )	m_pButtonRankingPlayerResu->DoImageFlip( TRUE );
			if ( m_pPageRankingPlayerResu )		m_pPageRankingPlayerResu->SetVisibleSingle( TRUE );
			GLGaeaClient::GetInstance().ReqClubWarRankResuInfo();
		}break;

	case WINDOW_RANKING_GUILD:
		{
			if ( m_pButtonRankingGuild )	m_pButtonRankingGuild->DoImageFlip( TRUE );
			if ( m_pPageRankingGuild )		m_pPageRankingGuild->SetVisibleSingle( TRUE );
			GLGaeaClient::GetInstance().ReqClubWarClubRankInfo();
		}break;
	};
}

void CClubWarRankingsDisplayMain::RefreshPage()
{
	m_pPageRankingPlayer->RefreshClubWarRanking();
	m_pPageRankingPlayerResu->RefreshClubWarRankingResu();
	m_pPageRankingGuild->RefreshClubWarClubRanking();
}