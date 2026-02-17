#include "StdAfx.h"
#include "./DailyLoginPage.h"

#include "./DailyLoginList.h"
#include "./DailyLoginCalendar.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextBoxEx.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicButton.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLDailyLogin.h"

#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-16-24, daily login - CNDev*/

CDailyLoginPage::CDailyLoginPage ()	
	: m_pTextTitle(NULL)
	, m_pTextDescription(NULL)
	, m_pDailyLoginList(NULL)
	, m_pDailyLoginCalendar(NULL)
	, m_fUpdateTime(0.0f)
{
}

CDailyLoginPage::~CDailyLoginPage ()
{
}

void CDailyLoginPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_STUDENTRECORD_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhite ( "RAN_STUDENTRECORD_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_DAILYLOGIN_LINEBOX_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_DAILYLOGIN_LINEBOX_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_DAILYLOGIN_LINEBOX_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_DAILYLOGIN_LINEBOX_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_DAILYLOGIN_LINEBOX_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_DAILYLOGIN_LINEBOX_DESCRIPTION" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_DAILYLOGIN_LINEBOX_RESET" );
	RegisterControl ( pLineBox );

	m_pTextTitle = new CBasicTextBox;
	m_pTextTitle->CreateSub ( this, "RAN_DAILYLOGIN_TITLE_STATIC" );
	m_pTextTitle->SetFont ( pFont9 );
	m_pTextTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTitle->SetText( ID2GAMEWORD( "RAN_DAILYLOGIN_WINDOW_TEXT", 0 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextTitle );

	m_pTextDescription = new CBasicTextBox;
	m_pTextDescription->CreateSub ( this, "RAN_DAILYLOGIN_DESCRIPTION_STATIC" );
	m_pTextDescription->SetFont ( pFont9 );
	m_pTextDescription->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pTextDescription );

	m_pTextTimer = new CBasicTextBox;
	m_pTextTimer->CreateSub ( this, "RAN_DAILYLOGIN_TIMER_VALUE" );
	m_pTextTimer->SetFont ( pFont9 );
	m_pTextTimer->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextTimer );

	m_pDailyLoginList = new CDailyLoginList;
	m_pDailyLoginList->CreateSub( this, "RAN_DAILYLOGIN_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE, DAILYLOGIN_LIST );
	m_pDailyLoginList->CreateSubControl ();
	RegisterControl ( m_pDailyLoginList );

	m_pDailyLoginCalendar = new CDailyLoginCalendar;
	m_pDailyLoginCalendar->CreateSub( this, "RAN_DAILYLOGIN_CALENDAR", UI_FLAG_XSIZE | UI_FLAG_YSIZE, DAILYLOGIN_CALENDAR );
	m_pDailyLoginCalendar->CreateSubControl ();
	RegisterControl ( m_pDailyLoginCalendar );
}

void CDailyLoginPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(ControlID, dwMsg);

}

void CDailyLoginPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f )
	{
		LoadData();
		m_fUpdateTime = 0.0f;
	}
}

void CDailyLoginPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		LoadData();
	}
}

void CDailyLoginPage::LoadData()
{
	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pcharacter) return;

	//daily login list
	if ( m_pDailyLoginList )	m_pDailyLoginList->ResetData();

	m_pDailyLoginList->LoadData();

	CString strHourText = ID2GAMEWORD("RAN_DAILYLOGIN_TIP_DURATION_TEXT", 1);
	CString strMinuteText = ID2GAMEWORD("RAN_DAILYLOGIN_TIP_DURATION_TEXT", 2);
	CString strSecondText = ID2GAMEWORD("RAN_DAILYLOGIN_TIP_DURATION_TEXT", 3);

	//tip
	if ( m_pTextDescription )
	{
		m_pTextDescription->ClearText();

		float fTime = RPARAM::fDailyLoginTimeMax;

		CString strCombine;
		strCombine.Format("%02d %s : %02d %s : %02d %s", 
			(DWORD)fTime / 3600, strHourText, 
			((DWORD)fTime % 3600) / 60, strMinuteText,
			(DWORD)fTime % 60, strSecondText);

		CString strText;
		strText.Format(ID2GAMEWORD("RAN_DAILYLOGIN_TIP_MESSAGE_TEXT", 0), strCombine);

		m_pTextDescription->AddText(strText.GetString(), NS_UITEXTCOLOR::WHITE);
		m_pTextDescription->AddText((char*)ID2GAMEWORD("RAN_DAILYLOGIN_TIP_MESSAGE_TEXT", 1), NS_UITEXTCOLOR::WHITE);
		m_pTextDescription->AddText((char*)ID2GAMEWORD("RAN_DAILYLOGIN_TIP_MESSAGE_TEXT", 2), NS_UITEXTCOLOR::WHITE);
		m_pTextDescription->AddText((char*)ID2GAMEWORD("RAN_DAILYLOGIN_TIP_MESSAGE_TEXT", 3), NS_UITEXTCOLOR::WHITE);
	}

	// Calendar
	if (m_pDailyLoginCalendar)
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter(); 
		if (pCharacter)
		{
				UINT nStreakCounter = pCharacter->m_dwDayStreakCounter;
				m_pDailyLoginCalendar->SetDays(nStreakCounter);
        
		}
	}

	//timer
	if ( m_pTextTimer )
	{
		m_pTextTimer->ClearText();

		float fTime = pcharacter->m_fDailyLoginRemain;

		if ( fTime > 0.0f )
		{
			CString strCombine;
			strCombine.Format("%02d %s : %02d %s : %02d %s", 
				(DWORD)fTime / 3600, strHourText, 
				((DWORD)fTime % 3600) / 60, strMinuteText,
				(DWORD)fTime % 60, strSecondText);

			CString strText;
			strText.Format(ID2GAMEWORD("RAN_DAILYLOGIN_TIP_MESSAGE_TEXT", 4), strCombine);

			m_pTextTimer->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
		}
		else
		{
			CString strText;
			strText.Format(ID2GAMEWORD("RAN_DAILYLOGIN_TIP_MESSAGE_TEXT", 5));
			m_pTextTimer->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
		}
	}
}