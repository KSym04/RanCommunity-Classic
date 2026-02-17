#include "StdAfx.h"
#include "./PVPCompetitionWindow.h"

#include "./BasicLineBox.h"
#include "./BasicLineBoxEx.h"
#include "./MultiModeButton.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "./UITextControl.h"

#include "./PVPTyrannyPage.h"			/*pvp tyranny, Juver, 2017/08/25 */
#include "./PVPSchoolWarsPage.h"		/*school wars, Juver, 2018/01/19 */
#include "./PVPCaptureTheFlagPage.h"	/*pvp capture the flag, Juver, 2018/01/25 */
#include "./PVPClubDeathMatchPage.h"	/* pvp club death match, Juver, 2020/11/26 */
#include "./PVPPartyBattleGroundsPage.h"
#include "./PVPWoePage.h"	/*woe Arc Development 08-06-2024*/

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPCompetitionWindow::CPVPCompetitionWindow ()
	: m_nPage(-1)
	, m_pButtonClose(NULL)
	, m_pPageTyranny(NULL)				/*pvp tyranny, Juver, 2017/08/25 */
	, m_pPageSchoolWars(NULL)			/*school wars, Juver, 2018/01/19 */
	, m_pPageCaptureTheFlag(NULL)		/*pvp capture the flag, Juver, 2018/01/25 */
	, m_pPageClubDeathMatch(NULL)		/* pvp club death match, Juver, 2020/11/26 */
	, m_pPagePartyBattleGrounds(NULL)
	, m_pPageWoe(NULL)	/*woe Arc Development 08-06-2024*/
{
	for ( int i=0; i<COMPETITION_MAX_PAGE; ++i )
	{
		m_pButtonEvent[i] = NULL;
		m_pPageEvent[i] = NULL;
	}
}

CPVPCompetitionWindow::~CPVPCompetitionWindow ()
{
}

void CPVPCompetitionWindow::CreateSubControl ()
{
	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_COMPETITION_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxCompetitionWindow( "PVP_COMPETITION_OBJECT_PAGE" );
	RegisterControl ( pLineBox );	

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, COMPETITION_WINDOW_BUTTON_CLOSE );
	m_pButtonClose->CreateBaseButton ( "PVP_COMPETITION_OBJECT_CLOSE", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PVP_COMPETITION_WINDOW", 1 ) );
	m_pButtonClose->SetFlip ( TRUE );
	RegisterControl ( m_pButtonClose );

	std::string strEventButtons[COMPETITION_MAX_PAGE] = 
	{
		"PVP_COMPETITION_OBJECT_TAP_0",
		"PVP_COMPETITION_OBJECT_TAP_1",
		"PVP_COMPETITION_OBJECT_TAP_2",
		"PVP_COMPETITION_OBJECT_TAP_3",
		"PVP_COMPETITION_OBJECT_TAP_4",
		"PVP_COMPETITION_OBJECT_TAP_5", // Woe
	};

	int nIndex = 0;
	if ( RPARAM::bEventTyranny )
	{
		m_pButtonEvent[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "PVP_COMPETITION_BUTTON", 0 ), COMPETITION_WINDOW_BUTTON_0 + nIndex );

		m_pPageTyranny = new CPVPTyrannyPage;
		m_pPageTyranny->CreateSub( this, "PVP_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_TYRANNY );
		m_pPageTyranny->CreateSubControl ();
		RegisterControl ( m_pPageTyranny );

		m_pPageEvent[nIndex] = m_pPageTyranny;

		nIndex ++;
	}

	if ( RPARAM::bEventSchoolWars )
	{
		m_pButtonEvent[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "PVP_COMPETITION_BUTTON", 1 ), COMPETITION_WINDOW_BUTTON_0 + nIndex );

		m_pPageSchoolWars = new CPVPSchoolWarsPage;
		m_pPageSchoolWars->CreateSub( this, "PVP_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_SCHOOLWARS );
		m_pPageSchoolWars->CreateSubControl ();
		RegisterControl ( m_pPageSchoolWars );

		m_pPageEvent[nIndex] = m_pPageSchoolWars;

		nIndex ++;
	}

	if ( RPARAM::bEventCaptureTheFlag )
	{
		m_pButtonEvent[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "PVP_COMPETITION_BUTTON", 2 ), COMPETITION_WINDOW_BUTTON_0 + nIndex );

		m_pPageCaptureTheFlag = new CPVPCaptureTheFlagPage;
		m_pPageCaptureTheFlag->CreateSub( this, "PVP_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_CAPTURE_THE_FLAG );
		m_pPageCaptureTheFlag->CreateSubControl ();
		RegisterControl ( m_pPageCaptureTheFlag );

		m_pPageEvent[nIndex] = m_pPageCaptureTheFlag;

		nIndex ++;
	}

	/* pvp club death match, Juver, 2020/11/26 */
	if ( RPARAM::bEventClubDeathMatch )
	{
		m_pButtonEvent[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "PVP_COMPETITION_BUTTON", 3 ), COMPETITION_WINDOW_BUTTON_0 + nIndex );

		m_pPageClubDeathMatch = new CPVPClubDeathMatchPage;
		m_pPageClubDeathMatch->CreateSub( this, "PVP_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_CLUB_DEATH_MATCH );
		m_pPageClubDeathMatch->CreateSubControl ();
		RegisterControl ( m_pPageClubDeathMatch );

		m_pPageEvent[nIndex] = m_pPageClubDeathMatch;

		nIndex ++;
	}

	if ( RPARAM::bEventPartyBattleGrounds )
	{
		m_pButtonEvent[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "PVP_COMPETITION_BUTTON", 4 ), COMPETITION_WINDOW_BUTTON_0 + nIndex );

		m_pPagePartyBattleGrounds = new CPVPPartyBattlePassPage;
		m_pPagePartyBattleGrounds->CreateSub( this, "PVP_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_PBG );
		m_pPagePartyBattleGrounds->CreateSubControl ();
		RegisterControl ( m_pPagePartyBattleGrounds );

		m_pPageEvent[nIndex] = m_pPagePartyBattleGrounds;

		nIndex ++;
	}

	if ( RPARAM::bPVPWoeWar )
	{
		m_pButtonEvent[nIndex] = CreateTabButton( strEventButtons[nIndex].c_str(), ID2GAMEWORD( "PVP_COMPETITION_BUTTON", 5 ), COMPETITION_WINDOW_BUTTON_0 + nIndex );

		m_pPageWoe = new CPVPWoePage;
		m_pPageWoe->CreateSub( this, "PVP_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_WOE );
		m_pPageWoe->CreateSubControl ();
		RegisterControl ( m_pPageWoe );

		m_pPageEvent[nIndex] = m_pPageWoe;

		nIndex ++;
	}

	OpenPage( 0 );
}

CMultiModeButton* CPVPCompetitionWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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

void CPVPCompetitionWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
	}
	else
	{
	}
}

void CPVPCompetitionWindow::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
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

	
	case COMPETITION_WINDOW_BUTTON_CLOSE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) ){
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case COMPETITION_WINDOW_BUTTON_0:
	case COMPETITION_WINDOW_BUTTON_1:
	case COMPETITION_WINDOW_BUTTON_2:
	case COMPETITION_WINDOW_BUTTON_3:
	case COMPETITION_WINDOW_BUTTON_4:
	case COMPETITION_WINDOW_BUTTON_5:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = cID - COMPETITION_WINDOW_BUTTON_0;
				OpenPage( nSelected );
			}
		}break;
	};

	
}

void CPVPCompetitionWindow::OpenPage( int nPage )
{
	if ( nPage == m_nPage )					return;
	if ( nPage >= COMPETITION_MAX_PAGE )	return;

	for ( int i=0; i<COMPETITION_MAX_PAGE; ++i )
	{
		if ( m_pButtonEvent[i] )
			m_pButtonEvent[i]->DoImageFlip( FALSE );

		if ( m_pPageEvent[i] )
			m_pPageEvent[i]->SetVisibleSingle( FALSE );
	}

	m_nPage = nPage;

	if ( m_pButtonEvent[m_nPage] )
		m_pButtonEvent[m_nPage]->DoImageFlip( TRUE );

	if ( m_pPageEvent[m_nPage] )
		m_pPageEvent[m_nPage]->SetVisibleSingle( TRUE );
}

void CPVPCompetitionWindow::SetCompetitionJoin( bool bEnable )
{
	if ( m_pPageTyranny )			
		m_pPageTyranny->SetCompetitionJoin( bEnable );

	if ( m_pPageSchoolWars )		
		m_pPageSchoolWars->SetCompetitionJoin( bEnable );

	if ( m_pPageCaptureTheFlag )	
		m_pPageCaptureTheFlag->SetCompetitionJoin( bEnable );

	if ( m_pPageClubDeathMatch )	
		m_pPageClubDeathMatch->SetCompetitionJoin( bEnable );

	if ( m_pPagePartyBattleGrounds )	
		m_pPagePartyBattleGrounds->SetCompetitionJoin( bEnable );

	if ( m_pPageWoe )	
			m_pPageWoe->SetCompetitionJoin( bEnable );
}