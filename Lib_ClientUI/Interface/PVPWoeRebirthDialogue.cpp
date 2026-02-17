#include "stdafx.h"
#include "PVPWoeRebirthDialogue.h"
#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "BasicProgressBar.h"
#include "InnerInterface.h"

#include "../Lib_Client/G-Logic/GLPVPWoeData.h"
#include "../Lib_Client/G-Logic/GLPVPWoeClient.h"
#include "BasicLineBox.h"
#include "../Lib_Engine/DxCommon/D3DFont.h"
#include "GameTextControl.h"
#include "UITextControl.h"

#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPVPWoeRebirthDialogue::CPVPWoeRebirthDialogue ()
	: m_pTextDesc(NULL)
	, m_pTimer(NULL)
{
	timeStart = CTime::GetCurrentTime().GetTime();
}

CPVPWoeRebirthDialogue::~CPVPWoeRebirthDialogue ()
{
}

void CPVPWoeRebirthDialogue::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "WOE_FORCE_REBIRTH_WATCH_ICON" );
	RegisterControl ( pControl );

	m_pTimer = new CBasicProgressBar;
	m_pTimer->CreateSub ( this, "WOE_FORCE_REBIRTH_BLANK_LINE" );
	m_pTimer->CreateOverImage ( "WOE_FORCE_REBIRTH_GAUGE_LINE" );
	RegisterControl ( m_pTimer );

	m_pTextDesc = new CBasicTextBox;
	m_pTextDesc->CreateSub ( this, "WOE_FORCE_REBIRTH_TEXT" );
	m_pTextDesc->SetFont ( pFont );
	m_pTextDesc->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y  );
	RegisterControl ( m_pTextDesc );
}

void CPVPWoeRebirthDialogue::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	float fseconds = (float)difftime( CTime::GetCurrentTime().GetTime(), timeStart );

	if ( fseconds >= WOE_REVIVE_TIME )
	{
		GLPVPWoeClient::GetInstance().DoRevive();
		timeStart = CTime::GetCurrentTime().GetTime();
	}

	float fremain = WOE_REVIVE_TIME - fseconds;
	float fpercentage = fremain / WOE_REVIVE_TIME;

	if ( m_pTimer )
		m_pTimer->SetPercent( fpercentage );

	if ( m_pTextDesc )
	{
		CString strtime = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("WOE_REBIRTH_TIMER"), (int)fremain );
		
		m_pTextDesc->ClearText();
		m_pTextDesc->AddText( strtime, NS_UITEXTCOLOR::WHITE );
	}
}
