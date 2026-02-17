#include "StdAfx.h"
#include "./PersonalLockPinInput.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./InnerInterface.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./UIEditBoxMan.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT	CPersonalLockPinInput::nLIMIT_PIN	= 4;

CPersonalLockPinInput::CPersonalLockPinInput(void)
	: m_emLock(EMPERSONAL_LOCK_SIZE)
	, m_pInfoDesc(NULL)
	, m_pInfoPin(NULL)
	, m_pEditBoxMan(NULL)
	, m_pButtonEnter(NULL)
	, m_pButtonClose(NULL)
{
}

CPersonalLockPinInput::~CPersonalLockPinInput(void)
{
}

void CPersonalLockPinInput::CreateSubControl()
{
	CD3DFontPar* pFont08 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont09 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_INPUT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_INPUT_INFO_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_INPUT_INPUT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PERSONAL_LOCK_PIN_INPUT_LINE_PIN" );
	RegisterControl ( pBasicLineBox );

	m_pInfoDesc = new CBasicTextBox;
	m_pInfoDesc->CreateSub ( this, "PERSONAL_LOCK_PIN_INPUT_INFO_TEXT", UI_FLAG_DEFAULT );
	m_pInfoDesc->SetFont ( pFont09 );
	m_pInfoDesc->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pInfoDesc->SetText( ID2GAMEWORD( "PERSONAL_LOCK_PIN_INPUT", 4 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pInfoDesc );

	m_pInfoPin = new CBasicTextBox;
	m_pInfoPin->CreateSub ( this, "PERSONAL_LOCK_PIN_INPUT_INFO_PIN", UI_FLAG_DEFAULT );
	m_pInfoPin->SetFont ( pFont09 );
	m_pInfoPin->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pInfoPin->SetText( ID2GAMEWORD( "PERSONAL_LOCK_PIN_INPUT", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pInfoPin );

	m_pEditBoxMan = new CUIEditBoxMan;
	m_pEditBoxMan->CreateSub ( this, "PERSONAL_LOCK_PIN_INPUT_EDITMAN", UI_FLAG_DEFAULT, PERSONAL_LOCK_PIN_INPUT_EDIT_MAN );
	m_pEditBoxMan->CreateEditBox ( PERSONAL_LOCK_PIN_INPUT_EDIT_PIN, "PERSONAL_LOCK_PIN_INPUT_TEXT_PIN", "PERSONAL_LOCK_PIN_INPUT_CARRAT", TRUE, UINT_MAX, pFont09, nLIMIT_PIN );
	m_pEditBoxMan->SetHide ( PERSONAL_LOCK_PIN_INPUT_EDIT_PIN, TRUE );
	RegisterControl ( m_pEditBoxMan );

	m_pButtonEnter = new CBasicTextButton;
	m_pButtonEnter->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_INPUT_BUTTON_ENTER );
	m_pButtonEnter->CreateBaseButton ( "PERSONAL_LOCK_PIN_INPUT_BUTTON_ENTER", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_INPUT", 2 ) );
	RegisterControl ( m_pButtonEnter );

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, PERSONAL_LOCK_PIN_INPUT_BUTTON_CANCEL );
	m_pButtonClose->CreateBaseButton ( "PERSONAL_LOCK_PIN_INPUT_BUTTON_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PERSONAL_LOCK_PIN_INPUT", 3 ) );
	RegisterControl ( m_pButtonClose );
}

void CPersonalLockPinInput::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
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

	case PERSONAL_LOCK_PIN_INPUT_BUTTON_CANCEL:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}break;

	case PERSONAL_LOCK_PIN_INPUT_BUTTON_ENTER:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CheckInfo();
			}
		}break;
	};
}

void CPersonalLockPinInput::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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

void CPersonalLockPinInput::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		if ( m_pEditBoxMan )
		{
			m_pEditBoxMan->EndEdit ();
			m_pEditBoxMan->ClearEdit ( PERSONAL_LOCK_PIN_INPUT_EDIT_PIN );

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

void CPersonalLockPinInput::CheckInfo()
{
	if ( !m_pEditBoxMan )	return;

	std::string strpin = m_pEditBoxMan->GetEditString( PERSONAL_LOCK_PIN_INPUT_EDIT_PIN );

	if ( m_emLock < 0 || m_emLock >= EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_INVALID_LOCK_TYPE") );
		return;
	}

	if ( strpin.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_EMPTY") );
		return;
	}

	if ( strpin.size() < nLIMIT_PIN || strpin.size() > nLIMIT_PIN  )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_SIZE") );
		return;
	}

	if ( strpin.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_NUMBER_ONLY") );
		return;
	}

	GLGaeaClient::GetInstance().GetCharacter()->PersonalLockInputPin( m_emLock, strpin.c_str() );
	CInnerInterface::GetInstance().HideGroup( GetWndID() );
}

void CPersonalLockPinInput::SetLockType( EMPERSONAL_LOCK emLock )
{
	m_emLock = emLock;

	if ( m_emLock < EMPERSONAL_LOCK_SIZE )
	{
		CString strTitle;
		strTitle.Format( "%s %s", ID2GAMEWORD( "PERSONAL_LOCK_INFO", m_emLock ), ID2GAMEWORD( "PERSONAL_LOCK_PIN_INPUT", 0 ) );
		SetTitleName( strTitle.GetString() ); 

		CString strDesc;
		strDesc.Format( "%s_%02d", "PERSONAL_LOCK_PIN_INPUT_DESC", m_emLock );

		if ( m_pInfoDesc )
		{
			m_pInfoDesc->ClearText();
			m_pInfoDesc->SetText( ID2GAMEINTEXT( strDesc.GetString() ), NS_UITEXTCOLOR::WHITE );
		}
	}

}
