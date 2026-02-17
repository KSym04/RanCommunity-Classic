#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLAutoPilotC.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicProgressBar.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"

#include "./AutoPilotWindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoPilotWindow::CAutoPilotWindow(void)
	: m_pScanRangeStatic(NULL)
	, m_pScanRangeProgressBar(NULL)
	, m_pScanRangeDummyControl(NULL)
	, m_pScanRangeTextBox(NULL)
	, m_pApplyButton(NULL)
	, m_pCloseButton(NULL)
	, m_pToggleButton(NULL)
	, m_fRange(100.0f)
	, m_bActive(FALSE)
{
}

CAutoPilotWindow::~CAutoPilotWindow(void)
{
}

void CAutoPilotWindow::CreateSubControl()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite( "AUTO_PILOT_WINDOW_REGION" );
	RegisterControl ( pBasicLineBox );

	CUIControl* pUIControl = new CUIControl;
	pUIControl->CreateSub ( this, "AUTO_PILOT_WINDOW_REGION_TEXTURE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
	pUIControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pUIControl );
	pUIControl->AlignToControl( pBasicLineBox, TRUE );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "AUTO_PILOT_WINDOW_SETTING" );
	RegisterControl ( pBasicLineBox );


	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOption ( "AUTO_PILOT_SCAN_RANGE_LINE" );
	RegisterControl ( pBasicLineBox );

	m_pScanRangeStatic = new CBasicTextBox;
	m_pScanRangeStatic->CreateSub ( this, "AUTO_PILOT_SCAN_RANGE_STATIC" );
	m_pScanRangeStatic->SetFont ( pFont );
	m_pScanRangeStatic->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pScanRangeStatic->SetOneLineText ( ID2GAMEWORD ( "AUTO_PILOT_WINDOW", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pScanRangeStatic );

	m_pScanRangeProgressBar = new CBasicProgressBar;
	m_pScanRangeProgressBar->CreateSub ( this, "AUTO_PILOT_SCAN_RANGE_PROGRESS" );
	m_pScanRangeProgressBar->CreateOverImage ( "AUTO_PILOT_SCAN_RANGE_PROGRESS_OVERIMAGE" );
	RegisterControl ( m_pScanRangeProgressBar );

	m_pScanRangeDummyControl = new CUIControl;
	m_pScanRangeDummyControl->CreateSub ( this, "AUTO_PILOT_SCAN_RANGE_PROGRESS_DUMMY", UI_FLAG_DEFAULT, AUTO_PILOT_SCAN_RANGE_PROGRESS );
	RegisterControl ( m_pScanRangeDummyControl );

	m_pScanRangeTextBox = new CBasicTextBox;
	m_pScanRangeTextBox->CreateSub ( this, "AUTO_PILOT_SCAN_RANGE_VALUE" );
	m_pScanRangeTextBox->SetFont ( pFont );
	m_pScanRangeTextBox->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pScanRangeTextBox->SetOneLineText ( "xxx", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pScanRangeTextBox );

	m_pApplyButton = new CBasicTextButton;
	m_pApplyButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, AUTO_PILOT_APPLY_BUTTON );
	m_pApplyButton->CreateBaseButton ( "AUTO_PILOT_BUTTON_APPLY", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "AUTO_PILOT_WINDOW", 3 ) );
	RegisterControl ( m_pApplyButton );

	m_pCloseButton = new CBasicTextButton;
	m_pCloseButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, AUTO_PILOT_CLOSE_BUTTON );
	m_pCloseButton->CreateBaseButton ( "AUTO_PILOT_BUTTON_CLOSE", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "AUTO_PILOT_WINDOW", 2 ) );
	RegisterControl ( m_pCloseButton );

	m_pToggleButton = new CBasicTextButton;
	m_pToggleButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, AUTO_PILOT_TOGGLE_BUTTON );
	m_pToggleButton->CreateBaseButton ( "AUTO_PILOT_BUTTON_TOGGLE", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "AUTO_PILOT_WINDOW", 4 ) );
	RegisterControl ( m_pToggleButton );
}

void CAutoPilotWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( bFirstControl )
	{
		if ( m_pScanRangeProgressBar && m_pScanRangeProgressBar->IsExclusiveSelfControl() )
		{
			const float fSize = m_pScanRangeProgressBar->GetGlobalPos ().sizeX;
			const float fPos = float(x) - m_pScanRangeProgressBar->GetGlobalPos ().left;
			const float fPercent = fPos / fSize;
			m_pScanRangeProgressBar->SetPercent ( fPercent );
		}
	}

	CString strText;
	if ( m_pScanRangeTextBox )
	{
		m_pScanRangeTextBox->ClearText();
		strText.Format( "%g %%", m_fRange );
		m_pScanRangeTextBox->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	GLAutoPilotC* pAutoPilot = GLGaeaClient::GetInstance().GetCharacter()->m_pAutoPilot;
	if ( pAutoPilot )
	{
		m_bActive = pAutoPilot->m_bActive;
	}

	if ( m_pToggleButton )
	{
		if ( m_bActive )
			m_pToggleButton->SetOneLineText( ID2GAMEWORD ( "AUTO_PILOT_WINDOW", 5 ));
		else
			m_pToggleButton->SetOneLineText( ID2GAMEWORD ( "AUTO_PILOT_WINDOW", 4 ));
	}
	
}

void CAutoPilotWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case AUTO_PILOT_SCAN_RANGE_PROGRESS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_DOWN_LIKE ( dwMsg ) )
				{			
					m_pScanRangeProgressBar->SetExclusiveControl();
				}
				else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_pScanRangeProgressBar->ResetExclusiveControl();
				}
			}
			else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
			{
				m_pScanRangeProgressBar->ResetExclusiveControl();
			}

			m_fRange = floor( RPARAM::fAutoPilotRangeMax * m_pScanRangeProgressBar->GetPercent() );
		}
		break;

	case AUTO_PILOT_APPLY_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//just update range
				GLGaeaClient::GetInstance().GetCharacter()->m_pAutoPilot->Set( m_bActive, m_fRange );
			}
		}
		break;

	case AUTO_PILOT_TOGGLE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//this one updates the range and also toggle to enable/disable
				GLGaeaClient::GetInstance().GetCharacter()->m_pAutoPilot->Set( !m_bActive, m_fRange );
			}
		}
		break;

	case AUTO_PILOT_CLOSE_BUTTON:
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}
		break;

	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID () );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}

void CAutoPilotWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadCurrentOption();
	}
}

void CAutoPilotWindow::LoadCurrentOption ()
{
	GLAutoPilotC* pAutoPilot = GLGaeaClient::GetInstance().GetCharacter()->m_pAutoPilot;
	if ( pAutoPilot == NULL )
		return;

	m_bActive = pAutoPilot->m_bActive;
	m_fRange = pAutoPilot->m_fRange;

	if ( m_pScanRangeProgressBar )
	{
		m_pScanRangeProgressBar->SetPercent ( m_fRange / RPARAM::fAutoPilotRangeMax );
	}
}
