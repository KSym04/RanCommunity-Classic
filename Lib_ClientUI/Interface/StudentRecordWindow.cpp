#include "StdAfx.h"
#include "./StudentRecordWindow.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "./MultiModeButton.h"
#include "./BasicTextBox.h"
#include "CodexWindow/CodexPage.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "./InnerInterface.h"

/*activity system, Juver, 2017/10/30 */
#include "./ActivityPage.h"

/* play time system, Juver, 2021/01/26 */
#include "./PlayStatsPage.h"

/*12-16-24, daily login - CNDev*/
#include "./DailyLoginPage.h"

/*12-9-14, Battle Pass - CNDev*/
#include "./BattlePassPage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CStudentRecordWindow::CStudentRecordWindow ()
	: m_nPage(-1)
	, m_pButtonPagePlayStats(NULL)
	, m_pButtonPageActivity(NULL)
	, m_pButtonPageDailyLogin(NULL)
	, m_pButtonPageCodex(NULL)
	, m_pButtonPageBattlepass(NULL)

	, m_pPagePlayStats(NULL)
	, m_pPageActivity(NULL) /*activity system, Juver, 2017/10/30 */
	, m_pPageDailyLogin(NULL)	/*12-16-24, daily login - CNDev*/
	, m_pPageCodex(NULL)
	, m_pPageBattlePass(NULL)	/*12-9-14, Battle Pass - CNDev*/
{
}

CStudentRecordWindow::~CStudentRecordWindow ()
{
}

void CStudentRecordWindow::CreateSubControl ()
{
	m_pButtonPagePlayStats = CreateTabButton( "RAN_STUDENTRECORD_OBJECT_TAP_0", ID2GAMEWORD("RAN_STUDENTRECORD_WINDOW_PAGES", 0 ), STUDENT_RECORD_WINDOW_BUTTON_PAGE_PLAY_STATS );
	
	/* play stats window, Juver, 2021/01/26 */
	m_pPagePlayStats = new CPlayStatsPage;
	m_pPagePlayStats->CreateSub( this, "RAN_STUDENTRECORD_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, STUDENT_RECORD_WINDOW_PAGE_PLAY_STATS );
	m_pPagePlayStats->CreateSubControl ();
	m_pPagePlayStats->SetVisibleSingle( FALSE );
	RegisterControl ( m_pPagePlayStats );

	/*activity system, Juver, 2017/10/30 */
	if ( RPARAM::bActivity )
	{
		m_pButtonPageActivity = CreateTabButton( "RAN_STUDENTRECORD_OBJECT_TAP_1", ID2GAMEWORD("RAN_STUDENTRECORD_WINDOW_PAGES", 1 ), STUDENT_RECORD_WINDOW_BUTTON_PAGE_ACTIVITY );

		m_pPageActivity = new CActivityPage;
		m_pPageActivity->CreateSub( this, "RAN_STUDENTRECORD_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, STUDENT_RECORD_WINDOW_PAGE_ACTIVITY );
		m_pPageActivity->CreateSubControl ();
		m_pPageActivity->SetVisibleSingle( FALSE );
		RegisterControl ( m_pPageActivity );
	}
		/*12-16-24, daily login - CNDev*/
	if ( RPARAM::bFeatureDailyLogin )
	{	
		m_pButtonPageDailyLogin = CreateTabButton( "RAN_STUDENTRECORD_OBJECT_TAP_2", ID2GAMEWORD("RAN_STUDENTRECORD_WINDOW_PAGES", 2 ), STUDENT_RECORD_WINDOW_BUTTON_PAGE_DAILYLOGIN );
	
		m_pPageDailyLogin = new CDailyLoginPage;
		m_pPageDailyLogin->CreateSub( this, "RAN_STUDENTRECORD_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, STUDENT_RECORD_WINDOW_PAGE_DAILYLOGIN );
		m_pPageDailyLogin->CreateSubControl ();
		m_pPageDailyLogin->SetVisibleSingle( FALSE );
		RegisterControl ( m_pPageDailyLogin );
	}
	
	if ( RPARAM::bFeatureCodex )
	{
		m_pButtonPageCodex = CreateTabButton( "RAN_STUDENTRECORD_OBJECT_TAP_3", ID2GAMEWORD("RAN_STUDENTRECORD_WINDOW_PAGES", 3 ), STUDENT_RECORD_WINDOW_BUTTON_PAGE_CODEX ); // Codex	m_pPageCodex = new CCodexPage;
		m_pPageCodex = new CCodexPage;
		m_pPageCodex->CreateSub( this, "RAN_STUDENTRECORD_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, STUDENT_RECORD_WINDOW_PAGE_CODEX );
		m_pPageCodex->CreateSubControl ();
		m_pPageCodex->SetVisibleSingle( FALSE );
		RegisterControl ( m_pPageCodex );
	}
	/*12-9-14, Battle Pass - CNDev*/
	if ( RPARAM::bFeatureBattlePass )
	{		
		m_pButtonPageBattlepass = CreateTabButton( "RAN_STUDENTRECORD_OBJECT_TAP_4", ID2GAMEWORD("RAN_STUDENTRECORD_WINDOW_PAGES", 4 ), STUDENT_RECORD_WINDOW_BUTTON_PAGE_BATTLEPASS );
		
		m_pPageBattlePass = new CBattlePassPage;
		m_pPageBattlePass->CreateSub( this, "RAN_STUDENTRECORD_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, STUDENT_RECORD_WINDOW_PAGE_BATTLEPASS );
		m_pPageBattlePass->CreateSubControl ();
		m_pPageBattlePass->SetVisibleSingle( FALSE );
		RegisterControl ( m_pPageBattlePass );
	}
}

CMultiModeButton* CStudentRecordWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
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

void CStudentRecordWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CStudentRecordWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
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

	case STUDENT_RECORD_WINDOW_BUTTON_PAGE_PLAY_STATS:
	case STUDENT_RECORD_WINDOW_BUTTON_PAGE_ACTIVITY:
	case STUDENT_RECORD_WINDOW_BUTTON_PAGE_DAILYLOGIN:
	case STUDENT_RECORD_WINDOW_BUTTON_PAGE_CODEX:
	case STUDENT_RECORD_WINDOW_BUTTON_PAGE_BATTLEPASS:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				int nSelected = ControlID - STUDENT_RECORD_WINDOW_BUTTON_PAGE_PLAY_STATS;
				OpenPage( nSelected );
			}
		}break;
	};
}

void CStudentRecordWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage( PAGE_PLAY_STATS );
	}
}

void CStudentRecordWindow::OpenPage( int nPage )
{
	if ( nPage == m_nPage )	return;

	/* play stats window, Juver, 2021/01/26 */
	if ( m_pButtonPagePlayStats )	
		m_pButtonPagePlayStats->DoImageFlip( FALSE );

	/* play stats window, Juver, 2021/01/26 */
	if ( m_pPagePlayStats )	
		m_pPagePlayStats->SetVisibleSingle( FALSE );

	/*activity system, Juver, 2017/10/30 */
	if ( RPARAM::bActivity )
	{
		if ( m_pButtonPageActivity )	
			m_pButtonPageActivity->DoImageFlip( FALSE );

		if ( m_pPageActivity )	
			m_pPageActivity->SetVisibleSingle( FALSE );
	}

	/*12-16-24, daily login - CNDev*/
	if ( RPARAM::bFeatureDailyLogin )
	{
		if ( m_pButtonPageDailyLogin )	
			m_pButtonPageDailyLogin->DoImageFlip( FALSE );
	
		if ( m_pPageDailyLogin )	
			m_pPageDailyLogin->SetVisibleSingle( FALSE );
	}
	
	if ( RPARAM::bFeatureCodex )
	{
		if ( m_pButtonPageCodex )	m_pButtonPageCodex->DoImageFlip( FALSE );
		if ( m_pPageCodex )	m_pPageCodex->SetVisibleSingle( FALSE );
	}
	
	/*12-9-14, Battle Pass - CNDev*/
	if ( RPARAM::bFeatureBattlePass )
	{
		if ( m_pButtonPageBattlepass )	
			m_pButtonPageBattlepass->DoImageFlip( FALSE );
		if ( m_pPageBattlePass )	
			m_pPageBattlePass->SetVisibleSingle( FALSE );
	}

	m_nPage = nPage;

	switch( nPage )
	{
		/* play stats window, Juver, 2021/01/26 */
	case PAGE_PLAY_STATS:
		{
			if ( m_pButtonPagePlayStats )	
				m_pButtonPagePlayStats->DoImageFlip( TRUE );

			if ( m_pPagePlayStats )	
				m_pPagePlayStats->SetVisibleSingle( TRUE );
		}break;

		/*activity system, Juver, 2017/10/30 */
	case PAGE_ACTIVITY:
		{
			if ( RPARAM::bActivity )
			{
				if ( m_pButtonPageActivity )	
					m_pButtonPageActivity->DoImageFlip( TRUE );

				if ( m_pPageActivity )
					m_pPageActivity->SetVisibleSingle( TRUE );
			}
		}break;
	case PAGE_DAILYLOGIN:
		{
			if ( RPARAM::bFeatureDailyLogin )
			{
				if ( m_pButtonPageDailyLogin )	
					m_pButtonPageDailyLogin->DoImageFlip( TRUE );
	
				if ( m_pPageDailyLogin )	
					m_pPageDailyLogin->SetVisibleSingle( TRUE );
			}
		}break;
		
	case PAGE_CODEX:
		{
			if ( RPARAM::bFeatureCodex )
			{
				if ( m_pButtonPageCodex )	m_pButtonPageCodex->DoImageFlip( TRUE );
				if ( m_pPageCodex )	m_pPageCodex->SetVisibleSingle( TRUE );
			}
		}break;
	
	case PAGE_BATTLEPASS:
		{
			if ( RPARAM::bFeatureBattlePass )
			{
				if ( m_pButtonPageBattlepass )	
					m_pButtonPageBattlepass->DoImageFlip( TRUE );
	
				if ( m_pPageBattlePass )	
					m_pPageBattlePass->SetVisibleSingle( TRUE );
			}
		}break;
	
	};
}