#include "StdAfx.h"

#include "./LoginDeviceCheckVerifyPage.h"

#include "./BasicTextBox.h"
#include "./BasicLineBox.h"
#include "./BasicTextButton.h"

#include "./UIEditBoxMan.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./OuterInterface.h"
#include "./ModalWindow.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"
#include "../../Lib_Engine/Common/StringUtils.h"

#include "../../Lib_Network/s_NetClient.h"

#include "../../Lib_Client/DxGlobalStage.h"

#include "../../Lib_Helper/HLibDataConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLoginDeviceCheckVerifyPage::CLoginDeviceCheckVerifyPage ()
	: m_pTextBoxDetail(NULL)
	, m_pTextBoxUser(NULL)
	, m_pTextBoxPass1(NULL)
	, m_pTextBoxPass2(NULL)
	, m_pTextBoxCaptcha(NULL)
	, m_pTextBoxAccount(NULL)

	, m_pEditBoxMan(NULL)

	, m_pTextBoxStatus(NULL)

	, m_pButtonPass1(NULL)
	, m_pButtonPass2(NULL)
	, m_pButtonCaptcha(NULL)
	, m_pButtonOK(NULL)
	, m_pButtonCancel(NULL)

	, m_strCaptchaText("")
	, m_strAccountText("")
	, m_bShowPass1(FALSE)
	, m_bShowPass2(FALSE)
	, m_bShowCaptcha(FALSE)
{
}

CLoginDeviceCheckVerifyPage::~CLoginDeviceCheckVerifyPage ()
{
}

void CLoginDeviceCheckVerifyPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	const int nBUTTONSIZE = CBasicTextButton::SIZE18;

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOuter ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_USER_BACK" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOuter ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_PASS_1_BACK" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOuter ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_PASS_2_BACK" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOuter ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_CAPTCHA_BACK" );
	RegisterControl ( pBasicLineBox );


	m_pTextBoxDetail = new CBasicTextBox;
	m_pTextBoxDetail->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_DETAIL" );
	m_pTextBoxDetail->SetFont ( pFont9 );
	m_pTextBoxDetail->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxDetail->AddText( ID2GAMEEXTEXT ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_DETAIL" ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxDetail );

	m_pTextBoxUser = new CBasicTextBox;
	m_pTextBoxUser->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_USER" );
	m_pTextBoxUser->SetFont ( pFont9 );
	m_pTextBoxUser->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxUser->AddText( ID2GAMEWORD ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxUser );

	m_pTextBoxPass1 = new CBasicTextBox;
	m_pTextBoxPass1->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_PASS_1" );
	m_pTextBoxPass1->SetFont ( pFont9 );
	m_pTextBoxPass1->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxPass1->AddText( ID2GAMEWORD ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxPass1 );

	m_pTextBoxPass2 = new CBasicTextBox;
	m_pTextBoxPass2->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_PASS_2" );
	m_pTextBoxPass2->SetFont ( pFont9 );
	m_pTextBoxPass2->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxPass2->AddText( ID2GAMEWORD ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE", 3 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxPass2 );

	m_pTextBoxCaptcha = new CBasicTextBox;
	m_pTextBoxCaptcha->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_CAPTCHA" );
	m_pTextBoxCaptcha->SetFont ( pFont9 );
	m_pTextBoxCaptcha->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxCaptcha->AddText( "captcha", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxCaptcha );

	m_pTextBoxAccount = new CBasicTextBox;
	m_pTextBoxAccount->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_USER" );
	m_pTextBoxAccount->SetFont ( pFont9 );
	m_pTextBoxAccount->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxAccount->AddText( "account", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxAccount );


	m_pEditBoxMan = new CUIEditBoxMan;
	m_pEditBoxMan->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDITMAN", UI_FLAG_DEFAULT, USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_MAN );
	m_pEditBoxMan->CreateEditBox ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1", "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, RPARAM::register_pass2_max );
	m_pEditBoxMan->CreateEditBox ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2", "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, RPARAM::register_pass2_max );
	m_pEditBoxMan->CreateEditBox ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA", "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, RPARAM::register_captcha_max );
	m_pEditBoxMan->SetHide ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1, TRUE );
	m_pEditBoxMan->SetHide ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2, TRUE );
	m_pEditBoxMan->SetHide ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA, TRUE );
	RegisterControl( m_pEditBoxMan );

	m_pTextBoxStatus = new CBasicTextBox;
	m_pTextBoxStatus->CreateSub ( this, "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_STATUS" );
	m_pTextBoxStatus->SetFont ( pFont9 );
	m_pTextBoxStatus->SetTextAlign ( TEXT_ALIGN_LEFT );	
	m_pTextBoxStatus->AddText( "status", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxStatus );

	m_pButtonPass1 = new CBasicTextButton;
	m_pButtonPass1->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_1 );
	m_pButtonPass1->CreateBaseButton ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_1", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "COMMON_SHOW_HIDE", 1 ) );
	RegisterControl ( m_pButtonPass1 );

	m_pButtonPass2 = new CBasicTextButton;
	m_pButtonPass2->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_2 );
	m_pButtonPass2->CreateBaseButton ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_2", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "COMMON_SHOW_HIDE", 1 ) );
	RegisterControl ( m_pButtonPass2 );

	m_pButtonCaptcha = new CBasicTextButton;
	m_pButtonCaptcha->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_CAPTCHA );
	m_pButtonCaptcha->CreateBaseButton ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_CAPTCHA", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "COMMON_SHOW_HIDE", 1 ) );
	RegisterControl ( m_pButtonCaptcha );

	m_pButtonOK = new CBasicTextButton;
	m_pButtonOK->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_OK );
	m_pButtonOK->CreateBaseButton ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_OK", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "COMMON_OK_CANCEL", 1 ) );
	m_pButtonOK->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );
	RegisterControl ( m_pButtonOK );

	m_pButtonCancel = new CBasicTextButton;
	m_pButtonCancel->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_CANCEL );
	m_pButtonCancel->CreateBaseButton ( "USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_CANCEL", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "COMMON_OK_CANCEL", 0 ) );
	m_pButtonCancel->SetShortcutKey ( DIK_ESCAPE );
	RegisterControl ( m_pButtonCancel );

	
}	

void CLoginDeviceCheckVerifyPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
	if ( !pNetClient->IsOnline() )
	{
		if ( !COuterInterface::GetInstance().IsVisibleGroup ( MODAL_WINDOW_OUTER ) )
		{
			if( !COuterInterface::GetInstance().IsLoginCancel() )
			{
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_1"), MODAL_INFOMATION, OK, OUTER_MODAL_RECONNECT );
			}
		}
	}

	if ( m_pEditBoxMan )
	{
		CUIControl* pParent = m_pEditBoxMan->GetTopParent ();
		if ( !pParent )	pParent = this;
		BOOL bFocus = ( pParent->IsFocusControl() );

		if ( bFocus )
		{
			if ( UIKeyCheck::GetInstance()->Check ( DIK_TAB, DXKEY_DOWN ) )
			{
				m_pEditBoxMan->GoNextTab ();
			}
		}
	}
}

void CLoginDeviceCheckVerifyPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_1:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bShowPass1 = !m_bShowPass1;
				CheckHideStatusPass1();
			}
		}
		break;

	case USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_PASS_2:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bShowPass2 = !m_bShowPass2;
				CheckHideStatusPass2();
			}
		}
		break;

	case USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_SHOW_CAPTCHA:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bShowCaptcha = !m_bShowCaptcha;
				CheckHideStatusCaptcha();
			}
		}
		break;

	case USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_CANCEL:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				VerifyEnd();
			}
		}
		break;

	case USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_BUTTON_OK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				VerifyStart();
			}
		}
		break;
	}
}

BOOL CLoginDeviceCheckVerifyPage::CheckString( CString strTemp )
{	
	strTemp = strTemp.Trim();
	if( STRUTIL::CheckString( strTemp ) )	return FALSE;
	return TRUE;
}

void CLoginDeviceCheckVerifyPage::ResetAll ()
{
	m_strCaptchaText = "";
	m_strAccountText = "";

	m_bShowPass1 = FALSE;
	m_bShowPass2 = FALSE;

	CheckHideStatusPass1();
	CheckHideStatusPass2();
	CheckHideStatusCaptcha();

	if ( m_pTextBoxAccount )
		m_pTextBoxAccount->ClearText();	

	if ( m_pEditBoxMan )
	{
		m_pEditBoxMan->EndEdit ();

		m_pEditBoxMan->ClearEdit ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1 );
		m_pEditBoxMan->ClearEdit ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2 );
		m_pEditBoxMan->ClearEdit ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA );

		m_pEditBoxMan->Init();
		m_pEditBoxMan->BeginEdit();

		if( m_pEditBoxMan->IsMODE_NATIVE() )
		{
			m_pEditBoxMan->DoMODE_TOGGLE();
		}
	}
	
	m_pTextBoxStatus->ClearText();
}

void CLoginDeviceCheckVerifyPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if ( bVisible )
	{
		ResetAll();

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();	
		if ( pNetClient )
		{
			int nServerGroup, nServerChannel;
			COuterInterface::GetInstance().GetConnectServerInfo( nServerGroup, nServerChannel );
			pNetClient->UserLoginDeviceCheckVerifyInit( TRUE, nServerChannel );
		}

		if ( m_pTextBoxCaptcha )
		{
			WORD wRandomNumber = (WORD)rand() % 32000 + 10001;
			CString strRandom;
			strRandom.Format( "%d", wRandomNumber );
			m_strCaptchaText = strRandom.GetString();

			m_pTextBoxCaptcha->ClearText();
			m_pTextBoxCaptcha->SetText( m_strCaptchaText.c_str(), NS_UITEXTCOLOR::ORNAGERED );
		}
	}
	else
	{
		if ( m_pEditBoxMan )	m_pEditBoxMan->EndEdit ();

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();	
		if ( pNetClient )
		{
			int nServerGroup, nServerChannel;
			COuterInterface::GetInstance().GetConnectServerInfo( nServerGroup, nServerChannel );
			pNetClient->UserLoginDeviceCheckVerifyInit( FALSE, nServerChannel );
		}
	}
}

void CLoginDeviceCheckVerifyPage::SetUser ( std::string strUser )
{
	m_strAccountText = strUser;

	if ( m_pTextBoxAccount )
	{
		m_pTextBoxAccount->ClearText();
		m_pTextBoxAccount->SetText( m_strAccountText.c_str(), NS_UITEXTCOLOR::WHITE );
	}	
}

void CLoginDeviceCheckVerifyPage::CheckHideStatusPass1()
{
	if ( m_pEditBoxMan )
	{
		m_pEditBoxMan->StartEDIT( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1 );
		m_pEditBoxMan->SetHide ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1, !m_bShowPass1 );
	}

	if ( m_pButtonPass1 )
	{
		if ( m_bShowPass1 )
			m_pButtonPass1->SetOneLineText( ID2GAMEWORD ( "COMMON_SHOW_HIDE", 0 ) );
		else
			m_pButtonPass1->SetOneLineText( ID2GAMEWORD ( "COMMON_SHOW_HIDE", 1 ) );
	}	
}

void CLoginDeviceCheckVerifyPage::CheckHideStatusPass2()
{
	if ( m_pEditBoxMan )
	{
		m_pEditBoxMan->StartEDIT( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2 );
		m_pEditBoxMan->SetHide ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2, !m_bShowPass2 );
	}

	if ( m_pButtonPass2 )
	{
		if ( m_bShowPass2 )
			m_pButtonPass2->SetOneLineText( ID2GAMEWORD ( "COMMON_SHOW_HIDE", 0 ) );
		else
			m_pButtonPass2->SetOneLineText( ID2GAMEWORD ( "COMMON_SHOW_HIDE", 1 ) );
	}	
}

void CLoginDeviceCheckVerifyPage::CheckHideStatusCaptcha()
{
	if ( m_pEditBoxMan )
	{
		m_pEditBoxMan->StartEDIT( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA );
		m_pEditBoxMan->SetHide ( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA, !m_bShowCaptcha );
	}

	if ( m_pButtonCaptcha )
	{
		if ( m_bShowCaptcha )
			m_pButtonCaptcha->SetOneLineText( ID2GAMEWORD ( "COMMON_SHOW_HIDE", 0 ) );
		else
			m_pButtonCaptcha->SetOneLineText( ID2GAMEWORD ( "COMMON_SHOW_HIDE", 1 ) );
	}	
}

void CLoginDeviceCheckVerifyPage::VerifyStart()
{
	if ( !m_pEditBoxMan )		return;
	if ( !m_pTextBoxStatus )	return;
	if ( !m_pTextBoxAccount )	return;

	m_pTextBoxStatus->ClearText();

	std::string strAccount = m_strAccountText;
	std::string strPass1 = m_pEditBoxMan->GetEditString( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_1 );
	std::string strPass2 = m_pEditBoxMan->GetEditString( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_PASS_2 );
	std::string strCaptcha = m_pEditBoxMan->GetEditString( USER_LOGIN_DEVICE_CHECK_VERIFY_PAGE_EDIT_CAPTCHA );


	if ( strAccount.empty() )
	{
		AddErrorString( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_INVALID_ACCOUNT" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strAccount.size() < RPARAM::register_user_min || strAccount.size() > RPARAM::register_user_max )
	{
		CString text = _HLIB::cstringformat( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_LEN_USERID" ), RPARAM::register_user_min, RPARAM::register_user_max );
		AddErrorString( text.GetString(), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strPass1.empty() )
	{
		AddErrorString( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_PASS1_EMPTY" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strPass2.empty() )
	{
		AddErrorString( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_PASS2_EMPTY" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strPass1.size() < RPARAM::register_pass2_min || strPass1.size() > RPARAM::register_pass2_max )
	{
		CString text = _HLIB::cstringformat( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_LEN_PASS" ), RPARAM::register_pass2_min, RPARAM::register_pass2_max );
		AddErrorString( text.GetString(), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strcmp( strPass1.c_str(), strPass2.c_str() ) != 0 )
	{
		AddErrorString( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_PASS12_MISMATCH" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strCaptcha.empty() )
	{
		AddErrorString( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_CAPTCHA_EMPTY" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strCaptcha.size() > RPARAM::register_captcha_max )
	{
		CString text = _HLIB::cstringformat( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_CAPTCHA_SIZE" ), RPARAM::register_captcha_max );
		AddErrorString( text.GetString(), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( strcmp( strCaptcha.c_str(), m_strCaptchaText.c_str() ) != 0 )
	{
		AddErrorString( ID2GAMEEXTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_CAPTCHA_MISMATCH" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( !CheckString ( m_strAccountText.c_str() ) )
	{
		AddErrorString( ID2GAMEINTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_INVALID_USERID" ), NS_UITEXTCOLOR::RED );
		return;
	}

	if ( !CheckString ( strPass1.c_str() ) )
	{
		AddErrorString( ID2GAMEINTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_INVALID_PASS" ), NS_UITEXTCOLOR::RED );
		return;
	}

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();	
	if ( pNetClient && pNetClient->IsOnline() )
	{
		std::transform ( strAccount.begin(), strAccount.end(), strAccount.begin(), tolower );
		std::transform ( strPass1.begin(), strPass1.end(), strPass1.begin(), tolower );

		int nServerGroup, nServerChannel;
		COuterInterface::GetInstance().GetConnectServerInfo( nServerGroup, nServerChannel );

		pNetClient->UserLoginDeviceCheckVerifyRequest( strAccount.c_str(), strPass1.c_str(), nServerChannel );
	}
	else
	{
		AddErrorString( ID2GAMEINTEXT( "EMUSER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB_ERROR_SERVER" ), NS_UITEXTCOLOR::RED );
		VerifyEnd();
	}
}

void CLoginDeviceCheckVerifyPage::VerifyEnd()
{
	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
	if( pNetClient->IsOnline() == true )
	{
		pNetClient->CloseConnect();
		COuterInterface::GetInstance().SetCancelToLogin();
	}

	COuterInterface::GetInstance().ToSelectServerPage ( GetWndID () );
}

void CLoginDeviceCheckVerifyPage::AddErrorString( std::string strError, DWORD dwColor )
{
	if ( strError.size() == 0 )	return;

	if ( m_pTextBoxStatus )
	{
		m_pTextBoxStatus->ClearText();
		m_pTextBoxStatus->SetText( strError.c_str(), dwColor );
	}
}
