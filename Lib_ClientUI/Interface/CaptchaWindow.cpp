#include "StdAfx.h"
#include "./CaptchaWindow.h"

#include "./BasicLineBox.h"
#include "./BasicProgressBar.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "./UIEditBox.h"
#include "./UIEditBoxMan.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/Common/DXInputString.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CCaptchaWindow::nLIMIT_CHAR = 8;
const char CCaptchaWindow::BLANK_SYMBOL = ' ';

CCaptchaWindow::CCaptchaWindow()
	: m_pCaptchaText(NULL)
	, m_pEditBox(NULL)
	, m_pTextDesc(NULL)
	, m_pTimer(NULL)
{
}

CCaptchaWindow::~CCaptchaWindow()
{
}

void CCaptchaWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WAITSERVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWaitServer ( "CAPTCHA_WINDOW_LINE_BOX" );
	RegisterControl ( pLineBox );	

	m_pCaptchaText = new CBasicTextBox;
	m_pCaptchaText->CreateSub ( this, "CAPTCHA_WINDOW_TEXTBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
    m_pCaptchaText->SetFont ( pFont9 );
	m_pCaptchaText->AddText ( "112233" );
	RegisterControl ( m_pCaptchaText );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxEditBox ( "CAPTCHA_WINDOW_EDIT_BACK" );
	RegisterControl ( pBasicLineBox );

	CUIEditBox* pEditBox = new CUIEditBox;
	pEditBox->CreateSub ( this, "CAPTCHA_WINDOW_EDIT", UI_FLAG_XSIZE | UI_FLAG_BOTTOM, CAPTCHA_EDIT );
	pEditBox->CreateCarrat ( "CAPTCHA_WINDOW_EDIT_CARRAT", TRUE, UINT_MAX );
	pEditBox->SetLimitInput ( nLIMIT_CHAR );
	pEditBox->SetFont ( pFont9 );	
	pEditBox->DoUSE_NUMBER(true);
	RegisterControl ( pEditBox );
	m_pEditBox = pEditBox;

	m_pTimer = new CBasicProgressBar;
	m_pTimer->CreateSub ( this, "CAPTCHA_WINDOW_BLANK_LINE" );
	m_pTimer->CreateOverImage ( "CAPTCHA_WINDOW_GAUGE_LINE" );
	RegisterControl ( m_pTimer );

	m_pTextDesc = new CBasicTextBox;
	m_pTextDesc->CreateSub ( this, "CAPTCHA_WINDOW_TIMER_TEXT" );
	m_pTextDesc->SetFont ( pFont9 );
	m_pTextDesc->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y  );
	RegisterControl ( m_pTextDesc );


	const int nBUTTONSIZE = CBasicTextButton::SIZE14;

	CBasicTextButton* pButton = new CBasicTextButton;
	pButton->CreateSub ( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, BUTTON_ENTER );
	pButton->CreateBaseButton ( "CAPTCHA_WINDOW_ENTER", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "CAPTCHA_WINDOW", 0 ) );
	pButton->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );
	RegisterControl ( pButton );

}

void CCaptchaWindow::ShowCaptcha ( DWORD dwCaptcha )
{
	CString strTemp;
	strTemp.Format( ID2GAMEINTEXT("CAPTCHA_INPUT_MESSAGE"), dwCaptcha);

	if (m_pCaptchaText)
	{
		m_pCaptchaText->ClearText();
		m_pCaptchaText->AddText(strTemp);
	}

	BeginEdit();
}

void CCaptchaWindow::BeginEdit()
{
	m_pEditBox->EndEdit ();
	m_pEditBox->ClearEdit ();
	m_pEditBox->BeginEdit ();
}

void CCaptchaWindow::EndEdit()
{
	m_pEditBox->EndEdit ();
	m_pEditBox->ClearEdit ();
}

void CCaptchaWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
	if ( pChar )
	{
		float fRemain = pChar->m_fCaptchaWaitTimer;
		float fPercentage = fRemain / RPARAM::fCaptchaWaitTimer;

		if ( m_pTimer )
			m_pTimer->SetPercent( fPercentage );

		if ( m_pTextDesc )
		{
			CString strtime = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("CAPTCHA_DISCONNECT_MESSAGE"), (int)fRemain );

			m_pTextDesc->ClearText();
			m_pTextDesc->AddText( strtime, NS_UITEXTCOLOR::WHITE );
		}
	}
	
}

void CCaptchaWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case BUTTON_ENTER:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( CHECK_KEYFOCUSED(dwMsg) &&
					!DXInputString::GetInstance().CheckEnterKeyDown() )	return;

				CString strTrim = m_pEditBox->GetEditString ();
				strTrim.Trim ( BLANK_SYMBOL );
				if ( !m_pEditBox->GetEditLength () || !strTrim.GetLength () ) return ;

 				m_pEditBox->EndEdit ();				

				DWORD dwCaptcha = (DWORD) atoi ( strTrim.GetString () );

				GLGaeaClient::GetInstance().GetCharacter()->CaptchaInput(dwCaptcha);
			}
		}
		break;

	case CAPTCHA_EDIT:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					if (m_pEditBox)
					{
						CString strName = m_pEditBox->GetEditString ();
						m_pEditBox->EndEdit ();
						m_pEditBox->ClearEdit ();
						m_pEditBox->SetEditString (strName);
						m_pEditBox->BeginEdit ();	
					}
							
				}
			}
			else
			{
				if ( UIMSG_LB_DOWN & dwMsg || UIMSG_LB_UP & dwMsg )
				{
					if (m_pEditBox)
						m_pEditBox->EndEdit ();
				}
			}
		}
		break;

	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//do not allow close
				return;
			}
		}
		break;
	}
}

void	CCaptchaWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindow::SetVisibleSingle ( bVisible );

	if ( !bVisible )
	{
		m_pEditBox->EndEdit ();
	}

}

// const CString& CCaptchaWindow::GetTITLE ()
// {
// 	static CString strTemp;
// 
// 	CString strTrim = m_pEditBox->GetEditString ();
// 	strTrim.Trim ( BLANK_SYMBOL );
// 	if ( !m_pEditBox->GetEditLength () || !strTrim.GetLength () ) return strTemp;
// 
// 	return m_pEditBox->GetEditString ();
// }