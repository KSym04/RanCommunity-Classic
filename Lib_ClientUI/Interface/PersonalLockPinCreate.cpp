#include "StdAfx.h"
#include "./PersonalLockPinCreate.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./UIEditBoxMan.h"

#include "./InnerInterface.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT	CPersonalLockPinCreate::nLIMIT_PIN		= 4;

CPersonalLockPinCreate::CPersonalLockPinCreate(void)
	: m_emLock(EMPERSONAL_LOCK_SIZE)
	, m_pInfoDesc(NULL)
	, m_pInfoPin(NULL)
	, m_pInfoPin2(NULL)
	, m_pEditBoxMan(NULL)
	, m_pButtonCreate(NULL)
	, m_pButtonClose(NULL)
{
}

CPersonalLockPinCreate::~CPersonalLockPinCreate(void)
{
}

void CPersonalLockPinCreate::CreateSubControl()
{
	CD3DFontPar* pFont08 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont09 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CREATE_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CREATE_INFO_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CREATE_INPUT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CREATE_LINE_PIN" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_CREATE_LINE_PIN2" );
	RegisterControl ( pBasicLineBox );

	m_pInfoDesc = new CBasicTextBox;
	m_pInfoDesc->CreateSub ( this, "PERSONAL_LOCK_PIN_CREATE_INFO_TEXT", UI_FLAG_DEFAULT );
	m_pInfoDesc->SetFont ( pFont09 );
	m_pInfoDesc->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pInfoDesc->SetText( ID2GAMEWORD( "PERSONAL_LOCK_PIN_CREATE", 5 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pInfoDesc );

	m_pInfoPin = new CBasicTextBox;
	m_pInfoPin->CreateSub ( this, "PERSONAL_LOCK_PIN_CREATE_INFO_PIN", UI_FLAG_DEFAULT );
	m_pInfoPin->SetFont ( pFont09 );
	m_pInfoPin->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pInfoPin->SetText( ID2GAMEWORD( "PERSONAL_LOCK_PIN_CREATE", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pInfoPin );

	m_pInfoPin2 = new CBasicTextBox;
	m_pInfoPin2->CreateSub ( this, "PERSONAL_LOCK_PIN_CREATE_INFO_PIN2", UI_FLAG_DEFAULT );
	m_pInfoPin2->SetFont ( pFont09 );
	m_pInfoPin2->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pInfoPin2->SetText( ID2GAMEWORD( "PERSONAL_LOCK_PIN_CREATE", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pInfoPin2 );

	m_pEditBoxMan = new CUIEditBoxMan;
	m_pEditBoxMan->CreateSub ( this, "PERSONAL_LOCK_PIN_CREATE_EDITMAN", UI_FLAG_DEFAULT, PERSONAL_LOCK_PIN_CREATE_EDIT_MAN );
	m_pEditBoxMan->CreateEditBox ( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN, "PERSONAL_LOCK_PIN_CREATE_TEXT_PIN", "PERSONAL_LOCK_PIN_CREATE_CARRAT", TRUE, UINT_MAX, pFont09, nLIMIT_PIN );
	m_pEditBoxMan->CreateEditBox ( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN2, "PERSONAL_LOCK_PIN_CREATE_TEXT_PIN2", "PERSONAL_LOCK_PIN_CREATE_CARRAT", TRUE, UINT_MAX, pFont09, nLIMIT_PIN );
	m_pEditBoxMan->SetHide ( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN, TRUE );
	m_pEditBoxMan->SetHide ( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN2, TRUE );
	RegisterControl ( m_pEditBoxMan );

	m_pButtonCreate = new CBasicTextButton;
	m_pButtonCreate->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_CREATE_BUTTON_CREATE );
	m_pButtonCreate->CreateBaseButton ( "PERSONAL_LOCK_PIN_CREATE_BUTTON_CREATE", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_CREATE", 3 ) );
	RegisterControl ( m_pButtonCreate );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_CREATE_BUTTON_CANCEL );
	m_pButtonClose->CreateBaseButton ( "PERSONAL_LOCK_PIN_CREATE_BUTTON_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_CREATE", 4 ) );
	RegisterControl ( m_pButtonClose );
}

void CPersonalLockPinCreate::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;

	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_CREATE_BUTTON_CANCEL:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_CREATE_BUTTON_CREATE:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CheckInfo();
			}
		}break;
	};
}

void CPersonalLockPinCreate::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

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

void CPersonalLockPinCreate::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{	
		if ( m_pEditBoxMan )
		{
			m_pEditBoxMan->EndEdit ();
			m_pEditBoxMan->ClearEdit ( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN );
			m_pEditBoxMan->ClearEdit ( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN2 );

			m_pEditBoxMan->Init();
			m_pEditBoxMan->BeginEdit();

			if( m_pEditBoxMan->IsMODE_NATIVE() )
			{
				m_pEditBoxMan->DoMODE_TOGGLE();
			}
		}
	}
	else
	{
		if ( m_pEditBoxMan )	m_pEditBoxMan->EndEdit ();
	}
}

void CPersonalLockPinCreate::CheckInfo()
{
	if ( !m_pEditBoxMan )	return;

	std::string strpin1 = m_pEditBoxMan->GetEditString( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN );
	std::string strpin2 = m_pEditBoxMan->GetEditString( PERSONAL_LOCK_PIN_CREATE_EDIT_PIN2 );

	if ( m_emLock < 0 || m_emLock >= EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_INVALID_LOCK_TYPE") );
		return;
	}

	if ( strpin1.empty() || strpin2.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_EMPTY") );
		return;
	}

	if ( strpin1.size() < nLIMIT_PIN || strpin1.size() > nLIMIT_PIN || strpin2.size() < nLIMIT_PIN || strpin2.size() > nLIMIT_PIN )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_SIZE") );
		return;
	}

	if ( strcmp( strpin1.c_str(), strpin2.c_str() ) != 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_MISMATCH") );
		return;
	}

	if ( strpin1.find_first_not_of( "0123456789" ) != std::string::npos || strpin2.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_NUMBER_ONLY") );
		return;
	}

	GLGaeaClient::GetInstance().GetCharacter()->PersonalLockCreatePin( m_emLock, strpin1.c_str(), strpin2.c_str() );

	CInnerInterface::GetInstance().HideGroup( GetWndID() );
}

void CPersonalLockPinCreate::SetLockType( EMPERSONAL_LOCK emLock )
{
	m_emLock = emLock;

	if ( m_emLock < EMPERSONAL_LOCK_SIZE )
	{
		CString strTitle;
		strTitle.Format( "%s %s", ID2GAMEWORD( "PERSONAL_LOCK_INFO", m_emLock ), ID2GAMEWORD( "PERSONAL_LOCK_PIN_CREATE", 0 ) );
		SetTitleName( strTitle.GetString() ); 

		CString strDesc;
		strDesc.Format( "%s_%02d", "PERSONAL_LOCK_CREATE_PIN_DESC", m_emLock );

		if ( m_pInfoDesc )
		{
			m_pInfoDesc->ClearText();
			m_pInfoDesc->SetText( ID2GAMEINTEXT( strDesc.GetString() ), NS_UITEXTCOLOR::WHITE );
		}
	}
	
}
