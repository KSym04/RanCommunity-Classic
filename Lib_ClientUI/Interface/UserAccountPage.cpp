#include "StdAfx.h"

#include "./UserAccountPage.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./OuterInterface.h"
#include "./ModalWindow.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Client/DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUserAccountPage::CUserAccountPage ()
	: m_pButtonBack(NULL)
	, m_pTextStatus(NULL)

	/* login device check, Juver, 2020/11/05 */
	, m_pStaticLoginDeviceCheck(NULL)
	, m_pTextLoginDeviceCheck(NULL)
	, m_pButtonLoginDeviceCheck(NULL)
{
}

CUserAccountPage::~CUserAccountPage ()
{
}

void CUserAccountPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextStatus = new CBasicTextBox;
	m_pTextStatus->CreateSub ( this, "USER_ACCOUNT_PAGE_TEXT_STATUS" );
	m_pTextStatus->SetFont ( pFont9 );
	m_pTextStatus->SetTextAlign ( TEXT_ALIGN_CENTER_X );	
	m_pTextStatus->SetOneLineText( "", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextStatus );

	m_pButtonBack = new CBasicTextButton;
	m_pButtonBack->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, ACCOUNT_PAGE_BUTTON_BACK );
	m_pButtonBack->CreateBaseButton ( "USER_ACCOUNT_PAGE_BUTTON_BACK", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "USER_ACCOUNT_PAGE", 1 ) );
	RegisterControl ( m_pButtonBack );

	{
		/* login device check, Juver, 2020/11/05 */
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxOuter ( "USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_LINEBOX" );
		RegisterControl ( pBasicLineBox );

		m_pStaticLoginDeviceCheck = new CBasicTextBox;
		m_pStaticLoginDeviceCheck->CreateSub ( this, "USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_STATIC" );
		m_pStaticLoginDeviceCheck->SetFont ( pFont9 );
		m_pStaticLoginDeviceCheck->SetTextAlign ( TEXT_ALIGN_LEFT );	
		m_pStaticLoginDeviceCheck->AddText( ID2GAMEWORD ( "USER_ACCOUNT_PAGE", 2 ), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pStaticLoginDeviceCheck );

		m_pTextLoginDeviceCheck = new CBasicTextBox;
		m_pTextLoginDeviceCheck->CreateSub ( this, "USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_STATUS" );
		m_pTextLoginDeviceCheck->SetFont ( pFont9 );
		m_pTextLoginDeviceCheck->SetTextAlign ( TEXT_ALIGN_CENTER_X );	
		m_pTextLoginDeviceCheck->SetOneLineText( ID2GAMEWORD ( "COMMON_ENABLED_DISABLED", 0 ), NS_UITEXTCOLOR::RED );
		RegisterControl ( m_pTextLoginDeviceCheck );

		m_pButtonLoginDeviceCheck = new CBasicTextButton;
		m_pButtonLoginDeviceCheck->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, ACCOUNT_PAGE_BUTTON_DEVICE_CHECK );
		m_pButtonLoginDeviceCheck->CreateBaseButton ( "USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_BUTTON", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, "" );
		m_pButtonLoginDeviceCheck->SetOneLineText( ID2GAMEWORD ( "COMMON_ENABLE_DISABLE", 1 ) );
		RegisterControl ( m_pButtonLoginDeviceCheck );
	}
}	

void CUserAccountPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CUserAccountPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case ACCOUNT_PAGE_BUTTON_BACK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				COuterInterface::GetInstance().ToSelectCharacterPage ( GetWndID () );
			}
		}break;

		/* login device check, Juver, 2020/11/05 */
	case ACCOUNT_PAGE_BUTTON_DEVICE_CHECK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				bool bUserLoginDeviceCheck = COuterInterface::GetInstance().GetUserLoginDeviceCheck();
				if ( bUserLoginDeviceCheck )
					DoModalOuter ( ID2GAMEEXTEXT ("USER_LOGIN_DEVICE_CHECK_SET_QUESTION_DISABLE"), MODAL_INPUT, EDITBOX, OUTER_MODAL_USER_LOGIN_DEVICE_CHECK_SET, TRUE );
				else
					DoModalOuter ( ID2GAMEEXTEXT ("USER_LOGIN_DEVICE_CHECK_SET_QUESTION_ENABLE"), MODAL_INPUT, EDITBOX, OUTER_MODAL_USER_LOGIN_DEVICE_CHECK_SET, TRUE );
			}
		}break;
	};
}

void CUserAccountPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if ( bVisible )
	{
		InfoShow();

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();	
		if ( pNetClient )
		{
			int nServerGroup, nServerChannel;
			COuterInterface::GetInstance().GetConnectServerInfo( nServerGroup, nServerChannel );
			pNetClient->UserAccountPageInit( TRUE, nServerChannel );
		}
	}
	else
	{
		InfoReset();

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();	
		if ( pNetClient )
		{
			int nServerGroup, nServerChannel;
			COuterInterface::GetInstance().GetConnectServerInfo( nServerGroup, nServerChannel );
			pNetClient->UserAccountPageInit( FALSE, nServerChannel );
		}
	}
}

void CUserAccountPage::InfoShow()
{
	/* login device check, Juver, 2020/11/05 */
	bool bUserLoginDeviceCheck = COuterInterface::GetInstance().GetUserLoginDeviceCheck();

	if ( m_pTextLoginDeviceCheck )
		m_pTextLoginDeviceCheck->SetOneLineText( ID2GAMEWORD( "COMMON_ENABLED_DISABLED", bUserLoginDeviceCheck ), bUserLoginDeviceCheck? NS_UITEXTCOLOR::GREENYELLOW:NS_UITEXTCOLOR::RED );

	if ( m_pButtonLoginDeviceCheck )
		m_pButtonLoginDeviceCheck->SetOneLineText( ID2GAMEWORD ( "COMMON_ENABLE_DISABLE", !bUserLoginDeviceCheck ) );
}

void CUserAccountPage::InfoReset()
{
	if ( m_pTextStatus )
		m_pTextStatus->ClearText();
}

void CUserAccountPage::AddErrorString( std::string strError, DWORD dwColor )
{
	if ( strError.size() == 0 )	return;

	if ( m_pTextStatus )
	{
		m_pTextStatus->ClearText();
		m_pTextStatus->SetText( strError.c_str(), dwColor );
	}
}